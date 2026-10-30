"""Reversible code mappings and the T-39020 projection report; synthetic images only.

Run: docker run --rm -v "$PWD:/work:ro" -w /work tokimemo-recovery \
       python3 -m unittest tools.port.test_rgb555_diagnostic
"""
import io
import json
import tempfile
import unittest
from contextlib import redirect_stdout
from pathlib import Path

from PIL import Image

from tools.port import rgb555_diagnostic as diag
from tools.port.rgb555_diagnostic import Rejected, compare_codes, decode_channel, projection_report


def zero(code):
    return code << 3


def replicated(code):
    return (code << 3) | (code >> 2)


def ramp(width, height, expand):
    image = Image.new("RGB", (width, height))
    for y in range(height):
        for x in range(width):
            image.putpixel((x, y), (expand((x + y) % 32), expand(x % 32), expand(y % 32)))
    return image


class RGB555DiagnosticTests(unittest.TestCase):
    def test_all_codes_roundtrip_under_both_explicit_conventions(self):
        for code in range(32):
            self.assertEqual(decode_channel(code << 3, 'zero_extended'), code)
            self.assertEqual(decode_channel((code << 3) | (code >> 2), 'bit_replicated'), code)
        for value in range(256):
            if value not in {c << 3 for c in range(32)} | {(c << 3) | (c >> 2) for c in range(32)}:
                with self.assertRaises(ValueError):
                    decode_channel(value, 'source_proven_mixed')

    def test_display_difference_is_only_diagnostic(self):
        a = Image.new('RGB', (32, 1))
        b = Image.new('RGB', (32, 1))
        for c in range(32):
            a.putpixel((c, 0), (c << 3,) * 3)
            b.putpixel((c, 0), ((c << 3) | (c >> 2),) * 3)
        report = compare_codes(a, b, 'zero_extended', 'bit_replicated')
        self.assertEqual(report['different_color_codes'], 0)
        self.assertFalse(report['acceptance'])
        b.putpixel((0, 0), (8, 0, 0))
        self.assertEqual(compare_codes(a, b, 'zero_extended', 'bit_replicated')['different_color_codes'], 1)

    def test_no_lossy_alpha_or_resize(self):
        a = Image.new('RGBA', (1, 1), (0, 0, 0, 255))
        b = Image.new('RGBA', (1, 1), (0, 0, 0, 0))
        with self.assertRaises(ValueError):
            compare_codes(a, b, 'zero_extended', 'zero_extended')
        with self.assertRaises(ValueError):
            compare_codes(a, Image.new('RGB', (2, 2)), 'zero_extended', 'zero_extended')


