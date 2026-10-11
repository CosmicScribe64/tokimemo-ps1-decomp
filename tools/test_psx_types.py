#!/usr/bin/env python3
"""Check fresh PsyQ layouts against PS1 ABI sizes/offsets using IDO (T-9610).

Run in Docker: python3 tools/test_psx_types.py. No game data is required.
"""
import os
from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parent.parent
CC = Path(os.environ.get('IDO_DIR', '/opt/ido')) / '5.3/cc'


class LayoutTests(unittest.TestCase):
    def test_packet_and_geometry_abi(self):
        sizes = {
            'RECT': 8, 'SVECTOR': 8, 'VECTOR': 16, 'MATRIX': 32,
            'POLY_F3': 20, 'POLY_F4': 24, 'POLY_FT3': 32, 'POLY_FT4': 40,
            'POLY_G3': 28, 'POLY_G4': 36, 'POLY_GT3': 40, 'POLY_GT4': 52,
            'SPRT': 20, 'SPRT_8': 16, 'SPRT_16': 16,
            'TILE': 16, 'TILE_1': 12, 'TILE_8': 12, 'TILE_16': 12,
            'DR_MODE': 12, 'DR_TWIN': 12, 'DR_AREA': 12, 'DR_OFFSET': 12,
            'DR_TPAGE': 8, 'DR_STP': 12, 'DR_MOVE': 24, 'DR_ENV': 64, 'DR_LOAD': 68,
        }
        offsets = {
            'RECT': {'h': 6}, 'SVECTOR': {'vz': 4, 'pad': 6},
            'VECTOR': {'vz': 8, 'pad': 12}, 'MATRIX': {'m[2][2]': 16, 't': 20},
            'POLY_F3': {'code': 7, 'x2': 16}, 'POLY_F4': {'x3': 20},
            'POLY_FT3': {'clut': 14, 'tpage': 22, 'u2': 28},
            'POLY_FT4': {'x3': 32, 'u3': 36},
            'POLY_G3': {'r1': 12, 'x2': 24}, 'POLY_G4': {'r3': 28, 'x3': 32},
            'POLY_GT3': {'r1': 16, 'tpage': 26, 'x2': 32, 'u2': 36},
            'POLY_GT4': {'r3': 40, 'x3': 44, 'u3': 48},
            'SPRT': {'code': 7, 'x0': 8, 'u0': 12, 'clut': 14, 'w': 16, 'h': 18},
            'TILE': {'w': 12, 'h': 14}, 'DR_LOAD': {'code': 4, 'p': 16},
        }
        lines = ['#include "psx_types.h"', '#include "libgte.h"',
                 '#include "psx_types.h"', '#include "libgte.h"']
        for name, size in sizes.items():
            lines.append(f'typedef char size_{name}[(sizeof({name}) == {size}) ? 1 : -1];')
        for name, fields in offsets.items():
            for index, (field, offset) in enumerate(fields.items()):
                lines.append(f'typedef char offset_{name}_{index}'
                             f'[((u32)&(({name} *)0)->{field} == {offset}) ? 1 : -1];')
        with tempfile.TemporaryDirectory(prefix='psx-layout-') as directory:
            source = Path(directory) / 'layout.c'
            source.write_text('\n'.join(lines) + '\n')
            result = subprocess.run([str(CC), '-c', '-EL', '-mips1', '-cckr',
                                     '-I' + str(ROOT / 'include'), str(source),
                                     '-o', str(Path(directory) / 'layout.o')],
                                    capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)


if __name__ == '__main__':
    unittest.main()
