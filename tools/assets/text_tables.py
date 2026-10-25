"""Read source-backed external text tables outside the talk containers (T-28001).

NAME_KAN: name_entry/name_entry.c loads six sectors at LBA 0x9DBF to
0x801E0000, relocates 50 offsets at +0x2B2C, and indexes 45 character
lists using u16 counts at the start of the file. The lists are glyph
selection data, not dialogue and not bitmap OCR.
"""
from __future__ import annotations

import struct
import re
from pathlib import Path

from .text import TextFormatError, sha256
from .text_fixed import tokenize_fixed_text
from .script_validation import checked_cd_trailer


def verify_table_source_proof(data: bytes, profile: dict, repository, files_root) -> dict:
    """Verify current pinned code, formulas and bounded initialized table bytes.

    Source labels may split a declared array into several assembly labels. Each
    claimed byte must still have an original-address anchor; pointer operands
    must resolve through pinned symbols or an explicit address label.
    """
    from .text_binary import _spans
    from .text_ui_tables import verify_ui_source_proof
    from .text import source_path
    common = verify_ui_source_proof(data, {**profile, 'tables': []}, repository, files_root)
    repo = Path(repository).resolve(strict=True)
    sources = {name: (repo/name).read_text(encoding='utf-8') for name in profile['source_pins']}
    for check in profile.get('source_checks', []):
        source = re.sub(r'\s+', '', sources[check['path']])
        if re.sub(r'\s+', '', check['contains']) not in source:
            raise TextFormatError('current table source formula/type differs from reviewed proof')
    symbols = {}
    for name, text in sources.items():
        if name.startswith('config/'):
            for match in re.finditer(r'^(\w+)\s*=\s*0x([0-9A-Fa-f]+);', text, re.M):
                value = int(match[2], 16)
                if match[1] in symbols and symbols[match[1]] != value:
                    raise TextFormatError('conflicting current table source symbols')
                symbols[match[1]] = value
    data_checks = []
    base, bias = profile['load_address'], profile.get('header_size', 0)
    for proof in profile.get('data_proofs', []):
        proof_data, proof_base, proof_bias = data, base, bias
        proof_path, proof_sha = profile['path'], sha256(data)
        if 'source_path' in proof:
            import hashlib
            inputs = Path(files_root).resolve(strict=True)
            path = (inputs/source_path(proof['source_path'])).resolve(strict=True)
            if not path.is_relative_to(inputs) or not path.is_file():
                raise TextFormatError('table data proof escapes original input root')
            proof_data = path.read_bytes()
            proof_path, proof_sha = proof['source_path'], hashlib.sha256(proof_data).hexdigest()
            if proof_sha != proof['source_sha256']:
                raise TextFormatError('table original data proof image differs from pinned hash')
            proof_base, proof_bias = proof['load_address'], proof['header_size']
        relative = source_path(proof['assembly_path'])
        if relative not in sources:
            raise TextFormatError('table assembly is not pinned')
        address, length = proof['address'], proof['length']
        start = address-proof_base+proof_bias
        if start < proof_bias or length <= 0 or start+length > len(proof_data):
            raise TextFormatError('table source proof outside original input')
        used = bytearray(length)
        root_seen = False
        binary = (proof_data, proof_base, proof_bias, proof_sha, proof_path)
        for span in _spans(repo/relative, repo, binary):
            if span['label'] == proof['label'] and span['address'] == address:
                root_seen = True
            if span['address'] is None:
                continue
            a, b = max(address, span['address']), min(address+length, span['address']+len(span['raw']))
            if a < b:
                used[a-address:b-address] = b'\1'*(b-a)
        active, pending = None, None
        for line in sources[relative].splitlines():
            label = re.fullmatch(r'\s*(dlabel|enddlabel)\s+(\w+)\s*', line)
            if label:
                active = label[2] if label[1] == 'dlabel' else None
                pending = active
                continue
            word = re.fullmatch(r'\s*/\*\s*([0-9A-Fa-f]+)\s+([0-9A-Fa-f]{8})\s+([0-9A-Fa-f]{8})\s*\*/\s*\.word\s+([A-Za-z_0-9]+)\s*', line)
            if word is None:
                continue
            offset, target = int(word[1],16), int(word[2],16)
            if pending == proof['label'] and target == address:
                root_seen = True
            pending = None
            if not address <= target < address+length:
                continue
            raw = bytes.fromhex(word[3])
            operand = word[4]
            label_address = re.fullmatch(r'D_([0-9A-Fa-f]{8})', operand)
            if label_address:
                value = int(label_address[1],16)
            elif operand in symbols:
                value = symbols[operand]
            else:
                try:
                    value = int(operand,0)
                except ValueError as exc:
                    raise TextFormatError('table pointer initializer lacks an exact source symbol address') from exc
            if (target+4 > address+length or offset != target-proof_base+proof_bias or
                    proof_data[offset:offset+4] != raw or int.from_bytes(raw,'little') != value & 0xFFFFFFFF):
                raise TextFormatError('table pointer source initializer differs from original word')
            used[target-address:target-address+4] = b'\1'*4
        if not root_seen or not all(used):
            raise TextFormatError('table source label lacks exact bounded initialized byte proof')
        data_checks.append({'label':proof['label'],'address':address,'initialized_bytes_verified':length})
    if not data_checks:
        raise TextFormatError('table source verification requires initialized data proofs')
    return {**common, 'data_labels':data_checks, 'source_formula_checks':len(profile.get('source_checks', []))}

