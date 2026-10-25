"""Tests for source/native-bound prologue NBN cells and output-path safety."""
import hashlib
import json
import os
import struct
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from tools.assets import image_packs_prologue as prologue
from tools.assets import manifest
from tools.assets.media_prologue_export import export_prologue_source
from tools.assets.visual_text import build_catalog

REPOSITORY = Path(os.environ.get("TOKIMEMO_REPOSITORY_ROOT", Path.cwd())).resolve()
TRACK = REPOSITORY / "disc/track1.bin"
TEST_CACHE = REPOSITORY / ".asset-cache/media/nbn-prologue-cohort-candidate-v3/test-output"


def synthetic_source():
    """Distinct page/row index values expose a mistaken 120-vs-128 row offset."""
    data = bytearray(prologue.FILE_SIZE)
    for page in range(prologue.PAGE_COUNT):
        for row in range(prologue.SOURCE_PAGE_HEIGHT):
            value = (page * 29 + row) & 0xff
            start = page * prologue.PAGE_SIZE + row * prologue.PAGE_WIDTH
            data[start:start + prologue.PAGE_WIDTH] = bytes([value]) * prologue.PAGE_WIDTH
    palette = [0] + [i for i in range(1, 256)]
    struct.pack_into("<256H", data, prologue.PALETTE_OFFSET, *palette)
    data[prologue.TRAILER_OFFSET:] = bytes((i * 7) & 0xff for i in range(len(data) - prologue.TRAILER_OFFSET))
    return bytes(data), palette


def expected_cell_indices(data, cell_index):
    row, col = divmod(cell_index, 3)
    upper = col * prologue.PAGE_SIZE
    if row == 0:
        return data[upper:upper + prologue.ACTIVE_CELL_HEIGHT * prologue.PAGE_WIDTH]
    lower = (col + 3) * prologue.PAGE_SIZE
    first = data[upper + prologue.ACTIVE_CELL_HEIGHT * prologue.PAGE_WIDTH:
                 upper + prologue.SOURCE_PAGE_HEIGHT * prologue.PAGE_WIDTH]
    second_rows = prologue.ACTIVE_CELL_HEIGHT - (prologue.SOURCE_PAGE_HEIGHT - prologue.ACTIVE_CELL_HEIGHT)
    second = data[lower:lower + second_rows * prologue.PAGE_WIDTH]
    return first + second


