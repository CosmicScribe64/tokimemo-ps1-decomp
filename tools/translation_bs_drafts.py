#!/usr/bin/env python3
"""T-39012: English lettering drafts for BS still backgrounds (agent drafts only).

`census` decodes every BS file named by the image catalog rows plus extra
corpus files and prints hash-bound facts. Files without caller-proved geometry
are decoded at the macroblock-count shape for pixel inspection only; that shape
is never used for a draft.

`build` makes drafts only for files with caller-proved geometry:

1. decode the original BS with the pinned fixed-point model;
2. erase the Japanese lettering inside a declared erase rectangle, filling each
   lettering pixel from the nearest non-lettering source pixel in the same row
   (every donor coordinate is recorded);
3. draw English with unscaled glyphs of the authenticated game font, in an ink
   colour copied from a recorded source pixel of the original lettering;
4. re-encode only the macroblocks touching those rectangles (tools/assets/mdec_splice.py)
   and keep every other macroblock's bits verbatim;
5. gate: the spliced stream parses with the project reader, pixels outside the
   edited macroblocks are bit-identical, ring pixels (edited macroblocks outside
   the rectangles) stay within +-1 RGB555 step, the file fits its sector budget,
   and a float-IDCT second decode stays within +-2 steps of the fixed-point decode.

`sweep` runs the P_01_02 class-header feasibility sweep (P01_02_SWEEP: every
lambda x ring weight) through the same splice and gates and prints the facts;
it writes no draft.

The decoded spliced stream is the English replacement payload. Full decoded
images and the spliced .BS stay in .asset-cache; `publish` copies only the
edited-macroblock crops and the manifest. Outputs are unreviewed drafts and
never claim human approval.
"""
import argparse
import json
from pathlib import Path
import struct
import sys

import numpy as np
from PIL import Image

from translation_assets import ROOT, digest, encoded, new_output
from translation_native_font import NativeLatin
sys.path.insert(0, str(ROOT))
from tools.assets import image_mdec_model as mdec_model
from tools.assets import image_packs_photos as photos
from tools.assets import mdec_splice
from tools.assets.input import open_input

SCHEMA = 'tokimemo.bs-translation-drafts.v1'
BS_DIR = 'disc/files/CDROM/BS'
RING_TOLERANCE = 1
FLOAT_TOLERANCE = 2
CENSUS_FILES = ('P_01_02', 'P_01_03', 'P_01_03B', 'C_01_28', 'D_01_47', 'D_01_48', 'D_01_67', 'D_01_69',
                'D_01_70', 'D_03_06', 'D_03_08', 'D_03_09', 'D_11_11', 'D_15_03', 'D_19_02', 'D_29_09',
                'I_17_01', 'I_28_02', 'I_11_15', 'D_11_10', 'D_11_12', 'D_29_08')
CENSUS_SHAPES = {200: (320, 160), 400: (640, 160), 120: (320, 88)}
# Caller-proved geometry (T-39004 profiles); anything else stays unresolved.
GEOMETRY = {
    'P_01_02': {'width': 640, 'height': 160, 'read_sectors': 9, 'evidence': [
        'src/ovl/name_entry/prologue.c:prologue_board_load cd_read_sectors(9,0x80162000,0x4954), bg_still_decode_to_vram(320,0,640,160)',
        'tools/port/first_arrival_board_content.py board read/decoder pin']},
    'P_01_03': {'width': 320, 'height': 160, 'read_sectors': 10, 'evidence': [
        'src/ovl/name_entry/prologue.c:prologue_board_scroll dec_bg_cd_read(0x495D,1), dec_bg_decode_to_vram(1)',
        'src/main/background_images.c:dec_bg_decode_to_vram bg_still_decode_to_vram(704,0,320,160)']},
    'P_01_03B': {'width': 320, 'height': 160, 'read_sectors': 10, 'evidence': [
        'src/ovl/name_entry/prologue.c:prologue_names_crossfade dec_bg_cd_read(0x4966,1), dec_bg_decode_to_vram(1)',
        'src/main/background_images.c:dec_bg_decode_to_vram bg_still_decode_to_vram(320,256,320,160)']},
    'C_01_28': {'width': 320, 'height': 88, 'read_sectors': 10, 'evidence': [
        'src/main/school_day.c:school_day_background_read seasonal branch; src/main/hud_panels.c:hud_season_index',
        'src/main/background_images.c:dec_bg_cd_read cd_read_sectors_nowait(0xA,0x80162000,lba), bg_still_decode_to_vram(..,0x140,0x58)',
        'tools/assets/bs_cohort_profile.py C_01_28.BS lba 0x3F0D file_sectors 10']},
}
PLANS = (
    {'id': 'bs-c-01-28-final-exams', 'file': 'C_01_28', 'catalog_item': 'catalog-147',
     'sha256': 'd7748c6a565f3f3bf8c7c0e39a90a701da9caf4fbaef8f93ad214db313d2daed',
     'erase': [270, 1, 26, 31], 'lettering': [256, 2, 48, 28], 'lines': ['FINAL', 'EXAMS'],
     'line_gap': 1, 'chalk_threshold': 5, 'lambda': 2.0, 'ring_weights': [50.0, 200.0, 1000.0]},
)