NAME_KAN_PROFILE = {'id': 'name-entry-candidates',
 'path': 'CDROM/FUKUTEST/NAME_KAN.NBN',
 'sha256': '29c22ef10985a33a93acb2fef4a6bb6942c1b73e6aff7f4ea974a265213039a9',
 'load_address': 2149449728,
 'counts_offset': 0,
 'counts_count': 48,
 'offsets_offset': 11052,
 'offsets_count': 50,
 'list_count': 45,
 'evidence': ['src/ovl/name_entry/name_entry.c::name_entry_load_init',
              'src/ovl/name_entry/name_entry.c::name_entry_load_fade_in',
              'src/ovl/name_entry/name_entry.c::name_entry_character_put',
              'include/ovl/name_entry.h:name_entry_candidate_counts,name_entry_candidate_lists'],
 'data_proofs': [{'label': 'name_entry_candidate_counts',
                  'address': 2148847204,
                  'length': 4,
                  'assembly_path': 'asm/ovl/NAME_ENT/data/name_entry/name_entry.data.s',
                  'source_path': 'CDROM/EXEDIR/NAME_ENT.EXN',
                  'source_sha256': 'f7384ad3617361282638ffd9bc1800bb6655d43ddee37ffe257de2ab485cd4a2',
                  'load_address': 2148737024,
                  'header_size': 0},
                 {'label': 'name_entry_candidate_lists',
                  'address': 2148847208,
                  'length': 4,
                  'assembly_path': 'asm/ovl/NAME_ENT/data/name_entry/name_entry.data.s',
                  'source_path': 'CDROM/EXEDIR/NAME_ENT.EXN',
                  'source_sha256': 'f7384ad3617361282638ffd9bc1800bb6655d43ddee37ffe257de2ab485cd4a2',
                  'load_address': 2148737024,
                  'header_size': 0}],
 'source_checks': [{'path': 'src/ovl/name_entry/name_entry.c',
                    'contains': 'cd_read_sectors(6U, 0x801E0000, 0x9DBF);'},
                   {'path': 'src/ovl/name_entry/name_entry.c',
                    'contains': 'p = name_entry_candidate_lists; for (i = 0; i < 50; i++) { p[i] += '
                                '0x801E0000; }'},
                   {'path': 'src/ovl/name_entry/name_entry.c', 'contains': 'name_entry_reading >= 0x2D'}],
 'code_proofs': [{'assembly_path': 'asm/ovl/NAME_ENT/matchings/name_entry/name_entry/name_entry_preset_apply.s',
                  'source_path': 'CDROM/EXEDIR/NAME_ENT.EXN',
                  'source_sha256': 'f7384ad3617361282638ffd9bc1800bb6655d43ddee37ffe257de2ab485cd4a2',
                  'load_address': 2148737024,
                  'header_size': 0},
                 {'assembly_path': 'asm/ovl/NAME_ENT/matchings/name_entry/name_entry/name_entry_load_init.s',
                  'source_path': 'CDROM/EXEDIR/NAME_ENT.EXN',
                  'source_sha256': 'f7384ad3617361282638ffd9bc1800bb6655d43ddee37ffe257de2ab485cd4a2',
                  'load_address': 2148737024,
                  'header_size': 0},
                 {'assembly_path': 'asm/ovl/NAME_ENT/matchings/name_entry/name_entry/name_entry_load_fade_in.s',
                  'source_path': 'CDROM/EXEDIR/NAME_ENT.EXN',
                  'source_sha256': 'f7384ad3617361282638ffd9bc1800bb6655d43ddee37ffe257de2ab485cd4a2',
                  'load_address': 2148737024,
                  'header_size': 0},
                 {'assembly_path': 'asm/ovl/NAME_ENT/matchings/name_entry/name_entry/name_entry_kanji_page_draw.s',
                  'source_path': 'CDROM/EXEDIR/NAME_ENT.EXN',
                  'source_sha256': 'f7384ad3617361282638ffd9bc1800bb6655d43ddee37ffe257de2ab485cd4a2',
                  'load_address': 2148737024,
                  'header_size': 0},
                 {'assembly_path': 'asm/ovl/NAME_ENT/matchings/name_entry/name_entry/name_entry_character_put.s',
                  'source_path': 'CDROM/EXEDIR/NAME_ENT.EXN',
                  'source_sha256': 'f7384ad3617361282638ffd9bc1800bb6655d43ddee37ffe257de2ab485cd4a2',
                  'load_address': 2148737024,
                  'header_size': 0},
                 {'assembly_path': 'asm/ovl/NAME_ENT/matchings/name_entry/name_entry/name_entry_name_finish.s',
                  'source_path': 'CDROM/EXEDIR/NAME_ENT.EXN',
                  'source_sha256': 'f7384ad3617361282638ffd9bc1800bb6655d43ddee37ffe257de2ab485cd4a2',
                  'load_address': 2148737024,
                  'header_size': 0}],
 'source_pins': {'asm/ovl/NAME_ENT/data/name_entry/name_entry.data.s': '02c5c37160e8c16d1c692b03123a090fb0dfce6d9f1fd5674613897617353c9c',
                 'asm/ovl/NAME_ENT/matchings/name_entry/name_entry/name_entry_character_put.s': 'bf7c367f8cc9ad150ca7a1d2e55c6a593ad02af5e43e69fc57e4aaaa5d27c139',
                 'asm/ovl/NAME_ENT/matchings/name_entry/name_entry/name_entry_kanji_page_draw.s': '3718e78759cbaf2d75f35e332e032b45369cd2f12f13f8d480d4d878bb419f25',
                 'asm/ovl/NAME_ENT/matchings/name_entry/name_entry/name_entry_load_fade_in.s': 'a1b1dfccf4fdf7eea137526981dbba8946a19dd24f777915d401a481173479ea',
                 'asm/ovl/NAME_ENT/matchings/name_entry/name_entry/name_entry_load_init.s': '4964f5e1ad356c53c32ef8449353315743dcfa6ea521f4996eecf1c9eb13a0d3',
                 'asm/ovl/NAME_ENT/matchings/name_entry/name_entry/name_entry_name_finish.s': '45e4f42e431153ab70d5ae7d1d6fefefb99024042e677d9a894d78e033df380a',
                 'asm/ovl/NAME_ENT/matchings/name_entry/name_entry/name_entry_preset_apply.s': '7f59b34ae3529f07c6893f1005ee9e6791e07dec98c228ff05356ed100b6f5aa',
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
                 'include/main/text.h': '3f79601270d9515b5714dc928f59196c1c84a7223f86321e4f49f23cc0ee5eb7',
                 'include/main/tpage.h': '277579106126d3d1e8b722dec269e7a7f9c8d47956ceac121253a5d54d74d4d0',
                 'include/main/vram.h': 'cbed858bdfd1e38a6711c3a798bdc773be6dc849863a597d9ffa062fd669f2f3',
                 'include/ovl/name_entry.h': 'ced2aa4851cd232c5e3682c99c9cfa944d2b71daa78fe9c48911b134cd60f842',
                 'include/psx_types.h': '2fcbf5a7b0db1a1ccd0bfdb691144ebe815b18a430ce92d3aaa2b4fc76223fc4',
                 'src/ovl/name_entry/name_entry.c': '840feed25a5b04687563e45a67eac1998fca9b050cdd59f80ad3a5f6e52cd35d'},
 'header_size': 0}


