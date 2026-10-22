"""Separate dispatcher structural differences from register allocation."""
from pathlib import Path
import sys
sys.path.insert(0,'tools')
import funcdiff as fd, regdiff as rd
name='func_8007132C'; c=fd.locate(name,'main')
e,problem=fd.expected_object(c); assert not problem
want=fd.resolved_functions(e,[name],c)[name]
for label in sys.argv[1:]:
 d=Path('build/t25052')/label
 got=fd.resolved_functions(d/'guarded.o',[name],c)[name]
 pairs, structural,left=rd.align(fd.keys(got),fd.keys(want))
 rows=['%s: %d structural regions, %d register pairs, %d immediate pairs'%(label,len(structural),len(pairs),len(left))]
 for op,(i1,i2),(j1,j2) in structural:
  rows.append('draft %d:%d / target %d:%d (0x%X)'%(i1,i2,j1,j2,0x8007132c+4*j1))
  rows.extend(' - '+rd.text_of(x) for x in got[i1:i2])
  rows.extend(' + '+rd.text_of(x) for x in want[j1:j2])
 (d/'structural.txt').write_text('\n'.join(rows)+'\n')
 print(rows[0])