# P_01_02 class headers read vertically 一年X組 (X is already Latin). Feasibility sweep only:
# 一 -> '1', 年 -> '-', 組 erased, so each header reads 1-A / 1-B / 1-C top to bottom. The
# column C header starts above the frame, so only its 年 and 組 are visible. Rects are
# measured on the fixed-point decode (stroke sum < 55) with a one-pixel margin.
P01_02_SWEEP = {
    'id': 'bs-p-01-02-class-headers', 'file': 'P_01_02', 'catalog_item': 'catalog-130',
    'sha256': '61b0cd543a73c64ff9ad6d1e77d48217c02dbfbe7dab4941e90451b76f20f398', 'threshold': 20,
    'columns': (
        {'letter': 'C', 'erase': ([181, 0, 19, 14], [181, 39, 19, 19]), 'draw': (([181, 0, 19, 14], '-'),)},
        {'letter': 'B', 'erase': ([375, 14, 16, 7], [371, 23, 17, 20], [372, 67, 17, 17]),
         'draw': (([374, 11, 18, 12], '1'), ([371, 23, 17, 20], '-'))},
        {'letter': 'A', 'erase': ([532, 45, 16, 6], [532, 52, 18, 20], [531, 89, 19, 16]),
         'draw': (([532, 42, 16, 11], '1'), ([532, 52, 18, 20], '-'))},
    ),
    'lambdas': (8.0, 12.0, 16.0, 24.0, 32.0, 50.0), 'ring_weights': (50.0, 200.0, 1000.0)}


# LASTPHOT ALL_C.NBN: 6 raw 8bpp 128x128 pages (3x2 texture), palette at 0x18000.
ALL_CLEAR = {'id': 'all-c-banner', 'catalog_item': 'catalog-127', 'path': 'CDROM/LASTPHOT/ALL_C.NBN',
             'sha256': 'e5a8440f4babcd56bea233e95a610cd70f14ab5ec1c959df9eebf3f955c26b52',
             'strip': [0, 202, 320, 38], 'paper_index': 6,
             # (text, palette index copied from the original lettering run it replaces)
             'runs': [('ALL CLEAR ', 67), ('CONGRATULATIONS', 36), ('!', 146)]}


def rgb8(rgb5):
    return ((rgb5 << 3) | (rgb5 >> 2)).astype('uint8')


def lettering_mask(rgb5, rect, threshold):
    """Pixels inside rect whose channel sum differs from the rect's median by more than threshold."""
    x, y, w, h = rect
    total = rgb5[y:y + h, x:x + w].sum(-1)
    mask = np.zeros(rgb5.shape[:2], bool)
    mask[y:y + h, x:x + w] = np.abs(total - np.median(total)) > threshold
    grown = mask.copy()  # one-pixel halo left by MDEC blur
    grown[1:] |= mask[:-1]; grown[:-1] |= mask[1:]; grown[:, 1:] |= mask[:, :-1]; grown[:, :-1] |= mask[:, 1:]
    region = np.zeros_like(mask)
    region[y:y + h, x:x + w] = True
    return mask, grown & region


