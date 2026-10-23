from harness import *
for name,expr in [('ternary','(p->y3 ? 0 : 0)'),('narrowfalse','((u16)p->y3 < 0)'),('logical','(p->y3 && 0)')]:
 run('cond_'+name,body.replace(uv,uv.replace('p->u0 = 0','p->u0 = '+expr)))
run('cond_samearms',body.replace(uv,'if (p->y3) { '+uv+' } else { '+uv+' }'))
run('cond_narrowtest',body.replace(uv,'if ((u16)p->y3 < 0) {}\n    '+uv))
