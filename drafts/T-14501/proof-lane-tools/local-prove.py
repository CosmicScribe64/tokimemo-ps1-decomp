"""Prove an isolated candidate batch against literal default linked bytes."""
import json, re, subprocess, sys
from pathlib import Path
rows=json.loads(Path('build/t14500/rows.json').read_text())
command=[sys.executable,'tools/funcdiff.py','--resolve','--no-build','--version','default']+[r['scope'] for r in rows]
p=subprocess.run(command,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True)
Path('build/t14500/proof.txt').write_text(p.stdout)
if p.returncode or p.stdout.count(': compared default build') != len(rows) or p.stdout.count(': MATCH (linked function bytes equal target (') != len(rows):
    raise SystemExit(p.stdout)
literal_size=sum(int(n) for n in re.findall(r'MATCH \(linked function bytes equal target \((\d+) bytes\)\)',p.stdout))
print('Literal linked proof passed',len(rows),'functions;',literal_size,'literal bytes;',sum(r['size'] for r in rows),'inventory coverage bytes')
