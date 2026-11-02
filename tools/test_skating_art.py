"""T-39513: generated billboard bounds and source identity, no game data."""
from pathlib import Path
import tempfile
import unittest

import numpy as np

from tools.patch.art import ArtBuildError, skating


class SkatingArtTests(unittest.TestCase):
    def test_unrelated_pixels_are_preserved(self):
        source = np.full((160, 320, 3), 28, dtype=int)
        result = skating.render(source)
        allowed = np.zeros((160, 320), bool)
        for (x0, y0, x1, y1), _, _ in skating.BANDS:
            allowed[y0:y1, x0:x1] = True
        self.assertTrue((source[~allowed] == result[~allowed]).all())
        self.assertTrue((source[allowed] != result[allowed]).any())

    def test_wrong_original_is_rejected(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            source = root / skating.PATH
            source.parent.mkdir(parents=True)
            source.write_bytes(b'wrong original')
            with self.assertRaisesRegex(ArtBuildError, 'source differs'):
                skating.item(root, root)


if __name__ == '__main__':
    unittest.main()
