"""T-39312: print the tools/emu/func_capture/run.sh command that captures functions of the generated modules.

usage: capture_cmd.py --report-dir DIR NAME STATE FRAMES HITS (ALL | ALL:<unit> | fn1,fn2,...) [run.sh options...]

Lists every external callee (functions that no engine module ports) of the selected functions with its address from
port/godot/engine/_symbols.gd, so the harness can replay them. The report directory is the --report-dir of cgen.py.
"""
import argparse
import glob
import json
import os
import re
import sys

ROOT = os.path.normpath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', '..'))
UNITS = ('hud_panels', 'scene_display', 'scene_library')


def load(report_dir):
    reports = {u: json.load(open(os.path.join(report_dir, u + '.json'))) for u in UNITS}
    symbols = open(os.path.join(ROOT, 'port/godot/engine/_symbols.gd')).read()
    addresses = {m.group(1): int(m.group(2), 16) for m in re.finditer(r'"(\w+)": (0x[0-9A-Fa-f]+)', symbols)}
    ported = set(('memcpy', 'memmove', 'strlen', 'strcpy', 'strncpy', 'strcat', 'strncat', 'strcmp', 'bzero'))
    own = set()
    for path in glob.glob(os.path.join(ROOT, 'port/godot/engine/*.gd')):
        name = os.path.basename(path)
        if name.startswith('_') or name == 'engine.gd':
            continue
        m = re.search(r'const C_FUNCTIONS: Array\[String\] = \[(.*?)\]', open(path).read(), re.S)
        if m:
            (own if name[:-3] in UNITS else ported).update(re.findall(r'"(\w+)"', m.group(1)))
    return reports, addresses, ported, own


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--report-dir', required=True)
    ap.add_argument('name')
    ap.add_argument('state')
    ap.add_argument('frames')
    ap.add_argument('hits')
    ap.add_argument('selection')
    ap.add_argument('extra', nargs=argparse.REMAINDER)
    a = ap.parse_args()
    reports, addresses, ported, own = load(a.report_dir)
    if a.selection == 'ALL':
        names = [f for u in UNITS for f in reports[u]['order']]
    elif a.selection.startswith('ALL:'):
        names = reports[a.selection[4:]]['order']
    else:
        names = a.selection.split(',')
    callees = set()
    for report in reports.values():
        for f in names:
            callees.update(report['callees'].get(f, []))
    external = sorted(c for c in callees if c not in own and c not in ported and not c.startswith('<'))
    missing = [c for c in external if c not in addresses]
    if missing:
        sys.exit('no address for: %s' % missing)
    print('tools/emu/func_capture/run.sh %s %s --frames %s %s %s --max-diff 0x200000 --ptr-bytes 32 %s' % (
        a.name, a.state, a.frames,
        ' '.join('--fn %s=0x%X:%s' % (n, addresses[n], a.hits) for n in names),
        ' '.join('--callee %s=0x%X' % (c, addresses[c]) for c in external), ' '.join(a.extra)))


if __name__ == '__main__':
    main()
