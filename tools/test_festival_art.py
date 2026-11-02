"""T-39516: plate geometry, erased glyphs, and source identity."""
from pathlib import Path
import tempfile
import unittest

import numpy as np

from tools.patch.art import ArtBuildError, festival


class FestivalArtTests(unittest.TestCase):
    def test_old_ink_is_removed_without_touching_the_border(self):
        source = np.full((160, 320, 3), 30, dtype=int)
        marked = source.copy()
        x, y, w, h = festival.RECT
        marked[y + 3:y + 15, x + 4:x + 40] = 2
        clean = festival.render(source)
        result = festival.render(marked)
        self.assertTrue(np.array_equal(clean, result))
        outside = np.ones((160, 320), dtype=bool)
        outside[y:y + h, x:x + w] = False
        self.assertTrue(np.array_equal(result[outside], marked[outside]))
        for top in (y + 1, y + 7, y + 13):
            self.assertTrue((result[top:top + 5, x:x + w] == 1).any())

    def test_wrong_source_is_rejected(self):
        with tempfile.TemporaryDirectory() as folder:
            root = Path(folder)
            path = root / 'CDROM/BS/D_01_47.BS'
            path.parent.mkdir(parents=True)
            path.write_bytes(b'wrong original')
            with self.assertRaisesRegex(ArtBuildError, 'source differs'):
                festival.items(root, root)

    def test_wrong_geometry_is_rejected(self):
        with self.assertRaisesRegex(ArtBuildError, 'geometry'):
            festival.render(np.zeros((240, 320, 3), dtype=int))
