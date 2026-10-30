#!/usr/bin/env python3
"""T-39004: restore native menu lettering and create unapproved OPTION drafts.

Run in the existing Pillow Docker image. User authorized image-editor fallback
in Tokimemo Orchestration, 2026-10-26. Source originals and approvals are immutable.
"""
import argparse
from collections import Counter
import json
from pathlib import Path

from PIL import Image

from translation_assets import ROOT, audit, digest, encoded, inside, new_output

SHADOW = (24, 24, 24, 255)
# New glyphs extend the recovered menu9 grid. Seven-row capitals use its exact
# duplicated first/middle rows; lowercase retains the existing six-row x-height.
CAPITALS = {
    'G': '01111 10000 10000 10111 10001 10001 01111',
    'H': '10001 10001 10001 11111 10001 10001 10001',
    'I': '111 010 010 010 010 010 111',
    'J': '00111 00010 00010 00010 10010 10010 01100',
    'K': '10001 10010 10100 11000 10100 10010 10001',
    'O': '01110 10001 10001 10001 10001 10001 01110',
    'P': '11110 10001 10001 11110 10000 10000 10000',
    'Q': '01110 10001 10001 10001 10101 10010 01101',
    'U': '10001 10001 10001 10001 10001 10001 01110',
    'W': '10001 10001 10001 10101 10101 10101 01010',
    'Y': '10001 10001 01010 00100 00100 00100 00100',
    'Z': '11111 00001 00010 00100 01000 10000 11111',
}
EXTRA = {
    ':': '0 1 1 0 0 0 1 1 0',
    '/': '0001 0001 0010 0010 0010 0100 0100 1000 1000',
    'j': '01 01 00 01 01 01 01 01 10',
    'q': '0000 0000 0000 0111 1001 1001 0111 0001 0001',
    'v': '00000 00000 00000 10001 10001 10001 10001 01010 00100',
    'x': '0000 0000 0000 1001 1001 0110 0110 1001 1001',
    'z': '0000 0000 0000 1111 0001 0010 0100 1000 1111',
    '(': '001 010 010 100 100 100 010 010 001',
    ')': '100 010 010 001 001 001 010 010 100',
    '.': '0 0 0 0 0 0 0 1 1',
    '-': '000 000 000 000 111 000 000 000 000',
    '+': '00000 00000 00100 00100 11111 00100 00100 00000 00000',
}
# Corrected from the restored original pixels, not the misordered old catalog.
CAST = {
    63: 'Kingetsu Mami (Fujisaki)', 65: 'Sekine Akiko (Kisaragi)',
    67: 'Naka Tomoko (Himoo)', 69: 'Kawaguchi Masayo (Katagiri)',
    71: 'Sugawara Sachiko (Nijino)', 73: 'Kurosaki Ayako (Koshiki)',
    75: 'Sasaki Ayako (Kiyokawa)', 77: 'Igarashi Rei (Kagami)',
    79: 'Teppouzuka Yoko (Asahina)', 81: 'Kurihara Mikiko (Mikihara)',
    83: 'Yoshiki Kurin (Saotome)', 85: 'Kikuchi Shiho (Tatebayashi)',
    87: 'Tsunoda Narumi (Ijuin)', 89: 'Ueda Yuji (Yoshio)',
}


def ink(pixel):
    return pixel[3] != 0 and pixel != SHADOW


def recover_font(snapshot):
    report = audit(snapshot)
    manifest = json.loads((snapshot / 'manifest.json').read_text())
    glyphs, witnesses, variants = {}, {}, {}
    for version in manifest['versions']:
        if not version['current']:
            continue
        for asset in version['assets']:
            if not asset['id'].startswith('option-label-'):
                continue
            image = Image.open(inside(snapshot, asset['output'])).convert('RGBA')
            layout = asset['layout']
            y = int(layout['origin'][1])
            columns = [any(ink(image.getpixel((x, row))) for row in range(image.height)) for x in range(image.width)]
            runs, start = [], None
            for x, occupied in enumerate(columns + [False]):
                if occupied and start is None:
                    start = x
                if not occupied and start is not None:
                    runs.append((start, x))
                    start = None
            letters = layout['text'].replace(' ', '')
            if len(letters) != len(runs):
                raise ValueError('cannot recover separated glyphs: ' + asset['id'])
            for letter, (left, right) in zip(letters, runs):
                mask = [''.join('1' if ink(image.getpixel((x, row))) else '0' for x in range(left, right)) for row in range(y, y + 9)]
                variants.setdefault(letter, {})[tuple(mask)] = asset['output_sha256']
                glyphs[letter] = mask
                witnesses[letter] = {'asset': asset['id'], 'output_sha256': asset['output_sha256'], 'rect': [left, y, right-left, 9]}
    for letter, rows in CAPITALS.items():
        seven = rows.split()
        glyphs[letter] = [seven[i] for i in [0, 0, 1, 2, 3, 3, 4, 5, 6]]
    glyphs.update({letter: rows.split() for letter, rows in EXTRA.items()})
    return glyphs, {'snapshot_sha256': report['snapshot_sha256'], 'recovered': witnesses,
                    'new_glyphs': sorted(CAPITALS.keys() | EXTRA.keys()),
                    'historical_variants': {k: len(v) for k, v in variants.items() if len(v) > 1}}


def text_width(text, glyphs):
    return sum(3 if letter == ' ' else len(glyphs[letter][0]) + 1 for letter in text)


