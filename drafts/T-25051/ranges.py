from harness import *
b=body.replace('s32 pad;','s32 pad; s16 y3;')
b=re.sub(r'p->y3 = ([^\n]+);',r'y3 = \1;',b).replace(uv,uv+'\n    p->y3 = y3;')
for n in [2,8]:
 k=[0]
 def wrap(m):
  k[0]+=1
  return ''.join('goto hold'+str(k[0])+'_'+str(j)+'; hold'+str(k[0])+'_'+str(j)+':\n' for j in range(n))+uv
 run('range_'+str(n),re.sub(re.escape(uv),wrap,b))
