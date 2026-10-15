"""Synthetic checks for the exhaustive census and failure retention (T-14500)."""
import json
import os
from pathlib import Path
import tempfile
from types import SimpleNamespace
import unittest
from unittest.mock import patch

import exhaustive_drafts as drafts


class ExhaustiveDraftTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.old = os.getcwd()
        os.chdir(self.tmp.name)
        drafts.BASE.mkdir(parents=True)
        (drafts.BASE / 'environment.json').write_text('{}')

    def tearDown(self):
        os.chdir(self.old)
        self.tmp.cleanup()

    def source(self, text, name='game.c', unit='TT'):
        src = Path(name)
        src.write_text(text)
        asm = Path(name + '-asm')
        asm.mkdir()
        for func in ('func_80132000', 'func_80132100'):
            (asm / (func + '.s')).write_text('')
        return SimpleNamespace(src=src, nonmatchings=asm, unit=unit)

    def test_census_keeps_guarded_and_tt_and_missing_size(self):
        c = self.source('#ifdef NON_MATCHING\nvoid func_80132000(void) {}\n#else\n'
                        'INCLUDE_ASM("asm/test", func_80132000);\n#endif\n'
                        'INCLUDE_ASM("asm/test", func_80132100);\n')
        with patch.object(drafts.srcscan, 'c_files', return_value=[c]), \
             patch.object(drafts.srcscan, 'nonmatching_size', side_effect=[('func_80132000', 4), None]):
            rows = drafts.census()
        self.assertEqual(len(rows), 2)
        self.assertTrue(rows[0]['guarded'])
        self.assertFalse(rows[1]['guarded'])
        self.assertEqual(rows[0]['unit'], 'TT')
        self.assertIsNone(rows[1]['size'])

    def test_duplicate_scope_fails_loudly(self):
        c = self.source('INCLUDE_ASM("asm/test", func_80132000);\n'
                        'INCLUDE_ASM("asm/test", func_80132000);\n')
        with patch.object(drafts.srcscan, 'c_files', return_value=[c]), \
             patch.object(drafts.srcscan, 'nonmatching_size', return_value=('func_80132000', 4)):
            with self.assertRaises(AssertionError):
                drafts.census()

    def test_generation_failure_is_saved_and_source_unchanged(self):
        original = 'INCLUDE_ASM("asm/test", func_80132000);\n'
        c = self.source(original)
        row = dict(unit=c.unit, src=str(c.src), func='func_80132000', scope='TT:func_80132000',
                   size=4, guarded=False, source_hash=drafts.digest(original))
        with patch.object(drafts.m2c, 'run_m2c', side_effect=RuntimeError('missing type Example')) as run:
            result = drafts.process(row)
        self.assertEqual(run.call_count, 2)
        self.assertEqual(result['status'], 'failed')
        self.assertIn('missing type Example', result['reason'])
        self.assertEqual(c.src.read_text(), original)
        folder = drafts.BASE / 'TT/func_80132000'
        self.assertEqual(json.loads((folder / 'result.json').read_text())['status'], 'failed')
        self.assertIn('missing type Example', (folder / 'diagnostics.txt').read_text())


if __name__ == '__main__':
    unittest.main()
