#!/usr/bin/env python3
"""Compare stripping time and exact compiler input against an older permute.py.

Run in Docker from the repo root. Export a reference revision's tools/permute.py
first, then pass --baseline PATH. Defaults to a bounded, deterministic sample of
main and non-TT overlay files. Optional positional paths replace that sample.
No compiler or game binaries are needed. A preprocessing or equality failure
exits nonzero. Timings cover stripping only, not preprocessing or compilation.
"""
import argparse
import importlib.util
import json
from pathlib import Path
import statistics
import subprocess
import time

import permute


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--baseline', required=True, type=Path)
    parser.add_argument('--repeat', type=int, default=3)
    parser.add_argument('files', nargs='*', type=Path)
    args = parser.parse_args()
    if args.repeat < 1:
        parser.error('--repeat must be positive')
    spec = importlib.util.spec_from_file_location('strip_baseline', args.baseline)
    baseline = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(baseline)
    paths = args.files or (sorted(Path('src/main').glob('*.c'))[:8] +
                          sorted(p for p in Path('src/ovl').glob('*/*.c')
                                 if p.parent.name != 'TT')[::30])
    rows = []
    for path in paths:
        pre = subprocess.run(permute.CPP + ['-'],
                             input=permute.strip_include_asm(path.read_text()),
                             text=True, capture_output=True, check=True).stdout
        targets = [m.group(1) for m in permute.SIGNATURE_PREFIX.finditer(pre)
                   if permute.def_regex(m.group(1)).match(pre, m.start())]
        if not targets:
            continue
        target = targets[0]
        samples = [[], []]
        for repeat in range(args.repeat):
            outputs = []
            for idx, module in enumerate((baseline, permute)):
                start = time.perf_counter()
                outputs.append(module.strip_functions(pre, target))
                samples[idx].append(time.perf_counter() - start)
            if outputs[0] != outputs[1]:
                raise SystemExit('stripped compiler input differs: %s' % path)
        row = dict(path=str(path), target=target,
                   names=len(permute.signature_names(pre)),
                   before=statistics.median(samples[0]),
                   after=statistics.median(samples[1]))
        rows.append(row)
        print(json.dumps(row), flush=True)
    if not rows:
        raise SystemExit('no function definitions tested')
    before = sum(r['before'] for r in rows)
    after = sum(r['after'] for r in rows)
    print(json.dumps(dict(files=len(rows), before=before, after=after,
                          speedup=before / after)), flush=True)


if __name__ == '__main__':
    main()
