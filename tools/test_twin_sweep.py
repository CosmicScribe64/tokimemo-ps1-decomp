#!/usr/bin/env python3
"""Unit tests for tools/twin_sweep.py on synthetic asm and C (no game data).

Run: tools/docker.sh python3 tools/test_twin_sweep.py
"""
import contextlib
import io
import os
import sys
import tempfile
import unittest
from unittest import mock

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dupes  # noqa: E402
import twin_sweep as ts  # noqa: E402
from test_dupes import SRC_C, TGT_C, func_asm, make_repo  # noqa: E402
from test_neardupes import SRC_ASM, TGT_ASM  # noqa: E402
from test_neardupes import make_repo as near_repo  # noqa: E402
from test_neardupes import func_asm as near_asm  # noqa: E402


def add_exact_twin(r, unit='CCC', name='ftgt2'):
    """A second unmatched twin of the exact repo's donor, in another overlay file."""
    r.write('asm/ovl/%s/nonmatchings/%s/%s.s' % (unit, unit, name),
            func_asm(name, 'D_80152220', 'D_80152224', 'func_80143333'))
    r.write('src/ovl/%s.c' % unit, '#include "common.h"\n#include "ovl/%s.h"\n\n' % unit
            + 'INCLUDE_ASM("asm/ovl/%s/nonmatchings/%s", %s);\n' % (unit, unit, name))
    r.write('include/ovl/%s.h' % unit, '#ifndef %s_H\n#define %s_H\n#include "common.h"\n#include "game.h"\n#endif\n'
            % (unit, unit))


def sweep(r, lenient=True):
    return ts.sweep(r.root, lenient)


class SweepTests(unittest.TestCase):
    def test_exact_twin_is_planned_with_unit_donor_and_size(self):
        r = make_repo(SRC_C, TGT_C)
        self.addCleanup(r.close)
        rows, skipped, stats = sweep(r)
        self.assertEqual(skipped, {})
        self.assertEqual(len(rows), 1)
        row = rows[0]
        self.assertEqual((row['mode'], row['unit'], row['name']), ('exact', 'BBB', 'ftgt'))
        self.assertEqual((row['donor_unit'], row['donor'], row['bytes']), ('AAA', 'fsrc', 0x20))
        self.assertEqual(row['file'], 'src/ovl/BBB.c')
        self.assertEqual(stats['functions'], 2)
        self.assertEqual(stats['matched'], 1)
        self.assertEqual(ts.describe(row), 'exact   BBB/ftgt <- AAA/fsrc  32 B  src/ovl/BBB.c')

    def test_near_twin_is_planned_with_its_constants(self):
        r = near_repo(SRC_ASM, TGT_ASM)
        self.addCleanup(r.close)
        rows, skipped, _stats = sweep(r)
        self.assertEqual([(x['mode'], x['name'], x['ndiff'], x['bytes']) for x in rows], [('near', 'ftgt', 3, 0x28)])
        self.assertIn('[3 consts]', ts.describe(rows[0]))

    def test_conflicting_declaration_is_planned_only_by_the_lenient_pass(self):
        r = make_repo(SRC_C, TGT_C, tgt_hdr='extern u8 D_80151110;\n')
        self.addCleanup(r.close)
        rows, skipped, _stats = sweep(r, lenient=False)
        self.assertEqual(rows, [])
        self.assertIn('declares D_80151110 differently', skipped['BBB/ftgt']['reasons'][0])
        rows, skipped, _stats = sweep(r)
        self.assertEqual([(x['mode'], x['via']) for x in rows], [('lenient', 'exact')])
        self.assertEqual(skipped, {})
        self.assertIn('[via exact]', ts.describe(rows[0]))

    def test_strict_plans_keep_their_mode_when_the_lenient_pass_runs(self):
        r = make_repo(SRC_C, TGT_C)
        self.addCleanup(r.close)
        add_exact_twin(r)
        r.write('include/ovl/CCC.h', '#ifndef CCC_H\n#define CCC_H\n#include "common.h"\n#include "game.h"\n'
                'extern u8 D_80152220;\n#endif\n')
        rows, _skipped, _stats = sweep(r)
        self.assertEqual(sorted((x['name'], x['mode']) for x in rows), [('ftgt', 'exact'), ('ftgt2', 'lenient')])
        self.assertEqual([x['mode'] for x in rows], ['exact', 'lenient'])

    def test_exact_before_near_and_each_target_once(self):
        r = near_repo(SRC_ASM, TGT_ASM)
        self.addCleanup(r.close)
        add_exact_twin(r)
        r.write('asm/ovl/CCC/nonmatchings/CCC/ftgt2.s', near_asm('ftgt2', 'func_80140000', 5, 0x30, 0x8010, 0))
        rows, _skipped, _stats = sweep(r)
        self.assertEqual([(x['mode'], x['name'], x['donor']) for x in rows],
                         [('exact', 'ftgt2', 'fsrc'), ('near', 'ftgt', 'fsrc')])

    def test_wider_shape_twin_is_reported_but_not_planned(self):
        from test_neardupes import hexw, w
        src = SRC_ASM.replace(hexw(w(0x2B, 29, 8, 4)), hexw(w(0x2B, 4, 8, 8)))
        tgt = TGT_ASM.replace(hexw(w(0x2B, 29, 8, 4)), hexw(w(0x2B, 4, 8, 12)))
        r = near_repo(src, tgt)
        self.addCleanup(r.close)
        rows, skipped, stats = sweep(r)
        self.assertEqual((rows, stats['wide']), ([], [('BBB', 'ftgt', 0x28)]))
        self.assertEqual(list(skipped), [])

    def test_target_without_a_matched_twin_is_not_planned(self):
        r = make_repo(SRC_C, TGT_C)
        self.addCleanup(r.close)
        r.write('src/ovl/AAA.c', '#include "common.h"\n#include "ovl/AAA.h"\n\n'
                'INCLUDE_ASM("asm/ovl/AAA/matchings/AAA", fsrc);\n')
        rows, skipped, stats = sweep(r)
        self.assertEqual((rows, skipped, stats['matched']), ([], {}, 0))

    def test_exact_view_leaves_the_near_functions_alone(self):
        r = near_repo(SRC_ASM, TGT_ASM)
        self.addCleanup(r.close)
        funcs = dupes.load_funcs(r.root, near=True)
        view = ts.exact_view(funcs)
        self.assertTrue(all(v.key == f.exact and f.key != f.exact for v, f in zip(view, funcs)))
        self.assertEqual(len({f.key for f in funcs}), 1)
        self.assertEqual(len({v.key for v in view}), 2)


