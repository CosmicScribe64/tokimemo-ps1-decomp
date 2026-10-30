#!/usr/bin/env python3
"""T-39004: source-palette name-entry button drafts; borders remain untouched."""
import argparse
from collections import Counter
import json
from pathlib import Path

from PIL import Image

from translation_assets import ROOT, digest, encoded, inside, new_output
from translation_native_font import NativeLatin

LABELS = {15:'Hira',16:'Hira',17:'Kata',18:'Kata',19:'Sym',20:'Sym',21:'Kanji',22:'Kanji',23:'Done',24:'Done'}
SOURCE_SHA = '8bfdd21c0fc2b63fb9b3041a21c7fdba82f3a071e6375193cdc4e679e165cb2f'
CONFIRM = [(8,8,56,14,'Name'),(8,40,64,14,'Nickname'),(8,72,64,14,'Birthday'),
           (104,72,16,14,'/'),(152,72,16,14,''),(8,104,64,14,'Blood type'),
           (104,104,16,14,''),(104,144,32,14,'Yes'),(152,144,48,14,'No')]
PANELS = {
    0: [(0,0,56,14,'Name:'),(160,0,64,14,'Nickname:')],
    1: [(0,0,56,14,'Birthday:'),(88,0,16,14,'/'),(136,0,16,14,''),
        (176,0,64,14,'Blood type:'),(264,0,16,14,''),
        (16,32,64,14,'Birth month'),(16,80,64,14,'Birth day'),(16,144,64,14,'Blood type')]
       + [(56+40*i,48+16*j,16,14,'') for j in range(2) for i in range(6)]
       + [(56+64*i,160,16,14,'') for i in range(4)],
    2: CONFIRM,
    14: [(8,8,56,14,'Name'),(8,40,64,14,'Birthday'),(104,40,16,14,'/'),
         (152,40,16,14,''),(8,72,64,14,'Blood type'),(104,72,16,14,''),
         (104,144,32,14,'Yes'),(152,144,48,14,'No')],
}


