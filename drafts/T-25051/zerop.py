from harness import *
for local in [False,True]:
 b=body.replace('s32 pad;','s32 pad; s16 y3;') if local else body
 b=re.sub(r'p->y3 = ([^\n]+);',lambda m:'pad = (u32)p < 0;\n    '+('y3' if local else 'p->y3')+' = ('+m[1]+') | pad;',b)
 if local:b=b.replace(uv,uv+'\n    p->y3 = y3;')
 run('zp_'+str(local),b)