def draft_all(source_root, output, snapshot):
    output = new_output(output)
    glyphs, provenance = recover_font(snapshot)
    source_manifest = json.loads((source_root / 'manifest.json').read_text())
    container_sha = source_manifest['metadata']['source_sha256']
    if container_sha != '285aebdee243903fc1eba7b38ea9a6e432c6961d16021aa1a812c36194a420fe':
        raise ValueError('wrong original OPTION container')
    sources = {row['frame_index']: row for row in source_manifest['images'] if 'frame_index' in row}
    catalog = [json.loads(line) for line in (snapshot.parent / 'image_text_catalog.jsonl').read_bytes().splitlines()]
    from translation_assets import inventory
    requirements = inventory(snapshot, snapshot.parent / 'image_text_catalog.jsonl', source_root)
    groups = {row['id']: row for row in requirements['requirements']}
    plans = []
    for row in requirements['option_frames']:
        if row['status'] != 'missing-artwork':
            continue
        frame = row['frame']
        group = groups[row['requirement']]
        text = catalog[group['catalog_line']-1]['english_translation']
        odd = frame if frame % 2 else frame - 1
        text = CAST.get(odd, text)
        if frame in (122, 123):
            text = 'Mouse-like'
        original = Image.open(inside(source_root, sources[frame]['png'])).convert('RGBA')
        width = text_width(text, glyphs)
        if width > original.width and odd in CAST:
            surname, given, role = text.split(' ', 2)
            text = given[0] + '. ' + surname + ' ' + role
            width = text_width(text, glyphs)
        if width > original.width and 91 <= frame <= 116:
            surname, given = text.split(' ', 1)
            text = surname + ' ' + given[0] + '.'
            width = text_width(text, glyphs)
        if width > original.width:
            raise ValueError('text needs an explicit wording decision: %s / %s' % (frame, text))
        pixels = [(x,y) for y in range(12) for x in range(original.width) if ink(original.getpixel((x,y)))]
        left, baseline = min(x for x,y in pixels), max(y for x,y in pixels)
        if left + width > original.width:
            raise ValueError('source bearing leaves insufficient space')
        colors = []
        all_colors = Counter(original.getpixel(p) for p in pixels)
        for y in range(11):
            counts = Counter(original.getpixel((x,y)) for x in range(original.width) if ink(original.getpixel((x,y))))
            colors.append((counts or all_colors).most_common(1)[0][0])
        palette = [colors[round(i*10/8)] for i in range(9)]
        result = original.copy()
        result.paste((0,0,0,0),(0,0,original.width,12))
        points = []; x = left; top = baseline - 8
        for letter in text:
            if letter == ' ':
                x += 3
                continue
            for y, bits in enumerate(glyphs[letter]):
                points.extend((x+dx,top+y,y) for dx,bit in enumerate(bits) if bit == '1')
            x += len(glyphs[letter][0]) + 1
        for x,y,_ in points:
            if x+1 >= original.width or y+1 >= 12:
                raise ValueError('glyph shadow overflow')
            result.putpixel((x+1,y+1),SHADOW)
        for x,y,row_index in points:
            result.putpixel((x,y),palette[row_index])
        plans.append((result, {'id':'option-label-%04d'%frame,'frame':frame,'text':text,
            'source_container_sha256':container_sha,'source_png_sha256':row['source_png_sha256'],
            'source_png':sources[frame]['png'],'dimensions':list(original.size),
            'output':'frame-%04d.png'%frame,'source_ink_left':left,'source_ink_baseline':baseline,
            'font_origin':[left,top],'row_ink_rgba':palette,'style':'unreviewed',
            'in_game_verified':False,'runtime_integrated':False}))
    output.mkdir(parents=True)
    for image, plan in plans:
        image.save(output/plan['output'])
        plan['output_sha256'] = digest((output/plan['output']).read_bytes())
    (output/'font.json').write_bytes(encoded({'glyphs':glyphs,'provenance':provenance})+b'\n')
    (output/'manifest.json').write_bytes(encoded({'schema':'tokimemo.option-translation-drafts.v1',
        'font_sha256':digest((output/'font.json').read_bytes()),'assets':[plan for _,plan in plans],
        'runtime_verified':False,'human_approved':False})+b'\n')
    return {'drafts':len(plans),'output':str(output)}


def restore_sources(output, disc_root):
    from assets.image_packs_option import export_option_ui
    output = new_output(output)
    original = disc_root / 'CDROM/FUKUTEST/OPTION.NBN'
    data = original.read_bytes()
    if digest(data) != '285aebdee243903fc1eba7b38ea9a6e432c6961d16021aa1a812c36194a420fe':
        raise ValueError('wrong original OPTION container')
    result = export_option_ui(data, output, 'option')
    (output/'manifest.json').write_bytes(encoded(result)+b'\n')
    return {'original_frames':len(result['images']),'output':str(output)}


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sources',type=Path,required=True)
    parser.add_argument('--output',type=Path)
    parser.add_argument('--restore-sources',action='store_true')
    parser.add_argument('--disc-root',type=Path,default=ROOT/'disc/files')
    args = parser.parse_args()
    if args.restore_sources:
        print(json.dumps(restore_sources(args.sources,args.disc_root)))
    elif args.output is None:
        parser.error('--output is required when rendering drafts')
    else:
        print(json.dumps(draft_all(args.sources,args.output,ROOT/'translations/en/graphics/artwork')))
