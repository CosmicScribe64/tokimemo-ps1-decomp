#!/usr/bin/env python3
"""Restore ours plus whole other-side additions in headers of a two-parent merge.

Run in Docker: python3 tools/header_merge.py [--root REPO] [--merge HEAD] [--check]
For each .h changed on both sides since the unique merge base, keep first-parent
text and append insert/replace blocks from the second parent. Keep new conditional
and braced declaration blocks whole even when difflib matches their closing lines.
Skip contiguous blocks already present; insert before a trailing include guard.
This deliberately ignores other-side deletions, as the orchestrator's rule does.
--check reports headers needing repair without writing them (exit 1).
"""
import argparse
import difflib
from pathlib import Path
import re
import subprocess
import sys


def guard_end(lines):
    """Index of a trailing #endif closing an outer include guard, or None."""
    clean = re.sub(r'/\*.*?\*/|//[^\n]*',
                   lambda m: ''.join('\n' if c == '\n' else ' ' for c in m[0]),
                   ''.join(lines), flags=re.S).splitlines()
    nonempty = [i for i, line in enumerate(clean) if line.strip()]
    if len(nonempty) < 3:
        return None
    first, second, last = nonempty[0], nonempty[1], nonempty[-1]
    opening = (re.fullmatch(r'\s*#\s*ifndef\s+(\w+)\s*', clean[first]) or
               re.fullmatch(r'\s*#\s*if\s+!\s*defined\s*\(\s*(\w+)\s*\)\s*', clean[first]) or
               re.fullmatch(r'\s*#\s*if\s+!\s*defined\s+(\w+)\s*', clean[first]))
    if not opening or not re.fullmatch(r'\s*#\s*define\s+' + re.escape(opening[1]) +
                                     r'(?:\s+.*)?\s*', clean[second]):
        return None
    depth = 0
    for i in range(first, last + 1):
        if re.match(r'\s*#\s*(if|ifdef|ifndef)\b', clean[i]):
            depth += 1
        elif re.match(r'\s*#\s*endif\b', clean[i]):
            depth -= 1
            if depth == 0:
                return i if i == last else None
    return None


def new_block_ranges(lines, added):
    """Whole new #if and braced declarations, excluding the outer guard."""
    # Preserve line numbers while hiding braces/directives inside comments/literals.
    masked = re.sub(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',
                    lambda m: ''.join('\n' if c == '\n' else ' ' for c in m[0]),
                    ''.join(lines), flags=re.S).splitlines()
    guard = guard_end(lines)
    stack, ranges = [], []
    depth, opening = 0, None
    for i, line in enumerate(masked):
        if re.match(r'\s*#\s*(if|ifdef|ifndef)\b', line):
            stack.append(i)
        elif re.match(r'\s*#\s*endif\b', line) and stack:
            start = stack.pop()
            if start in added and i != guard:
                ranges.append((start, i + 1))
        if line.lstrip().startswith('#'):
            continue
        for c in line:
            if c == '{':
                if depth == 0:
                    opening = i
                depth += 1
            elif c == '}':
                depth -= 1
                if depth == 0 and opening in added:
                    end = i + 1
                    # A typedef name/semicolon can be on the next line.
                    while end < len(masked) and ';' not in masked[end - 1]:
                        if masked[end].lstrip().startswith('#'):
                            break
                        end += 1
                    ranges.append((opening, end))
    return ranges


def merge_header(base, ours, theirs):
    """Pure text merge; preserve first-parent text and whole second-parent additions."""
    b, o, t = (text.splitlines(keepends=True) for text in (base, ours, theirs))
    edits = [(j1, j2) for tag, _, _, j1, j2 in
             difflib.SequenceMatcher(None, b, t, autojunk=False).get_opcodes()
             if tag in ('insert', 'replace') and j1 != j2]
    added = {i for a, z in edits for i in range(a, z)}
    blocks = new_block_ranges(t, added)
    expanded = []
    for a, z in edits:
        for start, end in sorted(blocks, key=lambda r: r[1] - r[0]):
            if a < end and start < z:
                a, z = min(a, start), max(z, end)
        if expanded and a <= expanded[-1][1]:
            expanded[-1] = (expanded[-1][0], max(z, expanded[-1][1]))
        else:
            expanded.append((a, z))
    extra = []
    for a, z in expanded:
        block = ''.join(t[a:z])
        # A set of individual lines can falsely claim an interleaved block exists.
        have = o + extra
        if block.strip() and not any(have[i:i + z - a] == t[a:z] for i in range(len(have) - (z - a) + 1)):
            extra.extend(t[a:z])
    if not extra:
        return ours
    cut = guard_end(o)
    cut = len(o) if cut is None else cut
    before, after = ''.join(o[:cut]), ''.join(o[cut:])
    if before and not before.endswith('\n'):
        before += '\n'
    addition = ''.join(extra)
    if after and not addition.endswith('\n'):
        addition += '\n'
    return before + addition + after


def git(root, *args):
    """Checked git reads; errors must not look like an empty merge base/header."""
    result = subprocess.run(['git', '-C', str(root), *args], capture_output=True)
    if result.returncode:
        raise ValueError(result.stderr.decode(errors='replace').strip())
    return result.stdout.decode('utf-8', errors='surrogateescape')


def repair(root, merge='HEAD', check=False):
    root = Path(git(root, 'rev-parse', '--show-toplevel').strip())
    parents = git(root, 'rev-list', '--parents', '-n', '1', merge).split()
    if len(parents) != 3:
        raise ValueError('expected a two-parent merge commit')
    _, ours, theirs = parents
    bases = git(root, 'merge-base', '--all', ours, theirs).split()
    if len(bases) != 1:
        raise ValueError('expected one merge base')
    base = bases[0]

    def changed(rev):
        # Include deletions: the rule keeps ours even if the other side deleted
        # the header. -z supports spaces in names.
        return {p for p in git(root, 'diff', '--name-only', '-z', base, rev).split('\0')
                if p.endswith('.h')}

    updates = []
    for path in sorted(changed(ours) & changed(theirs)):
        out = merge_header(*(git(root, 'show', rev + ':' + path) if
                             git(root, 'ls-tree', rev, '--', path) else ''
                             for rev in (base, ours, theirs)))
        target = root / path
        if target.is_symlink():
            raise ValueError('refusing symlink header: ' + path)
        current = target.read_bytes().decode('utf-8', errors='surrogateescape') if target.exists() else None
        if out != current and (out or current is not None):
            updates.append((path, out))
    # Compute every result before writing, so git errors never leave a partial repair.
    for path, out in updates:
        if not check:
            (root / path).parent.mkdir(parents=True, exist_ok=True)
            (root / path).write_bytes(out.encode('utf-8', errors='surrogateescape'))
        print(('needs repair' if check else 'rewrote') + ' ' + path)
    return [path for path, _ in updates]


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument('--root', default='.')
    ap.add_argument('--merge', default='HEAD')
    ap.add_argument('--check', action='store_true')
    args = ap.parse_args(argv)
    try:
        changed = repair(args.root, args.merge, args.check)
    except (ValueError, OSError) as exc:
        print('header_merge.py: ' + str(exc), file=sys.stderr)
        return 2
    return 1 if args.check and changed else 0


if __name__ == '__main__':
    sys.exit(main())
