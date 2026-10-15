#!/usr/bin/env python3
"""Recover concrete scalar/array views and existing K&R return views for T-15800 drafts."""
from pathlib import Path
import json,re,sys
sys.path.insert(0,'/work/tools');import permute
OUT=Path('/work/drafts/T-15800')
returns={'MASTER:func_8013B448','GYOZI:func_80144B70','SHOUGATU:func_8014288C','OPTION:func_80132BA8','OPTION:func_80133718'}
for row in json.loads((OUT/'inventory.json').read_text()):
 unit,func=row['scope'].split(':');folder=OUT/unit/func;p=folder/'body.c'
 if not p.exists() or not (folder/'context.c').exists():continue
 body=p.read_text();old=body;notes=[];context=(folder/'context.c').read_text();decl=(folder/'declarations.h').read_text()
 if row['scope'] in returns:
  body=body.replace('void '+func+'(', 's32 '+func+'(',1);notes.append('retain existing explicit/implicit s32 return view; no stub or new control flow')
 if row['scope']=='GYOZI:func_80143160':
  body=body.replace('D_80148970 = *(s32 *)((unsigned char *)(&D_800F53A0.girl[D_800F62CF]) + (8));','D_80148970 = *(Word4 *)&D_800F53A0.girl[D_800F62CF].unk_08;');notes.append('use existing Word4 copy view corroborated by inverse-copy neighbor func_80143110')
 if row['scope']=='OPTION:func_80133010':
  body=body.replace('*(void *)0x80120653 = (u8) (*(u8 *)0x80120653 | 0x80);','D_80120653 |= 0x80;');notes.append('byte store uses existing u8 global, not incomplete void dereference')
 if row['scope'] in {'OPTION:func_80132384','OPTION:func_80133330'}:
  local='sp50' if func=='func_80132384' else 'sp2C'
  if func=='func_80132384':
   body=body.replace('{\n','{\n    s16 sp50[4][8];\n',1)
   context=context.replace('void func_801325A0(Arrs4x8 *p);','void func_801325A0();');decl=decl.replace('void func_801325A0(Arrs4x8 *p);','void func_801325A0();')
  else:body=body.replace('s32 sp2C[16];','s16 sp2C[4][8];')
  body=body.replace('(&'+local+'[0])[D_800E7313].unk0',local+'[0][D_800E7313]')
  body=body.replace('*(s16 *)((unsigned char *)(&(&'+local+'[0])[D_800E7313]) + (16))',local+'[1][D_800E7313]')
  notes.append('coordinate helper func_801325A0 fills four arrays of eight s16 values; recover real 64-byte local and indexing')
 if unit=='SHOUGATU' and func in {'func_8013E50C','func_8013EA98'}:
  body=body.replace('{\n','{\n    s16 sp40[7];\n',1).replace('sp40[0].unk0 = (s32)','*(s32 *)sp40 = (s32)').replace('(&sp40[0])[var_v1]','sp40[var_v1]').replace('(&sp40[0])[var_v0_3]','sp40[var_v0_3]')
  body=re.sub(r'\*\(s16 \*\)\(\(unsigned char \*\)\(sp \+ (\w+)\) \+ \(48\)\)',r'((s16 *)sp30)[\1 / 2]',body)
  body=body.replace('&D_800E6280 + (D_800E71DF * 0x38)','(u8 *)&D_800E6280 + (D_800E71DF * 0x38)');notes.append('recover seven-halfword threshold local from 14-byte copy; use explicit saved-array indexing for stack-relative lookup')
 if row['scope']=='NAME_ENT:func_80133510':
  body=body.replace('void *','u8 *').replace('void **','u8 **');notes.append('string-buffer pointers are byte pointers; IDO C89 does not allow void-pointer arithmetic')
 # These calls consume v0 in the original despite retained C's void spelling.
 abi={'GYOZI:func_80141944':['func_801428EC','func_80142C14'],'GYOZI:func_80145384':['func_80145520'],'SHOUGATU:func_8013F224':['func_80140314','func_80140698']}
 for name in abi.get(row['scope'],[]):
  body=body.replace('= '+name+'(', '= ((s32 (*)()) '+name+')(');notes.append('call-site s32 ABI view for result consumed from '+name+' in target')
 if body!=old:
  context=permute.replace_function(context,func,body);assert context;p.write_text(body);(folder/'context.c').write_text(context);(folder/'declarations.h').write_text(decl)
  provenance=json.loads((folder/'provenance.json').read_text());provenance.setdefault('repairs',[]).extend(notes);(folder/'provenance.json').write_text(json.dumps(provenance,indent=2)+'\n');print(row['scope'],len(notes),'repairs')
