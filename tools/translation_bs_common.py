"""T-39400 / T-39412: code shared by the BS sign and still draft tools.

tools/translation_bs_signs.py (T-39400) and tools/translation_bs_stills.py (T-39412) both decode original BS stills with
the pinned MDEC model, prove each picture size from the game code that reads and decodes it, and publish crops, a
manifest and a ledger into a checkpoint folder. This module holds the parts they share:

* source citations: (path, function, call text, line) tuples. A citation holds when the call text occurs inside the body
  of that C function; the line number is recorded in manifests for a human reader and is never checked, so an unrelated
  line shift in the file does not break a build or a test;
* the geometry proof (picture size from the callers, macroblock count, end marker and trailing bytes of the stream);
* the 16x16 macroblocks a rectangle touches (tools/translation_bs_drafts.py helpers);
* preview sheets (original | English side by side, zoomed original-over-English pair);
* the environment record (Pillow and numpy versions), because the crops and manifests are byte-exact outputs of those
  libraries and a different version may round or compress differently;
* `publish`: copy crops and manifest into a new checkpoint folder with a ledger.
"""
import json
from pathlib import Path
import re
import sys

import numpy as np
import PIL
from PIL import Image

from translation_assets import ROOT, digest, encoded
from translation_bs_drafts import bounding, edited_macroblocks, rgb8
sys.path.insert(0, str(ROOT))
from tools.assets import mdec_splice  # noqa: E402


# --- source citations -----------------------------------------------------------
def function_body(text, name):
    """Text of the C function `name` (its definition line up to the closing brace at column 0), or None."""
    match = re.search(r'^[A-Za-z_][^;\n]*\b%s\s*\([^;\n]*\)\s*\{\s*$' % re.escape(name), text, re.M)
    if not match:
        return None
    end = re.search(r'^\}', text[match.end():], re.M)
    return text[match.start():match.end() + (end.end() if end else len(text) - match.end())]


def check_citations(cites, file_checks=(), root=ROOT):
    """Problems (empty when every cited call is still inside its function, and every file check text is in its file).

    cites: (path, function, text, line) tuples; the line is not checked. file_checks: (path, text) pairs.
    """
    problems = []
    for path, function, text, _line in cites:
        body = function_body((Path(root) / path).read_text(), function)
        if body is None:
            problems.append('%s has no function %s' % (path, function))
        elif text not in body:
            problems.append('%s:%s does not contain %s' % (path, function, text))
    for path, text in file_checks:
        if text not in (Path(root) / path).read_text():
            problems.append('%s lacks %s' % (path, text))
    return problems


def cite_record(cite):
    path, function, text, line = cite
    return {'path': path, 'function': function, 'text': text, 'line': line,
            'line_note': 'line at the time of writing, for a human reader; the build checks the function body only'}


# --- decode and geometry --------------------------------------------------------
def decode_still(data, width, height):
    return mdec_splice.decode_rgb5(data, width, height)[:height]


