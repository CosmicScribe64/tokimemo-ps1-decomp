"""Read source-pinned, headerless NAME_ENT tables from verified original input.

Production output belongs only in ignored .asset-cache/port; no original payload
is distributed with this adapter. Run inside the existing project Docker image.
"""
import hashlib
import json
import os
import re
import shutil
import sys
import tempfile
from pathlib import Path
ROOT = next(p for p in Path(__file__).resolve().parents if (p/'tools/assets/input.py').exists())
sys.path.insert(0, str(ROOT))
from tools.assets.input import open_input
from tools.port.school_day_tables import _reader_executable, parse_psx_exe, _check_export_path, _assert_no_symlink_path

OVERLAY = 'CDROM/EXEDIR/NAME_ENT.EXN'
OVERLAY_SHA256 = 'f7384ad3617361282638ffd9bc1800bb6655d43ddee37ffe257de2ab485cd4a2'
BASE = 0x80132000
TABLES = {'compatibility': (0x8014D03C,16), 'initial_scores': (0x8014D04C,20)}
SOURCE_PINS = {'tools/assets/input.py': 'e876bd062eb367f636d9703935305993d6e6cc2f9a9800c84571015b351dcd52', 'tools/port/school_day_tables.py': '6e97c0dae816c3202cc2f2946287262901c853c0cec1cccab2acef267c557550', 'src/main/game_init.c': 'ecd91c6bc45e2670ce332b5d53fb9e5fe2cfb767bb9e0c46e09f3741595bed53', 'src/ovl/name_entry/name_entry.c': '840feed25a5b04687563e45a67eac1998fca9b050cdd59f80ad3a5f6e52cd35d', 'src/main/overlay_loader.c': '71c1c1db9d0aaf13e85b99f9f420647e6623135e3c3974fbfef77e6114a029a8', 'config/overlays/NAME_ENT.yaml': '844d84467d937cd6a6af8cba884c4dbafde062ba898671f20ceadd7f8783809a', 'config/symbols_NAME_ENT.txt': 'bf7df8a2e4db9fe79899b4b65f5bfa5d685e397480edb9b6bfe4446d09e6090b', 'include/game/game_state.h': 'e58484d902e1c47696a3ae5086fa93626bebc107ab83cb0d8dc91cc0a3e4a6af', 'include/game/girl.h': '82c7a558ed145d60e82d57f3a83d331d7fee53a9b89a2edd7c56c284e5c09c58', 'include/game/word_views.h': '8a21797a38272607105b2ceef422c337e928f0b0d066aff5c81dfc6ec8bb0498', 'include/game_enums.h': 'cf35db6941db5bdae992b0228040a77336dce7a2f951d2427b7e7a71cd204987'} # Filled once from the reviewed source checkpoint; changes fail closed.
PAYLOAD_PINS = {'compatibility': '15f30eae0f0c6ee69a5c9545421e46b9462f9326d503b8d12a6d9fbded1fe6e4', 'initial_scores': '9c19e50ed151d648396d4f7eb81df29aa1730a38ab152b5ce704b85b7925048f', 'space': '5845a3f69ea59cfc12219a5272b8be0b9f9bb60648ed002627e6637041b1bff3', 'cheat_names': '1bb4c75cceaa83089bdb6cfde8b29ec3684c54360663b8c17af6832a53ede05b', 'girl_birthdays': '39a0cacfc6eba63c61497c2f8c12778d495cdc38a776fea6a53fde5404602f94', 'girl_bloods': 'cc5df113b389158e8294fffb81104648bb6aee44d332b4107462786dbc8df240', 'girl_fixed_clubs': 'e28e3af87748164d486c21cd6841e9edec9359dd9eeff8517bc1297b287ad5e7'}

def digest(data):
    return hashlib.sha256(data).hexdigest()

def source_proof():
    for relative, expected in SOURCE_PINS.items():
        if digest((ROOT/relative).read_bytes()) != expected:
            raise ValueError('New Game source pin changed: '+relative)
    return dict(SOURCE_PINS)

def validate(bundle):
    if not isinstance(bundle,dict) or set(bundle) != {'schema','tables','proof'} or bundle['schema'] != 'tokimemo.new-game-tables.v1':
        raise ValueError('invalid New Game table contract')
    if set(bundle['tables']) != {'compatibility','initial_scores','space','cheat_names','girl_birthdays','girl_bloods','girl_fixed_clubs'}:
        raise ValueError('incomplete New Game table contract')
    for key in ('compatibility','initial_scores','space','cheat_names','girl_birthdays','girl_bloods','girl_fixed_clubs'):
        values = bundle['tables'][key]
        if not isinstance(values,list): raise ValueError('invalid table shape')
        rows = values if key == 'cheat_names' else [values]
        if key == 'cheat_names' and len(rows) != 9: raise ValueError('invalid cheat name count')
        raw = bytearray()
        for row in rows:
            if not isinstance(row,list) or any(type(v) is not int or not 0 <= v <= 255 for v in row):
                raise ValueError('invalid byte table')
            raw.extend(row)
            if key == 'cheat_names': raw.append(0)
        if digest(raw) != PAYLOAD_PINS[key]: raise ValueError('original table digest differs: '+key)
    if not isinstance(bundle['proof'], dict) or bundle['proof'].get('source_pins') != SOURCE_PINS or bundle['proof'].get('overlay_sha256') != OVERLAY_SHA256:
        raise ValueError('original table provenance differs')
    return bundle

