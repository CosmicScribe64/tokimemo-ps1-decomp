from harness import *
for n in [1,16]:
 b=body.replace('s32 pad;','s32 pad; u32 y3;')
 b=re.sub(r'p->y3 = ([^\n]+);',lambda m:'y3 = (u32)('+m[1]+') << '+str(n)+';',b).replace(uv,uv+'\n    p->y3 = y3 >> '+str(n)+';')
 run('shift_'+str(n),b)
