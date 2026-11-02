#!/usr/bin/env python3
"""Scripted emulator route through the main screens, with screenshots (T-39504).

Plays a route file (default tools/emu/routes/main.route) from power-on in the tools/emu image
(PCSX-Redux, OpenBIOS, tools/emu/route.lua) and writes build/emu/route-<label>/: numbered PNGs,
result.json (checkpoint name -> frame -> file) and a contact sheet (sheet.html, sheet.png).
The route waits on the game's own scene bytes (GameState game_mode and friends) instead of
fixed frame counts, so it survives text of another length. See wiki/emulator.md, "Route".

Usage (from the host, through tools/emu/run.sh):
  EMU_CUE=/path/to/disc.cue tools/emu/run.sh route run --label jp
  tools/emu/run.sh route run --label en --build en --cue build/disc/en.cue
  tools/emu/run.sh route run --label jp --resume build/emu/route-jp:title_menu   (from a saved point)
  tools/emu/run.sh route compare build/emu/route-jp build/emu/route-en           (side by side)
  tools/emu/run.sh route sheet build/emu/route-jp                                (redo the sheet)

Route file, one step per line (# starts a comment):
  shot NAME                     screenshot, checkpoint NAME
  press BUTTON[+BUTTON] [hold=N] [gap=N]
  idle N                        wait N vsyncs
  wait COND... [timeout=N] [stable=N] [min=N] [mash=BUTTON[+BUTTON]/EVERY[/HOLD]] [delay=N]
       [shots=NAME/EVERY[/MAX]] [optional] [label=TEXT]
                                COND: FIELD=V[|V...] or FIELD!=V[|V...]; FIELD is mode, stage, step,
                                substep, ovl or a hex address (one byte)
  talk NAME COND... [press=BUTTON] [settle=N] [max=N] [repress=N] [timeout=N] [optional]
                                dialogue until COND: a screenshot of every whole page (the typewriter
                                has shown all of it), then BUTTON (default CIRCLE) for the next page
  steer x=N y=N [tol=N] [boost=BUTTON] [timeout=N]
                                move the free cursor (cursor_x, cursor_y) by holding the d-pad
  pick N [menu=0|1] [press=BUTTON] [timeout=N]
                                step the keyboard menu selection (menu_mode 1: the d-pad moves
                                GameState menu_cursor_0/1) with DOWN until it is N
  label NAME                    a jump target
  goto NAME [COND...] [max=N]   jump to `label NAME` while every COND holds, at most N times
                                (default 1)
  state NAME                    save state OUT/states/NAME.sstate (a --resume point)
  call FUNC [ARG...]            steps from a function of the selected build (BUILDS below);
                                _ in an argument stands for a space
"""

import argparse
import html
import json
import os
import re
import shutil
import struct
import subprocess
import sys
import time
import zlib

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import emu  # noqa: E402

ROOT = emu.ROOT
HERE = os.path.dirname(os.path.abspath(__file__))
DEFAULT_ROUTE = os.path.join(HERE, "routes", "main.route")

# GameState scene bytes (include/game/game_state.h; wiki/game-state.md)
FIELDS = {
    "mode": 0x800E7388,     # game_mode (GameMode, include/game_enums.h)
    "stage": 0x800E7389,    # mode_stage
    "step": 0x800E738A,     # mode_step
    "substep": 0x800E738D,  # substep
    "ovl": 0x800E738E,      # ovl_target_game_mode, the loader's mode byte (wiki/overlays.md)
}
KEY_FIELDS = ["mode", "stage", "step", "substep", "ovl"]
MENU_CURSOR = 0x800E7313  # GameState menu_cursor_0, menu_cursor_1 at +1 (s8)
CURSOR_X, CURSOR_Y = 0x8011ECF6, 0x8011ECFA  # s16 cursor_x, cursor_y (include/main/cursor.h)
TEXT_STATE = 0x800B3F60  # TextState text_state (include/main/text.h): total at +2, pos at +8
BUTTONS = set(emu.BUTTONS)
# RAM addresses above are those of the default (matching) link. A relinked build (`run --relink DIR`,
# tools/english_link.py, wiki/relink.md) moves them; ram() gives the address in the build being played.
_relinked = None


def ram(addr):
    """Address `addr` of the default link in the build being played (unchanged unless --relink)."""
    return _relinked(addr) if _relinked else addr


def use_relink(out_dir):
    """Translate main-executable addresses through the relinked build in out_dir (build/en, build/jr)."""
    global _relinked
    sys.path.insert(0, os.path.join(ROOT, "tools"))
    import english_link  # noqa: E402
    lay = english_link.Layout("main", out_dir, english_link.redirected_objects(out_dir, "main"))
    syms = english_link.symbols(os.path.join(out_dir, english_link.EXE + ".elf"))
    lo, hi = english_link.MAIN_RANGE
    _relinked = lambda a: lay.value(a, syms) if lo <= a <= hi else a


