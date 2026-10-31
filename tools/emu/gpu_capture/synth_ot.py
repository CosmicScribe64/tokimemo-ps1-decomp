"""T-39302: a synthetic ordering table that exercises the whole primitive repertoire of the renderer:
every polygon, line, tile and sprite type, the four semi-transparency modes, 4/8/15-bit textures with
CLUTs, texture windows, flips, dithering, mask bits, draw area and offset, and LoadImage/MoveImage/
Fill packets. All data is invented (deterministic pseudo-random texture pages and CLUTs);
nothing comes from the game. capture.lua copies it into RAM at DrawOTag entry and points DrawOTag
at it, so the emulator draws it over the real VRAM; the renderer draws the same bytes.

usage: python3 -I synth_ot.py <out.bin> <out.lua> [address (hex, default 8014E000) [groups, comma separated]]
Groups: polygons, semi, textured, lines, rects, sprites, flips, window_wrap, clip, mask, transfers (default: all).
The texture pages and CLUTs are always uploaded first.
Writes the table bytes and a Lua fragment `INJECT = {addr = ..., hex = '...'}`.
"""
import struct
import sys

PAGE15 = (640, 0)    # 15-bit texture page, 64x64 halfwords used
PAGE4 = (704, 0)     # 4-bit texture page
PAGE8 = (768, 0)     # 8-bit texture page
CLUT4 = (640, 256)   # 16 entries
CLUT8 = (640, 260)   # 256 entries
OUT_X, OUT_Y = 0, 0  # drawing region 320x240


class Rng:
    def __init__(self, seed):
        self.s = seed

    def next(self):
        self.s = (self.s * 1664525 + 1013904223) & 0xFFFFFFFF
        return self.s >> 8

    def pixel15(self, zero_rate=8, bit15_rate=3):
        v = self.next() & 0x7FFF
        if self.next() % zero_rate == 0:
            return 0
        if self.next() % bit15_rate == 0:
            v |= 0x8000
        return v or 1


def xy(x, y):
    return (x & 0x7FF) | ((y & 0x7FF) << 16)


def rgb(r, g, b):
    return r | (g << 8) | (b << 16)


