#!/usr/bin/env python3
"""Overlay symbols for the English island (T-39506, wiki/english-text-runtime.md).

Island code (src/english/*.c) that replaces overlay functions (config/english_hooks.txt) calls that
overlay's functions and reads its data. Those symbols are not defined in the main link. This step
writes a linker script that defines each symbol the island objects leave undefined and that an
overlay defines, at its address in the default build of that overlay. The English overlays keep every
address of their default build (tools/english_link.py checks it), so the addresses hold while the
overlay is in the slot; island code may use them only from a hook of that overlay.

Usage (in Docker, run by ninja):
  english_imports.py OUT --overlays ELF ... --island OBJ ...
Exits non-zero when a symbol is defined at two different addresses by two overlays.
"""
import subprocess
import sys

NM = "mips-linux-gnu-nm"
OVERLAY_SLOT = (0x80132000, 0x80162000)


def nm(path, *flags):
    return subprocess.run([NM, *flags, path], stdout=subprocess.PIPE, check=True).stdout.decode().splitlines()


def main(argv):
    if len(argv) < 4 or argv[2] != "--overlays" or "--island" not in argv:
        sys.exit(__doc__)
    out = argv[1]
    split = argv.index("--island")
    overlays, island = argv[3:split], argv[split + 1:]
    wanted = set()
    for obj in island:
        for line in nm(obj, "-u"):
            wanted.add(line.split()[-1])
    found = {}
    for elf in overlays:
        for line in nm(elf):
            parts = line.split()
            if len(parts) != 3 or parts[2] not in wanted:
                continue
            addr = int(parts[0], 16) & 0xFFFFFFFF
            if not OVERLAY_SLOT[0] <= addr < OVERLAY_SLOT[1]:
                continue
            if parts[2] in found and found[parts[2]][0] != addr:
                sys.exit("english_imports.py: %s is 0x%08X in %s and 0x%08X in %s"
                         % (parts[2], found[parts[2]][0], found[parts[2]][1], addr, elf))
            found[parts[2]] = (addr, elf)
    with open(out, "w") as f:
        f.write("/* Overlay symbols used by the English island (tools/english_imports.py) */\n")
        for name, (addr, elf) in sorted(found.items()):
            f.write("%s = 0x%08X; /* %s */\n" % (name, addr, elf))
    print("english imports: %s, %d symbols" % (out, len(found)))


if __name__ == "__main__":
    main(sys.argv)