def read_text(path):
    with open(path, encoding="utf-8", errors="replace") as f:
        return f.read()


def write_text(path, text):
    with open(path, "w", encoding="utf-8") as f:
        f.write(text)


def read_json(path):
    with open(path, encoding="utf-8") as f:
        return json.load(f)


class RouteError(ValueError):
    """A route file or route option that cannot be played."""


# ---------------------------------------------------------------- builds

def hiragana_cell(char):
    """(column, row) of a hiragana cell on the original name-entry page: three blocks of five
    columns (a gap column between blocks), six rows; block b row r holds the row string
    r * 3 + b of the page table (NAME_ENT name_entry_hiragana_rows). Gojuon order, so the
    table is the standard one and is written out here by romaji instead of game text."""
    rows = ["a i u e o", "ka ki ku ke ko", "sa shi su se so", "ta chi tsu te to", "na ni nu ne no",
            "ha hi fu he ho", "ma mi mu me mo", "ya yu yo - -", "ra ri ru re ro", "wa wo n - -",
            "xa xi xu xe xo", "xtsu xya xyu xyo ~", "ga gi gu ge go", "za ji zu ze zo",
            "da dji dzu de do", "ba bi bu be bo", "pa pi pu pe po"]
    for i, row in enumerate(rows):
        cells = row.split()
        if char in cells and char != "-":
            return (i % 3) * 6 + cells.index(char), i // 3
    raise RouteError("no hiragana cell for %r" % char)


# English letter page (src/english/name_entry.c, T-39506): columns 1-13 of the grid, rows A-M,
# N-Z, a-m, n-z; row 4 holds - ' . in columns 1-3 and the space key in columns 5-8.
LATIN_ROWS = ["ABCDEFGHIJKLM", "NOPQRSTUVWXYZ", "abcdefghijklm", "nopqrstuvwxyz", "-'."]
LATIN_FIRST_COLUMN = 1
LATIN_SPACE_CELL = (5, 4)


def latin_cell(char):
    """(column, row) of a character on the English name-entry page (see LATIN_ROWS)."""
    if char == " ":
        return LATIN_SPACE_CELL
    for row, letters in enumerate(LATIN_ROWS):
        if char in letters:
            return LATIN_FIRST_COLUMN + letters.index(char), row
    raise RouteError("no letter cell for %r" % char)


def cell_center(column, row):
    """Cursor position at the centre of a name-entry cell (name_entry_cell_hit_test:
    column = (x + 0x78) / 16, row = (y + 0x30) / 16). hud_cursor_update stops the cursor at
    x 0x8C, inside the last column, so the target there is moved left."""
    return min(column * 16 - 0x78 + 8, 0x8C - 3), row * 16 - 0x30 + 8


def type_name(cells):
    """Route lines that steer the cursor onto each cell and put its character (CIRCLE)."""
    lines = []
    for column, row in cells:
        x, y = cell_center(column, row)
        lines += ["steer x=%d y=%d" % (x, y), "press CIRCLE gap=8"]
    return lines


def name_entry_steps(cell_of, parts):
    """Family name, given name and nickname typed on the default page, each closed with START
    (name_entry_*_input: START finishes the part, an empty part takes a preset name)."""
    lines = []
    for i, part in enumerate(parts):
        # name_entry_mode_stage_input: steps 1, 3 and 5 take the three parts
        lines += ["wait mode=0x14 stage=1 step=%d timeout=1200 stable=20 label=name_part_%d" % (2 * i + 1, i)]
        if i == 0:
            lines += ["shot name_entry_page"]
        lines += type_name([cell_of(c) for c in part])
        lines += ["shot name_entry_%s" % ("family", "given", "nickname")[i], "press START gap=20"]
    return lines


def jp_name_entry(args):
    """Original disc: the name-entry page opens on hiragana (default family/given/nickname)."""
    parts = args or ["ya ma da", "ta ro u", "ta ro"]
    return name_entry_steps(hiragana_cell, [p.split() for p in parts])


def en_name_entry(args):
    """English build: the page opens on the letter page (A-Z, a-z); one argument per part."""
    parts = args or ["Smith", "Tom", "Tom"]
    return name_entry_steps(latin_cell, [list(p) for p in parts])


BUILDS = {
    "jp": {"name_entry": jp_name_entry},
    "en": {"name_entry": en_name_entry},
}


# ---------------------------------------------------------------- route parsing

def parse_int(text):
    try:
        return int(text, 0)
    except ValueError:
        raise RouteError("bad number %r" % text) from None


def parse_buttons(text):
    buttons = text.split("+")
    for b in buttons:
        if b not in BUTTONS:
            raise RouteError("unknown button %s" % b)
    return buttons


