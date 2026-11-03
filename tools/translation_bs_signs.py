#!/usr/bin/env python3
"""T-39400: port-made English sign lettering drafts for BS still backgrounds.

Each plan below names one sign (or one erased/redrawn part of a sign) in a BS
background still. `build` decodes the original BS with the pinned fixed-point
MDEC model, replaces only the declared rectangle, and writes:

* the English still (whole image), the source still and an original | English
  side-by-side at 2x, all under the output directory only (ignored, never
  committed: they contain the original image);
* one crop PNG per plan, which is exactly the declared rectangle. Every pixel in
  a crop is computed, none is copied: the face behind the lettering is rebuilt
  either from a plane fitted (trimmed least squares) to the clean face pixels or,
  for `fill: diffuse`, by a Laplace (diffusion) fill from the clean pixels around
  the rectangle, and the English is drawn on it with the skeleton letters of
  tools/translation_bs_font.py (not a system font, not an AI image, not a scaled
  raster font), stroked in the ink colour sampled from the original lettering;
* a manifest with source and decode identity, the proven picture size, the
  rectangles, the 16x16 macroblocks they touch, the lettering description, the
  count of crop pixels that equal the source decode at their position (a smooth
  face can reproduce a flat original colour) and the Pillow / numpy versions.

Pixels outside the rectangles stay bit-identical to the source decode. No BS file
is written here; the later patch step re-encodes the touched macroblocks.

`geometry` prints the size proofs. Width and height of every still come from the
calling code in src/ (and are cross-checked on the stream: a complete decode has
exactly (w/16)*(h/16) macroblocks and an end marker). A citation names the C
function that holds the call (tools/translation_bs_common.py).
`publish` copies the crops, manifest and ledger into a new checkpoint folder.
"""
import argparse
import json
from pathlib import Path
import sys

import numpy as np
from PIL import Image

from translation_assets import ROOT, MDEC_MODEL_ID, digest, encoded, new_output
import translation_bs_common as common
from translation_bs_common import decode_still
from translation_bs_drafts import rgb8
import translation_bs_font as font
sys.path.insert(0, str(ROOT))
from tools.assets import image_mdec_model as mdec_model  # noqa: E402

SCHEMA = 'tokimemo.bs-sign-drafts.v1'
TICKET = 'T-39400'
BS_DIR = 'disc/files/CDROM/BS'
CHECKPOINT = 'bs-signs-v1'

