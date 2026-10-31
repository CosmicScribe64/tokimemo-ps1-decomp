"""Export the TACO numeric tables read by the port's shooter rules.

Writes two manifests into a new directory under ignored .asset-cache/port/:
  combat-tables.json  schema tokimemo.taco-combat-tables.v1
  enemy-tables.json   schema tokimemo.taco-enemy-tables.v3
Each table is read from verified original bytes at the address named in
config/symbols_TACO.txt (TACO.EXN is headerless, loaded at 0x80132000) or, for
the sine quarter table, at the address rsin() loads in SLPM_86.053.

Usage (Docker only): python -I tools/port/taco_tables.py <input dir|image> <new output dir>
Prints the output SHA-256 of each manifest; exits non-zero on any mismatch.
"""
import hashlib
import json
import struct
import sys
from pathlib import Path

ROOT = next(p for p in Path(__file__).resolve().parents if (p / 'tools/assets/input.py').exists())
sys.path.insert(0, str(ROOT))
from tools.assets.input import open_input  # noqa: E402
from tools.port.school_day_tables import _check_export_path  # noqa: E402

OVERLAY = 'CDROM/EXEDIR/TACO.EXN'
OVERLAY_SHA256 = '1eb8bad166897063bbb9c4bb94dafa426d34b56db5aa55e1b4f9b8d998e29b80'
OVERLAY_SIZE = 0x30000
OVERLAY_BASE = 0x80132000
EXE = 'SLPM_86.053'
EXE_SHA256 = 'bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae'
EXE_BASE = 0x80041000
EXE_HEADER = 0x800
RSIN = 0x800A0070  # config/symbol_addrs_sdk.txt