def parse_cond(token):
    m = re.fullmatch(r"([a-z]+|0x[0-9A-Fa-f]+)(!?=)([0-9A-Fa-fx|]+)", token)
    if not m:
        raise RouteError("bad condition %r" % token)
    name, op, values = m.groups()
    if name in FIELDS:
        addr = ram(FIELDS[name])
    elif name.startswith("0x"):
        addr = ram(int(name, 16))
    else:
        raise RouteError("unknown field %s" % name)
    vals = [parse_int(v) for v in values.split("|")]
    if any(not 0 <= v <= 0xFF for v in vals):
        raise RouteError("byte value out of range in %r" % token)
    return {"addr": addr, "values": vals, "neg": op == "!="}


def split_options(tokens):
    opts, rest = {}, []
    for t in tokens:
        if "=" in t and t.split("=", 1)[0] in OPTION_KEYS:
            k, v = t.split("=", 1)
            opts[k] = v
        elif t == "optional":
            opts["optional"] = True
        else:
            rest.append(t)
    return opts, rest


OPTION_KEYS = {"timeout", "stable", "min", "mash", "delay", "shots", "label", "hold", "gap",
               "x", "y", "tol", "boost", "press", "settle", "max", "repress", "menu"}


def parse_step(tokens, lineno):
    op, args = tokens[0], tokens[1:]
    opts, rest = split_options(args)
    where = "line %d" % lineno
    if op == "shot":
        if len(rest) != 1:
            raise RouteError("%s: shot NAME" % where)
        return {"op": "shot", "name": check_name(rest[0], where)}
    if op == "press":
        if len(rest) != 1:
            raise RouteError("%s: press BUTTON[+BUTTON]" % where)
        return {"op": "press", "buttons": parse_buttons(rest[0]), "hold": parse_int(opts.get("hold", "4")),
                "gap": parse_int(opts.get("gap", "12"))}
    if op == "idle":
        if len(rest) != 1:
            raise RouteError("%s: idle FRAMES" % where)
        return {"op": "idle", "frames": parse_int(rest[0])}
    if op == "wait":
        if not rest:
            raise RouteError("%s: wait needs a condition" % where)
        step = {"op": "wait", "all": [parse_cond(t) for t in rest], "timeout": parse_int(opts.get("timeout", "1800")),
                "stable": parse_int(opts.get("stable", "1")), "min": parse_int(opts.get("min", "0")),
                "optional": bool(opts.get("optional")), "label": opts.get("label", " ".join(rest))}
        if "mash" in opts:
            parts = opts["mash"].split("/")
            if len(parts) not in (2, 3):
                raise RouteError("%s: mash=BUTTON/EVERY[/HOLD]" % where)
            step["mash"] = {"buttons": parse_buttons(parts[0]), "every": parse_int(parts[1]),
                            "hold": parse_int(parts[2]) if len(parts) == 3 else 4}
            step["mash_delay"] = parse_int(opts.get("delay", "0"))
        if "shots" in opts:
            parts = opts["shots"].split("/")
            if len(parts) not in (2, 3):
                raise RouteError("%s: shots=NAME/EVERY[/MAX]" % where)
            step["name"] = check_name(parts[0], where)
            step["shot_every"] = parse_int(parts[1])
            step["max_shots"] = parse_int(parts[2]) if len(parts) == 3 else 12
        return step
    if op == "pick":
        if len(rest) != 1:
            raise RouteError("%s: pick N" % where)
        menu = parse_int(opts.get("menu", "0"))
        if menu not in (0, 1):
            raise RouteError("%s: menu is 0 or 1" % where)
        n = parse_int(rest[0])
        return {"op": "wait", "all": [{"addr": ram(MENU_CURSOR + menu), "values": [n & 0xFF], "neg": False}],
                "timeout": parse_int(opts.get("timeout", "600")), "stable": 4, "min": 0, "optional": False,
                "label": "pick %d" % n, "mash": {"buttons": parse_buttons(opts.get("press", "DOWN")), "every": 12,
                                                 "hold": 4}, "mash_delay": 0}
    if op == "talk":
        if len(rest) < 2:
            raise RouteError("%s: talk NAME COND..." % where)
        return {"op": "talk", "name": check_name(rest[0], where), "all": [parse_cond(t) for t in rest[1:]],
                "buttons": parse_buttons(opts.get("press", "CIRCLE")), "settle": parse_int(opts.get("settle", "8")),
                "max": parse_int(opts.get("max", "99")), "repress": parse_int(opts.get("repress", "240")),
                "timeout": parse_int(opts.get("timeout", "36000")), "optional": bool(opts.get("optional")),
                "text_addr": ram(TEXT_STATE), "label": opts.get("label", "talk " + rest[0])}
    if op == "steer":
        if rest or "x" not in opts or "y" not in opts:
            raise RouteError("%s: steer x=N y=N" % where)
        return {"op": "steer", "x": parse_int(opts["x"]), "y": parse_int(opts["y"]),
                "tol": parse_int(opts.get("tol", "3")), "boost": parse_buttons(opts.get("boost", "TRIANGLE")),
                "timeout": parse_int(opts.get("timeout", "900")), "xaddr": ram(CURSOR_X), "yaddr": ram(CURSOR_Y),
                "label": "steer %s,%s" % (opts["x"], opts["y"])}
    if op == "label":
        if len(rest) != 1:
            raise RouteError("%s: label NAME" % where)
        return {"op": "label", "label": check_name(rest[0], where)}
    if op == "goto":
        if not rest:
            raise RouteError("%s: goto NAME [COND...]" % where)
        return {"op": "goto", "to": check_name(rest[0], where), "all": [parse_cond(t) for t in rest[1:]],
                "max": parse_int(opts.get("max", "1")), "label": "goto " + rest[0]}
    if op == "state":
        if len(rest) != 1:
            raise RouteError("%s: state NAME" % where)
        return {"op": "save", "state": check_name(rest[0], where)}
    raise RouteError("%s: unknown step %s" % (where, op))


