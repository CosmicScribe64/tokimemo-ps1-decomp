"""Replay one archived MAIN body in its current full translation unit (Docker only).

python3 drafts/T-24501/replay.py func_800767A4
Diagnostic edit cost is not a production literal-byte proof.
"""
import queue  # Import stdlib queue before tools/queue.py can shadow it.
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, 'tools')
import funcdiff as diff
import funcdiff_verify as verify

name = sys.argv[1]
ctx = diff.locate(name, 'main')
expected, _ = diff.expected_object(ctx)
want = diff.resolved_functions(expected, [name])[name]
body_path = Path(sys.argv[2]) if len(sys.argv) > 2 else Path('drafts/T-24501/main', name + '.c')
body = body_path.read_text()
# Archived SDK return views must suppress canonical declarations before includes.
prefix = []
while body.startswith('#define MAIN_API_OVERRIDE_'):
    line, body = body.split('\n', 1)
    prefix.append(line)

source = Path(ctx.src).read_text()
stem = Path(ctx.src).stem
marker = 'INCLUDE_ASM("asm/nonmatchings/main/' + stem + '", ' + name + ');'
assert source.count(marker) == 1, 'Expected one ASM marker for this archived function'
Path("build/scratch").mkdir(parents=True, exist_ok=True)

folder = Path(tempfile.mkdtemp(prefix='t24501-', dir='build/scratch'))
variant = folder / 'variant.c'
variant.write_text('\n'.join(prefix) + '\n' + source.replace(marker, body))
obj, _ = verify.compile_unit(ctx, folder, source=variant)
got = diff.resolved_functions(obj, [name])[name]
print(name, 'resolved instruction edit cost', diff.diff_score(want, got))

(folder / 'resolved.diff').write_text('\n'.join(diff.diff_lines(want, got)))
print('diagnostic source/object/diff:', folder)
