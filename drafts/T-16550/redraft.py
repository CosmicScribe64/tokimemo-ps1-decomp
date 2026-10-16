#!/usr/bin/env python3
"""T-16550: redraft every unmatched TAIIKU function after the types-first pass; never edits src/.

Run inside the project Docker image from the repo root:
    python3 drafts/T-16550/redraft.py --variant types|fold [--fixes] [--guarded] --out DIR [--files A.c,B.c] [FUNC ...]

Each function goes through tools/exhaustive_drafts.py's process() (m2c, cleanup, cheap compile fixes,
full-translation-unit score), so scores compare with the T-14500 census. Guarded drafts are
regenerated from m2c (TOKIMEMO_REGENERATE_GUARDS). Variants:
  types  m2c with the default context (game.h + include/ovl/TAIIKU.h), nothing else.
  fold   also rewrites the asm handed to m2c: a splat label inside a typed TAIIKU aggregate, inside
         GameState D_800E6280 or inside the D_8011ECD0 display records becomes `base + offset`
         (`%lo(D_80149974)` -> `%lo(D_80149968 + 0xC)`), and the m2c context sees D_8011ECD0 as
         DisplayActor44[]; the draft then writes ((DisplayActor44 *)D_8011ECD0). Without this m2c
         prints the splat label and never uses the struct.
  census the T-14500 census body as it is, scored in today's tree (the baseline of the comparison).
  --fixes  also applies the tools/draft_fixes.py rewrites (m2c.py --fix-defects).
  --fold-scope main  folds only GameState and the display records (separates the TAIIKU types' effect).
"""
import argparse
import json
import os
import re
import sys
from pathlib import Path

sys.path.insert(0, 'tools')
import exhaustive_drafts as ed  # noqa: E402
import m2c  # noqa: E402
import migrate_globals as mg  # noqa: E402

HEADER = 'include/ovl/TAIIKU.h'
DISPLAY = (0x8011ECD0, 0x44 * 160)
EXTERN_RE = re.compile(r'^\s*extern\s+(?:volatile\s+)?(\w+)\s+(D_([0-9A-F]{8}))\s*(?:\[(0x[0-9A-Fa-f]+|\d+)\])?\s*;', re.M)


SIZES = {}


def aggregate_ranges():
    """(start, end, base) for each typed TAIIKU aggregate, GameState and the display records."""
    text = mg.read(HEADER)
    types = mg.parse_types(mg.strip_comments_keep_offsets(text))
    SIZES.update((n, int(v, 16)) for n, v in re.findall(
        r'\}\s*(\w+)\s*;\s*/\*\s*size\s+(0x[0-9A-Fa-f]+)', text))
    # sizes from the `} Name; /* size 0xNN */` comments; the layout parser cannot read nested
    # anonymous structs (TaiHalves), so it is only the fallback
    sizes = SIZES
    out = []
    for tname, base, addr, count in EXTERN_RE.findall(text):
        size = sizes.get(tname)
        if size is None:
            try:
                size = mg.base_type(tname, types).size
            except mg.LayoutError:
                continue
        size *= int(count, 0) if count else 1
        out.append((int(addr, 16), int(addr, 16) + size, base))
    out.append((0x800E6280, 0x800E6280 + 0x1A90, 'D_800E6280'))
    out.append((DISPLAY[0], DISPLAY[0] + DISPLAY[1], 'D_8011ECD0'))
    return sorted(out)


RANGES = None
SYM_RE = re.compile(r'%(hi|lo)\(\s*(D_([0-9A-F]{8}))\s*(?:([+-])\s*(0x[0-9A-Fa-f]+|\d+)\s*)?\)')


def fold(text):
    def rep(m):
        addr = int(m.group(3), 16)
        if m.group(4):
            addr += int(m.group(5), 0) * (-1 if m.group(4) == '-' else 1)
        for start, end, base in RANGES:
            if start <= addr < end and m.group(2) != base:
                off = addr - start
                # m2c reads a lui %hi(sym+k) as the whole address sym+k for every %lo paired with it;
                # give the hi the bare base when its high half is the same, so each %lo keeps its own k
                if m.group(1) == 'hi' and (start + 0x8000) >> 16 == (addr + 0x8000) >> 16:
                    off = 0
                return '%%%s(%s+0x%X)' % (m.group(1), base, off) if off else '%%%s(%s)' % (m.group(1), base)
        return m.group(0)
    return SYM_RE.sub(rep, text)


