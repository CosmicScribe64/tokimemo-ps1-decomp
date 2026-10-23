from harness import *
temp=body.replace('s32 pad;','s32 y3;')
temp=re.sub(r'p->y3 = ([^\n]+);',r'y3 = \1;',temp).replace(uv,uv+'\n    p->y3 = y3;')
run('temp',temp)
pattern=r'    p->y3 = ([^\n]+);\n(?:    /\*[^\n]+\*/\n)?    '+re.escape(uv)
for name,op,zero in [('add_expr','+','0'),('or_expr','|','0'),('xor_expr','^','0'),('late_expr','+','((u32)arg0 < 0)')]:
 b,count=re.subn(pattern,lambda m:'    p->y3 = ('+m.group(1)+') '+op+' ('+uv[:-1]+', '+zero+');',body)
 assert count==2,count
 run(name,b)
