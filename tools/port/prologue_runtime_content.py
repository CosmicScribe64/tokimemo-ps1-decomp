"""Export bounded original prologue text storage, not opening execution/audio.

All wording is read from authenticated original inputs. Source literals are
lookup witnesses only; no original text is distributed in this module.
"""
from __future__ import annotations
import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import sys
import tempfile
from types import MappingProxyType

ROOT = Path(os.environ.get('TOKIMEMO_ROOT', Path(__file__).resolve().parents[2])).resolve()
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))
from tools.assets.input import open_input
from tools.assets.text_binary import _emitted
from tools.assets.text_fixed import tokenize_fixed_text
from tools.assets.text_source import _string_bytes
from tools.port.school_day_tables import _reader_executable, _check_export_path, _assert_no_symlink_path

EXE = 'SLPM_86.053'
OVERLAY = 'CDROM/EXEDIR/NAME_ENT.EXN'
EXE_SHA = 'bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae'
OVERLAY_SHA = 'f7384ad3617361282638ffd9bc1800bb6655d43ddee37ffe257de2ab485cd4a2'
BASE = 0x80132000
DATA = 'asm/ovl/NAME_ENT/data/name_entry/prologue.data.s'
RODATA = 'asm/ovl/NAME_ENT/data/name_entry/prologue.rodata.s'
MAIN_DATA = 'asm/data/data.data.s'
FUNCTIONS = ('prologue_line_start', 'prologue_line_run', 'prologue_lines_20_to_31_run', 'name_entry_commit', 'name_entry_new_game_setup', 'name_entry_profile_store', 'sndisp', 'sndi', 'get_p_name')
SOURCE = 'src/ovl/name_entry/prologue.c'
SCHEMA = 'tokimemo.prologue-text.v1'

class PrologueContentError(ValueError):
    """Original bytes or reviewed source evidence did not verify."""

def _sha(raw):
    return hashlib.sha256(raw).hexdigest()

def _safe_file(repo, relative):
    path = repo / relative
    if path.is_symlink() or not path.is_file() or not path.resolve().is_relative_to(repo):
        raise PrologueContentError('source proof path is missing or outside repository')
    return path

def _blocks(source):
    blocks = {}
    active = None
    for line in source.splitlines():
        match = re.fullmatch(r'\s*dlabel\s+(\w+)\s*', line)
        if match:
            active = match[1]
            if active in blocks: raise PrologueContentError('duplicate data label')
            blocks[active] = []
        elif re.match(r'\s*enddlabel\b', line): active = None
        elif active is not None: blocks[active].append(line)
    return blocks

def _address(lines):
    for line in lines:
        match = re.search(r'/\*\s*[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})(?:\s|\*/)',line)
        if match: return int(match[1],16)
    raise PrologueContentError('data label has no anchored original address')

def _label(label, blocks, symbols, data, base, bias, path):
    if label not in blocks: raise PrologueContentError('required data label missing: '+label)
    address = _address(blocks[label]); cursor = address; raw = bytearray()
    for line in blocks[label]:
        body = re.sub(r'/\*.*?\*/','',line).strip()
        if not body: continue
        match = re.fullmatch(r'\.(\w+)\s+(.+)',body)
        if not match: raise PrologueContentError('unsupported source directive in required label')
        anchor = re.search(r'/\*\s*[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})(?:\s|\*/)',line)
        if anchor and int(anchor[1],16) != cursor: raise PrologueContentError('noncontiguous required data label')
        name, arguments = match.groups()
        if name == 'word' and re.fullmatch(r'\w+',arguments) and arguments in symbols:
            emitted = symbols[arguments].to_bytes(4,'little')
        else: emitted = _emitted(name,arguments,cursor)
        if emitted is None: raise PrologueContentError('required data directive cannot be verified')
        offset = cursor-base+bias
        if offset < bias or offset+len(emitted)>len(data) or data[offset:offset+len(emitted)] != emitted:
            raise PrologueContentError('required data label differs from original bytes')
        raw.extend(emitted); cursor += len(emitted)
    return {'label':label,'address':address,'offset':address-base+bias,'length':len(raw),
            'raw_hex':raw.hex(),'sha256':_sha(raw),'source_path':path}

