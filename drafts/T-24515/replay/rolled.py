"""Recover compiler-generated peel/unroll from an ordinary thirteen-record loop."""
from probe import body,probe
name='func_801398EC';base=body(name)
start=base.index('    i = 1;')
loop=base[base.index('        if (tmp == 0)',start):base.index('        if (tmp == 0)',base.index('        if (tmp == 0)',start)+1)]
# The second same-indent condition begins the explicit second record arm.
head=base[:base.index('    if (tmp == 0)')]
single=head+'    i = 0;\n    rec = &D_801217D0[0];\n    do {\n'+loop+'        i++;\n        rec++;\n    } while (i != 13);\n}\n'
probe(name,'natural-single-record',single)
normal=single.replace('    i = 0;\n    rec = &D_801217D0[0];\n    do {','    rec = D_801217D0;\n    for (i = 0; i < 13; i++) {').replace('        i++;\n        rec++;\n    } while (i != 13);','        rec++;\n    }')
probe(name,'normal-for-loop',normal)
