#!/usr/bin/env python3
"""Headless PS1 runtime probe for naming work (T-20500), run inside the tools/emu image.

Boots the user's disc in PCSX-Redux (no UI, OpenBIOS), runs a job written as Lua
(tools/emu/emu.lua), and prints a short report: breakpoint hits with registers, arguments,
the pointed-to memory and a call stack, memory-watch hits, which overlay was resident,
screenshots (PNG) and save states. Outputs go under build/emu/ (gitignored). Use
tools/emu/run.sh from the host; see wiki/emulator.md.

Subcommands:
  boot                   run --frames frames (or --until-overlay), screenshot at the end
  break TARGET...        stop at a code address (TARGET: 0xADDR, func_XXXXXXXX or a symbol name;
                         append @OVERLAY for overlay code, e.g. 0x80143580@NAME_ENT;
                         --width N covers a range, e.g. any code run in the overlay slot)
  watch ADDR[:WIDTH]...  record writes (or --read reads) to memory
  trace TARGET...        count hits of many functions over a run (--all-named: every known
                         function of the main exe, or of --overlay NAME)
  report RESULT.json     print the report of an earlier run again
  gdb                    run with the GDB server on --port (attach gdb-multiarch, in the image)

Common options: --frames, --state (load a save state first), --save-state, --input, --mash,
--shot, --dump, --out, --timeout. Frames are vsyncs (60 per second, NTSC), counted from the
start of the run (frame 1 loads --state).
"""

import argparse
import glob
import hashlib
import json
import os
import re
import struct
import subprocess
import sys
import time
import zlib

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
REDUX = os.environ.get("PCSX_REDUX", "/opt/redux/pcsx-redux")
BIOS = os.path.join(os.path.dirname(REDUX), "openbios.bin")
EXE = os.path.join(ROOT, "disc/files/SLPM_86.053")
EXN_DIR = os.path.join(ROOT, "disc/files/CDROM/EXEDIR")
MAIN_YAML = os.path.join(ROOT, "config/SLPM_86.053.yaml")
MODE_ADDR = 0x800E738E  # overlay mode byte read by the loader (wiki/overlays.md)
SLOT_PROBES = [0x80132000, 0x80134000, 0x800F6000]  # overlay load addresses in use
BUTTONS = ["SELECT", "START", "UP", "RIGHT", "DOWN", "LEFT", "L2", "R2", "L1", "R1",
           "TRIANGLE", "CIRCLE", "CROSS", "SQUARE"]
GPR = ["r0", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
       "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"]
REG_ORDER = ["a0", "a1", "a2", "a3", "v0", "v1", "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
             "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7", "t8", "t9", "at", "gp", "sp", "fp",
             "ra", "lo", "hi"]
DATA_REACH = 0x2000  # name a data address as symbol+offset up to this far past a known symbol
# structs whose fields are the naming targets: (base, size, type); GameState: wiki/game-state.md
STRUCTS = [(0x800E6280, 0x1A90, "GameState")]


def die(msg):
    print("emu: " + msg, file=sys.stderr)
    sys.exit(2)


# ---------------------------------------------------------------- static code and symbols

def yaml_symbol_files(path):
    """The symbol_addrs_path list of a splat config."""
    files, inside = [], False
    for line in open(path):
        if re.match(r"\s*symbol_addrs_path:", line):
            inside = True
            continue
        if inside:
            m = re.match(r"\s*-\s*(\S+\.txt)", line)
            if m:
                files.append(os.path.join(ROOT, m.group(1)))
            elif not line.strip().startswith("#"):
                break
    return files


