"""Synthetic v1 contract tests; all PNG fixtures are locally invented."""
import hashlib
import json
import os
import struct
import tempfile
import unittest
import zlib
from pathlib import Path

from validate_content import ContentError, EquivalenceError, _decode_png, validate_manifest, validate_pack
from jsonschema import Draft202012Validator

CANONICAL_PATH = "CDROM/PROLODIR/CUT_05.NBN"
SCHEMA_PATH = Path(__file__).parent / "schemas/tokimemo.port-content.v1.schema.json"
SCHEMA_DOCUMENT = json.loads(SCHEMA_PATH.read_text())
SCHEMA_VALIDATOR = Draft202012Validator(SCHEMA_DOCUMENT)

CANONICAL_ID = "region-10ce2e8acb8c5672d2fcf21dae6687e599234b8d118b481a964defa71f2140d1"


def _png(width=1, height=1):
    def chunk(kind, payload):
        return struct.pack(">I", len(payload)) + kind + payload + struct.pack(">I", zlib.crc32(kind + payload) & 0xffffffff)
    ihdr = struct.pack(">IIBBBBB", width, height, 8, 6, 0, 0, 0)
    raw = b"".join(b"\0" + bytes((255, 0, 128, 255)) * width for _ in range(height))
    return b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", ihdr) + chunk(b"IDAT", zlib.compress(raw)) + chunk(b"IEND", b"")


def _canonical():
    path = Path(__file__).resolve().parents[2] / "port/godot/schemas/canonical_prologue_nbn_cells.v1.json"
    return json.loads(path.read_text())["entries"][CANONICAL_PATH + "#0"]


def manifest(component=None):
    value = {"schema": "tokimemo.port-content.v1", "schema_version": 1,
        "release_profile": "original-ja-ps-the-best",
        "source": {"format": "mode2-2352", "sha256": "28c3c501b0c67b87ca1e30515da73619497d509ff6adc28ff722aa02897dfb72", "size_bytes": 702072000, "read_only": True},
        "created_by": {"reader": "tools.assets.input.open_input", "media_schema": "tokimemo.media.v1",
            "reviewed_component_schema": "tokimemo-prologue-nbn-source-profile.v2",
            "media_manifest_sha256": "26d865d627e3ab103514d256b1b36aad6fd5eed5108877d82b863c22a3e5efde",
            "visual_catalog_sha256": "d155a2958e7e8694a9b5444412ef71843a0229b15503e713e911a9c1b5f8b4d6",
            "cell_map_sha256": "5eb43589bb9722a2792c6d8c880141c36a7aa01fcc14fdfbe82be6de07282209",
            "input_reader_source_sha256": "d" * 64, "importer_source_sha256": "e" * 64, "validator_source_sha256": "f" * 64},
        "components": [], "route_capabilities": {"available": [], "unavailable": ["route"]}}
    if component is not None:
        value["components"] = [component]
    return value


def _preview_component():
    cell = _canonical()
    return {"id": cell["canonical_region_id"], "version": 1, "kind": "png_preview", "status": "available",
        "capabilities": ["preview"], "source": {"path": CANONICAL_PATH, "format": "mode2-iso-file",
            "size_bytes": 100352, "sha256": cell["source_sha256"], "locator": {"lba": 1}},
        "payload": "payloads/preview.png", "payload_size_bytes": len(_png()), "payload_sha256": cell["png_sha256"],
        "metadata": {"canonical_region_id": cell["canonical_region_id"], "cell_index": 0,
            "source_variant": cell["source_variant"], "palette_variant": {"palette_index": cell["palette_index"],
                "source_sha256": cell["palette_source_sha256"], "source_offset": cell["palette_source_offset"], "status": cell["palette_status"]},
            "use_aliases": [{"caller": cell["caller"], "screen_context": cell["screen_context"]}],
            "width": cell["width"], "height": cell["height"], "grid_column": cell["grid_column"], "grid_row": cell["grid_row"],
            "source_texture_dimensions": cell["source_texture_dimensions"], "source_spans": cell["source_spans"],
            "unused_source_spans_for_cell": cell["unused_source_spans_for_cell"], "clut_bit15_status": cell["clut_bit15_status"],
            "static_only": True, "animation_composition_proven": False}}


