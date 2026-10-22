from harness import *
for name,idx in [('zero','0'),('late','((u32)arg0 < 0)')]:
 b=body.replace('s32 pad;','s32 pad; s16 y3[1];')
 b=re.sub(r'p->y3 = ([^\n]+);',lambda m:'y3['+idx+'] = '+m[1]+';',b).replace(uv,uv+'\n    p->y3 = y3[0];')
 run('ind_'+name,b)