class Image:
    """Bytes of the main exe and the overlays at their load addresses (code is static)."""

    def __init__(self):
        if not os.path.exists(EXE):
            die("missing %s (extract the disc first: wiki/disc-layout.md)" % EXE)
        data = open(EXE, "rb").read()
        self.main_base = struct.unpack_from("<I", data, 0x18)[0]
        self.main = data[0x800:]
        # main code ends where the first rodata/data subsegment of the splat config starts
        offs = [int(m.group(1), 16) for m in
                re.finditer(r"\[(0x[0-9A-Fa-f]+), \.?(?:rodata|data|bss)", open(MAIN_YAML).read())]
        self.main_text_end = self.main_base + min(offs) - 0x800 if offs else self.main_base + len(self.main)
        self.overlays = {}  # name -> (base, bytes, text_end)
        for line in open(os.path.join(ROOT, "config/overlays.txt")):
            f = line.split()
            if len(f) < 3 or line.startswith("#"):
                continue
            path = os.path.join(EXN_DIR, f[0] + ".EXN")
            if os.path.exists(path):
                base = int(f[1], 16)
                self.overlays[f[0]] = (base, open(path, "rb").read(), base + int(f[2], 16))

    def in_main(self, addr):
        return self.main_base <= addr < self.main_base + len(self.main)

    def in_overlay(self, addr, overlay):
        if overlay not in self.overlays:
            return False
        base, data, _ = self.overlays[overlay]
        return base <= addr < base + len(data)

    def is_code(self, addr, overlay=None):
        if self.main_base <= addr < self.main_text_end:
            return True
        if overlay in self.overlays:
            base, _, text_end = self.overlays[overlay]
            return base <= addr < text_end
        return False

    def word(self, addr, overlay=None):
        if self.in_overlay(addr, overlay):
            base, data, _ = self.overlays[overlay]
            return struct.unpack_from("<I", data, addr - base)[0]
        if self.in_main(addr):
            return struct.unpack_from("<I", self.main, addr - self.main_base)[0]
        return None

    def overlay_bytes(self, name, addr, n):
        base, data, _ = self.overlays[name]
        if not base <= addr <= base + len(data) - n:
            die("0x%08X is outside %s (0x%08X-0x%08X)" % (addr, name, base, base + len(data)))
        return data[addr - base:addr - base + n]

    def resident(self, slots):
        """Overlays whose first 32 bytes are at their load address in the slot probes."""
        found = []
        for name, (base, data, _) in sorted(self.overlays.items()):
            probe = (slots or {}).get("%08X" % base)
            if probe and bytes.fromhex(probe) == data[:32]:
                found.append(name)
        return found


class Symbols:
    """Names from the splat symbol files of each unit and, once built, its linker map."""

    SYM_RE = re.compile(r"^\s*([A-Za-z_]\w*)\s*=\s*(0x[0-9A-Fa-f]+)\s*;(.*)")
    MAP_SYM_RE = re.compile(r"^\s+0x([0-9a-f]{16})\s+([A-Za-z_]\w*)\s*$")
    MAP_TEXT_RE = re.compile(r"^\s*\.text\s+0x([0-9a-f]{16})\s+0x([0-9a-f]+)")

    def __init__(self, image):
        self.image = image
        self.names = {None: {}}  # unit (None = main exe) -> {addr: name}
        self.funcs = {None: set()}
        self.mapped = set()  # units whose linker map was read: their function list is complete
        for f in yaml_symbol_files(MAIN_YAML):
            self._load_symfile(f, None)
        self._load_map(os.path.join(ROOT, "build/SLPM_86.053.map"), None)
        for name in image.overlays:
            self.names[name], self.funcs[name] = {}, set()
            yaml = os.path.join(ROOT, "config/overlays/%s.yaml" % name)
            for f in yaml_symbol_files(yaml) if os.path.exists(yaml) else []:
                self._load_symfile(f, name)
            self._load_map(os.path.join(ROOT, "build/ovl/%s.map" % name), name)

    def _unit_of(self, addr, overlay):
        if overlay and self.image.in_overlay(addr, overlay):
            return overlay
        return None

    def _keep(self, unit, addr):
        """A unit's own addresses only: overlay files also list main-exe names."""
        return unit is None or self.image.in_overlay(addr, unit)

    def _load_symfile(self, path, unit):
        if not os.path.exists(path):
            return
        for line in open(path):
            m = self.SYM_RE.match(line)
            if m:
                addr = int(m.group(2), 16)
                if not self._keep(unit, addr):
                    continue
                self.names[unit][addr] = m.group(1)
                if "type:func" in m.group(3):
                    self.funcs[unit].add(addr)

    def _load_map(self, path, unit):
        if not os.path.exists(path):
            return
        self.mapped.add(unit)
        text = []
        for line in open(path):
            m = self.MAP_TEXT_RE.match(line)
            if m:
                start = int(m.group(1), 16) & 0xFFFFFFFF
                text.append((start, start + int(m.group(2), 16)))
                continue
            m = self.MAP_SYM_RE.match(line)
            if m:
                addr = int(m.group(1), 16) & 0xFFFFFFFF
                if not self._keep(unit, addr):
                    continue
                self.names[unit].setdefault(addr, m.group(2))
                if any(a <= addr < b for a, b in text[-4:]):
                    self.funcs[unit].add(addr)

    def resolve(self, target, overlay=None):
        """'0x8004F268', 'func_8004F268', 'load_string' -> address."""
        if re.fullmatch(r"(0x)?[0-9A-Fa-f]{8}", target):
            return int(target, 16)
        m = re.fullmatch(r"(?:func|D|jtbl)_([0-9A-Fa-f]{8})", target)
        if m:
            return int(m.group(1), 16)
        for unit in ([overlay] if overlay else []) + [None]:
            for a, n in self.names.get(unit, {}).items():
                if n == target:
                    return a
        die("unknown symbol %s" % target)

    def func_start(self, addr, overlay=None):
        """(start, exact) of the function holding addr: the map or symbol files, else the
        instruction after the previous jr $ra plus its delay slot (exact=False)."""
        unit = self._unit_of(addr, overlay)
        known = self.funcs.get(unit, set())
        best = max((f for f in known if f <= addr and addr - f < 0x8000), default=None)
        if unit in self.mapped and best is not None:
            return best, True
        a = addr - 4
        while a > addr - 0x8000:
            w = self.image.word(a, overlay)
            if w is None or not self.image.is_code(a, overlay):
                return best, best is not None
            if w == 0x03E00008:  # jr $ra: the next function starts after the delay slot
                start = a + 8
                while self.image.word(start, overlay) == 0 and start < addr:
                    start += 4
                if best is not None and best >= start:
                    return best, True
                return start, False
            a -= 4
        return best, best is not None

    def describe(self, addr, overlay=None):
        """'load_string+0x1C' for code, 'D_800E6280+0x12' for data near a symbol, else ''.
        Addresses inside a known struct also get its field offset: 'D_800E7388 = GameState+0x1108'."""
        name = self._describe(addr, overlay)
        for base, size, typ in STRUCTS:
            if base <= addr < base + size:
                return "%s = %s+0x%X" % (name, typ, addr - base) if name else "%s+0x%X" % (typ, addr - base)
        return name

    def _describe(self, addr, overlay):
        unit = self._unit_of(addr, overlay)
        names = self.names.get(unit, {})
        if addr in names:
            return names[addr]
        if self.image.is_code(addr, overlay):
            start, exact = self.func_start(addr, overlay)
            if start is None:
                return ""
            base = names.get(start) or ("func_%08X" % start)
            if not exact:
                base += "?"
            return base if start == addr else "%s+0x%X" % (base, addr - start)
        near = max((a for a in names if a <= addr < a + DATA_REACH and not self.image.is_code(a, overlay)),
                   default=None)
        if near is not None:
            return "%s+0x%X" % (names[near], addr - near)
        if unit:
            base = self.image.overlays[unit][0]
            return "%s+0x%X" % (unit, addr - base)
        return ""


