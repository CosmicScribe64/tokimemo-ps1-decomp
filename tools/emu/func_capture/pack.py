"""T-39301: pack a function-differential capture (run.sh output) for the GDScript runner.

usage: python3 -I pack.py <capture dir>

Gzips every <fn>/hitNNNN.entry.ram (2 MiB) to hitNNNN.entry.ram.gz and removes the raw file, checks that each
hit JSON parses and names its RAM image, and writes index.json: for each function the hits, their vsync,
arguments, whether the capture saw unported callees, and the diff sizes. Game data stays under .asset-cache.
"""
import gzip
import json
import sys
from pathlib import Path

RAM_SIZE = 0x200000


def main(argv):
    root = Path(argv[1])
    index = {}
    for fn_dir in sorted(p for p in root.iterdir() if p.is_dir()):
        hits = []
        for js in sorted(fn_dir.glob('hit*.json')):
            hit = json.loads(js.read_text())
            raw = js.with_suffix('').with_suffix('.entry.ram')
            gz = Path(str(raw) + '.gz')
            if raw.exists():
                data = raw.read_bytes()
                if len(data) != RAM_SIZE:
                    raise SystemExit('%s: %d bytes, expected %d' % (raw, len(data), RAM_SIZE))
                gz.write_bytes(gzip.compress(data, 6, mtime=0))
                raw.unlink()
            if not gz.exists():
                raise SystemExit('%s: no entry RAM image' % js)
            hits.append({'hit': hit['hit'], 'vsync': hit['vsync'], 'file': js.name,
                         'args': [hit['a0'], hit['a1'], hit['a2'], hit['a3']],
                         'ret_v0': hit['ret_v0'], 'exit_diff_bytes': hit['exit_diff_bytes'],
                         'callees': [c['name'] for c in hit['callees']]})
        # raw images of dropped hits (the hit JSON was never written) are removed too
        for raw in fn_dir.glob('hit*.entry.ram'):
            raw.unlink()
        if hits:
            index[fn_dir.name] = hits
    (root / 'index.json').write_text(json.dumps(index, indent=1))
    for name, hits in index.items():
        print('%-28s %3d hits, %d with callee records' % (name, len(hits), sum(1 for h in hits if h['callees'])))


main(sys.argv)
