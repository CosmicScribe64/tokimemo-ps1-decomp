#!/usr/bin/env python3
"""Inventory T-0018 rows using symptoms and original asm; no compiler diagnosis.

Run in Docker after configure.py. Counts retain duplicate historical rows; unique
functions merge their symptoms. Family mentions are not silently expanded. JSON
includes row line numbers, asm features and unresolved lookups for auditing.
"""
import argparse
from collections import Counter
import json
from pathlib import Path
import re
import sys

import funcloc

import srcscan


def rows(text):
    for line, text_line in enumerate(text.splitlines(), 1):
        cells = text_line.split('|', 4)
        if len(cells) < 5:
            continue
        name = re.match(r'\s*`(\w+)`', cells[2])
        if name:
            yield dict(line=line, file=cells[1].strip(), name=name[1],
                       category=cells[3].strip(), symptom=cells[4].strip(' |'))


def features(asm):
    insns = re.findall(r'/\*[^\n]*?\*/\s+(\w+)\s*([^\n]*)', asm)
    constants = []
    for op, operands in insns:
        if op == 'addiu' and '$zero,' in operands:
            value = operands.rsplit(',', 1)[-1].strip()
            try:
                constants.append(int(value, 0))
            except ValueError:
                pass
    return dict(multu=sum(op == 'multu' for op, _ in insns),
                instructions=len(insns), constants=constants,
                gamestate=any(0x800E6280 <= int(a, 16) < 0x800E7D10
                              for a in re.findall(r'\bD_([0-9A-Fa-f]{8})\b', asm)))


def shape(unit, name, symptom, feat):
    """Exclusive, ordered triage buckets; broad labels are symptom-based."""
    s = symptom.lower()
    if 'normal_date_' in name and any(k in name for k in ('girl', 'fade')):
        return 'fade/girl'
    if unit == 'ENDING' and re.search(r'%\s*(240|0xf0)|remainder', s):
        return 'ENDING remainder'
    if unit == 'DATE' and feat['multu'] >= 4 and any(k in s for k in ('table', 'product', 'cse')):
        return 'DATE repeated products'
    if 2048 in feat['constants'] and 64 in feat['constants'] and 'constant' in s:
        return '0x800/0x40 constants'
    if any(k in s for k in ('multu', 'stride', 'strength reduc')):
        return 'multiply/stride'
    if any(k in s for k in ('switch', 'selector', 'compare chain', 'compare-chain')):
        return 'selector/compare chain'
    if any(k in s for k in ('constant', 'li ', 'shared immediate')):
        return 'constant allocation/sharing'
    if any(k in s for k in ('temporary', 'temporaries', 'temp numbering', 'temp order')):
        return 'temporary numbering/order'
    if any(k in s for k in ('schedul', 'hoist', 'load order', 'store order', 'delay slot')):
        return 'scheduling/order'
    if any(k in s for k in ('pointer', 'base', 'address', 'cse')):
        return 'pointer/base/CSE'
    if any(k in s for k in ('frame', 'stack', 'spill')):
        return 'frame/spill'
    return 'other register/promotion'


def inventory(root, table, name=None, unit=None, note=None):
    """Inventory historical rows, optionally scoped; absent asm is noted and skipped."""
    note = note or (lambda msg: print(msg, file=sys.stderr))
    if name:
        name, unit = funcloc.split_scope(name, unit)
    by_name = {}
    for c in srcscan.c_files(root):
        text = c.src.read_text()
        includes = set(srcscan.INCLUDE_ASM_RE.findall(text))
        names = srcscan.defined_functions(c.src) | {n for _, n in includes}
        for fn in names:
            by_name.setdefault((c.unit, fn), []).append((c, any(n == fn for _, n in includes)))
    records = []
    for row in rows(table.read_text()):
        row_unit = row['file'].split('/')[0]
        if re.fullmatch(r'[0-9A-Fa-f]{8}', row_unit) or row_unit == 'main':
            row_unit = 'main'
        if (name and row['name'] != name) or (unit and row_unit != unit):
            continue
        hits = by_name.get((row_unit, row['name']), [])
        if len(hits) != 1:
            row.update(unit=row_unit, state='unresolved', shape='unresolved', features={})
        else:
            c, unmatched = hits[0]
            paths = [p / (row['name'] + '.s') for p in (c.nonmatchings, c.matchings)]
            asm = next((p for p in paths if p.exists()), None)
            if asm is None:
                note('register_shapes.py: skipping %s:%s: missing original asm (%s)' %
                     (row_unit, row['name'], paths[0]))
                continue
            feat = features(asm.read_text())
            row.update(unit=row_unit, state='open' if unmatched else 'C',
                       features=feat, asm=str(asm), source=str(c.src),
                       shape=shape(row_unit, row['name'], row['symptom'], feat))
        records.append(row)
    unique = {}
    for row in records:
        key = (row['unit'], row['name'])
        if key not in unique:
            unique[key] = dict(row, lines=[row['line']])
        else:
            unique[key]['lines'].append(row['line'])
            unique[key]['symptom'] += '; ' + row['symptom']
    for row in unique.values():
        if row['state'] != 'unresolved':
            row['shape'] = shape(row['unit'], row['name'], row['symptom'], row['features'])
    return records, list(unique.values())


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument('func', nargs='?', help='function name or UNIT:function')
    ap.add_argument('--unit', help='main or overlay name')
    ap.add_argument('--root', type=Path, default=Path('.'))
    ap.add_argument('--rows', type=Path, default=Path('wiki/data/t0018-cases.md'))
    ap.add_argument('--json', type=Path)
    args = ap.parse_args(argv)
    records, unique = inventory(args.root, args.rows, args.func, args.unit)
    for row in unique if args.func else []:
        print('%s:%s: %s (%s); %s' % (row['unit'], row['name'], row['shape'],
                                     row['state'], row['symptom']))
    print(f'{len(records)} rows; {len(unique)} unique functions; states {dict(Counter(r["state"] for r in unique))}')
    print('| Shape | Rows | Unique | Open |')
    print('|---|---:|---:|---:|')
    for label, count in Counter(r['shape'] for r in records).most_common():
        group = [r for r in unique if r['shape'] == label]
        print(f'| {label} | {count} | {len(group)} | {sum(r["state"] == "open" for r in group)} |')
    state_users = [r for r in unique if r['state'] == 'open' and r['features']['gamestate']]
    print('Open functions referencing GameState (overlapping subset):', len(state_users))
    print('  selectors:', sum(r['shape'] == 'selector/compare chain' for r in state_users),
          'pointer/base/CSE:', sum(r['shape'] == 'pointer/base/CSE' for r in state_users))
    if args.json:
        args.json.write_text(json.dumps(dict(rows=records, functions=unique), indent=2) + '\n')
    if any(r['state'] == 'unresolved' for r in records):
        raise SystemExit('Unresolved row identities; inspect JSON before interpreting counts.')


if __name__ == '__main__':
    main()
