"""Synthetic tests for the T-9410 inventory (Docker only)."""
import unittest
from pathlib import Path
from tempfile import TemporaryDirectory
from types import SimpleNamespace
from unittest.mock import patch

from register_shapes import features, inventory, rows, shape


class RegisterShapesTest(unittest.TestCase):
    def test_inventory_units_duplicates_and_guarded_draft(self):
        with TemporaryDirectory() as tmp:
            root = Path(tmp)
            units = []
            for unit, source in [('DATE', 'void f(void) {}'), ('TT',
                    '#ifdef NON_MATCHING\nvoid f(void) {}\n#else\n'
                    'INCLUDE_ASM("asm", f);\n#endif')]:
                folder = root / unit
                folder.mkdir()
                src = folder / 'unit.c'
                src.write_text(source)
                (folder / 'f.s').write_text('/* 0 0 0 */ multu $t0, $t1\n')
                units.append(SimpleNamespace(unit=unit, src=src,
                                             nonmatchings=folder, matchings=folder))
            table = root / 'rows.md'
            table.write_text('| DATE | `f` | regorder | temp numbering |\n'
                             '| DATE/unit | `f` | promo | selector |\n'
                             '| TT | `f` | regorder | pointer |\n')
            with patch('register_shapes.srcscan.c_files', return_value=units):
                records, unique = inventory(root, table)
                self.assertEqual(len(records), 3)
                self.assertEqual(len(unique), 2)
                self.assertEqual(unique[0]['lines'], [1, 2])
                self.assertEqual(unique[0]['state'], 'C')
                self.assertEqual(unique[0]['shape'], 'selector/compare chain')
                self.assertEqual(unique[1]['state'], 'open')
                table.write_text('| MISSING | `f` | regorder | pointer |\n')
                unresolved, _ = inventory(root, table)
                self.assertEqual(unresolved[0]['state'], 'unresolved')
                table.write_text('| TT | `f` | regorder | pointer |\n')
                (root / 'TT/f.s').unlink()
                with self.assertRaisesRegex(ValueError, 'missing original asm'):
                    inventory(root, table)

    def test_rows_keep_history_and_family_anchor(self):
        table = '''| file | function | category | symptom |
| DATE | `func_80132000` (family: `func_80132010`) | hoist | a | b |
| DATE/80132000 | `func_80132000` | regorder | retry |
'''
        result = list(rows(table))
        self.assertEqual([r['name'] for r in result], ['func_80132000'] * 2)
        self.assertEqual([r['line'] for r in result], [2, 3])
        self.assertEqual(result[0]['symptom'], 'a | b')

    def test_asm_features_and_priority(self):
        asm = '''/* 0 0 0 */ addiu $v1, $zero, 0x800
/* 4 4 4 */ addiu $v0, $zero, 0x40
/* 8 8 8 */ multu $t7, $v1
/* C C C */ lbu $v0, %lo(D_800E6280)($v0)
'''
        f = features(asm)
        self.assertEqual(f['constants'], [2048, 64])
        self.assertEqual(f['multu'], 1)
        self.assertTrue(f['gamestate'])
        self.assertFalse(features('D_800E7D10 D_800E627F')['gamestate'])
        self.assertTrue(features('D_800E7D0F')['gamestate'])
        self.assertEqual(shape('TACO', 'f', 'hoisted constant order', f), '0x800/0x40 constants')
        self.assertEqual(shape('DATE', 'f', 'table CSE', f), 'pointer/base/CSE')
        f['multu'] = 6
        self.assertEqual(shape('DATE', 'f', 'table CSE', f), 'DATE repeated products')
        self.assertEqual(shape('ENDING', 'f', '% 240 temporary', f), 'ENDING remainder')


if __name__ == '__main__':
    unittest.main()