def _text(extent, renderer='sndisp', context=None):
    raw = bytes.fromhex(extent['raw_hex'])
    end = raw.find(b'\0')
    if end < 0: raise PrologueContentError('source text lacks in-label NUL')
    return {**extent,'tokenizer_model_only':renderer != 'sndisp','text':tokenize_fixed_text(raw[:end],renderer,{'y':0} if context is None else context),
            'terminator_offset':end,'padding_hex':raw[end+1:].hex()}

def _code_proof(repo, overlay, exe):
    result = []
    for function in FUNCTIONS:
        main_function = function in ('sndisp','sndi','get_p_name')
        relative = ('asm/matchings/main/menu_speaker_girl/' if main_function else 'asm/ovl/NAME_ENT/matchings/name_entry/'+('name_entry' if function.startswith('name_entry_') else 'prologue')+'/')+function+'.s'
        source = _safe_file(repo,relative).read_text()
        words = []
        for line in source.splitlines():
            match = re.search(r'/\*\s*([0-9A-Fa-f]+)\s+([0-9A-Fa-f]{8})\s+([0-9A-Fa-f]{8})\s*\*/',line)
            if not match: continue
            offset,address=int(match[1],16),int(match[2],16); raw=bytes.fromhex(match[3])
            expected_offset=address-0x80041000+2048 if main_function else address-BASE
            binary=exe if main_function else overlay
            if offset != expected_offset or binary[offset:offset+4] != raw:
                raise PrologueContentError('original prologue instruction differs')
            words.append((address,raw))
        if not words or any(b[0]!=a[0]+4 for a,b in zip(words,words[1:])):
            raise PrologueContentError('original prologue code is incomplete')
        result.append({'function':function,'entry':words[0][0],'instruction_words':len(words),
                       'code_sha256':_sha(b''.join(raw for _,raw in words)),'assembly':relative})
    return result

