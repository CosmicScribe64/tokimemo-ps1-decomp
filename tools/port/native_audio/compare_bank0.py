"""native-bank0: compare test_bank0 logs with the M3 emulator run.

usage: python3 -I compare_bank0.py <m3 run dir> <test_bank0 out dir>
       python3 -I compare_bank0.py --emit <m3 run dir> <expectations.json>
--emit writes the per-pump M3 gain/MVOL/key-on table that
port/godot/tests/opening/run_bank0_native_audio.gd checks the extension against.

Timeline row f (state at vsync f) is native mix-time pump f-R; an SPU event tagged
frame E is native pump E-R (R = 2206, the stage10/substep8 C1 call frame, as in M2).
Checks gain/MVOL at every pump of the span from the stage16 commands (R+4759) through
spring start (R+7255), and the per-pump key-on sample addresses
over the same span. Voice channel numbers are not compared (the SDK allocator
starts from different prior state, documented for M2).
"""
import json, struct, sys
R = 2206
WINDOWS = [(4759, 7255)]
emit = sys.argv[1] == '--emit'
if emit: sys.argv.pop(1)
run, out = sys.argv[1], sys.argv[2]
rows = {}
for line in open(run + '/frames.jsonl'):
    r = json.loads(line); rows[r['f']] = r
kon = {}; start = {}; theirs = {}
for line in open(run + '/events.jsonl'):
    e = json.loads(line)
    if e['k'] != 'spu' or e['v'] is None: continue
    a = e['a']
    if a < 0x1F801D80 and (a - 0x1F801C00) % 16 == 6:
        start[(a - 0x1F801C00) // 16] = e['v'] * 8
    elif a in (0x1F801D88, 0x1F801D8A) and e['v']:
        mask = e['v'] << (0 if a == 0x1F801D88 else 16)
        for ch in range(24):
            if mask & (1 << ch): theirs.setdefault(e['f'] - R, []).append(start.get(ch))
if emit:
    table = {}
    for lo, hi in WINDOWS:
        for p in range(lo, hi + 1):
            a = bytes.fromhex(rows[p + R]['as'])
            table[str(p)] = [struct.unpack_from('<H', bytes.fromhex(rows[p + R]['hwspu']), 0)[0], struct.unpack_from('<h', a, 0x20)[0],
                             sorted(x for x in theirs.get(p, []) if x is not None)]
    json.dump({'schema': 'tokimemo.bank0-m3-expectations.v1', 'R': R, 'windows': WINDOWS, 'skip_gain': [4770], 'pumps': table},
              open(out, 'w'), sort_keys=True, separators=(',', ':'))
    sys.exit(0)
log = {}
for line in open(out + '/pump_log.txt'):
    p, mv, gain, _ = map(int, line.split()); log[p] = (mv, gain)
ours = {}
for line in open(out + '/kon_log.txt'):
    p, _, addr = map(int, line.split()); ours.setdefault(p, []).append(addr)
failures = []; checked = 0; kons = 0
for lo, hi in WINDOWS:
    for p in range(lo, hi + 1):
        f = p + R
        if f not in rows or p not in log: failures.append('missing row/pump %d' % p); continue
        a = bytes.fromhex(rows[f]['as'])
        gain = struct.unpack_from('<h', a, 0x20)[0]
        mvol = struct.unpack_from('<H', bytes.fromhex(rows[f]['hwspu']), 0)[0]
        # bgm_volume is not touched by a 0x11 fade finish; the native gain is the same field.
        # Row R+4770 is sampled mid audio_bgm_start (the vsync fell inside its reverb-clear wait).
        if p != 4770 and log[p] != (mvol, gain): failures.append('pump %d native MVOL/gain %s, M3 %s' % (p, log[p], (mvol, gain)))
        checked += 1
        a_set, b_set = sorted(ours.get(p, [])), sorted(x for x in theirs.get(p, []) if x is not None)
        kons += len(b_set)
        if a_set != b_set: failures.append('pump %d KON samples native %s, M3 %s' % (p, [hex(x) for x in a_set], [hex(x) for x in b_set]))
print('compared %d pumps, %d M3 key-ons' % (checked, kons))
for f in failures[:400]: print('FAIL:', f)
print('%d failures' % len(failures))
sys.exit(1 if failures else 0)