class FilesTests(unittest.TestCase):
    def test_file_spec_of_src_paths(self):
        self.assertEqual(ts.file_spec('src/ovl/GEKO/8013C000.c'), 'GEKO/8013C000')
        self.assertEqual(ts.file_spec('src/ovl/TEL.c'), 'TEL')
        self.assertEqual(ts.file_spec('/w/tokimemo/src/main/80061710.c'), 'main/80061710')
        self.assertEqual(ts.file_spec('GEKO/8013C000'), 'GEKO/8013C000')
        self.assertEqual(ts.file_spec('TEL.c'), 'TEL')

    def test_parse_files_accepts_names_paths_and_list_files(self):
        self.assertEqual(ts.parse_files(None), '')
        self.assertEqual(ts.parse_files('TEL, 80041000 src/ovl/GEKO.c'), 'TEL,80041000,GEKO')
        self.assertEqual(ts.parse_files('all'), ts.ALL)
        with tempfile.NamedTemporaryFile('w', suffix='.txt', delete=False) as fh:
            fh.write('src/ovl/DATE.c\nsrc/main/80061710.c\n\n')
        self.addCleanup(os.remove, fh.name)
        self.assertEqual(ts.parse_files('TEL,@' + fh.name), 'TEL,DATE,main/80061710')

    ROWS = [dict(unit='TEL', name='a', file='src/ovl/TEL.c'), dict(unit='DATE', name='b', file='src/ovl/DATE.c'),
            dict(unit='main', name='c', file='src/main/80061710.c'),
            dict(unit='GEKO', name='a', file='src/ovl/GEKO/8013C000.c')]
    SKIPPED = {'TEL/z': dict(unit='TEL', name='z', file='src/ovl/TEL.c'),
               'DATE/y': dict(unit='DATE', name='y', file='src/ovl/DATE.c')}

    def names(self, **kw):
        rows, skipped = ts.select(self.ROWS, self.SKIPPED, **kw)
        return [r['unit'] + '/' + r['name'] for r in rows], sorted(skipped)

    def test_select_by_file_exclude_unit_and_function(self):
        self.assertEqual(self.names()[0], ['TEL/a', 'DATE/b', 'main/c', 'GEKO/a'])
        self.assertEqual(self.names(files='TEL,main/80061710'), (['TEL/a', 'main/c'], ['TEL/z']))
        self.assertEqual(self.names(files=ts.ALL, exclude='DATE,geko/8013c000'), (['TEL/a', 'main/c'], ['TEL/z']))
        self.assertEqual(self.names(exclude='80061710')[0], ['TEL/a', 'DATE/b', 'GEKO/a'])
        self.assertEqual(self.names(units=['DATE', 'GEKO'])[0], ['DATE/b', 'GEKO/a'])
        self.assertEqual(self.names(names=['a'])[0], ['TEL/a', 'GEKO/a'])

    def test_a_unit_name_does_not_select_its_per_object_files(self):
        self.assertEqual(self.names(files='GEKO')[0], [])       # GEKO/8013C000 is a different spec


