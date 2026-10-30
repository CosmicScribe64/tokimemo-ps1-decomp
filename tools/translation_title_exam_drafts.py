#!/usr/bin/env python3
"""T-39004: native-size title/exam drafts; preserve non-text source decoration."""
import argparse
from collections import Counter
import json
from pathlib import Path

from PIL import Image

from translation_assets import ROOT, digest, encoded, inside, new_output
from translation_option_drafts import recover_font, text_width


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


def build(sources,kind,output):
    output=new_output(output)
    source=json.loads((sources/'manifest.json').read_text())
    expected={'title':'7116859fbb555b3e7b33dabbcd1bd2afc10862d4496e26156fb5f442f8b03206',
              'exam':'f990c8a8eb5cc547204102d1e25fc2d5fda5fa18ed076bc543a77eb76e0dc37a'}[kind]
    if source['metadata']['source_sha256']!=expected: raise ValueError('original source differs')
    rows={row['frame_index']:row for row in source['images'] if row.get('frame_index') is not None}
    glyphs,proof=recover_font(ROOT/'translations/en/graphics/artwork')
    plans=[]
    for frame,text in ({1:'GAME START',2:'GAME START',3:'OPTIONS',4:'OPTIONS'} if kind=='title' else {0:'Exam Results',1:''}).items():
        row=rows[frame];path=inside(sources,row['png'])
        original=Image.open(path).convert('RGBA');result=original.copy()
        protected=set()
        if kind=='title':
            selected=frame in (1,3)
            span=112 if frame in (1,2) else 80
            left=(8 if selected else 0)+(span-text_width(text,glyphs))//2
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
            points=lettering(text,glyphs,left,2)
            for x,y in points:
                if (x+1,y+1) not in protected: result.putpixel((x+1,y+1),shadow)
            for x,y in points:
                if (x,y) not in protected: result.putpixel((x,y),gradient[y-2])
        elif frame==0:
            # Exact composed source pixels: the older catalog's x20/y56 is wrong.
            region=[104,7,112,10]
            result.paste((0,0,0,0),(104,7,216,17))
            left=104+(112-text_width(text,glyphs))//2
            points=lettering(text,glyphs,left,7)
            for x,y in points: result.putpixel((x+1,y+1),(41,41,41,255))
            for x,y in points: result.putpixel((x,y),(165,206,206,255))
        else:
            region=[0,0,8,16]
            result.paste((0,0,0,0),(0,0,8,16))
        if not set(result.getdata())<=set(original.getdata()): raise ValueError('new color outside native palette')
        if any(result.getpixel(point)!=original.getpixel(point) for point in protected): raise ValueError('title selection underline changed')
        plans.append((result,{'id':f'{kind}-{frame:04d}','frame':frame,'text':text,
            'source_png':row['png'],'source_png_sha256':digest(path.read_bytes()),
            'source_container_sha256':expected,'dimensions':list(original.size),
            'output':f'frame-{frame:04d}.png','edit_region':region,'protected_decoration_pixels':len(protected),
            'style':'unreviewed','in_game_verified':False,'runtime_integrated':False}))
    output.mkdir(parents=True)
    for image,row in plans:
        image.save(output/row['output']);row['output_sha256']=digest((output/row['output']).read_bytes())
    (output/'manifest.json').write_bytes(encoded({'schema':f'tokimemo.{kind}-translation-drafts.v1',
        'font_provenance':proof,'assets':[row for image,row in plans],'human_approved':False})+b'\n')
    return {'output':str(output),'drafts':len(plans)}


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sources',type=Path,required=True)
    parser.add_argument('--kind',choices=['title','exam'],required=True)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args()
    print(json.dumps(build(args.sources,args.kind,args.output)))