SOURCE_PINS = MappingProxyType({'asm/data/data.data.s': '811a8037fc3ded3891e44b55d6d9b8fbe2159321df0fd20b31cfb2227d8c672d',
 'asm/data/data_800B5950.data.s': '4482f3ec44d7a64fbcb1b305e015452e014382d6d819dd8759e730d3f0b2d439',
 'asm/data/main/menu_speaker_girl.rodata.s': 'b1701d0ff468bf1d114ab22b11d09cfdd1b988a563ca514e87f3a23f249547e4',
 'asm/matchings/main/menu_speaker_girl/get_p_name.s': 'd31286db8f496a0d1a99a3d5342654103632227dd0f6b59e457eec54c9b56244',
 'asm/matchings/main/menu_speaker_girl/sndi.s': '53de12343d5466dfe4c6590b4d02a5990e6b6797ffb46f2725c23f812f60db03',
 'asm/matchings/main/menu_speaker_girl/sndisp.s': '908580eb6ec6e7f3c224e7523085ea99fd3d43b735cd5500a6aa55eb8e76b9b1',
 'asm/ovl/NAME_ENT/data/name_entry/prologue.data.s': 'd65c1754e1bf9cfffb01250d2b748ef3b870cd2abc906f17e03d31c3d3156414',
 'asm/ovl/NAME_ENT/data/name_entry/prologue.rodata.s': 'cfac6e0ef3b45316f6b75ea13e49cfe7f36075008489d32ebefe9d48e98872b4',
 'asm/ovl/NAME_ENT/matchings/name_entry/name_entry/name_entry_commit.s': '6681da0d8c86d491fb0ec2f4718f4427decdb8e507cf9bc7b263e5f29dedb0d0',
 'asm/ovl/NAME_ENT/matchings/name_entry/name_entry/name_entry_new_game_setup.s': '71400ed25e0e1c8c4614fd6afa89b639112ab5b81c3a929e1bb653420591e8f6',
 'asm/ovl/NAME_ENT/matchings/name_entry/name_entry/name_entry_profile_store.s': 'a90eefb674a56f87d772af8369afdc5223a044e24214365fa08a3eefaee874be',
 'asm/ovl/NAME_ENT/matchings/name_entry/prologue/prologue_line_run.s': '4df063163d6c8838aae961b9f151f6f549cf465effaa6933f25b61e7fed11839',
 'asm/ovl/NAME_ENT/matchings/name_entry/prologue/prologue_line_start.s': '7145dadb3274dc30924655a6d629ae6067f2b1d371df92dba1be33c7576daa73',
 'asm/ovl/NAME_ENT/matchings/name_entry/prologue/prologue_lines_20_to_31_run.s': 'e8b54dffb67f32c1c19105ea3ec73aa6a5cff5017ba02f35948eb965f23054ff',
 'config/overlays/NAME_ENT.yaml': '844d84467d937cd6a6af8cba884c4dbafde062ba898671f20ceadd7f8783809a',
 'config/symbol_addrs.txt': '003c6169775031a58015faebba3d8674754a5d3ed166a9b06f9528055cbc3bfe',
 'config/symbol_addrs_hypotheses.txt': '83ddd84cc1cd2587fbc8df9099de486b34dc42ffa103d3104d5dacc3fefc4a1f',
 'config/symbols_NAME_ENT.txt': 'bf7df8a2e4db9fe79899b4b65f5bfa5d685e397480edb9b6bfe4446d09e6090b',
 'include/common.h': 'dc411aae03844dfa54650a626636ec55f2ce0c67f3ee099afb76e158ee0edfc1',
 'include/game/album.h': 'ff44f26b4fac5fcad09c7d2da96bb60949c724557286c5de3cf7c605f3c5006c',
 'include/game/appointment.h': 'fefe3a4c97bbb2812989d326d065d3948cb42119ed37d0e4ecce393154e3ec2a',
 'include/game/calendar.h': '44e07c9582c54980f4f8fe8c54adc061a0b9dcd68a5fd061014241ceb01974e9',
 'include/game/club.h': '2bfd606956b1baf60ea2e370ad1f11297fd40e721d2303b46cd7915d2b46552d',
 'include/game/day_event.h': 'efe0cdf794e1bf5f35bd5ff16feeecf3946f4695e98e066c6eca50effdf6c86d',
 'include/game/display_records.h': '54590f2da1130d31ded81ba2f52509f1781006646c75865b2f6fb832af8bc77b',
 'include/game/draw_records.h': '77cf6a1567a9877f55fe0238566b08af034c921da8c16f503f2c115f68efdcb2',
 'include/game/game_state.h': 'e58484d902e1c47696a3ae5086fa93626bebc107ab83cb0d8dc91cc0a3e4a6af',
 'include/game/girl.h': '82c7a558ed145d60e82d57f3a83d331d7fee53a9b89a2edd7c56c284e5c09c58',
 'include/game/layer.h': 'b0a33122c990abad593818fd9520ebd7f43250a4f93042492d9c12d851f97c29',
 'include/game/menu_records.h': '1849d0f486659abea223b922fe0d4397e5133387af39f98c50d83780ca9f58fa',
 'include/game/param.h': '627d01af9cc16b4e3a2819610acf9ee1fd174af5ab768c803facf1a9ec86ba55',
 'include/game/save_image.h': '2b51497cc1a103db1c91400a1b66c3a036bb634630fdad74e6821d8ec98f9a84',
 'include/game/system_data.h': '5754c2cbc7969b308dafd57593648ab1f96d7191e80813165b85f77325f7c48c',
 'include/game/word_views.h': '8a21797a38272607105b2ceef422c337e928f0b0d066aff5c81dfc6ec8bb0498',
 'include/game_enums.h': 'cf35db6941db5bdae992b0228040a77336dce7a2f951d2427b7e7a71cd204987',
 'include/include_asm.h': 'f0b8655093a7dac35e22ae67a63743121f275cd7427e0f61f5e642303acc8371',
 'include/libapi.h': 'e4c93af4e724109d86a3532e61cdd415c93bdca6d439149a393ebbf5a1a9065e',
 'include/libcd.h': 'd22297d4669d6bacd3672c871e6fa2e57cf2377c6f73298014938db7eea02e57',
 'include/libgpu.h': 'f0197b81d8f9b4889c0abd2dd0070c536f7cdf5da09b74c3d5fbc10090fe5f27',
 'include/libgte.h': '368db521cf1db72d862d6e9c1d23acdf745ec16dc844c95345993706fe6c1a84',
 'include/main/actor.h': '4377eb95590cb89931e8a0bfb8b80440bfb312d8bbd1c67e6aceb34519545dba',
 'include/main/audio.h': '16877729ae9cc510dd3b7c609c71bb1dd97b5dda46e17f8141f318bf6e952c9d',
 'include/main/bg.h': 'bd778800b70dba2bb6b2811b79086fd3c391b2448dc2ecca808698c5100237bd',
 'include/main/boot.h': '9393101d26e9ed59e96da16efbe8eae0abb9c7d2a8c5226047ce6301196763af',
 'include/main/calendar.h': '8863720cc8b1d3cbb3b0cf536fc83b35bb313616434a31d6093ac6ed47cc2285',
 'include/main/cd.h': '2175ff58a8673adbeb77cb78372f0517fa5913c77e363c277d0ecb766ced2b58',
 'include/main/club.h': '53176ba8ea59b8f005f2f8f8f4ae127cce26c73aaf40bbab05dc7612f5734ab4',
 'include/main/color.h': '9d730ad7f3b353bd0014a8ce2fca8a3f44fc13d459917af7c7bb18a08e419965',
 'include/main/cursor.h': 'b4e359b7c6278fce81566e7042e872f2025d0bfb5d2ffa2c83595fb1e02c3164',
 'include/main/debug.h': '4565888d481634611fed69f0215e97299a49597561b7f9406b805aedf7e2459d',
 'include/main/draw.h': 'b57934cf272e8d85f7b5c43369307702181b026e202d4a04ac56d9904bf93b99',
 'include/main/game_mode.h': '104f931cec5e4b9133baccca3eeea596d4966e46d24bf719a082a7931d6a8da5',
 'include/main/girl.h': 'e605fa35dd623771ffcbd60070b7edef7a8beb07942ed19a826be7391c77a8fd',
 'include/main/hud.h': 'dd5a7374a8469741052dc7abb8162fe98901eccbdaeb008f1aa97ecd48b84fd3',
 'include/main/import_views/entry_ui.h': 'eac3a572e784c88406acc5b4c40c019df3888aadd6df5c6061baeb09bfa49129',
 'include/main/import_views/entry_ui_scenes.h': 'f6ab201e31749a0a649a40cd51f54123fde6967a6d2bca6d0a309326ab3cfedf',
 'include/main/import_views/entry_ui_scenes_school_events.h': '1efff14748e011b3c4980a3636735e0d90c3e4e6dd7af2597ba0beb13c454fbb',
 'include/main/import_views/minigame_scenes_school_events.h': '0eb540cfcd85781805e3f25c11244afc6f657f11416fedf7fd0e0d080a6956c9',
 'include/main/import_views/scenes.h': 'e6e7abb06e10ecefdc007011ef75dc7e386b98344d1b6059542382812241b5ea',
 'include/main/import_views/scenes_school_events.h': '887df254ca70274155d79a878a6f94a9f9c2e7ae32c7d2cd87f11f3f997452a4',
 'include/main/layer.h': '2cca7cdd41d2a6ac4f8ce81adb3285241e802ea181906f4d5f086b969493eb06',
 'include/main/memory_card.h': '7efb3e2cbfab8ca751ebb4dc907450c289d61231aa281c8ec840546e8406a23c',
 'include/main/menu.h': 'b171ce815adeee4fdb2c4f86258ebcc2b7c1b75eafc2f26ce7e9dd9494ca35fe',
 'include/main/message.h': '71ce494eca28a75312cd49c2ba41fdfb8e1f9df7732063f374c6ecfad1fb9c06',
 'include/main/movie.h': '3ea025d0ed9db748cb4108b3360fe792a92d7b732aa33ebba3570a83ccf2b28e',
 'include/main/pad.h': '66db19784afce6a71b067e93b7a50f56be2431bb5ad31ebba4fc24ceb7dcb19c',
 'include/main/prim.h': 'a5820d7fa0051f4fe561274f5f7bc28f1df6cc87bfd76786a9c69f8a8a3ca732',
 'include/main/settings.h': '1c824ac1c30d076264b8a7d13ea97c11c395da9d9c6c77cb5c889568139b1aff',
 'include/main/speaker.h': 'b405b17fdf8298320ea1df1b51f00beb46b292b32526b488bff9a9137e7965d1',
 'include/main/sprite.h': '2b82233ad846ad0615e3dc667573984362a5518810b5b54ec973465f98994ffa',
 'include/main/state.h': 'aafe390c388f5b2fd36e8a2f3b491cd516364a01c03f258f940cac77b319fd01',
 'include/main/talk.h': 'ea765428db83f6fda60eb89c7e753180ddfe2893cc9de0391b66b32dc2dadc22',
 'include/main/text.h': '3f79601270d9515b5714dc928f59196c1c84a7223f86321e4f49f23cc0ee5eb7',
 'include/main/tpage.h': '277579106126d3d1e8b722dec269e7a7f9c8d47956ceac121253a5d54d74d4d0',
 'include/main/vram.h': 'cbed858bdfd1e38a6711c3a798bdc773be6dc849863a597d9ffa062fd669f2f3',
 'include/ovl/name_entry.h': 'ced2aa4851cd232c5e3682c99c9cfa944d2b71daa78fe9c48911b134cd60f842',
 'include/psx_types.h': '2fcbf5a7b0db1a1ccd0bfdb691144ebe815b18a430ce92d3aaa2b4fc76223fc4',
 'src/main/menu_speaker_girl.c': 'bac5ba1d7ea8f17e9bf69df803c9c1f1cebbf21fd5c3b3fc1e568bce20ca7185',
 'src/ovl/name_entry/name_entry.c': '840feed25a5b04687563e45a67eac1998fca9b050cdd59f80ad3a5f6e52cd35d',
 'src/ovl/name_entry/prologue.c': 'cce3a4abaf102b745e0b116fcd1331ec63b35234d45b5bab4cbd9c39fc10470e'})