def table_types():
    """sym -> (type, array field) for globals whose type is a one-array-field struct (stack tables)."""
    text = mg.read(HEADER)
    field = {}
    for m in re.finditer(r'typedef\s+struct\s+\w*\s*\{\s*(?:/\*[^*]*\*/\s*)?\w+\s+(\w+)\[[^\]]+\];\s*\}\s*(\w+)\s*;', text):
        field[m.group(2)] = m.group(1)
    out = {}
    for tname, base, _addr, count in EXTERN_RE.findall(text):
        if tname in field and not count:
            out[base] = (tname, field[tname])
    return out


COPY_HEAD = re.compile(r'^(\s+)(?:\*\(M2cBlock\d+ \*\)\(&)?(sp\w+)\)?(?:\.unk0)? = (?:\*\(M2cBlock\d+ \*\)\(&)?(?:\(\w+\) )?(D_[0-9A-F]{8})\)?(?:\.unk0)?;$', re.M)


def struct_copies(body, tables):
    """m2c prints a struct copy of a constant table to the stack as one assignment per word;
    write it as `spXX = D_YYY;` with spXX of the table's type, and its element uses as spXX.v[i]."""
    for m in list(COPY_HEAD.finditer(body)):
        indent, sp, sym = m.groups()
        if sym not in tables:
            continue
        tname, fld = tables[sym]
        word = r'\*\(\w+ \*\)\(\(unsigned char \*\)\(&%s\) \+ \(\d+\)\) = (?:\(\w+\) )?\*\(\w+ \*\)\(\(unsigned char \*\)\(&%s\) \+ \(\d+\)\);' % (sp, sym)
        lines = body.split('\n')
        out, done = [], False
        for line in lines:
            if not done and line == m.group(0).strip('\n'):
                out.append('%s%s = %s;' % (indent, sp, sym))
                done = True
            elif re.fullmatch(r'\s+' + word, line):
                continue
            else:
                out.append(line)
        if not done:
            continue
        body = '\n'.join(out)
        body = re.sub(r'^(\s+)\w+ %s(?:\[\d+\])?;$' % sp, r'\g<1>%s %s;' % (tname, sp), body, count=1, flags=re.M)
        body = re.sub(r'\(&%s\)\[' % sp, '%s.%s[' % (sp, fld), body)
        body = re.sub(r'&%s\b(?!\.)' % sp, '%s.%s' % (sp, fld), body)
        body = re.sub(r'(?<![.\w])%s\[' % sp, '%s.%s[' % (sp, fld), body)
        body = re.sub(r'(?<![.\w&])%s\b(?![.\w\[])(?! = %s;)' % (sp, sym), '%s.%s[0]' % (sp, fld), body)
        body = body.replace('%s %s.%s[0];' % (tname, sp, fld), '%s %s;' % (tname, sp))
    return body


