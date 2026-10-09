"""Assemble one splat asm file into an object without gas's 16-byte section padding.

usage: asm.py IN.s OUT.o

gas rounds the size of the standard .text section up to 16 bytes, which shifts everything after an
SDK object whose size is not a multiple of 16 (T-0010 object-level splits). The text is assembled
into the custom section .text.sdk (alignment 1, no padding) and renamed to .text afterwards;
SUBALIGN(2) in the linker script keeps the placement exact. Files without a .text section
(data, rodata, bss, header) are assembled unchanged.

Shift-JIS strings (T-0012): splat with `string_encoding: SHIFT-JIS` writes rodata and data strings
as readable UTF-8 (`.asciz "..."`), and gas would emit the UTF-8 bytes (+0xC70 bytes in the main
exe). Before assembling, every non-ASCII character inside a .asciz/.ascii/.string literal is
re-encoded to Shift-JIS and written as octal escapes, which restores the original bytes.
"""
import re
import subprocess
import sys

AS = ["mips-linux-gnu-as", "-EL", "-march=r3000", "-mabi=32", "-G0", "-Iinclude", "-I."]
TEXT = '.section .text, "ax"'
STRING_LINE = re.compile(r'^(\s*(?:/\*[^*]*\*/\s*)?\.(?:asciz|ascii|string)\s+)"((?:[^"\\]|\\.)*)"', re.M)


def sjis_escape(match):
    """Replace non-ASCII characters of one string literal by Shift-JIS octal escapes."""
    body = "".join(c if ord(c) < 0x80 else "".join("\\%03o" % b for b in c.encode("shift_jis"))
                   for c in match.group(2))
    return '%s"%s"' % (match.group(1), body)


def main():
    src, out = sys.argv[1], sys.argv[2]
    text = open(src, encoding="utf-8").read()
    text = STRING_LINE.sub(sjis_escape, text)
    custom = TEXT in text
    if not custom and "/data/" not in src and not src.endswith("header.s"):
        sys.exit("asm.py: %s has code but no `%s` line (splat format changed?)" % (src, TEXT))
    if custom:
        text = text.replace(TEXT, '.section .text.sdk, "ax"')
    subprocess.run(AS + ["-o", out, "-"], input=text.encode("utf-8"), check=True)
    if custom:
        subprocess.run(["mips-linux-gnu-objcopy", "--rename-section", ".text.sdk=.text", out], check=True)


if __name__ == "__main__":
    main()