# ---------------------------------------------------------------- call stack

def s16(x):
    return x - 0x10000 if x & 0x8000 else x


def unwind(hit, image, syms, overlay, depth=16):
    """Call stack from the stack dump: walk each function's prologue (addiu sp,-N / sw ra,X(sp))."""
    sp0 = hit["regs"]["sp"]
    stack = bytes.fromhex(hit.get("stack") or "")

    def stack_word(a):
        o = a - sp0
        if 0 <= o <= len(stack) - 4:
            return struct.unpack_from("<I", stack, o)[0]
        return None

    pc, sp, ra = hit["pc"], sp0, hit["regs"]["ra"]
    frames = [pc]
    for level in range(depth):
        if not image.is_code(pc, overlay):
            break
        start, _ = syms.func_start(pc, overlay)
        if start is None:
            break
        size, ra_off = 0, None
        a = start
        while a < pc and a < start + 0x200:
            w = image.word(a, overlay) or 0
            if w >> 16 == 0x27BD and w & 0x8000:  # addiu $sp, $sp, -N
                size = -s16(w & 0xFFFF)
            elif w >> 16 == 0xAFBF:  # sw $ra, X($sp)
                ra_off = s16(w & 0xFFFF)
            elif w == 0x03E00008:
                break
            a += 4
        if ra_off is not None:
            ret = stack_word(sp + ra_off)
        elif level == 0:
            ret = ra
        else:
            break
        if ret is None or not image.is_code(ret, overlay):
            break
        frames.append(ret - 8)  # the jal
        pc, sp = ret, sp + size
    return frames


# ---------------------------------------------------------------- outputs

