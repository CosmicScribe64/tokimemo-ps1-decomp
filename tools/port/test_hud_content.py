"""Synthetic export-boundary tests; no game bytes or RAM fixtures are required."""
import importlib.util
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

SOURCE = next(parent / 'hud_content.py' for parent in Path(__file__).resolve().parents
              if (parent / 'hud_content.py').is_file())
SPEC = importlib.util.spec_from_file_location('hud_content_under_test', SOURCE)
hud = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(hud)


class ExportBoundaryTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.root = Path(self.temp.name)
        self.addCleanup(self.temp.cleanup)

    def test_checked_input_rejects_changed_bytes_size_and_symlinks(self):
        path = self.root / 'input.bin'
        path.write_bytes(b'original fixture')
        expected = hud.digest(path.read_bytes())
        self.assertEqual(hud.read_checked(path, expected, 16), b'original fixture')
        with self.assertRaises(ValueError):
            hud.read_checked(path, expected, 15)
        path.write_bytes(b'modified fixture')
        with self.assertRaises(ValueError):
            hud.read_checked(path, expected, 16)
        link = self.root / 'linked.bin'
        link.symlink_to(path)
        with self.assertRaises(ValueError):
            hud.read_checked(link, hud.digest(path.read_bytes()), 16)

    def test_parent_symlink_is_rejected(self):
        real = self.root / 'real'
        real.mkdir()
        (real / 'input.bin').write_bytes(b'x')
        (self.root / 'alias').symlink_to(real, target_is_directory=True)
        with self.assertRaises(ValueError):
            hud.read_checked(self.root / 'alias/input.bin', hud.digest(b'x'), 1)

    def test_output_is_versioned_ignored_and_never_existing(self):
        output = self.root / '.asset-cache/port/hud-v1'
        self.assertEqual(hud.output_directory(self.root, output), output)
        for bad in [self.root / 'port/assets', self.root / '.asset-cache/port',
                    self.root / '.asset-cache/port/hud-v1/nested']:
            with self.assertRaises(ValueError):
                hud.output_directory(self.root, bad)
        output.mkdir(parents=True)
        (output / 'immutable').write_bytes(b'keep')
        with self.assertRaises(ValueError):
            hud.output_directory(self.root, output)
        self.assertEqual((output / 'immutable').read_bytes(), b'keep')

    def test_json_duplicate_keys_and_non_objects_are_rejected(self):
        self.assertEqual(hud.json_object(b'{"version":1}'), {'version': 1})
        for raw in [b'{"version":1,"version":2}', b'[]']:
            with self.assertRaises(ValueError):
                hud.json_object(raw)

    def test_manifest_paths_cannot_escape(self):
        self.assertEqual(hud.relative_path('assets/a.png'), 'assets/a.png')
        for path in ['../a.png', '/absolute.png', 'assets/../a.png', 'assets\\a.png', './a.png']:
            with self.assertRaises(ValueError):
                hud.relative_path(path)

    def test_room_manifest_requires_reviewed_profile_hash(self):
        path = self.root / 'manifest.json'
        path.write_text('{}')
        with self.assertRaises(ValueError):
            hud.verified_room(path, hud.digest(path.read_bytes()))

    def test_source_strings_require_exact_verified_label_extent(self):
        repo = self.root
        for assembly, _, _ in hud.STRING_LABELS.values():
            path = repo / assembly
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text('synthetic witness')

        def spans(path, selected, binary):
            for assembly, label, address in hud.STRING_LABELS.values():
                if path == selected / assembly:
                    raw = b'\x81\x69' if label == 'D_800AFB74' else b'\x82\x4f' * 13
                    yield {'label': label, 'address': address, 'raw': raw + b'\0\0'}

        result = hud.source_strings(repo, b'', spans)
        self.assertEqual(result['narration_prefix']['raw_hex'], '8169')
        with self.assertRaises(ValueError):
            hud.source_strings(repo, b'', lambda *args: [])

    def test_optimized_cli_rejects_invalid_original_before_publication(self):
        repo = self.root / 'repo'
        reader = repo / 'tools/assets/image_packs.py'
        reader.parent.mkdir(parents=True)
        reader.write_text('raise RuntimeError("must not import before source validation")')
        bad = repo / 'bad.bin'
        bad.write_bytes(b'not the original')
        output = repo / '.asset-cache/port/invalid-version'
        arguments = ['--repository', str(repo), '--executable', str(bad), '--mainrom', str(bad),
                     '--calendar', str(bad), '--room-manifest', str(bad),
                     '--room-manifest-sha256', hud.ROOM_MANIFEST_SHA, '--output', str(output)]
        result = subprocess.run([sys.executable, '-O', str(SOURCE), *arguments], capture_output=True, text=True)
        self.assertEqual(result.returncode, 2, result.stdout + result.stderr)
        self.assertIn('input size differs', result.stderr)
        self.assertNotIn('must not import', result.stderr)
        self.assertFalse(output.exists())


if __name__ == '__main__':
    unittest.main()
