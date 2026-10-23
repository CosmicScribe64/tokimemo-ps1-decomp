from harness import *
for name,lhs in [('s16ptr','((s16 *)p)[17]'),('u16ptr','((u16 *)p)[17]'),('field_u16','*(u16 *)&p->y3'),('field_s16','*(s16 *)&p->y3')]:
 run('store_'+name,body.replace('p->y3 =',lhs+' ='))
