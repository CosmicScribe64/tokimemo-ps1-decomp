"""Compile the two narrow MASTER research bodies against current source and headers in Docker.

Run from the repository root after configure and the MASTER target build.
Only build/t24514/replay is written. Scores are diagnostic; no C match is claimed.
"""
import json
from pathlib import Path
import sys

sys.path.insert(0, 'tools')
import funcdiff
import order_search


def main():
    results = {}
    for name in ('func_80135FB8', 'func_801363F4'):
        context = funcdiff.locate(name, 'MASTER')
        source = Path(context.src).read_text()
        body = Path('drafts/T-24514/' + name + '.c').read_text()
        anchor = 'INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80133680", ' + name + ');'
        if source.count(anchor) != 1:
            raise SystemExit('expected exactly one production assembly anchor for ' + name)
        if name == 'func_801363F4':
            body = 'void func_80137BF4(void);\n' + body
        text = source.replace(anchor, body)
        work = Path('build/t24514/replay') / name
        work.mkdir(parents=True, exist_ok=True)
        score = order_search.make_text_scorer(context, name, resolve=True)(text, work)
        if score >= 1000000:
            raise SystemExit('candidate failed to compile: ' + name)
        results[name] = {'aligned_score': score, 'verified_c_match': False}
        print(name, score, flush=True)
    Path('build/t24514/replay/results.json').write_text(json.dumps(results, indent=2) + '\n')


if __name__ == '__main__':
    main()