def png_from_raw(raw_path, width, height, bpp, png_path):
    if any(type(value) is not int for value in (width, height, bpp)) or not (0 < width <= 1024 and 0 < height <= 512) or bpp not in (16, 24):
        raise ValueError("unsupported screenshot dimensions or pixel format")
    with open(raw_path, "rb") as stream:
        raw = stream.read()
    if len(raw) != width * height * (bpp // 8):
        raise ValueError("screenshot payload length differs from dimensions")
    rows = []
    for y in range(height):
        if bpp == 16:
            row = bytearray([0])
            for x in range(width):
                v = raw[(y * width + x) * 2] | raw[(y * width + x) * 2 + 1] << 8
                row += bytes(((v & 31) << 3, (v >> 5 & 31) << 3, (v >> 10 & 31) << 3))
            rows.append(bytes(row))
        else:
            rows.append(b"\0" + raw[y * width * 3:(y + 1) * width * 3])

    def chunk(tag, data):
        c = tag + data
        return struct.pack(">I", len(data)) + c + struct.pack(">I", zlib.crc32(c) & 0xFFFFFFFF)

    with open(png_path, "wb") as f:
        f.write(b"\x89PNG\r\n\x1a\n")
        f.write(chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 8, 2, 0, 0, 0)))
        f.write(chunk(b"IDAT", zlib.compress(b"".join(rows), 6)))
        f.write(chunk(b"IEND", b""))
    os.remove(raw_path)


def prepare_shot(shot, out):
    """Keep state observations even when no usable GPU image was captured."""
    raw = os.path.join(out, os.path.basename(shot["path"]))
    png = os.path.splitext(raw)[0] + ".png"
    try:
        png_from_raw(raw, shot["width"], shot["height"], shot["bpp"], png)
    except (OSError, ValueError) as exc:
        shot["image_status"] = "unavailable"
        shot["image_error"] = str(exc)
        shot.pop("png", None)
        shot.pop("png_sha256", None)
    else:
        shot["image_status"] = "ok"
        shot["png"] = os.path.basename(png)
        with open(png, "rb") as stream:
            shot["png_sha256"] = hashlib.file_digest(stream, "sha256").hexdigest()


def as_list(x):
    return x if isinstance(x, list) else []


def hexdump(addr, data_hex, limit):
    data = bytes.fromhex(data_hex)[:limit]
    return ["    %08X: %s" % (addr + i, data[i:i + 16].hex(" ")) for i in range(0, len(data), 16)]


def print_hit(i, h, image, syms, verbose):
    ovl = image.resident(h.get("slots"))
    overlay = h.get("overlay") or (ovl[0] if ovl else None)
    r = h["regs"]
    print("\n#%d %s  pc=0x%08X %s  frame %d  overlay %s" % (
        i + 1, h.get("label"), h["pc"], syms.describe(h["pc"], overlay), h["frame"], ",".join(ovl) or "none"))
    if h.get("kind") in ("Read", "Write"):
        insn = struct.unpack("<I", bytes.fromhex(h["insn"]))[0] if h.get("insn") else 0
        rt = GPR[insn >> 16 & 31]
        dumps = as_list(h.get("dumps"))
        old = dumps[0].get("data") if dumps else None
        what = "stores %s=0x%08X" % (rt, r[rt]) if h["kind"] == "Write" else "loads into %s" % rt
        print("  %s of 0x%08X (%s) width %d: insn %08X %s; memory before: %s" % (
            h["kind"].lower(), h["address"], syms.describe(h["address"], overlay) or "?", h["width"], insn,
            what, old))
    print("  args: " + "  ".join("%s=%08X" % (k, r[k]) for k in REG_ORDER[:4]))
    for k in REG_ORDER[:4]:
        d = syms.describe(r[k], overlay)
        mem = (h.get("args") or {}).get(k)
        if mem:
            print("    %s -> %s *= %s" % (k, d or "?", mem[:64]))
        elif d:
            print("    %s -> %s" % (k, d))
    if verbose:
        print("  regs: " + " ".join("%s=%08X" % (k, r[k]) for k in REG_ORDER[4:]))
    stack = unwind(h, image, syms, overlay)
    line = " <- ".join(syms.describe(a, overlay) or "0x%08X" % a for a in stack)
    if not image.is_code(h["pc"], overlay):  # BIOS or kernel code: its caller is in $ra
        line += " (not game code; ra=0x%08X %s)" % (r["ra"], syms.describe(r["ra"], overlay))
    print("  stack: " + line)
    for d in as_list(h.get("dumps"))[(1 if h.get("kind") in ("Read", "Write") else 0):]:
        if d.get("data"):
            print("  dump %s:" % (syms.describe(d["addr"], overlay) or "0x%08X" % d["addr"]))
            print("\n".join(hexdump(d["addr"], d["data"], 256)))