def _source_proof(repo):
    for relative, expected in SOURCE_PINS.items():
        if _sha(_safe_file(repo,relative).read_bytes()) != expected:
            raise PrologueContentError('reviewed prologue source pin changed: '+relative)

def _unit(repo, data, base, bias, paths, source_path):
    blocks = {}; locations = {}; symbols = {}
    for path in paths:
        parsed = _blocks(_safe_file(repo,path).read_text())
        for label, lines in parsed.items():
            if label in blocks: raise PrologueContentError('ambiguous source data label')
            blocks[label] = lines; locations[label] = path
            try: symbols[label] = _address(lines)
            except PrologueContentError: pass
    def read(label):
        result = _label(label,blocks,symbols,data,base,bias,source_path)
        result['assembly_path'] = locations[label]
        return result
    def by_address(address):
        matches = [label for label,value in symbols.items() if value == address]
        if len(matches) != 1: raise PrologueContentError('pointer has no unique anchored source label')
        return read(matches[0])
    return read, by_address, blocks

def _pointers(extent, count):
    raw = bytes.fromhex(extent['raw_hex'])
    if len(raw) != count*4: raise PrologueContentError('pointer table cardinality differs')
    return [int.from_bytes(raw[i:i+4],'little') for i in range(0,len(raw),4)]