def parse_character_lists(data: bytes, profile=None) -> dict:
    """Read exact counted glyph lists and preserve relocation/table metadata."""
    profile = NAME_KAN_PROFILE if profile is None else profile
    digest = sha256(data)
    if profile.get("sha256") and digest != profile["sha256"]:
        raise TextFormatError("character-list source differs from verified layout")
    co, cc = profile["counts_offset"], profile["counts_count"]
    po, pc = profile["offsets_offset"], profile["offsets_count"]
    count = profile["list_count"]
    if co < 0 or po < 0 or co + cc * 2 > len(data) or po + pc * 4 > len(data) or not 0 < count <= min(cc, pc):
        raise TextFormatError("character-list tables outside source")
    counts = struct.unpack_from("<" + "H" * cc, data, co)
    offsets = struct.unpack_from("<" + "I" * pc, data, po)
    records = []
    spans = []
    used = bytearray(len(data))
    used[co:co + cc * 2] = b"\x01" * (cc * 2)
    used[po:po + pc * 4] = b"\x01" * (pc * 4)
    for index in range(count):
        start, length = offsets[index], counts[index] * 2
        if start < co + cc * 2 or start + length + 2 > po:
            raise TextFormatError("character list outside its source data region")
        if data[start + length:start + length + 2] != b"\0\0":
            raise TextFormatError("counted character list lacks its u16 terminator")
        if any(max(start, a) < min(start + length + 2, b) for a, b in spans):
            raise TextFormatError("character list extents overlap")
        spans.append((start, start + length + 2))
        used[start:start + length + 2] = b"\x01" * (length + 2)
        raw = data[start:start + length]
        view = tokenize_fixed_text(raw, "load_string")
        if view["source_decoded_text"] is None or len(view["source_decoded_text"]) != counts[index]:
            raise TextFormatError("character list is not exact full-width Shift-JIS")
        records.append({"id": f"character-list:{profile['path']}:{index:02d}",
                        "source_path": profile["path"], "source_sha256": digest,
                        "index": index, "offset": start, "length": length, "count": counts[index],
                        "count_offset": co + index * 2, "pointer_offset": po + index * 4,
                        "pointer_encoding": "relative_to_load_address", "pointer_value": start,
                        "terminator_hex": "0000", "text": view})
    trailer = checked_cd_trailer(data)
    if trailer.get("marker_present"):
        used[-4:] = b"\x01" * 4
    unclaimed = []
    position = 0
    while position < len(data):
        if used[position]:
            position += 1
            continue
        start = position
        while position < len(data) and not used[position]:
            position += 1
        raw = data[start:position]
        unclaimed.append({"offset": start, "length": len(raw), "raw_hex": raw.hex(),
                          "sha256": sha256(raw), "classification": "zero_padding" if not any(raw) else "uninterpreted"})
    return {"schema": "tokimemo.character-lists.v1", "profile": profile,
            "source": {"path": profile["path"], "sha256": digest, "size": len(data)},
            "counts": list(counts), "offsets": list(offsets),
            "records": records, "cd_trailer": trailer, "unclaimed_ranges": unclaimed,
            "coverage": {"lists": len(records), "glyph_entries": sum(counts[:count]),
                         "unreferenced_count_slots": list(range(count, cc)),
                         "unreferenced_pointer_slots": list(range(count, pc))}}