# --- picture size proof ---------------------------------------------------------
# Files in CDROM/BS lie back to back in file-name order starting at LBA 0x3F03 (each file
# takes ceil(size / 2048) sectors). Eleven LBAs that the game code itself uses pin the model,
# including the last file group (P_01_02 at 0x4954 after 4177 sectors).
LAYOUT_START = 0x3F03
LAYOUT_ANCHORS = {
    'C_01_27': 0x3F03, 'C_01_28': 0x3F0D, 'C_01_29': 0x3F17, 'C_01_30': 0x3F20,
    'C_02_02A': 0x3FD7, 'C_02_02F': 0x3FE0, 'C_02_02H': 0x3FE9, 'C_02_02N': 0x3FF2,
    'P_01_02': 0x4954, 'P_01_03': 0x495D, 'P_01_03B': 0x4966,
}
DECODE_EVIDENCE = [
    'src/main/background_images.c:dec_bg_decode_to_vram(1) calls bg_still_decode_to_vram(0x2C0, 0, 0x140, 0xA0) or (0x140, 0x100, 0x140, 0xA0): width 0x140 = 320, height 0xA0 = 160',
    'src/main/memory_card.c:bg_still_decode_to_vram loops rect.x += 16 from arg0 to arg0 + arg2 and uploads one column of (arg3 + 8) >> 4 macroblocks per pass: 20 passes x 10 macroblocks = 200',
    'src/main/background_images.c:dec_bg_cd_read(lba, 1) calls cd_read_sectors_nowait(0xA, 0x80162000, lba): a ten-sector read, 20480 bytes',
]
SCENE_READ = 'src/main/scene_library.c:bg_read_sub2 calls dec_bg_cd_read(arg0, 1); src/main/scene_library.c:dec_init calls dec_bg_decode_to_vram(1)'
# (file, function, call text inside that function, line at the time of writing) of the code that reads and decodes each
# still; the build checks the function body only (tools/translation_bs_common.py: check_citations).
CALLERS = {
    'D_01_47': {'read': ('src/ovl/sports_festival/festival_entry_and_competition_select.c', 'sports_festival_entry_and_scene_select', 'dec_bg_cd_read(0x4020, 1)', 47),
                'decode': ('src/ovl/sports_festival/festival_entry_and_competition_select.c', 'sports_festival_entry_and_scene_select', 'dec_bg_decode_to_vram(1)', 54)},
    'D_01_48': {'read': ('src/ovl/culture_festival/festival_entry.c', 'culture_festival_mode_handler', 'dec_bg_cd_read(0x4029, 1)', 43),
                'decode': ('src/ovl/culture_festival/festival_entry.c', 'culture_festival_mode_handler', 'dec_bg_decode_to_vram(1)', 49)},
    'D_01_69': {'read': ('src/ovl/culture_festival/yumi_visit.c', 'culture_yumi_visit_experiment_show', 'dec_bg_cd_read(0x40FC, 1)', 450),
                'decode': ('src/ovl/culture_festival/yumi_visit.c', 'culture_yumi_visit_experiment_show', 'dec_bg_decode_to_vram(1)', 462)},
    'D_01_70': {'read': ('src/ovl/culture_festival/yumi_visit.c', 'culture_yumi_visit_experiment_show', 'dec_bg_cd_read(0x4105, 1)', 452),
                'decode': ('src/ovl/culture_festival/yumi_visit.c', 'culture_yumi_visit_experiment_show', 'dec_bg_decode_to_vram(1)', 462)},
    'D_03_08': {'read': ('src/ovl/club_master/club_master_scene_and_tables.c', 'master_companion_select', 'dec_bg_cd_read(0x41BA, 1)', 99),
                'decode': ('src/ovl/club_master/club_master_scene_and_tables.c', 'master_companion_select', 'dec_bg_decode_to_vram(1)', 161)},
    'D_11_11': {'read': ('src/ovl/date/rides.c', 'date_ride_scream_machine', 'cd_read_sectors_nowait(0xA, 0x80162000, 0x4359)', 273),
                'decode': ('src/ovl/date/rides.c', 'date_ride_scream_machine', 'dec_bg_decode_to_vram(1)', 284)},
    'D_15_03': {'read': ('src/ovl/date/scene_ayako.c', 'date_scene_ayako_autumn_background_2', 'bg_read_sub2(0x44F1)', 217), 'decode': None},
    'D_19_02': {'read': ('src/ovl/date/scene_yuko.c', 'date_scene_yuko_winter_background_2', 'bg_read_sub2(0x456C)', 354), 'decode': None},
    'D_24_03': {'read': ('src/ovl/date/place_scenes.c', 'date_place_setup_botanical_garden', 'bg_read_sub2(0x45D3)', 843), 'decode': None},
    'D_29_09': {'read': ('src/ovl/club_master/club_master_mode_stage_dispatch.c', 'master_award_intro_step_run', 'dec_bg_cd_read(0x4673, 1)', 58),
                'decode': ('src/ovl/club_master/club_master_mode_stage_dispatch.c', 'master_award_intro_step_run', 'dec_bg_decode_to_vram(1)', 74)},
    'I_11_15': {'read': ('src/ovl/date/scene_mira.c', 'date_scene_mira_autumn_background_1', 'bg_read_sub2(0x482A)', 215), 'decode': None},
    'I_17_01': {'read': ('src/ovl/date/scene_mio.c', 'date_scene_mio_autumn_background_1', 'bg_read_sub2(0x4876)', 192), 'decode': None},
    'I_28_02': {'read': ('src/ovl/date/scene_nozomi.c', 'date_scene_nozomi_winter_background_1', 'bg_read_sub2(0x492F)', 202), 'decode': None},
}
SHARED_CHECKS = [('src/main/scene_library.c', 'dec_bg_cd_read(arg0, 1)'), ('src/main/scene_library.c', 'dec_bg_decode_to_vram(1)'),
                 ('src/main/background_images.c', 'bg_still_decode_to_vram(0x2C0, 0, 0x140, 0xA0)'),
                 ('src/main/background_images.c', 'bg_still_decode_to_vram(0x140, 0x100, 0x140, 0xA0)'),
                 ('src/main/background_images.c', 'cd_read_sectors_nowait(0xA, 0x80162000, arg0)')]
