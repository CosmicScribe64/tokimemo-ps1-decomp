"""Capture IDO pass files before forwarding unchanged arguments; Docker diagnostics only."""
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

stage = sys.argv[1][2:]
root = Path(os.environ['TEL_CAPTURE']) / stage
root.mkdir(parents=True, exist_ok=True)
args = sys.argv[3:]
(root / 'args.json').write_text(json.dumps(args))
for i, arg in enumerate(args):
    if Path(arg).is_file():
        shutil.copyfile(arg, root / ('before-%02d' % i))
script = {'uopt': 'cvt_pass', 'ugen': 'ugen_pass', 'as1': 'frame_pass'}[stage]
rc = subprocess.call([sys.executable, '/work/tools/' + script + '.py'] + sys.argv[1:])
for i, arg in enumerate(args):
    if Path(arg).is_file():
        shutil.copyfile(arg, root / ('after-%02d' % i))
sys.exit(rc)