def erase(rgb5, rect, threshold):
    """Fill lettering pixels from the nearest clean pixel in the same row; returns (image, donors, ink_pixel)."""
    strokes, removed = lettering_mask(rgb5, rect, threshold)
    if not strokes.any():
        raise ValueError('no lettering found in erase rectangle')
    result = rgb5.copy()
    donors = []
    for y, x in zip(*np.nonzero(removed)):
        for distance in range(1, rgb5.shape[1]):
            for dx in (-distance, distance):
                sx = x + dx
                if 0 <= sx < rgb5.shape[1] and not removed[y, sx]:
                    break
            else:
                continue
            break
        else:
            raise ValueError('no donor pixel in row %d' % y)
        result[y, x] = rgb5[y, sx]
        donors.append([int(x), int(y), int(sx), int(y)])
    # Ink: the stroke pixel at the 90th brightness percentile (ties broken by position),
    # i.e. a stroke core rather than an MDEC-blurred edge.
    points = sorted((int(rgb5[y, x].sum()), int(y), int(x)) for y, x in zip(*np.nonzero(strokes)))
    _, iy, ix = points[len(points) * 9 // 10]
    return result, donors, {'xy': [ix, iy], 'rgb555': [int(v) for v in rgb5[iy, ix]]}


def draw_lines(rgb5, rect, lines, gap, font, ink):
    """Draw centred lines of unscaled native glyphs; fails instead of clipping."""
    x, y, w, h = rect
    rows = []
    for text in lines:
        glyphs = [font.glyph(character) for character in text]
        top = min(py for glyph in glyphs for _, py in glyph['points'])
        bottom = max(py for glyph in glyphs for _, py in glyph['points'])
        width = sum(glyph['width'] for glyph in glyphs) + len(glyphs) - 1
        if width > w:
            raise ValueError('line does not fit unscaled: ' + text)
        rows.append((text, glyphs, top, bottom, width))
    height = sum(bottom - top + 1 for _, _, top, bottom, _ in rows) + gap * (len(rows) - 1)
    if height > h:
        raise ValueError('lines do not fit unscaled')
    result = rgb5.copy()
    cursor_y = y + (h - height) // 2
    placed = []
    for text, glyphs, top, bottom, width in rows:
        cursor_x = x + (w - width) // 2
        for glyph in glyphs:
            for px, py in glyph['points']:
                result[cursor_y + py - top, cursor_x + px] = ink
            cursor_x += glyph['width'] + 1
        placed.append({'text': text, 'origin': [x + (w - width) // 2, cursor_y], 'width': width, 'height': bottom - top + 1})
        cursor_y += bottom - top + 1 + gap
    return result, placed


def edited_macroblocks(rects, padded_height):
    return sorted({mdec_splice.macroblock_index(px, py, padded_height)
                   for x, y, w, h in rects for px in range(x, x + w) for py in range(y, y + h)})


def splice_target(data, source, goal, rects, width, height, lam, ring_weight):
    """Re-encode the macroblocks touched by rects towards goal; returns (bytes, facts, decoded)."""
    padded = (height + 15) // 16 * 16
    parsed = mdec_splice.parse(data)
    quant = parsed['quantization']
    inside = np.zeros(goal.shape[:2], bool)
    for x, y, w, h in rects:
        inside[y:y + h, x:x + w] = True
    basis = mdec_splice._linear_basis(quant)
    replacements = {}
    for index in edited_macroblocks(rects, padded):
        mx, my = index // (padded // 16) * 16, index % (padded // 16) * 16
        weights = np.where(inside[my:my + 16, mx:mx + 16], 1.0, ring_weight)
        replacements[index] = mdec_splice.encode_macroblock(parsed['macroblocks'][index]['levels'],
                                                            goal[my:my + 16, mx:mx + 16], weights, quant, lam, basis=basis)
    result, facts = mdec_splice.rebuild(data, replacements)
    decoded = mdec_splice.decode_rgb5(result, width, height)
    edited = np.zeros(goal.shape[:2], bool)
    float_error = 0
    for index in replacements:
        mx, my = index // (padded // 16) * 16, index % (padded // 16) * 16
        edited[my:my + 16, mx:mx + 16] = True
        second = mdec_splice.float_macroblock_rgb5(replacements[index], quant)
        float_error = max(float_error, int(np.abs(second - decoded[my:my + 16, mx:mx + 16]).max()))
    ring = edited & ~inside
    ring_error = np.abs(decoded - source)[ring]
    rect_error = np.abs(decoded - goal)[inside]
    facts.update({
        'quantization': quant, 'version': parsed['version'], 'lambda': lam, 'ring_weight': ring_weight,
        'edited_macroblocks': sorted(replacements),
        'outside_edited_macroblocks_identical': bool((decoded[~edited] == source[~edited]).all()),
        'ring': {'pixels': int(ring.sum()), 'max_abs_rgb555': int(ring_error.max()) if ring.any() else 0,
                 'exact_fraction': round(float((ring_error == 0).all(-1).mean()), 4) if ring.any() else 1.0},
        'lettering_error': {'max_abs_rgb555': int(rect_error.max()), 'mean_abs_rgb555': round(float(rect_error.mean()), 4),
                            'within_one_fraction': round(float((rect_error <= 1).all(-1).mean()), 4)},
        'float_idct_max_abs_rgb555': float_error})
    return result, facts, decoded


def gates(facts, read_sectors):
    failures = []
    if not facts['outside_edited_macroblocks_identical']:
        failures.append('pixels changed outside edited macroblocks')
    if facts['ring']['max_abs_rgb555'] > RING_TOLERANCE:
        failures.append('ring error above tolerance')
    if facts['float_idct_max_abs_rgb555'] > FLOAT_TOLERANCE:
        failures.append('float IDCT cross-check differs')
    if not facts['fits_original_sectors'] or facts['file_bytes']['new'] > read_sectors * 2048:
        failures.append('stream exceeds sector budget or caller read')
    return failures


def bounding(indices, padded_height):
    xs = [index // (padded_height // 16) * 16 for index in indices]
    ys = [index % (padded_height // 16) * 16 for index in indices]
    return [min(xs), min(ys), max(xs) + 16 - min(xs), max(ys) + 16 - min(ys)]


def build(output, disc=None, track=None):
    disc = Path(disc or ROOT / BS_DIR)
    output = new_output(output)
    font = NativeLatin(ROOT / 'disc/files/SLPM_86.053')
    assets, files = [], {}
    for plan in PLANS:
        data = (disc / (plan['file'] + '.BS')).read_bytes()
        if digest(data) != plan['sha256']:
            raise ValueError('source BS differs: ' + plan['file'])
        geometry = GEOMETRY[plan['file']]
        width, height = geometry['width'], geometry['height']
        source = mdec_splice.decode_rgb5(data, width, height)
        cleaned, donors, ink = erase(source, plan['erase'], plan['chalk_threshold'])
        goal, placed = draw_lines(cleaned, plan['lettering'], plan['lines'], plan['line_gap'], font, ink['rgb555'])
        rects = [plan['erase'], plan['lettering']]
        attempts = []
        for ring_weight in plan['ring_weights']:
            spliced, facts, decoded = splice_target(data, source, goal, rects, width, height, plan['lambda'], ring_weight)
            failures = gates(facts, geometry['read_sectors'])
            attempts.append({'ring_weight': ring_weight, 'failures': failures, 'ring_max': facts['ring']['max_abs_rgb555']})
            if not failures:
                break
        padded = source.shape[0]
        crop = bounding(facts['edited_macroblocks'], padded)
        name = plan['file']
        row = {'id': plan['id'], 'catalog_item': plan['catalog_item'], 'source_container': 'CDROM/BS/%s.BS' % name,
               'source_container_sha256': plan['sha256'], 'frame': 0, 'palette_id': 'mdec:' + mdec_model.MODEL_ID,
               'geometry': geometry, 'decoder_model': mdec_model.MODEL_ID,
               'source_words_sha256': digest(mdec_model.decode_padded_words(data, width, height)),
               'english_words_sha256': digest(mdec_model.decode_padded_words(spliced, width, height)),
               'spliced_bs_sha256': digest(spliced), 'erase_region': plan['erase'], 'lettering_region': plan['lettering'],
               'edit_regions': [crop], 'edit_region_policy': 'edited 16x16 macroblocks; every pixel outside is bit-identical to the source decode',
               'lines': placed, 'ink': ink, 'fill_donors': donors, 'chalk_threshold': plan['chalk_threshold'],
               'splice': facts, 'attempts': attempts, 'gates_passed': not failures, 'gate_failures': failures,
               'style': 'unreviewed', 'human_approved': False, 'runtime_integrated': False, 'in_game_verified': False,
               'crop': {'rect': crop, 'png': name + '.en.edited-macroblocks.png'},
               'local_files': {'spliced_bs': name + '.en.BS', 'source_png': name + '.source.png', 'english_png': name + '.en.png'}}
        files[name + '.en.BS'] = spliced
        files[name + '.source.png'] = rgb8(source[:height])
        files[name + '.en.png'] = rgb8(decoded[:height])
        x, y, w, h = crop
        files[row['crop']['png']] = rgb8(decoded[y:y + h, x:x + w])
        assets.append(row)
    output.mkdir(parents=True)
    indexed = [build_all_clear(output, Path(track or ROOT / 'disc/track1.bin'), font)]
    for filename, content in files.items():
        path = output / filename
        if isinstance(content, bytes):
            path.write_bytes(content)
        else:
            Image.fromarray(content, 'RGB').save(path, optimize=False)
    for row in assets:
        row['crop']['sha256'] = digest((output / row['crop']['png']).read_bytes())
        row['english_png_sha256'] = digest((output / row['local_files']['english_png']).read_bytes())
        row['source_png_sha256'] = digest((output / row['local_files']['source_png']).read_bytes())
    manifest = {'schema': SCHEMA, 'ticket': 'T-39012', 'human_approved': False, 'assets': assets, 'indexed_assets': indexed,
                'ring_tolerance_rgb555': RING_TOLERANCE, 'float_tolerance_rgb555': FLOAT_TOLERANCE,
                'font_provenance': font.provenance(),
                'committed_files': 'edited-macroblock crops and this manifest only; full decodes and spliced BS rebuild into .asset-cache'}
    (output / 'manifest.json').write_bytes(encoded(manifest) + b'\n')
    return {'output': str(output), 'drafts': sum(row['gates_passed'] for row in assets + indexed),
            'failed': [row['id'] for row in assets if not row['gates_passed']]}


def page_texture(data):
    """3x2 page grid of 8bpp indices as a 256x384 array (texture v = screen y)."""
    texture = np.zeros((256, 384), np.uint8)
    for page in range(6):
        row, column = divmod(page, 3)
        texture[row * 128:row * 128 + 128, column * 128:column * 128 + 128] = np.frombuffer(
            data, np.uint8, 0x4000, page * 0x4000).reshape(128, 128)
    return texture


def texture_bytes(data, texture):
    result = bytearray(data)
    for page in range(6):
        row, column = divmod(page, 3)
        result[page * 0x4000:page * 0x4000 + 0x4000] = texture[row * 128:row * 128 + 128, column * 128:column * 128 + 128].tobytes()
    return bytes(result)


def letter_indexed(texture, strip, paper, runs, font):
    """Replace every non-paper index in strip by paper, then draw runs of unscaled glyphs with existing indices."""
    x, y, w, h = strip
    result = texture.copy()
    area = result[y:y + h, x:x + w]
    rows = np.nonzero((area != paper).any(1))[0]
    if not rows.size:
        raise ValueError('no original lettering in strip')
    box_top, box_height = y + int(rows[0]), int(rows[-1] - rows[0] + 1)
    erased = int((area != paper).sum())
    area[area != paper] = paper
    glyphs = [(font.glyph(character), index) for text, index in runs for character in text]
    width = sum(glyph['width'] for glyph, _ in glyphs) + len(glyphs) - 1
    top = min(py for glyph, _ in glyphs for _, py in glyph['points'])
    bottom = max(py for glyph, _ in glyphs for _, py in glyph['points'])
    if width > w or bottom - top + 1 > h:
        raise ValueError('banner text does not fit unscaled')
    # Centre vertically on the rows the original lettering occupied, horizontally on the strip.
    cursor = x + (w - width) // 2
    baseline = min(max(box_top + (box_height - (bottom - top + 1)) // 2, y), y + h - (bottom - top + 1)) - top
    for glyph, index in glyphs:
        for px, py in glyph['points']:
            result[baseline + py, cursor + px] = index
        cursor += glyph['width'] + 1
    return result, {'erased_pixels': erased, 'original_lettering_rows': [box_top, box_height], 'text_origin': [x + (w - width) // 2, baseline + top],
                    'text_size': [width, bottom - top + 1]}


def indexed_failures(data, english, texture, lettered, strip, palette, runs):
    """Gate failures for an indexed page draft (empty list when every check passes)."""
    failures = []
    if any(not palette[index] & 0x7FFF for _, index in runs):
        failures.append('ink index is transparent')
    x, y, w, h = strip
    outside = np.ones(texture.shape, bool)
    outside[y:y + h, x:x + w] = False
    if (lettered[outside] != texture[outside]).any():
        failures.append('pixels changed outside the strip')
    if not set(np.unique(lettered[y:y + h, x:x + w])) <= set(np.unique(texture[y:y + h, x:x + w])):
        failures.append('palette index absent from the source strip')
    if len(english) != len(data) or english[0x18000:] != data[0x18000:]:
        failures.append('byte extent or palette changed')
    return failures


def build_all_clear(output, track, font):
    """Indexed ALL_C banner draft through the original caller-bound photo-page profile."""
    with open_input(str(track)) as source:
        entries = {entry['path']: entry for entry in source.entries()}
        data = source.read(entries[ALL_CLEAR['path']])
        overlay = source.read(entries['CDROM/EXEDIR/ETC.EXN'])
    if digest(data) != ALL_CLEAR['sha256']:
        raise ValueError('ALL_C source differs')
    exported = photos.export_photos(data, None, output / 'all-c-source', 'all-c', source_entry=entries[ALL_CLEAR['path']],
                                    overlay_data=overlay, overlay_entry=entries['CDROM/EXEDIR/ETC.EXN'])
    palette = struct.unpack_from('<256H', data, exported['metadata']['palette_source_offset'])
    texture = page_texture(data)
    lettered, facts = letter_indexed(texture, ALL_CLEAR['strip'], ALL_CLEAR['paper_index'], ALL_CLEAR['runs'], font)
    english = texture_bytes(data, lettered)
    failures = indexed_failures(data, english, texture, lettered, ALL_CLEAR['strip'], palette, ALL_CLEAR['runs'])
    if failures:
        raise ValueError('ALL_C draft rejected: ' + '; '.join(failures))
    x, y, w, h = ALL_CLEAR['strip']
    (output / 'ALL_C.en.NBN').write_bytes(english)
    words = np.array([palette[i] for i in range(256)], np.uint16)
    def image(indices):
        rgb = np.stack([(words[indices] >> shift) & 31 for shift in (0, 5, 10)], -1).astype(np.int64)
        return rgb8(rgb)
    crops = []
    for page in (3, 4, 5):
        column = page - 3
        left, right = max(x, column * 128), min(x + w, column * 128 + 128)
        if left >= right:
            continue
        rect = [left - column * 128, y - 128, right - left, h]
        name = 'all-c.source-page-%d.en.edit-region.png' % page
        Image.fromarray(image(lettered[y:y + h, left:right]), 'RGB').save(output / name)
        crops.append({'page': page, 'page_rect': rect, 'png': name, 'sha256': digest((output / name).read_bytes())})
    return {'id': ALL_CLEAR['id'], 'catalog_item': ALL_CLEAR['catalog_item'], 'source_container': ALL_CLEAR['path'],
            'source_container_sha256': ALL_CLEAR['sha256'], 'english_container_sha256': digest(english),
            'source_pages': [{'variant': row['variant'], 'source_png_sha256': digest((output / 'all-c-source' / row['png']).read_bytes())}
                             for row in exported['images']],
            'palette_id': 'source-offset-0x18000', 'strip_texture_rect': ALL_CLEAR['strip'], 'paper_index': ALL_CLEAR['paper_index'],
            'runs': [{'text': text, 'palette_index': index, 'rgb555': palette[index] & 0x7FFF} for text, index in ALL_CLEAR['runs']],
            'lettering': facts, 'edit_regions': crops, 'gates_passed': True,
            'checks': ['ink indices opaque', 'indices outside strip unchanged', 'only indices already in the source strip',
                       'palette and trailer bytes unchanged', 'file size unchanged'],
            'runtime_composition_verified': False, 'style': 'unreviewed', 'human_approved': False,
            'runtime_integrated': False, 'in_game_verified': False}


def publish(built, destination):
    """Copy crops and manifest (no full game images, no BS bytes) into a new repository directory.

    Every crop hash is checked before anything is written; the checkpoint is
    assembled in a temporary sibling and renamed into place, so a failed publish
    leaves nothing behind.
    """
    manifest_bytes = (built / 'manifest.json').read_bytes()
    manifest = json.loads(manifest_bytes)
    destination = destination.absolute()
    if destination.exists():
        raise ValueError('destination must be new')
    crops, ledger = {}, []
    for row in manifest['assets']:
        if not row['gates_passed']:
            continue
        crops[row['crop']['png']] = (row['id'], row['crop']['sha256'])
        ledger.append({'id': row['id'], 'catalog_item': row['catalog_item'], 'path': 'bs-v1/' + row['crop']['png'],
                       'crop_sha256': row['crop']['sha256'], 'source_container_sha256': row['source_container_sha256'],
                       'english_words_sha256': row['english_words_sha256'], 'spliced_bs_sha256': row['spliced_bs_sha256'],
                       'manifest_sha256': digest(manifest_bytes), 'status': 'draft', 'human_approved': False})
    for row in manifest['indexed_assets']:
        for crop in row['edit_regions']:
            crops[crop['png']] = (row['id'], crop['sha256'])
        ledger.append({'id': row['id'], 'catalog_item': row['catalog_item'],
                       'paths': ['bs-v1/' + crop['png'] for crop in row['edit_regions']],
                       'source_container_sha256': row['source_container_sha256'],
                       'english_container_sha256': row['english_container_sha256'],
                       'manifest_sha256': digest(manifest_bytes), 'status': 'draft', 'human_approved': False})
    contents = {}
    for name, (asset, expected) in crops.items():
        contents[name] = (built / name).read_bytes()
        if digest(contents[name]) != expected:
            raise ValueError('built crop changed: ' + asset)
    staging = destination.with_name(destination.name + '.partial')
    if staging.exists():
        raise ValueError('stale staging directory: ' + str(staging))
    (staging / 'bs-v1').mkdir(parents=True)
    for name, data in contents.items():
        (staging / 'bs-v1' / name).write_bytes(data)
    (staging / 'bs-v1' / 'manifest.json').write_bytes(manifest_bytes)
    (staging / 'ledger.json').write_bytes(encoded({
        'schema': 'tokimemo.translation-checkpoint.v1', 'ticket': 'T-39012', 'assets': ledger,
        'candidate_assets': len(ledger), 'original_images_included': False,
        'full_images_policy': 'only edited-macroblock crops are committed; run build to rebuild full images into .asset-cache',
        'complete_translation_inventory': False, 'separate_reviewed_revisions': 0}) + b'\n')
    staging.rename(destination)
    return {'destination': str(destination), 'assets': len(ledger)}


def compose_columns(source, plan, font):
    """Erase and redraw every P_01_02 header column; returns (goal, rects, per-column records)."""
    goal, rects, records = source.copy(), [], []
    for column in plan['columns']:
        inks, donors = [], 0
        for rect in column['erase']:
            goal, filled, ink = erase(goal, rect, plan['threshold'])
            inks.append(ink)
            donors += len(filled)
            if list(rect) not in rects:
                rects.append(list(rect))
        placed = []
        for rect, text in column['draw']:
            goal, lines = draw_lines(goal, rect, [text], 0, font, inks[0]['rgb555'])
            placed += lines
            if list(rect) not in rects:
                rects.append(list(rect))
        records.append({'letter': column['letter'], 'ink': inks[0], 'fill_donor_count': donors, 'lines': placed})
    return goal, rects, records


def sweep(disc=None, font=None):
    """Lambda x ring-weight feasibility sweep for the P_01_02 class headers (no draft is written)."""
    plan = P01_02_SWEEP
    data = (Path(disc or ROOT / BS_DIR) / (plan['file'] + '.BS')).read_bytes()
    if digest(data) != plan['sha256']:
        raise ValueError('source BS differs: ' + plan['file'])
    font = font or NativeLatin(ROOT / 'disc/files/SLPM_86.053')
    geometry = GEOMETRY[plan['file']]
    width, height = geometry['width'], geometry['height']
    source = mdec_splice.decode_rgb5(data, width, height)
    goal, rects, columns = compose_columns(source, plan, font)
    parsed = mdec_splice.parse(data)
    trailing = parsed['trailing']
    zero_run = next((offset for offset, value in enumerate(trailing) if value), len(trailing))
    attempts = []
    for lam in plan['lambdas']:
        for ring_weight in plan['ring_weights']:
            row = {'lambda': lam, 'ring_weight': ring_weight}
            try:
                _, facts, _ = splice_target(data, source, goal, rects, width, height, lam, ring_weight)
            except mdec_splice.SpliceError as error:
                row['error'] = str(error)
            else:
                row.update({'stream_bytes': facts['stream_bytes']['new'], 'ring_max': facts['ring']['max_abs_rgb555'],
                            'float_idct_max': facts['float_idct_max_abs_rgb555'],
                            'lettering_mean_abs': facts['lettering_error']['mean_abs_rgb555'],
                            'failures': gates(facts, geometry['read_sectors'])})
            attempts.append(row)
    return {'id': plan['id'], 'catalog_item': plan['catalog_item'], 'source_container_sha256': plan['sha256'],
            'decoder_model': mdec_model.MODEL_ID, 'quantization': parsed['quantization'],
            'stream_bytes': parsed['consumed_bytes'], 'trailing_bytes': len(trailing), 'trailing_leading_zero_bytes': zero_run,
            'stream_byte_limit': parsed['consumed_bytes'] + zero_run, 'threshold': plan['threshold'],
            'edit_rects': rects, 'edited_macroblocks': edited_macroblocks(rects, source.shape[0]),
            'columns': columns, 'attempts': attempts,
            'any_passed': any(row.get('failures') == [] for row in attempts)}


def census(disc=None):
    """Hash-bound decode facts for every census file (shape inferred for inspection only)."""
    disc = Path(disc or ROOT / BS_DIR)
    rows = []
    for name in CENSUS_FILES:
        data = (disc / (name + '.BS')).read_bytes()
        parsed = mdec_splice.parse(data)
        count = len(parsed['macroblocks'])
        proved = name in GEOMETRY
        width, height = (GEOMETRY[name]['width'], GEOMETRY[name]['height']) if name in GEOMETRY else CENSUS_SHAPES[count]
        words = mdec_model.decode_padded_words(data, width, height)
        rows.append({'file': 'CDROM/BS/%s.BS' % name, 'sha256': digest(data), 'bytes': len(data),
                     'bs_version': parsed['version'], 'quantization': parsed['quantization'], 'macroblocks': count,
                     'trailing_bytes': len(parsed['trailing']), 'trailing_nonzero': any(parsed['trailing']),
                     'census_shape': [width, height], 'geometry': 'caller-proved' if proved else 'unresolved (census shape only)',
                     'census_words_sha256': digest(words), 'identity_splice_exact': mdec_splice.rebuild(data)[0] == data})
    return rows


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('command', choices=('census', 'build', 'sweep'))
    parser.add_argument('--output', type=Path)
    parser.add_argument('--publish', type=Path, help='also copy crops and manifest into this new repository directory')
    args = parser.parse_args()
    if args.command == 'census':
        print(json.dumps(census(), ensure_ascii=False, indent=1))
    elif args.command == 'sweep':
        print(json.dumps(sweep(), ensure_ascii=False, indent=1))
    else:
        if not args.output:
            parser.error('build needs --output')
        result = build(args.output)
        if args.publish:
            result['published'] = publish(args.output, args.publish)
        print(json.dumps(result))
