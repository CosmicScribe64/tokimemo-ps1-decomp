"""Synthetic quality-audit checks separate compilability from unresolved reconstruction (T-14500)."""
import json
from pathlib import Path
import tempfile
import unittest
import audit_drafts as audit


class DraftAuditTests(unittest.TestCase):
    def test_placeholders_ignore_comments_and_strings(self):
        body='void f(void) { /* M2C_ERROR */ puts("M2C_LWL"); M2C_ERROR(); }'
        flags=audit.residuals(body,body)
        self.assertEqual([f['kind'] for f in flags],['m2c-placeholder'])

    def test_byte_cast_precedes_address_arithmetic(self):
        safe='void f(void) { use((u8 *)&D_800E6280 + 4); use((s8 *)&sp20 + 4); }'
        self.assertFalse(audit.residuals(safe,safe))
        bad='void f(void) { use(&D_800E6280 + 4); use(&sp20 + 4); use(8 + &D_800E6280); use(8 + &sp20); }'
        self.assertEqual({f['kind'] for f in audit.residuals(bad,bad)},
                         {'gamestate-address-arithmetic','stack-address-arithmetic'})

    def test_quality_clear_alternative_precedes_lower_score_with_synthetic_local(self):
        with tempfile.TemporaryDirectory() as tmp:
            base=Path(tmp)
            row=dict(scope='TT:f',unit='TT',func='f',src='src/f.c',size=4,guarded=False)
            (base/'inventory.json').write_text(json.dumps([row]))
            old=base/'TT/f';old.mkdir(parents=True)
            (old/'body.c').write_text('void f(void) {\n    s32 sp20;\n}\n')
            (old/'m2c.c').write_text('void f(void) {\n}\n')
            (old/'context.c').write_text('context')
            original=json.dumps(dict(row,status='scored',score=1))
            (old/'result.json').write_text(original)
            good=base/'repairs/TT/f';good.mkdir(parents=True)
            (good/'body.c').write_text('void f(void) {\n    s32 actual_counter;\n}\n')
            (good/'context.c').write_text('context')
            (base/'repairs/catalog.jsonl').write_text(json.dumps(dict(row,status='scored',score=8,repair_path=str(good)))+'\n')
            summary=audit.audit(base)
            self.assertEqual(summary['statuses'],{'compiling-audit-clear':1})
            selected=json.loads((base/'quality-audit.jsonl').read_text())
            self.assertEqual(selected['selected_path'],str(good))
            self.assertEqual((old/'result.json').read_text(),original)


if __name__=='__main__':
    unittest.main()
