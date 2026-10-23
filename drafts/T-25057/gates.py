"""Verify promoted NAME_ENT literal bytes, all binaries and declarations in Docker."""
from pathlib import Path
import hashlib
import json
import subprocess

commands = [
    ['python3', 'tools/funcdiff.py', '--version', 'default', '--resolve', '--no-build', 'NAME_ENT:func_801398EC'],
    ['python3', 'tools/check_headers.py'],
    ['python3', 'tools/migrate_globals.py', '--check'],
    ['python3', 'tools/sync_protos.py', '--check-branch'],
]
for command in commands:
    subprocess.run(command, check=True)
records = []
configs = [Path('config/SLPM_86.053.sha1')] + sorted(Path('config/overlays').glob('*.sha1'))
assert len(configs) == 27, len(configs)
for config in configs:
    binary = Path('build/SLPM_86.053.bin') if config.parent.name != 'overlays' else Path('build/ovl')/(config.stem+'.bin')
    expected = config.read_text().split()[0]
    actual = hashlib.sha1(binary.read_bytes()).hexdigest()
    assert actual == expected, str(binary)
    records.append(dict(binary=str(binary), sha1=actual, verified=True))
Path('drafts/T-25057/gate-results.json').write_text(json.dumps(records, indent=2)+'\n')
print('27/27 binary SHA1 checks passed', flush=True)