def report(result_path, verbose=False):
    image = Image()
    syms = Symbols(image)
    res = json.load(open(result_path))
    out = os.path.dirname(os.path.abspath(result_path))
    print("stop: %s after %d frames (%.1f s game time)" % (res.get("reason"), res.get("frames", 0),
                                                           res.get("frames", 0) / 60.0))
    resident = image.resident(res.get("slots"))
    print("resident overlay at end: %s, mode byte 0x%02X" % (",".join(resident) or "none", res.get("mode", 0)))
    seen = []
    for e in as_list(res.get("slot_log")):
        names = image.resident({"%08X" % e["addr"]: e["data"]})
        if names and (not seen or seen[-1][1] != names[0]):
            seen.append((e["frame"], names[0]))
    if seen:
        print("overlay loads (frame the first bytes landed): " + ", ".join("%s@%d" % (n, f) for f, n in seen))
    modes = as_list(res.get("mode_log"))
    if modes:
        print("mode byte changes: " + ", ".join("0x%02X@%d" % (m["mode"], m["frame"]) for m in modes[-12:]))
    counts = {k: v for k, v in (res.get("counts") or {}).items() if isinstance(v, int)}
    if counts:
        print("hit counts: " + ", ".join("%s=%d" % kv for kv in sorted(counts.items(), key=lambda kv: -kv[1])))
    hits = as_list(res.get("hits"))
    limit = len(hits) if verbose else min(len(hits), 12)
    for i, h in enumerate(hits[:limit]):
        print_hit(i, h, image, syms, verbose)
    if limit < len(hits):
        print("\n(%d more hits in result.json; -v prints all)" % (len(hits) - limit))
    for d in as_list(res.get("final_dumps")):
        if d.get("data"):
            print("memory at end, %s:" % (syms.describe(d["addr"]) or "0x%08X" % d["addr"]))
            print("\n".join(hexdump(d["addr"], d["data"], 1 << 20)))
    for s in as_list(res.get("shots")):
        if s.get("image_status") == "ok":
            png = os.path.join(out, s["png"])
            print("screenshot frame %d: %s (%dx%d)" % (s["frame"], os.path.relpath(png, ROOT), s["width"], s["height"]))
        else:
            print("screenshot frame %d: unavailable or unvalidated (%s)" % (s["frame"], s.get("image_error", "legacy capture lacks image validation")))
    if res.get("saved_state"):
        print("saved state: %s" % os.path.relpath(res["saved_state"].replace("/work", ROOT, 1), ROOT))
    for n in as_list(res.get("notes")):
        print("note: " + n)
    if None not in syms.mapped:
        print("note: no build/SLPM_86.053.map, so function names marked ? are guessed from jr $ra (build first)")
    print("full result: %s" % os.path.relpath(os.path.join(out, "result.json"), ROOT))


# ---------------------------------------------------------------- jobs

def lua_value(v):
    if isinstance(v, bool):
        return "true" if v else "false"
    if v is None:
        return "nil"
    if isinstance(v, (int, float)):
        return repr(v)
    if isinstance(v, str):
        return json.dumps(v)
    if isinstance(v, (list, tuple)):
        return "{" + ", ".join(lua_value(x) for x in v) + "}"
    return "{" + ", ".join("[%s] = %s" % (json.dumps(k), lua_value(x)) for k, x in v.items()) + "}"


def parse_inputs(args):
    events = []
    for spec in (args.input or "").split(","):
        spec = spec.strip()
        if not spec:
            continue
        m = re.fullmatch(r"(\d+):([A-Z0-9+]+)(?:/(\d+))?", spec)
        if not m:
            die("bad --input entry %r (want FRAME:BUTTON[+BUTTON][/HOLD])" % spec)
        buttons = m.group(2).split("+")
        for b in buttons:
            if b not in BUTTONS:
                die("unknown button %s (one of %s)" % (b, " ".join(BUTTONS)))
        events.append({"frame": int(m.group(1)), "buttons": buttons, "hold": int(m.group(3) or 6)})
    for spec in args.mash or []:
        m = re.fullmatch(r"([A-Z0-9]+):(\d+):(\d+)(?::(\d+))?", spec)
        if not m or m.group(1) not in BUTTONS:
            die("bad --mash %r (want BUTTON:FIRST:PERIOD[:LAST])" % spec)
        last = int(m.group(4) or args.frames)
        for f in range(int(m.group(2)), last + 1, int(m.group(3))):
            events.append({"frame": f, "buttons": [m.group(1)], "hold": 4})
    return events


