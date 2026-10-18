"""Docker-only bounded full-unit scoring of MASTER forms, T-24048."""
import sys, tempfile, json, re
from pathlib import Path
sys.path.insert(0,'tools')
import funcdiff as fd
import funcdiff_verify as proof
name=sys.argv[1]
unit=sys.argv[4] if len(sys.argv)>4 else 'MASTER'
c=fd.locate(name,unit)
e,problem=fd.expected_object(c)
assert not problem,problem
want=fd.resolved_functions(e,[name],c)[name]
text=Path(c.src).read_text()
inc='INCLUDE_ASM("asm/ovl/%s/nonmatchings/%s/%s", %s);'%(unit,unit,Path(c.src).stem,name)
idx=text.index(inc)
start=text.rfind('#ifdef NON_MATCHING\n',0,idx)
if start < 0 or '#endif' in text[start:idx]: start,end=idx,idx+len(inc)
else: end=text.index('#endif',idx)+len('#endif')
prefix = Path(sys.argv[3]).read_text() if len(sys.argv) > 3 else ""
failed = False
for label,body in json.loads(Path(sys.argv[2]).read_text()):
    d=Path(tempfile.mkdtemp(dir='/tmp'))
    src=d/'candidate.c'
    src.write_text(prefix+text[:start]+body+text[end:])
    try:
        b=proof.compile_unit(c,d,source=str(src))[0]
        got=fd.resolved_functions(b,[name])[name]
        diff=fd.diff_lines(want,got)
        score=len(diff)
        print(label,'score',score,'aligned',fd.diff_score(want,got),flush=True)
        out=Path('drafts/T-24048')/label
        trace = Path('build/T-24048-probes')
        trace.mkdir(parents=True, exist_ok=True)
        (trace / (label + '.diff')).write_text('\n'.join(diff))
        out.with_suffix('.c').write_text(body)
    except Exception as ex:
        failed = True
        print(label, 'ERROR', ex, flush=True)
if failed:
    sys.exit(1)
