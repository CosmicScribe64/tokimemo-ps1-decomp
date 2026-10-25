"""Export bounded original name-entry kana/preset inputs, never translated text.

Requires the reviewed tools.port.new_game_tables adapter. Output is external,
source-pinned data only; no original payload is installed in the Godot project.
"""
import hashlib
import json
import re
import struct
import sys
from pathlib import Path
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/assets/input.py').exists())
sys.path.insert(0,str(ROOT))
from tools.port import new_game_tables as original
from tools.assets.input import open_input
from tools.port.school_day_tables import _check_export_path,_assert_no_symlink_path
import os,shutil,tempfile

ROOT=original.ROOT
BASE=original.BASE
FIELDS={'hiragana':(0x8014CD28,17*4),'katakana':(0x8014CD6C,17*4),
        'preset_family':(0x8014CE6C,15*8),'preset_given':(0x8014CEE4,15*8),
        'preset_nickname':(0x8014CF5C,15*12)}
# Only hashes of extracted payloads are distributed in code.
PAYLOAD_SHA256='7458e8d0ee2d286c17ffe9b0d4201544ac716325273a97e02e5b12ab9fd660af'

def build_bundle(input_path):
    base=original.build_bundle(input_path) # Release, reader/source and opcode proofs.
    symbols=(ROOT/'config/symbols_NAME_ENT.txt').read_text()
    for key,(address,_size)in FIELDS.items():
        symbol='name_entry_'+(key+'_rows' if key in ('hiragana','katakana')else key+'_names' if key!='preset_nickname'else 'preset_nicknames')
        match=re.search(r'^'+re.escape(symbol)+r'\s*=\s*(0x[0-9A-Fa-f]+);',symbols,re.M)
        if not match or int(match.group(1),16)!=address:raise ValueError('name-entry symbol mapping changed: '+symbol)
    with open_input(input_path) as source:
        entries=[e for e in source.entries()if e['path']==original.OVERLAY]
        if len(entries)!=1:raise ValueError('expected exact NAME_ENT overlay')
        overlay=source.read(entries[0])
    if hashlib.sha256(overlay).hexdigest()!=original.OVERLAY_SHA256:raise ValueError('overlay changed')
    data={'space':base['tables']['space']}
    proof={}
    for key,(address,size)in FIELDS.items():
        offset=address-BASE
        raw=overlay[offset:offset+size]
        proof[key]={'address':address,'offset':offset,'size':size,'sha256':hashlib.sha256(raw).hexdigest()}
        if key in ('hiragana','katakana'):
            rows=[];row_proof=[]
            for i in range(17):
                pointer=struct.unpack_from('<I',raw,i*4)[0]
                if not BASE<=pointer<BASE+len(overlay):raise ValueError('kana row pointer outside original overlay')
                start=pointer-BASE;end=overlay.find(b'\0',start,start+33)
                if end<start:raise ValueError('kana row has invalid bounded pair extent')
                row=overlay[start:end]
                if len(row)>11:raise ValueError('kana row exceeds native five-cell string plus possible trailing byte')
                rows.append(list(row));row_proof.append({'address':pointer,'offset':start,'size':len(row),'sha256':hashlib.sha256(row).hexdigest()})
            data[key]=rows;proof[key]['rows']=row_proof
        else:
            width=12 if key=='preset_nickname'else 8
            rows=[list(raw[i*width:(i+1)*width])for i in range(15)]
            if any(row[-2:]!=[0,0]for row in rows):raise ValueError('preset terminator tail differs from native editable width')
            data[key]=rows
    payload=json.dumps(data,sort_keys=True,separators=(',',':')).encode()
    sha=hashlib.sha256(payload).hexdigest()
    if PAYLOAD_SHA256 and sha!=PAYLOAD_SHA256:raise ValueError('entry payload differs from pinned original')
    return {'schema':'tokimemo.new-game-entry.v1','payload_sha256':sha,'content':data,
            'proof':{'overlay_sha256':original.OVERLAY_SHA256,'source_pins':base['proof']['source_pins'],'fields':proof}}

def export(input_path,output_path):
    destination,_=_check_export_path(output_path);bundle=build_bundle(input_path)
    destination.parent.mkdir(parents=True,exist_ok=True);_assert_no_symlink_path(destination.parent)
    stage=Path(tempfile.mkdtemp(prefix='.name-entry-',dir=destination.parent))
    try:
        payload=json.dumps(bundle['content'],sort_keys=True,separators=(',',':')).encode()
        (stage/'entry.json').write_bytes(payload)
        del bundle['content'];bundle['payload']='entry.json'
        (stage/'manifest.json').write_text(json.dumps(bundle,sort_keys=True,indent=2)+'\n')
        _check_export_path(destination);os.rename(stage,destination)
    except Exception:shutil.rmtree(stage,ignore_errors=True);raise
    return destination

if __name__=='__main__':
    import argparse
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--input',required=True);p.add_argument('--output',required=True);a=p.parse_args();print(export(a.input,a.output))