def clut_word(cx, cy):
    return (cx // 16) | (cy << 6)


class Table:
    def __init__(self):
        self.packets = []

    def add(self, words):
        assert len(words) <= 255
        self.packets.append(list(words))

    def env(self, x1, y1, x2, y2, ox, oy, tpage=0, window=0, mask=0):
        self.add([0xE3000000 | x1 | (y1 << 10), 0xE4000000 | x2 | (y2 << 10),
                  0xE5000000 | (ox & 0x7FF) | ((oy & 0x7FF) << 11), 0xE2000000 | window,
                  0xE6000000 | mask, 0xE1000000 | tpage])

    def e1(self, tx=0, ty=0, semi=0, depth=0, dither=False, flipx=False, flipy=False):
        self.add([0xE1000000 | tx | (ty << 4) | (semi << 5) | (depth << 7) | (0x200 if dither else 0) |
                  (0x1000 if flipx else 0) | (0x2000 if flipy else 0)])

    def load(self, x, y, w, h, halfwords):
        words = []
        for i in range(0, len(halfwords), 2):
            lo = halfwords[i]
            hi = halfwords[i + 1] if i + 1 < len(halfwords) else 0
            words.append(lo | (hi << 16))
        self.add([0xA0000000, xy(x, y), (h << 16) | w] + words)

    def serialise(self, base):
        out = bytearray()
        addr = base
        for index, words in enumerate(self.packets):
            last = index == len(self.packets) - 1
            size = 4 + 4 * len(words)
            nxt = 0xFFFFFF if last else (addr + size) & 0xFFFFFF
            out += struct.pack('<I', (len(words) << 24) | nxt)
            out += struct.pack('<%dI' % len(words), *words)
            addr += size
        return bytes(out)


GROUPS = ('polygons', 'semi', 'textured', 'lines', 'rects', 'sprites', 'flips', 'window_wrap', 'clip', 'mask',
          'transfers')


def build(groups):
    t = Table()
    rng = Rng(12345)
    t.env(0, 0, 319, 239, 0, 0)
    # texture pages and CLUTs
    for y in range(64):
        t.load(PAGE15[0], PAGE15[1] + y, 64, 1, [rng.pixel15() for _ in range(64)])
        t.load(PAGE4[0], PAGE4[1] + y, 64, 1, [rng.next() & 0xFFFF for _ in range(64)])
        t.load(PAGE8[0], PAGE8[1] + y, 64, 1, [rng.next() & 0xFFFF for _ in range(64)])
    t.load(CLUT4[0], CLUT4[1], 16, 1, [0] + [rng.pixel15(99, 3) for _ in range(15)])
    t.load(CLUT8[0], CLUT8[1], 256, 1, [rng.pixel15(6, 3) for _ in range(256)])
    c4 = clut_word(*CLUT4)
    c8 = clut_word(*CLUT8)
    tp15, tp4, tp8 = PAGE15[0] // 64, PAGE4[0] // 64, PAGE8[0] // 64

    def tpage(tx, depth, semi=0):
        return tx | (semi << 5) | (depth << 7)

    if 'polygons' in groups:
        # --- flat and Gouraud polygons, all winding orders, dither on and off
        t.e1()
        t.add([0x20000000 | rgb(200, 60, 30), xy(8, 8), xy(40, 12), xy(20, 36)])                   # F3
        t.add([0x20000000 | rgb(30, 160, 220), xy(80, 8), xy(50, 40), xy(60, 10)])                  # F3 reversed
        t.add([0x28000000 | rgb(90, 200, 90), xy(90, 8), xy(130, 14), xy(96, 40), xy(126, 44)])      # F4
        t.add([0x2A000000 | rgb(255, 255, 255), xy(140, 8), xy(150, 8), xy(140, 18), xy(150, 18)])   # F4 semi
        t.e1(dither=True)
        t.add([0x30000000 | rgb(255, 0, 0), xy(170, 8), rgb(0, 255, 0), xy(210, 12), rgb(0, 0, 255), xy(186, 44)])      # G3
        t.add([0x38000000 | rgb(255, 255, 0), xy(220, 8), rgb(0, 255, 255), xy(260, 8),
               rgb(255, 0, 255), xy(220, 48), rgb(40, 40, 40), xy(260, 48)])                                           # G4
        t.e1(dither=False)
        t.add([0x38000000 | rgb(255, 255, 0), xy(270, 8), rgb(0, 255, 255), xy(310, 8),
               rgb(255, 0, 255), xy(270, 48), rgb(40, 40, 40), xy(310, 48)])                                           # G4 no dither
    if 'semi' in groups:
        # --- semi-transparent flat polygons, four modes
        for mode in range(4):
            t.e1(semi=mode)
            t.add([0x22000000 | rgb(160, 120, 80), xy(8 + mode * 36, 56), xy(40 + mode * 36, 56), xy(8 + mode * 36, 88)])
            t.add([0x2A000000 | rgb(80, 160, 240), xy(150 + mode * 40, 56), xy(185 + mode * 40, 56),
                   xy(150 + mode * 40, 90), xy(185 + mode * 40, 90)])
        # semi Gouraud with dither
        t.e1(semi=2, dither=True)
        t.add([0x32000000 | rgb(250, 10, 10), xy(8, 96), rgb(10, 250, 10), xy(60, 96), rgb(10, 10, 250), xy(30, 130)])
        t.e1(semi=1, dither=True)
        t.add([0x3A000000 | rgb(250, 100, 10), xy(70, 96), rgb(10, 250, 100), xy(120, 96),
               rgb(100, 10, 250), xy(70, 130), rgb(200, 200, 200), xy(120, 130)])
        t.e1()
    if 'textured' in groups:
        # --- textured polygons: 15-bit, 8-bit, 4-bit; raw, modulated; scaled and rotated
        t.add([0x24000000 | rgb(128, 128, 128), xy(130, 96), 0 | (c4 << 16), xy(170, 96), 0 | (tpage(tp4, 0) << 16) | 63,
               xy(130, 130), (63 << 8)])                                                                              # FT3 4-bit 1.6x
        t.add([0x2C000000 | rgb(200, 90, 255), xy(180, 96), 0 | (c8 << 16), xy(220, 100),
               0 | (tpage(tp8, 1) << 16) | 126, xy(184, 130), (60 << 8), xy(226, 134), (60 << 8) | 126])              # FT4 8-bit modulated
        t.add([0x2D000000, xy(230, 96), 0, xy(270, 96), (tpage(tp15, 2) << 16) | 31, xy(230, 130), 31 << 8,
               xy(270, 130), (31 << 8) | 31])                                                                        # FT4 15-bit raw 1.25x
        t.add([0x34000000 | rgb(255, 255, 255), xy(8, 140), 0 | (c4 << 16), rgb(80, 80, 80), xy(60, 140),
               (tpage(tp4, 0) << 16) | 63, rgb(255, 255, 255), xy(34, 170), (63 << 8) | 31])                         # GT3 4-bit
        t.add([0x3C000000 | rgb(255, 128, 128), xy(70, 140), 0 | (c8 << 16), rgb(128, 255, 128), xy(120, 140),
               (tpage(tp8, 1) << 16) | 100, rgb(128, 128, 255), xy(70, 175), (100 << 8),
               rgb(255, 255, 255), xy(120, 175), (100 << 8) | 100])                                                  # GT4 8-bit
        t.e1(dither=True)
        t.add([0x3C000000 | rgb(255, 128, 128), xy(130, 140), 0, rgb(128, 255, 128), xy(180, 140),
               (tpage(tp15, 2) << 16) | 63, rgb(128, 128, 255), xy(130, 175), (63 << 8),
               rgb(255, 255, 255), xy(180, 175), (63 << 8) | 63])                                                    # GT4 15-bit dither
        t.e1()
        for mode in range(4):
            t.add([0x2E000000 | rgb(160, 160, 160), xy(190 + mode * 30, 140), 0 | (c4 << 16),
                   xy(215 + mode * 30, 140), (tpage(tp4, 0, mode) << 16) | 40, xy(190 + mode * 30, 165), 40 << 8,
                   xy(215 + mode * 30, 165), (40 << 8) | 40])                                                        # FT4 semi, modes 0..3
    if 'lines' in groups:
        # --- lines
        t.e1()
        t.add([0x40000000 | rgb(255, 200, 0), xy(8, 184), xy(60, 190)])
        t.add([0x40000000 | rgb(255, 200, 0), xy(8, 186), xy(12, 230)])
        t.add([0x40000000 | rgb(0, 200, 255), xy(70, 184), xy(110, 224)])
        t.add([0x40000000 | rgb(0, 200, 255), xy(150, 184), xy(110, 230)])
        t.add([0x42000000 | rgb(250, 250, 250), xy(8, 200), xy(110, 200)])
        t.add([0x50000000 | rgb(255, 0, 0), xy(120, 184), rgb(0, 0, 255), xy(190, 214)])
        t.e1(dither=True)
        t.add([0x50000000 | rgb(255, 0, 0), xy(120, 190), rgb(0, 255, 0), xy(190, 232)])
        t.e1()
        t.add([0x48000000 | rgb(200, 255, 100), xy(200, 184), xy(230, 220), xy(260, 184), xy(290, 220), 0x55555555])
        t.add([0x58000000 | rgb(255, 0, 0), xy(200, 226), rgb(0, 255, 0), xy(240, 232), rgb(0, 0, 255), xy(280, 226),
               0x55555555])
    if 'rects' in groups:
        # --- rectangles (second pass on top; the area at the bottom right)
        t.add([0x60000000 | rgb(255, 0, 255), xy(270, 184), (10 << 16) | 14])
        t.add([0x68000000 | rgb(255, 255, 255), xy(300, 184)])
        t.add([0x70000000 | rgb(0, 255, 255), xy(290, 200)])
        t.add([0x78000000 | rgb(255, 128, 0), xy(268, 210)])
        t.e1(semi=3)
        t.add([0x62000000 | rgb(255, 255, 255), xy(296, 216), (12 << 16) | 20])
        t.e1()
    if 'sprites' in groups:
        # sprites: all sizes, each depth; raw and modulated
        t.e1(tx=tp4, depth=0)
        t.add([0x64000000 | rgb(128, 128, 128), xy(8, 6), 0 | (c4 << 16), (20 << 16) | 30])
        t.add([0x7C000000 | rgb(100, 200, 100), xy(44, 6), 8 | (3 << 8) | (c4 << 16)])
        t.add([0x75000000, xy(64, 6), 20 | (10 << 8) | (c4 << 16)])
        t.e1(tx=tp8, depth=1)
        t.add([0x64000000 | rgb(255, 255, 255), xy(84, 6), 0 | (c8 << 16), (24 << 16) | 40])
        t.add([0x7C000000 | rgb(200, 100, 100), xy(130, 6), 8 | (c8 << 16)])
        t.e1(tx=tp15, depth=2)
        t.add([0x65000000, xy(150, 6), 0, (30 << 16) | 40])
        t.add([0x7D000000, xy(200, 6), 4 | (4 << 8)])
        t.add([0x6C000000 | rgb(255, 255, 255), xy(222, 6), 3 | (3 << 8)])
        # sprite semi-transparency, four modes, on the 15-bit page
        for mode in range(4):
            t.e1(tx=tp15, depth=2, semi=mode)
            t.add([0x66000000, xy(230 + 20 * mode, 4), 8 | (8 << 8), (14 << 16) | 14])
    # flips, texture window, wrap
    if 'flips' in groups:
        t.e1(tx=tp15, depth=2, flipx=True)
        t.add([0x65000000, xy(8, 44), 31 | (2 << 8), (10 << 16) | 32])
        t.e1(tx=tp15, depth=2, flipy=True)
        t.add([0x65000000, xy(44, 44), 2 | (20 << 8), (12 << 16) | 10])
        t.e1(tx=tp15, depth=2, flipx=True, flipy=True)
        t.add([0x65000000, xy(60, 44), 30 | (30 << 8), (12 << 16) | 12])
    if 'window_wrap' in groups:
        t.e1(tx=tp15, depth=2)
        t.add([0x65000000, xy(76, 44), 250 | (250 << 8), (12 << 16) | 14])     # wraps in u and v
        t.env(0, 0, 319, 239, 0, 0, tpage(tp15, 2), window=(2 | (2 << 5) | (1 << 10) | (3 << 15)))
        t.add([0x65000000, xy(96, 44), 5 | (5 << 8), (24 << 16) | 24])        # texture window
        t.env(0, 0, 319, 239, 0, 0, tpage(tp15, 2), window=0)
    if 'clip' in groups:
        # --- clipping, offset and oversize
        t.env(100, 150, 180, 200, 0, 0)
        t.add([0x20000000 | rgb(255, 255, 0), xy(60, 120), xy(220, 140), xy(120, 230)])  # clipped by the area
        t.env(0, 0, 319, 239, 160, 120)
        t.add([0x20000000 | rgb(0, 255, 0), xy(-60, -40), xy(-20, -40), xy(-60, -10)])
        t.add([0x60000000 | rgb(255, 0, 0), xy(-8, -8), (6 << 16) | 6])
        t.env(0, 0, 319, 239, 0, 0)
        t.add([0x20000000 | rgb(255, 255, 255), xy(-600, 0), xy(600, 0), xy(0, 20)])     # 1200 wide: skipped
        t.add([0x20000000 | rgb(255, 255, 255), xy(0, -300), xy(20, 300), xy(0, 0)])     # 600 tall: skipped
        t.add([0x20000000 | rgb(255, 0, 0), xy(300, 100), xy(301, 100), xy(300, 101)])    # one pixel
        t.add([0x20000000 | rgb(255, 0, 0), xy(305, 100), xy(305, 100), xy(310, 105)])    # degenerate
    if 'mask' in groups:
        # --- mask bits: set while drawing, then check
        t.env(0, 0, 319, 239, 0, 0, mask=1)
        t.add([0x60000000 | rgb(255, 255, 255), xy(8, 70), (10 << 16) | 20])
        t.add([0x62000000 | rgb(255, 255, 255), xy(30, 70), (10 << 16) | 20])
        t.env(0, 0, 319, 239, 0, 0, mask=2)
        t.add([0x60000000 | rgb(255, 0, 0), xy(4, 66), (20 << 16) | 20])                    # protected where bit 15 is set
        t.env(0, 0, 319, 239, 0, 0, mask=3)
        t.add([0x60000000 | rgb(0, 255, 0), xy(50, 66), (10 << 16) | 10])
        t.env(0, 0, 319, 239, 0, 0, mask=0)
    if 'transfers' in groups:
        # --- transfers and fill inside the table
        t.add([0x80000000, xy(0, 0), xy(200, 100), (30 << 16) | 40])           # MoveImage, VRAM to VRAM
        t.add([0x02000000 | rgb(30, 90, 250), xy(240, 100), (12 << 16) | 33])  # fill: x/width rounding
        t.add([0x80000000, xy(190, 190), xy(190, 192), (30 << 16) | 20])       # overlapping move
        return t


def main(argv):
    base = int(argv[3], 16) if len(argv) > 3 else 0x8014E000
    groups = argv[4].split(',') if len(argv) > 4 else GROUPS
    unknown = [g for g in groups if g not in GROUPS]
    if unknown:
        raise ValueError('unknown groups %s; known: %s' % (unknown, ','.join(GROUPS)))
    data = build(groups).serialise(base)
    with open(argv[1], 'wb') as f:
        f.write(data)
    with open(argv[2], 'w') as f:
        f.write("INJECT = {addr = 0x%08X, hex = '%s'}\n" % (base, data.hex()))
    print('synthetic ordering table: %d bytes at %08X' % (len(data), base))


if __name__ == '__main__':
    main(sys.argv)
