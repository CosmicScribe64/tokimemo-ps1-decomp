#!/usr/bin/env python3
"""Retain return-register ABI views where old-style callers consume a helper result."""
from pathlib import Path
import json,re,sys
sys.path.insert(0,'/work/tools');import permute
OUT=Path('/work/drafts/T-15800')
fixes={'GYOZI:func_80141944':['func_801428EC','func_80142C14'],'SHOUGATU:func_8013F224':['func_80140314','func_80140698'],'SHOUGATU:func_801430A0':['func_80143228']}
for scope,names in fixes.items():
 unit,func=scope.split(':');folder=OUT/unit/func;body=(folder/'body.c').read_text();context=(folder/'context.c').read_text();decls=(folder/'declarations.h').read_text();notes=[]
 for name in names:
  body=body.replace('= '+name+'(', '= ((s32 (*)()) '+name+')(')
  declaration='void '+name+'();\n'
  # Function names used as values need explicit forward declarations; the casts
  # retain the result-register view consumed by the target without changing the
  # retained definitions or their canonical return spellings.
  if not re.search(r'\b(?:void|s32)\s+'+name+r'\s*\(',context[:permute.function_range(context,func)[0]]):
   at=permute.function_range(context,func)[0];context=context[:at]+declaration+context[at:];decls+=declaration
  notes.append('caller consumes v0 from '+name+'; explicit s32 callback view preserves call-site ABI while frozen helper definition stays unchanged')
 context=permute.replace_function(context,func,body);assert context
 (folder/'body.c').write_text(body);(folder/'context.c').write_text(context);(folder/'declarations.h').write_text(decls)
 p=folder/'provenance.json';provenance=json.loads(p.read_text());provenance.setdefault('repairs',[]).extend(notes);p.write_text(json.dumps(provenance,indent=2)+'\n')
 print(scope,'return ABI view fixed')
