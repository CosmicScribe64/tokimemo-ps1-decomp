from harness import *
for name,op in [('or','|'),('add','+')]:
 for local in [False,True]:
  b=body.replace('s32 pad;','s32 pad; s16 y3;') if local else body
  b=re.sub(r'p->y3 = ([^\n]+);',lambda m:('y3' if local else 'p->y3')+' = ('+m[1]+') '+op+' ((u32)p < 0);',b)
  if local:b=b.replace(uv,uv+'\n    p->y3 = y3;')
  run('ze_'+name+('_local' if local else '_direct'),b)
