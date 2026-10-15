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


    def implicit_return_case(self, explicit=False):
        func = 'func_80132000'
        original = ('void func_80132000(void);\n' if explicit else '') + (
            'void earlier(void) { func_80132000(); }\n'
            'INCLUDE_ASM("asm/test", func_80132000);\n')
        c = self.source(original)
        row = dict(unit=c.unit, src=str(c.src), func=func, scope='TT:'+func,
                   size=4, guarded=False, source_hash=drafts.digest(original))
        with patch.object(drafts.m2c, 'run_m2c', return_value='void func_80132000(void) {\n}\n'), \
             patch.object(drafts.funcloc, 'locate', return_value=c), \
             patch.object(drafts.funcdiff, 'expected_object', return_value=('expected.o', None)), \
             patch.object(drafts.funcdiff, 'resolved_functions', return_value={func: ['same']}), \
             patch.object(drafts.funcdiff, 'keys', side_effect=lambda x: x), \
             patch.object(drafts.order_search, 'compile_variant', side_effect=[
                 SystemExit("redeclaration of 'func_80132000'; Incompatible function return type"),
                 'candidate.o']) as compile_mock:
            result = drafts.process(row)
        self.assertEqual(c.src.read_text(), original)
        return result, compile_mock.call_count, (drafts.BASE/'TT'/func/'body.c').read_text()

    def test_implicit_int_return_is_repaired(self):
        result, calls, body = self.implicit_return_case()
        self.assertEqual(result['status'], 'candidate-zero')
        self.assertEqual(calls, 2)
        self.assertTrue(body.startswith('s32 func_80132000'))

    def test_explicit_prototype_prevents_implicit_int_guess(self):
        result, calls, body = self.implicit_return_case(explicit=True)
        self.assertEqual(result['status'], 'failed')
        self.assertEqual(calls, 1)
        self.assertTrue(body.startswith('void func_80132000'))


    def test_failed_guard_regeneration_preserves_original(self):
        func='func_80132000'
        original=('#ifdef NON_MATCHING\nvoid func_80132000(void) { bad_old_body(); }\n'
                  '#else\nINCLUDE_ASM("asm/test", func_80132000);\n#endif\n')
        c=self.source(original)
        row=dict(unit='TT',src=str(c.src),func=func,scope='TT:'+func,size=4,guarded=True,
                 source_hash=drafts.digest(original))
        with patch.dict(os.environ,{'TOKIMEMO_REGENERATE_GUARDS':'1'}), \
             patch.object(drafts.m2c,'run_m2c',return_value='void func_80132000(void) {\n}\n') as generate, \
             patch.object(drafts.funcloc,'locate',return_value=c), \
             patch.object(drafts.funcdiff,'expected_object',return_value=('expected.o',None)), \
             patch.object(drafts.funcdiff,'resolved_functions',return_value={func:['same']}), \
             patch.object(drafts.funcdiff,'keys',side_effect=lambda x:x), \
             patch.object(drafts.order_search,'compile_variant',return_value='candidate.o'):
            result=drafts.process(row)
        self.assertEqual(generate.call_count,1)
        self.assertEqual(result['status'],'candidate-zero')
        self.assertEqual(result['variant'],'regenerated-default')
        self.assertEqual(c.src.read_text(),original)
        context=(drafts.BASE/'TT'/func/'context.c').read_text()
        self.assertNotIn('bad_old_body',context)
        self.assertNotIn('INCLUDE_ASM',context)


if __name__ == '__main__':
    unittest.main()
