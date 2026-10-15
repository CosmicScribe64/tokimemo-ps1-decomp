"""Synthetic checks for immutable initial results and reserved repair ownership (T-14500)."""
import json
import os
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

import exhaustive_repair as repair


class ExhaustiveRepairTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.old = os.getcwd()
        os.chdir(self.tmp.name)
        repair.BASE.mkdir(parents=True)
        (repair.BASE / 'environment.json').write_text('{}')

    def tearDown(self):
        os.chdir(self.old)
        self.tmp.cleanup()

    def manifest(self, rows):
        (repair.BASE/'inventory.json').write_text(json.dumps(rows))

    def row(self, func, status='failed', guarded=False):
        row = dict(scope='TT:'+func,unit='TT',src='game.c',func=func,size=4,guarded=guarded)
        folder = repair.BASE/'TT'/func
        folder.mkdir(parents=True)
        (folder/'result.json').write_text(json.dumps(dict(row,status=status,score=7 if status=='scored' else None)))
        return row

    def test_incomplete_default_census_refuses_repairs(self):
        self.manifest([dict(scope='TT:f',unit='TT',func='f')])
        with patch('sys.argv',['repair']), patch.object(repair.subprocess,'run') as run:
            with self.assertRaisesRegex(SystemExit,'default census incomplete'):
                repair.main()
        run.assert_not_called()

    def test_reservations_and_initial_results_remain_untouched(self):
        a=self.row('a'); b=self.row('b'); c=self.row('c',status='scored'); d=self.row('d',guarded=True)
        rows=[a,b,c,d]
        self.manifest(rows)
        (repair.BASE/'date-repair-reservation.json').write_text(json.dumps([b]))
        snapshots={r['scope']:(repair.BASE/r['unit']/r['func']/'result.json').read_text() for r in rows}
        seen=[]
        def complete(cmd,env,check):
            base=Path(env['TOKIMEMO_DRAFT_BASE'])
            cohort=json.loads((base/'inventory.json').read_text())
            seen.append([r['scope'] for r in cohort])
            for row in cohort:
                folder=base/row['unit']/row['func'];folder.mkdir(parents=True)
                (folder/'result.json').write_text(json.dumps(dict(row,status='failed' if row['guarded'] else 'scored',score=None if row['guarded'] else 9)))
        with patch('sys.argv',['repair']),patch.object(repair.subprocess,'run',side_effect=complete):
            repair.main()
        self.assertEqual(seen,[['TT:a','TT:d']])
        for row in rows:
            self.assertEqual((repair.BASE/row['unit']/row['func']/'result.json').read_text(),snapshots[row['scope']])
        coverage=json.loads((repair.BASE/'coverage.json').read_text())
        self.assertEqual(coverage['repaired'],1)
        self.assertEqual(coverage['reserved'],1)
        best=[json.loads(line) for line in (repair.BASE/'best-catalog.jsonl').read_text().splitlines()]
        self.assertTrue(next(r for r in best if r['scope']=='TT:b')['reserved'])


if __name__=='__main__':
    unittest.main()
