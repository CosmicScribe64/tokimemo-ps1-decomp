# mk.py real_src func body out -> writes guarded unit copy
import sys,re
real,name,body,out=sys.argv[1:5]
t=open(real).read()
m=re.search(r'INCLUDE_ASM\("[^"]*", %s\);\n'%name,t)
t=t[:m.start()]+"#ifdef NON_MATCHING\n"+open(body).read()+"\n#else\n"+m.group(0)+"#endif\n"+t[m.end():]
open(out,'w').write(t)
