"""Project an emulator breakpoint run into a school-events golden (birthdays).

Reads a tools/emu/emu.py `break` result whose hits carry a GameState dump
(--dump 800E6280:0x1A90) and writes one JSON record per hit with the fields
the port's birthday rules read, in the port's rule_state shape:
  year month day weekday player_flags player_birthday params[9] girls[16]
  calendar[2] day_events[32] day_event_index day_event_done_bits
  event_log_count current_girl game_mode mode_stage mode_step
plus the call arguments of the hit (a0..a3) and, for
day_event_queue_push_birthdays, the queue index read through a0.

When the hits also dump talk_girl_selector_plus_one (--dump 80122D40:4) the
golden is schema v2 and every record's state carries that word, which the
GEKO girl select reads under done bit 0x80 and clears. A run that dumps it on
some hits and not others is refused. Runs written with emu.py --poke list the
RAM writes under "pokes", so a golden from a constructed state says so.

Usage:
  python -m tools.port.school_events_golden RESULT.json OUT.json [--label TEXT]
The output must not exist. It holds game state derived from the original
program and belongs under ignored .asset-cache/; never commit it.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import struct
import sys
from pathlib import Path

SCHEMA_V1 = "tokimemo.school-events-golden.v1"
SCHEMA = "tokimemo.school-events-golden.v2"
GAME_STATE = 0x800E6280
GAME_STATE_SIZE = 0x1A90
PUSH_BIRTHDAYS = 0x8006D52C
TALK_GIRL_SELECTOR_PLUS_ONE = 0x80122D40  # config/symbol_addrs_main.txt


class GoldenError(ValueError):
    """The run does not hold what a golden needs."""


def _hex(data: str) -> bytes:
    return bytes.fromhex(data.replace(" ", "").replace("\n", ""))


def _address(value) -> int:
    return int(value, 16) if isinstance(value, str) else int(value)


def decode_game_state(data: bytes) -> dict:
    """GameState fields by the offsets of include/game/game_state.h."""
    if len(data) < GAME_STATE_SIZE:
        raise GoldenError("GameState dump is shorter than 0x1A90 bytes")
    u8 = lambda o: data[o]
    u16 = lambda o: struct.unpack_from("<H", data, o)[0]
    s32 = lambda o: struct.unpack_from("<i", data, o)[0]
    u32 = lambda o: struct.unpack_from("<I", data, o)[0]
    girls = []
    for index in range(16):
        base = 0x1BC + 0x38 * index
        girls.append({"tokimeki_word": s32(base), "affection_word": s32(base + 4),
                      "syoushin_word": s32(base + 8), "flags": u32(base + 0xC),
                      "birthday": s32(base + 0x10),
                      "counters": list(data[base + 0x14:base + 0x38])})
    calendar = []
    for month in range(2):
        base = 0x44 + 0x40 * month
        calendar.append({"day_kind": list(data[base:base + 32]),
                         "marker": list(data[base + 32:base + 64])})
    return {
        "year": u8(0x3E), "month": u8(0x3F), "day": u8(0x40), "weekday": u8(0x41),
        "day_count": u16(0x42), "calendar": calendar,
        "player_flags": u16(0xF4), "player_flags_upper": u16(0xF6),
        "player_birthday": s32(0xF8),
        "params": [s32(0xFC + 4 * i) for i in range(9)],
        "girls": girls,
        "day_events": [list(data[0x69C + 4 * i:0x6A0 + 4 * i]) for i in range(32)],
        "day_event_index": u8(0x71C), "day_event_done_bits": u8(0x71E),
        "event_log_count": u8(0xF5E), "current_girl": u8(0xF5F),
        "frame_count": u32(0x10F8),
        "game_mode": u8(0x1108), "mode_stage": u8(0x1109), "mode_step": u8(0x110A),
    }


def _game_state_dump(hit: dict) -> bytes:
    for dump in hit.get("dumps", []):
        if _address(dump["addr"]) == GAME_STATE:
            return _hex(dump["data"])
    raise GoldenError("hit %s has no GameState dump" % hit.get("name", "?"))


def _selector_word(hit: dict) -> int | None:
    for dump in hit.get("dumps", []):
        if _address(dump["addr"]) == TALK_GIRL_SELECTOR_PLUS_ONE:
            data = _hex(dump["data"])
            if len(data) < 4:
                raise GoldenError("talk_girl_selector_plus_one dump holds fewer than four bytes")
            return struct.unpack_from("<i", data, 0)[0]
    return None


def _arg(hit: dict, name: str) -> int:
    regs = hit.get("regs", {})
    if name not in regs:
        raise GoldenError("hit lacks register %s" % name)
    return _address(regs[name])


def _pointed_word(hit: dict, register: str) -> int | None:
    """The word the emulator read through a pointer argument at the hit."""
    memory = (hit.get("args") or {}).get(register)
    if not memory:
        return None
    data = _hex(memory)
    if len(data) < 4:
        raise GoldenError("pointer argument %s holds fewer than four bytes" % register)
    return struct.unpack_from("<i", data, 0)[0]


def project(result: dict, source_sha256: str, label: str) -> dict:
    hits = result.get("hits") or []
    if not hits:
        raise GoldenError("run recorded no breakpoint hits")
    records = []
    selectors = [_selector_word(hit) for hit in hits]
    with_selector = sum(word is not None for word in selectors)
    if with_selector not in (0, len(hits)):
        raise GoldenError("talk_girl_selector_plus_one is dumped on %d of %d hits" % (with_selector, len(hits)))
    for order, hit in enumerate(hits):
        pc = _address(hit["pc"])
        record = {"order": order, "pc": "0x%08X" % pc, "label": hit.get("label"),
                  "overlay": hit.get("overlay"), "frame": hit.get("frame"),
                  "args": {r: _arg(hit, r) for r in ("a0", "a1", "a2", "a3")},
                  "state": decode_game_state(_game_state_dump(hit))}
        if with_selector:
            record["state"]["talk_girl_selector_plus_one"] = selectors[order]
        if pc == PUSH_BIRTHDAYS:
            record["queue_index"] = _pointed_word(hit, "a0")
        records.append(record)
    pokes = [{"frame": int(p["frame"]), "addr": "0x%08X" % _address(p["addr"]),
              "before": p["before"], "after": p["after"]} for p in result.get("pokes") or []]
    return {"schema": SCHEMA if with_selector else SCHEMA_V1, "label": label,
            "source_result_sha256": source_sha256,
            "game_state_address": "0x%08X" % GAME_STATE, "pokes": pokes, "records": records}


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description="Project a breakpoint run into a school-events golden.")
    parser.add_argument("result", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("--label", default="")
    args = parser.parse_args(argv)
    if args.output.exists():
        print("output exists; goldens are immutable", file=sys.stderr)
        return 1
    raw = args.result.read_bytes()
    try:
        golden = project(json.loads(raw), hashlib.sha256(raw).hexdigest(), args.label)
    except GoldenError as error:
        print("school events golden: %s" % error, file=sys.stderr)
        return 1
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(golden, indent=1, sort_keys=True) + "\n", encoding="utf-8")
    print("wrote %s: %d records" % (args.output, len(golden["records"])))
    return 0


if __name__ == "__main__":
    sys.exit(main())
