#!/usr/bin/env python3
"""Recover typed copied callback tables from measured MIPS word tables; no source ASM changes."""
from pathlib import Path
import json,re,sys
sys.path.insert(0,'/work/tools');import permute
OUT=Path('/work/drafts/T-15800');ROOT=Path('/work/build/t15800/root')
for row in json.loads((OUT/'inventory.json').read_text()):
 unit,func=row['scope'].split(':');folder=OUT/unit/func;p=folder/'body.c'
 if not p.exists():continue
 body=p.read_text();block=re.search(r'typedef struct \{ s32 w\[(\d+)\]; \} (M2cBlock\w+);[^\n]*\n',body)
 if not block or not re.search(r'\]\([^;\n]*\);|\(\*\(s32 \(\*\*\)',body):continue
 copy=re.search(r'\*\('+block[2]+r' \*\)\(&(?P<local>sp\w+)(?:\[0\])?\) = \*\('+block[2]+r' \*\)\(&(?P<symbol>D_\w+)\);',body)
 if not copy:continue
 local,symbol=copy['local'],copy['symbol'];count=int(block[1]);datafile=None;words=None
 for path in (ROOT/'asm/ovl'/unit/'data').rglob('*.s'):
  asm=path.read_text();m=re.search(r'^dlabel '+symbol+r'\n(.*?)^enddlabel '+symbol+r'$',asm,re.M|re.S)
  if m:
   words=sum(len(x.split(',')) for x in re.findall(r'\.word\s+([^\n]+)',m[1]));datafile=str(path.relative_to(ROOT));break
 if words is None or words<count:raise ValueError((row['scope'],symbol,count,words))
 extra='*(s32 *)((unsigned char *)(&'+local+' + 0xA8) + (4))'
 extra_line=next((line for line in body.splitlines() if extra in line),'')
 if extra_line:
  count+=1;body=body.replace(extra_line+'\n','')
 if count>words:raise ValueError('copy exceeds observed table: '+row['scope'])
 body=body.replace(block[0],'');body=re.sub(r'^    s32 '+local+r'\[\d*\];\n','',body,flags=re.M)
 copy=re.search(r'\*\(M2cBlock\w+ \*\)\(&'+local+r'(?:\[0\])?\) = \*\(M2cBlock\w+ \*\)\(&'+symbol+r'\);',body);assert copy
 body=body[:copy.start()]+'callbacks = *(T15800Callbacks *)&'+symbol+';'+body[copy.end():]
 body=body.replace('(&'+local+'[0])','callbacks.f')
 body=body.replace('(*(s32 (**)(s32))((unsigned char *)(sp + var_t1) + (44)))(0x80);','callbacks.f[var_t1 / 4](0x80);')
 body=re.sub(r'\*\(s32 \*\)\(\(unsigned char \*\)\(sp \+ \(([^\n;]+) \* 4\)\) \+ \(44\)\)',r'(s32) callbacks.f[\1]',body)
 at=body.index('{')+1;body=body[:at]+f'\n    typedef struct {{ s32 (*f[{count}])(); }} T15800Callbacks;\n    T15800Callbacks callbacks;'+body[at:]
 context=(folder/'context.c').read_text();context=permute.replace_function(context,func,body);assert context
 p.write_text(body);(folder/'context.c').write_text(context)
 provenance=json.loads((folder/'provenance.json').read_text());provenance['type_reconstruction']={'kind':'copied callback table','entries_copied':count,'observed_data_words':words,'table_symbol':symbol,'data_source':datafile,'reason':'word copy followed by indexed jalr; callback table occupies full copied size'};(folder/'provenance.json').write_text(json.dumps(provenance,indent=2)+'\n')
 print(row['scope'],'callback entries',count,'observed words',words)
