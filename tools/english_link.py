#!/usr/bin/env python3
"""Link steps of the English build variant (T-39502, wiki/english-text-runtime.md).

The English build keeps every original address: overlays call the main executable at absolute
addresses, and data in both hold raw code addresses, so nothing may move. Objects whose C source uses
TOKIMEMO_ENGLISH are rebuilt with the flag into build/en/ and linked in the original object's place,
pinned to its original address (a smaller object is padded, a larger one fails the link). New main-exe
code and data go to the island after the main bss (src/english/boot.s copies it there). Entry points
of original functions are redirected with jumps listed in config/english_hooks.txt.

Usage (in Docker, run by ninja):
  english_link.py ld UNIT DEFAULT_LD DEFAULT_MAP OUT_LD [--redirect OBJ ...] [--island OBJ ...]
                  [--boot OBJ]
      English linker script from the default one: redirected objects (default build path) are read
      from build/en/ instead, every input section is pinned to its default address, and for `main`
      the boot stub and island sections are added after the data.
  english_link.py syms ELF OUT_LD
      Linker script with the island symbols of the English main executable, for the overlay links.
  english_link.py exe ELF RAW DEFAULT_ELF HOOKS OUT
      Main executable: apply hooks, point the header entry at english_boot, set t_size, pad to
      2048 bytes; check that no original symbol moved, the island ends below the overlay slot and
      the file fits the disc slot.
  english_link.py ovl NAME ELF RAW DEFAULT_ELF HOOKS OUT
      Overlay: check the size limit, pad to 0x30000, apply hooks, check that no function moved and
      write the file checksum (last 4 bytes: sum16 of all other bytes, big-endian, then 0x12 0x10).
Exits non-zero on any failed check; prints one summary line on success.
"""
import re
import struct
import subprocess
import sys

NM = "mips-linux-gnu-nm"
EXE_HEADER = 0x800
EXE_VRAM = 0x80041000
# Sectors 24-399 of the disc hold SLPM_86.053; KONAMIC3.STR starts at LBA 400, so a larger file
# needs the disc rebuild to move it (T-39501).
EXE_SLOT_BYTES = (400 - 24) * 2048
OVERLAY_SLOT = 0x80132000
OVERLAY_BYTES = 0x30000
ISLAND_LIMIT = OVERLAY_SLOT
TRAILER = b"\x12\x10"

INPUT_LINE = re.compile(r"^(\s+)(build/\S+\.o)\((\.\w+|COMMON)\);$")
MAP_OUTPUT = re.compile(r"^(\.\w+)\s+0x([0-9a-f]+)\s+0x[0-9a-f]+")
MAP_STATEMENT = re.compile(r"^ (build/\S+\.o)\((\.\w+)\)$")
MAP_INPUT = re.compile(r"^ (\.\w+)?\s+0x([0-9a-f]+)\s+0x([0-9a-f]+) (build/\S+\.o)$")


def die(msg):
    sys.exit("english_link.py: " + msg)


def read_map(path):
    """{(object, section): (address, output section start)} of the input sections with contents."""
    out = {}
    current_out = None
    pending = None
    for line in open(path):
        line = line.rstrip("\n")
        m = MAP_OUTPUT.match(line)
        if m:
            current_out = int(m.group(2), 16)
            pending = None
            continue
        m = MAP_STATEMENT.match(line)
        if m:
            pending = (m.group(1), m.group(2))
            continue
        m = MAP_INPUT.match(line)
        if m and pending and m.group(4) == pending[0] and current_out is not None:
            if pending not in out and int(m.group(3), 16) > 0:
                out[pending] = (int(m.group(2), 16), current_out)
            continue
    return out


def english_path(obj):
    return "build/en/" + obj[len("build/"):]