def _literal_pieces(repo, function, read, blocks, source_path=SOURCE, assembly_path=None):
    source = _safe_file(repo,source_path).read_text()
    markers=list(re.finditer(r'(?:void|s32)\s+'+re.escape(function)+r'\([^)]*\)\s*\{',source))
    if len(markers)!=1: raise PrologueContentError('source consumer missing or ambiguous')
    body=source[markers[0].end():].split('\n}\n',1)[0]
    literals = re.findall(r'"(?:\\.|[^"\\])*"',body)
    asm_path = assembly_path or 'asm/ovl/NAME_ENT/matchings/name_entry/prologue/'+function+'.s'
    asm = _safe_file(repo,asm_path).read_text()
    labels = set(re.findall(r'%lo\((\w+)\)',asm)) & blocks.keys()
    pieces=[]
    for ordinal,literal in enumerate(literals):
        raw = _string_bytes(literal)
        matches=[]
        for label in labels:
            if not any(re.search(r"\.(?:ascii|asciz|string)\b",line) for line in blocks[label]): continue
            extent = read(label)
            if bytes.fromhex(extent['raw_hex']).split(b'\0',1)[0] == raw: matches.append(extent)
        if len(matches) != 1: raise PrologueContentError('literal has no unique same-function original label witness')
        pieces.append({**_text(matches[0],'set_kanji_string'),'source_function':function,
                       'source_literal_ordinal':ordinal,'assembly':asm_path})
    return pieces