def find_iso(args):
    if args.iso:
        return os.path.abspath(args.iso)
    cues = sorted(glob.glob(os.path.join(ROOT, "disc", "*.cue")))
    usable = []
    for cue in cues:
        with open(cue, encoding="utf-8-sig") as source:
            members = re.findall(r'^\s*FILE\s+(?:"([^"]+)"|(\S+))\s+\S+\s*$',
                                 source.read(), re.MULTILINE | re.IGNORECASE)
        if members and all(os.path.isfile(os.path.join(os.path.dirname(cue), quoted or plain))
                           for quoted, plain in members):
            usable.append(cue)
    if len(usable) != 1:
        die("expected one disc/*.cue with existing track files; found %d; pass --iso PATH explicitly" % len(usable))
    return usable[0]


def parse_target(spec, syms, image, default_overlay):
    overlay = default_overlay
    if "@" in spec:
        spec, overlay = spec.split("@", 1)
    if overlay and overlay not in image.overlays:
        die("unknown overlay %s" % overlay)
    return spec, syms.resolve(spec, overlay), overlay


def build_job(args, out, image, syms):
    validate_sampling(args.sample_every, args.frames, args.dump, args.cmd)
    if args.save_state:
        os.makedirs(os.path.dirname(os.path.abspath(args.save_state)), exist_ok=True)
    if args.state and not os.path.exists(args.state):
        die("no save state %s" % args.state)
    breaks = []
    hits = args.hits
    if args.cmd in ("break", "trace"):
        targets = list(args.targets)
        if args.cmd == "trace" and args.all_named:  # every known function of --overlay, else of the main exe
            unit = args.overlay
            targets += ["0x%08X" % a for a in sorted(syms.funcs.get(unit, ())) if image.is_code(a, unit)]
        for spec in targets:
            name, addr, overlay = parse_target(spec, syms, image, args.overlay)
            if image.is_code(addr):  # main-exe code is always resident
                overlay = None
            if not image.is_code(addr) and not overlay:
                die("%s is not main-exe code: name its overlay (%s@NAME or --overlay)" % (spec, spec))
            if overlay and not image.is_code(addr, overlay):
                die("%s is not code in %s" % (spec, overlay))
            b = {"addr": addr, "kind": "Exec", "width": args.width, "overlay": overlay,
                 "label": "%s@%s" % (name, overlay) if overlay else name,
                 "record": 1 if args.cmd == "trace" else hits,
                 "stop_after": 0 if (args.cmd == "trace" or args.no_stop) else hits,
                 "dumps": [[a, n] for a, n in args.dump]}
            if overlay:  # the slot holds other overlays most of the time: check the code bytes
                b["expect"] = image.overlay_bytes(overlay, addr, 16).hex()
            breaks.append(b)
    elif args.cmd == "watch":
        for spec in args.targets:
            name, _, width = spec.partition(":")
            addr = syms.resolve(name)
            width = int(width or "4", 0)
            breaks.append({"addr": addr, "kind": "Read" if args.read else "Write", "width": width,
                           "label": spec, "overlay": args.overlay, "record": hits,
                           "stop_after": 0 if args.no_stop else hits,
                           "dumps": [[addr, width]] + [[a, n] for a, n in args.dump]})
    job = {
        "out": out, "frames": args.frames, "breaks": breaks, "inputs": parse_inputs(args),
        "shots": [int(x) for x in (args.shot or "").split(",") if x],
        "capture_dumps": [[a, n] for a, n in args.dump],
        "sample_every": args.sample_every,
        "final_shot": args.cmd == "boot" or args.final_shot,
        "final_dumps": [[a, n] for a, n in args.dump] if args.cmd == "boot" else [],
        "load_state": os.path.abspath(args.state) if args.state else None,
        "save_state": os.path.abspath(args.save_state) if args.save_state else None,
        "ram_dump": os.path.join(out, "ram.bin") if args.ram_dump else None,
        "stack_bytes": 0x800, "arg_bytes": 32, "mode_addr": MODE_ADDR, "slot_probes": SLOT_PROBES,
    }
    if args.until_overlay:
        if args.until_overlay not in image.overlays:
            die("unknown overlay %s" % args.until_overlay)
        base, data, _ = image.overlays[args.until_overlay]
        if base not in SLOT_PROBES:
            die("no slot probe at 0x%08X" % base)
        job["until_overlay"] = {"name": args.until_overlay, "addr": base, "head": data[:32].hex(),
                                "tail_addr": base + len(data) - 16, "tail": data[-16:].hex()}
    return job


