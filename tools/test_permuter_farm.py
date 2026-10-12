"""Synthetic queue, resume and real-build verification tests for T-9810 (Docker)."""
import json
import os
from pathlib import Path
import subprocess
import tempfile
import unittest
from types import SimpleNamespace
from unittest.mock import patch

import permute
import permuter_farm as farm


class FarmTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.base = Path(self.tmp.name)
        self.root = self.base / 'root'
        self.scratch = self.base / 'scratchpad'
        self.farm = self.scratch / 'r9-farm'
        self.wave = self.scratch / 'w8-1'
        self.wave.mkdir(parents=True)
        self.source('main', '80041000', 'first')
        self.source('DATE', '80132000', 'shared')
        self.source('TT', '80132000', 'shared')

    def source(self, unit, stem, name, guarded=False):
        rel = 'src/main/%s.c' % stem if unit == 'main' else 'src/ovl/%s/%s.c' % (unit, stem)
        path = self.root / rel
        path.parent.mkdir(parents=True, exist_ok=True)
        body = ('#ifdef NON_MATCHING\nvoid %s(void) { }\n#else\n' % name) if guarded else ''
        path.write_text(body + 'INCLUDE_ASM("asm/x", %s);\n' % name + ('#endif\n' if guarded else ''))
        return path

    def rows(self, rows):
        (self.wave / 'ranked.jsonl').write_text('\n'.join(json.dumps(r) for r in rows))

    def worker_args(self, wall_time=None):
        return SimpleNamespace(farm=str(self.farm), root=str(self.root), wall_time=wall_time,
                               time=17, max_time=68, patience=1)

    def test_worker_without_wall_time_keeps_budget_and_drains_multiple_claims(self):
        self.rows([dict(func='first', body='void first(void) { }', edits=1),
                   dict(func='shared', unit='TT', body='void shared(void) { }', edits=2),
                   dict(func='shared', unit='DATE', body='void shared(void) { }', edits=3)])
        farm.collect(self.root, self.scratch, self.farm)
        with patch.object(farm, 'private_root', return_value=self.root), \
                patch.object(farm, 'search', side_effect=lambda *a: dict(score=2, baseline_score=2)) as search:
            farm.worker(self.worker_args(), '0')
        self.assertEqual(search.call_count, 3)
        self.assertEqual([call.args[4] for call in search.call_args_list], [17, 17, 17])
        state = farm.load_json(self.farm / 'state.json', {})
        self.assertTrue(all(r['status'] == 'hopeless' for r in state.values()))

    def test_worker_waits_for_retryable_claims_held_by_other_workers(self):
        self.farm.mkdir(parents=True)
        cand = dict(unit='main', function='first')
        farm.atomic_json(self.farm / 'queue.json', [cand])
        farm.atomic_json(self.farm / 'state.json', {'main:first': dict(status='running')})
        with patch.object(farm, 'private_root', return_value=self.root), \
                patch.object(farm, 'claim', side_effect=[None, (cand, 17), None]), \
                patch.object(farm.time, 'sleep') as sleep, \
                patch.object(farm, 'search', return_value={}) as search, \
                patch.object(farm, 'finish', side_effect=lambda *a: farm.atomic_json(
                    self.farm / 'state.json', {'main:first': dict(status='won')})):
            farm.worker(self.worker_args(), '0')
        sleep.assert_called_once()
        search.assert_called_once()

    def test_worker_honors_stop_file(self):
        self.farm.mkdir(parents=True)
        (self.farm / 'STOP').touch()
        with patch.object(farm, 'private_root', return_value=self.root), patch.object(farm, 'claim') as claim:
            farm.worker(self.worker_args(), '0')
        claim.assert_not_called()

    def test_worker_with_deadline_caps_claimed_budget(self):
        self.farm.mkdir(parents=True)
        cand = dict(unit='main', function='first')
        with patch.object(farm, 'private_root', return_value=self.root), \
                patch.object(farm, 'claim', return_value=(cand, 17)), \
                patch.object(farm, 'search', return_value={}) as search, \
                patch.object(farm, 'finish'), \
                patch.object(farm.time, 'monotonic', side_effect=[0, 0, 2, 6]):
            farm.worker(self.worker_args(wall_time=5), '0')
        self.assertEqual(search.call_args.args[4], 3)

    def test_inline_scoped_ranking_and_duplicate_best(self):
        self.rows([dict(func='shared', unit='TT', body='void shared(void) { }', edits=8),
                   dict(func='shared', unit='DATE', mapped='void shared(void) { }', lines=2),
                   dict(func='shared', unit='TT', body='void shared(void) { int x; }', edits=3),
                   dict(func='first', body='void first(void) { }', diff_count=1)])
        queue = farm.collect(self.root, self.scratch, self.farm)
        self.assertEqual([(q['unit'], q['function'], q['last_best_score']) for q in queue],
                         [('main', 'first', 1), ('DATE', 'shared', 2), ('TT', 'shared', 3)])
        self.assertIn('int x', Path(queue[-1]['draft_path']).read_text())

    def test_missing_drafts_and_ambiguous_units_are_excluded(self):
        self.rows([dict(func='first', draft_path='absent.c', edits=1),
                   dict(func='shared', body='void shared(void) { }', edits=1)])
        self.assertEqual(farm.collect(self.root, self.scratch, self.farm), [])

    def test_standalone_nonmatching_and_t0018_drafts(self):
        self.source('main', '80042000', 'guarded', guarded=True)
        (self.wave / 'first.c').write_text('void first(void) { }')
        (self.wave / 'shared.c').write_text('void shared(void) { }')
        cases = self.root / 'wiki/data/t0018-cases.md'
        cases.parent.mkdir(parents=True)
        cases.write_text('| DATE/80132000 | `shared` | regorder | draft in scratchpad w8-1/shared.c |\n')
        queue = farm.collect(self.root, self.scratch, self.farm)
        self.assertEqual({(q['unit'], q['function']) for q in queue},
                         {('main', 'first'), ('main', 'guarded'), ('DATE', 'shared')})

    def test_path_alias_score_association_and_malformed_json(self):
        (self.wave / 'first.c').write_text('void first(void) { }')
        self.rows([dict(func='first', draft_path='/scratch/w8-1/first.c', edits=2)])
        with (self.wave / 'ranked.jsonl').open('a') as out:
            out.write('\ninvalid')
        queue = farm.collect(self.root, self.scratch, self.farm)
        self.assertEqual(queue[0]['last_best_score'], 2)
        self.assertEqual(len(farm.load_json(self.farm / 'collection.json', {})['warnings']), 1)

    def test_matched_functions_are_excluded(self):
        (self.wave / 'matched.c').write_text('void matched(void) { }')
        self.assertEqual(farm.collect(self.root, self.scratch, self.farm), [])

    def test_standalone_multiline_ansi_draft_is_collected(self):
        (self.wave / 'first.c').write_text('void first(\nint a,\nint b)\n{ }\n')
        queue = farm.collect(self.root, self.scratch, self.farm)
        self.assertEqual(queue[0]['function'], 'first')
        self.assertIn('int b', Path(queue[0]['draft_path']).read_text())

    def test_score_uses_measured_diff_not_row_index(self):
        self.assertIsNone(farm.score_of(dict(n=77)))
        self.assertIsNone(farm.score_of(dict(status='build-error', edits=0)))
        self.assertIsNone(farm.score_of(dict(state='error', n=9999)))
        self.assertEqual(farm.score_of(dict(n=77, state='diff')), 77)
        self.assertEqual(farm.score_of(dict(diff='f: DIFF\n  -1234abcd\n  +9876abcd\n')), 2)
        self.assertEqual(farm.score_of(dict(edits=0, lines=0, diff='f: DIFF\n  -1234abcd\n  +9876abcd\n')), 2)
        self.assertEqual(farm.score_of(dict(diff='f: MATCH (relocations resolved)')), 0)
        self.assertIsNone(farm.score_of(dict(edits=0, diff='ERROR missing expected')))

    def test_stem_scopes_inline_drafts_and_build_output_is_not_a_unit(self):
        self.rows([dict(name='shared', stem='TT/80132000', unit='[1/4] CC file.c', body='void shared(void) { }', lines=2)])
        queue = farm.collect(self.root, self.scratch, self.farm)
        self.assertEqual(queue[0]['unit'], 'TT')

    def candidate(self):
        self.rows([dict(func='first', body='void first(void) { }', edits=1)])
        return farm.collect(self.root, self.scratch, self.farm)[0]

    def test_improvement_extends_budget_and_stagnation_exhausts(self):
        cand = self.candidate()
        farm.claim(self.farm, 'test', 10, 40, 2)
        farm.finish(self.farm, cand, dict(score=5, baseline_score=10), 10, 40, 2)
        rec = farm.load_json(self.farm / 'state.json', {})['main:first']
        self.assertEqual(rec['next_budget'], 20)
        self.assertEqual(farm.claim(self.farm, 'test', 10, 40, 2)[1], 20)
        farm.finish(self.farm, cand, dict(score=5, baseline_score=5), 10, 40, 2)
        farm.claim(self.farm, 'test', 10, 40, 2)
        farm.finish(self.farm, cand, dict(score=5, baseline_score=5), 10, 40, 2)
        self.assertIsNone(farm.claim(self.farm, 'test', 10, 40, 2))

    def test_dead_owner_claim_recovers_and_new_draft_resets(self):
        cand = self.candidate()
        farm.claim(self.farm, 'dead', 10, 40, 2)
        self.assertIsNotNone(farm.claim(self.farm, 'new', 10, 40, 2))
        farm.finish(self.farm, cand, dict(error='bad draft'), 10, 40, 2)
        self.assertIsNone(farm.claim(self.farm, 'new', 10, 40, 2))
        self.rows([dict(func='first', body='void first(void) { int x; }', edits=1)])
        farm.collect(self.root, self.scratch, self.farm)
        self.assertIsNotNone(farm.claim(self.farm, 'new', 10, 40, 2))

    def test_live_owner_cannot_be_stolen(self):
        self.candidate()
        with (self.farm / 'owner-live.lock').open('a') as lock:
            farm.fcntl.flock(lock, farm.fcntl.LOCK_EX)
            farm.claim(self.farm, 'live', 10, 40, 2)
            self.assertIsNone(farm.claim(self.farm, 'other', 10, 40, 2))

    def test_recollect_does_not_replace_a_live_claim(self):
        cand = self.candidate()
        with (self.farm / 'owner-live.lock').open('a') as lock:
            farm.fcntl.flock(lock, farm.fcntl.LOCK_EX)
            farm.claim(self.farm, 'live', 10, 40, 2)
            self.rows([dict(func='first', body='void first(void) { int changed; }', edits=1)])
            farm.collect(self.root, self.scratch, self.farm)
            self.assertIsNone(farm.claim(self.farm, 'other', 10, 40, 2))
            farm.finish(self.farm, cand, dict(score=0, win='first.c'), 10, 40, 2)
            new, _ = farm.claim(self.farm, 'other', 10, 40, 2)
            self.assertNotEqual(new['fingerprint'], cand['fingerprint'])

    def test_report_records_candidate_timings(self):
        cand = self.candidate()
        farm.claim(self.farm, 'test', 10, 40, 2)
        farm.finish(self.farm, cand, dict(started=1, score=0, win='first.c', elapsed_seconds=2.5), 10, 40, 2)
        summary = farm.report(self.farm)
        self.assertEqual(summary['wins'], 1)
        self.assertIn('2.500', (self.farm / 'report.md').read_text())
        self.assertEqual(farm.load_json(self.farm / 'report.json', {})['candidates'][0]['function'], 'main:first')

    def test_report_keeps_archived_fingerprint_timings_and_deduplicates(self):
        cand = self.candidate()
        farm.claim(self.farm, 'test', 10, 40, 2)
        result = dict(started=1, score=4, elapsed_seconds=2.5, log='unique.log', actual_budget=10)
        farm.finish(self.farm, cand, result, 10, 40, 2)
        (self.farm / 'worker-0.log').write_text(json.dumps(dict(function='first', unit='main', **result)) + '\n')
        self.assertEqual(farm.report(self.farm)['attempts'], 1)
        self.rows([dict(func='first', body='void first(void) { int changed; }', edits=1)])
        farm.collect(self.root, self.scratch, self.farm)
        farm.claim(self.farm, 'test', 10, 40, 2)
        self.assertEqual(farm.report(self.farm)['attempts'], 1)

    def setup_verification(self, body='void first(void) { int dummy; }'):
        cand = self.candidate()
        private = self.farm / 'workers/0/repo'
        src = private / cand['source']
        src.parent.mkdir(parents=True)
        src.write_text((self.root / cand['source']).read_text())
        output = self.farm / 'searches/first/output-0-1'
        output.mkdir(parents=True)
        (output / 'source.c').write_text(body)
        return cand, private, src, output

    def verify_with_results(self, codes, texts, body='void first(void) { int dummy; }'):
        cand, private, src, output = self.setup_verification(body)
        original = src.read_text()
        calls = []

        def runner(cmd, **kw):
            self.assertEqual(Path(kw['cwd']), private)
            self.assertNotEqual(src.read_text(), original)
            calls.append(cmd)
            return subprocess.CompletedProcess(cmd, codes[len(calls)-1], texts[len(calls)-1])

        def verifier(root, meta, directory):
            return permute.verify_candidate(root, meta, directory, runner=runner)

        win, report = farm.verify(self.root, private, cand, output, self.farm, verifier)
        self.assertEqual(src.read_text(), original)
        self.assertEqual((self.root / cand['source']).read_text(), original)
        return win, report, calls

    def test_match_and_sha1_required_before_export(self):
        win, _, calls = self.verify_with_results([0, 0], ['first: MATCH', 'unit: OK'])
        self.assertEqual(calls[0][-2:], ['--resolve', 'main:first'])
        self.assertEqual(calls[1], ['ninja', 'build/SLPM_86.053.ok'])
        self.assertIn('FAKE:', Path(win).read_text())
        self.assertIn('T-9810', Path(win).read_text())

    def test_function_match_with_failed_sha1_is_rejected(self):
        win, _, _ = self.verify_with_results([0, 1], ['first: MATCH', 'sha1 failed'])
        self.assertIsNone(win)
        self.assertFalse((self.farm / 'wins').exists())

    def test_diff_is_rejected_without_sha1_call(self):
        win, _, calls = self.verify_with_results([1], ['first: DIFF REAL DIFFERENCE'])
        self.assertIsNone(win)
        self.assertEqual(len(calls), 1)

    def test_jump_table_diff_requires_unit_sha1(self):
        win, _, calls = self.verify_with_results([1, 0], ['first: DIFF jtbl_1234 .rodata', 'OK'])
        self.assertIsNotNone(win)
        self.assertEqual(len(calls), 2)

    def test_refuse_verification_in_live_tree(self):
        cand, _, _, output = self.setup_verification()
        with self.assertRaises(ValueError):
            farm.verify(self.root, self.root, cand, output, self.farm)

    def test_queue_cannot_redirect_verification_to_live_source(self):
        cand, private, _, output = self.setup_verification()
        cand['source'] = str(self.root / cand['source'])
        with self.assertRaises(ValueError):
            farm.verify(self.root, private, cand, output, self.farm)

    def test_verification_exception_restores_private_source(self):
        cand, private, src, output = self.setup_verification()
        original = src.read_text()

        def verifier(root, meta, directory):
            def runner(*args, **kwargs):
                raise OSError('synthetic build failure')
            return permute.verify_candidate(root, meta, directory, runner=runner)

        with self.assertRaises(OSError):
            farm.verify(self.root, private, cand, output, self.farm, verifier)
        self.assertEqual(src.read_text(), original)
        self.assertFalse((self.farm / 'wins').exists())

    def test_new_score_zero_always_reaches_verification(self):
        cand, private, _, _ = self.setup_verification()
        run_dir = self.farm / 'searches' / 'main' / 'first' / cand['fingerprint'][:12]
        output = run_dir / 'output-0-1'
        old_cwd = Path.cwd()
        self.addCleanup(os.chdir, old_cwd)
        with patch.object(farm, 'command'), patch.object(farm, 'base_score', return_value=5), \
                patch.object(permute, 'run', return_value=[(0, 1, str(output))]), \
                patch.object(farm, 'verify', return_value=('verified.c', ['MATCH', 'unit OK'])) as check:
            result = farm.search(self.root, private, self.farm, cand, 10, '0')
        self.assertEqual(result['score'], 0)
        self.assertEqual(result['win'], 'verified.c')
        check.assert_called_once()

    def test_every_zero_is_verified_even_after_a_win(self):
        cand, private, _, _ = self.setup_verification()
        run_dir = self.farm / 'searches' / 'main' / 'first' / cand['fingerprint'][:12]
        old_cwd = Path.cwd()
        self.addCleanup(os.chdir, old_cwd)
        with patch.object(farm, 'command'), patch.object(farm, 'base_score', return_value=5), \
                patch.object(permute, 'run', return_value=[(0, 1, str(run_dir/'output-0-1')), (0, 2, str(run_dir/'output-0-2'))]), \
                patch.object(farm, 'verify', side_effect=[('verified.c', ['MATCH']), (None, ['DIFF'])]) as check:
            result = farm.search(self.root, private, self.farm, cand, 10, '0')
        self.assertEqual(check.call_count, 2)
        self.assertEqual(result['win'], 'verified.c')
        self.assertEqual(len(result['verification']), 2)


if __name__ == '__main__':
    unittest.main()
