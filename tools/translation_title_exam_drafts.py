#!/usr/bin/env python3
"""T-39004: native-size title/exam drafts; preserve non-text source decoration.

Title lettering uses the unscaled original-font masks (tools/translation_native_font.py,
Phase 4 WP-L) by default; the earlier `menu9` masks recovered from retained artwork
were visibly too small against the original title lettering. Exam keeps `menu9`
unless `--font native` is given.
"""
import argparse
from collections import Counter
import json
from pathlib import Path

from PIL import Image

from translation_assets import ROOT, digest, encoded, inside, new_output
from translation_native_font import NativeLatin
from translation_option_drafts import recover_font, text_width

DEFAULT_FONT={'title':'native','exam':'menu9'}


def lettering(text,glyphs,left,top):
    points=[]
    for letter in text:
        if letter==' ':
            left+=3
            continue
        for y,bits in enumerate(glyphs[letter]):
            points.extend((left+x,top+y) for x,bit in enumerate(bits) if bit=='1')
        left+=len(glyphs[letter][0])+1
    return points


class Menu9:
    """Glyph masks recovered from retained artwork (rows 0..8)."""
    top=0
    def __init__(self):
        self.glyphs,self.proof=recover_font(ROOT/'translations/en/graphics/artwork')
    def width(self,text): return text_width(text,self.glyphs)
    def points(self,text,left,top): return lettering(text,self.glyphs,left,top)
    def provenance(self): return self.proof


class Native:
    """Unscaled original 12x12 masks; capitals occupy glyph rows 1..9."""
    top=1
    def __init__(self):
        self.font=NativeLatin(ROOT/'disc/files/SLPM_86.053')
    def width(self,text): return self.font.width(text)
    def points(self,text,left,top):
        points=[]
        for letter in text:
            glyph=self.font.glyph(letter)
            points.extend((left+x,top+y) for x,y in glyph['points'])
            left+=glyph['width']+1
        return points
    def provenance(self): return self.font.provenance()


def build(sources,kind,output,font=None):
    font=font or DEFAULT_FONT[kind]
    lettering_font=Native() if font=='native' else Menu9()
    output=new_output(output)
    source=json.loads((sources/'manifest.json').read_text())
    expected={'title':'7116859fbb555b3e7b33dabbcd1bd2afc10862d4496e26156fb5f442f8b03206',
              'exam':'f990c8a8eb5cc547204102d1e25fc2d5fda5fa18ed076bc543a77eb76e0dc37a'}[kind]
    if source['metadata']['source_sha256']!=expected: raise ValueError('original source differs')
    rows={row['frame_index']:row for row in source['images'] if row.get('frame_index') is not None}
    plans=[]
    for frame,text in ({1:'GAME START',2:'GAME START',3:'OPTIONS',4:'OPTIONS'} if kind=='title' else {0:'Exam Results',1:''}).items():
        row=rows[frame];path=inside(sources,row['png'])
        original=Image.open(path).convert('RGBA');result=original.copy()
        protected=set()
        if kind=='title':
            selected=frame in (1,3)
            span=112 if frame in (1,2) else 80
            left=(8 if selected else 0)+(span-lettering_font.width(text))//2
            region=[0,0,original.width,original.height]
            shadow=(0,0,115,255) if selected else (24,24,24,255)
            gradient=[]
            for y in range(2,11):
                colors=Counter(original.getpixel((x,y)) for x in range(original.width)
                    if original.getpixel((x,y))[3] and original.getpixel((x,y))[2]>=original.getpixel((x,y))[0]
                    and original.getpixel((x,y))!=shadow)
                if not colors and not (selected and y==10): raise ValueError('title gradient row has no source ink')
                # The selected underline occludes the final text row in source.
                gradient.append(colors.most_common(1)[0][0] if colors else gradient[-1])
            for y in range(original.height):
                for x in range(original.width):
                    color=original.getpixel((x,y))
                    if selected and color[3] and color[0]>color[2]: protected.add((x,y))
                    else: result.putpixel((x,y),(0,0,0,0))
            points=lettering_font.points(text,left,2-lettering_font.top)
            if any(not 2<=y<=10 or not 0<=x<original.width-1 for x,y in points): raise ValueError('title lettering leaves the source text rows')
            for x,y in points:
                if (x+1,y+1) not in protected: result.putpixel((x+1,y+1),shadow)
            for x,y in points:
                if (x,y) not in protected: result.putpixel((x,y),gradient[y-2])
        elif frame==0:
            # Exact composed source pixels: the older catalog's x20/y56 is wrong.
            region=[104,7,112,10]
            result.paste((0,0,0,0),(104,7,216,17))
            left=104+(112-lettering_font.width(text))//2
            points=lettering_font.points(text,left,7-lettering_font.top)
            if any(not (104<=x<215 and 7<=y<16) for x,y in points): raise ValueError('exam lettering leaves its region')
            for x,y in points: result.putpixel((x+1,y+1),(41,41,41,255))
            for x,y in points: result.putpixel((x,y),(165,206,206,255))
        else:
            region=[0,0,8,16]
            result.paste((0,0,0,0),(0,0,8,16))
        if not set(result.getdata())<=set(original.getdata()): raise ValueError('new color outside native palette')
        if any(result.getpixel(point)!=original.getpixel(point) for point in protected): raise ValueError('title selection underline changed')
        plans.append((result,{'id':f'{kind}-{frame:04d}','frame':frame,'text':text,
            'source_png':row['png'],'source_png_sha256':digest(path.read_bytes()),
            'source_container_sha256':expected,'dimensions':list(original.size),'font':font,
            'output':f'frame-{frame:04d}.png','edit_region':region,'protected_decoration_pixels':len(protected),
            'style':'unreviewed','in_game_verified':False,'runtime_integrated':False}))
    output.mkdir(parents=True)
    for image,row in plans:
        image.save(output/row['output']);row['output_sha256']=digest((output/row['output']).read_bytes())
    (output/'manifest.json').write_bytes(encoded({'schema':f'tokimemo.{kind}-translation-drafts.v1',
        'font':font,'font_provenance':lettering_font.provenance(),'assets':[row for image,row in plans],'human_approved':False})+b'\n')
    return {'output':str(output),'drafts':len(plans)}


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sources',type=Path,required=True)
    parser.add_argument('--kind',choices=['title','exam'],required=True)
    parser.add_argument('--output',type=Path,required=True)
    parser.add_argument('--font',choices=['native','menu9'],help='lettering masks (default: native for title, menu9 for exam)')
    args=parser.parse_args()
    print(json.dumps(build(args.sources,args.kind,args.output,args.font)))
