import hashlib
import struct
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools/port"))
from school_day_tables import (  # noqa: E402
    EXPECTED_LOAD_ADDRESS, EXPECTED_TEXT_SIZE, HEADER_SIZE, TABLES,
    TableExportError, _check_export_path, _read_descriptor, _validate_values, parse_psx_exe,
    sha256, validate_table_contract,
)


def fake_exe():
    data = bytearray(HEADER_SIZE + EXPECTED_TEXT_SIZE)
    data[:8] = b"PS-X EXE"
    struct.pack_into("<II", data, 0x10, EXPECTED_LOAD_ADDRESS, EXPECTED_LOAD_ADDRESS + 0x100)
    struct.pack_into("<II", data, 0x18, EXPECTED_LOAD_ADDRESS, EXPECTED_TEXT_SIZE)
    return data


def shaped_zero(shape):
    if len(shape) == 1:
        return [0] * shape[0]
    return [shaped_zero(shape[1:]) for _ in range(shape[0])]


class SchoolDayTableTests(unittest.TestCase):
    def test_psx_header_and_vaddr_to_file_offset(self):
        exe = fake_exe()
        header = parse_psx_exe(bytes(exe), sha256(exe))
        self.assertEqual(header["payload_file_offset"], 0x800)
        self.assertEqual(header["loaded_vaddr_start"], EXPECTED_LOAD_ADDRESS)
        self.assertEqual(header["loaded_vaddr_end_exclusive"], EXPECTED_LOAD_ADDRESS + EXPECTED_TEXT_SIZE)

    def test_little_endian_signed_multidimensional_decode(self):
        exe = fake_exe()
        desc = {"type": "s8", "shape": [2, 2], "vaddr": EXPECTED_LOAD_ADDRESS + 4}
        offset = HEADER_SIZE + 4
        exe[offset:offset + 4] = struct.pack("<bbbb", -128, -1, 0, 127)
        header = parse_psx_exe(bytes(exe), sha256(exe))
        values, raw, file_offset, size = _read_descriptor(bytes(exe), header, desc)
        self.assertEqual(values, [[-128, -1], [0, 127]])
        self.assertEqual((file_offset, size, raw), (offset, 4, bytes(exe[offset:offset + 4])))

    def test_bad_signature_digest_and_short_header_rejected(self):
        exe = fake_exe()
        with self.assertRaisesRegex(TableExportError, "signature"):
            parse_psx_exe(b"NOPE" + bytes(exe[4:]), sha256(exe))
        with self.assertRaisesRegex(TableExportError, "SHA-256"):
            parse_psx_exe(bytes(exe), "0" * 64)
        with self.assertRaisesRegex(TableExportError, "truncated"):
            parse_psx_exe(b"short", sha256(b"short"))

    def test_header_mapping_and_table_bounds_or_alignment_rejected(self):
        exe = fake_exe()
        struct.pack_into("<I", exe, 0x18, EXPECTED_LOAD_ADDRESS + 4)
        with self.assertRaisesRegex(TableExportError, "loaded address"):
            parse_psx_exe(bytes(exe), sha256(exe))
        exe = fake_exe()
        header = parse_psx_exe(bytes(exe), sha256(exe))
        with self.assertRaisesRegex(TableExportError, "unaligned"):
            _read_descriptor(bytes(exe), header, {"type": "u16", "shape": [1], "vaddr": EXPECTED_LOAD_ADDRESS + 1})
        with self.assertRaisesRegex(TableExportError, "outside"):
            _read_descriptor(bytes(exe), header, {"type": "s32", "shape": [2], "vaddr": header["loaded_vaddr_end_exclusive"] - 4})

    def test_shape_and_integer_range_are_enforced(self):
        _validate_values([[0, 127], [-128, -1]], [2, 2], "s8")
        for bad in ([[0], [1, 2]], [[0, True], [1, 2]], [[0, 128], [1, 2]]):
            with self.subTest(bad=bad), self.assertRaises(TableExportError):
                _validate_values(bad, [2, 2], "s8")

    def test_symlinked_port_cache_is_rejected_before_any_publication(self):
        with tempfile.TemporaryDirectory(prefix="route-tables-output-guard-") as temp:
            root = Path(temp) / "repo"
            cache = root / ".asset-cache"
            cache.mkdir(parents=True)
            outside = Path(temp) / "outside"
            outside.mkdir()
            (cache / "port").symlink_to(outside, target_is_directory=True)
            with self.assertRaisesRegex(TableExportError, "symlink"):
                _check_export_path(root / ".asset-cache/port/escaped", root)
            self.assertEqual(list(outside.iterdir()), [])

    def test_existing_destination_and_dot_segment_are_rejected(self):
        with tempfile.TemporaryDirectory(prefix="route-tables-output-guard-") as temp:
            root = Path(temp) / "repo"
            port = root / ".asset-cache/port"
            port.mkdir(parents=True)
            existing = port / "existing"
            existing.mkdir()
            with self.assertRaisesRegex(TableExportError, "overwrite"):
                _check_export_path(existing, root)
            with self.assertRaisesRegex(TableExportError, "noncanonical"):
                _check_export_path(str(port) + "/../outside", root)

    def test_symlinked_repository_ancestor_and_nested_publisher_are_rejected(self):
        with tempfile.TemporaryDirectory(prefix="route-tables-output-guard-") as temp:
            root = Path(temp) / "repo"
            port = root / ".asset-cache/port"
            port.mkdir(parents=True)
            outside = Path(temp) / "outside"
            outside.mkdir()
            alias = Path(temp) / "repo-alias"
            alias.symlink_to(root, target_is_directory=True)
            with self.assertRaisesRegex(TableExportError, "symlink"):
                _check_export_path(alias / ".asset-cache/port/escaped", alias)
            (port / "publisher").symlink_to(outside, target_is_directory=True)
            with self.assertRaisesRegex(TableExportError, "symlink"):
                _check_export_path(port / "publisher/escaped", root)
            self.assertEqual(list(outside.iterdir()), [])

    def test_bundle_requires_all_named_tables_and_validates_raw_hashes(self):
        tables = {name: shaped_zero(desc["shape"]) for name, desc in TABLES.items()}
        proofs = {}
        for name, desc in TABLES.items():
            width, fmt, _low, _high = {"s8": (1,"b",0,0), "u8": (1,"B",0,0), "s16": (2,"h",0,0), "u16": (2,"H",0,0), "s32": (4,"i",0,0)}[desc["type"]]
            count = 1
            for dim in desc["shape"]: count *= dim
            proofs[name] = {"symbol": desc["symbol"], "virtual_address": f"0x{desc['vaddr']:08X}",
                "file_offset": HEADER_SIZE + desc["vaddr"] - EXPECTED_LOAD_ADDRESS,
                "size_bytes": width * count, "raw_sha256": sha256(struct.pack("<" + fmt * count, *([0] * count))),
                "element_type": desc["type"], "shape": desc["shape"], "source_evidence": [{"path": "synthetic", "sha256": "a" * 64, "reference": "synthetic"}]}
        bundle = {"schema": "tokimemo.port-rule-tables.v1", "schema_version": 1, "tables": tables,
            "provenance": {"reader": "tools.assets.input.open_input", "reader_source_sha256": "c" * 64, "input": {"source_sha256": "a" * 64, "entry_path": "SLPM_86.053"},
                "release_executable": {"sha256": "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae",
                    "release": "Tokimeki Memorial - Forever with You (Japan), PlayStation the Best",
                    "psx_exe_header": {"magic": "PS-X EXE", "load_address": EXPECTED_LOAD_ADDRESS,
                        "text_size_bytes": EXPECTED_TEXT_SIZE, "payload_file_offset": HEADER_SIZE}},
                "tables": proofs, "exporter": {"name": "tools/port/school_day_tables.py", "version": 1, "source_sha256": "b" * 64}}}
        validate_table_contract(bundle)
        bundle["tables"]["command_deltas"][0][0] = 1
        with self.assertRaisesRegex(TableExportError, "raw-byte digest"):
            validate_table_contract(bundle)


if __name__ == "__main__":
    unittest.main(verbosity=2)
