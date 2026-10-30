#!/usr/bin/env python3
"""T-39026: human visual review queue bound to image sha256 (tokimemo.visual-review.v1).

The queue lives in a JSON file under .asset-cache next to the contact-sheet rows
it reviews. Rules:

- every item is bound to the sha256 of its image. A verdict names the hash it
  saw; a verdict for another hash is refused. When a new contact sheet brings a
  different hash for the same checkpoint, the old verdict is moved to
  ``invalidated`` and the item returns to ``pending``;
- states: pending, approved, rejected, needs-fix, needs-art (human verdicts)
  and resolved-objective (the only state an agent close can set);
- an item is objective only when its contact-sheet row carries linter findings
  (overflow, untranslated glyphs, missing art); a row without findings is
  subjective, and a row that declares ``kind: objective`` without findings is
  refused;
- human verdicts (``verdict``) and agent first-pass notes (``agent-note``) are
  stored in separate lists. An agent note never changes a subjective item. An
  agent may close an objective item, only as ``resolved-objective`` and never
  as ``approved``, and the item records ``closed_by: agent``; only a human
  verdict closes a subjective one (tone, font readability, art quality).
  ``status`` reports counts split by ``closed_by`` so agent closes are never
  counted as human approvals;
- the ``verdict`` command is for the human reviewer only. ``--reviewer`` is
  audit metadata, not authentication: nothing here can tell a human from an
  agent, so an agent must never call ``verdict``, and agent review is never
  human approval;
- ``enqueue`` invalidates every item whose image hash changed, whatever the
  cap, and refreshes its findings, defect id, kind, severity and image path
  from the new sheet. The cap only limits how many new or reopened items are
  surfaced per run (highest severity first); the rest are listed as deferred,
  are not published, and are surfaced first-come by severity on a later run;
- ``publish`` copies the surfaced pending images into a media directory that
  tools/port/progress/server.py serves unchanged (manifest version 1, files
  named by hash prefix), for example:
  ``docker run --rm -p 127.0.0.1:8765:8765 -v <media>:/media:ro -v tools/port/progress:/app:ro
  --entrypoint python3 tokimemo-recovery /app/server.py --host 0.0.0.0 --media-root /media``.

All paths must be inside a directory named .asset-cache. Nothing is committed
or published to a remote.
"""
from __future__ import annotations

import argparse
import datetime
import hashlib
import json
from pathlib import Path
import re
import shutil
import sys

SCHEMA = "tokimemo.visual-review.v1"
SHEET_SCHEMA = "tokimemo.contact-sheet.v1"
STATES = ("pending", "approved", "rejected", "needs-fix", "needs-art", "resolved-objective")
HUMAN_STATES = ("approved", "rejected", "needs-fix", "needs-art")
AGENT_CLOSE_STATE = "resolved-objective"
KINDS = ("objective", "subjective")
HASH_RE = re.compile(r"^[0-9a-f]{64}$")
ITEM_RE = re.compile(r"^[A-Za-z0-9][A-Za-z0-9._:-]{0,127}$")


class QueueError(ValueError):
    """Refused queue operation."""


class ItemReset(QueueError):
    """Refused because the image changed on disk; the item was reset and must be saved."""