class ContentContractTests(unittest.TestCase):
    def test_synthetic_png_is_decoded_and_truncation_rejected(self):
        width, height, pixels = _decode_png(_png())
        self.assertEqual((width, height, pixels[:4]), (1, 1, b"\xff\0\x80\xff"))
        with self.assertRaisesRegex(EquivalenceError, "incomplete|signature/size"):
            _decode_png(b"\x89PNG\r\n\x1a\n")

    def test_unknown_schema_and_non_integer_version_rejected(self):
        value = manifest(); value["schema"] = "tokimemo.port-content.v2"
        with self.assertRaisesRegex(ContentError, "unsupported content schema"):
            validate_manifest(value)
        value = manifest(); value["schema_version"] = True
        with self.assertRaisesRegex(ContentError, "schema_version"):
            validate_manifest(value)

    def test_missing_required_source_format_and_extra_component_field_rejected(self):
        component = {"id": "unsupported-0", "version": 1, "kind": "inventory_entry", "status": "unsupported", "capabilities": [],
            "source": {"path": CANONICAL_PATH, "size_bytes": 1, "sha256": "a" * 64, "locator": {}}}
        value = manifest(component); value["source"].pop("format")
        with self.assertRaisesRegex(ContentError, "source fields or format"):
            validate_manifest(value)
        component["source"]["format"] = "mode2-iso-file"
        component["source"].pop("format")
        value = manifest(component)
        with self.assertRaisesRegex(ContentError, "component .* source fields or format"):
            validate_manifest(value)
        component["source"]["format"] = "mode2-iso-file"
        value = manifest(component); component["new_schema_feature"] = True
        with self.assertRaisesRegex(ContentError, "component .* fields"):
            validate_manifest(value)


    def test_unknown_source_format_and_creator_keys_rejected(self):
        value = manifest(); value["source"]["format"] = "mode2-2352-extra"
        with self.assertRaisesRegex(ContentError, "source fields or format"):
            validate_manifest(value)
        value = manifest(); value["created_by"]["extra"] = "not-in-v1"
        with self.assertRaisesRegex(ContentError, "created_by reader provenance"):
            validate_manifest(value)
        component = {"id": "inventory-0", "version": 1, "kind": "inventory_entry", "status": "unsupported", "capabilities": [],
            "source": {"path": CANONICAL_PATH, "format": "mode2-iso-file-extra", "size_bytes": 1, "sha256": "a" * 64, "locator": {}}}
        with self.assertRaisesRegex(ContentError, "source fields or format"):
            validate_manifest(manifest(component))


    def test_draft_2020_12_schema_matches_positive_inventory_and_actual_pack(self):
        Draft202012Validator.check_schema(SCHEMA_DOCUMENT)
        positive_inventory = manifest({"id": "inventory-0", "version": 1, "kind": "inventory_entry", "status": "unsupported",
            "capabilities": [], "source": {"path": CANONICAL_PATH, "format": "mode2-iso-file", "size_bytes": 1, "sha256": "a" * 64, "locator": {}}})
        self.assertTrue(SCHEMA_VALIDATOR.is_valid(positive_inventory))
        validate_manifest(positive_inventory)
        pack_root = os.environ.get("TOKIMEMO_CONTENT_PACK")
        if pack_root:
            actual = json.loads((Path(pack_root) / "manifest.json").read_text())
            self.assertTrue(SCHEMA_VALIDATOR.is_valid(actual), list(SCHEMA_VALIDATOR.iter_errors(actual)))
            self.assertEqual(len(actual["components"]), 24)
            validate_pack(pack_root)

    def test_schema_and_python_reject_dot_segments_unknown_keys_and_bad_inventory_sha(self):
        for unsafe_path in (".", "foo/.", "..", "foo/..", "../escape"):
            value = manifest({"id": "inventory-0", "version": 1, "kind": "inventory_entry", "status": "unsupported",
                "capabilities": [], "source": {"path": unsafe_path, "format": "mode2-iso-file", "size_bytes": 1, "sha256": "a" * 64, "locator": {}}})
            with self.subTest(path=unsafe_path):
                self.assertFalse(SCHEMA_VALIDATOR.is_valid(value))
                with self.assertRaisesRegex(ContentError, "not canonical"):
                    validate_manifest(value)
        value = manifest({"id": "inventory-0", "version": 1, "kind": "inventory_entry", "status": "unsupported",
            "capabilities": [], "source": {"path": CANONICAL_PATH, "format": "mode2-iso-file", "size_bytes": 1, "sha256": "a" * 64, "locator": {}}, "unknown": 1})
        self.assertFalse(SCHEMA_VALIDATOR.is_valid(value))
        with self.assertRaisesRegex(ContentError, "component .* fields"):
            validate_manifest(value)
        bad_hash = {"id": "inventory-0", "version": 1, "kind": "inventory_entry", "status": "unsupported", "capabilities": [],
            "source": {"path": CANONICAL_PATH, "format": "mode2-iso-file", "size_bytes": 1, "sha256": "bad", "locator": {}}}
        value = manifest(bad_hash)
        self.assertFalse(SCHEMA_VALIDATOR.is_valid(value))
        with self.assertRaisesRegex(ContentError, "source sha256 is invalid"):
            validate_manifest(value)

    def test_malformed_component_capability_type_is_rejected(self):
        component = _preview_component(); component["capabilities"] = 123
        with self.assertRaisesRegex(ContentError, "capabilities must be unique nonempty strings"):
            validate_manifest(manifest(component))

    def test_unsupported_component_cannot_back_route_capability(self):
        component = {"id": "inventory-0", "version": 1, "kind": "inventory_entry", "status": "unsupported",
            "capabilities": ["preview"], "source": {"path": CANONICAL_PATH, "format": "mode2-iso-file", "size_bytes": 1, "sha256": "a" * 64, "locator": {}}}
        value = manifest(component); value["route_capabilities"]["available"] = ["preview"]
        with self.assertRaisesRegex(ContentError, "cannot advertise capabilities"):
            validate_manifest(value)

    def test_wrong_canonical_cell_rejected(self):
        component = _preview_component(); component["id"] = "region-" + "0" * 64
        component["metadata"]["canonical_region_id"] = component["id"]
        value = manifest(component); value["route_capabilities"]["available"] = ["preview"]
        with self.assertRaisesRegex(ContentError, "canonical cell identity"):
            validate_manifest(value)

    def test_payload_symlink_escape_and_corrupt_hash_rejected(self):
        component = _preview_component()
        value = manifest(component); value["route_capabilities"]["available"] = ["preview"]
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp); (root / "payloads").mkdir(); (root / "payloads/preview.png").write_bytes(_png())
            (root / "manifest.json").write_text(json.dumps(value))
            with self.assertRaisesRegex(ContentError, "SHA-256 mismatch"):
                validate_pack(root)
            (root / "payloads/preview.png").unlink()
            outside = root.parent / "preview-outside.png"; outside.write_bytes(_png())
            (root / "payloads/preview.png").symlink_to(outside)
            with self.assertRaisesRegex(ContentError, "symlink"):
                validate_pack(root)


if __name__ == "__main__":
    unittest.main()
