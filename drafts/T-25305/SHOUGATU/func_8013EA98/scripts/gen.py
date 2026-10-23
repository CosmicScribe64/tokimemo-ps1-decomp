# gen.py name loopfile ['decl']: n2 body with loop replaced; G=F5F, R=records, P=D_80146054 view
import sys
b=open('n2.c').read()
s=b.index('    status = 0;\n'); e=b.index('    if (((*(u8 *)&D_80145FE4)')
loop=open(sys.argv[2]).read().replace('G','D_800E6280.unk_F5F').replace('REC','D_800E6280.unk_1BC').replace('PREV','(*(u8 *)&D_80146054)').replace('BASE','D_800E6280.unk_10F8')
out=b[:s]+loop+b[e:]
if len(sys.argv)>3:
    out=out.replace('    u32 status;\n','    u32 status;\n'+sys.argv[3]+'\n',1)
open(sys.argv[1]+'.c','w').write(out)