# P_01_02 is read and decoded by the prologue board, not by the shared still path.
PROLOGUE = {'width': 640, 'height': 160, 'macroblocks': 400, 'read_sectors': 9,
            'read': ('src/ovl/name_entry/prologue.c', 'prologue_board_load', 'cd_read_sectors(9, 0x80162000, 0x4954)', 365),
            'decode': ('src/ovl/name_entry/prologue.c', 'prologue_board_load', 'bg_still_decode_to_vram(0x140, 0, 0x280, 0xA0)', 373)}
STILL_SIZE = (320, 160)
STILL_MACROBLOCKS = 200

# --- sign plans -----------------------------------------------------------------
# rect = [x, y, w, h] of the rectangle that is rebuilt (face fill + English). Every rectangle lies on
# the sign face, clear of the sign border. `runs` are the English lines: box [x, y, w, h] is where the
# line sits in the picture; rotation 90 turns the text to run down the sign (letter tops face right).
STROKE_NOTE = 'single-stroke skeleton letters, stroke weight set per sign from the original lettering'
SIGNS = [
    {'id': 'bs-p-01-02-class-1a-top', 'file': 'P_01_02', 'catalog_item': 'catalog-130', 'sign': 'class 1-A header (year mark)',
     'rect': [529, 43, 20, 30], 'fill': 'diffuse',
     'runs': [{'text': '1-', 'box': [531, 58, 15, 12], 'cap': 12, 'aspect': 0.8, 'stroke': 1.8}]},
    {'id': 'bs-p-01-02-class-1a-bottom', 'file': 'P_01_02', 'catalog_item': 'catalog-130', 'sign': 'class 1-A header (class mark removed)',
     'rect': [525, 89, 23, 18], 'fill': 'diffuse', 'runs': []},
    {'id': 'bs-p-01-02-class-1b-top', 'file': 'P_01_02', 'catalog_item': 'catalog-130', 'sign': 'class 1-B header (year mark)',
     'rect': [372, 13, 19, 31], 'fill': 'diffuse',
     'runs': [{'text': '1-', 'box': [374, 29, 14, 12], 'cap': 12, 'aspect': 0.8, 'stroke': 1.8}]},
    {'id': 'bs-p-01-02-class-1b-bottom', 'file': 'P_01_02', 'catalog_item': 'catalog-130', 'sign': 'class 1-B header (class mark removed)',
     'rect': [371, 66, 19, 19], 'fill': 'diffuse', 'runs': []},
    {'id': 'bs-p-01-02-class-1c-top', 'file': 'P_01_02', 'catalog_item': 'catalog-130', 'sign': 'class 1-C header (year mark)',
     'rect': [183, 0, 18, 15], 'fill': 'diffuse',
     'runs': [{'text': '1-', 'box': [184, 0, 15, 12], 'cap': 12, 'aspect': 0.8, 'stroke': 1.8}]},
    {'id': 'bs-p-01-02-class-1c-bottom', 'file': 'P_01_02', 'catalog_item': 'catalog-130', 'sign': 'class 1-C header (class mark removed)',
     'rect': [180, 38, 20, 20], 'fill': 'diffuse', 'runs': []},
]
SIGNS += [
    {'id': 'bs-d-01-47-standing-sign', 'file': 'D_01_47', 'catalog_item': 'catalog-133', 'sign': 'standing signboard, two columns',
     'init': 'light', 'rect': [18, 12, 42, 110], 'fit': [17, 11, 43, 111],
     'runs': [{'text': 'KIRAMEKI HIGH', 'box': [41, 14, 16, 96], 'cap': 11, 'aspect': 1.0, 'stroke': 2.2, 'rotation': 90},
              {'text': 'SPORTS FESTIVAL', 'box': [21, 14, 16, 104], 'cap': 10, 'aspect': 1.0, 'stroke': 2.0, 'rotation': 90}]},
    {'id': 'bs-d-01-48-standing-sign', 'file': 'D_01_48', 'catalog_item': 'catalog-134', 'sign': 'standing signboard, two columns',
     'init': 'light', 'rect': [18, 12, 42, 110], 'fit': [17, 11, 43, 111],
     'runs': [{'text': 'KIRAMEKI HIGH', 'box': [41, 14, 16, 96], 'cap': 11, 'aspect': 1.0, 'stroke': 2.2, 'rotation': 90},
              {'text': 'CULTURAL FESTIVAL', 'box': [21, 14, 16, 104], 'cap': 10, 'aspect': 1.0, 'stroke': 2.0, 'rotation': 90}]},
    {'id': 'bs-d-01-70-club-banner', 'file': 'D_01_70', 'catalog_item': 'catalog-136', 'sign': 'computer club banner',
     'init': 'light', 'rect': [113, 17, 90, 22], 'fit': [121, 17, 80, 22],
     'runs': [{'text': 'COMPUTER CLUB', 'box': [116, 19, 84, 17], 'cap': 11, 'aspect': 0.95, 'stroke': 1.6}]},
    {'id': 'bs-d-03-08-march-placard', 'file': 'D_03_08', 'catalog_item': 'catalog-139', 'sign': 'march placard',
     'init': 'light', 'rect': [213, 57, 31, 13], 'fit': [213, 57, 31, 13],
     'runs': [{'text': 'KIRAMEKI', 'box': [214, 58, 29, 11], 'cap': 8, 'aspect': 9, 'stroke': 1.0, 'gap': 2.6}]},
    {'id': 'bs-d-15-03-monument', 'file': 'D_15_03', 'catalog_item': 'catalog-143', 'sign': 'stone monument, two lines',
     'init': 'dark', 'rect': [230, 129, 53, 31], 'fit': [229, 128, 55, 32],
     'runs': [{'text': 'CENTRAL CITY', 'box': [231, 130, 50, 10], 'cap': 9, 'aspect': 9, 'stroke': 1.1, 'gap': 1.6},
              {'text': 'ART MUSEUM', 'box': [232, 146, 48, 10], 'cap': 9, 'aspect': 9, 'stroke': 1.1, 'gap': 1.6}]},
    {'id': 'bs-d-19-02-entrance-sign', 'file': 'D_19_02', 'catalog_item': 'catalog-144', 'sign': 'hanging entrance sign (arrow kept)',
     'init': 'light', 'rect': [37, 70, 24, 10], 'fit': [37, 69, 24, 12],
     'runs': [{'text': 'ENTER', 'box': [38, 71, 22, 8], 'cap': 7, 'aspect': 9, 'stroke': 1.1, 'gap': 2.2}]},
    {'id': 'bs-d-19-02-maiko-board', 'file': 'D_19_02', 'catalog_item': 'catalog-144', 'sign': 'rink dasher board, small shop line',
     'rect': [139, 41, 37, 5], 'fit': [138, 40, 40, 7],
     'runs': [{'text': 'LOVELY SHOP', 'box': [140, 41, 35, 5], 'cap': 5, 'aspect': 9, 'stroke': 0.9, 'gap': 1.3}]},
    {'id': 'bs-d-24-03-ticket-plaque', 'file': 'D_24_03', 'catalog_item': 'catalog-145', 'sign': 'ticket booth plaque',
     'init': 'light', 'rect': [235, 62, 20, 10], 'fit': [235, 62, 20, 10],
     'runs': [{'text': 'TICKET', 'box': [236, 62, 18, 10], 'cap': 6, 'aspect': 9, 'stroke': 0.9, 'gap': 2.0}]},
    {'id': 'bs-d-29-09-stage-banner', 'file': 'D_29_09', 'catalog_item': 'catalog-141', 'sign': 'stage banner',
     'init': 'light', 'rect': [12, 12, 284, 26], 'fit': [12, 2, 284, 35],
     'runs': [{'text': "INT'L BRASS BAND WINNERS CONCERT", 'box': [16, 15, 276, 19], 'cap': 12, 'aspect': 0.85, 'stroke': 1.7, 'gap': 3.4}]},
    {'id': 'bs-i-17-01-hanging-sign', 'file': 'I_17_01', 'catalog_item': 'catalog-149', 'sign': 'library aisle hanging sign',
     'init': 'light', 'rect': [109, 43, 34, 8], 'fit': [109, 43, 34, 8],
     'runs': [{'text': 'FINE ARTS', 'box': [110, 43, 32, 8], 'cap': 7, 'aspect': 9, 'stroke': 1.0, 'gap': 1.6}]},
    {'id': 'bs-i-28-02-counter-sign', 'file': 'I_28_02', 'catalog_item': 'catalog-146', 'sign': 'neon counter sign',
     'init': 'light', 'rect': [68, 34, 75, 9], 'fit': [26, 34, 114, 9], 'ink_share': 0.15,
     'runs': [{'text': 'SHOE RENTAL', 'box': [70, 34, 68, 9], 'cap': 7, 'aspect': 1.0, 'stroke': 1.0, 'gap': 4.0}]},
]
DEFAULTS = {'rotation': 0, 'aspect': 1.0, 'gap': font.GAP}
# How the face of each `fill` kind is made (recorded per crop).
FILL_NOTES = {'plane': 'face: per-channel plane fitted by trimmed least squares to the clean face pixels; English strokes mixed '
                       'over it in the sampled ink colour; no source pixel is copied',
              'diffuse': 'face: Laplace (diffusion) fill from the clean pixels around the rectangle; English strokes mixed over it '
                         'in the sampled ink colour; no source pixel is copied'}