def _field_stores(blob, base, start, end):
    """Decode bounded linear bitfield assignment dataflow, never execute code.

    A load introduces a symbolic (address, mask, value) expression. Only
    LUI/ADDIU, LBU/LHU, ANDI/ORI and stores may contribute to a witness; unknown
    instructions invalidate destinations. Branches are not traversed: callers
    require one unique load-mask-store witness at every expected field address.
    """
    regs = [None] * 32
    regs[0] = ('constant', 0)
    stores = []
    pending_call = 0
    for pc in range(start,end,4):
        word = int.from_bytes(blob[pc-base:pc-base+4],'little')
        op, rs, rt = word>>26, (word>>21)&31, (word>>16)&31
        imm = word&0xffff
        signed = imm-0x10000 if imm&0x8000 else imm
        left = regs[rs]
        if op == 15:
            regs[rt] = ('constant',imm<<16)
        elif op == 9:
            regs[rt] = ('constant',(left[1]+signed)&0xffffffff) if left and left[0]=='constant' else None
        elif op in (36,37):
            regs[rt] = ('field',(left[1]+signed)&0xffffffff,0xff if op==36 else 0xffff,0,pc) if left and left[0]=='constant' else None
        elif op in (12,13):
            if not left: regs[rt] = None
            elif left[0]=='constant':
                regs[rt] = ('constant',left[1]&imm if op==12 else left[1]|imm)
            elif op==12: regs[rt] = ('field',left[1],left[2]&imm,left[3]&imm,left[4])
            else: regs[rt] = ('field',left[1],left[2]&~imm,left[3]|imm,left[4])
        elif op in (40,41):
            value=regs[rt]
            if left and left[0]=='constant' and value and value[0]=='field':
                address=(left[1]+signed)&0xffffffff
                if address==value[1]:
                    stores.append({'address':address,'mask':value[2],'value':value[3],'load_pc':value[4],'store_pc':pc,'width':1 if op==40 else 2})
        elif op==0:
            rd=(word>>11)&31
            if word&63 in (33,37) and rt==0: regs[rd]=left
            elif word&63 not in (8,9): regs[rd]=None
        elif op in (2,3,4,5,6,7):
            if op==3: pending_call=2
        elif op not in (40,41,43): regs[rt]=None
        regs[0]=('constant',0)
        if pending_call:
            pending_call-=1
            if not pending_call:
                for register in [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,24,25,31]:regs[register]=None
    return stores


def _decoded_character_tables(exe,overlay):
    main=(ROOT/'src/main/game_init.c').read_text()
    setup=(ROOT/'src/ovl/name_entry/name_entry.c').read_text().split('void name_entry_new_game_setup(void) {',1)[1].split('void name_entry_cheat_names_apply',1)[0]
    birthdays={}
    for girl,field,value in re.findall(r'g->girls\[(\d+)\]\.birthday\.date\.(month|day) = (\d+);',main):
        key=(int(girl),field)
        if key in birthdays: raise ValueError('duplicate birthday source assignment')
        birthdays[key]=int(value)
    bloods={}
    for girl,value in re.findall(r'game_state\.girls\[(\d+)\]\.birthday\.date\.blood_type = (\d+);',setup):
        if int(girl) in bloods: raise ValueError('duplicate blood source assignment')
        bloods[int(girl)]=int(value)
    if set(birthdays)!={(i,f)for i in range(14)for f in ('month','day')} or set(bloods)!=set(range(1,13)):
        raise ValueError('character source assignment coverage differs')
    birthday_stores=_field_stores(exe[0x800:],0x80041000,0x80041d04,0x80041f48)
    blood_stores=_field_stores(overlay,BASE,0x801390c8,0x80139440)
    clubs = {}
    enum_text=(ROOT/'include/game_enums.h').read_text()
    for girl,name in re.findall(r'game_state\.girls\[(\d+)\]\.flags\.f\.club = (CLUB_[A-Z_]+);',setup):
        if 5 <= int(girl) <= 11:
            value=re.search(r'\b'+re.escape(name)+r'\s*=\s*(\d+)',enum_text)
            if not value: raise ValueError('club enum missing')
            clubs[int(girl)]=int(value.group(1))
    if set(clubs)!=set(range(5,12)): raise ValueError('fixed club assignment coverage differs')
    proof=[]
    def witness(stores,address,mask,value,width,label):
        found=[r for r in stores if r['address']==address and r['mask']==mask and r['value']==value and r['width']==width]
        if len(found)!=1: raise ValueError('no unique original bitfield-store witness: '+label)
        proof.append(dict(found[0],source_assignment=label))
    for i in range(14):
        address=0x800e6280+0x1cc+i*0x38
        witness(birthday_stores,address,0xf0,birthdays[i,'month'],1,f'girl[{i}].month')
        witness(birthday_stores,address,0xfe0f,birthdays[i,'day']<<4,2,f'girl[{i}].day')
    for i in range(1,13):
        witness(blood_stores,0x800e6280+0x1cc+i*0x38+1,0xf9,bloods[i]<<1,1,f'girl[{i}].blood_type')
    for i in range(5,12):
        witness(blood_stores,0x800e6280+0x1ca+i*0x38,0xf,clubs[i]<<4,1,f'girl[{i}].club')
    return [birthdays[i,f]for i in range(14)for f in ('month','day')],[bloods[i]for i in range(1,13)],[clubs[i]for i in range(5,12)],proof


