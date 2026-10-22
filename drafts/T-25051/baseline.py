from harness import *
variants={'base':body,'after_uv':body.replace(uv,uv+'\n    if (p->y3) {}'),'false_after_uv':body.replace(uv,uv+'\n    if ((u32)p->y3 < 0) {}'),'before_uv':re.sub(r'(    p->y3 = [^\n]+;)',r'\1\n    if (p->y3) {}',body)}
for name,b in variants.items():run(name,b)
