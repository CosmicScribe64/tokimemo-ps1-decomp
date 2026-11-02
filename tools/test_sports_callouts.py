"""T-39521: source identity, palette and byte-preservation checks for sports bubbles."""
import tempfile
import unittest
from pathlib import Path

from tools.patch.art import ArtBuildError, sports_callouts, sprites
from tools.patch.art.verify import verify_sprites

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / 'disc/files' / sports_callouts.PATH


class SportsCalloutTest(unittest.TestCase):
    def test_unknown_source_is_rejected(self):
        with self.assertRaisesRegex(ArtBuildError, 'source differs'):
            sports_callouts.render(bytes(0x27800))

    @unittest.skipUnless(SOURCE.is_file(), 'original NINSETU unavailable')
    def test_round_trip_changes_only_bubble_texels(self):
        original = SOURCE.read_bytes()
        with tempfile.TemporaryDirectory() as folder:
            items = sports_callouts.items(Path(folder), ROOT / 'disc/files')
            result = sprites.place_container(sports_callouts.PATH, original, b'', items)
            self.assertEqual(set(result.images.values()), {'in_place'})
            proof = verify_sprites(sports_callouts.PATH, original, result.data,
                                   b'', b'', result, {})
            self.assertEqual(set(proof['images'].values()), {'verified'})
            allowed = set()
            for rect, _interior, _label, _color in sports_callouts.BUBBLES.values():
                x, y, w, h = rect
                allowed.update(sports_callouts.PAGE + row * 128 + col
                               for row in range(y, y+h) for col in range(x, x+w))
            changed = {i for i, (a, b) in enumerate(zip(original, result.data)) if a != b}
            self.assertTrue(changed)
            self.assertFalse(changed - allowed)
            self.assertEqual(len(result.data), len(original))
            self.assertEqual(result.data[:0x200], original[:0x200])
            damaged = bytearray(result.data)
            damaged[0x6000] ^= 1
            with self.assertRaises(ArtBuildError):
                verify_sprites(sports_callouts.PATH, original, bytes(damaged), b'', b'', result, {})


if __name__ == '__main__':
    unittest.main()
