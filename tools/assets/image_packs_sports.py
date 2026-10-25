"""Bounded original sports instruction viewports, from TAIIKU C (T-28002).

The MOD tile palette byte is not a CLUT binding: mod_trans_vram ignores it.
Scroll displays the reconstructed 8bpp VRAM at CLUT (0, 0x1f0), whose
unchanged mode-1 palette is the first 256 words of each source file.
"""
import hashlib
import re
import struct
from pathlib import Path
from .images import ImageFormatError, _color, _png

LOAD_BASE = 0x8018f000
PROFILES = {
    'KASETU': (0xf800, [(0xe000, 0xe008, 0xf218)], 'runner_race_dispatch.c'),
    'SPSETU': (0x13000, [(0x129cc, 0xe000, 0xf190)], 'runner_actor_display.c'),
    'TASETU': (0x13800, [(0x12000, 0x12008, 0x13198)], 'candidate_girl_scene.c'),
    'NINSETU': (0x27800, [(0x1e000, 0x1e008, 0x201b0), (0x1f018, 0x1f020, 0x239ec)], 'schoolyard_scene_dispatch.c'),
}


def _parse(data, source_path):
    name = Path(source_path).stem.upper()
    if name not in PROFILES or len(data) != PROFILES[name][0]:
        raise ImageFormatError('sports profile requires an exact original source name and size')
    cards = []
    for branch, (page_table, tile_table, screen_table) in enumerate(PROFILES[name][1]):
        mod = struct.unpack_from('<4I', data, tile_table)
        screen = struct.unpack_from('<5I', data, screen_table)
        if mod[:3] != (2, 0x04040808, 0x400) or not 1 < mod[3] <= 128:
            raise ImageFormatError('unsupported sports MOD header/depth/cell geometry')
        if screen[0:2] != (0x303, 0x04042020) or screen[2] not in (0x08080201, 0x08084001) or screen[3:] != (0x3000, 0xe0a):
            raise ImageFormatError('unsupported sports screen header/geometry')
        columns = ((screen[2] >> 8) & 255) * 8
        map_count = min(columns * 8, screen[4])
        map_end = screen_table + 20 + map_count * 4
        tile_end = tile_table + 16 + mod[3] * 16 * 4
        if map_end > len(data) or tile_end > screen_table:
            raise ImageFormatError('sports map or MOD frame extent outside source bounds')
        cells = struct.unpack_from('<%dI' % map_count, data, screen_table + 20)
        nonzero = [(i % columns, i // columns, value) for i, value in enumerate(cells) if value & 0xffff]
        if len(nonzero) != 80 or {(x, y) for x, y, _ in nonzero} != {(x, y) for y in range(8) for x in range(10)}:
            raise ImageFormatError('sports viewport includes unwritten or unexpected map cells')
        tiles = struct.unpack_from('<%dI' % (mod[3] * 16), data, tile_table + 16)
        if any((word >> 24) > 1 or word & 0xfc for word in tiles):
            raise ImageFormatError('unsupported sports source bank or glyph flip flags')
        pages = [p - LOAD_BASE for p in struct.unpack_from('<2I', data, page_table)]
        expected_pages = ([0xe000, 0x12000] if branch == 0 else [0x16000, 0x1a000]) if name == 'NINSETU' else [0x2000, 0x6000]
        if pages != expected_pages or any(p + 0x4000 > tile_table for p in pages):
            raise ImageFormatError('sports source texture pointers differ from original profile')
        parts = []
        for x, y, value in nonzero:
            frame = value & 0xffff
            if frame >= mod[3]:
                raise ImageFormatError('sports map frame outside MOD table')
            for tile in range(16):
                word = tiles[frame * 16 + tile]
                bank, glyph, flags = word >> 24, (word >> 16) & 255, word & 255
                parts.append({'map_cell': [x, y], 'map_word': value, 'frame_index': frame,
                              'source_map_offset': screen_table + 20 + (y * columns + x) * 4,
                              'source_record_offset': tile_table + 16 + (frame * 16 + tile) * 4,
                              'source_word': word, 'source_sheet_offset': pages[bank],
                              'u': (glyph % 16) * 8, 'v': (glyph // 16) * 8,
                              'x': x * 32 + (tile % 4) * 8, 'y': y * 32 + (tile // 4) * 8,
                              'width': 8, 'height': 8, 'flip_flags': flags,
                              'ignored_mod_palette_byte': (word >> 8) & 255})
        cards.append({'branch': branch, 'source_page_table_offset': page_table,
                      'source_tile_table_offset': tile_table, 'source_screen_table_offset': screen_table,
                      'screen_header_words': list(screen), 'mod_header_words': list(mod),
                      'stored_map_columns': columns, 'stored_map_words': map_count,
                      'source_map_extent': [screen_table, map_end], 'source_mod_extent': [tile_table, tile_end],
                      'source_pages': pages, 'parts': parts})
    return name, cards


def probe_sports_ui(data: bytes, source_path: str) -> dict:
    """Validate only the four original instruction-pack profiles."""
    try:
        name, cards = _parse(data, source_path)
    except ImageFormatError as error:
        return {'status': 'invalid', 'format': 'tokimemo-sports-ui', 'reason': str(error)}
    return {'status': 'supported', 'format': 'tokimemo-sports-ui',
            'metadata': {'profile': name, 'cards': len(cards), 'width': 320, 'height': 240}}


def export_sports_ui(data: bytes, exe, output_dir, asset_id: str, *, source_path: str) -> dict:
    """Export fully written initial Scroll viewports with their actual source CLUT."""
    name, cards = _parse(data, source_path)
    if not re.fullmatch(r'[A-Za-z0-9][A-Za-z0-9_.-]{0,159}', asset_id):
        raise ImageFormatError('unsafe sports asset_id')
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    palette = struct.unpack_from('<256H', data)
    images, files = [], []
    for card in cards:
        pixels = bytearray(320 * 240)
        for part in card['parts']:
            for y in range(8):
                target_y = part['y'] + y
                if target_y >= 240:
                    continue
                sy = 7 - y if part['flip_flags'] & 1 else y
                for x in range(8):
                    sx = 7 - x if part['flip_flags'] & 2 else x
                    offset = part['source_sheet_offset'] + (part['v'] + sy) * 128 + part['u'] + sx
                    pixels[target_y * 320 + part['x'] + x] = data[offset]
        words = [palette[index] for index in pixels]
        variant = 'instruction-card-%d' % card['branch']
        png, mask = '%s.%s.png' % (asset_id, variant), '%s.%s.stp.png' % (asset_id, variant)
        for filename, payload in ((png, _png(320, 240, b''.join(_color(w) for w in words), 4)),
                                  (mask, _png(320, 240, bytes(255 if w & 0x8000 else 0 for w in words), 1))):
            with (output_dir / filename).open('xb') as output:
                output.write(payload)
            files.append(filename)
        images.append(dict(card, png=png, stp_png=mask, variant=variant,
                           frame_index=card['branch'], preview_kind='static_frame_composite', width=320, height=240,
                           bits_per_pixel=8, palette_source_offset=0, source_palette_words=list(palette),
                           preview_palette_words=list(palette), clut_vram=[0, 0x1f0], runtime_palette_mode=1,
                           palette_status='established: source mode1 row0 upload and Scroll hardcoded CLUT',
                           source_region={'x': 0, 'y': 0, 'width': 320, 'height': 240},
                           runtime_viewport={'scroll': 0, 'width': 320, 'height': 240, 'wrap_width': 640},
                           cropped_source_rows={'y': 240, 'height': 16, 'width': 320}))
    return {'status': 'exported', 'format': 'tokimemo-sports-ui', 'files': files, 'images': images,
            'metadata': {'source_sha256': hashlib.sha256(data).hexdigest(), 'source_size': len(data),
                         'profile': name, 'source_encoding': '8bpp MOD glyph frames and sparse SRN map',
                         'alpha_policy': 'BGR555 zero word transparent; nonzero opaque',
                         'stp_policy': 'separate original palette bit15 mask; Scroll disables semitransparency',
                         'source_evidence': ['src/ovl/TAIIKU/' + PROFILES[name][2],
                                             'src/main/screen_layers.c:func_8004E44C/srn_init/srn_vram_set/mod_trans_vram/get_fnt',
                                             'src/main/scene_display.c:Scroll', 'src/main/frame_render_audio.c:palette_load_vram'],
                         'coverage_policy': 'initial 320x240 instruction viewport only; not whole pack or runtime screenshot',
                         'unrepresented': ['other source texture pixels and MOD frames not used by this viewport',
                                           'palette rows1..15', 'actors, overlays, fades and audio',
                                           'bottom16 rows of the reconstructed320x256 map'],
                         'hardware_raster_parity_verified': False, 'round_trip_verified': False}}
