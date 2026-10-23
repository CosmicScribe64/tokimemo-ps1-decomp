from pathlib import Path
import sys
sys.path.insert(0,'tools')
import regdiff,cvt_pass as cv
regdiff.WRAPPER=regdiff.WRAPPER.replace('import os, shutil, sys','import os, shutil, sys, subprocess').replace('os.execv(real, [real] + argv)','rc = subprocess.call([real] + argv)\nshutil.copy(files[1], os.environ["REGDIFF_UCODE"] + ".after")\nsys.exit(rc)')
label=sys.argv[1]; name='func_8013EA98'
p=Path('build/ea98')/label/'trace'; src='build/ea98/%s/unit.c'%label
r=regdiff.compile_draft(str(Path.cwd()),name,src,'/work/build/ea98/tr/uopt-trace',str(p),unit='SHOUGATU')
assert r.returncode==0,r.log[-2000:]
for suffix in ('','.after'):
    records=cv.parse((p/('draft.B'+suffix)).read_bytes())
    ch=[(a,b) for a,b in cv._procedures(records) if any(name.encode() in x.extra for x in records[a:b+1])]
    a,b=ch[0]
    (p/('fn'+suffix+'.txt')).write_text('\n'.join('%03d %-5s %s %r'%(i,cv.OPS[x.opc],' '.join('%08x'%w for w in x.words),x.extra) for i,x in enumerate(records[a:b+1]))+'\n')
print('ok',p)
