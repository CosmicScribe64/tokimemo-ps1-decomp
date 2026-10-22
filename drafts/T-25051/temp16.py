from harness import *
b=body.replace('s32 pad;', 's32 pad; s16 y3;')
b=re.sub(r'p->y3 = ([^\n]+);',r'y3 = \1;',b).replace(uv,uv+'\n    p->y3 = y3;')
run('temp16',b)