def prepare_emulator_cue(source, out):
    """Redux's pinned CUE reader rejects DCP; preserve raw input and track layout."""
    if not source.lower().endswith(".cue"):
        return source
    with open(source, "rb") as cue:
        raw = cue.read()
    lines = []
    removed = []
    tracks = []
    for line in raw.decode("utf-8-sig").splitlines():
        if re.fullmatch(r"\s*FLAGS\s+DCP\s*", line, re.IGNORECASE):
            removed.append(line.strip())
            continue
        if re.match(r"\s*FLAGS\b", line, re.IGNORECASE):
            die("unsupported CUE FLAGS directive requires explicit review: " + line.strip())
        match = re.fullmatch(r'\s*FILE\s+(?:"([^"]+)"|(\S+))\s+(\S+)\s*', line, re.IGNORECASE)
        if match:
            track = os.path.abspath(os.path.join(os.path.dirname(source), match[1] or match[2]))
            if not os.path.isfile(track) or any(c in track for c in ('"', '\n', '\r')):
                die("CUE track is missing or cannot be represented safely: " + track)
            with open(track, "rb") as original:
                digest = hashlib.file_digest(original, "sha256").hexdigest()
            tracks.append({"path": track, "bytes": os.path.getsize(track), "sha256": digest})
            line = 'FILE "%s" %s' % (track, match[3])
        lines.append(line)
    payload = ("\n".join(lines) + "\n").encode("utf-8")
    target = os.path.join(out, "emulator.cue")
    with open(target, "xb") as generated:
        generated.write(payload)
    with open(os.path.join(out, "cue-provenance.json"), "x") as provenance:
        json.dump({"source": source, "source_sha256": hashlib.sha256(raw).hexdigest(),
                   "generated_sha256": hashlib.sha256(payload).hexdigest(),
                   "tracks": tracks, "removed_copy_permission_directives": removed}, provenance, indent=2)
    return target


def run_emulator(args, job, out, extra=()):
    lua = "JOB = %s\n" % lua_value(job) + open(os.path.join(os.path.dirname(__file__), "emu.lua")).read()
    job_path = os.path.join(out, "job.lua")
    open(job_path, "w").write(lua)
    cards = os.path.join(ROOT, "build/emu")
    cmd = [REDUX, "-no-ui", "-stdout", "-lua_stdout", "-run", "-interpreter", "-debugger", "-bios", BIOS,
           "-iso", prepare_emulator_cue(find_iso(args), out), "-memcard1", os.path.join(cards, "memcard1.mcd"),
           "-memcard2", os.path.join(cards, "memcard2.mcd"), "-dofile", job_path] + list(extra)
    log_path = os.path.join(out, "emulator.log")
    env = dict(os.environ, SDL_AUDIO_DRIVER="dummy", SDL_AUDIODRIVER="dummy", SDL_VIDEO_DRIVER="dummy")
    t0 = time.time()
    with open(log_path, "w") as log:
        try:
            rc = subprocess.run(["nice", "-n", "19"] + cmd, cwd=out, env=env, stdout=log,
                                stderr=subprocess.STDOUT, timeout=args.timeout).returncode
        except subprocess.TimeoutExpired:
            rc = "timeout"
    print("emulator exit %s in %.0f s (log %s)" % (rc, time.time() - t0, os.path.relpath(log_path, ROOT)))
    result = os.path.join(out, "result.json")
    if not os.path.exists(result):
        tail = [l for l in open(log_path, errors="replace").read().splitlines() if "BIOS Trace" not in l]
        die("no result.json (run stopped before its end); emulator log tail:\n" + "\n".join(tail[-15:]))
    # carry each breakpoint's overlay and kind into its hits, for symbols and the call stack
    res = json.load(open(result))
    samples = as_list(res.pop("samples", []))
    if job.get("sample_every", 0):
        sample_path = os.path.join(out, "memory-samples.jsonl")
        with open(sample_path, "x") as stream:
            for sample in samples:
                stream.write(json.dumps(sample, sort_keys=True) + "\n")
        with open(sample_path, "rb") as stream:
            sample_sha = hashlib.file_digest(stream, "sha256").hexdigest()
        res["memory_samples"] = {"path": sample_path, "count": len(samples), "sha256": sample_sha,
                                 "every_vsync": job["sample_every"]}
    by_label = {b["label"]: b for b in job["breaks"]}
    for h in as_list(res.get("hits")):
        b = by_label.get(h.get("label"), {})
        h["overlay"], h["kind"] = b.get("overlay"), b.get("kind")
    for shot in as_list(res.get("shots")):
        prepare_shot(shot, out)
    json.dump(res, open(result, "w"))
    report(result, args.verbose)


def validate_sampling(every, frames, dumps, command):
    if every < 0:
        raise ValueError("sample interval must be nonnegative")
    if not every:
        return
    if command == "gdb" or not dumps or frames < 1:
        raise ValueError("bounded memory sampling needs --dump and a finite non-GDB run")
    samples = frames // every
    if samples < 1 or samples > 4096 or samples * sum(length * 2 + 128 for _, length in dumps) > 64 * 1024 * 1024:
        raise ValueError("memory samples must fit 1..4096 records and 64 MiB; shorten run or increase interval")


