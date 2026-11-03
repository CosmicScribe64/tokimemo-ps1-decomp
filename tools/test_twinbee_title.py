"""T-39548: keep the TwinBee title replacement inside its two sprite regions."""
import os
import unittest
from pathlib import Path

from tools.patch.art import ArtBuildError, twinbee
from tools.patch.disc import Track, Iso

SOURCE = Path('disc/files') / twinbee.PATH


@unittest.skipUnless(SOURCE.is_file(), 'requires original disc assets')
class TwinBeeTitle(unittest.TestCase):
    def test_only_logo_regions_change(self):
        original = SOURCE.read_bytes()
        written, facts = twinbee.place(original)
        self.assertEqual(len(written), len(original))
        self.assertGreater(facts['changed_pixels'], 1000)
        self.assertEqual(written[:0x50000], original[:0x50000])
        self.assertEqual(written[0x52000:], original[0x52000:])
        # The second piece occupies only the left half of the lower 64 rows.
        for row in range(64, 128):
            at = 0x50000 + row * 64 + 32
            self.assertEqual(written[at:at + 32], original[at:at + 32])
        with self.assertRaises(ArtBuildError):
            twinbee.verify(original, original)
        damaged = bytearray(written)
        damaged[0x52000] ^= 1
        with self.assertRaises(ArtBuildError):
            twinbee.verify(original, bytes(damaged))

    def test_unknown_source_is_rejected(self):
        original = bytearray(SOURCE.read_bytes())
        original[0] ^= 1
        with self.assertRaises(ArtBuildError):
            twinbee.place(bytes(original))

    @unittest.skipUnless(os.environ.get('TOKIMEMO_ENGLISH_DISC') == '1', 'requires rebuilt disc')
    def test_final_disc_contains_title(self):
        track = Track('build/en/disc/track1.bin')
        try:
            result = twinbee.verify(SOURCE.read_bytes(), Iso(track).read(twinbee.PATH))
            self.assertEqual(result['images'][twinbee.IMAGE_ID], 'verified')
        finally:
            track.close()


if __name__ == '__main__':
    unittest.main()
