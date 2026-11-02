#!/usr/bin/env python3
"""Unit tests for tools/emu/route.py on synthetic data (no game data, no emulator).

Run: tools/emu/run.sh shell -c "python3 tools/emu/test_route.py"
"""
import json
import os
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import route  # noqa: E402


def read(path):
    with open(path, encoding="utf-8") as f:
        return f.read()


def steps_of(text, build="jp"):
    return [{k: v for k, v in s.items() if k != "line"} for s in route.parse_route(text, build)]


class ParseTest(unittest.TestCase):
    def test_main_route_parses_for_every_build(self):
        text = read(route.DEFAULT_ROUTE)
        for build in route.BUILDS:
            steps = route.parse_route(text, build)
            names = [s["name"] for s in steps if s["op"] == "shot"]
            for want in ("title_menu", "options", "name_entry_page", "command_screen", "calendar",
                         "phone_book", "save_done", "after_date", "loaded"):
                self.assertIn(want, names)

    def test_wait_conditions_and_options(self):
        (s,) = steps_of("wait mode=0x14 stage!=1|2 0x800E7313=9 timeout=50 stable=3 mash=CIRCLE+START/20/6 "
                        "shots=talk/30/4 optional label=x")
        self.assertEqual(s["all"], [
            {"addr": 0x800E7388, "values": [0x14], "neg": False},
            {"addr": 0x800E7389, "values": [1, 2], "neg": True},
            {"addr": 0x800E7313, "values": [9], "neg": False}])
        self.assertEqual((s["timeout"], s["stable"], s["optional"], s["label"]), (50, 3, True, "x"))
        self.assertEqual(s["mash"], {"buttons": ["CIRCLE", "START"], "every": 20, "hold": 6})
        self.assertEqual((s["name"], s["shot_every"], s["max_shots"]), ("talk", 30, 4))

    def test_wait_defaults(self):
        (s,) = steps_of("wait mode=0x12")
        self.assertEqual((s["timeout"], s["stable"], s["min"], s["optional"]), (1800, 1, 0, False))
        self.assertNotIn("mash", s)

    def test_press_idle_shot_state(self):
        self.assertEqual(steps_of("press DOWN\nidle 30\nshot a_1\nstate s1"), [
            {"op": "press", "buttons": ["DOWN"], "hold": 4, "gap": 12},
            {"op": "idle", "frames": 30},
            {"op": "shot", "name": "a_1"},
            {"op": "save", "state": "s1"}])

    def test_pick_is_a_wait_on_the_menu_cursor(self):
        (s,) = steps_of("pick 13 menu=1")
        self.assertEqual(s["op"], "wait")
        self.assertEqual(s["all"], [{"addr": route.MENU_CURSOR + 1, "values": [13], "neg": False}])
        self.assertEqual(s["mash"]["buttons"], ["DOWN"])

    def test_talk(self):
        (s,) = steps_of("talk prologue mode!=0x15 max=5")
        self.assertEqual((s["op"], s["name"], s["max"], s["buttons"]), ("talk", "prologue", 5, ["CIRCLE"]))
        self.assertEqual(s["text_addr"], route.TEXT_STATE)

    def test_comments_and_blank_lines(self):
        self.assertEqual(steps_of("# heading\n\nidle 1  # trailing\n"), [{"op": "idle", "frames": 1}])

    def test_errors(self):
        for text in ("press FOO", "wait", "wait mode=0x100", "wait size=1", "shot Bad-Name", "jump x",
                     "shot a\nshot a", "goto nowhere", "label a\nlabel a", "call nothing", "steer x=1",
                     "idle ten"):
            with self.assertRaises(route.RouteError, msg=text):
                route.parse_route(text)
        with self.assertRaises(route.RouteError):
            route.parse_route("idle 1", "xx")

    def test_line_numbers(self):
        steps = route.parse_route("# c\nidle 1\n\nidle 2")
        self.assertEqual([s["line"] for s in steps], [2, 4])

    def test_loop_shot_names_may_repeat_at_runtime_but_not_in_the_text(self):
        route.parse_route("label a\nshot x\ngoto a max=2")


