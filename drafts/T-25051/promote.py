from harness import *
for name,zero in [('mul','p->y3 * 0'),('false','((u32)p->y3 < 0)'),('sub','(p->y3 - p->y3)')]:
 run('prom_'+name,body.replace(uv,uv.replace('p->u0 = 0','p->u0 = '+zero)))
run('prom_addprim',body.replace('D_8011ECA0 << 0xA','(D_8011ECA0 + p->y3 * 0) << 0xA'))