def check_name(name, where):
    if not re.fullmatch(r"[a-z0-9_]+", name):
        raise RouteError("%s: names are lowercase letters, digits and _ (%r)" % (where, name))
    return name


def parse_route(text, build="jp"):
    """Route text -> list of step dicts (call lines expanded through BUILDS[build])."""
    if build not in BUILDS:
        raise RouteError("unknown build %s (one of %s)" % (build, ", ".join(sorted(BUILDS))))
    steps = []
    for lineno, raw in enumerate(text.splitlines(), 1):
        line = raw.split("#", 1)[0].strip()
        if not line:
            continue
        tokens = line.split()
        if tokens[0] == "call":
            if len(tokens) < 2 or tokens[1] not in BUILDS[build]:
                raise RouteError("line %d: unknown call %s for build %s" % (lineno, " ".join(tokens[1:2]), build))
            args = [a.replace("_", " ") for a in tokens[2:]]
            for sub in BUILDS[build][tokens[1]](args):
                step = parse_step(sub.split(), lineno)
                step["line"] = lineno
                steps.append(step)
            continue
        step = parse_step(tokens, lineno)
        step["line"] = lineno
        steps.append(step)
    names = [s["name"] for s in steps if s["op"] in ("shot", "talk") or "shot_every" in s]
    dup = sorted({n for n in names if names.count(n) > 1})
    if dup:
        raise RouteError("checkpoint names used twice: %s" % ", ".join(dup))
    labels = [s["label"] for s in steps if s["op"] == "label"]
    for s in steps:
        if s["op"] == "goto" and s["to"] not in labels:
            raise RouteError("line %d: no label %s" % (s["line"], s["to"]))
    dup = sorted({n for n in labels if labels.count(n) > 1})
    if dup:
        raise RouteError("labels used twice: %s" % ", ".join(dup))
    return steps


def resume_index(steps, name):
    """Index of the first step after `state NAME`."""
    for i, s in enumerate(steps):
        if s["op"] == "save" and s.get("state") == name:
            return i + 1
    raise RouteError("no state %s in the route" % name)


# ---------------------------------------------------------------- PNG and sheets

def png_write(path, width, height, rows):
    """rows: list of bytes, 3 * width each (RGB)."""
    def chunk(tag, data):
        c = tag + data
        return struct.pack(">I", len(data)) + c + struct.pack(">I", zlib.crc32(c) & 0xFFFFFFFF)
    raw = b"".join(b"\0" + bytes(r) for r in rows)
    with open(path, "wb") as f:
        f.write(b"\x89PNG\r\n\x1a\n")
        f.write(chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 8, 2, 0, 0, 0)))
        f.write(chunk(b"IDAT", zlib.compress(raw, 6)))
        f.write(chunk(b"IEND", b""))


def png_read(path):
    """(width, height, rows) of an 8-bit RGB PNG with filter 0 rows (what png_write and
    emu.png_from_raw write)."""
    with open(path, "rb") as f:
        data = f.read()
    if data[:8] != b"\x89PNG\r\n\x1a\n":
        raise ValueError("not a PNG: %s" % path)
    pos, idat, width = 8, b"", None
    while pos < len(data):
        n, tag = struct.unpack_from(">I4s", data, pos)
        body = data[pos + 8:pos + 8 + n]
        if tag == b"IHDR":
            width, height, depth, color = struct.unpack_from(">IIBB", body)
            if depth != 8 or color != 2:
                raise ValueError("only 8-bit RGB PNGs: %s" % path)
        elif tag == b"IDAT":
            idat += body
        pos += 12 + n
    raw = zlib.decompress(idat)
    stride = width * 3 + 1
    rows = []
    for y in range(height):
        line = raw[y * stride:(y + 1) * stride]
        if line[0] != 0:
            raise ValueError("unsupported PNG row filter in %s" % path)
        rows.append(line[1:])
    return width, height, rows