def parse_dump(spec):
    a, _, n = spec.partition(":")
    try:
        address, length = int(a, 16), int(n or "64", 0)
    except ValueError as exc:
        raise argparse.ArgumentTypeError("dump requires HEXADDR[:positive byte length]") from exc
    if not 0 <= address <= 0xffffffff or not 0 < length <= 0x200000:
        raise argparse.ArgumentTypeError("dump address or length is outside PS1 memory bounds")
    physical = address & 0x1fffffff
    ram = physical < 0x800000 and (physical & 0x1fffff) + length <= 0x200000
    scratch = 0x1f800000 <= physical and physical + length <= 0x1f800400
    bios = 0x1fc00000 <= physical and physical + length <= 0x1fc80000
    if address + length > 0x100000000 or not (ram or scratch or bios):
        raise argparse.ArgumentTypeError("dump must fit one readable RAM, scratchpad or BIOS region")
    return address, length


def main():
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    p.add_argument("cmd", choices=["boot", "break", "watch", "trace", "report", "gdb"])
    p.add_argument("targets", nargs="*")
    p.add_argument("--frames", type=int, default=3600, help="stop after this many vsyncs (default 3600)")
    p.add_argument("--sample-every", type=int, default=0,
                   help="write requested --dump memory every N vsyncs (0 disables; bounded to 4096 records/64 MiB)")
    p.add_argument("--hits", type=int, default=1, help="hits to record; the run stops after them")
    p.add_argument("--width", type=lambda s: int(s, 0), default=4,
                   help="break: bytes covered from TARGET (e.g. 0x30000: any code run in the overlay slot)")
    p.add_argument("--no-stop", action="store_true", help="record --hits hits but run all --frames")
    p.add_argument("--overlay", help="overlay of the targets (else TARGET@OVERLAY)")
    p.add_argument("--all-named", action="store_true", help="trace: every known function (of --overlay, else of the main exe)")
    p.add_argument("--read", action="store_true", help="watch: reads instead of writes")
    p.add_argument("--dump", type=parse_dump, action="append", default=[],
                   help="HEXADDR[:LEN] memory to save at each hit (boot: at the end)")
    p.add_argument("--input", help="pad 1 presses: FRAME:BUTTON[+BUTTON][/HOLD],...")
    p.add_argument("--mash", action="append", help="BUTTON:FIRST:PERIOD[:LAST] repeated presses")
    p.add_argument("--shot", help="screenshot at these frames: F,F,...")
    p.add_argument("--final-shot", action="store_true", help="screenshot when the run stops")
    p.add_argument("--until-overlay", metavar="NAME", help="stop when this overlay is loaded")
    p.add_argument("--state", help="load this save state at the start")
    p.add_argument("--save-state", help="write a save state when the run stops")
    p.add_argument("--ram-dump", action="store_true", help="save the 2 MB RAM when the run stops")
    p.add_argument("--iso", help="disc image (default: disc/*.cue)")
    p.add_argument("--out", help="output directory (default build/emu/<cmd>-<time>)")
    p.add_argument("--timeout", type=int, help="wall-clock limit in seconds (default 900; gdb: none)")
    p.add_argument("--port", type=int, default=3333, help="gdb: GDB server port")
    p.add_argument("-v", "--verbose", action="store_true", help="all registers and hits in the report")
    args = p.parse_args()

    if args.cmd == "report":
        if not args.targets:
            die("report needs RESULT.json")
        report(args.targets[0], args.verbose)
        return
    if args.cmd in ("break", "trace", "watch") and not (args.targets or args.all_named):
        die("%s needs at least one target" % args.cmd)
    if args.timeout is None and args.cmd != "gdb":
        args.timeout = 900
    out = os.path.abspath(args.out or os.path.join(ROOT, "build/emu", "%s-%s" % (
        args.cmd, time.strftime("%Y%m%d-%H%M%S"))))
    os.makedirs(out, exist_ok=True)
    image = Image()
    job = build_job(args, out, image, Symbols(image))
    extra = []
    if args.cmd == "gdb":
        job["frames"] = 1 << 30
        extra = ["-gdb", "-gdb-port", str(args.port)]
        print("GDB server on port %d: gdb-multiarch -ex 'set architecture mips:3000' "
              "-ex 'target remote localhost:%d'" % (args.port, args.port), flush=True)
    run_emulator(args, job, out, extra)


if __name__ == "__main__":
    main()
