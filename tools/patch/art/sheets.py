"""Sprite-sheet profiles of the containers that hold English art (T-39508).

Each profile turns one original container (and the executable, where its palettes or tables
live) into the views of tools/patch/art/indexed.py, with the same geometry and palettes as the
asset reader that exported the images the English art was drawn on (tools/assets/image_packs*.py;
their `_parse` functions are reused). A profile also says where its part records are and how a
texture id is encoded, which a repack needs; MAINROM's frame table is in the executable, so its
records are rewritten in the English executable.

View keys: `frame-NNN.<variant>` (variant `base` unless the reader exports others), `part-NNN-PP.
<variant>` for one part of a frame, `page-<group>-N` for a whole 128x128 page.
"""
from __future__ import annotations

import struct
from dataclasses import dataclass, field
from typing import Callable

from tools.assets import image_packs as mainrom
from tools.assets import image_packs_battle as battle
from tools.assets import image_packs_exam as exam
from tools.assets import image_packs_filesel as filesel
from tools.assets import image_packs_name as name_ent
from tools.assets import image_packs_option as option
from tools.assets import image_packs_sports as sports
from tools.assets import image_packs_title as title
from tools.patch.art.indexed import Part, View


@dataclass
class Profile:
    container: str
    views: dict = field(default_factory=dict)        # key -> View
    pages: dict = field(default_factory=dict)        # page index -> (offset, bpp), repackable pages
    align: int = 8
    encode: Callable | None = None                   # (page index, u, v) -> texture id; None: no repack
    exe: bytes | None = None                         # the records are in the executable (MAINROM)
    direct: bool = True                              # code may draw page texels without a frame
    page_palette: tuple = ()                         # the page's palette (region views)
    pack: Callable | None = None                     # (part, texture id) -> record bytes; None: 14-byte part
    page_sets: dict = field(default_factory=dict)    # view key -> pages its new parts may use (default all)
    prefer_rebuild: bool = False                     # label sheets: new parts per word, the whole frame freed
    grow: int = 0                                    # tile size when a frame may get a new, longer record

    def record_bytes(self, data: bytes) -> bytes:
        """The bytes that hold the frame and part records: the container, or the executable."""
        return self.exe if self.exe is not None else data

    def add(self, view: View) -> None:
        self.views[view.key] = view

    def frame(self, number: int, variant: str = "base") -> View | None:
        return self.views.get("frame-%03d.%s" % (number, variant))

    def part_view(self, number: int, part: int, variant: str = "base") -> View | None:
        frame = self.frame(number, variant)
        if frame is None or part >= len(frame.parts):
            return None
        return View.of("part-%03d-%02d.%s" % (number, part, variant), [frame.parts[part]], frame=number)


def _frame_views(profile: Profile, frames: list, variant: str, make_part) -> None:
    for frame in frames:
        parts = [make_part(p) for p in frame["parts"]]
        if parts:
            record = frame.get("source_frame_offset", frame.get("exe_frame_offset"))
            profile.add(View.of("frame-%03d.%s" % (frame["frame_index"], variant), parts,
                                frame=frame["frame_index"], record=record))


def _stp_forced(address: int) -> bool:
    """Boot sets STP on the nonzero colours of these two executable palette ranges."""
    return 0x800C6200 <= address < 0x800C6220 or 0x800C62C0 <= address < 0x800C6340


def _exe_palette(read, address: int) -> tuple:
    words = struct.unpack("<16H", read(address, 32))
    if _stp_forced(address):
        words = tuple(w | 0x8000 if w & 0x7FFF else w for w in words)
    return words


