#!/usr/bin/env python3
"""T-39004: source-font sports card drafts from a local reviewed translation plan.

The plan contains original source identity, English wording and source-bound
paper regions. Original text and generated game-derived images stay ignored.
"""
import argparse
import json
from pathlib import Path

from PIL import Image

from translation_assets import ROOT,digest,encoded,inside,new_output
from translation_native_font import NativeLatin


def build(plan_path,executable,output):
    plan_bytes=plan_path.read_bytes();plan=json.loads(plan_bytes)
    output=new_output(output);font=NativeLatin(executable);prepared=[]
    for card in plan['cards']:
        source=inside(ROOT,card['source'])
        if digest(source.read_bytes())!=card['source_png_sha256']: raise ValueError('sports source changed')
        original=Image.open(source).convert('RGBA');result=original.copy()
        if original.size!=(320,240): raise ValueError('sports viewport differs')
        paper=tuple(card['paper_rgba']);ink=tuple(card['ink_rgba']);red=tuple(card['emphasis_rgba'])
        colors=set(original.getdata())
        if not {paper,ink,red}<=colors: raise ValueError('sports palette differs')
        layers=[]
        for section in ['heading','body']:
            region=card[section]['region'];left,top,width,height=region
            if left<0 or top<0 or left+width>320 or top+height>240: raise ValueError('sports edit leaves viewport')
            rows=[];row=''
            for word in card[section]['text'].split():
                candidate=(row+' '+word).strip()
                if font.width(candidate)>width:
                    if not row or font.width(word)>width: raise ValueError('sports word exceeds source paper')
                    rows.append(row);row=word
                else: row=candidate
            if row: rows.append(row)
            if (len(rows)-1)*16+12>height: raise ValueError('sports translation exceeds source text rows')
            result.paste(paper,(left,top,left+width,top+height))
            for row_number,text in enumerate(rows):
                x=left+(width-font.width(text))//2 if section=='heading' else left
                for word_number,word in enumerate(text.split(' ')):
                    if word_number: x+=font.glyph(' ')['width']+1
                    color=red if word.strip('.,!?').lower() in card.get('emphasis_words',[]) else ink
                    for character in word:
                        glyph=font.glyph(character)
                        for dx,dy in glyph['points']: result.putpixel((x+dx,top+row_number*16+dy),color)
                        x+=glyph['width']+1
            layers.append({'section':section,'region':region,'rows':rows})
        if not set(result.getdata())<=colors: raise ValueError('sports draft invents palette colors')
        record={key:card[key] for key in ['id','source','source_png_sha256','source_container_sha256','frame']}
        record.update({'output':card['id']+'.png','dimensions':[320,240],'layers':layers,
            'style':'unreviewed','runtime_integrated':False,'in_game_verified':False})
        prepared.append((result,record))
    output.mkdir(parents=True)
    for image,row in prepared:
        image.save(output/row['output']);row['output_sha256']=digest((output/row['output']).read_bytes())
    (output/'manifest.json').write_bytes(encoded({'schema':'tokimemo.sports-translation-drafts.v1',
        'plan_sha256':digest(plan_bytes),'font_provenance':font.provenance(),'assets':[row for image,row in prepared]})+b'\n')
    return {'drafts':len(prepared),'output':str(output)}


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--plan',type=Path,required=True)
    parser.add_argument('--executable',type=Path,required=True)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args();print(json.dumps(build(args.plan,args.executable,args.output)))
