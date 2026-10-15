from pathlib import Path
import os, shutil
src=Path('/scratch/integration')
for name in ('asm','expected','build'):
    shutil.copytree(src/name,Path(name),dirs_exist_ok=True)
for p in [src/'disc',*sorted((src/'disc').rglob('*'))]:
    dest=Path(p.relative_to(src))
    if p.is_dir(): dest.mkdir(parents=True,exist_ok=True)
    elif not dest.exists(): os.link(p,dest)
print('Proof inputs copied; disc hardlinked')