def run_main(root, *args):
    out, err = io.StringIO(), io.StringIO()
    with contextlib.redirect_stdout(out), contextlib.redirect_stderr(err):
        rc = ts.main(['--root', root, '--jobs', '0'] + list(args))
    return rc, out.getvalue()


class CommandTests(unittest.TestCase):
    def setUp(self):
        self.r = make_repo(SRC_C, TGT_C)
        add_exact_twin(self.r)
        self.addCleanup(self.r.close)

    def test_dry_run_prints_plan_and_changes_nothing(self):
        before = (self.r.read('src/ovl/BBB.c'), self.r.read('src/ovl/CCC.c'))
        rc, out = run_main(self.r.root)
        self.assertEqual(rc, 0)
        self.assertIn('planned 2 copies, 64 bytes', out)
        self.assertIn('by mode: exact 2/64B, near 0/0B, lenient 0/0B', out)
        self.assertIn('PLAN exact   BBB/ftgt <- AAA/fsrc', out)
        self.assertIn('PLAN exact   CCC/ftgt2 <- AAA/fsrc', out)
        self.assertEqual(before, (self.r.read('src/ovl/BBB.c'), self.r.read('src/ovl/CCC.c')))

    def test_files_limit_the_dry_run_plan(self):
        _rc, out = run_main(self.r.root, '--files', 'src/ovl/CCC.c')
        self.assertIn('planned 1 copies', out)
        self.assertNotIn('BBB/ftgt', out)

    def test_list_files_prints_only_the_target_files(self):
        _rc, out = run_main(self.r.root, '--list-files', '--exclude', 'BBB')
        self.assertEqual(out, 'CCC\n')
        _rc, out = run_main(self.r.root, '--list-files')
        self.assertEqual(out, 'BBB,CCC\n')

    def test_apply_needs_files(self):
        for args in (['--apply'], ['--apply', '--exclude', 'BBB'], ['--apply', '--files', 'BBB', '--list-files']):
            with self.subTest(args=args), contextlib.redirect_stderr(io.StringIO()):
                with self.assertRaises(SystemExit) as e:
                    run_main(self.r.root, *args)
                self.assertEqual(e.exception.code, 2)

    def test_apply_writes_only_the_given_files(self):
        builds = []
        with mock.patch.object(dupes, 'build_ok', lambda root, unit: (builds.append(unit) or True, '')):
            _rc, out = run_main(self.r.root, '--apply', '--files', 'src/ovl/BBB.c')
        self.assertIn('applied 1 of 1 copies (32 bytes); rejected 0, held 0, units skipped 0', out)
        self.assertNotIn('INCLUDE_ASM', self.r.read('src/ovl/BBB.c'))
        self.assertIn('void ftgt(void)', self.r.read('src/ovl/BBB.c'))
        self.assertIn('INCLUDE_ASM', self.r.read('src/ovl/CCC.c'))
        self.assertEqual(builds, ['BBB', 'BBB'])             # sha1 of the unit before and after

    def test_apply_all_minus_exclude(self):
        with mock.patch.object(dupes, 'build_ok', lambda root, unit: (True, '')):
            _rc, out = run_main(self.r.root, '--apply', '--files', 'ALL', '--exclude', 'src/ovl/BBB.c')
        self.assertIn('applied 1 of 1', out)
        self.assertIn('INCLUDE_ASM', self.r.read('src/ovl/BBB.c'))
        self.assertNotIn('INCLUDE_ASM', self.r.read('src/ovl/CCC.c'))

    def test_a_copy_that_does_not_match_is_taken_back(self):
        calls = []

        def build(root, unit):
            calls.append(unit)
            return len(calls) == 1, 'sha1 mismatch'          # fine before the sweep, not after it
        with mock.patch.object(dupes, 'build_ok', build):
            _rc, out = run_main(self.r.root, '--apply', '--files', 'BBB')
        self.assertIn('applied 0 of 1 copies (0 bytes); rejected 1', out)
        self.assertIn('INCLUDE_ASM', self.r.read('src/ovl/BBB.c'))
        self.assertNotIn('D_80151110', self.r.read('include/ovl/BBB.h'))

    def test_a_unit_that_does_not_build_before_the_sweep_is_left_alone(self):
        with mock.patch.object(dupes, 'build_ok', lambda root, unit: (False, 'broken')):
            _rc, out = run_main(self.r.root, '--apply', '--files', 'BBB,CCC')
        self.assertIn('SKIP UNIT BBB', out)
        self.assertIn('applied 0 of 0 copies', out)
        self.assertIn('INCLUDE_ASM', self.r.read('src/ovl/BBB.c'))
        self.assertIn('INCLUDE_ASM', self.r.read('src/ovl/CCC.c'))

    def test_jobs_become_a_ninja_argument(self):
        with mock.patch.object(dupes, 'NINJA_ARGS', []):
            with contextlib.redirect_stdout(io.StringIO()), contextlib.redirect_stderr(io.StringIO()):
                ts.main(['--root', self.r.root, '--jobs', '3', '--list-files'])
            self.assertEqual(dupes.NINJA_ARGS, ['-j3'])


