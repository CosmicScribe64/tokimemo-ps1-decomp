"""Tests for the English art writers (T-39508): indexed sheets, repack, stills, report, executable.

Synthetic fixtures need no game data. The game-data tests run when the original files are in
disc/files/: every view the profiles compose must equal the asset readers' exports (the hashes in
translation_inventory.json), and placing the original images must change no byte.

  IMG=tokimemo-patch tools/docker.sh python3 -m unittest tools.test_patch_art -v
"""
import hashlib
import json
import struct
import tempfile
import unittest
from dataclasses import dataclass, field
from pathlib import Path

from PIL import Image as PILImage

from tools.assets.images import _png
from tools.patch.art import ArtBuildError
from tools.patch.art import build as art_build
from tools.patch.art import indexed, placer, sheets, sprites, stills
from tools.patch.art.indexed import TRANSPARENT, Part, Sheet, View, compose, plan, rgba
from tools.patch.art.items import Image, Item

REPOSITORY = Path(__file__).resolve().parents[1]
FILES = REPOSITORY / "disc" / "files"
INVENTORY = REPOSITORY / "translations/en/graphics/translation_inventory.json"

RED, GREEN, BLUE = 0x001F, 0x03E0, 0x7C00
PALETTE = tuple([0, RED, GREEN, RED | 0x8000, GREEN | 0x8000, BLUE] + [0] * 250)
RECORDS = 0x4000            # frame records after the 128x128 page in the synthetic container


def page(fill=None) -> bytearray:
    data = bytearray(RECORDS + 0x100)
    for (x, y), index in (fill or {}).items():
        data[y * 128 + x] = index
    return data


def part(x=0, y=0, w=4, h=2, u=0, v=0, record=None, flip=0):
    return Part(x, y, w, h, flip, 0, 8, u, v, PALETTE, record, 0, 0x10)


