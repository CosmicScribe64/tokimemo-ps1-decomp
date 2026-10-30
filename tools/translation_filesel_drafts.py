#!/usr/bin/env python3
"""T-39004: save-screen panel/badge drafts from source-verified frame identities.

`pages` (Phase 4 WP-L) maps the T-39011 index-space information-page drafts
(tools/translation_filesel_indexed.py) to v2 `page` rows: each row replaces
only the page rects of the label regions that page actually edited
(translations/en/graphics/filesel_page_regions.json), and every pixel outside
those rects must equal the exported source page.
"""
import argparse
import json
from pathlib import Path
import shutil

import numpy as np
from PIL import Image

from translation_assets import ROOT,check_regions,digest,encoded,inside,new_output,rgba_sha256
from translation_native_font import NativeLatin

SOURCE_SHA='1a0e42ade6320df564cdb1017f73ba05083394c1974d3bc7a4af21618725e2a6'
INDEXED_SCHEMA='tokimemo.filesel-indexed-drafts.v1'
PAGE_SCHEMA='tokimemo.filesel-page-drafts.v1'
REGIONS=ROOT/'translations/en/graphics/filesel_page_regions.json'
INK=(16,16,16,255)
PAPER=(231,231,231,255)
BLUE=(0,57,132,255)


def build(sources,output):
    output=new_output(output)
    source=json.loads((sources/'manifest.json').read_text())
    if source['metadata']['source_sha256']!=SOURCE_SHA: raise ValueError('FILESEL source differs')
    font=NativeLatin(ROOT/'disc/files/SLPM_86.053')
    plans=[]
    for row in source['images']:
        frame=row.get('frame_index')
        if frame not in [19,20,21,22,30,31]: continue
        path=inside(sources,row['png']);original=Image.open(path).convert('RGBA');result=original.copy()
        edits=[]
        def label(rect,text,ink,background):
            x,y,width,height=rect
            glyphs=[font.glyph(character) for character in text]
            gap=1
            text_width=sum(glyph['width'] for glyph in glyphs)+len(glyphs)-1
            if text_width>width:
                gap=0
                text_width=sum(glyph['width'] for glyph in glyphs)
            if text_width>width: raise ValueError('FILESEL label overflows: '+text)
            top=min(dy for glyph in glyphs for dx,dy in glyph['points'])
            bottom=max(dy for glyph in glyphs for dx,dy in glyph['points'])
            if bottom-top+1>height: raise ValueError('FILESEL glyph exceeds source rows')
            # Only the known text color is removed; preserve borders/other ink.
            for yy in range(y,y+height):
                for xx in range(x,x+width):
                    if result.getpixel((xx,yy))==ink: result.putpixel((xx,yy),background)
            points=[]
            cursor=x+(width-text_width)//2
            baseline=y+(height-(bottom-top+1))//2-top
            for glyph in glyphs:
                points.extend((cursor+dx,baseline+dy) for dx,dy in glyph['points'])
                cursor+=glyph['width']+gap
            for point in points:
                if result.getpixel(point)!=background: raise ValueError('replacement touches non-text decoration')
                result.putpixel(point,ink)
            edits.append({'rect':rect,'text':text,'letter_gap':gap})
        if frame==19:
            label([16,4,80,14],'Stats',BLUE,(0,0,0,0))
            for index,text in enumerate(['Health','Lit.','Sci.','Art']): label([5,24+index*16,43,11],text,INK,PAPER)
            for index,text in enumerate(['Sport','Trivia','Looks','Guts','Stress']): label([110,7 if index==0 else 8+index*16,44,11],text,INK,PAPER)
        elif frame==20:
            for left,width,labels,first in [(1,29,['Shiori','Mio','Yuina','Ayako'],24),
                                           (64,29,['Saki','Yukari','Nozomi','Mira'],24),
                                           (127,43,['Rei','Yuko','Megumi','Yumi','Miharu'],7)]:
                for index,text in enumerate(labels):
                    top=first+index*16+(1 if first==7 and index>0 else 0)
                    label([left,top,width,11],text,INK,PAPER)
        elif frame in [21,22]:
            label([0,0,96,16],'Clears' if frame==21 else 'Ratings',BLUE,(0,0,0,0))
        else:
            selected=row['variant'].endswith('.selected')
            ink=(255,255,255,255) if selected else (123,123,123,255)
            background=((181,0,66,255) if frame==30 else (115,74,189,255)) if selected else (255,255,255,255)
            label([1 if frame==30 else 0,7,12,11],'Y' if frame==30 else 'N',ink,background)
        if not set(result.getdata())<=set(original.getdata()): raise ValueError('FILESEL palette changed')
        plans.append((result,{'id':'filesel-'+row['variant'],'frame':frame,'variant':row['variant'],
            'source_png':row['png'],'source_png_sha256':digest(path.read_bytes()),'source_container_sha256':SOURCE_SHA,
            'output':row['variant']+'.png','dimensions':list(original.size),'edit_regions':edits,
            'style':'unreviewed','runtime_integrated':False,'in_game_verified':False}))
    output.mkdir(parents=True)
    for image,row in plans:
        image.save(output/row['output']);row['output_sha256']=digest((output/row['output']).read_bytes())
    (output/'manifest.json').write_bytes(encoded({'schema':'tokimemo.filesel-translation-drafts.v1',
        'assets':[row for image,row in plans],'font_provenance':font.provenance(),'human_approved':False})+b'\n')
    return {'output':str(output),'drafts':len(plans)}


