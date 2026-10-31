"""Offline checks for tools/port/taco_tables.py; invented bytes only, no game data."""
import struct
import tempfile
import unittest
from pathlib import Path

from tools.port import taco_tables as T


def _enemy(**override):
    t = {'model_base': [0] * 17, 'joint_count': [6] * 17, 'joint_poses': [0] * 17, 'scale': [0] * 17,
         'radius': [3] * 17, 'zero_position_a': [0] * 4, 'zero_position_b': [0] * 4,
         'keys_d': [0, 5, 15, 15, 15, 1], 'sine_quarter': [i * 4 for i in range(1025)]}
    t.update(override)
    return t


class TacoTablesTests(unittest.TestCase):
    def test_enemy_validation_accepts_source_shape(self):
        T._validate_enemy(_enemy())

    def test_enemy_validation_rejects_bad_tables(self):
        for bad in (_enemy(keys_d=[0, 6, 15, 15, 15, 1]), _enemy(keys_d=[0, 5, 15]),
                    _enemy(sine_quarter=[0] * 1025), _enemy(radius=[3] * 16)):
            with self.assertRaises(ValueError):
                T._validate_enemy(bad)

    def test_rsin_table_decoded_from_load_sequence(self):
        exe = bytearray(T.RSIN - T.EXE_BASE + T.EXE_HEADER + 256)
        code = [0x27BDFFE8, 0x3C01800E, 0x00220821, 0x8422D6C0]  # addiu; lui; addu; lh
        struct.pack_into('<4I', exe, T.RSIN - T.EXE_BASE + T.EXE_HEADER, *code)
        self.assertEqual(T._rsin_table(bytes(exe)), 0x800DD6C0)
        with self.assertRaises(ValueError):
            T._rsin_table(bytes(len(exe)))

    def test_unverified_overlay_refused(self):
        with tempfile.TemporaryDirectory() as root:
            (Path(root) / 'CDROM/EXEDIR').mkdir(parents=True)
            (Path(root) / T.OVERLAY).write_bytes(bytes(T.OVERLAY_SIZE))
            (Path(root) / T.EXE).write_bytes(b'not an exe')
            with self.assertRaisesRegex(ValueError, 'TACO.EXN'):
                T.build(root)

    def test_output_outside_port_cache_refused(self):
        with tempfile.TemporaryDirectory() as root:
            self.assertEqual(T.main(['taco_tables.py', root, str(Path(root) / 'out')]), 1)

    def test_encoding_is_canonical(self):
        self.assertEqual(T.encode({'b': 1, 'a': [1, 2]}), b'{"a":[1,2],"b":1}\n')


if __name__ == '__main__':
    unittest.main()
