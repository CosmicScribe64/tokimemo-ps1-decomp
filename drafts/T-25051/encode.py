from harness import *
for name,enc,dec in [('xorbit','({e}) ^ 0x8000','y3 ^ 0x8000'),('xorhigh','({e}) ^ 0x10000','y3'),('not','~({e})','~y3')]:
 b=body.replace('s32 pad;','s32 pad; s32 y3;')
 b=re.sub(r'p->y3 = ([^\n]+);',lambda m:'y3 = '+enc.replace('{e}',m[1])+';',b).replace(uv,uv+'\n    p->y3 = '+dec+';')
 run('enc_'+name,b)