def build(sources, output):
    output = new_output(output)
    manifest = json.loads((sources/'manifest.json').read_text())
    if manifest['metadata']['source_sha256'] != SOURCE_SHA:
        raise ValueError('wrong NAME_ENT source')
    frames = {row['frame_index']:row for row in manifest['images'] if 'frame_index' in row}
    font = NativeLatin(ROOT/'disc/files/SLPM_86.053')
    letters = set(''.join(LABELS.values())+''.join(edit[4] for edits in PANELS.values() for edit in edits))
    glyphs = {}
    for letter in letters-{' '}:
        native = font.glyph(letter)
        points = set(native['points'])
        glyphs[letter] = [''.join('1' if (x,y) in points else '0' for x in range(native['width'])) for y in range(12)]
    plans = []
    for frame,text in LABELS.items():
        source_path = inside(sources,frames[frame]['png'])
        original = Image.open(source_path).convert('RGBA')
        if original.size != (32,16):
            raise ValueError('name button size differs')
        # Original frame border: x2/x30 and y0/y15; text+shadow occupy y2..13.
        interior = original.crop((3,1,30,15))
        colors = [color for color,count in Counter(interior.getdata()).items() if color[3]]
        if len(colors) != 2:
            raise ValueError('name text palette differs')
        foreground,shadow = sorted(colors,key=lambda c:sum(c[:3]),reverse=True)
        width = font.width(text)
        if width > 27:
            raise ValueError('name-entry wording exceeds source button interior')
        x = 3+(27-width)//2; top = 3
        points = []
        for letter in text:
            for y,bits in enumerate(glyphs[letter]):
                points.extend((x+dx,top+y) for dx,bit in enumerate(bits) if bit == '1')
            x += len(glyphs[letter][0])+1
        result = original.copy(); result.paste((0,0,0,0),(3,1,30,15))
        for x,y in points:
            if x+1>=30 or y+1>=15:
                raise ValueError('name button shadow overflow')
            result.putpixel((x+1,y+1),shadow)
        for x,y in points:
            result.putpixel((x,y),foreground)
        plans.append((result,{'id':'name-label-%04d'%frame,'frame':frame,'text':text,
            'source_container_sha256':SOURCE_SHA,'source_png_sha256':digest(source_path.read_bytes()),
            'source_png':frames[frame]['png'],'dimensions':[32,16],
            'output':'frame-%04d.png'%frame,'edit_region':[3,1,27,14],
            'source_ink_baseline':12,'font_origin':[3+(27-width)//2,top],
            'foreground_rgba':foreground,'shadow_rgba':shadow,
            'style':'unreviewed','in_game_verified':False,'runtime_integrated':False}))
    for frame,edits in PANELS.items():
        source_path=inside(sources,frames[frame]['png'])
        original=Image.open(source_path).convert('RGBA');result=original.copy()
        regions=[]
        for left,top,width,height,text in edits:
            crop=original.crop((left,top,left+width,top+height))
            colors=[color for color in Counter(crop.getdata()) if color[3]]
            if not colors and not text:
                continue
            if len(colors)!=2:
                raise ValueError('panel edit region crosses non-text pixels: %s %s' % (frame,(left,top,width,height)))
            foreground,shadow=sorted(colors,key=lambda c:sum(c[:3]),reverse=True)
            points=[(x,y) for y in range(height) for x in range(width) if crop.getpixel((x,y))==foreground]
            ink_bottom=max((y for letter in text if letter!=' ' for x,y in font.glyph(letter)['points']),default=9)
            origin_x=left+min(x for x,y in points);origin_y=top+max(y for x,y in points)-ink_bottom
            result.paste((0,0,0,0),(left,top,left+width,top+height))
            if text and origin_x+font.width(text)>left+width:
                raise ValueError('panel wording overflow: '+text)
            rendered=[];x=origin_x
            for letter in text:
                if letter==' ':
                    x+=5
                    continue
                for y,bits in enumerate(glyphs[letter]):
                    rendered.extend((x+dx,origin_y+y) for dx,bit in enumerate(bits) if bit=='1')
                x+=len(glyphs[letter][0])+1
            for x,y in rendered:
                if not (left<=x and top<=y and x+1<left+width and y+1<top+height):
                    raise ValueError('panel shadow overflow')
                result.putpixel((x+1,y+1),shadow)
            for x,y in rendered:
                result.putpixel((x,y),foreground)
            regions.append({'rect':[left,top,width,height],'text':text,'origin':[origin_x,origin_y]})
        plans.append((result,{'id':'name-panel-%04d'%frame,'frame':frame,
            'source_container_sha256':SOURCE_SHA,'source_png_sha256':digest(source_path.read_bytes()),
            'source_png':frames[frame]['png'],'dimensions':list(original.size),
            'output':'frame-%04d.png'%frame,'edit_regions':regions,
            'style':'unreviewed','in_game_verified':False,'runtime_integrated':False}))
    output.mkdir(parents=True)
    for image,row in plans:
        image.save(output/row['output'])
        row['output_sha256']=digest((output/row['output']).read_bytes())
    (output/'manifest.json').write_bytes(encoded({'schema':'tokimemo.name-translation-drafts.v1',
        'font_provenance':font.provenance(),'assets':[row for _,row in plans],
        'human_approved':False,'runtime_verified':False})+b'\n')
    return {'drafts':len(plans),'output':str(output)}


def restore_sources(output,disc_root):
    from assets.image_packs_name import export_name_ui
    output=new_output(output)
    data=(disc_root/'CDROM/NAME_ENT/NAME_ENT.NBN').read_bytes()
    if digest(data)!=SOURCE_SHA: raise ValueError('wrong original NAME_ENT container')
    result=export_name_ui(data,(disc_root/'SLPM_86.053').read_bytes(),output,'name')
    (output/'manifest.json').write_bytes(encoded(result)+b'\n')
    return {'original_images':len(result['images']),'output':str(output)}


if __name__ == '__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sources',type=Path,required=True)
    parser.add_argument('--output',type=Path)
    parser.add_argument('--restore-sources',action='store_true')
    parser.add_argument('--disc-root',type=Path,default=ROOT/'disc/files')
    args=parser.parse_args()
    if args.restore_sources: print(json.dumps(restore_sources(args.sources,args.disc_root)))
    elif args.output is None: parser.error('--output is required when rendering drafts')
    else: print(json.dumps(build(args.sources,args.output)))