def pages(indexed,sources,output,regions_path=REGIONS):
    """v2 page rows for the information pages of one index-space draft build."""
    regions_bytes=Path(regions_path).read_bytes();regions=json.loads(regions_bytes)
    manifest=json.loads((indexed/'manifest.json').read_text())
    if manifest.get('schema')!=INDEXED_SCHEMA or manifest.get('source_container_sha256')!=SOURCE_SHA or regions.get('container_sha256')!=SOURCE_SHA:
        raise ValueError('FILESEL page draft source differs')
    if manifest.get('page_regions_sha256')!=digest(regions_bytes):
        raise ValueError('page drafts were built from another region map')
    exported=json.loads((sources/'manifest.json').read_text())
    if exported['metadata']['source_sha256']!=SOURCE_SHA: raise ValueError('FILESEL source differs')
    by_id={row['id']:row for row in regions['regions']}
    rows,skipped,copies=[],[],[]
    for asset in manifest['assets']:
        variant=asset.get('variant','')
        if not variant.startswith('source-page-information-'): continue
        page=int(variant.rsplit('-',1)[1])
        english=indexed/asset['output']
        if not english.is_file():
            skipped.append({'id':asset['id'],'reason':'English page not in this build'});continue
        if digest(english.read_bytes())!=asset['output_sha256']: raise ValueError('page draft bytes changed: '+asset['id'])
        source_path=inside(sources,asset['source_png'])
        if digest(source_path.read_bytes())!=asset['source_png_sha256']: raise ValueError('source page differs: '+asset['id'])
        rects=[rect['rect'] for name in sorted(asset['edited_regions']) if by_id[name]['status']=='candidate'
               for rect in by_id[name]['page_rects'] if rect['page']==page]
        if not rects:
            skipped.append({'id':asset['id'],'reason':'no candidate region on this page'});continue
        source=np.asarray(Image.open(source_path).convert('RGBA'));result=np.asarray(Image.open(english).convert('RGBA'))
        if source.shape!=result.shape: raise ValueError('page draft size differs: '+asset['id'])
        check_regions(rects,[source.shape[1],source.shape[0]])
        outside=np.ones(source.shape[:2],bool)
        for x,y,w,h in rects: outside[y:y+h,x:x+w]=False
        if ((source!=result).any(-1)&outside).any():
            raise ValueError('page draft changes pixels outside its label regions: '+asset['id'])
        rows.append({'id':asset['id'],'kind':'page','source_container':'CDROM/FUKUTEST/FILESEL.NBN',
            'source_container_sha256':SOURCE_SHA,'page':page,'page_variant':variant,
            'source_png':asset['source_png'],'source_png_sha256':asset['source_png_sha256'],
            'source_rgba_sha256':rgba_sha256(Image.open(source_path)),
            'regions':rects,'region_ids':sorted(asset['edited_regions']),
            'source_regions_remaining':asset.get('source_regions_remaining',[]),
            'dimensions':[int(source.shape[1]),int(source.shape[0])],'output':asset['output'],'output_sha256':asset['output_sha256'],
            'revision':manifest.get('revision'),'style':'unreviewed','human_approved':False,
            'runtime_integrated':False,'in_game_verified':False})
        copies.append((english,source_path,asset))
    output=new_output(output);output.mkdir(parents=True)
    try:
        for english,source_path,asset in copies:
            shutil.copyfile(english,output/asset['output']);shutil.copyfile(source_path,output/asset['source_png'])
        (output/'manifest.json').write_bytes(encoded({'schema':PAGE_SCHEMA,'source_manifest_sha256':digest((indexed/'manifest.json').read_bytes()),
            'page_regions_sha256':digest(regions_bytes),'assets':rows,'skipped':skipped,'human_approved':False})+b'\n')
    except Exception:
        shutil.rmtree(output);raise
    return {'output':str(output),'pages':len(rows),'skipped':len(skipped)}


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('operation',nargs='?',choices=['frames','pages'],default='frames')
    parser.add_argument('--sources',type=Path,required=True,help='original-filesel export directory')
    parser.add_argument('--indexed',type=Path,help='pages: T-39011 filesel-indexed draft directory')
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args()
    if args.operation=='pages':
        if args.indexed is None: parser.error('pages needs --indexed')
        print(json.dumps(pages(args.indexed,args.sources,args.output)))
    else: print(json.dumps(build(args.sources,args.output)))
