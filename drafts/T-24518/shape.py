"""Localize structural differences; branch deltas/registers are diagnostic-only masked."""
from pathlib import Path
import difflib
import sys
sys.path.insert(0,'tools')
import funcdiff,funcloc,regdiff
name='func_8013FFAC';c=funcloc.locate(name,'ETC');expected,_=funcdiff.expected_object(c)
want=funcdiff.resolved_functions(expected,[name],c)[name]
body=sys.argv[1] if len(sys.argv)>1 else 'shared-signed'
got=funcdiff.resolved_functions('build/T-24518/'+body+'/guarded.o',[name])[name]
if '--prove-first' in sys.argv:
 assert funcdiff.keys(want[162:172])==funcdiff.keys(got[162:172])
 print('EXACT first table:10 resolved words at function offsets0x288..0x2AC')
def shape(row):
 key=row[0];word=int(key[:8],16);d=regdiff.Decoded(word);w=d.shape
 if d.kind=='branch':w&=~0xffff
 return '%08x'%w+key[8:]
a=[shape(r) for r in want];b=[shape(r) for r in got]
matcher=difflib.SequenceMatcher(None,a,b,autojunk=False)
matched=0
for tag,i,j,k,l in matcher.get_opcodes():
 if tag=='equal':matched+=j-i;continue
 print(tag,'original',i,j,'candidate',k,l)
 for r in want[i:j]:print(' -',r[1])
 for r in got[k:l]:print(' +',r[1])
print('normalized diagnostic positions',matched,'/',len(want),'target;',len(got),'candidate')