# Original addresses/counts are established by the named pointer arrays and
# consumers in the current main/name-entry headers. Fixed-width presets use the source's % 15 and
# 8/8/12-byte strncpy strides, not string scanning.
DICTIONARY_PROFILES = [{'id': 'main-terms',
  'path': 'SLPM_86.053',
  'sha1': 'e823bd844a8f8fa4d05483b59c66bc54b8393b26',
  'load_address': 2147749888,
  'header_size': 2048,
  'tables': [{'id': 'parameter_names', 'address': 2148217628, 'count': 9, 'kind': 'pointer_strings'},
             {'id': 'place_names', 'address': 2148217900, 'count': 21, 'kind': 'pointer_strings'},
             {'id': 'girl_full_names', 'address': 2148218168, 'count': 16, 'kind': 'pointer_strings'},
             {'id': 'girl_short_names', 'address': 2148218356, 'count': 16, 'kind': 'pointer_strings'},
             {'id': 'club_names', 'address': 2148219144, 'count': 12, 'kind': 'pointer_strings'}],
  'evidence': ['include/main/girl.h:girl_full_name_table[16]',
               'include/main/club.h:club_name_table',
               'src/main/menu_speaker_girl.c::get_g_name',
               'asm/data/data.data.s:original named table extents'],
  'sha256': 'bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae',
  'data_proofs': [{'label': 'param_name_table',
                   'address': 2148217628,
                   'length': 36,
                   'assembly_path': 'asm/data/data.data.s'},
                  {'label': 'place_name_table',
                   'address': 2148217900,
                   'length': 84,
                   'assembly_path': 'asm/data/data.data.s'},
                  {'label': 'girl_full_name_table',
                   'address': 2148218168,
                   'length': 64,
                   'assembly_path': 'asm/data/data.data.s'},
                  {'label': 'girl_short_name_table',
                   'address': 2148218356,
                   'length': 64,
                   'assembly_path': 'asm/data/data.data.s'},
                  {'label': 'club_name_table',
                   'address': 2148219144,
                   'length': 48,
                   'assembly_path': 'asm/data/data.data.s'}],
  'source_checks': [{'path': 'include/main/girl.h', 'contains': 'u8 *girl_full_name_table[16];'}],
  'code_proofs': [{'assembly_path': 'asm/matchings/main/menu_speaker_girl/get_g_name.s',
                   'source_path': 'SLPM_86.053',
                   'source_sha256': 'bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae',
                   'load_address': 2147749888,
                   'header_size': 2048},
                  {'assembly_path': 'asm/matchings/main/menu_speaker_girl/get_p_name.s',
                   'source_path': 'SLPM_86.053',
                   'source_sha256': 'bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae',
                   'load_address': 2147749888,
                   'header_size': 2048}],
  'source_pins': {'asm/data/data.data.s': '811a8037fc3ded3891e44b55d6d9b8fbe2159321df0fd20b31cfb2227d8c672d',
                  'asm/matchings/main/menu_speaker_girl/get_g_name.s': 'ba3881f9a3b381365fb1e3143aa4acff0e2c832cbd5f699f747ea4881ff0e51b',
                  'asm/matchings/main/menu_speaker_girl/get_p_name.s': 'd31286db8f496a0d1a99a3d5342654103632227dd0f6b59e457eec54c9b56244',
                  'config/symbol_addrs.txt': '003c6169775031a58015faebba3d8674754a5d3ed166a9b06f9528055cbc3bfe',
                  'config/symbol_addrs_main.txt': 'd9024a12793c93d95f9c3ff82088b515f0f0891495a73204cd4a58c3cbad2a90',
                  'include/common.h': 'dc411aae03844dfa54650a626636ec55f2ce0c67f3ee099afb76e158ee0edfc1',
                  'include/include_asm.h': 'f0b8655093a7dac35e22ae67a63743121f275cd7427e0f61f5e642303acc8371',
                  'include/main/club.h': '53176ba8ea59b8f005f2f8f8f4ae127cce26c73aaf40bbab05dc7612f5734ab4',
                  'include/main/girl.h': 'e605fa35dd623771ffcbd60070b7edef7a8beb07942ed19a826be7391c77a8fd',
                  'include/main/import_views/entry_ui_scenes.h': 'f6ab201e31749a0a649a40cd51f54123fde6967a6d2bca6d0a309326ab3cfedf'}},
 {'id': 'name-entry-terms',
  'path': 'CDROM/EXEDIR/NAME_ENT.EXN',
  'sha1': 'e9a9a0f059dc0029e292363ecf09cffb4068dad9',
  'load_address': 2148737024,
  'header_size': 0,
  'tables': [{'id': 'hiragana_grid', 'address': 2148846888, 'count': 17, 'kind': 'pointer_strings'},
             {'id': 'katakana_grid', 'address': 2148846956, 'count': 17, 'kind': 'pointer_strings'},
             {'id': 'reading_labels', 'address': 2148847024, 'count': 45, 'kind': 'pointer_strings'},
             {'id': 'preset_family_names',
              'address': 2148847212,
              'count': 15,
              'kind': 'fixed_strings',
              'stride': 8},
             {'id': 'preset_given_names',
              'address': 2148847332,
              'count': 15,
              'kind': 'fixed_strings',
              'stride': 8},
             {'id': 'preset_nicknames',
              'address': 2148847452,
              'count': 15,
              'kind': 'fixed_strings',
              'stride': 12}],
  'evidence': ['src/ovl/name_entry/name_entry.c::name_entry_preset_apply',
               'src/ovl/name_entry/name_entry.c::name_entry_character_put',
               'include/ovl/name_entry.h:NameEntryKanaPage'],
  'sha256': 'f7384ad3617361282638ffd9bc1800bb6655d43ddee37ffe257de2ab485cd4a2',
  'data_proofs': [{'label': 'name_entry_hiragana_rows',
                   'address': 2148846888,
                   'length': 68,
                   'assembly_path': 'asm/ovl/NAME_ENT/data/name_entry/name_entry.data.s'},
                  {'label': 'name_entry_katakana_rows',
                   'address': 2148846956,
                   'length': 68,
                   'assembly_path': 'asm/ovl/NAME_ENT/data/name_entry/name_entry.data.s'},
                  {'label': 'name_entry_reading_labels',
                   'address': 2148847024,
                   'length': 180,
                   'assembly_path': 'asm/ovl/NAME_ENT/data/name_entry/name_entry.data.s'},
                  {'label': 'name_entry_preset_family_names',
                   'address': 2148847212,
                   'length': 120,
                   'assembly_path': 'asm/ovl/NAME_ENT/data/name_entry/name_entry.data.s'},
                  {'label': 'name_entry_preset_given_names',
                   'address': 2148847332,
                   'length': 120,
                   'assembly_path': 'asm/ovl/NAME_ENT/data/name_entry/name_entry.data.s'},
                  {'label': 'name_entry_preset_nicknames',
                   'address': 2148847452,
                   'length': 180,
                   'assembly_path': 'asm/ovl/NAME_ENT/data/name_entry/name_entry.data.s'}],
  'source_checks': [{'path': 'include/ovl/name_entry.h', 'contains': 'u8 *text[17];'},
                    {'path': 'include/ovl/name_entry.h', 'contains': 'u8 *name_entry_reading_labels[45];'},
                    {'path': 'src/ovl/name_entry/name_entry.c',
                     'contains': 'preset_row = preset_index % 15;'},
                    {'path': 'src/ovl/name_entry/name_entry.c',
                     'contains': 'name_entry_preset_family_names + preset_row * 8, 8'},
                    {'path': 'src/ovl/name_entry/name_entry.c',
                     'contains': 'name_entry_preset_given_names + preset_row * 8, 8'},
                    {'path': 'src/ovl/name_entry/name_entry.c',
                     'contains': 'name_entry_preset_nicknames + preset_row * 0xC, 0xC'}],
  'code_proofs': [{'assembly_path': 'asm/ovl/NAME_ENT/matchings/name_entry/name_entry/name_entry_preset_apply.s',
                   'source_path': 'CDROM/EXEDIR/NAME_ENT.EXN',
                   'source_sha256': 'f7384ad3617361282638ffd9bc1800bb6655d43ddee37ffe257de2ab485cd4a2',
                   'load_address': 2148737024,
                   'header_size': 0},
                  {'assembly_path': 'asm/ovl/NAME_ENT/matchings/name_entry/name_entry/name_entry_load_init.s',
                   'source_path': 'CDROM/EXEDIR/NAME_ENT.EXN',
                   'source_sha256': 'f7384ad3617361282638ffd9bc1800bb6655d43ddee37ffe257de2ab485cd4a2',
                   'load_address': 2148737024,
                   'header_size': 0},
                  {'assembly_path': 'asm/ovl/NAME_ENT/matchings/name_entry/name_entry/name_entry_load_fade_in.s',
                   'source_path': 'CDROM/EXEDIR/NAME_ENT.EXN',
                   'source_sha256': 'f7384ad3617361282638ffd9bc1800bb6655d43ddee37ffe257de2ab485cd4a2',
                   'load_address': 2148737024,
                   'header_size': 0},
                  {'assembly_path': 'asm/ovl/NAME_ENT/matchings/name_entry/name_entry/name_entry_kanji_page_draw.s',
                   'source_path': 'CDROM/EXEDIR/NAME_ENT.EXN',
                   'source_sha256': 'f7384ad3617361282638ffd9bc1800bb6655d43ddee37ffe257de2ab485cd4a2',
                   'load_address': 2148737024,
                   'header_size': 0},
                  {'assembly_path': 'asm/ovl/NAME_ENT/matchings/name_entry/name_entry/name_entry_character_put.s',
                   'source_path': 'CDROM/EXEDIR/NAME_ENT.EXN',
                   'source_sha256': 'f7384ad3617361282638ffd9bc1800bb6655d43ddee37ffe257de2ab485cd4a2',
                   'load_address': 2148737024,
                   'header_size': 0},
                  {'assembly_path': 'asm/ovl/NAME_ENT/matchings/name_entry/name_entry/name_entry_name_finish.s',
                   'source_path': 'CDROM/EXEDIR/NAME_ENT.EXN',
                   'source_sha256': 'f7384ad3617361282638ffd9bc1800bb6655d43ddee37ffe257de2ab485cd4a2',
                   'load_address': 2148737024,
                   'header_size': 0}],
  'source_pins': {'asm/ovl/NAME_ENT/data/name_entry/name_entry.data.s': '02c5c37160e8c16d1c692b03123a090fb0dfce6d9f1fd5674613897617353c9c',
                  'asm/ovl/NAME_ENT/matchings/name_entry/name_entry/name_entry_character_put.s': 'bf7c367f8cc9ad150ca7a1d2e55c6a593ad02af5e43e69fc57e4aaaa5d27c139',
                  'asm/ovl/NAME_ENT/matchings/name_entry/name_entry/name_entry_kanji_page_draw.s': '3718e78759cbaf2d75f35e332e032b45369cd2f12f13f8d480d4d878bb419f25',
                  'asm/ovl/NAME_ENT/matchings/name_entry/name_entry/name_entry_load_fade_in.s': 'a1b1dfccf4fdf7eea137526981dbba8946a19dd24f777915d401a481173479ea',
                  'asm/ovl/NAME_ENT/matchings/name_entry/name_entry/name_entry_load_init.s': '4964f5e1ad356c53c32ef8449353315743dcfa6ea521f4996eecf1c9eb13a0d3',
                  'asm/ovl/NAME_ENT/matchings/name_entry/name_entry/name_entry_name_finish.s': '45e4f42e431153ab70d5ae7d1d6fefefb99024042e677d9a894d78e033df380a',
                  'asm/ovl/NAME_ENT/matchings/name_entry/name_entry/name_entry_preset_apply.s': '7f59b34ae3529f07c6893f1005ee9e6791e07dec98c228ff05356ed100b6f5aa',
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
                  'include/main/text.h': '3f79601270d9515b5714dc928f59196c1c84a7223f86321e4f49f23cc0ee5eb7',
                  'include/main/tpage.h': '277579106126d3d1e8b722dec269e7a7f9c8d47956ceac121253a5d54d74d4d0',
                  'include/main/vram.h': 'cbed858bdfd1e38a6711c3a798bdc773be6dc849863a597d9ffa062fd669f2f3',
                  'include/ovl/name_entry.h': 'ced2aa4851cd232c5e3682c99c9cfa944d2b71daa78fe9c48911b134cd60f842',
                  'include/psx_types.h': '2fcbf5a7b0db1a1ccd0bfdb691144ebe815b18a430ce92d3aaa2b4fc76223fc4',
                  'src/ovl/name_entry/name_entry.c': '840feed25a5b04687563e45a67eac1998fca9b050cdd59f80ad3a5f6e52cd35d'}}]


