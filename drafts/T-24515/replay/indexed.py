"""Transfer the matched sibling's direct-global actor indexing to NAME_ENT398EC."""
from pathlib import Path
from probe import body,probe
name='func_801398EC'
base=Path('build/T-24515/func_801398EC/normal-for-loop/body.c').read_text()
indexed=base.replace('    Rec24 *rec;\n','').replace('    rec = D_801217D0;\n','').replace('        rec++;\n','').replace('rec->unk_', 'D_801217D0[i].unk_')
probe(name,'global-index-loop',indexed)
paired=body(name).replace('    Rec24 *rec;\n','').replace('    rec = &D_801217D0[1];\n','').replace('        rec += 2;\n','').replace('rec->unk_', 'D_801217D0[i].unk_').replace('rec[1].unk_', 'D_801217D0[i + 1].unk_')
probe(name,'global-index-paired',paired)