class PrologueImageReaderTests(unittest.TestCase):
    def test_import_does_not_mutate_shared_manifest_roots(self):
        manifest_repository = Path(manifest.__file__).resolve().parents[2]
        self.assertEqual(manifest.REPOSITORY, manifest_repository)
        self.assertEqual(manifest.CACHE, manifest_repository / ".asset-cache")
        self.assertEqual(TEST_CACHE, REPOSITORY / ".asset-cache/media/nbn-prologue-cohort-candidate-v3/test-output")

    def test_native_uv_mapping_uses_top_tail_then_lower_page_head(self):
        data, palette = synthetic_source()
        row = {"path": prologue.FILES[0]["path"],
               "source_sha256": hashlib.sha256(data).hexdigest(),
               "source_size": len(data)}
        self.assertEqual(prologue.probe_prologue_pages(data, source_row=row)["status"], "supported")
        with tempfile.TemporaryDirectory() as temporary:
            result = prologue.export_prologue_pages(data, temporary, "prologue-test", source_row=row)
            self.assertEqual(result["format"], "tokimemo-prologue-native-cells")
            self.assertEqual(len(result["images"]), 6)
            for cell in result["images"]:
                idx = cell["cell_index"]
                indices = expected_cell_indices(data, idx)
                rgba = b"".join(prologue._color(palette[value]) for value in indices)
                expected = prologue._png(128, 120, rgba, 4)
                self.assertEqual((Path(temporary) / cell["png"]).read_bytes(), expected)
                expected_bit15 = bytes(255 if palette[value] & 0x8000 else 0 for value in indices)
                self.assertEqual((Path(temporary) / cell["clut_bit15_png"]).read_bytes(),
                                 prologue._png(128, 120, expected_bit15, 1))
                self.assertEqual(cell["preview_kind"], "native_uv_mapped_texture_cell")
            # A lower cell is intentionally not the former lower-page top-120 crop.
            old_crop = data[3 * prologue.PAGE_SIZE:3 * prologue.PAGE_SIZE + 120 * 128]
            self.assertNotEqual(expected_cell_indices(data, 3), old_crop)
            self.assertEqual(result["images"][3]["source_spans"][0]["source_region"], [0, 120, 128, 8])
            self.assertEqual(result["images"][3]["source_spans"][1]["source_region"], [0, 0, 128, 112])
            self.assertEqual(result["images"][3]["unused_source_spans_for_cell"][0]["source_region"], [0, 112, 128, 16])
            self.assertTrue(result["metadata"]["source_bytes_unmodified"])
            self.assertFalse(result["metadata"]["hardware_raster_parity_verified"])

    def test_visual_text_catalog_gives_each_native_cell_a_stable_identity(self):
        data, _ = synthetic_source()
        row = {"path": prologue.FILES[0]["path"],
               "source_sha256": hashlib.sha256(data).hexdigest(),
               "source_size": len(data)}
        with tempfile.TemporaryDirectory() as temporary:
            result = prologue.export_prologue_pages(data, temporary, "prologue-test", source_row=row)
            outputs = []
            for filename in result["files"]:
                content = (Path(temporary) / filename).read_bytes()
                outputs.append({"path": filename, "sha256": hashlib.sha256(content).hexdigest()})
            media_manifest = {
                "schema": "tokimemo.media.v1",
                "input": {"source_sha256": prologue.TRACK_SHA256},
                "assets": [{
                    "id": "prologue-test",
                    "path": row["path"],
                    "status": "exported",
                    "format": result["format"],
                    "source": {"path": row["path"], "sha256": row["source_sha256"]},
                    "outputs": outputs,
                    "export": result,
                }],
            }
            catalog = build_catalog(media_manifest)
        regions = catalog["regions"]
        self.assertEqual(len(regions), 6)
        self.assertEqual(len({region["region_id"] for region in regions}), 6)
        by_variant = {region["variant_metadata"]["variant"]: region for region in regions}
        self.assertEqual(set(by_variant), {"native-cell-%d" % index for index in range(6)})
        for index in range(6):
            region = by_variant["native-cell-%d" % index]
            self.assertEqual(region["variant_metadata"]["cell_index"], index)
            self.assertEqual(region["png"]["path"], result["images"][index]["png"])
            self.assertIsNone(region["stp"])
            self.assertTrue(result["images"][index]["clut_bit15_png"].endswith(".clut-bit15.png"))

    def test_decoder_rejects_unsafe_filename_components_before_writing(self):
        data, _ = synthetic_source()
        row = {"path": prologue.FILES[0]["path"],
               "source_sha256": hashlib.sha256(data).hexdigest(),
               "source_size": len(data)}
        for bad in ("../escape", "a/../../escape", "/tmp/escaped-asset", "..", "a\\b", ""):
            with self.subTest(asset_id=bad), tempfile.TemporaryDirectory() as temporary:
                with self.assertRaisesRegex(ValueError, "safe filename component"):
                    prologue.export_prologue_pages(data, temporary, bad, source_row=row)
                self.assertEqual(list(Path(temporary).iterdir()), [])

    def test_wrong_source_bytes_or_size_fail_closed(self):
        data, _ = synthetic_source()
        row = {"path": prologue.FILES[0]["path"],
               "source_sha256": hashlib.sha256(data).hexdigest(),
               "source_size": len(data)}
        self.assertEqual(prologue.probe_prologue_pages(data[:-1], source_row=row)["status"], "invalid")
        changed = bytearray(data)
        changed[0] ^= 1
        self.assertEqual(prologue.probe_prologue_pages(bytes(changed), source_row=row)["status"], "invalid")