def cmd_ld(argv):
    unit, default_ld, default_map, out_ld = argv[:4]
    opts = {"--redirect": [], "--island": [], "--boot": []}
    key = None
    for a in argv[4:]:
        if a in opts:
            key = a
        elif key is None:
            die("ld: unexpected argument %r" % a)
        else:
            opts[key].append(a)
    redirect = set(opts["--redirect"])
    placed = read_map(default_map)
    lines = open(default_ld).read().split("\n")
    out = []
    seen = set()
    for line in lines:
        m = INPUT_LINE.match(line)
        if m:
            indent, obj, sec = m.groups()
            where = placed.get((obj, sec))
            if where is not None:
                addr, start = where
                out.append("%s. = 0x%X; /* pin %s(%s) at 0x%08X */" % (indent, addr - start, obj, sec, addr))
            if obj in redirect:
                seen.add(obj)
                line = "%s%s(%s);" % (indent, english_path(obj), sec)
        if line.strip() == "/DISCARD/ :" and unit == "main":
            out += island_sections(opts["--boot"], opts["--island"])
        out.append(line)
    missing = redirect - seen
    if missing:
        die("ld: %s not in %s" % (", ".join(sorted(missing)), default_ld))
    with open(out_ld, "w") as f:
        f.write("/* English build (T-39502): tools/english_link.py ld from %s */\n" % default_ld)
        f.write("\n".join(out))


def island_sections(boot, island):
    """Boot stub after the data (loaded with the file), island after the main bss (copied by it)."""
    if len(boot) != 1 or not island:
        die("ld main: needs one --boot object and at least one --island object")
    s = ["    .english_boot main_DATA_END : AT(main_ROM_END)",
         "    {",
         "        %s(.text);" % boot[0],
         "        . = ALIGN(., 16);",
         "    }",
         "    english_island_source = main_DATA_END + SIZEOF(.english_boot);",
         "    .english main_BSS_END : AT(main_ROM_END + SIZEOF(.english_boot))",
         "    {",
         "        english_island_start = .;"]
    for sec in (".text", ".rodata", ".data"):
        s += ["        %s(%s);" % (o, sec) for o in island]
    s += ["        . = ALIGN(., 16);",
          "        english_island_end = .;",
          "    }",
          "    .english_bss (NOLOAD) :",
          "    {",
          "        english_bss_start = .;"]
    for sec in (".bss", "COMMON"):
        s += ["        %s(%s);" % (o, sec) for o in island]
    s += ["        . = ALIGN(., 16);",
          "        english_bss_end = .;",
          "    }",
          '    english_limit_ok = ASSERT(english_bss_end <= 0x%08X, "English island runs into the overlay slot");' % ISLAND_LIMIT,
          ""]
    return s


def symbols(elf):
    """{name: (address, type)} of the defined symbols of an ELF."""
    text = subprocess.run([NM, elf], stdout=subprocess.PIPE, check=True).stdout.decode()
    out = {}
    for line in text.splitlines():
        parts = line.split()
        if len(parts) == 3 and parts[1] not in "Uw":
            out[parts[2]] = (int(parts[0], 16) & 0xFFFFFFFF, parts[1])
    return out


def cmd_syms(argv):
    elf, out_ld = argv
    syms = symbols(elf)
    start = syms["english_island_start"][0]
    with open(out_ld, "w") as f:
        f.write("/* English island symbols for the overlay links (T-39502) */\n")
        for name, (addr, _t) in sorted(syms.items()):
            if start <= addr < ISLAND_LIMIT and name.startswith("english_"):
                f.write("%s = 0x%08X;\n" % (name, addr))


def read_hooks(path, unit):
    hooks = []
    for n, line in enumerate(open(path), 1):
        words = line.split("#", 1)[0].split()
        if not words:
            continue
        if len(words) != 3:
            die("%s:%d: expected `<unit> <original> <replacement>`" % (path, n))
        if words[0] == unit:
            hooks.append((words[1], words[2]))
    return hooks