def build_bundle(input_path, repository=None):
    """Re-read exact original storage; do not execute/composite dynamic text."""
    repo = Path(repository or ROOT).resolve(strict=True)
    _source_proof(repo)
    exe, _info, _entry = _reader_executable(str(input_path))
    if len(exe) != 667648 or _sha(exe) != EXE_SHA:
        raise PrologueContentError('prologue executable release differs')
    with open_input(str(input_path)) as source:
        entries=[e for e in source.entries() if e.get('path') == OVERLAY]
        if len(entries)!=1: raise PrologueContentError('expected unique original NAME_ENT overlay')
        overlay=source.read(entries[0])
    if len(overlay)!=196608 or _sha(overlay)!=OVERLAY_SHA:
        raise PrologueContentError('prologue overlay release differs')
    code = _code_proof(repo,overlay,exe)
    read, by_address, blocks = _unit(repo,overlay,BASE,0,[DATA,RODATA],OVERLAY)
    main, main_address, main_blocks = _unit(repo,exe,0x80041000,2048,[MAIN_DATA,'asm/data/main/menu_speaker_girl.rodata.s'],EXE)
    pointers = read('prologue_line_texts')
    addresses = _pointers(pointers,43)
    speakers = read('prologue_line_speakers')
    if speakers['length'] != 44: raise PrologueContentError('speaker storage extent differs')
    clips = [read(name) for name in ('prologue_line_clips','prologue_line_clips_second')]
    if clips[0]['length'] != 43*8 or clips[1]['length'] < 43*8: raise PrologueContentError('clip pair table cardinality differs')
    secondary_full = bytes.fromhex(clips[1]['raw_hex'])
    clips[1] = {**clips[1], 'length':344, 'raw_hex':secondary_full[:344].hex(), 'sha256':_sha(secondary_full[:344]),
                'trailing_label_storage':{'offset':clips[1]['offset']+344,'length':len(secondary_full)-344,'sha256':_sha(secondary_full[344:])},
                'count_basis':'same line-id domain 0..42 as fixed text pointer table; trailing label storage unclaimed'}
    clip_raw=[bytes.fromhex(e['raw_hex']) for e in clips]
    rows=[]
    for index,address in enumerate(addresses):
        extent=read('prologue_line_text_'+str(index).zfill(2))
        if extent['address']!=address: raise PrologueContentError('line pointer disagrees with source label')
        speaker=bytes.fromhex(speakers['raw_hex'])[index]
        role='player_family_name' if speaker&15==15 else 'static_label' if speaker in (0x1e,0x2e) else 'girl_short_name'
        pairs=[[int.from_bytes(raw[index*8+j:index*8+j+4],'little',signed=True) for j in (0,4)] for raw in clip_raw]
        rows.append({'id':'prologue:line:'+str(index).zfill(2),'line_id':index,
                     'pointer_address':pointers['address']+index*4,'text_address':address,
                     'speaker':speaker,'speaker_binding':role,'clip1':pairs[0],'clip2':pairs[1],
                     'storage':_text(extent,context={'y':0,'col':speaker}),'renderer':'sndisp','renderer_arguments':{'y':0,'speaker':speaker},
                     'runtime_execution_proven':False})
    renderer_pieces=_literal_pieces(repo,'sndi',main,main_blocks,'src/main/menu_speaker_girl.c','asm/matchings/main/menu_speaker_girl/sndi.s')
    if len(renderer_pieces)!=3: raise PrologueContentError('sndi decoration piece count changed')
    static_labels=_literal_pieces(repo,'prologue_line_start',read,blocks)
    dynamic=_literal_pieces(repo,'prologue_lines_20_to_31_run',read,blocks)
    if len(static_labels)!=2 or len(dynamic)!=3: raise PrologueContentError('source literal piece count changed')
    blood_table=read('prologue_blood_texts')
    blood_rows=[_text(by_address(p),'set_kanji_string') for p in _pointers(blood_table,4)]
    # Consumed indices are 0..12: name_entry_profile_store stores UI month/day +1,
    # so months use 1..12 and day tens/units use 0..3 / 0..9.
    digits=[]; digit_words=[main('D_800B3288'),main('D_800B328C')]
    numeric_addresses=_pointers(digit_words[0],1)+_pointers(digit_words[1],16)
    for address in numeric_addresses[:13]:
        digits.append(_text(main_address(address),'set_kanji_string'))
    mascot_read, _, _ = _unit(repo,exe,0x80041000,2048,['asm/data/data_800B5950.data.s'],EXE)
    mascot=mascot_read('hud_mascot_choice')
    if mascot['address']!=0x800b5c08 or mascot['length']!=8: raise PrologueContentError('initial mascot storage differs')
    initial_mascot=int.from_bytes(bytes.fromhex(mascot['raw_hex'])[:2],'little',signed=True)
    name_table=[main('girl_short_name_table'),main('girl_short_name_rei'),main('girl_short_name_yumi_given')]
    name_addresses=_pointers(name_table[0],12)+_pointers(name_table[1],3)+_pointers(name_table[2],1)
    names=[_text(main_address(p),'set_kanji_string') for p in name_addresses]
    return {'schema':SCHEMA,'version':1,'lines':rows,
            'initial_mascot':{'value':initial_mascot,'scalar_width':2,'scalar_signed':True,'storage':mascot},
            'tables':{'line_text_pointers':pointers,'speakers':speakers,'clip1':clips[0],'clip2':clips[1],
                      'blood_pointers':blood_table,'birthday_numeric_pointer_words':digit_words,'girl_short_names':name_table},
            'speaker_labels':{'30':static_labels[0],'46':static_labels[1],'girl_short_name_rows':names,
                              'player_family_name':'live_state_binding_not_exported'},
            'renderer_pieces':dict(zip(('player_quote_open','parenthesis_open','girl_quote_open'),renderer_pieces)),
            'dynamic_pieces':{'birthday_numeric_rows':digits,'blood_rows':blood_rows,
                              'month_suffix':dynamic[0],'day_suffix':dynamic[1],'blood_suffix':dynamic[2]},
            'dynamic_recipes':{'birthday':{'source_function':FUNCTIONS[2],'substep':12,'speaker':15,'source_girl':0,
                    'month_index':'native_month','native_month_domain':[1,12],'native_day_domain':[1,31],'day_tens_if':'native_day >= 10','day_tens_index':'native_day // 10',
                    'day_units_index':'native_day % 10','append_byte_limits':[2,2,2,4],'original_buffer_bytes':128},
                'blood':{'source_function':FUNCTIONS[2],'substep':16,'speaker':15,'source_girl':0,
                         'blood_index':'native_blood_type','append_byte_limit':4,'original_buffer_bytes':128}},
            'proof':{'source_pins':dict(SOURCE_PINS),'exe_sha256':EXE_SHA,'overlay_sha256':OVERLAY_SHA,'code':code},
            'limits':{'runtime_execution_proven':False,'audio_playback_proven':False,'dynamic_composition_executed':False,
                      'sdk_string_copy_semantics_proven':False,'whole_overlay_text_coverage_claimed':False,
                      'speaker_storage_final_byte_is_padding':True,'birthday_numeric_table_scope':'consumed indices 0..12 only; stored month/day are one-based'}}

