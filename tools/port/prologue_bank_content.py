"""Source/verified-original-bound metadata for the two opening bank0 requests.

Exports only external JSON metadata; actual VAB/SEQ payload readers and playback
remain separate. Source call arguments are never copied into runtime source.
"""
import hashlib,json,os,re,shutil,sys,tempfile
from pathlib import Path
ROOT=next(p for p in Path(__file__).resolve().parents if (p/'tools/assets/input.py').exists());sys.path.insert(0,str(ROOT))
from tools.port import new_game_tables as original
from tools.port.school_day_tables import _check_export_path,_assert_no_symlink_path
from tools.assets.input import open_input
SOURCE_PINS={'src/ovl/name_entry/prologue.c': 'cce3a4abaf102b745e0b116fcd1331ec63b35234d45b5bab4cbd9c39fc10470e', 'src/main/frame_render_audio.c': 'b0932ca59d48bec7b3a3be55b8188b09f55f15721dbe96d84de14f2a0526d08e', 'src/main/cd_queue.c': '722050726a5240caffcc107cef6a8806dff428fdf4f92a087a081a7f4cf0f0e9', 'include/main/audio.h': '16877729ae9cc510dd3b7c609c71bb1dd97b5dda46e17f8141f318bf6e952c9d'}
PAYLOAD_SHA256='1aea3228d41a007f6044ef8b6661e8fe498fc35178cde0f0838c5ebd37557bf7'
ROLES={'prologue_arrival':'prologue_arrival_bank_start','normal_gameplay':'prologue_finish_run'}

def digest(data):return hashlib.sha256(data).hexdigest()

def _arguments(source,function):
    marker='void '+function+'(void) {'
    if source.count(marker)!=1:raise ValueError('bank request source function missing/ambiguous')
    body=source.split(marker,1)[1].split('\n}\n',1)[0]
    matches=re.findall(r'audio_bank_load_start\(([^()]*)\);',body)
    if len(matches)!=1:raise ValueError('bank request call missing/ambiguous')
    tokens=[t.strip()for t in matches[0].split(',')]
    if len(tokens)!=6 or any(not re.fullmatch(r'(?:0x[0-9A-Fa-f]+|[0-9]+)',t)for t in tokens):raise ValueError('bank request must have six integer literals')
    values=[int(t,16 if t.startswith('0x')else 10)for t in tokens]
    if values[0]!=0:raise ValueError('only source bank0 requests supported')
    return dict(zip(['bank','se_header','se_body','music_header','music_body','sequence'],values))

def build_bundle(input_path):
    original.build_bundle(input_path) # Strict release executable/overlay/input and source validation.
    for rel,sha in SOURCE_PINS.items():
        if digest((ROOT/rel).read_bytes())!=sha:raise ValueError('bank source pin differs: '+rel)
    source_text=(ROOT/'src/ovl/name_entry/prologue.c').read_text()
    roles={};proof={}
    with open_input(input_path)as source:
        entries=source.entries()
        for role,function in ROLES.items():
            binding=_arguments(source_text,function);assets={}
            for field,lba in binding.items():
                if field=='bank':continue
                found=[e for e in entries if e.get('location',{}).get('lba')==lba]
                if len(found)!=1:raise ValueError('bank argument is not a unique exact ISO entry: '+field)
                entry=found[0];raw=source.read(entry)
                assets[field]={'path':entry['path'],'lba':lba,'size':len(raw),'sha256':digest(raw)}
                if field=='sequence':
                    if len(raw)<4:raise ValueError('native sequence package header truncated')
                    assets[field]['word0']=int.from_bytes(raw[:4],'little')
            roles[role]={'binding':binding,'assets':assets};proof[role]={'function':function}
    payload=json.dumps(roles,sort_keys=True,separators=(',',':')).encode();sha=digest(payload)
    if PAYLOAD_SHA256 and sha!=PAYLOAD_SHA256:raise ValueError('bank metadata differs from verified original ISO payload')
    return payload,{'schema':'tokimemo.prologue-bank-bindings.v1','payload':'banks.json','payload_sha256':sha,'source_pins':SOURCE_PINS,'requests':proof}

def export(input_path,output_path):
    destination,_=_check_export_path(output_path);payload,manifest=build_bundle(input_path)
    destination.parent.mkdir(parents=True,exist_ok=True);_assert_no_symlink_path(destination.parent)
    stage=Path(tempfile.mkdtemp(prefix='.prologue-banks-',dir=destination.parent))
    try:
        (stage/'banks.json').write_bytes(payload);(stage/'manifest.json').write_text(json.dumps(manifest,sort_keys=True,indent=2)+'\n')
        _check_export_path(destination);os.rename(stage,destination)
    except Exception:shutil.rmtree(stage,ignore_errors=True);raise
    return destination
if __name__=='__main__':
    import argparse
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('--input',required=True);p.add_argument('--output',required=True);a=p.parse_args();print(export(a.input,a.output))