def apply_hooks(data, base, file_offset, syms, hooks, unit):
    for original, replacement in hooks:
        if original not in syms or replacement not in syms:
            die("%s hook %s -> %s: symbol not found" % (unit, original, replacement))
        src, dst = syms[original][0], syms[replacement][0]
        if (src ^ dst) & 0xF0000000:
            die("%s hook %s -> %s: out of j range" % (unit, original, replacement))
        at = src - base + file_offset
        if not 0 <= at <= len(data) - 8:
            die("%s hook %s at 0x%08X is outside the binary" % (unit, original, src))
        data[at:at + 8] = struct.pack("<II", 0x08000000 | ((dst >> 2) & 0x03FFFFFF), 0)


def moved(default_syms, new_syms, types=None):
    out = []
    for name, (addr, t) in default_syms.items():
        if types and t not in types:
            continue
        if name in new_syms and new_syms[name][0] != addr:
            out.append("%s 0x%08X -> 0x%08X" % (name, addr, new_syms[name][0]))
    return out


def cmd_exe(argv):
    elf, raw, default_elf, hooks_path, out = argv
    data = bytearray(open(raw, "rb").read())
    syms = symbols(elf)
    bad = moved(symbols(default_elf), syms)
    if bad:
        die("exe: %d original symbols moved, first: %s" % (len(bad), "; ".join(bad[:5])))
    bss_end = syms["english_bss_end"][0]
    if bss_end > ISLAND_LIMIT:
        die("exe: island ends at 0x%08X, past 0x%08X" % (bss_end, ISLAND_LIMIT))
    apply_hooks(data, EXE_VRAM, EXE_HEADER, syms, read_hooks(hooks_path, "main"), "main")
    data += bytes(-len(data) % 2048)
    if len(data) > EXE_SLOT_BYTES:
        die("exe: %d bytes, the disc slot holds %d (T-39501 must move files)" % (len(data), EXE_SLOT_BYTES))
    struct.pack_into("<I", data, 0x10, syms["english_boot"][0])
    struct.pack_into("<I", data, 0x1C, len(data) - EXE_HEADER)
    with open(out, "wb") as f:
        f.write(data)
    island = syms["english_island_end"][0] - syms["english_island_start"][0]
    print("english exe: %s %d bytes (0x%X), island %d bytes + bss to 0x%08X (limit 0x%08X)"
          % (out, len(data), len(data), island, bss_end, ISLAND_LIMIT))


def overlay_base(name):
    """Load address of an overlay from config/overlays.txt."""
    for line in open("config/overlays.txt"):
        words = line.split()
        if words and words[0] == name:
            return int(words[1], 16)
    die("ovl %s: not in config/overlays.txt" % name)


def cmd_ovl(argv):
    name, elf, raw, default_elf, hooks_path, out = argv
    data = bytearray(open(raw, "rb").read())
    if len(data) > OVERLAY_BYTES:
        die("ovl %s: %d bytes, the overlay slot holds %d" % (name, len(data), OVERLAY_BYTES))
    data += bytes(OVERLAY_BYTES - len(data))
    syms = symbols(elf)
    bad = moved(symbols(default_elf), syms, types="Tt")
    if bad:
        die("ovl %s: %d functions moved, first: %s" % (name, len(bad), "; ".join(bad[:5])))
    hooks = read_hooks(hooks_path, name)
    if hooks:
        apply_hooks(data, overlay_base(name), 0, syms, hooks, name)
    total = sum(data[:-4]) & 0xFFFF
    data[-4:] = bytes([total >> 8, total & 0xFF]) + TRAILER
    with open(out, "wb") as f:
        f.write(data)
    print("english ovl: %s checksum %04X" % (out, total))


def main(argv):
    cmds = {"ld": cmd_ld, "syms": cmd_syms, "exe": cmd_exe, "ovl": cmd_ovl}
    if len(argv) < 2 or argv[1] not in cmds:
        sys.exit(__doc__)
    cmds[argv[1]](argv[2:])


if __name__ == "__main__":
    main(sys.argv)