def parse_dictionary(data: bytes, profile: dict) -> dict:
    """Read explicit pointer/fixed-width term tables; retain padding and slots."""
    import hashlib
    if profile.get("sha1") and hashlib.sha1(data).hexdigest() != profile["sha1"]:
        raise TextFormatError("dictionary source differs from verified layout")
    digest = sha256(data)
    base, bias = profile["load_address"], profile["header_size"]
    records = []
    for table in profile["tables"]:
        if table["kind"] not in ("pointer_strings", "fixed_strings"):
            raise TextFormatError("unsupported dictionary slot kind")
        root = table["address"] - base + bias
        stride = 4 if table["kind"] == "pointer_strings" else table["stride"]
        if root < 0 or root + table["count"] * stride > len(data):
            raise TextFormatError("dictionary table outside source")
        for index in range(table["count"]):
            slot = root + index * stride
            record = {"id": f"dictionary:{profile['path']}:{table['id']}:{index:02d}",
                      "table": table["id"], "index": index, "source_path": profile["path"],
                      "source_sha256": digest, "slot_offset": slot}
            if table["kind"] == "pointer_strings":
                pointer = struct.unpack_from("<I", data, slot)[0]
                start = pointer - base + bias
                if not 0 <= start < len(data):
                    raise TextFormatError("dictionary string pointer outside source")
                end = data.find(b"\0", start, min(start + 65536, len(data)))
                if end < 0:
                    raise TextFormatError("dictionary string lacks terminator")
                record.update(pointer_value=pointer, slot_raw_hex=data[slot:slot + 4].hex())
            elif table["kind"] == "fixed_strings":
                start = slot
                end = data.find(b"\0", start, start + stride)
                if end < 0:
                    raise TextFormatError("fixed dictionary slot lacks terminator")
                record.update(stride=stride, slot_raw_hex=data[slot:slot + stride].hex(),
                              padding_raw_hex=data[end + 1:slot + stride].hex())
            else:
                raise TextFormatError("unsupported dictionary slot kind")
            raw = data[start:end]
            view = tokenize_fixed_text(raw, "load_string")
            if view["source_decoded_text"] is None:
                raise TextFormatError("dictionary has unsupported source encoding")
            record.update(offset=start, length=len(raw), text=view)
            records.append(record)
    return {"schema": "tokimemo.dictionary.v1", "profile": profile,
            "source_sha256": digest, "records": records,
            "coverage": {"tables": len(profile["tables"]), "records": len(records)}}
