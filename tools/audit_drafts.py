#!/usr/bin/env python3
"""T-14500: audit complete draft evidence and residual synthetic expressions.

Run in Docker: python3 tools/audit_drafts.py --archive drafts/T-14500
Compilation and reconstruction quality are separate. This conservative pattern audit
identifies placeholders, inferred stack locals, suspicious scaled address arithmetic,
and generic copy layouts; it does not prove the semantics of an audit-clear draft.
Initial outcomes are immutable. Context-qualified repair alternatives are ranked by
compilability, unresolved flags, then full-TU score and retained in a separate catalog.
"""
import argparse
from collections import Counter
import json
from pathlib import Path
import re
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))
import autotweak
import bulk_m2c as bulk

STACK_DECL = re.compile(r'^\s*(?:s8|u8|s16|u16|s32|u32|void)\s+\**\s*(sp[0-9A-Fa-f]+)\s*(\[[^]]*\])?\s*;', re.M)
RAW_STACK_DECL = re.compile(r'^\s*(?:\?|(?:[A-Za-z_]\w*\s+)*[A-Za-z_]\w*)\s+\**\s*(sp[0-9A-Fa-f]+)\s*(?:\[[^]]*\])?\s*;', re.M)


def residuals(body, raw):
    """Structured review flags; a flag requires inspection, not an automatic semantic verdict."""
    masked = autotweak.mask(body)
    flags = []
    def add(kind, detail, start=0):
        flags.append(dict(kind=kind, detail=detail, line=body.count('\n', 0, start)+1))
    for m in re.finditer(r'\bM2C_[A-Za-z_0-9]+\b', masked):
        add('m2c-placeholder', m.group(), m.start())
    original = set(RAW_STACK_DECL.findall(autotweak.mask(raw)))
    original = {x[0] if isinstance(x, tuple) else x for x in original}
    for m in STACK_DECL.finditer(masked):
        name, dims = m.groups()
        if raw and name not in original:
            add('synthesized-stack-local', name + (dims or ''), m.start())
    def byte_view(m):
        return bool(re.search(r'\(\s*(?:u8|s8|char|unsigned\s+char|signed\s+char)\s*\*\s*\)\s*$', masked[:m.start()]))
    for m in re.finditer(r'&\s*(sp[0-9A-Fa-f]+)\s*(?:\[\s*0\s*\])?\s*[+\-]', masked):
        if byte_view(m):
            continue
        add('stack-address-arithmetic', m.group(), m.start())
    for m in re.finditer(r'&\s*D_800E6280\s*[+\-]', masked):
        if byte_view(m):
            continue
        add('gamestate-address-arithmetic', m.group(), m.start())
    for m in re.finditer(r'\bM2cBlock[0-9A-Fa-f]+\b', masked):
        if not any(f['kind']=='inferred-copy-layout' and f['detail']==m.group() for f in flags):
            add('inferred-copy-layout', m.group(), m.start())
    for m in re.finditer(r'[+]\s*&\s*(sp[0-9A-Fa-f]+)\b(?!\s*[.\[])', masked):
        add('stack-address-arithmetic', m.group(), m.start())
    for m in re.finditer(r'[+]\s*&\s*D_800E6280\b(?!\s*[.\[])', masked):
        add('gamestate-address-arithmetic', m.group(), m.start())
    return flags


def manual_catalogs(base):
    for catalog in (base / 'repairs').rglob('catalog.*'):
        if 'automatic' in catalog.parts or catalog.suffix not in ('.json','.jsonl'):
            continue
        entries = json.loads(catalog.read_text()) if catalog.suffix=='.json' else [json.loads(line) for line in catalog.read_text().splitlines()]
        for r in entries:
            path = r.get('repair_path') or r.get('path')
            if path:
                result = Path(path) / 'result.json'
                complete = json.loads(result.read_text()) if result.exists() else {}
                yield {**complete, **r, "repair_path": path}


def raw_body(base, row):
    path = base / row['unit'] / row['func'] / 'm2c.c'
    if path.exists():
        return path.read_text()
    src = base / 'baseline' / row['src']
    if src.exists() and row['guarded']:
        text = src.read_text()
        for guard in autotweak.guards(text):
            if guard.func == row['func']:
                return text[guard.body:guard.else_start]
    return ''


def alternatives(base, rows):
    found = {r['scope']: [] for r in rows}
    for row in rows:
        folder = base / row['unit'] / row['func']
        result = folder / 'result.json'
        if result.exists():
            found[row['scope']].append(dict(path=str(folder), result=json.loads(result.read_text()), origin='initial'))
    # Automatic alternatives have full result schemas; manual catalogs carry their complete scope/context.
    for result in (base / 'repairs' / 'automatic').glob('*/*/*/result.json'):
        r = json.loads(result.read_text())
        if r.get('scope') in found:
            found[r['scope']].append(dict(path=str(result.parent), result=r, origin='automatic'))
    for r in manual_catalogs(base):
        if r.get('scope') in found:
            found[r['scope']].append(dict(path=r['repair_path'],result=r,origin='manual'))
    return found


def audit(base):
    rows = json.loads((base / 'inventory.json').read_text())
    candidates = alternatives(base, rows)
    audit_rows = []
    for row in rows:
        raw = raw_body(base, row)
        inspected = []
        for candidate in candidates[row['scope']]:
            folder = Path(candidate['path'])
            body = folder / 'body.c'
            context = folder / 'context.c'
            r = candidate['result']
            compiled = r.get('score') is not None and r.get('status') in ('scored','candidate-zero')
            flags = residuals(body.read_text(), raw) if body.exists() else [dict(kind='missing-body',detail='no generated C body',line=0)]
            if not context.exists():
                flags.append(dict(kind='missing-context',detail='no complete candidate translation unit',line=0))
            inspected.append(dict(path=candidate['path'],origin=candidate['origin'],compiled=compiled,
                                  score=r.get('score'),status=r.get('status'),flags=flags,
                                  reason=r.get('reason')))
        if inspected:
            winner = min(inspected,key=lambda c:(not c['compiled'],bool(c['flags']),c['score'] if c['score'] is not None else 10**9,len(c['flags'])))
            status = 'compiling-review-needed' if winner['compiled'] and winner['flags'] else 'compiling-audit-clear' if winner['compiled'] else 'uncompilable'
            selected = winner['path']
            flags = winner['flags']
        else:
            status,selected,flags='unattempted',None,[]
        audit_rows.append(dict(scope=row['scope'],src=row['src'],size=row['size'],status=status,
                               selected_path=selected,flags=flags,alternatives=inspected))
    summary = dict(total=len(rows),statuses=dict(Counter(r['status'] for r in audit_rows)),
                   flag_counts=dict(Counter(f['kind'] for r in audit_rows for f in r['flags'])),
                   qualification='Audit-clear means no listed residual pattern; semantic equivalence is unproven.')
    (base/'quality-audit.jsonl').write_text(''.join(json.dumps(r,sort_keys=True)+'\n' for r in audit_rows))
    (base/'quality-summary.json').write_text(json.dumps(summary,indent=2)+'\n')
    return summary


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--archive',type=Path,default=Path('drafts/T-14500'))
    args=p.parse_args()
    print(json.dumps(audit(args.archive),sort_keys=True))


if __name__=='__main__':
    main()
