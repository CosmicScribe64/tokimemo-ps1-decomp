"""Fresh-context baseline for the audited NAME_ENT introduction reconstruction."""
from pathlib import Path
from probe import probe,ROOT
name='func_80143580';out=ROOT/name/'typed-archive-control';out.mkdir(parents=True,exist_ok=True)
old=Path('drafts/T-20008/NAME_ENT/func_80143580')
(out/'declarations.h').write_text((old/'declarations.h').read_text())
probe(name,'typed-archive-control',(old/'body.c').read_text())
