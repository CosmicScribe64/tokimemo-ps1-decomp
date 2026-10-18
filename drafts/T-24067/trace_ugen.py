"""Private logging-only ugen diagnostic; run in Docker with /scratch read-only."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import sys

ROOT = Path('build/etc24067/ugen-state')
REFERENCE = Path('build/etc24067/ugen-reference/ugen.c')
EXPECTED = '4079660f9ebb068a791c6f54abb67cf7c3bb792207c89a6eb9ac2d9e146341ec'


def digest(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def generate():
    if digest(REFERENCE) != EXPECTED:
        raise SystemExit('reference hash changed')
    text = REFERENCE.read_text()
    helper = r'''
/* Native logging state only; no emulated-memory or allocator writes. */
static void etc_copy_log(uint8_t *mem, const char *phase,
                         uint32_t size, uint32_t src, uint32_t dst) {
    if (size == 8 && src == 0x75e) {
        fprintf(stderr, "ETC_COPY %s src=%x dst=%x free=%u/%u used=%u/%u fp-free=%u/%u\n",
                phase, src, dst,
                MEM_U8(0x10019da4), MEM_U8(0x10019da5),
                MEM_U8(0x10019da8), MEM_U8(0x10019da9),
                MEM_U8(0x10019dac), MEM_U8(0x10019dad));
    }
}
'''
    anchor = 'static void f_eval_mov(uint8_t *mem, uint32_t sp, uint32_t a0) {'
    if text.count(anchor) != 1:
        raise SystemExit('eval_mov definition changed')
    text = text.replace(anchor, helper + '\n' + anchor)
    for label, phase in [('L42a8e0', 'before-destination'),
                         ('L42a904', 'after-destination-before-source'),
                         ('L42a914', 'after-source'),
                         ('L42aa44', 'before-dummy'),
                         ('L42aa84', 'after-dummy'),
                         ('L42ab84', 'before-copy-selection'),
                         ('L42abb4', 'after-second-scratch')]:
        needle = label + ':\n'
        if text.count(needle) != 1:
            raise SystemExit('label changed: ' + label)
        text = text.replace(needle, needle +
            'etc_copy_log(mem, "' + phase + '", s1, s2, s3);\n')
    ROOT.mkdir(parents=True, exist_ok=True)
    output = ROOT / 'ugen-log.c'
    output.write_text(text)
    subprocess.run(['cc', '-std=c11', '-Os', '-fno-strict-aliasing',
        '-I/scratch/recomp', '-o', str(ROOT / 'ugen-log'), str(output),
        '/scratch/recomp/build/5.3/libc_impl.o',
        '/scratch/recomp/build/5.3/version_info.o', '-lm'], check=True)
    print('Private logging binary generated; audit required before using logs.')


def compile_and_audit():
    sys.path.insert(0, 'tools')
    import cc
    source = Path('build/etc24067/archive/continue/source.c')
    stock, logged = ROOT / 'stock.o', ROOT / 'logged.o'
    saved_env = cc.ido_frame_env
    cc.main(['cc.py', str(source), str(stock), 'ido', '5.3'])

    def environment(version, temp, extra_shims=None):
        env = saved_env(version, temp, extra_shims)
        shim = Path(temp) / 'ugen'
        if not shim.is_symlink() or str(shim.readlink()) != '/opt/ido/5.3/ugen':
            raise RuntimeError('unexpected production ugen path')
        shim.unlink()
        shim.symlink_to((ROOT / 'ugen-log').resolve())
        return env

    cc.ido_frame_env = environment
    try:
        cc.main(['cc.py', str(source), str(logged), 'ido', '5.3'])
    finally:
        cc.ido_frame_env = saved_env
    audit()


def audit():
    sys.path.insert(0, 'tools')
    from trailing_pad import Elf
    source = Path('build/etc24067/archive/continue/source.c')
    stock, logged = ROOT / 'stock.o', ROOT / 'logged.o'
    sections = {}
    a, b = Elf(stock.read_bytes()), Elf(logged.read_bytes())
    for name in ['.text', '.rel.text', '.rodata', '.rel.rodata']:
        x, y = a.sections[a.find(name)]['data'], b.sections[b.find(name)]['data']
        if x != y:
            raise SystemExit('section mismatch: ' + name)
        sections[name] = len(x)
    if stock.read_bytes() != logged.read_bytes():
        raise SystemExit('whole object differs; do not trust logs')
    manifest = {str(path): digest(path) for path in [source, REFERENCE,
        ROOT / 'ugen-log.c', ROOT / 'ugen-log', stock, logged]}
    manifest['equal_sections'] = sections
    (ROOT / 'audit.json').write_text(json.dumps(manifest, indent=2) + '\n')
    print('PASS: same-input whole object and code/data/relocations identical.')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compile-audit', action='store_true')
    parser.add_argument('--audit', action='store_true')
    args = parser.parse_args()
    if args.audit:
        audit()
    elif args.compile_audit:
        compile_and_audit()
    else:
        generate()