BLOCK = ('#ifdef NON_MATCHING\n/* NON_MATCHING: T-1 near miss */\nvoid ftgt(void) {\n    D_80151110 = 1;\n}\n#else\n'
         'INCLUDE_ASM("asm/ovl/BBB/nonmatchings/BBB", ftgt);\n#endif')


class DraftBlockTests(unittest.TestCase):
    def test_block_around_the_include_asm_line(self):
        text = 'int x;\n\n' + BLOCK + '\n\nint y;\n'
        self.assertEqual(ts.draft_block(text, 'ftgt'), BLOCK)
        self.assertEqual(ts.draft_block(text.replace('\n', '\r\n'), 'ftgt'), BLOCK.replace('\n', '\r\n'))

    def test_only_the_block_of_that_function(self):
        other = BLOCK.replace('ftgt', 'fother')
        text = other + '\n\n' + BLOCK + '\n'
        self.assertEqual(ts.draft_block(text, 'ftgt'), BLOCK)
        self.assertEqual(ts.draft_block(text, 'fother'), other)
        self.assertIsNone(ts.draft_block(text, 'fnone'))

    def test_nested_conditionals_in_the_draft_are_balanced(self):
        block = BLOCK.replace('    D_80151110 = 1;\n', '#if 0\n    D_80151110 = 2;\n#endif\n    D_80151110 = 1;\n')
        self.assertEqual(ts.draft_block('int a;\n' + block + '\nint b;\n', 'ftgt'), block)

    def test_blank_lines_before_the_endif_are_inside_the_block(self):
        block = BLOCK.replace('ftgt);\n#endif', 'ftgt);\n\n#endif')
        self.assertEqual(ts.draft_block(block + '\n', 'ftgt'), block)

    def test_other_shapes_are_not_drafts(self):
        line = 'INCLUDE_ASM("asm/ovl/BBB/nonmatchings/BBB", ftgt);'
        self.assertIsNone(ts.draft_block(line + '\n', 'ftgt'))                                  # bare line
        self.assertIsNone(ts.draft_block('#if 0\nint a;\n#else\n' + line + '\n#endif\n', 'ftgt'))
        self.assertIsNone(ts.draft_block('#ifdef NON_MATCHING\nint a;\n#elif 1\n' + line + '\n#endif\n', 'ftgt'))
        self.assertIsNone(ts.draft_block('#ifdef NON_MATCHING\n' + line + '\n#endif\n', 'ftgt'))   # no #else
        self.assertIsNone(ts.draft_block('#ifdef NON_MATCHING\nint a;\n#else\nint b;\n' + line + '\n#endif\n', 'ftgt'))
        self.assertIsNone(ts.draft_block('#ifdef NON_MATCHING\nint a;\n#else\n' + line + '\nint b;\n#endif\n', 'ftgt'))