def stream_facts(data, width, height):
    """Macroblock count, end marker and trailing bytes of a BS v1/v2 stream, checked against width x height."""
    parsed = mdec_splice.parse(data)
    count = len(parsed['macroblocks'])
    expected = (width // 16) * ((height + 15) // 16)
    if count != expected:
        raise ValueError('stream has %d macroblocks, %dx%d needs %d' % (count, width, height, expected))
    decode_still(data, width, height)  # the pinned model decodes it with every word STP-marked
    trailing = parsed['trailing']
    return {'macroblocks': count, 'expected_macroblocks': expected, 'consumed_bytes': parsed['consumed_bytes'],
            'trailing_bytes': len(trailing), 'trailing_all_zero': not any(trailing), 'end_marker': True}


def geometry_proof(data, lba, sectors, width, height, read_sectors, calls, evidence, lba_basis):
    """Size proof of one still: the caller-proved size, the cited calls and the stream facts at that size."""
    return {'width': width, 'height': height, 'lba': lba, 'lba_hex': '0x%X' % lba, 'file_sectors': sectors,
            'read_sectors': read_sectors, 'lba_basis': lba_basis, 'callers': [cite_record(c) for c in calls],
            'code_evidence': list(evidence), 'stream': stream_facts(data, width, height)}


def macroblocks(rect, padded_height):
    """(sorted macroblock indices, bounding [x, y, w, h] in pixels) of the 16x16 macroblocks rect touches."""
    indices = edited_macroblocks([rect], padded_height)
    return indices, bounding(indices, padded_height)


def source_equal_pixels(crop, source_block):
    """Count of written crop pixels (opaque, or alpha 255 in an RGBA crop) whose RGB555 value equals the source decode at
    the same position. The 8-bit crop values are replicate-expanded RGB555, so >> 3 restores them exactly."""
    pixels = np.asarray(crop)
    written = pixels[..., 3] == 255 if pixels.shape[-1] == 4 else np.ones(pixels.shape[:2], bool)
    rgb5 = pixels[..., :3].astype(int) >> 3
    return int(((rgb5 == source_block).all(-1) & written).sum())


# --- previews -------------------------------------------------------------------
def side_by_side(source, english):
    """Original | English at 2x, pixelated."""
    left = Image.fromarray(rgb8(source), 'RGB').resize((source.shape[1] * 2, source.shape[0] * 2), Image.NEAREST)
    right = Image.fromarray(rgb8(english), 'RGB').resize((english.shape[1] * 2, english.shape[0] * 2), Image.NEAREST)
    sheet = Image.new('RGB', (left.width * 2 + 8, left.height), (255, 0, 255))
    sheet.paste(left, (0, 0))
    sheet.paste(right, (left.width + 8, 0))
    return sheet


def zoom_pair(source, english, rect, margin=8, limit=900, max_scale=10):
    """Original over English of one rectangle with a margin of surrounding picture, pixelated at the largest whole scale
    (2 to max_scale) that keeps the sheet within `limit` px wide."""
    x, y, w, h = rect
    x0, y0 = max(0, x - margin), max(0, y - margin)
    x1, y1 = min(source.shape[1], x + w + margin), min(source.shape[0], y + h + margin)
    scale = max(2, min(max_scale, limit // (x1 - x0)))
    size = ((x1 - x0) * scale, (y1 - y0) * scale)
    top = Image.fromarray(rgb8(source[y0:y1, x0:x1]), 'RGB').resize(size, Image.NEAREST)
    bottom = Image.fromarray(rgb8(english[y0:y1, x0:x1]), 'RGB').resize(size, Image.NEAREST)
    sheet = Image.new('RGB', (size[0], size[1] * 2 + 6), (255, 0, 255))
    sheet.paste(top, (0, 0))
    sheet.paste(bottom, (0, size[1] + 6))
    return sheet


# --- environment ----------------------------------------------------------------
def environment():
    """Library versions the byte-exact outputs depend on (PNG encoding, float rounding)."""
    return {'pillow': PIL.__version__, 'numpy': np.__version__}


def environment_mismatch(manifest):
    """None when this run uses the library versions the manifest records, else a message naming both."""
    recorded = manifest.get('environment')
    if recorded == environment():
        return None
    return 'byte-exact comparison needs %s (recorded in the published manifest); this run has %s' % (recorded, environment())


# --- publish --------------------------------------------------------------------
def publish(built, destination, checkpoint, ticket, ledger_facts, row_extra=None):
    """Copy crops and manifest from `built` into the new folder destination/checkpoint and write destination/ledger.json.

    ledger_facts: the ledger keys besides schema, ticket and assets (policies, authorization). row_extra(asset) returns
    extra keys for that asset's ledger row. Built through a staging folder that is renamed at the end.
    """
    manifest_bytes = (built / 'manifest.json').read_bytes()
    manifest = json.loads(manifest_bytes)
    destination = destination.absolute()
    if destination.exists():
        raise ValueError('destination must be new')
    staging = destination.with_name(destination.name + '.partial')
    if staging.exists():
        raise ValueError('stale staging directory: ' + str(staging))
    (staging / checkpoint).mkdir(parents=True)
    ledger = []
    for asset in manifest['assets']:
        crop = built / 'crops' / asset['crop']['png']
        if digest(crop.read_bytes()) != asset['crop']['sha256']:
            raise ValueError('built crop changed: ' + asset['id'])
        (staging / checkpoint / asset['crop']['png']).write_bytes(crop.read_bytes())
        row = {'id': asset['id'], 'catalog_item': asset['catalog_item'], 'path': '%s/%s' % (checkpoint, asset['crop']['png']),
               'crop_sha256': asset['crop']['sha256'], 'dimensions': asset['crop']['rect'][2:],
               'source_container_sha256': asset['source_container_sha256'], 'source_png_sha256': asset['source_png_sha256'],
               'decode_identity': {'decoder_model': asset['decoder_model'], 'source_words_sha256': asset['source_words_sha256'],
                                   'width': asset['geometry']['width'], 'height': asset['geometry']['height']},
               'edit_regions': asset['edit_regions'], 'port_made': True, 'port_made_elements': asset['port_made_elements'],
               'manifest_sha256': digest(manifest_bytes), 'status': 'draft', 'human_approved': False}
        row.update(row_extra(asset) if row_extra else {})
        ledger.append(row)
    (staging / checkpoint / 'manifest.json').write_bytes(manifest_bytes)
    (staging / 'ledger.json').write_bytes(encoded({
        'schema': 'tokimemo.translation-checkpoint.v1', 'ticket': ticket, 'assets': ledger,
        'candidate_assets': len(ledger), 'original_images_included': False, 'port_made_assets': len(ledger),
        'complete_translation_inventory': False, 'separate_reviewed_revisions': 0, **ledger_facts}) + b'\n')
    staging.rename(destination)
    return {'destination': str(destination), 'assets': len(ledger)}
