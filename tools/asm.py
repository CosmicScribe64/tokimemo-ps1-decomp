"""Assemble one splat asm file into an object without gas's 16-byte section padding.

usage: asm.py IN.s OUT.o

gas rounds the size of the standard .text section up to 16 bytes, which shifts everything after an
SDK object whose size is not a multiple of 16 (T-0010 object-level splits). The text is assembled
into the custom section .text.sdk (alignment 1, no padding) and renamed to .text afterwards;
SUBALIGN(2) in the linker script keeps the placement exact. Files without a .text section
(data, rodata, bss, header) are assembled unchanged.
"""
import subprocess
import sys

AS = ["mips-linux-gnu-as", "-EL", "-march=r3000", "-mabi=32", "-G0", "-Iinclude", "-I."]
TEXT = '.section .text, "ax"'


def main():
    src, out = sys.argv[1], sys.argv[2]
    text = open(src, encoding="utf-8").read()
    custom = TEXT in text
    if custom:
        text = text.replace(TEXT, '.section .text.sdk, "ax"')
    subprocess.run(AS + ["-o", out, "-"], input=text.encode("utf-8"), check=True)
    if custom:
        subprocess.run(["mips-linux-gnu-objcopy", "--rename-section", ".text.sdk=.text", out], check=True)


if __name__ == "__main__":
    main()