def export(input_path, output_path, repository=None):
    """Publish one new data-only JSON bundle under ignored external port cache."""
    repo=Path(repository or ROOT).resolve(strict=True)
    destination,_=_check_export_path(output_path,repo)
    bundle=build_bundle(input_path,repo)
    destination.parent.mkdir(parents=True,exist_ok=True)
    _assert_no_symlink_path(destination.parent)
    stage=Path(tempfile.mkdtemp(prefix='.prologue-text-',dir=destination.parent))
    try:
        raw=(json.dumps(bundle,sort_keys=True,ensure_ascii=False,separators=(',',':'))+'\n').encode('utf-8')
        (stage/'prologue.json').write_bytes(raw)
        manifest={'schema':'tokimemo.prologue-text-pack.v1','payload':'prologue.json','size':len(raw),
                  'sha256':_sha(raw),'line_count':len(bundle['lines']),'runtime_execution_proven':False}
        (stage/'manifest.json').write_text(json.dumps(manifest,sort_keys=True,indent=2)+'\n')
        _check_export_path(destination,repo);os.rename(stage,destination)
    except Exception:
        shutil.rmtree(stage,ignore_errors=True);raise
    return destination/'manifest.json'

if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--input',required=True)
    parser.add_argument('--output',required=True)
    parser.add_argument('--repository',type=Path,default=ROOT)
    args=parser.parse_args()
    print(export(args.input,args.output,args.repository))
