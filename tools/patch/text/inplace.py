"""Write in-place text records into the built English executable and overlays (T-39507).

The main executable and the overlays are built from the decomp, so their text containers
(tools/patch/text/containers.py, ``growable=False``) are models of the original files. The
English build keeps the original addresses of every object it does not rebuild, so a table or
extent record that is not a C literal sits at the same file offset in the English binary.
``patch_built`` writes each such record through the container's ``Image.write`` (on the
original bytes) and copies the slot's bytes into the built binary, but only when the built
binary still holds the original bytes there; otherwise the slot moved in the English link and
the record is reported. C literals are compiled from build/en/src and are skipped here.
"""
from __future__ import annotations

from dataclasses import dataclass, field

from .containers import Container
from .image import TRAILER_MARKER, has_cd_trailer


@dataclass
class InPlaceResult:
    data: bytes
    written: list = field(default_factory=list)
    overflows: list = field(default_factory=list)
    moved_in_build: list = field(default_factory=list)


def patch_built(container: Container, payloads: dict[str, bytes], built: bytes) -> InPlaceResult:
    """Copy the English in-place records of ``container`` into ``built``."""
    image = container.image
    wanted = {rid: payload for rid, payload in payloads.items()
              if rid in image.slots and image.slots[rid].mode == "in_place"
              and container.records[rid].kind != "source_literal"}
    result = image.write(wanted)
    out = bytearray(built)
    report = InPlaceResult(b"")
    over = {o["id"]: o for o in result.overflows if o.get("id")}
    for rid in sorted(wanted, key=lambda r: image.slots[r].offset):
        slot = image.slots[rid]
        start, end = slot.offset, slot.offset + slot.capacity
        if rid in over:
            report.overflows.append({"id": rid, "needed": over[rid]["needed"], "capacity": slot.capacity})
            continue
        if bytes(out[start:end]) != image.data[start:end]:
            report.moved_in_build.append(rid)
            continue
        out[start:end] = result.data[start:end]
        report.written.append(rid)
    if has_cd_trailer(built):
        total = sum(out[:-4]) & 0xFFFF
        out[-4:] = total.to_bytes(2, "big") + TRAILER_MARKER
    report.data = bytes(out)
    return report