def scale_rows(width, height, rows, out_w, out_h):
    """Nearest-neighbour resize of RGB rows."""
    xs = [min(width - 1, x * width // out_w) for x in range(out_w)]
    out = []
    for y in range(out_h):
        src = rows[min(height - 1, y * height // out_h)]
        line = bytearray(out_w * 3)
        for i, x in enumerate(xs):
            line[i * 3:i * 3 + 3] = src[x * 3:x * 3 + 3]
        out.append(line)
    return out


# 3x5 pixel font for the PNG sheet labels: each glyph is five rows of three bits
FONT = {
    "a": "010101111101101", "b": "110101110101110", "c": "011100100100011", "d": "110101101101110",
    "e": "111100110100111", "f": "111100110100100", "g": "011100101101011", "h": "101101111101101",
    "i": "111010010010111", "j": "001001001101010", "k": "101101110101101", "l": "100100100100111",
    "m": "101111111101101", "n": "110101101101101", "o": "010101101101010", "p": "110101110100100",
    "q": "010101101110011", "r": "110101110101101", "s": "011100010001110", "t": "111010010010010",
    "u": "101101101101111", "v": "101101101101010", "w": "101101111111101", "x": "101101010101101",
    "y": "101101010010010", "z": "111001010100111", "0": "111101101101111", "1": "010110010010111",
    "2": "110001010100111", "3": "110001010001110", "4": "101101111001001", "5": "111100110001110",
    "6": "011100111101111", "7": "111001010010010", "8": "111101111101111", "9": "111101111001110",
    "_": "000000000000111", "-": "000000111000000", ".": "000000000000010", ":": "000010000010000",
    " ": "000000000000000", "@": "010101111100011",
}


def draw_text(canvas, cw, x0, y0, text, scale=2, color=(255, 255, 255)):
    """Draw lowercase text into canvas (list of bytearrays, width cw) at x0, y0."""
    x = x0
    for ch in text.lower():
        glyph = FONT.get(ch, FONT["."])
        for gy in range(5):
            for gx in range(3):
                if glyph[gy * 3 + gx] == "1":
                    for dy in range(scale):
                        for dx in range(scale):
                            px, py = x + gx * scale + dx, y0 + gy * scale + dy
                            if 0 <= px < cw and 0 <= py < len(canvas):
                                canvas[py][px * 3:px * 3 + 3] = bytes(color)
        x += 4 * scale
        if x >= cw:
            break


CELL_W, CELL_H, LABEL_H = 256, 192, 14


def grid_png(path, cells, columns):
    """cells: list of (label, png path or None); writes a labelled grid PNG."""
    rows_n = max(1, (len(cells) + columns - 1) // columns)
    cw, ch = columns * CELL_W, rows_n * (CELL_H + LABEL_H)
    canvas = [bytearray(b"\x20" * (cw * 3)) for _ in range(ch)]
    for i, (label, png) in enumerate(cells):
        cx, cy = (i % columns) * CELL_W, (i // columns) * (CELL_H + LABEL_H)
        draw_text(canvas, cw, cx + 2, cy + 2, label)
        if png and os.path.exists(png):
            w, h, rows = png_read(png)
            for y, line in enumerate(scale_rows(w, h, rows, CELL_W - 4, CELL_H - 4)):
                canvas[cy + LABEL_H + y][(cx + 2) * 3:(cx + 2) * 3 + len(line)] = line
    png_write(path, cw, ch, canvas)


PAGE_CSS = """
:root { --bg: #f6f5f2; --fg: #1d1d1b; --muted: #6b6a66; --card: #ffffff; --line: #d9d7d0; }
@media (prefers-color-scheme: dark) { :root { --bg: #1b1b1a; --fg: #ecebe6; --muted: #a09f99;
  --card: #262624; --line: #3a3936; } }
body { background: var(--bg); color: var(--fg); font: 14px/1.4 system-ui, sans-serif; margin: 16px; }
h1 { font-size: 18px; } .meta { color: var(--muted); }
.grid { display: grid; grid-template-columns: repeat(auto-fill, minmax(260px, 1fr)); gap: 12px; }
figure { margin: 0; background: var(--card); border: 1px solid var(--line); border-radius: 6px; padding: 6px; }
figure img { width: 100%; image-rendering: pixelated; display: block; }
figcaption { font-size: 12px; color: var(--muted); margin-top: 4px; word-break: break-all; }
table { border-collapse: collapse; width: 100%; } td, th { border-bottom: 1px solid var(--line);
  padding: 6px; vertical-align: top; text-align: left; } td img { width: 100%; max-width: 480px;
  image-rendering: pixelated; } .missing { color: var(--muted); font-style: italic; }
"""


def write_sheet(out):
    """sheet.html and sheet.png from out/result.json."""
    res = read_json(os.path.join(out, "result.json"))
    figs = []
    for c in res["checkpoints"]:
        img = ('<img src="%s" alt="%s">' % (html.escape(c["file"]), html.escape(c["name"]))
               if c.get("file") else '<span class="missing">no image</span>')
        figs.append('<figure>%s<figcaption>%s<br>frame %d, %s</figcaption></figure>' % (
            img, html.escape(c["name"]), c["frame"], html.escape(key_text(c.get("key")))))
    meta = "%s; stop: %s after %d frames; %s s wall time" % (
        html.escape(res.get("disc", "")), html.escape(str(res.get("reason"))), res.get("frames", 0),
        res.get("wall_seconds", "?"))
    page = ("<!doctype html><html><head><meta charset=utf-8><meta name=viewport "
            "content='width=device-width, initial-scale=1'><title>Route %s</title><style>%s</style></head>"
            "<body><h1>Route %s</h1><p class=meta>%s</p><div class=grid>%s</div></body></html>") % (
        html.escape(res["label"]), PAGE_CSS, html.escape(res["label"]), meta, "".join(figs))
    write_text(os.path.join(out, "sheet.html"), page)
    grid_png(os.path.join(out, "sheet.png"),
             [(c["name"], os.path.join(out, c["file"]) if c.get("file") else None) for c in res["checkpoints"]], 6)


def key_text(key):
    if not key:
        return ""
    return " ".join("%s %02X" % (f, v) for f, v in zip(KEY_FIELDS, key))


def pixels_differing(path_a, path_b):
    """Number of pixels that differ between two screenshots (-1 if their sizes differ)."""
    wa, ha, ra = png_read(path_a)
    wb, hb, rb = png_read(path_b)
    if (wa, ha) != (wb, hb):
        return -1
    n = 0
    for x, y in zip(ra, rb):
        if x != y:
            n += sum(1 for i in range(0, len(x), 3) if x[i:i + 3] != y[i:i + 3])
    return n


def compare(dir_a, dir_b, out):
    """compare.html and compare.png: the checkpoints of two runs side by side, by name; the header of
    each row says whether the two screenshots are identical pixel for pixel (T-39505)."""
    runs = []
    for d in (dir_a, dir_b):
        res = read_json(os.path.join(d, "result.json"))
        runs.append((d, res, {c["name"]: c for c in res["checkpoints"]}))
    names = []
    for _, res, _ in runs:
        for c in res["checkpoints"]:
            if c["name"] not in names:
                names.append(c["name"])
    os.makedirs(out, exist_ok=True)
    rows, cells = [], []
    same = []
    for name in names:
        tds = []
        a, b = runs[0][2].get(name), runs[1][2].get(name)
        verdict = ""
        if a and b and a.get("file") and b.get("file"):
            n = pixels_differing(os.path.join(runs[0][0], a["file"]), os.path.join(runs[1][0], b["file"]))
            verdict = "identical" if n == 0 else "%d px differ" % n if n > 0 else "sizes differ"
            same.append(n == 0)
        for d, res, by in runs:
            c = by.get(name)
            if c and c.get("file"):
                src = os.path.relpath(os.path.join(d, c["file"]), out)
                tds.append('<td><img src="%s" alt="%s"><br><span class=meta>frame %d</span></td>' % (
                    html.escape(src), html.escape(name), c["frame"]))
                cells.append(("%s %s" % (res["label"], name), os.path.join(d, c["file"])))
            else:
                tds.append('<td class=missing>not reached</td>')
                cells.append(("%s %s" % (res["label"], name), None))
        rows.append("<tr><th>%s<br><span class=meta>%s</span></th>%s</tr>" % (
            html.escape(name), verdict, "".join(tds)))
    print("%d of %d shared checkpoints identical" % (sum(same), len(same)))
    head = "".join("<th>%s</th>" % html.escape(res["label"]) for _, res, _ in runs)
    page = ("<!doctype html><html><head><meta charset=utf-8><meta name=viewport "
            "content='width=device-width, initial-scale=1'><title>Route compare</title><style>%s</style>"
            "</head><body><h1>Route compare</h1><table><tr><th>checkpoint</th>%s</tr>%s</table></body></html>") % (
        PAGE_CSS, head, "".join(rows))
    write_text(os.path.join(out, "compare.html"), page)
    grid_png(os.path.join(out, "compare.png"), cells, 2)
    return os.path.join(out, "compare.html")


# ---------------------------------------------------------------- running

def build_job(steps, out, frames, load_state=None, probes=()):
    """Job for route.lua from the steps to play (all of them, or those after a resume point)."""
    where = {s["label"]: i + 1 for i, s in enumerate(steps) if s["op"] == "label"}  # Lua counts from 1
    lua_steps = []
    for s in steps:
        s = dict(s)
        if s["op"] == "save":
            s["path"] = os.path.join(out, "states", s["state"] + ".sstate")
        if s["op"] == "goto":
            if s["to"] not in where:
                raise RouteError("goto %s: the label lies before the resume point" % s["to"])
            s["target"] = where[s["to"]]
        lua_steps.append(s)
    return {"out": out, "steps": lua_steps, "frames": frames, "key_addrs": [ram(FIELDS[f]) for f in KEY_FIELDS],
            "cursor_addrs": [ram(CURSOR_X), ram(CURSOR_Y)], "probes": [list(p) for p in probes],
            "load_state": load_state}


def result_from_raw(raw, out, offset=0, earlier=(), first=0):
    """Checkpoints (name, frame, file, key) from route.lua's raw output; converts the shots."""
    checkpoints = list(earlier)
    for shot in raw.get("shots", []):
        png = "%03d_%s.png" % (len(checkpoints) + 1, shot["name"])
        raw_path = os.path.join(out, os.path.basename(shot["raw"]))
        entry = {"name": shot["name"], "frame": shot["frame"] + offset, "step": (shot.get("step") or 0) + first,
                 "key": shot.get("key"),
                 "cursor": shot.get("cursor"), "probes": shot.get("probes") or [],
                 "width": shot.get("width"), "height": shot.get("height")}
        try:
            emu.png_from_raw(raw_path, shot["width"], shot["height"], shot["bpp"], os.path.join(out, png))
            entry["file"] = png
        except (OSError, ValueError) as exc:
            entry["file"] = None
            entry["error"] = str(exc)
        checkpoints.append(entry)
    return checkpoints


def resume_point(spec, steps, out):
    """--resume RUN_DIR:STATE: (state file, its frame, first step to play, earlier checkpoints
    and states). The earlier run's checkpoints before the `state` step are copied into out."""
    src_dir, _, name = spec.rpartition(":")
    src_dir = os.path.abspath(src_dir)
    first = resume_index(steps, name)
    src = read_json(os.path.join(src_dir, "result.json"))
    if name not in src.get("states", {}):
        raise RouteError("%s saved no state %s" % (src_dir, name))
    offset = src["states"][name]
    earlier = []
    for c in src["checkpoints"]:
        if c["step"] >= first:  # route.lua numbers steps from 1: step `first` is the `state` step
            break
        if c.get("file"):
            shutil.copy(os.path.join(src_dir, c["file"]), os.path.join(out, c["file"]))
        earlier.append(dict(c))
    states = {k: v for k, v in src["states"].items() if v <= offset}
    return os.path.join(src_dir, "states", name + ".sstate"), offset, first, earlier, states


def play(job, cue, out, timeout):
    """Run route.lua in PCSX-Redux; returns (raw result or None, emulator exit, wall seconds)."""
    lua = "JOB = %s\n" % emu.lua_value(job) + read_text(os.path.join(HERE, "route.lua"))
    job_path = os.path.join(out, "job.lua")
    write_text(job_path, lua)
    cmd = ["nice", "-n", "19", emu.REDUX, "-no-ui", "-stdout", "-lua_stdout", "-run", "-interpreter", "-debugger",
           "-bios", emu.BIOS, "-iso", emu.prepare_emulator_cue(cue, out),
           "-memcard1", os.path.join(out, "cards", "memcard1.mcd"),
           "-memcard2", os.path.join(out, "cards", "memcard2.mcd"), "-dofile", job_path]
    env = dict(os.environ, SDL_AUDIO_DRIVER="dummy", SDL_AUDIODRIVER="dummy", SDL_VIDEO_DRIVER="dummy")
    t0 = time.time()
    with open(os.path.join(out, "emulator.log"), "w") as log:
        try:
            rc = subprocess.run(cmd, cwd=out, env=env, stdout=log, stderr=subprocess.STDOUT,
                                timeout=timeout).returncode
        except subprocess.TimeoutExpired:
            rc = "timeout"
    raw_path = os.path.join(out, "route_result.json")
    raw = None
    if os.path.exists(raw_path):
        raw = read_json(raw_path)
        os.remove(raw_path)
    return raw, rc, round(time.time() - t0)


def run(args):
    if args.relink:
        use_relink(args.relink)
    steps = parse_route(read_text(args.route), args.build)
    out = os.path.abspath(args.out or os.path.join(ROOT, "build/emu", "route-" + args.label))
    if os.path.exists(out):
        if not args.force:
            emu.die("%s exists (pass --force to replace it)" % os.path.relpath(out, ROOT))
        shutil.rmtree(out)
    os.makedirs(os.path.join(out, "states"))
    os.makedirs(os.path.join(out, "cards"))  # new cards: the file select formats an album on slot 1
    load_state, offset, first, earlier, states = None, 0, 0, [], {}
    if args.resume:
        load_state, offset, first, earlier, states = resume_point(args.resume, steps, out)
    job = build_job(steps[first:], out, args.frames, load_state, args.probe)
    cue = emu.find_iso(args)
    raw, rc, wall = play(job, cue, out, args.timeout)
    if raw is None:
        tail = read_text(os.path.join(out, "emulator.log")).splitlines()
        emu.die("no route_result.json (emulator exit %s); log tail:\n%s" % (
            rc, "\n".join(l for l in tail[-15:] if "BIOS Trace" not in l)))
    for s in raw.get("steps", []):
        step = job["steps"][s["index"] - 1]
        s.update(index=s["index"] + first, line=step.get("line"), start=s["start"] + offset,
                 finish=s["finish"] + offset)
        if step["op"] == "save":
            states[step["state"]] = s["finish"]
    res = {"label": args.label, "build": args.build, "disc": os.path.basename(cue), "cue": cue,
           "route": os.path.relpath(args.route, ROOT), "reason": raw.get("reason"),
           "frames": raw.get("frames", 0) + offset, "wall_seconds": wall, "resumed_from": args.resume,
           "last_step": raw.get("last_step", 0) + first, "steps_total": len(steps),
           "checkpoints": result_from_raw(raw, out, offset, earlier, first), "states": states,
           "steps": raw.get("steps", []),
           "mode_log": [dict(m, frame=m["frame"] + offset) for m in raw.get("mode_log", [])],
           "notes": raw.get("notes", [])}
    write_text(os.path.join(out, "result.json"), json.dumps(res, indent=1))
    write_sheet(out)
    print_summary(res, out)
    return 0 if res["reason"] == "done" else 1


def print_summary(res, out):
    print("route %s: stop %s after %d frames, %s s (emulator exit)" % (
        res["label"], res["reason"], res["frames"], res["wall_seconds"]))
    for s in res["steps"]:
        if not s.get("ok"):
            print("  step %d (line %s) %s %s: %s" % (s["index"], s.get("line"), s["op"], s.get("label") or "", s.get("why")))
    shown = [c for c in res["checkpoints"] if not re.search(r"_\d\d$", c["name"])]
    for c in shown:
        print("  %-24s frame %6d  %s" % (c["name"], c["frame"], c.get("file") or "no image"))
    print("  (%d screenshots in all) sheet: %s" % (len(res["checkpoints"]),
                                                  os.path.relpath(os.path.join(out, "sheet.html"), ROOT)))


def main():
    p = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    sub = p.add_subparsers(dest="cmd", required=True)
    r = sub.add_parser("run", help="play the route and write build/emu/route-<label>/")
    r.add_argument("--label", default="jp", help="output name: build/emu/route-<label> (default jp)")
    r.add_argument("--build", default="jp", choices=sorted(BUILDS), help="per-build steps (name entry)")
    r.add_argument("--route", default=DEFAULT_ROUTE, help="route file")
    r.add_argument("--iso", "--cue", dest="iso", metavar="PATH",
                   help="disc cue inside the repository (default: $EMU_CUE, else the one disc/*.cue)")
    r.add_argument("--out", help="output directory (default build/emu/route-<label>)")
    r.add_argument("--resume", metavar="RUN_DIR:STATE", help="start from a `state` point of an earlier run")
    r.add_argument("--frames", type=int, default=60 * 60 * 30, help="vsync limit (default 30 minutes)")
    r.add_argument("--timeout", type=int, default=3600, help="wall-clock limit in seconds (default 3600)")
    r.add_argument("--force", action="store_true", help="replace an existing output directory")
    r.add_argument("--relink", metavar="DIR",
                   help="the disc holds the relinked build in DIR (build/en, build/jr): read the scene bytes "
                        "at their addresses there")
    r.add_argument("--probe", type=emu.parse_dump, action="append", default=[],
                   help="HEXADDR[:LEN] memory recorded with every screenshot (result.json probes)")
    c = sub.add_parser("compare", help="two runs side by side by checkpoint name")
    c.add_argument("run_a")
    c.add_argument("run_b")
    c.add_argument("--out", help="output directory (default build/emu/compare-<a>-<b>)")
    s = sub.add_parser("sheet", help="write the contact sheet of a run again")
    s.add_argument("run_dir")
    v = sub.add_parser("check", help="parse a route file and list its steps")
    v.add_argument("--route", default=DEFAULT_ROUTE)
    v.add_argument("--build", default="jp", choices=sorted(BUILDS))
    args = p.parse_args()
    try:
        if args.cmd == "run":
            return run(args)
        if args.cmd == "compare":
            out = args.out or os.path.join(ROOT, "build/emu", "compare-%s-%s" % (
                os.path.basename(os.path.normpath(args.run_a)), os.path.basename(os.path.normpath(args.run_b))))
            print(os.path.relpath(compare(args.run_a, args.run_b, os.path.abspath(out)), ROOT))
            return 0
        if args.cmd == "sheet":
            write_sheet(os.path.abspath(args.run_dir))
            return 0
        steps = parse_route(read_text(args.route), args.build)
        for i, st in enumerate(steps, 1):
            print("%3d line %3d %s" % (i, st["line"], json.dumps({k: v for k, v in st.items() if k != "line"})))
        return 0
    except RouteError as exc:
        emu.die("route: %s" % exc)


if __name__ == "__main__":
    sys.exit(main())
