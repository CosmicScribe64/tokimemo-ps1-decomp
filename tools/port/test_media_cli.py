"""Clean-process command-line regressions; real original inputs remain optional and external."""
import json
import os
import shutil
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]


def clean_cli(*arguments):
    env = dict(os.environ)
    for name in ('PYTHONPATH', 'PYTHONHOME', 'TOKIMEMO_ROOT', 'TOKIMEMO_REPOSITORY_ROOT'):
        env.pop(name, None)
    return subprocess.run([sys.executable, *arguments], cwd=ROOT, env=env,
                          capture_output=True, text=True, timeout=90)


class CleanEntryPointTests(unittest.TestCase):
    def test_package_import_uses_its_own_reader_namespace(self):
        result = clean_cli('-c', 'from tools.assets import media_mdec_export as m; '
                           'assert m.export_mdec.__module__ == "tools.assets.image_mdec"; '
                           'assert m.build_profile_context.__module__ == "tools.assets.bs_cohort_profile"')
        self.assertEqual(result.returncode, 0, result.stderr)

    def test_direct_and_module_exporter_cli_help(self):
        for arguments in ((str(ROOT/'tools/assets/media_mdec_export.py'), '--help'),
                          ('-m', 'tools.assets.media_mdec_export', '--help')):
            with self.subTest(arguments=arguments):
                result = clean_cli(*arguments)
                self.assertEqual(result.returncode, 0, result.stderr)
                self.assertIn('--bs-command7', result.stdout)


@unittest.skipUnless(os.environ.get('TOKIMEMO_MEDIA_V2'), 'set original-input paths for clean real CLI tests')
class OriginalCleanCliTests(unittest.TestCase):
    def setUp(self):
        self.pack = Path(os.environ['TOKIMEMO_MEDIA_V2'])
        self.expected = json.loads((self.pack/'manifest.json').read_text())
        cache = ROOT/'.asset-cache/port'
        cache.mkdir(parents=True, exist_ok=True)
        self.temp = Path(tempfile.mkdtemp(prefix='test-clean-cli-', dir=cache))

    def tearDown(self): shutil.rmtree(self.temp)

    def test_documented_port_cli_creates_exact_validated_pack_without_pythonpath(self):
        target = self.temp/'pack'
        result = clean_cli(str(ROOT/'tools/port/school_day_media.py'),
            '--input', os.environ['TOKIMEMO_IMPORT_INPUT'],
            '--media-manifest', os.environ['TOKIMEMO_IMPORT_MEDIA'],
            '--native-profile', os.environ['TOKIMEMO_IMPORT_NATIVE'], '--output', str(target))
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual((target/'manifest.json').read_bytes(), (self.pack/'manifest.json').read_bytes())
        from tools.port.validate_school_day_media import validate_pack
        self.assertEqual(validate_pack(str(target))['components'], self.expected['components'])
        for path in (self.pack/'assets').glob('*.png'):
            self.assertEqual((target/'assets'/path.name).read_bytes(), path.read_bytes())

    def test_direct_mdec_cli_exports_matching_command7_images_without_pythonpath(self):
        cache = ROOT/'.asset-cache/media'
        cache.mkdir(parents=True, exist_ok=True)
        scratch = Path(tempfile.mkdtemp(prefix='test-clean-mdec-', dir=cache))
        try:
            target = scratch/'export'
            result = clean_cli(str(ROOT/'tools/assets/media_mdec_export.py'),
                os.environ['TOKIMEMO_IMPORT_INPUT'], '--bs-command7',
                '--baseline-media', os.environ['TOKIMEMO_IMPORT_MEDIA'],
                '--only-source', 'CDROM/BS/C_01_44.BS', '--repository-root', str(ROOT),
                '--output', str(target))
            self.assertEqual(result.returncode, 0, result.stderr)
            doc = json.loads((target/'manifest.json').read_text())
            bg = self.expected['components']['command_7_background']
            record = next(a for a in doc['assets'] if a['source']['path'] == bg['source']['path'])
            self.assertEqual(record['status'], 'exported')
            self.assertEqual({o['sha256'] for o in record['outputs']},
                             {bg[k]['sha256'] for k in ('image','stp_mask','padded_image','padded_stp_mask')})
            self.assertEqual(record['native_cd_read_extent'], bg['native_read_extent'])
        finally: shutil.rmtree(scratch)


if __name__ == '__main__': unittest.main()
