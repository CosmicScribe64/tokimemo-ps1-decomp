"""T-39312: write config/port/engine-diff/<unit>.json from the captures that hit the unit's functions.

usage: mkspec.py --report-dir DIR --captures-root DIR UNIT capture1,capture2,...

Inputs (all committed, no game data): config/port/engine-gen/exclude.json (RAM exclusions that apply to every function of
the units), funcspec_<unit>.json (per-function exclusions), handchecks.json (functions covered by a hand check). A function
with no hit in any capture is listed under "untested" with the reason and, when there is one, its hand check.
"""
import argparse
import json
import os

ROOT = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', '..'))
GEN = os.path.join(ROOT, 'config', 'port', 'engine-gen')


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--report-dir', required=True)
    ap.add_argument('--captures-root', required=True)
    ap.add_argument('unit')
    ap.add_argument('captures')
    a = ap.parse_args()
    report = json.load(open(os.path.join(a.report_dir, a.unit + '.json')))
    hits = {f: 0 for f in report['order']}
    used = []
    for capture in a.captures.split(','):
        index = json.load(open(os.path.join(a.captures_root, capture, 'index.json')))
        got = False
        for f, rows in index.items():
            if f in hits and rows:
                hits[f] += len(rows)
                got = True
        if got:
            used.append(capture)
    hand = json.load(open(os.path.join(GEN, 'handchecks.json'))).get(a.unit, {})
    exclude = json.load(open(os.path.join(GEN, 'exclude.json'))).get(a.unit, json.load(open(os.path.join(GEN, 'exclude.json'))).get('default', []))
    func_path = os.path.join(GEN, 'funcspec_%s.json' % a.unit)
    functions = json.load(open(func_path))['functions'] if os.path.exists(func_path) else {}
    untested = {}
    for f in report['order']:
        if hits[f] == 0:
            reason = 'not reached from any sampled state (school day, events, phone, title, prologue, seasonal event days)'
            untested[f] = reason + '; ' + hand.get(f, 'no hand check either: the body is the mechanical translation of the C that no capture or test exercised (see the ticket notes)')
    spec = {'captures': used, 'exclude': exclude, 'functions': functions, 'untested': untested}
    out = os.path.join(ROOT, 'config', 'port', 'engine-diff', a.unit + '.json')
    json.dump(spec, open(out, 'w'), indent=1)
    print('%s: captures %s, untested %d of %d' % (a.unit, used, len(untested), len(report['order'])))


if __name__ == '__main__':
    main()