def now() -> str:
    return datetime.datetime.now(datetime.timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")


def sha256_file(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def in_cache(path: Path) -> Path:
    path = path.absolute()
    if ".asset-cache" not in path.parts[:-1]:
        raise QueueError("review queue files stay under .asset-cache: " + str(path))
    return path


def empty_queue(queue_id: str) -> dict:
    return {"schema": SCHEMA, "queue_id": queue_id, "created_utc": now(), "items": [], "deferred": []}


def load(path: Path) -> dict:
    path = in_cache(path)
    if not path.exists():
        return empty_queue(path.stem)
    queue = json.loads(path.read_text())
    if not isinstance(queue, dict) or queue.get("schema") != SCHEMA or not isinstance(queue.get("items"), list):
        raise QueueError("not a " + SCHEMA + " queue")
    return queue


def save(path: Path, queue: dict) -> None:
    path = in_cache(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_name(path.name + ".tmp")
    temporary.write_text(json.dumps(queue, indent=1, sort_keys=True) + "\n")
    temporary.replace(path)


def _item(queue: dict, item_id: str) -> dict:
    for item in queue["items"]:
        if item["item_id"] == item_id:
            return item
    raise QueueError("no such item: " + item_id)


def _invalidate(item: dict, new_hash: str, reason: str) -> None:
    item["invalidated"].append({"image_sha256": item["image_sha256"], "state": item["state"],
                                "closed_by": item["closed_by"], "recorded_utc": now(), "reason": reason})
    item.update(image_sha256=new_hash, state="pending", closed_by=None)


def row_kind(row: dict) -> str:
    """Objective only when the row carries linter findings; an unbacked objective claim is refused."""
    findings = row.get("findings", [])
    if not isinstance(findings, list):
        raise QueueError(f"{row.get('key')}: findings must be a list")
    kind = row.get("kind") or ("objective" if findings else "subjective")
    if kind not in KINDS:
        raise QueueError(f"{row.get('key')}: kind must be objective or subjective")
    if kind == "objective" and not findings:
        raise QueueError(f"{row.get('key')}: kind objective needs linter findings on the row")
    return kind


def _row_fields(row: dict, image: Path, severity: int, kind: str) -> dict:
    return {"kind": kind, "severity": severity, "image_path": str(image.resolve()),
            "defect_id": row.get("defect_id"),
            "findings": [str(f.get("rule", "finding")) if isinstance(f, dict) else str(f) for f in row.get("findings", [])]}


def enqueue(queue: dict, sheet_path: Path, cap: int) -> dict:
    """Add or refresh items from a contact sheet.

    Every changed hash invalidates its item's verdict; ``cap`` limits only how many
    new or reopened items are surfaced in this run.
    """
    if cap < 1:
        raise QueueError("cap must be at least 1")
    sheet = json.loads(sheet_path.read_text())
    if sheet.get("schema") != SHEET_SCHEMA:
        raise QueueError("not a " + SHEET_SCHEMA + " manifest")
    candidates = []
    for row in sheet["rows"]:
        image = sheet_path.parent / row["image"]
        actual = sha256_file(image)
        if actual != row["image_sha256"]:
            raise QueueError(f"{row['key']}: contact sheet image changed after the manifest was written")
        severity = row.get("severity", 1)
        if type(severity) is not int or not 0 <= severity <= 3:
            raise QueueError(f"{row['key']}: severity 0..3 required")
        candidates.append((severity, row, image, row_kind(row)))
    candidates.sort(key=lambda entry: (-entry[0], entry[1]["key"]))
    previously_deferred = {entry["item_id"] for entry in queue.get("deferred", [])}
    changed, deferred, invalidated = [], [], []
    existing = {item["item_id"]: item for item in queue["items"]}
    for severity, row, image, kind in candidates:
        item_id = row["key"]
        item = existing.get(item_id)
        reason = "per-run cap reached"
        if item is not None and item["image_sha256"] != row["image_sha256"]:
            # Invalidation never waits for the cap: the old verdict names another image.
            _invalidate(item, row["image_sha256"], "contact sheet image hash changed")
            item.update(_row_fields(row, image, severity, kind))
            invalidated.append(item_id)
            reason = "per-run cap reached; old verdict invalidated"
        elif item is not None:
            if not (item_id in previously_deferred and item["state"] == "pending"):
                continue
            reason = "per-run cap reached; old verdict invalidated" if item["invalidated"] else reason
        if len(changed) >= cap:
            deferred.append({"item_id": item_id, "severity": severity, "reason": reason})
            continue
        if item is None:
            item = {"item_id": item_id, "checkpoint": row["key"], "image_sha256": row["image_sha256"],
                    "state": "pending", "closed_by": None,
                    "human_verdicts": [], "agent_notes": [], "invalidated": [], **_row_fields(row, image, severity, kind)}
            queue["items"].append(item)
        changed.append(item_id)
    # Deferred items this sheet does not mention stay deferred until a sheet surfaces them.
    seen = {entry[1]["key"] for entry in candidates}
    deferred += [entry for entry in queue.get("deferred", []) if entry["item_id"] not in seen
                 and entry["item_id"] in existing and existing[entry["item_id"]]["state"] == "pending"]
    queue["deferred"] = deferred
    return {"enqueued": changed, "deferred": deferred, "invalidated": invalidated}


def _current_hash(item: dict, seen: str) -> None:
    if not HASH_RE.fullmatch(seen or ""):
        raise QueueError("image sha256 must be 64 lowercase hex digits")
    if seen != item["image_sha256"]:
        raise QueueError("verdict is for another image hash; the item was re-rendered")
    path = Path(item["image_path"])
    if not path.is_file() or sha256_file(path) != item["image_sha256"]:
        _invalidate(item, sha256_file(path) if path.is_file() else item["image_sha256"],
                    "image file changed on disk")
        raise ItemReset("image file no longer matches the reviewed hash; item reset to pending")


def human_verdict(queue: dict, item_id: str, state: str, reviewer: str, seen: str, note: str = "") -> dict:
    """Record the human reviewer's verdict. Never call this from an agent (see the module docstring)."""
    if state not in HUMAN_STATES:
        raise QueueError("verdict state must be one of " + ", ".join(HUMAN_STATES))
    if not reviewer:
        raise QueueError("reviewer is required")
    item = _item(queue, item_id)
    _current_hash(item, seen)
    record = {"state": state, "reviewer": reviewer, "recorded_utc": now(), "image_sha256": seen, "note": note}
    item["human_verdicts"].append(record)
    item.update(state=state, closed_by="human")
    return record


def agent_note(queue: dict, item_id: str, agent: str, seen: str, note: str,
               proposed_state: str | None = None, close: bool = False) -> dict:
    if not agent:
        raise QueueError("agent name is required")
    if proposed_state is not None and proposed_state not in STATES:
        raise QueueError("unknown proposed state")
    item = _item(queue, item_id)
    _current_hash(item, seen)
    if close:
        if item["kind"] != "objective":
            raise QueueError("an agent cannot close a subjective item; only a human verdict can")
        if proposed_state != AGENT_CLOSE_STATE:
            raise QueueError("an agent close sets only " + AGENT_CLOSE_STATE + ", never a human verdict state")
        if not item["findings"]:
            raise QueueError("an agent can close only an item with linter findings")
    record = {"agent": agent, "recorded_utc": now(), "image_sha256": seen, "note": note,
              "proposed_state": proposed_state, "closes": close}
    item["agent_notes"].append(record)
    if close:
        item.update(state=proposed_state, closed_by="agent")
    return record


def publish(queue: dict, media_root: Path) -> list[dict]:
    """Copy surfaced pending item images (not deferred ones) into a server.py media directory (manifest version 1)."""
    media_root = in_cache(media_root)
    deferred = {entry["item_id"] for entry in queue.get("deferred", [])}
    if media_root.exists():
        raise QueueError("media directory must be new")
    media_root.mkdir(parents=True)
    items = []
    for item in sorted(queue["items"], key=lambda i: (-i["severity"], i["item_id"])):
        if item["state"] != "pending" or item["item_id"] in deferred:
            continue
        source = Path(item["image_path"])
        if sha256_file(source) != item["image_sha256"]:
            raise QueueError(item["item_id"] + ": image changed; re-run enqueue")
        identifier = item["image_sha256"][:16]
        shutil.copyfile(source, media_root / (identifier + ".png"))
        items.append({"id": identifier, "file": identifier + ".png", "sha256": item["image_sha256"],
                      "bytes": source.stat().st_size, "created": f"{item['severity']}-{item['item_id']}"})
    (media_root / "manifest.json").write_text(json.dumps({"version": 1, "items": items}, indent=1) + "\n")
    return items


def status(queue: dict) -> dict:
    """Counts per state, split by who closed the item, so agent closes never read as human approvals."""
    split = {who: {state: 0 for state in STATES[1:]} for who in ("human", "agent")}
    for item in queue["items"]:
        if item["state"] != "pending":
            split[item["closed_by"] or "human"][item["state"]] += 1
    deferred = {entry["item_id"] for entry in queue.get("deferred", [])}
    pending = [i for i in queue["items"] if i["state"] == "pending"]
    return {"pending": len(pending), "pending_deferred": sum(1 for i in pending if i["item_id"] in deferred),
            "closed_by_human": split["human"], "closed_by_agent": split["agent"],
            "human_approved": split["human"]["approved"]}


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--queue", type=Path, required=True)
    sub = parser.add_subparsers(dest="command", required=True)
    add = sub.add_parser("enqueue"); add.add_argument("--sheet", type=Path, required=True)
    add.add_argument("--cap", type=int, default=20)
    verdict = sub.add_parser("verdict", help="human reviewer only; --reviewer is audit metadata, not authentication")
    note = sub.add_parser("agent-note")
    for command in (verdict, note):
        command.add_argument("--item", required=True); command.add_argument("--image-sha256", required=True)
        command.add_argument("--note", default="")
    verdict.add_argument("--state", required=True); verdict.add_argument("--reviewer", required=True)
    note.add_argument("--agent", required=True); note.add_argument("--proposed-state")
    note.add_argument("--close", action="store_true")
    media = sub.add_parser("publish"); media.add_argument("--media-root", type=Path, required=True)
    sub.add_parser("status")
    args = parser.parse_args(argv)
    try:
        queue = load(args.queue)
        if args.command == "enqueue":
            result = enqueue(queue, args.sheet, args.cap)
        elif args.command == "verdict":
            result = human_verdict(queue, args.item, args.state, args.reviewer, args.image_sha256, args.note)
        elif args.command == "agent-note":
            result = agent_note(queue, args.item, args.agent, args.image_sha256, args.note, args.proposed_state, args.close)
        elif args.command == "publish":
            result = publish(queue, args.media_root)
        else:
            result = status(queue)
        if args.command != "status":
            save(args.queue, queue)
    except ItemReset as error:
        # The verdict is refused, but the reset item must be recorded.
        save(args.queue, queue)
        print("FAIL: " + str(error), file=sys.stderr)
        return 2
    except (QueueError, ValueError, OSError, KeyError) as error:
        print("FAIL: " + str(error), file=sys.stderr)
        return 2
    print(json.dumps(result, sort_keys=True))
    return 0


if __name__ == "__main__":
    sys.exit(main())