# name -> (symbol, address, element format, count). Addresses: config/symbols_TACO.txt.
COMBAT = {
    'score_by_class': ('shooter_score_by_class', 0x8015F504, 'i', 17),
    'damage_by_kind': ('shooter_shot_damage_by_kind', 0x8015F548, 'B', 9),
    'se_cooldown_range': ('shooter_se_cooldown_range', 0x8015E22C, 'B', 10),
}
ENEMY = {
    'model_base': ('shooter_enemy_model_base', 0x80161DDC, 'i', 17),
    'joint_count': ('shooter_enemy_joint_count', 0x80161E20, 'B', 17),
    'joint_poses': ('shooter_enemy_joint_poses', 0x80161E60, 'i', 17),
    'scale': ('shooter_enemy_scale', 0x80161EA4, 'h', 17),
    'radius': ('shooter_enemy_radius', 0x80161EC8, 'B', 17),
    'zero_position_a': ('shooter_zero_position_a', 0x801604D4, 'h', 4),
    'zero_position_b': ('shooter_zero_position_b', 0x801604E4, 'h', 4),
    # s16 key script; its extent ends where shooter_enemy_keys_e begins.
    'keys_d': ('shooter_enemy_keys_d', 0x8016090C, 'h', (0x80160A60 - 0x8016090C) // 2),
}


def digest(data):
    return hashlib.sha256(data).hexdigest()


def _read(blob, offset, fmt, count):
    size = struct.calcsize('<' + fmt) * count
    if offset < 0 or offset + size > len(blob):
        raise ValueError('table extent outside original file')
    return list(struct.unpack_from('<%d%s' % (count, fmt), blob, offset))


def _check_symbols():
    text = (ROOT / 'config/symbols_TACO.txt').read_text()
    for spec in list(COMBAT.values()) + list(ENEMY.values()):
        if ('%s = 0x%08X;' % (spec[0], spec[1])) not in text:
            raise ValueError('symbol address differs from config: ' + spec[0])
    if 'shooter_enemy_keys_e = 0x80160A60;' not in text:
        raise ValueError('keys_d end bound differs from config')
    if 'rsin = 0x%08X;' % RSIN not in (ROOT / 'config/symbol_addrs_sdk.txt').read_text():
        raise ValueError('rsin address differs from config')


def _rsin_table(exe):
    """Return the s16 table address that rsin()'s first lookup loads (lui/addu/lh)."""
    off = RSIN - EXE_BASE + EXE_HEADER
    words = struct.unpack_from('<64I', exe, off)
    for i in range(len(words) - 2):
        a, b, c = words[i:i + 3]
        if a >> 26 == 0x0F and c >> 26 == 0x21 and (b & 0xFC00003F) == 0x21:
            imm = c & 0xFFFF
            return ((a & 0xFFFF) << 16) + (imm - 0x10000 if imm & 0x8000 else imm)
    raise ValueError('rsin table load not found')


def _validate_enemy(t):
    """Mirror port/godot/services/shooter_enemy_tables.gd::validate."""
    shapes = {'model_base': 17, 'joint_count': 17, 'joint_poses': 17, 'scale': 17, 'radius': 17,
              'zero_position_a': 4, 'zero_position_b': 4, 'sine_quarter': 1025}
    for key, n in shapes.items():
        if len(t[key]) != n:
            raise ValueError('enemy table shape: ' + key)
    keys = t['keys_d']
    if not 6 <= len(keys) <= 260 or len(keys) % 2:
        raise ValueError('keys_d shape')
    if keys[0] < 0 or keys[1] < keys[0] or keys[1] >= t['joint_count'][13] or keys[2] <= 0 or keys[3] <= 0 or keys[-2] < keys[2]:
        raise ValueError('keys_d bounds')
    sine = t['sine_quarter']
    if sine[0] != 0 or sine[1024] != 4096 or any(sine[i] > sine[i + 1] for i in range(1024)):
        raise ValueError('sine quarter table is not monotonic 0..4096')


def build(input_path):
    _check_symbols()
    with open_input(input_path) as source:
        entries = {e['path']: e for e in source.entries()}
        if OVERLAY not in entries or EXE not in entries:
            raise ValueError('input lacks TACO.EXN or the main executable')
        overlay = source.read(entries[OVERLAY])
        exe = source.read(entries[EXE])
    if len(overlay) != OVERLAY_SIZE or digest(overlay) != OVERLAY_SHA256:
        raise ValueError('TACO.EXN is not the verified original')
    if digest(exe) != EXE_SHA256:
        raise ValueError('main executable is not the verified original')
    combat = {k: _read(overlay, a - OVERLAY_BASE, f, n) for k, (_, a, f, n) in COMBAT.items()}
    enemy = {k: _read(overlay, a - OVERLAY_BASE, f, n) for k, (_, a, f, n) in ENEMY.items()}
    sine_addr = _rsin_table(exe)
    enemy['sine_quarter'] = _read(exe, sine_addr - EXE_BASE + EXE_HEADER, 'h', 1025)
    _validate_enemy(enemy)
    # The manifest depends only on original bytes and this layout, so its pinned
    # digest stays valid while the cited source files are edited.
    origin = {'producer': 'tools/port/taco_tables.py'}
    combat_doc = dict(origin, schema='tokimemo.taco-combat-tables.v1', source_overlay_sha256=OVERLAY_SHA256,
                      symbols={k: [s, a] for k, (s, a, _, _) in COMBAT.items()}, tables=combat)
    enemy_symbols = {k: [s, a] for k, (s, a, _, _) in ENEMY.items()}
    enemy_symbols['sine_quarter'] = ['rsin table', sine_addr]
    enemy_doc = dict(origin, schema='tokimemo.taco-enemy-tables.v3', overlay_sha256=OVERLAY_SHA256,
                     exe_sha256=EXE_SHA256, symbols=enemy_symbols, tables=enemy)
    return combat_doc, enemy_doc


def encode(doc):
    return (json.dumps(doc, sort_keys=True, separators=(',', ':')) + '\n').encode()


def main(argv):
    if len(argv) != 3:
        print(__doc__.strip().splitlines()[-2], file=sys.stderr)
        return 2
    try:
        output, _ = _check_export_path(Path(argv[2]))
        combat, enemy = build(argv[1])
        output.mkdir(parents=True)
        for name, doc in (('combat-tables.json', combat), ('enemy-tables.json', enemy)):
            data = encode(doc)
            (output / name).write_bytes(data)
            print('%s %s %d' % (name, digest(data), len(data)))
    except (ValueError, OSError) as exc:
        print('taco_tables: ' + str(exc), file=sys.stderr)
        return 1
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
