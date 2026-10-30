"""T-39004: export source-authenticated NAME_ENT actors for actual entry rendering."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import sys
import tempfile
import struct

ROOT=Path(__file__).resolve().parents[2]
sys.path.insert(0,str(ROOT))
from tools.assets.image_packs_name import export_name_ui
from tools.assets.image_packs_name import _mode2_palette
from tools.assets.images import _color, _png
from tools.assets.image_mdec import export_mdec
from tools.assets.image_packs import _parse as parse_main_ui
from tools.assets.input import open_input
from tools.port.school_day_tables import _check_export_path, _assert_no_symlink_path

INPUTS={'name':('CDROM/NAME_ENT/NAME_ENT.NBN','8bfdd21c0fc2b63fb9b3041a21c7fdba82f3a071e6375193cdc4e679e165cb2f'),
        'mainrom':('CDROM/FUKUTEST/MAINROM.NBN','f425e1d19d64afcbafeadece7d72602e50e26ce88093047f73dbb0d21a9b9c1c'),
        'background':('CDROM/NAME_ENT/NAME_SAK.BS','7e9491995df6d5a373d5636c8148b1768bddef3113db49a512c813d6c37874da'),
        'overlay':('CDROM/EXEDIR/NAME_ENT.EXN','f7384ad3617361282638ffd9bc1800bb6655d43ddee37ffe257de2ab485cd4a2'),
        'executable':('SLPM_86.053','bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae')}


def sha(data):
    return hashlib.sha256(data).hexdigest()


def export(input_path, output_path):
    output,_=_check_export_path(output_path)
    output.parent.mkdir(parents=True,exist_ok=True)
    _assert_no_symlink_path(output.parent)
    blobs={}
    with open_input(input_path) as original:
        entries={row['path']:row for row in original.entries()}
        for key,(path,expected) in INPUTS.items():
            blobs[key]=original.read(entries[path])
            if sha(blobs[key])!=expected:
                raise ValueError('NAME_ENT media original hash differs: '+path)
    stage=Path(tempfile.mkdtemp(prefix='.name-media-',dir=output.parent))
    try:
        assets=stage/'assets'
        result=export_name_ui(blobs['name'],blobs['executable'],assets,'name')
        def payload(path,width,height):
            data=(assets/path).read_bytes()
            return {'path':'assets/'+path,'sha256':sha(data),'size':len(data),'width':width,'height':height}
        decoded=export_mdec(blobs['background'],assets,'name-base',
            source_path=INPUTS['background'][0],context={'width':320,'height':240,
            'evidence':['src/ovl/name_entry/name_entry.c:name_entry_load_init reads LBA0xaa18 and decodes320x240']})
        base_image=decoded['images'][0]
        base={'png':payload(base_image['png'],320,240),'stp_png':payload(base_image['stp_png'],320,240),
              'decode':decoded['metadata']}
        read,_,main_frames,_=parse_main_ui(blobs['mainrom'],blobs['executable'])
        cursors={}
        # hud_cursor_actor_init accepts default0 and styles155..173. Square's
        # normal cycle returns from172 to0;173 is still valid on initialization.
        for index in [0,*range(155,174)]:
            frame=main_frames[index]
            left,top,width,height=frame['bounds']
            words=[0]*(width*height)
            parts=[]
            for part in reversed(frame['parts']):
                address=0x800c6020+((part['palette_info']>>8)&15)*512+(part['palette_info']&0xf0)*2
                palette=struct.unpack('<16H',read(address,32))
                palette=tuple(word|0x8000 if word&0x7fff and
                    (0x800c6200<=address<0x800c6220 or 0x800c62c0<=address<0x800c6340) else word for word in palette)
                parts.append(dict(part,palette_address=address))
                for y in range(part['height']):
                    sy=part['height']-1-y if part['flip_flags']&1 else y
                    for x in range(part['width']):
                        sx=part['width']-1-x if part['flip_flags']&2 else x
                        pixel=(part['v']+sy)*128+part['u']+sx
                        byte=blobs['mainrom'][part['sheet_source_offset']+pixel//2]
                        word=palette[(byte>>(4*(pixel%2)))&15]
                        if word:words[(part['y']-top+y)*width+part['x']-left+x]=word
            name=f'cursor-{index}'
            (assets/(name+'.png')).write_bytes(_png(width,height,b''.join(_color(word) for word in words),4))
            (assets/(name+'.stp.png')).write_bytes(_png(width,height,bytes(255 if word&0x8000 else 0 for word in words),1))
            cursors[str(index)]={'png':payload(name+'.png',width,height),'stp_png':payload(name+'.stp.png',width,height),
                'frame':index,'origin':frame['origin'],'bounds':frame['bounds'],'source_parts':parts}
        cursor=cursors['156'] # Preserve the earlier optional-style record for legacy readers.
        sequence_pointer=struct.unpack_from('<I',blobs['name'],0x1afd0)[0]
        sequence=struct.unpack_from('<8h',blobs['name'],sequence_pointer-0x80190000)
        if sequence!=(3,10,3,11,3,12,3,0):raise ValueError('native marker sequence changed')
        frames={}; backgrounds=[]
        for row in result['images']:
            item={'png':payload(row['png'],row['width'],row['height']),
                  'stp_png':payload(row['stp_png'],row['width'],row['height'])}
            if row.get('frame_index') is None:
                item.update({k:row[k] for k in ('variant','runtime_bg_cell','vram_rectangle_words')})
                backgrounds.append(item)
            else:
                item.update({k:row[k] for k in ('frame_index','bounds','origin')})
                frames[str(row['frame_index'])]=item
        # Source digit UV tables belong to NAME_ENT.EXN, not guessed grid crops.
        digits={}
        palette=_mode2_palette(blobs['name'],0)[1][16:32]
        for kind,address,width in [('small',0x8014d014,8),('wide',0x8014d028,16)]:
            for digit in range(10):
                offset=address-0x80132000+digit*2
                u,v=blobs['overlay'][offset:offset+2]
                if u+width>128 or v+16>128:
                    raise ValueError('NAME_ENT digit UV exceeds actor page')
                words=[]
                for y in range(v,v+16):
                    for x in range(u,u+width):
                        byte=blobs['name'][0x200+y*64+x//2]
                        words.append(palette[(byte>>(4*(x%2)))&15])
                name=f'digit-{kind}-{digit}.png'
                (assets/name).write_bytes(_png(width,16,b''.join(_color(word) for word in words),4))
                digits[f'{kind}-{digit}']=payload(name,width,16)
        prompts={}
        for key,address in [('family',0x8014cce4),('given',0x8014cd04),('nickname',0x8014cd10),
                            ('birthday',0x8014b4c0),('confirm',0x8014b510),
                            ('shiori_family',0x8014b56c),('shiori_given',0x8014b574)]:
            offset=address-0x80132000
            raw=blobs['overlay'][offset:offset+128].split(b'\0',1)[0]
            if len(raw)%2: raise ValueError('NAME_ENT heading contains incomplete glyph')
            prompts[key]=raw.hex()
        manifest={'schema':'tokimemo.name-entry-media.v1','frames':frames,'backgrounds':backgrounds,'base_background':base,
            'cursor':cursor,'cursors':cursors,'marker_sequence':{'pointer':sequence_pointer,'steps':[[10,3],[11,3],[12,3]]},
            'digits':digits,'prompts':prompts,
            'inputs':{key:{'path':path,'sha256':expected} for key,(path,expected) in INPUTS.items()},
            'source_proof':{path:sha((ROOT/path).read_bytes()) for path in
                ['src/ovl/name_entry/name_entry.c','tools/assets/image_packs_name.py']},
            'scope':'Original actor composites and uploaded background pages; no native timing or pixel parity claim.'}
        (stage/'manifest.json').write_text(json.dumps(manifest,sort_keys=True,indent=2)+'\n')
        _check_export_path(output)
        os.rename(stage,output)
    except Exception:
        shutil.rmtree(stage,ignore_errors=True)
        raise
    return output


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--input',required=True)
    parser.add_argument('--output',required=True)
    args=parser.parse_args()
    print(export(args.input,args.output))
