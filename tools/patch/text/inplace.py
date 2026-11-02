"""Write in-place text records into the built English executable and overlays (T-39507).

The main executable and the overlays are built from the decomp, so their text containers
(tools/patch/text/containers.py, ``growable=False``) are models of the original files. The
English build relinks them freely (T-39505, wiki/relink.md): ``locate`` maps an original file
offset to its offset in the English binary (None when the bytes sit in a rebuilt object).
``patch_built`` writes each table or extent record through the container's ``Image.write`` (on
the original bytes) and copies the slot's bytes to the located place, but only when the built
binary still holds the original bytes there; otherwise the record is reported as moved in the
build. C literals are compiled from build/en/src and are skipped here. A record too long for its slot
goes to ``retarget`` (tools/patch/text/retarget.py, T-39506) when one is given: written elsewhere in the
binary, with its references re-pointed.
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
    retargeted: list = field(default_factory=list)


def patch_built(container: Container, payloads: dict[str, bytes], built: bytes,
                locate=lambda offset: offset, retarget=None) -> InPlaceResult:
    """Copy the English in-place records of ``container`` into ``built``; ``locate`` maps an original
    file offset to the built one (identity by default)."""
    image = container.image
    wanted = {rid: payload for rid, payload in payloads.items()
              if rid in image.slots and image.slots[rid].mode == "in_place"
              and container.records[rid].kind != "source_literal"}
    result = image.write(wanted)
    out = bytearray(built)
    report = InPlaceResult(b"")
    over = {o["id"]: o for o in result.overflows if o.get("id")}
    # T-39514: a zero tail can still belong to the final fixed-stride slot.
    # Reserve every modeled slot, including retained ones, before any overflow
    # allocation can alter its bytes and make the later identity check fail.
    if retarget is not None and retarget.pool is not None:
        for slot in image.slots.values():
            if slot.mode != "in_place":
                continue
            start = locate(slot.offset)
            if start is not None:
                retarget.pool.reserve(start, start + slot.capacity)
    stride_tables = _stride_tables(wanted, image, locate, retarget) if retarget is not None else set()
    for rid in sorted(wanted, key=lambda r: image.slots[r].offset):
        slot = image.slots[rid]
        start, end = slot.offset, slot.offset + slot.capacity
        new = locate(start)
        if new is None and retarget is not None and rid in over:
            new = retarget.locate_by_label(image.base + start)
        if rid in over and (new is None or bytes(out[new:new + end - start]) != image.data[start:end]):
            # the slot lies in an object the English build compiles again (its C literal is English)
            report.moved_in_build.append(rid)
            continue
        if rid in over:
            body = wanted[rid] + slot.terminator
            if retarget is not None and new is not None and _table_of(rid) not in stride_tables \
                    and bytes(out[new:new + end - start]) == image.data[start:end] \
                    and retarget.place(out, rid, new, slot.capacity, body):
                report.retargeted.append(rid)
                continue
            report.overflows.append({"id": rid, "needed": over[rid]["needed"], "capacity": slot.capacity})
            continue
        if new is None or bytes(out[new:new + end - start]) != image.data[start:end]:
            report.moved_in_build.append(rid)
            continue
        out[new:new + end - start] = result.data[start:end]
        report.written.append(rid)
    if has_cd_trailer(built):
        total = sum(out[:-4]) & 0xFFFF
        out[-4:] = total.to_bytes(2, "big") + TRAILER_MARKER
    report.data = bytes(out)
    return report


def _table_of(rid: str) -> str | None:
    """Table name of a typed table entry (``dictionary:<file>:<table>:<index>``), else None."""
    if rid.startswith(("dictionary:", "overlay-table:")):
        return rid.rsplit(":", 1)[0]
    return None


def _stride_tables(wanted: dict, image, locate, retarget) -> set:
    """Tables the code indexes by a fixed stride: some translated entry has no reference of its own.
    Moving any of their entries would move the table base the stride is added to (T-39506)."""
    tables: dict = {}
    for rid in wanted:
        table = _table_of(rid)
        if table is None:
            continue
        slot = image.slots[rid]
        new = locate(slot.offset)
        referenced = new is not None and retarget.referenced(new)
        tables[table] = tables.get(table, True) and referenced
    return {table for table, every_entry_referenced in tables.items() if not every_entry_referenced}