def mainrom_profile(pack: bytes, exe: bytes) -> Profile:
    """MAINROM.NBN: 4bpp sheets; frame records and palettes in the executable."""
    read, _base, frames, _sequences = mainrom._parse(pack, exe)
    # boot_hud_textures_load uploads sheets 0-3 and 7-10 only (4-6 are not in VRAM, so the
    # allocator never uses them: no frame draws them). The one direct sprite into these tpages
    # (hud_panels.c, the checker background) samples u 128-159, outside the sheets.
    # A frame may get a longer record in english_hud_frames (src/english/hud_frames.c): the sheets
    # have free 8x8 cells but no free 16x16 one, so a changed cell is redrawn from 8x8 tiles.
    profile = Profile("CDROM/FUKUTEST/MAINROM.NBN", align=8, direct=False, grow=8,
                      pages={i: (offset, 4) for i, offset in enumerate(mainrom.TEXTURE_OFFSETS)},
                      encode=lambda page, u, v: (page << 8) | ((v // 8) << 4) | (u // 8), exe=exe)
    for variant in ("base",) + tuple(mainrom.OVERRIDES):
        def make(p, variant=variant):
            if variant == "base":
                info = p["palette_info"]
                address = 0x800C6020 + ((info >> 8) & 15) * 512 + (info & 0xF0) * 2
            else:
                address = mainrom.OVERRIDES[variant][0]
            return Part(p["x"], p["y"], p["width"], p["height"], p["flip_flags"], p["sheet_source_offset"], 4,
                        p["u"], p["v"], _exe_palette(read, address), p["exe_record_offset"], p["sheet_index"],
                        p["palette_info"])
        _frame_views(profile, frames, variant, make)
    return profile


def mainrom_parts(record: bytes, exe: bytes, variant: str = "base") -> list:
    """The parts of a MAINROM frame record (count, origin, flags, 14-byte parts), as mainrom_profile
    builds them: for a frame moved to english_hud_frames."""
    read, _base = mainrom._reader(exe)
    parts = []
    for i in range(struct.unpack_from("<h", record)[0]):
        x, y, texture, info, flip, width, height = struct.unpack_from("<2h3H2h", record, 8 + 14 * i)
        sheet = texture >> 8
        if variant == "base":
            address = 0x800C6020 + ((info >> 8) & 15) * 512 + (info & 0xF0) * 2
        else:
            address = mainrom.OVERRIDES[variant][0]
        parts.append(Part(x, y, width, height, flip, mainrom.TEXTURE_OFFSETS[sheet], 4, (texture & 15) * 8,
                          ((texture & 255) >> 4) * 8, _exe_palette(read, address), None, sheet, info))
    return parts


def option_profile(data: bytes) -> Profile:
    """OPTION.NBN: four 8bpp actor pages, frame records and CLUTs in the file."""
    pages, frames, _extents, _sequence = option._parse(data)
    palettes = {}
    # the options overlay draws its pages only through frames (no direct sprite UVs in src/ovl/options)
    # the Japanese labels share kanji tiles; English labels are cut per word (REGISTER.md section 6
    # of T39410-menus-fixes-v1: the page has to be repacked)
    profile = Profile("CDROM/FUKUTEST/OPTION.NBN", align=8, direct=False, prefer_rebuild=True,
                      pages={p["page_index"]: (p["source_offset"], 8) for p in pages},
                      encode=lambda page, u, v: (page << 8) | ((v // 8) << 4) | (u // 8))

    def make(p):
        offset = p["palette_source_offset"]
        if offset not in palettes:
            palettes[offset] = struct.unpack_from("<256H", data, offset)
        return Part(p["x"], p["y"], p["width"], p["height"], p["flip_flags"], p["source_sheet_offset"], 8,
                    p["u"], p["v"], palettes[offset], p["source_record_offset"], p["page_index"], p["clut_info"])
    _frame_views(profile, [f for f in frames if all(p["palette_source_offset"] is not None for p in f["parts"])],
                 "base", make)
    return profile


def title_profile(data: bytes) -> Profile:
    """TITLE.NBN: seven 8bpp actor sheets, mode-2 CLUT rows 14 and 15."""
    frames, _sequence = title._parse(data)
    palettes = {row: title._palette(data, 0x4C200 + row * 512, 2)[1] for row in (14, 15)}
    # title_screen.c draws the actor sheets only through frames
    profile = Profile("CDROM/FUKUTEST/TITLE.NBN", align=8, direct=False,
                      pages={i: (offset, 8) for i, offset in enumerate(title.ACTOR_SHEETS)},
                      encode=lambda page, u, v: (page << 8) | ((v // 8) << 4) | (u // 8))

    def make(p):
        return Part(p["x"], p["y"], p["width"], p["height"], p["flip_flags"], p["sheet_source_offset"], 8,
                    p["u"], p["v"], palettes[p["palette_row_index"]], p["source_record_offset"], p["sheet_index"],
                    p["palette_info"])
    _frame_views(profile, frames, "base", make)
    return profile


def filesel_profile(data: bytes) -> Profile:
    """FILESEL.NBN: 8bpp actor sheets in 16-texel cells, base and selected CLUTs, info pages."""
    frames = filesel._parse(data)
    base = struct.unpack_from("<256H", data, 0x22000)
    selected = struct.unpack_from("<256H", data, 0x22200)
    profile = Profile("CDROM/FUKUTEST/FILESEL.NBN", align=16,
                      pages={i: (offset, 8) for i, offset in enumerate(filesel.ACTOR_SHEETS)},
                      encode=lambda page, u, v: (page << 6) | ((v // 16) << 3) | (u // 16))
    for variant, palette in (("base", base), ("selected", selected)):
        def make(p, palette=palette):
            return Part(p["x"], p["y"], p["width"], p["height"], p["flip_flags"], p["sheet_source_offset"], 8,
                        p["u"], p["v"], palette, p["source_record_offset"], p["sheet_index"], p["palette_info"])
        chosen = frames if variant == "base" else [f for f in frames if f["frame_index"] in filesel.HIGHLIGHT_FRAMES]
        _frame_views(profile, chosen, variant, make)
    for group, start, palette_offset, _clut_y, _slot in filesel.BACKGROUND_GROUPS:
        palette = struct.unpack_from("<256H", data, palette_offset)
        for index in range(6):
            page = Part(0, 0, 128, 128, 0, start + index * 0x4000, 8, 0, 0, palette)
            profile.add(View.of("page-%s-%d" % (group, index), [page]))
    return profile


def name_profile(data: bytes) -> Profile:
    """NAME_ENT.NBN: one 4bpp actor page, palette row 0x1F0 by column (mode 2)."""
    page, frames = name_ent._parse(data)
    _raw, actor = name_ent._mode2_palette(data, 0)
    profile = Profile("CDROM/NAME_ENT/NAME_ENT.NBN", align=8, pages={0: (page, 4)},
                      encode=lambda index, u, v: ((v // 8) << 4) | (u // 8))

    def make(p):
        column = p["clut_vram"][0]
        return Part(p["x"], p["y"], p["width"], p["height"], p["flip_flags"], page, 4, p["u"], p["v"],
                    tuple(actor[column:column + 16]), p["source_record_offset"], 0, p["palette_info"])
    _frame_views(profile, frames, "base", make)
    return profile


def exam_profile(data: bytes, exe: bytes) -> Profile:
    """EXAMINE.NBN: three 4bpp pages, frame records in the file, CLUTs from the executable."""
    pages, frames = exam._parse(data, exe)
    profile = Profile("CDROM/FUKUTEST/EXAMINE.NBN", align=8,
                      pages={i: (offset, 4) for i, offset in enumerate(pages)},
                      encode=lambda page, u, v: (page << 8) | ((v // 8) << 4) | (u // 8))

    def make(p):
        return Part(p["x"], p["y"], p["width"], p["height"], p["flip_flags"], p["sheet_source_offset"], 4,
                    p["u"], p["v"], tuple(p["preview_palette_words"]), p["source_record_offset"], p["sheet_index"],
                    p["palette_info"])
    _frame_views(profile, frames, "base", make)
    return profile


def battle_profile(data: bytes, path: str) -> Profile:
    """RPG_BAT encounter BIN: one 8bpp page; the English art is page regions (command labels).

    The images are crops of the page as the reader exports its regions; every frame that draws
    those texels shows the English, which is the point. Region views are written in place.
    """
    parsed = battle._parse(data, path)
    page = parsed["page_offset"]
    profile = Profile(path, align=8, pages={0: (page, 8)}, direct=True)
    profile.page_palette = tuple(parsed["palette"])
    return profile


def page_region(profile: Profile, page_index: int, region) -> View:
    """A rectangle of one 8bpp page drawn with the page palette (no frame)."""
    x, y, w, h = region
    page, _bpp = profile.pages[page_index]
    part = Part(x, y, w, h, 0, page, 8, x, y, profile.page_palette, None, page_index)
    return View.of("region-%d-%d-%d" % (page_index, x, y), [part])


ALLC_PALETTE = 0x18000
ALLC_BANNER = (74, 38)      # page rows of the banner strip on pages 3, 4 and 5 (texture rows 202-239)


def allc_profile(data: bytes, path: str) -> Profile:
    """LASTPHOT/ALL_C.NBN: six 8bpp photo pages (3 x 2) and one palette; the banner is page regions.

    Same layout as tools/translation_bs_drafts.py page_texture; the English strip uses only
    indices the original strip uses (tools/translation_menus_fixes.py allc_container).
    """
    if len(data) < ALLC_PALETTE + 512:
        raise ValueError("%s is too short for the ALL_C profile" % path)
    profile = Profile(path, align=8, pages={i: (i * 0x4000, 8) for i in range(6)}, direct=True)
    profile.page_palette = struct.unpack_from("<256H", data, ALLC_PALETTE)
    return profile


def sports_profile(data: bytes, path: str) -> Profile:
    """TAIIKU instruction cards (*SETU.BIN): 320x240 cards of 8x8 tiles from two 8bpp pages.

    Tiles are written in place where their texels are not shared; otherwise a tile gets a fresh
    8x8 glyph cell and its tile word is rewritten. A tile word that two places use with different
    English cannot be split.
    """
    _name, cards = sports._parse(data, path)
    palette = struct.unpack_from("<256H", data)
    offsets = sorted({offset for card in cards for offset in card["source_pages"]})
    pages = {i: (offset, 8) for i, offset in enumerate(offsets)}
    bank = {}       # page index -> bank number in its card's page table
    for card in cards:
        for number, offset in enumerate(card["source_pages"]):
            bank[offsets.index(offset)] = number
    # a tile record is one word: bank << 24 | glyph << 16 | (ignored palette byte) << 8 | flip
    profile = Profile(path, align=8, direct=True, pages=pages,
                      encode=lambda page, u, v: (bank[page] << 8) | ((v // 8) * 16 + u // 8),
                      pack=lambda part, glyph: struct.pack("<I", ((glyph >> 8) << 24) | ((glyph & 0xFF) << 16)
                                                           | (part.clut & 0xFF00) | part.flip))
    for card in cards:
        profile.page_sets["card-%d" % card["branch"]] = {offsets.index(o) for o in card["source_pages"]}
        parts = [Part(p["x"], p["y"], 8, 8, p["flip_flags"], p["source_sheet_offset"], 8, p["u"], p["v"], palette,
                      p["source_record_offset"], offsets.index(p["source_sheet_offset"]), p["source_word"])
                 for p in card["parts"] if p["y"] < 240]
        view = View("card-%d" % card["branch"], parts, 0, 0, 320, 240, card["branch"])
        profile.add(view)
    return profile