class ProjectionReportTests(unittest.TestCase):
    def test_zero_extended_vs_bit_replicated_has_exact_diff_but_no_residual(self):
        native, port = ramp(40, 20, zero), ramp(40, 20, replicated)
        report = projection_report(native, port, "zero_extended", "bit_replicated")
        self.assertFalse(report["acceptance"])
        self.assertEqual(report["rgb555_residual"]["pixels"], 0)
        self.assertIsNone(report["rgb555_residual"]["bbox"])
        # codes 0..3 expand identically under both rules; every other pixel differs exactly
        self.assertGreater(report["exact_rgba8"]["pixels"], 0)
        self.assertEqual(report["compared_pixels"], 800)

    def test_mixed_encoding_accepts_both_forms_per_channel(self):
        native = ramp(16, 16, zero)
        port = ramp(16, 16, replicated)
        port.putpixel((3, 3), (zero(6), replicated(3), zero(3)))  # same codes as native (6,3,3)
        report = projection_report(native, port, "zero_extended", "source_proven_mixed")
        self.assertEqual(report["rgb555_residual"]["pixels"], 0)
        with self.assertRaises(Rejected):
            projection_report(native, port, "zero_extended", "bit_replicated")

    def test_residual_count_bbox_tiles_and_deltas(self):
        native = ramp(48, 32, zero)
        port = native.copy()
        for x, y in ((5, 6), (20, 17), (33, 30)):
            r, g, b = port.getpixel((x, y))
            port.putpixel((x, y), (zero((r >> 3) ^ 1), g, b))
        report = projection_report(native, port, "zero_extended", "zero_extended")
        residual = report["rgb555_residual"]
        self.assertEqual(residual["pixels"], 3)
        self.assertEqual(residual["bbox"], [5, 6, 33, 30])
        self.assertEqual([(t["x"], t["y"], t["pixels"]) for t in residual["tiles"]], [(0, 0, 1), (16, 16, 1), (32, 16, 1)])
        self.assertEqual(sum(d["pixels"] for d in residual["deltas"]), 3)
        self.assertEqual(report["exact_rgba8"]["pixels"], 3)

    def test_edge_exclusion_is_named_and_counted_never_dropped(self):
        native = ramp(10, 8, zero)
        port = native.copy()
        for y in range(8):
            native.putpixel((0, y), (0, 0, 0))  # emulator black column
        port.putpixel((4, 0), (zero(31), zero(31), zero(31)))
        report = projection_report(native, port, "zero_extended", "zero_extended",
                                   ["emulator_x0_column", "emulator_y0_row"])
        rows = {row["name"]: row for row in report["domain_exclusions"]}
        self.assertEqual(rows["emulator_x0_column"]["pixels"], 8)
        self.assertGreater(rows["emulator_x0_column"]["exact_changed"], 0)
        self.assertEqual(rows["emulator_y0_row"]["pixels"], 9)  # (0,0) belongs to the first named domain
        self.assertEqual(rows["emulator_y0_row"]["rgb555_changed"], 1)
        self.assertTrue(all(row["reason"] for row in rows.values()))
        self.assertEqual(report["rgb555_residual"]["pixels"], 0)
        self.assertEqual(report["compared_pixels"], 80 - 17)
        unexcluded = projection_report(native, port, "zero_extended", "zero_extended")
        self.assertGreater(unexcluded["rgb555_residual"]["pixels"], 0)
        self.assertEqual(unexcluded["domain_exclusions"], [])

    def test_rejects_values_outside_declared_encoding_with_counts(self):
        native = ramp(8, 8, zero)
        port = ramp(8, 8, zero)
        port.putpixel((2, 1), (13, 0, 0))  # 13 = (1<<3)|5: neither form
        with self.assertRaises(Rejected) as caught:
            projection_report(native, port, "zero_extended", "source_proven_mixed")
        detail = caught.exception.detail
        self.assertEqual(detail["port"]["pixels_outside_encoding"], 1)
        self.assertEqual(detail["port"]["examples"], [{"x": 2, "y": 1, "rgb": [13, 0, 0]}])
        self.assertEqual(detail["native"]["pixels_outside_encoding"], 0)
        # a replicated native image is outside zero_extended: rejected, not tolerated
        with self.assertRaises(Rejected):
            projection_report(ramp(8, 8, replicated), ramp(8, 8, zero), "zero_extended", "zero_extended")

    def test_rejects_undeclared_inputs(self):
        a = ramp(4, 4, zero)
        with self.assertRaises(ValueError):
            projection_report(a, a, "nearest", "zero_extended")
        with self.assertRaises(ValueError):
            projection_report(a, a, "zero_extended", "zero_extended", ["phase_search"])
        with self.assertRaises(ValueError):
            projection_report(a, ramp(4, 5, zero), "zero_extended", "zero_extended")
        translucent = Image.new("RGBA", (4, 4), (0, 0, 0, 128))
        with self.assertRaises(ValueError):
            projection_report(a, translucent, "zero_extended", "zero_extended")

    def test_cli_writes_new_report_with_input_hashes_and_rejection_exit(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            ramp(8, 8, zero).save(root / "n.png")
            ramp(8, 8, replicated).save(root / "p.png")
            args = [str(root / "n.png"), str(root / "p.png"), "--native-encoding", "zero_extended",
                    "--port-encoding", "bit_replicated", "--basis", "synthetic", "--out", str(root / "r.json")]
            self.assertEqual(diag.main(args), 0)
            report = json.loads((root / "r.json").read_text())
            self.assertEqual(report["status"], "reported")
            self.assertEqual(len(report["inputs"]["native"]["sha256"]), 64)
            with self.assertRaises(FileExistsError):
                diag.main(args)
            bad = list(args)
            bad[bad.index("bit_replicated")] = "zero_extended"
            bad[-1] = str(root / "r2.json")
            self.assertEqual(diag.main(bad), 2)
            self.assertEqual(json.loads((root / "r2.json").read_text())["status"], "rejected")
            with redirect_stdout(io.StringIO()):
                self.assertEqual(diag.main(args[:-2]), 0)


if __name__ == '__main__':
    unittest.main()
