from harness import *
for name,suffix in [('self','p->y3 = p->y3;'),('or0','p->y3 |= 0;'),('argfalse','if ((u32)arg0 < 0) {}'),('pfalse','if ((u32)p < 0) {}')]:
 if name.endswith('false'):b=re.sub(r'(p->y3 = [^\n]+;)',lambda m:m[0]+'\n    '+suffix,body)
 else:b=body.replace(uv,uv+'\n    '+suffix)
 run('dep_'+name,b)
b=body.replace('s32 pad;','s32 pad; struct { s16 y; } tmp;')
b=re.sub(r'p->y3 = ([^\n]+);',r'tmp.y = \1;',b).replace(uv,uv+'\n    p->y3 = tmp.y;')
run('dep_struct',b)
b=body.replace('s32 pad;','s32 pad; s16 y3;')
b=re.sub(r'p->y3 = ([^\n]+);',r'y3 = \1;',b).replace(uv,uv+'\n    p->y3 = y3;')
b=b.replace('AddPrim((D_8011ECA0 << 0xA)', 'AddPrim(((D_8011ECA0 + y3 * 0) << 0xA)')
run('dep_ext16',b)
