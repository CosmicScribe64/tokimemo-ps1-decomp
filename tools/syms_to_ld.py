"""Turn splat symbol_addrs files into a linker script of `name = addr;` lines.

Overlays call into the main exe by name. splat only applies a symbol_addrs
file to its own segment, so names that live in the main exe (O.BIN and SDK
names) are handed to each overlay link through this script instead.

Usage: python3 tools/syms_to_ld.py OUT.ld SYMBOL_ADDRS.txt...
"""
import re
import sys

LINE = re.compile(r'^\s*(\w+)\s*=\s*(0x[0-9A-Fa-f]+)\s*;')


def main(argv):
    if len(argv) < 3:
        print(__doc__.strip().splitlines()[-1], file=sys.stderr)
        return 2
    seen = {}
    for path in argv[2:]:
        with open(path) as f:
            for line in f:
                m = LINE.match(line)
                if not m:
                    continue
                name, addr = m.groups()
                if seen.get(name, addr) != addr:
                    print("syms_to_ld: %s has two addresses (%s, %s)" % (name, seen[name], addr), file=sys.stderr)
                    return 1
                seen[name] = addr
    if not seen:
        print("syms_to_ld: no symbols found in %s" % ", ".join(argv[2:]), file=sys.stderr)
        return 1
    with open(argv[1], "w") as f:
        for name, addr in sorted(seen.items(), key=lambda kv: int(kv[1], 16)):
            f.write("%s = %s;\n" % (name, addr))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
