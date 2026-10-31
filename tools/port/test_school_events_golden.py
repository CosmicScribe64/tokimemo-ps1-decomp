"""Tests for tools/port/school_events_golden.py with invented dumps."""
import struct
import unittest

from tools.port import school_events_golden as golden


def _state_bytes():
    data = bytearray(golden.GAME_STATE_SIZE)
    data[0x3E:0x42] = bytes([95, 4, 7, 4])
    struct.pack_into("<i", data, 0xF8, 4 | (7 << 4))
    struct.pack_into("<i", data, 0x1BC + 0x10, 4 | (7 << 4))
    data[0x69C:0x6A0] = bytes([0x32, 0, 0, 0])
    data[0x1108] = 0x56
    return bytes(data)


def _hit(pc, a0_memory=None):
    hit = {"pc": pc, "label": "x", "frame": 9,
           "regs": {"a0": 0x801FFE00, "a1": 0, "a2": 7, "a3": 0},
           "dumps": [{"addr": golden.GAME_STATE, "data": _state_bytes().hex()}]}
    if a0_memory is not None:
        hit["args"] = {"a0": a0_memory}
    return hit


class ProjectTests(unittest.TestCase):
    def test_decodes_fields_and_queue_index(self):
        out = golden.project({"hits": [_hit(golden.PUSH_BIRTHDAYS, "03000000")]}, "ab", "t")
        record = out["records"][0]
        self.assertEqual(record["queue_index"], 3)
        state = record["state"]
        self.assertEqual((state["month"], state["day"], state["weekday"]), (4, 7, 4))
        self.assertEqual(state["player_birthday"], 0x74)
        self.assertEqual(state["girls"][0]["birthday"], 0x74)
        self.assertEqual(state["day_events"][0], [0x32, 0, 0, 0])
        self.assertEqual(state["game_mode"], 0x56)
        self.assertEqual(record["args"]["a2"], 7)

    def test_refuses_runs_without_hits_or_dumps(self):
        with self.assertRaises(golden.GoldenError):
            golden.project({"hits": []}, "ab", "t")
        hit = _hit(golden.PUSH_BIRTHDAYS)
        hit["dumps"] = []
        with self.assertRaises(golden.GoldenError):
            golden.project({"hits": [hit]}, "ab", "t")

    def test_v2_projects_selector_and_pokes(self):
        hits = [_hit(golden.PUSH_BIRTHDAYS, "03000000"), _hit(0x80134808)]
        for hit, word in zip(hits, ("03000000", "00000000")):
            hit["dumps"].append({"addr": golden.TALK_GIRL_SELECTOR_PLUS_ONE, "data": word})
        run = {"hits": hits, "pokes": [{"frame": 5, "addr": 0x800E6448, "before": "00", "after": "42"}]}
        out = golden.project(run, "ab", "t")
        self.assertEqual(out["schema"], golden.SCHEMA)
        self.assertEqual([r["state"]["talk_girl_selector_plus_one"] for r in out["records"]], [3, 0])
        self.assertEqual(out["pokes"], [{"frame": 5, "addr": "0x800E6448", "before": "00", "after": "42"}])

    def test_v1_without_selector_and_refuses_partial_selector(self):
        out = golden.project({"hits": [_hit(golden.PUSH_BIRTHDAYS, "03000000")]}, "ab", "t")
        self.assertEqual(out["schema"], golden.SCHEMA_V1)
        self.assertNotIn("talk_girl_selector_plus_one", out["records"][0]["state"])
        hits = [_hit(golden.PUSH_BIRTHDAYS, "03000000"), _hit(0x80134808)]
        hits[0]["dumps"].append({"addr": golden.TALK_GIRL_SELECTOR_PLUS_ONE, "data": "00000000"})
        with self.assertRaises(golden.GoldenError):
            golden.project({"hits": hits}, "ab", "t")

    def test_refuses_short_dump(self):
        with self.assertRaises(golden.GoldenError):
            golden.decode_game_state(b"\0" * 16)


if __name__ == "__main__":
    unittest.main()
