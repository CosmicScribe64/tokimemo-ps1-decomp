"""T-39510: reject stale or altered generated source using synthetic fixtures."""
import tempfile
import unittest
from pathlib import Path

from tools.patch.audit_generated_sources import audit_sources
from tools.patch.text.c_literals import LiteralSite


class GeneratedSourcesAuditTest(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.source = 'void first() { show("old"); }\nvoid second() { show("old"); }\n'
        (self.root / 'sample.c').write_text(self.source)
        starts = [self.source.index('"old"'), self.source.rindex('"old"')]
        self.sites = [LiteralSite(str(i), 'sample.c', at, at + 5, b'old')
                      for i, at in enumerate(starts)]
        self.payloads = {'0': b'First', '1': b'Second'}
        self.snapshots = [self.root / 'live', self.root / 'frozen']
        for path in self.snapshots:
            path.mkdir()
            (path / 'sample.c').write_text(
                'void first() { show("First"); }\nvoid second() { show("Second"); }\n')

    def run_audit(self, **kwargs):
        return audit_sources(self.root, kwargs.get('sites', self.sites),
                             kwargs.get('payloads', self.payloads),
                             kwargs.get('skipped', set()), self.snapshots)

    def test_exact_source_generation(self):
        result = self.run_audit()
        self.assertEqual(result['active_literals'], 2)
        self.assertEqual(result['generated_files'], 1)
        self.assertEqual(result['snapshot_count'], 2)

    def test_swapped_calls_fail_even_when_both_payloads_are_present(self):
        (self.snapshots[1] / 'sample.c').write_text(
            'void first() { show("Second"); }\nvoid second() { show("First"); }\n')
        with self.assertRaisesRegex(ValueError, 'differs'):
            self.run_audit()

    def test_changed_non_literal_code_fails(self):
        file = self.snapshots[0] / 'sample.c'
        file.write_text(file.read_text().replace('show(', 'hide(', 1))
        with self.assertRaisesRegex(ValueError, 'differs'):
            self.run_audit()

    def test_missing_source_fails(self):
        (self.snapshots[1] / 'sample.c').unlink()
        with self.assertRaisesRegex(ValueError, 'Missing generated'):
            self.run_audit()

    def test_unlocated_translation_and_duplicate_sites_fail(self):
        with self.assertRaisesRegex(ValueError, 'no source site'):
            self.run_audit(payloads={**self.payloads, 'missing': b'Text'})
        with self.assertRaisesRegex(ValueError, 'Duplicate'):
            self.run_audit(sites=self.sites + self.sites[:1])

    def test_excluded_overlay_is_reported_without_requiring_generated_file(self):
        (self.root / 'unloaded.c').write_text('char *text = "old";\n')
        site = LiteralSite('unused', 'unloaded.c', 13, 18, b'old')
        result = self.run_audit(sites=self.sites + [site],
                                payloads={**self.payloads, 'unused': b'Unused'},
                                skipped={'unloaded.c'})
        self.assertEqual(result['excluded_literal_ids'], ['unused'])
        self.assertEqual(result['active_literals'], 2)

    def test_vacuous_audit_fails(self):
        with self.assertRaisesRegex(ValueError, 'requires active'):
            self.run_audit(payloads={})


if __name__ == '__main__':
    unittest.main()
