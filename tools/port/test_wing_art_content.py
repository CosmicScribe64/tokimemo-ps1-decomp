"""Original-free tests for the wing-art pack exporter (T-39220)."""
import importlib.util
import json
from pathlib import Path
import tempfile
import unittest
import zlib

import numpy as np

MODULE = Path(__file__).with_name("wing_art_content.py")
SPEC = importlib.util.spec_from_file_location("wing_art_content", MODULE)
wing = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(wing)
RUNTIME = Path(__file__).with_name("build_installed_runtime.py")
RSPEC = importlib.util.spec_from_file_location("build_installed_runtime", RUNTIME)
runtime = importlib.util.module_from_spec(RSPEC)
RSPEC.loader.exec_module(runtime)


def synthetic_still(w=320, h=160, seed=3):
    """Asymmetric 5-bit test still (no left/right symmetry anywhere)."""
    y, x = np.mgrid[0:h, 0:w]
    r = (x * 31 // (w - 1))
    g = ((x * x + seed * y) // 97) % 32
    b = (y * 31 // (h - 1))
    return np.stack([r, g, b], -1).astype(np.uint8)


class IntegerMathTests(unittest.TestCase):
    def test_smoothstep_endpoints_and_monotonic(self):
        values = wing.smooth_q12(np.arange(-100, wing.Q + 100, 7))
        self.assertEqual(int(wing.smooth_q12(0)), 0)
        self.assertEqual(int(wing.smooth_q12(wing.Q)), wing.Q)
        self.assertTrue(np.all(np.diff(values) >= 0))

    def test_truncating_division_matches_gdscript(self):
        self.assertEqual(wing.tdiv(-7, 2), -3)
        self.assertEqual(wing.tdiv(7, 2), 3)
        self.assertEqual(wing.tdiv(-8, 4), -2)

    def test_tables_are_integer_and_bounded(self):
        sin = wing.sin_table()
        env = wing.envelope_table()
        self.assertEqual(len(sin), 64)
        self.assertEqual(sin[0], 0)
        self.assertEqual(sin[16], 4096)
        self.assertTrue(all(isinstance(v, int) for v in sin + env))
        self.assertEqual(env[0], 0)
        self.assertLessEqual(max(env), 256)

    def test_png_is_deterministic_and_valid(self):
        pixels = np.arange(4 * 5 * 3, dtype=np.uint8).reshape(4, 5, 3)
        first, second = wing.png_bytes(pixels), wing.png_bytes(pixels)
        self.assertEqual(first, second)
        self.assertTrue(first.startswith(b"\x89PNG\r\n\x1a\n"))
        idat = first[first.index(b"IDAT") + 4:first.index(b"IEND") - 8]
        raw = zlib.decompress(idat)
        self.assertEqual(len(raw), 4 * (1 + 5 * 3))


class PlateTests(unittest.TestCase):
    def setUp(self):
        self.look = dict(wing.LOOKS["yozakura"])
        self.still = synthetic_still()

    def test_plate_is_deterministic_5bit_and_sized(self):
        a = wing.build_plate(self.still, self.look)
        b = wing.build_plate(self.still, self.look)
        self.assertEqual(a.shape, (wing.PLATE_H, wing.PLATE_W, 3))
        self.assertTrue(np.array_equal(a, b))
        self.assertLessEqual(int(a.max()), 31)

    def test_wing_rows_mirror_but_band_centre_does_not(self):
        field = wing.panorama(self.still, self.look)
        row = wing.AY + 100  # a wing row
        left = field[row, :wing.AX]
        right = field[row, wing.PLATE_W - wing.AX:][::-1]
        self.assertTrue(np.array_equal(left, right), "wings are mirror images")
        band = wing.AY + 245  # a bottom band row
        c = wing.CENTRE_X
        self.assertFalse(np.array_equal(field[band, c - 20:c], field[band, c:c + 20][::-1]),
                         "the band centre is not an exact mirror seam")

    def test_band_zone_fades_in_and_out_smoothly(self):
        _wing, _zone, weight = wing.column_map(159, 320, 176)
        start, end = wing.ZONE_START, wing.ZONE_START + wing.ZONE_W
        self.assertEqual(int(weight[start - 1]), 0)
        self.assertEqual(int(weight[end]), 0)
        self.assertEqual(int(weight[wing.CENTRE_X]), wing.Q)
        ramp = weight[start:start + 33].astype(int)
        self.assertTrue(np.all(np.diff(ramp) >= 0))
        self.assertLess(int(np.diff(ramp).max()), wing.Q // 8, "no hard step at the zone edge")

    def test_sky_fill_has_no_seam_stronger_than_the_still(self):
        # A row-ramp still: adjacent source rows differ by about one step, so
        # a tiled fill would show a jump of `rows` steps at each wrap. The
        # reflected fill may only repeat a row, never jump. In the cross-fade
        # into the still two shifted reads blend, which may add a small step.
        y, x = np.mgrid[0:160, 0:320]
        still = np.stack([y * 31 // 159, x * 31 // 319, y // 6], -1).astype(np.uint8)
        own = np.abs(np.diff(wing.expand5(still).astype(int), axis=0)).max()
        sides = np.r_[0:wing.AX, wing.AX + 320:wing.PLATE_W]
        for name, look in wing.LOOKS.items():
            if not look.get("sky"):
                continue
            field = wing.panorama(still, look)[:, sides].astype(int)
            _rows, _dx, fade = look["sky"]
            pure = look["row0"] - fade // 2  # rows above this read only the fill
            step = np.abs(np.diff(field[:pure], axis=0)).max()
            self.assertLessEqual(step, own, f"{name}: sky fill row step {step} > still's own {own}")
            blend = np.abs(np.diff(field[:look["row0"] + fade // 2 + 1], axis=0)).max()
            self.assertLessEqual(blend, 2 * own, f"{name}: cross-fade row step {blend} > 2x still's own {own}")

    def test_gain_is_bright_near_the_picture_and_dark_far_away(self):
        g = wing.gain_field(self.look)
        near = int(g[wing.AY + 120, wing.AX - 14])
        far = int(g[wing.AY + 120, 5])
        shadow = int(g[wing.AY + 120, wing.AX - 1])
        self.assertGreater(near, far)
        self.assertLess(shadow, near)
        self.assertTrue(np.all(g >= 0))

    def test_every_look_names_a_pinned_source(self):
        for name, look in wing.LOOKS.items():
            self.assertIn(look["src"], wing.SOURCES, name)
            self.assertIn(look["particles"], ("petals", "leaves", "fireworks", "none", "season"), name)
        self.assertEqual(wing.SEASONS["spring"], "yozakura")
        self.assertEqual(wing.LOOKS["winter"]["particles"], "none", "winter has no re-tinted petals")


class ParticleTests(unittest.TestCase):
    def test_drifters_are_deterministic_and_bounded(self):
        a = wing.drifters("yozakura", 16, cap_q8=205)
        b = wing.drifters("yozakura", 16, cap_q8=205)
        self.assertEqual(a, b)
        self.assertEqual(len(a), 32)
        for item in a:
            self.assertIn(item["side"], (0, 1))
            self.assertTrue(0 <= item["t0"] < wing.LOOP_TICKS)
            self.assertTrue(0 <= item["ph"] < 64)
            self.assertLessEqual(item["cap"], 205)

    def test_fireworks_are_dim_and_alternate(self):
        spec = wing.particle_spec("summer", "fireworks")
        self.assertLessEqual(spec["strength_q8"], 102)  # <= 0.40
        sides = sorted(b["side"] for b in spec["bursts"])
        self.assertEqual(sides, [0, 1])
        self.assertNotEqual(spec["bursts"][0]["t0"], spec["bursts"][1]["t0"])
        self.assertEqual(spec["bursts"][0]["y"], spec["bursts"][1]["y"])

    def test_firework_steps_are_cropped_to_their_radius(self):
        rows = np.zeros((2144, 128), np.uint8)
        for k in range(16):  # CLUT step k: index 10 lit
            rows[k * 4, 20:22] = [0xff, 0x7f]
        yy, xx = np.mgrid[0:103, 0:128]
        rows[1622:1725][(xx - 63) ** 2 + (yy - 51) ** 2 < 55 ** 2] = 10
        for r0, r1 in wing.HANABI_SHAPES[1:]:
            rows[r0:r1, 60:66] = 10
        frames, size = wing.hanabi_steps(rows.tobytes(), 30)
        self.assertEqual(size, 61)
        self.assertEqual(len(frames), 16)
        first = frames[0]
        self.assertTrue(first[..., 3].any())
        corner = first[:6, :6, 3]
        self.assertFalse(corner.any(), "nothing outside the crop circle")


class OverrideTests(unittest.TestCase):
    def test_default_table_is_valid(self):
        wing.validate_overrides(json.loads(json.dumps(wing.DEFAULT_OVERRIDES)), wing.LOOKS)

    def test_bad_tables_are_rejected(self):
        good = json.loads(json.dumps(wing.DEFAULT_OVERRIDES))
        for mutate in (lambda t: t.update(schema="x"),
                       lambda t: t.update(default="invented"),
                       lambda t: t["rules"].append({"match": {"stage": "x"}, "style": "invented"}),
                       lambda t: t["rules"].append({"match": {"weather": "rain"}, "style": "winter"}),
                       lambda t: t["rules"].append({"match": {"stage": "a", "class": "A"}, "style": "winter"})):
            table = json.loads(json.dumps(good))
            mutate(table)
            with self.assertRaises(wing.WingArtError):
                wing.validate_overrides(table, wing.LOOKS)


class SourceTests(unittest.TestCase):
    def test_changed_original_is_refused(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "CDROM" / "BS"
            path.mkdir(parents=True)
            (path / "D_10_08.BS").write_bytes(b"not the original")
            with self.assertRaises(wing.WingArtError):
                wing.read_sources(tmp, ["D_10_08"])

    def test_missing_original_is_refused(self):
        with tempfile.TemporaryDirectory() as tmp:
            with self.assertRaises(wing.WingArtError):
                wing.read_sources(tmp, ["HANABI"])

    def test_output_must_be_new_and_under_the_cache(self):
        with self.assertRaises(wing.WingArtError):
            wing._check_output("/tmp/elsewhere")
        with self.assertRaises(wing.WingArtError):
            wing._check_output(str(wing.ROOT / ".asset-cache"))


class InstalledRoleTests(unittest.TestCase):
    def test_wing_art_is_an_optional_directory_role(self):
        self.assertEqual(runtime.ROLE_KINDS.get("wing_art"), "dir")
        self.assertNotIn("wing_art", runtime.UNSUPPLIED_ROLES)
        coverage = runtime._coverage({"rule_tables": "packs/rules/manifest.json"},
                                     {"wing_art": "wing_art: exporter failed"})
        self.assertIn("wing_art", coverage["unavailable_roles"])
        self.assertNotIn("wing_art", coverage["available_roles"])


if __name__ == "__main__":
    unittest.main()
