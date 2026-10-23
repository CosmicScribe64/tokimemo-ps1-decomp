# usage: mk.py base.c out.c loopfile [decl]
import sys
b=open(sys.argv[1]).read()
s=b.index('    status = 0;\n'); e=b.index('    if (((*(u8 *)&D_80145FE4)')
loop=open(sys.argv[3]).read()
out=b[:s]+loop+b[e:]
if len(sys.argv)>4:
    out=out.replace('    u32 status;\n','    u32 status;\n'+sys.argv[4]+'\n',1)
open(sys.argv[2],'w').write(out)
