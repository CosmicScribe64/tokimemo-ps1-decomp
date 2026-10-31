"""Gameplay loop: compare the native confirm effect with the emulator golden.

usage: python3 -I compare_confirm_effect.py <test_bank0 out dir> <route golden dir>

test_bank0 writes the engine RAM before and after the command select confirm's
audio_command(3) + (0x500) (ram_before_confirm.bin, ram_after_confirm.bin). The
golden is .asset-cache/emu/route-golden-smoke-v1: select-001 (before the
confirm), enter-30-001 (the queue holds [3, 0x500]) and enter-31-001 (the
pump has run). The SDK voice state written by SpuVmSeKeyOn must equal the
golden after-state: the voice's attribute row (vab 0, program 0, note 0x18,
volume 0x5A), its SPU register shadow, and the voice allocation bits.
"""
import sys

# (address, length, label). 0x801295AA (one byte after the row) is a voice age
# counter that advances per tick and is excluded.
RANGES = (
    (0x80129590, 5, 'SpuVm voice row (vab/program/note/volume)'),
    (0x8012959C, 14, 'SpuVm voice attributes'),
    (0x800E5E98, 12, 'SPU voice register shadow'),
    (0x80129411, 1, 'voice allocation bits a'),
    (0x80129461, 1, 'voice allocation bits b'),
    (0x801294B1, 1, 'voice allocation bits c'),
)


def view(ram, address, length):
    offset = address - 0x80000000
    return ram[offset:offset + length]


def main(argv):
    if len(argv) != 3:
        print(__doc__)
        return 2
    out, golden = argv[1], argv[2]
    native_before = open(out + '/ram_before_confirm.bin', 'rb').read()
    native_after = open(out + '/ram_after_confirm.bin', 'rb').read()
    golden_before = open(golden + '/select-001.ram.bin', 'rb').read()
    golden_after = open(golden + '/enter-31-001.ram.bin', 'rb').read()
    queued = open(golden + '/enter-30-001.ram.bin', 'rb').read()
    failures = 0
    queue = [int.from_bytes(view(queued, 0x80125CC0 + 2 * i, 2), 'little') for i in range(3)]
    if queue != [2, 3, 0x500]:
        print('FAIL: golden enter-30-001 audio queue is not [3, 0x500]:', queue)
        failures += 1
    for address, length, label in RANGES:
        n0, n1 = view(native_before, address, length), view(native_after, address, length)
        g0, g1 = view(golden_before, address, length), view(golden_after, address, length)
        changed = n0 != n1 and g0 != g1
        if n1 != g1 or not changed:
            print('FAIL: %s 0x%08X native %s -> %s, golden %s -> %s' % (label, address, n0.hex(), n1.hex(), g0.hex(), g1.hex()))
            failures += 1
        else:
            print('ok: %s 0x%08X = %s' % (label, address, n1.hex()))
    print('%d failures' % failures)
    return 1 if failures else 0


if __name__ == '__main__':
    raise SystemExit(main(sys.argv))
