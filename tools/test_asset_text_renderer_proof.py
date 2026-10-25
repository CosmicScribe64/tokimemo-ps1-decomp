"""Tests for bounded source and byte pin verification of renderer roles."""
import hashlib
import tempfile
import unittest
from pathlib import Path

from tools.assets.text_renderer_proof import _check_proofs


class RendererProofTests(unittest.TestCase):
    def test_source_assembly_binary_and_window_pins_are_all_required(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            source_root = root / "source"
            original_root = root / "original"
            source_root.mkdir()
            original_root.mkdir()
            source = source_root / "src/proof.c"
            assembly = source_root / "asm/proof.s"
            binary = original_root / "disc.bin"
            source.parent.mkdir()
            assembly.parent.mkdir()
            source.write_bytes(b"source metadata fixture\n")
            assembly.write_bytes(b"assembly metadata fixture\n")
            binary.write_bytes(b"0123456789abcdef")
            digest = lambda path: hashlib.sha256(path.read_bytes()).hexdigest()
            proofs = {"fixture": {
                "source": "src/proof.c", "source_sha256": digest(source),
                "assembly": "asm/proof.s", "assembly_sha256": digest(assembly),
                "binary": "disc.bin", "binary_sha256": digest(binary),
                "window_offset": 4, "window_size": 4,
                "window_sha256": hashlib.sha256(b"4567").hexdigest(),
                "read_arguments": [{"index": 0, "role": "display", "renderer": "fixture"}],
                "evidence": "synthetic source fact",
            }}
            report = _check_proofs(source_root, original_root, proofs)
            self.assertTrue(report["verified"])
            self.assertTrue(report["proofs"]["fixture"]["original_code_window"]["ok"])
            source.write_bytes(b"edited source fixture\n")
            changed = _check_proofs(source_root, original_root, proofs)
            self.assertFalse(changed["verified"])
            self.assertFalse(changed["proofs"]["fixture"]["checks"][0]["ok"])


if __name__ == "__main__":
    unittest.main()
