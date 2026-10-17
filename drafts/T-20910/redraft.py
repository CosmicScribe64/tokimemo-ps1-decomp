#!/usr/bin/env python3
"""T-20910: tools/redraft.py for OPTION with the second type pass and the hand drafts of this ticket.

Runs tools/redraft.py (T-20800) unchanged except for two additions, both set up again in every worker
process (the workers run this file instead of tools/redraft.py):

  object view   OPTION drafts read main's BG slot 0 at D_801220D0 as one SpriteSlot24 (game.h, GsBG layout),
                as main src/main/80041000.c does: the m2c context declares `extern SpriteSlot24 D_801220D0;`,
                the fold turns the slot labels D_801220D4 .. D_801220F0 into D_801220D0 + offset, and the draft
                writes `((SpriteSlot24 *)&D_801220D0)->unk_XX` (redraft_rewrites.OBJ_VIEWS, as ETC's
                D_801230D0). The BG map tables D_80122120 / D_80122140 / D_80122540 are header types of
                include/ovl/OPTION.h and need nothing here.
  variant hand  the hand draft drafts/T-20910/OPTION/<func>/hand.c, scored as it is (like archived-raw), for
                the functions that have one. It competes with the other variants; the best score wins.

Run in Docker from the repo root:
    tools/docker.sh python3 drafts/T-20910/redraft.py --ticket T-20910 --unit OPTION --jobs 2 --timeout 200
Other options as tools/redraft.py. `--variants` defaults to the tools/redraft.py default plus `hand`.
"""
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
import redraft  # noqa: E402
import redraft_rewrites as rw  # noqa: E402

HERE = Path(__file__).resolve()
HAND = ROOT / "drafts" / "T-20910"

rw.OBJ_VIEWS.setdefault("OPTION", {})["D_801220D0"] = ("SpriteSlot24", 0x24)
redraft.VARIANTS["hand"] = {"source": "archived"}
DEFAULT = ",".join(redraft.DEFAULT_VARIANTS + ("hand",))

_applicable = redraft.applicable
_build_tasks = redraft.build_tasks


def hand_file(row):
    path = HAND / row["unit"] / row["func"] / "hand.c"
    return path if path.exists() else None


def applicable(row, variant):
    if variant == "hand":
        return hand_file(row) is not None
    return _applicable(row, variant)


def build_tasks(rows, variants, work, out, ticket, timeout, fixes, force):
    for r in rows:   # tools/redraft.py reads the archived body's path for every archived-source variant
        if not r.get("archived") and hand_file(r) is not None:
            r["archived"] = {"path": str(hand_file(r).parent.relative_to(ROOT)), "file": "hand.c"}
    tasks = _build_tasks(rows, variants, work, out, ticket, timeout, fixes, force)
    by_scope = {r["scope"]: r for r in rows}
    for t in tasks:
        t.cmd = [str(HERE) if c == str(Path(redraft.__file__).resolve()) else c for c in t.cmd]
        if "--variant" in t.cmd and t.cmd[t.cmd.index("--variant") + 1] == "hand":
            row = by_scope[t.cmd[t.cmd.index("--worker") + 1]]
            path = hand_file(row)
            if "--archived" in t.cmd:
                i = t.cmd.index("--archived")
                del t.cmd[i:i + 2]
            t.cmd += ["--archived", "%s|%s" % (path.parent.relative_to(ROOT), path.name)]
    return tasks


redraft.applicable = applicable
redraft.build_tasks = build_tasks

if __name__ == "__main__":
    if "--variants" not in sys.argv and not any(a in sys.argv for a in ("--worker", "--finish")):
        sys.argv += ["--variants", DEFAULT]
    sys.exit(redraft.main())