# Files decoded and proven although no sign in them gets a draft (catalog bindings and a sign too
# cut off by the frame edge to read).
EXTRA_PROOF = ('D_01_69', 'D_11_11', 'I_11_15')


# --- decode / draw helpers ------------------------------------------------------
def contiguous_lbas(bs_dir):
    """{name: (lba, sectors)} for every file in the folder under the back-to-back layout."""
    lbas, lba = {}, LAYOUT_START
    for path in sorted(Path(bs_dir).glob('*.BS')):
        sectors = -(-path.stat().st_size // 2048)
        lbas[path.stem] = (lba, sectors)
        lba += sectors
    return lbas


def fit_plane(rgb5, rect, fit=None, ring=2, tolerance=2.0, rounds=6, init='median'):
    """Per-channel plane for the clean face colour behind rect; returns (float array of rect size, samples of the final fit).

    The samples are the pixels of `fit` (a face rectangle with margin from the sign border) or, without
    one, a ring around rect. Lettering and borders are rejected by trimmed least squares: start from the
    median colour (of the lightest or darkest samples for `init` light or dark, which keeps a face that
    is mostly lettering from pulling the fit), keep samples within `tolerance` (5-bit steps, any channel), refit, repeat.
    """
    x, y, w, h = rect
    height, width = rgb5.shape[:2]
    if fit is not None:
        fx, fy, fw, fh = fit
        x0, y0, x1, y1 = fx, fy, fx + fw, fy + fh
        keep = np.ones((y1 - y0, x1 - x0), bool)
    else:
        x0, y0, x1, y1 = max(0, x - ring), max(0, y - ring), min(width, x + w + ring), min(height, y + h + ring)
        keep = np.ones((y1 - y0, x1 - x0), bool)
        keep[y - y0:y - y0 + h, x - x0:x - x0 + w] = False
    ys, xs = np.nonzero(keep)
    design = np.stack([np.ones_like(xs), xs + x0, ys + y0], 1).astype(float)
    sample = rgb5[y0:y1, x0:x1][keep].astype(float)
    if len(xs) < 6:
        raise ValueError('no pixels to fit the face')
    brightness = sample.sum(1)
    if init == 'light':
        pick = brightness >= np.percentile(brightness, 60)
    elif init == 'dark':
        pick = brightness <= np.percentile(brightness, 40)
    else:
        pick = np.ones(len(xs), bool)
    prediction = np.broadcast_to(np.median(sample[pick], 0), sample.shape)
    use = np.abs(sample - prediction).max(1) <= 2 * tolerance
    if use.sum() < 6:
        use = np.ones(len(xs), bool)
    for _ in range(rounds):
        coefficients = np.stack([np.linalg.lstsq(design[use], sample[use, c], rcond=None)[0] for c in range(3)], 1)
        fitted = int(use.sum())  # the samples these coefficients were fitted to
        new = np.abs(design @ coefficients - sample).max(1) <= tolerance
        if new.sum() < 6 or (new == use).all():
            break
        use = new
    gy, gx = np.mgrid[y:y + h, x:x + w]
    grid = np.stack([np.ones(w * h), gx.ravel(), gy.ravel()], 1).astype(float)
    return (grid @ coefficients).reshape(h, w, 3), fitted


def fill_diffuse(rgb5, rect, margin=3, bad=10, iterations=900):
    """Face behind rect by diffusion (Laplace fill) from the clean pixels around it.

    Pixels in the margin ring whose brightness differs from the ring median by more than `bad` (5-bit
    channel sum) are treated as lettering and do not feed the fill; only rect is returned.
    """
    x, y, w, h = rect
    height, width = rgb5.shape[:2]
    x0, y0, x1, y1 = max(0, x - margin), max(0, y - margin), min(width, x + w + margin), min(height, y + h + margin)
    block = rgb5[y0:y1, x0:x1].astype(float)
    unknown = np.zeros(block.shape[:2], bool)
    unknown[y - y0:y - y0 + h, x - x0:x - x0 + w] = True
    ring = block.sum(-1)
    reference = np.median(ring[~unknown])
    unknown |= np.abs(ring - reference) > bad
    value = block.copy()
    known = ~unknown
    if known.sum() < 6:
        raise ValueError('no clean pixels around the rectangle')
    value[unknown] = block[known].mean(0)
    for _ in range(iterations):
        padded = np.pad(value, ((1, 1), (1, 1), (0, 0)), mode='edge')
        mean = (padded[:-2, 1:-1] + padded[2:, 1:-1] + padded[1:-1, :-2] + padded[1:-1, 2:]) / 4.0
        value[unknown] = mean[unknown]
    return value[y - y0:y - y0 + h, x - x0:x - x0 + w], int(known.sum())


def sample_ink(rgb5, rect, face, share=0.06):
    """Mean colour of the pixels that differ most from the face inside rect, and the pixel count used."""
    x, y, w, h = rect
    block = rgb5[y:y + h, x:x + w].astype(float)
    distance = np.abs(block - face).sum(-1)
    peak = distance.max()
    if peak < 6:
        raise ValueError('no lettering found in the rectangle')
    count = max(3, int(round(share * (distance > 0.35 * peak).sum())))
    order = np.argsort(distance.ravel())[::-1][:count]
    return block.reshape(-1, 3)[order].mean(0), count


def run_lines(run):
    """(image-space stroke lines, facts) of one English line."""
    spec = {**DEFAULTS, **run}
    bx, by, bw, bh = spec['box']
    vertical = spec['rotation'] == 90
    length = bh if vertical else bw
    local = font.strokes_for(spec['text'], length, spec['cap'], spec['aspect'], spec['gap'])
    facts = font.text_extent(spec['text'], length, spec['cap'], spec['aspect'], spec['gap'])
    if vertical:
        lines = font.to_image(local, (bx + (bw + spec['cap']) / 2.0, by), 90)
    else:
        lines = font.to_image(local, (bx, by + (bh - spec['cap']) / 2.0), 0)
    return lines, {'text': spec['text'], 'box': spec['box'], 'cap_px': spec['cap'], 'rotation': spec['rotation'],
                   'stroke_px': spec['stroke'], 'letter_gap_units': spec['gap'], **facts}


def draw_plan(source, plan):
    """(English rgb5 picture, record) for one plan; changes nothing outside plan['rect']."""
    x, y, w, h = plan['rect']
    if plan.get('fill') == 'diffuse':
        face, fitted = fill_diffuse(source, plan['rect'], plan.get('margin', 3))
    else:
        face, fitted = fit_plane(source, plan['rect'], plan.get('fit'), tolerance=plan.get('tolerance', 2.0), init=plan.get('init', 'median'))
    ink, ink_count = (sample_ink(source, plan['rect'], face, plan.get('ink_share', 0.06)) if plan['runs'] else (None, 0))
    english = source.copy()
    coverage = np.zeros((h, w))
    placed = []
    for run in plan['runs']:
        lines, facts = run_lines(run)
        coverage = np.maximum(coverage, font.render_coverage((h, w), lines, run['stroke'], (x, y)))
        placed.append(facts)
    mix = face if ink is None else face * (1 - coverage[..., None]) + ink * coverage[..., None]
    english[y:y + h, x:x + w] = np.clip(np.rint(mix), 0, 31).astype(int)
    record = {'face_fit_pixels': fitted, 'ink_rgb555': None if ink is None else [round(float(v), 2) for v in ink],
              'ink_pixels_used': ink_count, 'lines': placed}
    return english, record


def check_callers(root=ROOT):
    """Problems (empty when every cited call is still inside its function and every shared check holds)."""
    cites = [c for pair in CALLERS.values() for c in pair.values() if c] + [PROLOGUE['read'], PROLOGUE['decode']]
    return common.check_citations(cites, SHARED_CHECKS, root)


def geometry_proof(name, data, lba, sectors):
    if name == 'P_01_02':
        width, height = PROLOGUE['width'], PROLOGUE['height']
        calls = [PROLOGUE['read'], PROLOGUE['decode']]
        read_sectors = PROLOGUE['read_sectors']
        evidence = ['src/ovl/name_entry/prologue.c: prologue_board_load reads 9 sectors from LBA 0x4954 and decodes with bg_still_decode_to_vram(0x140, 0, 0x280, 0xA0): 640 x 160']
    else:
        width, height = STILL_SIZE
        calls = [c for c in (CALLERS[name]['read'], CALLERS[name]['decode']) if c]
        read_sectors = 10
        evidence = list(DECODE_EVIDENCE) + ([] if CALLERS[name]['decode'] else [SCENE_READ])
    return common.geometry_proof(data, lba, sectors, width, height, read_sectors, calls, evidence,
                                 'back-to-back layout from 0x3F03, pinned by %d game-code LBAs' % len(LAYOUT_ANCHORS))


def build(output, disc=None):
    disc = Path(disc or ROOT / BS_DIR)
    output = new_output(output)
    problems = check_callers()
    if problems:
        raise ValueError('cited calls changed: ' + '; '.join(problems))
    lbas = contiguous_lbas(disc)
    for name, lba in LAYOUT_ANCHORS.items():
        if lbas[name][0] != lba:
            raise ValueError('layout model disagrees with game-code LBA for ' + name)
    files = sorted({plan['file'] for plan in SIGNS} | set(EXTRA_PROOF))
    proofs, decoded, hashes = {}, {}, {}
    for name in files:
        data = (disc / (name + '.BS')).read_bytes()
        proofs[name] = geometry_proof(name, data, *lbas[name])
        width, height = proofs[name]['width'], proofs[name]['height']
        decoded[name] = decode_still(data, width, height)
        hashes[name] = {'container': digest(data), 'words': digest(mdec_model.decode_padded_words(data, width, height)),
                        'rgb8': digest(rgb8(decoded[name]).tobytes())}
    output.mkdir(parents=True)
    (output / 'previews').mkdir()
    (output / 'crops').mkdir()
    englishes = {name: decoded[name].copy() for name in files}
    assets = []
    for plan in SIGNS:
        name = plan['file']
        source = decoded[name]
        english, record = draw_plan(source, plan)
        x, y, w, h = plan['rect']
        outside = np.ones(source.shape[:2], bool)
        outside[y:y + h, x:x + w] = False
        if (english[outside] != source[outside]).any():
            raise ValueError('draft changes pixels outside its rectangle: ' + plan['id'])
        englishes[name][y:y + h, x:x + w] = english[y:y + h, x:x + w]
        crop = rgb8(english[y:y + h, x:x + w])
        common.zoom_pair(source, english, plan['rect']).save(output / 'previews' / (plan['id'] + '.zoom.png'))
        png = plan['id'] + '.en.png'
        Image.fromarray(crop, 'RGB').save(output / 'crops' / png, optimize=False)
        indices, mb_rect = common.macroblocks(plan['rect'], proofs[name]['height'] + (-proofs[name]['height'] % 16))
        changed = int((english[y:y + h, x:x + w] != source[y:y + h, x:x + w]).any(-1).sum())
        assets.append({
            'id': plan['id'], 'catalog_item': plan['catalog_item'], 'sign': plan['sign'],
            'source_container': 'CDROM/BS/%s.BS' % name, 'source_container_sha256': hashes[name]['container'],
            'frame': 0, 'palette_id': 'mdec:' + MDEC_MODEL_ID, 'decoder_model': MDEC_MODEL_ID,
            'source_words_sha256': hashes[name]['words'], 'source_png_sha256': hashes[name]['rgb8'],
            'source_png_sha256_basis': 'raw 8-bit RGB pixels of the decoded still (replicate expansion), not PNG file bytes',
            'geometry': {'width': proofs[name]['width'], 'height': proofs[name]['height'], 'proof': 'geometry_proofs.' + name},
            'edit_regions': [plan['rect']], 'edit_macroblocks': indices, 'edit_macroblock_bounds': mb_rect,
            'edit_region_policy': 'the declared rectangle is rebuilt (face fill + lettering); every pixel outside is bit-identical to the source decode',
            'crop': {'png': png, 'rect': plan['rect'], 'sha256': digest((output / 'crops' / png).read_bytes()),
                     'source_equal_pixels': common.source_equal_pixels(crop, source[y:y + h, x:x + w]),
                     'source_equal_pixels_basis': 'crop pixels whose RGB555 value equals the source decode at the same position',
                     'fill': FILL_NOTES[plan.get('fill', 'plane')]},
            'changed_pixels': changed, 'lettering': record, 'port_made': True,
            'port_made_elements': [{'kind': 'lettering', 'face': 'port-skeleton-gothic', 'origin': 'port-made',
                                    'description': STROKE_NOTE + '; lines: ' + ', '.join(r['text'] for r in plan['runs']) if plan['runs']
                                    else 'class mark removed; face refilled by a Laplace (diffusion) fill from the clean pixels around it, no lettering drawn'}],
            'style': 'unreviewed', 'human_approved': False, 'runtime_integrated': False, 'in_game_verified': False})
    for name in files:
        plans = [p for p in SIGNS if p['file'] == name]
        if not plans:
            continue
        Image.fromarray(rgb8(englishes[name]), 'RGB').save(output / 'previews' / (name + '.en.png'))
        common.side_by_side(decoded[name], englishes[name]).save(output / 'previews' / (name + '.compare-2x.png'))
    manifest = {'schema': SCHEMA, 'ticket': TICKET, 'human_approved': False, 'assets': assets,
                'geometry_proofs': proofs, 'layout_anchors': {k: '0x%X' % v for k, v in LAYOUT_ANCHORS.items()},
                'decoder_model': MDEC_MODEL_ID, 'environment': common.environment(),
                'committed_files': 'crop PNGs (computed rectangles: face fill and lettering, no pixel copied from the source) and this manifest; whole stills and comparisons stay local'}
    (output / 'manifest.json').write_bytes(encoded(manifest) + b'\n')
    return {'output': str(output), 'assets': len(assets), 'files': files}


def publish(built, destination):
    """Copy crops, manifest and ledger into a new checkpoint folder (no full images, no BS bytes)."""
    return common.publish(built, destination, CHECKPOINT, TICKET, {
        'full_images_policy': 'only crops of rectangles that hold port-made pixels are committed; run build to rebuild whole stills into .asset-cache',
        'publication_authorization': 'Owner allowed new in-style lettering, 2026-11-01 (T-39400).'})


def geometry(disc=None):
    disc = Path(disc or ROOT / BS_DIR)
    lbas = contiguous_lbas(disc)
    out = {}
    for name in sorted({p['file'] for p in SIGNS} | set(EXTRA_PROOF)):
        out[name] = geometry_proof(name, (disc / (name + '.BS')).read_bytes(), *lbas[name])
    return out


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('command', choices=('build', 'geometry'))
    parser.add_argument('--output', type=Path)
    parser.add_argument('--publish', type=Path, help='also copy crops, manifest and ledger into this new checkpoint folder')
    args = parser.parse_args()
    if args.command == 'geometry':
        print(json.dumps(geometry(), indent=1))
    else:
        if not args.output:
            parser.error('build needs --output')
        result = build(args.output)
        if args.publish:
            result['published'] = publish(args.output, args.publish)
        print(json.dumps(result))
