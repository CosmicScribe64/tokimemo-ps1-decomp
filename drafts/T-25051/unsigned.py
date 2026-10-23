from harness import *
for name,expr in [('u32','{e}'),('u32_arith','(u32)({e})'),('u16_add','(u32)(s16)({e})')]:
 b=body.replace('s32 pad;','s32 pad; u32 y3;')
 b=re.sub(r'p->y3 = ([^\n]+);',lambda m:'y3 = '+expr.replace('{e}',m[1])+';',b).replace(uv,uv+'\n    p->y3 = y3;')
 run('unsigned_'+name,b)
