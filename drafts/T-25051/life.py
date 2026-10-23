from harness import *
b=body.replace('s32 pad;','s32 pad; s16 y3;')
b=re.sub(r'p->y3 = ([^\n]+);',r'y3 = \1;',b).replace(uv,uv+'\n    p->y3 = y3;')
call='AddPrim((D_8011ECA0 << 0xA) + 0x3FC + D_800E8CA0, p);'
run('life_call',b.replace(call,call+'\n    if ((u32)y3 < 0) {}'))
run('life_falseuv',b.replace(uv,uv.replace('p->u0 = 0','p->u0 = (u32)y3 < 0')))
idx=b.index(call)+len(call)
bx=b[:idx]+b[idx:].replace('p->r0 = 0x80;', 'p->r0 = 0x80 + y3 * 0;')
run('life_next',bx)
run('life_unsigned',b.replace('s16 y3;','u16 y3;'))
# The s64 spelling was rejected by CFE (not declared here); no object was produced.
