"""T-39002: protect existing builds and verify downloaded dependency identity."""
import hashlib
import io
from pathlib import Path
import tarfile
import tempfile
import unittest
from unittest.mock import patch

from tools.port.native_audio import build


class NativeBuildTests(unittest.TestCase):
    def test_output_cannot_overwrite_or_escape_cache(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / '.asset-cache/old').mkdir(parents=True)
            (root / '.asset-cache/link').symlink_to(root, target_is_directory=True)
            with patch.object(build, 'ROOT', root):
                self.assertEqual(build.output_path('.asset-cache/new'), root / '.asset-cache/new')
                for path in ('.asset-cache/old', '.asset-cache/../outside', 'outside', '.asset-cache/link/new'):
                    with self.subTest(path=path), self.assertRaises(ValueError):
                        build.output_path(path)

    def test_download_requires_size_and_hash_before_writing(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / 'download'
            spec = dict(url='https://example.invalid/pinned', bytes=3,
                        sha256=hashlib.sha256(b'abc').hexdigest())
            for content in (b'ab', b'abd'):
                with patch.object(build.urllib.request, 'urlopen', return_value=io.BytesIO(content)):
                    with self.assertRaises(ValueError):
                        build.download(spec, path)
                    self.assertFalse(path.exists())
            with patch.object(build.urllib.request, 'urlopen', return_value=io.BytesIO(b'abc')):
                build.download(spec, path)
            self.assertEqual(path.read_bytes(), b'abc')

    def test_tar_cannot_escape_after_prefix_removal(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            archive = root / 'source.tar'
            with tarfile.open(archive, 'w') as stream:
                member = tarfile.TarInfo('package/../../escaped')
                member.size = 1
                stream.addfile(member, io.BytesIO(b'x'))
            with self.assertRaises(tarfile.FilterError):
                build.extract(archive, root / 'target', 'tar')
            self.assertFalse((root / 'escaped').exists())


if __name__ == '__main__':
    unittest.main()
