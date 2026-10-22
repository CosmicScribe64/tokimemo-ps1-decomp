from harness import *
for name,typ,wrap in [('mask32','s32',lambda e:'('+e+') & 0xFFFF'),('signmask32','s32',lambda e:'((s16)('+e+')) & 0xFFFF'),('mask16','s16',lambda e:'('+e+') & 0xFFFF'),('signmask16','s16',lambda e:'((s16)('+e+')) & 0xFFFF')]:
 b=body.replace('s32 pad;','s32 pad; '+typ+' y3;')
 b=re.sub(r'p->y3 = ([^\n]+);',lambda m:'y3 = '+wrap(m[1])+';',b).replace(uv,uv+'\n    p->y3 = y3;')
 run('tc_'+name,b)