@dataclass
class FakeProfile:
    """The parts of sheets.Profile the placer uses."""
    container: str = "TEST.NBN"
    pages: dict = field(default_factory=lambda: {0: (0, 8)})
    align: int = 8
    encode: object = staticmethod(lambda page_index, u, v: (page_index << 8) | ((v // 8) << 4) | (u // 8))
    direct: bool = False
    pack: object = None
    page_sets: dict = field(default_factory=dict)
    exe: bytes = None

    def record_bytes(self, data):
        return data


def write_frame(data: bytearray, offset: int, parts: list) -> None:
    struct.pack_into("<4h", data, offset, len(parts), 0, 0, 0)
    for i, p in enumerate(parts):
        texture = (p.page_index << 8) | ((p.v // 8) << 4) | (p.u // 8)
        struct.pack_into("<2h3H2h", data, offset + 8 + i * 14, p.x, p.y, texture, p.clut, p.flip, p.width, p.height)


def read_frame(data, offset: int) -> list:
    count = struct.unpack_from("<h", data, offset)[0]
    out = []
    for i in range(count):
        x, y, texture, clut, flip, w, h = struct.unpack_from("<2h3H2h", data, offset + 8 + i * 14)
        out.append(Part(x, y, w, h, flip, 0, 8, (texture & 15) * 8, ((texture >> 4) & 15) * 8, PALETTE,
                        offset + 8 + i * 14, texture >> 8, clut))
    return out


class IndexedTest(unittest.TestCase):
    def test_identity_writes_nothing(self):
        data = page({(0, 0): 1, (1, 0): 2, (2, 1): 5})
        view = View.of("frame-000.base", [part()])
        result = plan(data, [(view, compose(data, view), view.width)])
        self.assertEqual(result.writes, {})
        self.assertEqual(result.errors, {})

    def test_changed_pixel_maps_to_palette_index(self):
        data = page({(0, 0): 1})
        view = View.of("frame-000.base", [part()])
        image = compose(data, view)
        image[0] = rgba(GREEN)
        result = plan(data, [(view, image, view.width)])
        self.assertEqual(set(result.writes.values()), {2})   # green without STP: the original texel had none

    def test_stp_follows_the_original_texel(self):
        data = page({(0, 0): 4})                             # green with STP
        view = View.of("frame-000.base", [part()])
        image = compose(data, view)
        image[0] = rgba(RED)
        self.assertEqual(plan(data, [(view, image, view.width)]).writes, {(0, -1): 3})   # red with STP

    def test_colour_outside_the_palette_is_an_error(self):
        data = page()
        view = View.of("frame-000.base", [part()])
        image = compose(data, view)
        image[1] = (8, 16, 24, 255)
        result = plan(data, [(view, image, view.width)])
        self.assertIn("not in the part's palette", result.errors["frame-000.base"])

    def test_shared_texel_with_two_wishes_conflicts(self):
        data = page()
        a = View.of("frame-000.base", [part()])
        b = View.of("frame-001.base", [part(x=10)])
        one, two = compose(data, a), compose(data, b)
        one[0], two[0] = rgba(RED), rgba(GREEN)
        result = plan(data, [(a, one, a.width), (b, two, b.width)])
        self.assertIn("frame-000.base", result.conflicts)
        self.assertEqual(result.writes, {})

    def test_4bpp_nibbles(self):
        data = bytearray(2)
        indexed.write_index(data, (0, 4), 9)
        indexed.write_index(data, (0, 0), 3)
        self.assertEqual(data[0], 0x93)
        self.assertEqual(indexed.read_index(data, (0, 4)), 9)

    def test_flip_reads_mirrored_texels(self):
        data = page({(0, 0): 1})
        view = View.of("frame-000.base", [part(flip=2)])
        self.assertEqual(compose(data, view)[3], rgba(RED))

    def test_sheet_overlaps_only_equal_texels(self):
        data = page({(0, 0): 1})
        sheet = Sheet(data, {0: (0, 8)}, 8, [part(w=8, h=8)])
        self.assertEqual(sheet.take(8, 8, 8, [1] + [0] * 63), (0, 0, 0))   # same indices: shares the texels
        self.assertEqual(sheet.take(8, 8, 8, [2] * 64), (0, 8, 0))         # different: next free cell

    def test_unreferenced_ink_is_held_for_direct_sprites(self):
        data = page({(0, 0): 5})
        sheet = Sheet(data, {0: (0, 8)}, 8, [], every_part=[part(u=64, v=64)])
        self.assertEqual(sheet.take(8, 8, 8, [0] * 64), (0, 8, 0))


class PlacerTest(unittest.TestCase):
    def frames(self):
        data = page({(0, 0): 1, (1, 0): 1, (2, 0): 2, (3, 0): 2})
        shared = part()
        write_frame(data, RECORDS, [shared])
        write_frame(data, RECORDS + 0x40, [replace_x(shared, 20)])
        a = View.of("frame-000.base", read_frame(data, RECORDS), frame=0, record=RECORDS)
        b = View.of("frame-001.base", read_frame(data, RECORDS + 0x40), frame=1, record=RECORDS + 0x40)
        return data, a, b

    def test_shared_tile_is_repacked_and_the_other_frame_kept(self):
        data, a, b = self.frames()
        image = compose(data, a)
        image[0] = rgba(BLUE)
        out = placer.place(bytes(data), FakeProfile(), [placer.Target("en-a", "item-a", a, image, a.width)], [a, b])
        self.assertIn(out.placed["en-a"], ("relocated", "rebuilt"))
        new = bytearray(out.data)
        for offset, record in out.records.items():
            new[offset:offset + len(record)] = record
        reread = View.of("a", read_frame(new, RECORDS))
        self.assertEqual(compose(new, placer._shift(reread, a), a.width), image)
        self.assertEqual(compose(new, View.of("b", read_frame(new, RECORDS + 0x40))), compose(data, b))

    def test_widened_image_is_rebuilt(self):
        data, a, b = self.frames()
        image = [TRANSPARENT] * (12 * 2)
        image[11] = rgba(GREEN)
        image[0] = rgba(RED)
        out = placer.place(bytes(data), FakeProfile(), [placer.Target("en-a", "item-a", a, image, 12)], [a, b])
        self.assertEqual(out.placed["en-a"], "rebuilt")
        new = bytearray(out.data)
        for offset, record in out.records.items():
            new[offset:offset + len(record)] = record
        reread = View.of("a", read_frame(new, RECORDS))
        self.assertEqual(compose(new, placer._shift(reread, a), 12), image)

    def test_no_records_defers(self):
        data, a, b = self.frames()
        image = compose(data, a)
        image[0] = rgba(BLUE)
        profile = FakeProfile(encode=None)
        out = placer.place(bytes(data), profile, [placer.Target("en-a", "item-a", a, image, a.width)], [a, b])
        self.assertIn("en-a", out.deferred)
        self.assertEqual(out.data, data)

    def test_part_image_is_folded_into_its_frame(self):
        data, a, b = self.frames()
        local = compose(data, View.of("p", [a.parts[0]]))
        local[1] = rgba(GREEN)
        merged = placer.frame_image(data, a, 0, local, 4)
        self.assertEqual(merged[1], rgba(GREEN))


def replace_x(p, x):
    from dataclasses import replace
    return replace(p, x=x)


class StillTest(unittest.TestCase):
    def test_goal_takes_only_written_pixels(self):
        import numpy as np
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "crop.png"
            crop = PILImage.new("RGBA", (2, 1), (0, 0, 0, 0))
            crop.putpixel((1, 0), (248, 0, 0, 255))
            crop.save(path)
            values, written = stills.crop_pixels(path, [1, 1, 2, 1])
            source = np.zeros((16, 16, 3), int)
            goal = stills.goal_image(source, [([1, 1, 2, 1], values, written)])
        self.assertEqual(goal[1, 2].tolist(), [31, 0, 0])
        self.assertEqual(int((goal != source).any(-1).sum()), 1)

    def test_partial_alpha_is_refused(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "crop.png"
            PILImage.new("RGBA", (1, 1), (1, 2, 3, 128)).save(path)
            with self.assertRaises(stills.StillError):
                stills.crop_pixels(path, [0, 0, 1, 1])


class ReportTest(unittest.TestCase):
    def test_every_item_has_one_status(self):
        def image(i):
            return Image(i, Path("x.png"), "0" * 64, "r", {})
        items = [Item("a", "candidate-only", "C", "frame", 1, [image("a1"), image("a2")]),
                 Item("b", "candidate-only", "C", "frame", 2, [image("b1"), image("b2")]),
                 Item("c", "deliberately-untranslated", "C", "frame", 3, [], "kept"),
                 Item("d", "missing", "C", "group", None, []),
                 Item("e", "candidate-only", "C", "frame", 4, [image("e1")])]
        states = {"a1": "in_place", "a2": "not_drawn", "b1": "rebuilt", "b2": "deferred", "e1": "deferred"}
        report = art_build.item_report(items, states, {"b2": "no room", "e1": "no room"}, {"b1"})
        status = {e["id"]: e["status"] for e in report["items"]}
        self.assertEqual(status, {"a": "placed", "b": "partly_placed", "c": "untranslated_by_decision",
                                  "d": "missing", "e": "deferred"})
        self.assertEqual(sum(report["totals"].values()), 5)
        self.assertEqual(report["items"][1]["images"]["b1"], "rebuilt (fallback)")

    def test_runtime_variant_filter(self):
        self.assertFalse(sprites.drawn_at_runtime({"runtime_variant_scope": {"runtime_applicable": False}}))
        self.assertFalse(sprites.drawn_at_runtime({"runtime_variant_scope": {"uses": {}}}))
        self.assertTrue(sprites.drawn_at_runtime({"runtime_variant_scope": {"uses": {"hud-43": "Saturday"}}}))


class ExecutableTest(unittest.TestCase):
    def test_records_follow_the_moved_block(self):
        start, end = art_build.MAINROM_RECORDS
        original = bytearray(end + 64)
        original[start:end] = hashlib.sha256(b"block").digest() * ((end - start) // 32) + bytes((end - start) % 32)
        english = bytes(16) + bytes(original)
        out = art_build.patch_executable(english, bytes(original), {start + 8: b"\x01\x02"})
        self.assertEqual(out[16 + start + 8:16 + start + 10], b"\x01\x02")
        with self.assertRaises(ArtBuildError):
            art_build.patch_executable(english[:16 + start], bytes(original), {start + 8: b"\x01\x02"})


@unittest.skipUnless((FILES / "SLPM_86.053").is_file(), "needs the original files in disc/files")
class GameDataTest(unittest.TestCase):
    def test_profiles_compose_the_reader_exports(self):
        inventory = json.loads(INVENTORY.read_text())
        exe = (FILES / "SLPM_86.053").read_bytes()
        checked = 0
        for path in ("CDROM/FUKUTEST/OPTION.NBN", "CDROM/FUKUTEST/TITLE.NBN", "CDROM/FUKUTEST/FILESEL.NBN",
                     "CDROM/NAME_ENT/NAME_ENT.NBN", "CDROM/FUKUTEST/EXAMINE.NBN", "CDROM/FUKUTEST/MAINROM.NBN"):
            data = (FILES / path).read_bytes()
            profile = sprites.PROFILES[path](data, exe, path)
            for item in inventory["artwork_items"]:
                if item["container"] != path or item.get("frame") is None:
                    continue
                for variant, sha in (item.get("source_png_sha256") or {}).items():
                    if "." in variant:
                        name = variant.rsplit(".", 1)[1]
                    else:
                        name = "base" if variant.startswith(("frame", "source")) else variant
                    view = profile.frame(item["frame"], name)
                    png = _png(view.width, view.height, b"".join(bytes(p) for p in compose(data, view)), 4)
                    self.assertEqual(hashlib.sha256(png).hexdigest(), sha, "%s %s" % (item["id"], variant))
                    checked += 1
        self.assertGreater(checked, 400)

    def test_original_images_change_nothing(self):
        exe = (FILES / "SLPM_86.053").read_bytes()
        for path in ("CDROM/FUKUTEST/OPTION.NBN", "CDROM/FUKUTEST/MAINROM.NBN"):
            data = (FILES / path).read_bytes()
            profile = sprites.PROFILES[path](data, exe, path)
            views = [v for v in profile.views.values() if v.key.endswith(".base")]
            targets = [placer.Target(v.key, v.key, v, compose(data, v), v.width) for v in views[:40]]
            out = placer.place(data, profile, targets, views)
            self.assertEqual(bytes(out.data), data)
            self.assertEqual(out.records, {})
            self.assertEqual(set(out.placed.values()), {"in_place"})


if __name__ == "__main__":
    unittest.main()
