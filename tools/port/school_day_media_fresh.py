"""T-39008 fresh original-source command0/7 media; no retained emulator report.

Finite April, non-vacation, non-sick, extra15/no-companion assets only. This
proves source selectors and media identity, not a played route or scene parity.
"""
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import sys
import tempfile

if __package__ in (None, ""):
    sys.path.insert(0, str(Path(__file__).resolve().parents[2]))
from tools.assets.input import open_input
from tools.assets.bs_cohort_profile import build_profile_context, SOURCE_FILES
from tools.assets.manifest import asset_id
from tools.assets.media_mdec_export import _native_cd_read_extent
from tools.port.school_day_media import TRACK_SHA, EXE_SHA, _safe_copy
from tools.port.school_day_tables import build_bundle
from tools.port.spring_command_content import _safe_output
from tools.port.media_components import export_sd_component, export_background_component
from tools.port.validate_school_day_media import SOURCE_PINS, EXPECTED_CAPABILITIES, EXPECTED_UNSUPPORTED

SCHEMA = "tokimemo.school-day-media.v4"
PROFILE = "source-checked-april-command0-7-extra15.v1"
ROOT = Path(__file__).resolve().parents[2]


def sha(data):
    return hashlib.sha256(data).hexdigest()


def export(input_path, output_path):
    out = _safe_output(ROOT, output_path)
    tables = build_bundle(input_path)
    t = tables['tables']
    # school_day_sd_set_select: non-summer variant1 for extra15; command7
    # uses fixed table index13, selecting rest set16 (school_day.c:272).
    # Original table values bind both LBAs.
    if t['sd_select_non_summer'][1] != 3 or t['sd_select_non_summer'][13] != 16:
        raise ValueError('original command0/7 selectors differ')
    evidence = []
    for path, expected in SOURCE_FILES.items():
        data = (ROOT/path).read_bytes()
        if sha(data) != expected:
            raise ValueError('source proof changed: '+path)
        evidence.append({'path':path,'sha256':expected,'size':len(data)})
    out.parent.mkdir(parents=True,exist_ok=True)
    stage = Path(tempfile.mkdtemp(prefix='.school-fresh-',dir=out.parent))
    try:
        components = {}
        with open_input(input_path) as source:
            if source.describe().get('source_sha256') != TRACK_SHA or source.describe().get('format') != 'mode2-2352':
                raise ValueError('original track identity differs')
            entries = source.entries(); by_path = {e['path']:e for e in entries}
            exe = source.read(by_path['SLPM_86.053'])
            if sha(exe) != EXE_SHA: raise ValueError('original executable differs')
            for command,index,kind in [(0,3,'weekly'),(7,16,'command7')]:
                profile = build_profile_context(source,entries,ROOT,kind)
                route = {'command':command,'actual_last_command':command,'actors':[96],
                         'profile':PROFILE,'evidence':evidence}
                for role in ['background','sd']:
                    key = f'command_{command}_{role}'
                    path,lba,size,digest = SOURCE_PINS[key]
                    entry = by_path[path]; data = source.read(entry)
                    if entry['location']['lba'] != lba or len(data) != size or sha(data) != digest:
                        raise ValueError('original component differs: '+key)
                    table_lba = t['weekday_background_lbas'][command] if role=='background' else t['weekday_sd_lbas'][index]
                    if table_lba != lba: raise ValueError('source selector LBA differs: '+key)
                    identity = asset_id(path,digest)
                    if role=='sd':
                        components[key],_ = export_sd_component(data,exe,stage,entry,identity,'weekly',index,route,_safe_copy)
                    else:
                        rec = next(a for a in profile['assets'] if a['path']==path)
                        context = {'width':320,'height':88,'padded_height':96,
                            'source_profile':profile['source_profile'],'geometry':profile['geometry'],
                            'evidence':rec['source_evidence']}
                        components[key] = export_background_component(data,stage,entry,identity,context,route,_safe_copy)
                        if command==7:
                            components[key]['native_read_extent'] = _native_cd_read_extent(source,entry,TRACK_SHA,10)
        manifest = {'schema':SCHEMA,'version':4,'components':components,
            'provenance':{'original_input_sha256':TRACK_SHA,'original_executable_sha256':EXE_SHA,
                'profile':PROFILE,'source_evidence':evidence,
                'selector_table_proof':{k:tables['provenance']['tables'][k] for k in ['weekday_background_lbas','weekday_sd_lbas','sd_select_non_summer']}},
            'route_capabilities':EXPECTED_CAPABILITIES+['command_7_static_background'],
            'unsupported':{k:v for k,v in EXPECTED_UNSUPPORTED.items() if k!='command_7_background'}}
        (stage/'manifest.json').write_text(json.dumps(manifest,sort_keys=True,indent=2)+'\n')
        from tools.port.validate_school_day_media import validate_pack
        validate_pack(stage)
        if out.exists(): raise ValueError('immutable output appeared during export')
        stage.rename(out)
        return out/'manifest.json'
    finally:
        if stage.exists(): shutil.rmtree(stage)


if __name__=='__main__':
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--input',required=True);p.add_argument('--output',required=True)
    a=p.parse_args();print(export(a.input,a.output))
