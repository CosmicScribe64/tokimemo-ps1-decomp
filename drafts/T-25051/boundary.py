from harness import *
for name,wrap in [('s16mask',lambda e:'((s16)('+e+')) & 0xFFFF'),('u16mask',lambda e:'((u16)('+e+')) & 0xFFFF'),('s16pre',lambda e:'(s16)('+e.rsplit(' - 0x78',1)[0]+') - 0x78'),('s16premask',lambda e:'((s16)('+e.rsplit(' - 0x78',1)[0]+') & 0xFFFF) - 0x78'),('s16mask32',lambda e:'((s16)('+e+')) & 0xFFFFFFFF')]:
 b=re.sub(r'p->y3 = ([^\n]+);',lambda m:'p->y3 = '+wrap(m[1])+';',body)
 run('bound_'+name,b)