class NameEntryTest(unittest.TestCase):
    def test_hiragana_cells(self):
        self.assertEqual(route.hiragana_cell("a"), (0, 0))
        self.assertEqual(route.hiragana_cell("ka"), (6, 0))
        self.assertEqual(route.hiragana_cell("ro"), (16, 2))   # block 2, row 2, column 4
        self.assertEqual(route.hiragana_cell("pa"), (6, 5))
        with self.assertRaises(route.RouteError):
            route.hiragana_cell("-")

    def test_cell_center_and_cursor_limit(self):
        self.assertEqual(route.cell_center(0, 0), (-112, -40))
        x, _ = route.cell_center(16, 2)
        self.assertEqual(x, 0x8C - 3)  # last column: the cursor stops at 0x8C
        self.assertEqual((x + 0x78) // 16, 16)

    def test_build_name_entry_is_pluggable(self):
        jp = route.parse_route("call name_entry", "jp")
        en = route.parse_route("call name_entry", "en")
        self.assertEqual([s["op"] for s in jp][:3], ["wait", "shot", "steer"])
        self.assertEqual(sum(s["op"] == "steer" for s in jp), 8)   # 3 + 3 + 2 characters
        self.assertEqual(sum(s["op"] == "steer" for s in en), 11)  # Smith, Tom, Tom
        waits = [s for s in jp if s["op"] == "wait"]
        self.assertEqual([w["all"][2]["values"] for w in waits], [[1], [3], [5]])

    def test_call_arguments(self):
        steps = route.parse_route("call name_entry a ka a", "jp")
        self.assertEqual(sum(s["op"] == "steer" for s in steps), 3)
        steps = route.parse_route("call name_entry a_i ka a", "jp")  # _ is a space: two characters
        self.assertEqual(sum(s["op"] == "steer" for s in steps), 4)
        with self.assertRaises(route.RouteError):
            route.parse_route("call name_entry Q", "jp")


class JobTest(unittest.TestCase):
    TEXT = "idle 1\nlabel top\nstate s\nidle 2\ngoto top mode=0x46 max=3\nshot end"

    def test_goto_targets_count_from_one(self):
        steps = route.parse_route(self.TEXT)
        job = route.build_job(steps, "/out", 100)
        goto = job["steps"][4]
        self.assertEqual((goto["target"], goto["max"]), (2, 3))
        self.assertEqual(job["steps"][2]["path"], "/out/states/s.sstate")
        self.assertEqual(job["key_addrs"], [route.FIELDS[f] for f in route.KEY_FIELDS])

    def test_resume_index_and_goto_before_it(self):
        steps = route.parse_route(self.TEXT)
        first = route.resume_index(steps, "s")
        self.assertEqual(first, 3)
        with self.assertRaises(route.RouteError):
            route.build_job(steps[first:], "/out", 100)  # the label lies before the resume point
        with self.assertRaises(route.RouteError):
            route.resume_index(steps, "nope")

    def test_resume_point_copies_earlier_checkpoints(self):
        steps = route.parse_route("shot a\nstate s\nshot b")
        with tempfile.TemporaryDirectory() as src, tempfile.TemporaryDirectory() as out:
            for name in ("001_a.png", "002_b.png"):
                route.write_text(os.path.join(src, name), "x")
            route.write_text(os.path.join(src, "result.json"), json.dumps({"states": {"s": 50, "later": 90}, "checkpoints": [
                {"name": "a", "frame": 50, "step": 1, "file": "001_a.png"},
                {"name": "b", "frame": 50, "step": 3, "file": "002_b.png"}]}))
            state, offset, first, earlier, states = route.resume_point(src + ":s", steps, out)
            self.assertEqual((offset, first, states), (50, 2, {"s": 50}))
            self.assertEqual([c["name"] for c in earlier], ["a"])
            self.assertTrue(os.path.exists(os.path.join(out, "001_a.png")))
            self.assertFalse(os.path.exists(os.path.join(out, "002_b.png")))
            self.assertTrue(state.endswith("states/s.sstate"))


def write_raw(path, w, h, color):
    """A 16-bit BGR555 raw screenshot of one colour, as route.lua writes it."""
    r, g, b = color
    v = (r >> 3) | (g >> 3) << 5 | (b >> 3) << 10
    with open(path, "wb") as f:
        f.write(bytes([v & 0xFF, v >> 8]) * (w * h))


class SheetTest(unittest.TestCase):
    def test_png_round_trip_and_scale(self):
        with tempfile.TemporaryDirectory() as d:
            rows = [b"".join(bytes([x * 10, y * 10, 7]) for x in range(6)) for y in range(4)]
            route.png_write(os.path.join(d, "a.png"), 6, 4, rows)
            w, h, back = route.png_read(os.path.join(d, "a.png"))
            self.assertEqual((w, h, back), (6, 4, rows))
            small = route.scale_rows(6, 4, rows, 3, 2)
            self.assertEqual(small[1][3:6], bytes([20, 20, 7]))

    def test_result_sheet_and_compare(self):
        with tempfile.TemporaryDirectory() as d:
            runs = []
            for label, color, names in (("jp", (248, 0, 0), ["title", "talk_01", "talk_02"]),
                                        ("en", (0, 0, 248), ["title", "talk_01"])):
                out = os.path.join(d, "route-" + label)
                os.makedirs(out)
                shots = []
                for i, name in enumerate(names, 1):
                    raw = os.path.join(out, "shot_%03d.raw" % i)
                    write_raw(raw, 32, 24, color)
                    shots.append({"name": name, "frame": i * 10, "step": i, "key": [0x12, 2, 0, 0, 0],
                                  "raw": "/work/x/" + os.path.basename(raw), "width": 32, "height": 24, "bpp": 16})
                checkpoints = route.result_from_raw({"shots": shots}, out, offset=100, first=4)
                self.assertEqual([c["file"] for c in checkpoints][:2], ["001_title.png", "002_talk_01.png"])
                self.assertEqual((checkpoints[0]["frame"], checkpoints[0]["step"]), (110, 5))
                self.assertFalse(os.path.exists(os.path.join(out, "shot_001.raw")))
                _, _, rows = route.png_read(os.path.join(out, "001_title.png"))
                self.assertEqual(rows[0][:3], bytes(color))
                route.write_text(os.path.join(out, "result.json"), json.dumps(
                    {"label": label, "reason": "done", "frames": 30, "checkpoints": checkpoints}))
                route.write_sheet(out)
                page = read(os.path.join(out, "sheet.html"))
                self.assertIn("001_title.png", page)
                w, h, _ = route.png_read(os.path.join(out, "sheet.png"))
                self.assertEqual((w, h), (6 * route.CELL_W, route.CELL_H + route.LABEL_H))
                runs.append(out)
            html_path = route.compare(runs[0], runs[1], os.path.join(d, "cmp"))
            page = read(html_path)
            self.assertEqual(page.count("<tr><th>"), 4)        # heading, title, talk_01, talk_02
            self.assertIn("not reached", page)                  # en has no talk_02
            self.assertIn("../route-jp/001_title.png", page)
            w, h, _ = route.png_read(os.path.join(d, "cmp", "compare.png"))
            self.assertEqual((w, h), (2 * route.CELL_W, 3 * (route.CELL_H + route.LABEL_H)))

    def test_bad_screenshot_has_no_file(self):
        with tempfile.TemporaryDirectory() as out:
            route.write_text(os.path.join(out, "shot_001.raw"), "0123456789")
            (c,) = route.result_from_raw({"shots": [{"name": "x", "frame": 1, "raw": "shot_001.raw",
                                                      "width": 32, "height": 24, "bpp": 16}]}, out)
            self.assertIsNone(c["file"])
            self.assertIn("length", c["error"])


if __name__ == "__main__":
    unittest.main()
