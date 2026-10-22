from harness import *
run('vol_y3',body.replace('p->y3 =','((volatile POLY_FT4 *)p)->y3 ='))
run('vol_u0',body.replace(uv,uv.replace('p->u0','((volatile POLY_FT4 *)p)->u0')))
run('vol_uv',body.replace(uv,uv.replace('p->','((volatile POLY_FT4 *)p)->')))
