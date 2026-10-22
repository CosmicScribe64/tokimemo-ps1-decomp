from harness import *
pat=r'p->y3 = ([^\n]+);\s*'+re.escape(uv)
for name,typ in [('const','const s16'),('register','register s16'),('auto','s16')]:
 b=re.sub(pat,lambda m:'{\n    '+typ+' y3 = '+m[1]+';\n    '+uv+'\n    p->y3 = y3;\n    }',body)
 run('local_'+name,b)
