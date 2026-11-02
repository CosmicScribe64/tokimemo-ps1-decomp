"""The English art to ship: one entry per artwork item of the inventory (T-39508).

Source: translations/en/graphics/translation_inventory.json. An item ships its registered English
images: the `candidates` of the item (the inventory step already keeps only the newest
non-superseded candidate per variant; the owner approved the candidates, 2026-11-01), or, for an
english-reviewed item without candidates, its approved historical asset under artwork/batches.
Items marked deliberately-untranslated or missing ship nothing and are reported as such.

Each image is resolved to its PNG under artwork/checkpoints/<checkpoint>/<revision>/ (its SHA-256
must equal the registered one) and to its manifest entry, which says what the image covers
(frame, palette variant, part, page, region or still crop).
"""
from __future__ import annotations

import hashlib
import json
from dataclasses import dataclass, field
from pathlib import Path

GRAPHICS = Path("translations/en/graphics")
INVENTORY = GRAPHICS / "translation_inventory.json"
ARTWORK = GRAPHICS / "artwork"
SHIPPED_STATUSES = ("candidate-only", "english-reviewed")
FALLBACK_LEDGER = ARTWORK / "checkpoints/T39410-menus-fixes-v1/fallback-ledger.json"


class ItemError(ValueError):
    """The registered art does not match the files in the repository."""


@dataclass
class Image:
    """One English PNG and what its manifest says about it."""
    id: str
    path: Path
    sha256: str
    revision: str
    meta: dict
    approved: bool = False


@dataclass
class Item:
    id: str
    status: str
    container: str
    scope: str
    frame: int | None
    images: list[Image] = field(default_factory=list)
    reason: str = ""


def _sha256(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def _entries(node, out: list) -> None:
    """Every dict of a manifest that names an output (id or hash)."""
    if isinstance(node, dict):
        if "id" in node or "output_sha256" in node or "sha256" in node:
            out.append(node)
        for value in node.values():
            _entries(value, out)
    elif isinstance(node, list):
        for value in node:
            _entries(value, out)


class Manifests:
    def __init__(self, root: Path):
        self.root = root
        self._cache: dict[Path, list] = {}

    def entry(self, png: Path, image_id: str, sha256: str) -> dict:
        manifest = png.parent / "manifest.json"
        while manifest.parent != self.root and not manifest.is_file():
            manifest = manifest.parent.parent / "manifest.json"
        if not manifest.is_file():
            return {}
        if manifest not in self._cache:
            out: list = []
            _entries(json.loads(manifest.read_text()), out)
            self._cache[manifest] = out
        entries = self._cache[manifest]
        by_id = [e for e in entries if e.get("id") == image_id and _names(e, sha256)]
        if by_id:
            return by_id[0]
        by_hash = [e for e in entries if e.get("output_sha256") == sha256]
        by_hash = by_hash or [e for e in entries if isinstance(e.get("crop"), dict) and e["crop"].get("sha256") == sha256]
        by_hash = by_hash or [e for e in entries if e.get("sha256") == sha256]
        return by_hash[0] if by_hash else {}


def _names(entry: dict, sha256: str) -> bool:
    crop = entry.get("crop")
    return sha256 in (entry.get("output_sha256"), entry.get("sha256"), crop.get("sha256") if isinstance(crop, dict) else None) \
        or "output_sha256" not in entry


def load(root: Path) -> list[Item]:
    """Every artwork item with its resolved English images."""
    inventory = json.loads((root / INVENTORY).read_text())
    files: dict[str, Path] = {}
    checkpoints = root / ARTWORK / "checkpoints"
    for path in sorted(checkpoints.rglob("*.png")):
        rel = path.relative_to(checkpoints)
        files.setdefault("/".join(rel.parts[1:]), path)
    historical = {h["id"]: h for h in inventory["historical_assets"]}
    manifests = Manifests(checkpoints)
    items = []
    for raw in inventory["artwork_items"]:
        item = Item(raw["id"], raw["status"], raw["container"], raw["scope"], raw.get("frame"))
        english = raw["english"]
        approved = set(english["approved_candidates"])
        if raw["status"] not in SHIPPED_STATUSES:
            item.reason = raw.get("reason", "")
        for candidate in english["candidates"]:
            path = files.get(candidate["path"])
            if path is None:
                raise ItemError("%s: %s is not in the checkpoints" % (item.id, candidate["path"]))
            if _sha256(path) != candidate["output_sha256"]:
                raise ItemError("%s: %s does not have the registered hash" % (item.id, candidate["path"]))
            meta = manifests.entry(path, candidate["id"], candidate["output_sha256"])
            item.images.append(Image(candidate["id"], path, candidate["output_sha256"], candidate["revision"],
                                     meta, candidate["id"] in approved))
        if not english["candidates"] and raw["status"] == "english-reviewed":
            for asset_id in english["historical_assets"]:
                asset = historical[asset_id]
                path = root / ARTWORK / asset["output"]
                if _sha256(path) != asset["output_sha256"]:
                    raise ItemError("%s: %s does not have the registered hash" % (item.id, asset["output"]))
                meta = {"frame": asset.get("frame"), "dimensions": asset.get("dimensions"),
                        "palette": asset.get("palette")}
                item.images.append(Image(asset_id, path, asset["output_sha256"], asset["batch"], meta, True))
        items.append(item)
    return items


def by_container(items: list[Item]) -> dict[str, list[Item]]:
    out: dict[str, list[Item]] = {}
    for item in items:
        if item.images:
            out.setdefault(item.container, []).append(item)
    return out


def fallbacks(root: Path) -> dict[str, Image]:
    """Same-size fallback images by candidate id (fallback-ledger.json, T-39411)."""
    ledger = root / FALLBACK_LEDGER
    folder = ledger.parent
    manifests = Manifests(folder)
    out = {}
    for asset in json.loads(ledger.read_text())["assets"]:
        path = folder / asset["path"]
        if _sha256(path) != asset["output_sha256"]:
            raise ItemError("fallback %s does not have the registered hash" % asset["path"])
        meta = manifests.entry(path, asset["id"], asset["output_sha256"])
        out[asset["id"]] = Image(asset["id"], path, asset["output_sha256"], asset["revision"], meta)
    return out
