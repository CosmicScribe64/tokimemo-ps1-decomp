# variant.py real_src base.c out_prefix  -- helper lib: units(real, base, variants:dict, outdir)
import sys
def write_units(real, base, V, outdir):
    t=open(real).read()
    assert base in t
    for k,v in V.items():
        open('%s/%s.unit'%(outdir,k),'w').write(t.replace(base,v))