@unittest.skipUnless(TRACK.is_file(), "original MODE2 track is unavailable")
class OriginalPrologueWitnessTests(unittest.TestCase):
    def setUp(self):
        self._manifest_roots_before = (manifest.REPOSITORY, manifest.CACHE)
        self.addCleanup(self._assert_manifest_roots_restored)
        repository_patch = patch.object(manifest, "REPOSITORY", REPOSITORY)
        cache_patch = patch.object(manifest, "CACHE", REPOSITORY / ".asset-cache")
        repository_patch.start()
        cache_patch.start()
        self.addCleanup(repository_patch.stop)
        self.addCleanup(cache_patch.stop)
        TEST_CACHE.mkdir(parents=True, exist_ok=True)

    def _assert_manifest_roots_restored(self):
        self.assertEqual((manifest.REPOSITORY, manifest.CACHE), self._manifest_roots_before)

    def test_original_track_upload_uv_closure_and_source_build_exact_four_rows(self):
        from tools.assets.input import open_input
        with open_input(TRACK) as source:
            context = prologue.build_profile_context(source, source.entries(), REPOSITORY)
        self.assertEqual([row["path"] for row in context["source_assets"]],
                         [row["path"] for row in prologue.FILES])
        for record, expected in zip(context["source_assets"], prologue.FILES):
            self.assertEqual(record["lba"], expected["lba"])
            self.assertEqual(record["native_read_extent"]["requested_sector_count"], 49)
            self.assertEqual(record["native_read_extent"]["sectors_after_iso_extent"], 0)
        profile = context["texture_profile"]
        self.assertEqual(profile["native_uv_mapping"]["lower_grid_row"]["source_spans"],
                         [{"page_row": 0, "rows": [120, 128]}, {"page_row": 1, "rows": [0, 112]}])
        self.assertTrue({"src/main/actor_sprites.c", "src/main/frame_render_audio.c",
                         "asm/matchings/main/actor_sprites/load_tpage_buf_lock.s",
                         "asm/matchings/main/frame_render_audio/csr_load_vram.s",
                         "asm/matchings/main/frame_render_audio/LoadSquare.s"}.issubset(context["source_files"]))
        self.assertEqual(context["texture_profile"]["grid_columns"], 3)
        self.assertEqual(context["texture_profile"]["grid_rows"], 2)

    def _minimal_baseline_with_selected(self, source, selected, id_override=None):
        entries = {entry["path"]: entry for entry in source.entries()}
        rows = []
        for path in selected:
            row = entries[path]
            asset = {"id": manifest.asset_id(path, row["sha256"]),
                     "source": {"path": path, "sha256": row["sha256"]}}
            if path == prologue.FILES[0]["path"] and id_override is not None:
                asset["id"] = id_override
            rows.append(asset)
        return {"schema": manifest.SCHEMA,
                "input": {"source_sha256": prologue.TRACK_SHA256}, "assets": rows}

    def _run_export(self, source, destination, baseline):
        return export_prologue_source(source, destination, baseline, REPOSITORY,
                                      [row["path"] for row in prologue.FILES])

    def test_exporter_rejects_absolute_and_traversal_ids_without_escaped_files(self):
        from tools.assets.input import open_input
        selected = [row["path"] for row in prologue.FILES]
        with open_input(TRACK) as source:
            for traversal in (False, True):
                with self.subTest(traversal=traversal), tempfile.TemporaryDirectory(dir=TEST_CACHE) as parent, tempfile.TemporaryDirectory() as outside_root:
                    destination = Path(parent) / "export"
                    outside = Path(outside_root) / "escaped"
                    bad = os.path.relpath(outside, TEST_CACHE) if traversal else str(outside)
                    baseline = self._minimal_baseline_with_selected(source, selected, bad)
                    with self.assertRaisesRegex(ValueError, "source-derived stable ID"):
                        self._run_export(source, destination, baseline)
                    self.assertFalse(outside.exists())
                    self.assertFalse(destination.exists())

    def test_exporter_preflights_all_paths_and_rejects_symlink_escape_before_pngs(self):
        from tools.assets.input import open_input
        selected = [row["path"] for row in prologue.FILES]
        with tempfile.TemporaryDirectory(dir=TEST_CACHE) as parent, tempfile.TemporaryDirectory() as outside:
            destination = Path(parent) / "export"
            expected_id = manifest.asset_id(prologue.FILES[0]["path"], prologue.FILES[0]["sha256"])
            target = destination / "assets" / expected_id
            target.mkdir(parents=True)
            outside_png = Path(outside) / (expected_id + ".cell-0.png")
            (target / (expected_id + ".cell-0.png")).symlink_to(outside_png)
            with open_input(TRACK) as source:
                baseline = self._minimal_baseline_with_selected(source, selected)
                with self.assertRaisesRegex(ValueError, "output must be below .asset-cache"):
                    self._run_export(source, destination, baseline)
            self.assertFalse(outside_png.exists())
            self.assertEqual([item.name for item in target.iterdir()], [expected_id + ".cell-0.png"])
            self.assertTrue((target / (expected_id + ".cell-0.png")).is_symlink())

    def test_fixed_file_lba_rows_reject_path_and_lba_substitution(self):
        from tools.assets.input import open_input
        with open_input(TRACK) as source:
            rows = source.entries()
        original = next(row for row in rows if row["path"] == prologue.FILES[0]["path"])
        altered = dict(original, path="CDROM/PROLODIR/CUT_06.NBN")
        rows.remove(original)
        rows.append(altered)
        with self.assertRaisesRegex(ValueError, "CUT_05"):
            prologue._check_asset_inventory(rows)

    def test_original_overlay_instruction_and_helper_bytes_are_pinned(self):
        exe = (REPOSITORY / "disc/files/SLPM_86.053").read_bytes()
        overlay = (REPOSITORY / "disc/files/CDROM/EXEDIR/NAME_ENT.EXN").read_bytes()
        changed = bytearray(overlay)
        changed[prologue.FILES[0]["native_code_offset"]] ^= 1
        with self.assertRaisesRegex(ValueError, "overlay differs"):
            prologue._check_original_code(exe, bytes(changed))


if __name__ == "__main__":
    unittest.main()
