from harness import *
for name,shape in [('do','do { '+uv+' } while (0);'),('doarg','do { '+uv+' } while ((u32)arg0 < 0);'),('falseif','if ((u32)arg0 >= 0) { '+uv+' }'),('gotolabel','goto uvstage; uvstage: '+uv)]:
 if name=='gotolabel':
  k=[0]
  def lab(m):k[0]+=1;return shape.replace('uvstage','uvstage'+str(k[0]))
  b=re.sub(re.escape(uv),lab,body)
 else:b=body.replace(uv,shape)
 run('scope_'+name,b)