class DraftSweepTests(unittest.TestCase):
    def setUp(self):
        self.r = make_repo(SRC_C, BLOCK + '\n')
        self.addCleanup(self.r.close)

    def test_sweep_marks_the_row_and_the_plan(self):
        rows, _skipped, _stats = sweep(self.r)
        self.assertEqual([(x['name'], x['draft']) for x in rows], [('ftgt', 8)])
        self.assertIn('[replaces a 8-line NON_MATCHING draft]', ts.describe(rows[0]))
        rows, _skipped, _stats = ts.sweep(self.r.root, True, False)
        self.assertIsNone(rows[0]['draft'])
        self.assertNotIn('pattern', rows[0]['plan'])

    def test_an_accepted_copy_replaces_the_whole_block(self):
        with mock.patch.object(dupes, 'build_ok', lambda root, unit: (True, '')):
            run_main(self.r.root, '--apply', '--files', 'BBB')
        c = self.r.read('src/ovl/BBB.c')
        for gone in ('NON_MATCHING', '#else', '#endif', 'INCLUDE_ASM', 'D_80151110 = 1'):
            self.assertNotIn(gone, c)
        self.assertIn('void ftgt(void) {', c)

    def test_a_rejected_copy_brings_the_block_back(self):
        before = self.r.read('src/ovl/BBB.c')
        calls = []
        with mock.patch.object(dupes, 'build_ok', lambda root, unit: (calls.append(1) or len(calls) == 1, 'bad')):
            run_main(self.r.root, '--apply', '--files', 'BBB')
        self.assertEqual(self.r.read('src/ovl/BBB.c'), before)

    def test_keep_drafts_writes_into_the_else_arm(self):
        with mock.patch.object(dupes, 'build_ok', lambda root, unit: (True, '')):
            run_main(self.r.root, '--apply', '--files', 'BBB', '--keep-drafts')
        c = self.r.read('src/ovl/BBB.c')
        self.assertIn('#ifdef NON_MATCHING', c)
        self.assertEqual(c.count('void ftgt(void)'), 2)
        self.assertNotIn('INCLUDE_ASM', c)


class HeldTests(unittest.TestCase):
    def test_a_plan_whose_table_is_outside_its_function_file_is_held_and_never_applied(self):
        island = dict(unit='BBB', name='a', src='x', src_unit='AAA', bytes=8, tgtfile='src/ovl/BBB.c', jtbl='island')
        outside = dict(unit='BBB', name='b', src='x', src_unit='AAA', bytes=8, tgtfile='src/ovl/BBB.c', data='outside')
        rows = [ts.make_row('exact', 'exact', island), ts.make_row('exact', 'exact', outside)]
        self.assertEqual([r['held'] for r in rows], [None, 'data outside'])
        self.assertIn('jtbl island: sha1 check', ts.describe(rows[0]))
        self.assertIn('HELD, never applied: data outside', ts.describe(rows[1]))
        with mock.patch.object(dupes, 'build_ok', lambda root, unit: (True, '')), \
                mock.patch.object(dupes, 'apply_all', return_value=[]) as apply:
            ts.apply_rows('.', rows, log=lambda m: None)
        self.assertEqual([p['name'] for p in apply.call_args.args[1]], ['a'])
        self.assertTrue(apply.call_args.args[2])             # the sha1 check is always on

    def test_modes_are_applied_one_batch_each_in_order(self):
        rows = [ts.make_row(m, 'near' if m == 'near' else 'exact',
                            dict(unit='BBB', name=m, src='x', src_unit='AAA', bytes=4, tgtfile='src/ovl/BBB.c'))
                for m in ts.MODES]
        with mock.patch.object(dupes, 'build_ok', lambda root, unit: (True, '')), \
                mock.patch.object(dupes, 'apply_all', return_value=[]) as apply:
            ts.apply_rows('.', rows, log=lambda m: None)
        self.assertEqual([[p['name'] for p in c.args[1]] for c in apply.call_args_list],
                         [['exact'], ['near'], ['lenient']])


if __name__ == '__main__':
    unittest.main()
