"""Unit tests for tools/cc.py pad_text (T-0012), on synthetic objects (no game data).

Run from tools/: python3 test_cc.py   (inside Docker: tools/docker.sh sh -c 'cd tools && python3 test_cc.py')
"""
import os
import subprocess
import tempfile
import unittest

import cc


def build(words, tmp):
    """Assemble `words` nops plus a jal to an external symbol; return (object path)."""
    src = os.path.join(tmp, "t.s")
    obj = os.path.join(tmp, "t.o")
    with open(src, "w") as f:
        f.write('.section .text, "ax"\nglabel_start:\njal ext\nnop\n' + "nop\n" * (words - 2))
    subprocess.run(["mips-linux-gnu-as", "-EL", "-march=r3000", "-o", obj, src], check=True)
    return obj


def text_size(obj):
    out = subprocess.run(["mips-linux-gnu-readelf", "-S", "-W", obj], check=True,
                         stdout=subprocess.PIPE, text=True).stdout
    for line in out.splitlines():
        if " .text " in line:
            return int(line.split(".text")[1].split()[3], 16)
    raise AssertionError("no .text")


class PadText(unittest.TestCase):
    def test_pads_to_multiple_of_16_with_zeros(self):
        with tempfile.TemporaryDirectory() as tmp:
            obj = build(5, tmp)                       # 20 bytes
            cc.pad_text(obj)
            self.assertEqual(text_size(obj), 32)
            raw = os.path.join(tmp, "raw")
            subprocess.run(["mips-linux-gnu-objcopy", "--dump-section", ".text=" + raw, obj, os.devnull], check=True)
            self.assertEqual(open(raw, "rb").read()[20:], b"\0" * 12)

    def test_aligned_object_unchanged(self):
        with tempfile.TemporaryDirectory() as tmp:
            obj = build(8, tmp)                       # 32 bytes
            before = open(obj, "rb").read()
            cc.pad_text(obj)
            self.assertEqual(open(obj, "rb").read(), before)

    def test_relocation_survives(self):
        with tempfile.TemporaryDirectory() as tmp:
            obj = build(3, tmp)
            cc.pad_text(obj)
            out = subprocess.run(["mips-linux-gnu-objdump", "-r", obj], check=True,
                                 stdout=subprocess.PIPE, text=True).stdout
            self.assertIn("ext", out)


if __name__ == "__main__":
    unittest.main()
