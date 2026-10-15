#!/usr/bin/env python3
"""Prepare a source-only frozen analysis root for T-15800; link existing immutable build inputs."""
from pathlib import Path
import shutil
root=Path('/work/build/t15800/root');root.mkdir(parents=True,exist_ok=True)
base=Path('/work/drafts/T-14500/baseline')
if not base.exists():base=Path('/scratch/drafts-14500/drafts/T-14500/baseline')
for name in ['include','src']:shutil.copytree(base/name,root/name,dirs_exist_ok=True)
for name in ['tools','config','asm','expected','disc']:
 dest=root/name
 if not dest.exists():dest.symlink_to('/work/'+name,target_is_directory=True)
print('Frozen source/header analysis root ready; no game inputs copied')