def install(variant, fixes, scope='all'):
    global RANGES
    RANGES = aggregate_ranges()
    if scope == 'main':
        RANGES = [r for r in RANGES if r[2] in ('D_800E6280', 'D_8011ECD0')]
    if variant == 'census':
        # the T-14500 census body, unchanged, through the same cleanup and full-TU score
        def run_census(root, name, *a, **k):
            func = name.split(':')[-1]
            d = Path('drafts/T-14500/TAIIKU') / func
            decls = (d / 'declarations.h').read_text() if (d / 'declarations.h').exists() else ''
            return decls + '\n' + (d / 'body.c').read_text()
        m2c.run_m2c = run_census
    if variant == 'fold':
        orig_hi = m2c.fix_shared_hi
        m2c.fix_shared_hi = lambda t: orig_hi(fold(t))
        orig_ctx = m2c.build_context

        def build_context(root, heads, out_path, *a, **k):
            orig_ctx(root, heads, out_path, *a, **k)
            s = open(out_path).read()
            # DisplayActor44 is defined after main_api.h's declaration: declare the view at the end
            s2 = re.sub(r'\bextern\s+u8\s+D_8011ECD0\s*\[\s*\]\s*;', '', s) + '\nextern DisplayActor44 D_8011ECD0[];\n'
            open(out_path, 'w').write(s2)
        m2c.build_context = build_context
        orig_split = m2c.split_draft

        sizes = dict(SIZES, DisplayActor44=0x44)
        tables = table_types()

        def split_draft(text, name):
            decls, body = orig_split(text, name)
            # m2c prints `D_8011ECD0 + 0x88` as a byte offset even with the typed view
            def disp(m):
                n = int(m.group(1), 0)
                return '@@REC%d@@' % (n // 0x44) if n % 0x44 == 0 else '@@BYTE%d@@' % n
            body = re.sub(r'\bD_8011ECD0 \+ (0x[0-9A-Fa-f]+|\d+)\b', disp, body)
            body = re.sub(r'\bD_8011ECD0\b', '((DisplayActor44 *)D_8011ECD0)', body)
            body = re.sub(r'@@REC(\d+)@@', r'&((DisplayActor44 *)D_8011ECD0)[\1]', body)
            body = re.sub(r'@@BYTE(\d+)@@', lambda m: '(DisplayActor44 *)(D_8011ECD0 + 0x%X)' % int(m.group(1)), body)
            # m2c steps a typed record pointer by its byte size (`p += 0x1C;`): one element
            for tname, var in re.findall(r'^\s+(\w+) \*(\w+);$', body, re.M):
                if tname in sizes:
                    body = re.sub(r'\b%s ([+-])= 0x%X;' % (var, sizes[tname]), r'%s \1= 1;' % var, body)
            body = struct_copies(body, tables)
            return decls, body
        m2c.split_draft = split_draft
    if fixes:
        orig_run = m2c.run_m2c

        def run_m2c(*a, **k):
            k['fixes'] = True
            return orig_run(*a, **k)
        m2c.run_m2c = run_m2c


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument('--variant', choices=('types', 'fold', 'census'), required=True)
    ap.add_argument('--fold-scope', choices=('all', 'main'), default='all',
                    help='main: fold only GameState and the display records, not the TAIIKU types')
    ap.add_argument('--fixes', action='store_true')
    ap.add_argument('--out', type=Path, required=True)
    ap.add_argument('--files', help='comma-separated src files to restrict to')
    ap.add_argument('--guarded', action='store_true',
                    help='score the guarded drafts in src as they are (with the new headers) instead of m2c')
    ap.add_argument('funcs', nargs='*')
    a = ap.parse_args()
    if not a.guarded:
        os.environ['TOKIMEMO_REGENERATE_GUARDS'] = '1'
    install(a.variant, a.fixes, a.fold_scope)
    ed.BASE = a.out
    a.out.mkdir(parents=True, exist_ok=True)
    env = a.out / 'environment.json'
    if not env.exists():
        env.write_text(Path('drafts/T-14500/environment.json').read_text())
    rows = [r for r in ed.census() if r['unit'] == 'TAIIKU']
    if a.files:
        keep = set(a.files.split(','))
        rows = [r for r in rows if os.path.basename(r['src']) in keep]
    if a.funcs:
        rows = [r for r in rows if r['func'] in a.funcs]
    if a.guarded:
        rows = [r for r in rows if r['guarded']]
    for r in rows:
        if (a.out / r['unit'] / r['func'] / 'result.json').exists():
            continue
        try:
            got = ed.process(r)
        except Exception as exc:  # one failure must not stop the batch
            got = dict(r, status='driver-error', reason=str(exc))
        got['redraft_variant'] = 'guarded' if a.guarded else a.variant + ('+fixes' if a.fixes else '') + (
            '-mainonly' if a.fold_scope == 'main' else '')
        print(r['func'], got.get('status'), got.get('score'), (got.get('reason') or '')[:120], flush=True)
        (a.out / r['unit'] / r['func']).mkdir(parents=True, exist_ok=True)
        (a.out / r['unit'] / r['func'] / 'result.json').write_text(json.dumps(got, sort_keys=True) + '\n')


if __name__ == '__main__':
    main()