def build_bundle(input_path):
    pins = source_proof()
    exe, exe_input, exe_entry = _reader_executable(input_path)
    parse_psx_exe(exe)
    with open_input(input_path) as source:
        matches = [e for e in source.entries() if e['path'] == OVERLAY]
        if len(matches) != 1: raise ValueError('expected exact '+OVERLAY)
        entry = matches[0]
        overlay = source.read(entry)
        info = source.describe()
    if len(overlay) != 0x30000 or digest(overlay) != OVERLAY_SHA256:
        raise ValueError('overlay is not verified PlayStation the Best NAME_ENT')
    # Headerless mapping is independently established by loader LBA 0x95EA and
    # splat start 0 / vram 0x80132000. Exact source hashes pin both observations.
    if info['format'] != 'directory' and entry['location']['lba'] != 0x95ea:
        raise ValueError('NAME_ENT ISO LBA differs from original loader')
    tables = {key:list(overlay[address-BASE:address-BASE+size]) for key,(address,size) in TABLES.items()}
    tables['space'] = list(exe[0x800+0x800b3644-0x80041000:0x800+0x800b3644-0x80041000+2])
    body = (ROOT/'src/ovl/name_entry/name_entry.c').read_text().split('void name_entry_cheat_names_apply(void) {',1)[1].split('\n}\n',1)[0]
    literals = re.findall(r'"([^"\n]*)"',body)
    if len(literals) != 9: raise ValueError('cheat source contract differs')
    tables['cheat_names'] = []
    offsets = []
    for literal in literals:
        raw = literal.encode('shift_jis')
        offset = overlay.find(raw+b'\0',0x19140,0x19660)
        if offset < 0: raise ValueError('cheat source has no original rodata witness')
        offsets.append(offset)
        tables['cheat_names'].append(list(raw))
    tables['girl_birthdays'],tables['girl_bloods'],tables['girl_fixed_clubs'],character_proof = _decoded_character_tables(exe,overlay)
    bundle = {'schema':'tokimemo.new-game-tables.v1','tables':tables,'proof':{
        'source_pins':pins,'overlay_sha256':digest(overlay),'overlay_size':len(overlay),
        'overlay_entry':entry,'input':info,'exe_sha256':digest(exe),
        'mapping':{'header_bytes':0,'base':BASE,'loader_lba':0x95ea,'loader_sectors':0x60},
        'tables':{key:{'address':address,'offset':address-BASE,'size':size} for key,(address,size) in TABLES.items()},
        'cheat_offsets':offsets,'character_instruction_witnesses':character_proof}}
    return validate(bundle)

def export(input_path, output_path):
    destination,_ = _check_export_path(output_path)
    bundle = build_bundle(input_path)
    _check_export_path(destination)
    destination.parent.mkdir(parents=True,exist_ok=True)
    _assert_no_symlink_path(destination.parent)
    stage=Path(tempfile.mkdtemp(prefix='.new-game-tables-',dir=destination.parent))
    try:
        (stage/'manifest.json').write_text(json.dumps(bundle,sort_keys=True,indent=2)+'\n')
        _assert_no_symlink_path(destination.parent)
        if destination.exists() or destination.is_symlink():
            raise ValueError('destination appeared during export')
        os.rename(stage,destination)
    except Exception:
        shutil.rmtree(stage,ignore_errors=True)
        raise
    return destination/'manifest.json'

if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--input',required=True)
    parser.add_argument('--output',required=True)
    args = parser.parse_args()
    print(export(args.input,args.output))
