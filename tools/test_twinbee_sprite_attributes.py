"""T-39547: translation must preserve TwinBee texture attributes and bindings.

Run in Docker with TOKIMEMO_ENGLISH_DISC=1 after building the English disc.
"""
import os
import re
import struct
import unittest
from pathlib import Path

from tools import english_link
from tools.patch.disc import Iso, Track


@unittest.skipUnless(os.environ.get('TOKIMEMO_ENGLISH_DISC') == '1', 'requires English disc')
class TwinBeeSpriteAttributes(unittest.TestCase):
    def test_all_sprite_attributes_match_original(self):
        original = Path('disc/files/CDROM/EXEDIR/TT.EXN').read_bytes()
        symbols = english_link.symbols('build/en/ovl/TT.elf')
        config = Path('config/symbols_TT.txt').read_text()
        # Sets 7 and 8 retain assembly labels; recover all twelve original locations
        # from the original sprite-set pointer table, in its stable index order.
        table = int(re.search(r'tt_sprite_set_table = (0x[0-9A-Fa-f]+)', config)[1], 16)
        base = english_link.OVERLAY_SLOT
        with_track = Track('build/en/disc/track1.bin')
        try:
            current = Iso(with_track).read('CDROM/EXEDIR/TT.EXN')
            new_table = symbols['tt_sprite_set_table'][0] - base
            for index in range(12):
                with self.subTest(sprite_set=index):
                    old_set = struct.unpack_from('<I', original, table - base + index * 4)[0] - base
                    new_set = struct.unpack_from('<I', current, new_table + index * 4)[0] - base
                    old_attr = struct.unpack_from('<I', original, old_set + 12)[0] - base
                    new_attr = struct.unpack_from('<I', current, new_set + 12)[0] - base
                    # These arrays immediately precede their descriptor; set 11
                    # has eight bytes, while the other eleven occupy four.
                    size = old_set - old_attr
                    self.assertIn(size, (4, 8))
                    self.assertEqual(current[new_attr:new_attr + size], original[old_attr:old_attr + size])
        finally:
            with_track.close()


if __name__ == '__main__':
    unittest.main()
