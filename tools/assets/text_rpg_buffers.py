"""Read five source-proven RPG initial mutable text-buffer slots.

Views describe original initialized storage. They do not establish runtime
contents, execution, or preservation across producer writes.
"""
from __future__ import annotations

import hashlib
import re
from pathlib import Path

from .text import TextFormatError, source_path
from .text_binary import _spans
from .text_fixed import tokenize_fixed_text
from .text_source import _decoded_view

_LABEL = re.compile(r'^\s*(dlabel|enddlabel)\s+(\w+)\s*$')
_WORD = re.compile(r'^\s*/\*\s*([0-9A-Fa-f]+)\s+([0-9A-Fa-f]{8})\s+([0-9A-Fa-f]{8})\s*\*/\s*\.word\s+(\w+)\s*$')
_INSTRUCTION = re.compile(r'/\*\s*[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})\s+([0-9A-Fa-f]{8})\s*\*/\s*([^\s]+)')


def _sources(repository: Path, profile: dict) -> dict[str, Path]:
    if not profile.get('source_pins'):
        raise TextFormatError('RPG initial-buffer reader requires pinned source evidence')
    result = {}
    for relative, digest in profile['source_pins'].items():
        source_path(relative)
        try:
            path = (repository / relative).resolve(strict=True)
        except OSError as exc:
            raise TextFormatError('RPG initial-buffer source evidence is missing') from exc
        if not path.is_relative_to(repository) or not path.is_file():
            raise TextFormatError('RPG initial-buffer source path escapes repository')
        raw = path.read_bytes()
        if hashlib.sha256(raw).hexdigest() != digest:
            raise TextFormatError('RPG initial-buffer source differs from reviewed pin')
        try:
            text = raw.decode('utf-8', errors='strict')
        except UnicodeDecodeError as exc:
            raise TextFormatError('RPG initial-buffer evidence is not strict UTF-8') from exc
        if '\ufffd' in text or text.encode('utf-8') != raw:
            raise TextFormatError('RPG initial-buffer evidence does not round-trip UTF-8')
        result[relative] = path
    return result


def parse_rpg_initial_buffers(data: bytes, repository, profile=None) -> dict:
    """Return initial body/full-slot views with independently verified evidence."""
    profile = RPG_INITIAL_BUFFER_PROFILE if profile is None else profile
    data = bytes(data)
    digest = hashlib.sha256(data).hexdigest()
    if digest != profile['sha256'] or len(data) != profile['size']:
        raise TextFormatError('RPG original input differs from pinned initial-buffer profile')
    source_path(profile['path'])
    repo = Path(repository).resolve(strict=True)
    sources = _sources(repo, profile)
    base, stride = profile['load_address'], profile['stride']
    buffers = profile['buffers']
    if (not isinstance(base, int) or isinstance(base, bool) or
            not isinstance(stride, int) or isinstance(stride, bool) or not 1 <= stride <= 4096 or
            len(buffers) != 5 or len({v['address'] for v in buffers}) != 5):
        raise TextFormatError('RPG initial-buffer profile lacks five bounded distinct slots')
    code = []
    if len(profile.get('code_proofs', [])) != 5:
        raise TextFormatError('RPG initial-buffer reader requires five reviewed function proofs')
    for proof in profile['code_proofs']:
        words = []
        text = sources[proof['assembly_path']].read_text(encoding='utf-8')
        for number, line in enumerate(text.splitlines(), 1):
            match = _INSTRUCTION.search(line)
            if match is None or match[3].startswith('.'):
                continue
            address, raw = int(match[1], 16), bytes.fromhex(match[2])
            offset = address - base
            if (offset < 0 or data[offset:offset + 4] != raw or
                    words and address != words[-1]['address'] + 4):
                raise TextFormatError('RPG initial-buffer instruction differs from original image')
            words.append({'address': address, 'offset': offset,
                          'raw_sha256': hashlib.sha256(raw).hexdigest(), 'assembly_line': number})
        label = re.search(r'^\s*glabel\s+(\w+)\s*$', text, re.M)
        if (label is None or label[1] != proof['function'] or not words or
                words[0]['address'] != proof['address'] or len(words) != proof['instruction_words']):
            raise TextFormatError('RPG initial-buffer function identity/extent differs from reviewed proof')
        code.append({**proof, 'verified_instruction_words': len(words), 'instructions': words})
    table = profile['pointer_table']
    assembly = sources[table['assembly_path']]
    active = False
    blocks = []
    for number, line in enumerate(assembly.read_text(encoding='utf-8').splitlines(), 1):
        label = _LABEL.fullmatch(line)
        if label:
            active = label[1] == 'dlabel' and label[2] == table['label']
            if active:
                blocks.append([])
        elif active and line.strip():
            blocks[-1].append((number, line))
    if len(blocks) != 1 or len(blocks[0]) != 5:
        raise TextFormatError('RPG initial-buffer pointer table lacks exactly five initialized words')
    rows = []
    used = bytearray(len(data))
    for index, (number, line) in enumerate(blocks[0]):
        match = _WORD.fullmatch(line)
        if match is None:
            raise TextFormatError('RPG initial-buffer pointer initializer is unsupported')
        offset, address, raw = int(match[1], 16), int(match[2], 16), bytes.fromhex(match[3])
        target = int.from_bytes(raw, 'little')
        if (address != table['address'] + index * 4 or offset != address - base or offset < 0 or
                data[offset:offset + 4] != raw or target != buffers[index]['address'] or
                match[4] != buffers[index]['label']):
            raise TextFormatError('RPG initial-buffer pointer word differs from source/profile/original image')
        used[offset:offset + 4] = b'\1' * 4
        rows.append({'index': index, 'address': address, 'offset': offset, 'raw_hex': raw.hex(),
                     'target_address': target, 'target_label': match[4], 'assembly_line': number})
    spans = list(_spans(assembly, repo, (data, base, 0, digest, profile['path'])))
    records, physical, issues = [], [], []
    previous_end = -1
    for index, slot in enumerate(buffers):
        offset = slot['address'] - base
        if (not isinstance(slot['address'], int) or isinstance(slot['address'], bool) or
                offset < 0 or offset + stride > len(data) or offset < previous_end):
            raise TextFormatError('RPG initial-buffer slots overlap or escape original image')
        previous_end = offset + stride
        matches = [s for s in spans if s['label'] == slot['label'] and s['address'] == slot['address']]
        if len(matches) != 1 or len(matches[0]['raw']) != stride:
            raise TextFormatError('RPG initial-buffer label does not have the exact declared initialized extent')
        raw = bytes(matches[0]['raw'])
        if data[offset:offset + stride] != raw:
            raise TextFormatError('RPG initial-buffer bytes differ from original input')
        end = raw.find(b'\0')
        if end < 0:
            raise TextFormatError('RPG initial-buffer string has no NUL inside its declared slot')
        body, padding = raw[:end], raw[end + 1:]
        text = tokenize_fixed_text(body, 'set_kanji_string')
        parent = f"rpg-initial-storage:{profile['path']}:{slot['address']:08x}:{hashlib.sha256(raw).hexdigest()[:12]}"
        identifier = 'rpg-buffer-initial:' + parent + ':body'
        flags = {'initial_only': True, 'initial_contents_only': True, 'source_slot_initial_only': True,
                 'runtime_value_unproven': True, 'runtime_contents_unknown': True, 'runtime_read_value_proven': False,
                 'runtime_reachability_proven': False, 'runtime_execution_proven': False, 'execution_proven': False}
        full_tokens = [dict(t) for t in text['tokens']]
        full_tokens.append({'kind': 'protected_source_terminator', 'offset': end,
                            'raw_hex': '00', 'translatable': False})
        if padding:
            full_tokens.append({'kind': 'protected_slot_padding', 'offset': end + 1,
                                'raw_hex': padding.hex(), 'translatable': False})
        physical.append({'id': parent, 'raw_hex': raw.hex(), 'raw_length': stride,
            'source': {'path': profile['path'], 'sha256': digest, 'file_size': len(data),
                       'address': slot['address'], 'offset': offset, 'length': stride},
            **_decoded_view(raw, False), 'terminator_relative_offset': end,
            'assembly_path': table['assembly_path'], 'assembly_label': slot['label'], **flags})
        records.append({'id': identifier, 'index': index, 'source_path': profile['path'],
            'source_sha256': digest, 'source_physical_record_id': parent, 'offset': offset,
            'length': len(body), 'text': text, 'full_offset': offset, 'full_address': slot['address'],
            'full_length': stride, 'full_raw_hex': raw.hex(), 'full_tokens': full_tokens,
            'terminator_offset': offset + end, 'padding_offset': offset + end + 1,
            'padding_length': len(padding), 'padding_raw_hex': padding.hex(),
            'source_slot_status': 'initial_mutable_storage', **flags,
            'context': {**flags, 'table_address': table['address'], 'pointer_index': index,
                'pointer_word': rows[index], 'renderer': 'set_kanji_string',
                'renderer_binding_status': 'potential_source_reader_association',
                'renderer_model_only': True, 'tokenizer_model': 'set_kanji_string',
                'renderer_association': 'potential read through installed pointer table; actual runtime contents unproved',
                'requires_no_overwrite_assumption': False, 'original_extent_is_translation_limit': False}})
        if text['source_decoded_text'] is None:
            issues.append({'code': 'initial_source_encoding_unsupported', 'record_id': identifier})
        if any(t['kind'] in {'renderer_rejected_pair', 'not_rendered_after_rejected_pair',
                             'unmapped_numeric_glyph'} for t in text['tokens']):
            issues.append({'code': 'initial_renderer_code_unit_rejected', 'record_id': identifier})
        used[offset:offset + stride] = b'\1' * stride
    _sources(repo, profile)
    unclaimed = []
    cursor = 0
    while cursor < len(data):
        if used[cursor]:
            cursor += 1
            continue
        start = cursor
        while cursor < len(data) and not used[cursor]:
            cursor += 1
        raw = data[start:cursor]
        unclaimed.append({'offset': start, 'length': len(raw),
                          'sha256': hashlib.sha256(raw).hexdigest(), 'classification': 'uninterpreted_original_bytes'})
    return {'schema': 'tokimemo.rpg-initial-buffers.v1', 'profile': profile,
        'source': {'path': profile['path'], 'sha256': digest, 'size': len(data), 'load_address': base},
        'records': records, 'physical_records': physical, 'pointer_table': {**table, 'rows': rows},
        'code_proofs': code, 'issues': issues, 'unclaimed_ranges': unclaimed,
        'coverage': {'initial_buffer_views': len(records), 'claimed_bytes': sum(used),
            'source_decode_failures': sum(r['text']['source_decoded_text'] is None for r in records),
            'renderer_anomaly_records': [i['record_id'] for i in issues if i['code'] == 'initial_renderer_code_unit_rejected'],
            'unclaimed_bytes': len(data) - sum(used), 'whole_file_text_absence_proven': False,
            'serialized_selected_bytes': sum(used), 'unclaimed_payload_serialized': False,
            'serialized_whole_file_reconstructible': False,
            'runtime_execution_proven': False, 'unconditional_category_promotions': 0},
        'limits': ['Original initial file storage only; runtime producer writes and read values are unproved.',
                   'Original buffer extents are provenance, not future translation layout limits.',
                   'Only selected slot/pointer bytes are serialized; other ranges retain identity metadata only.']}


RPG_INITIAL_BUFFER_PROFILE = {'id': 'rpg-original-initial-enemy-buffers',
 'path': 'CDROM/EXEDIR/RPG_BAT.EXN',
 'sha256': '59a6d1687e59132017eb7212fd6560b2e96bddad0a19ecac0929517b9ec4944c',
 'size': 196608,
 'load_address': 2148737024,
 'stride': 40,
 'pointer_table': {'assembly_path': 'asm/ovl/RPG_BAT/data/RPG_BAT_data.data.s',
                   'label': 'rpg_text_enemy_pointers',
                   'address': 2148919536},
 'buffers': [{'label': 'rpg_text_enemy_0', 'address': 2148919336},
             {'label': 'rpg_text_enemy_1', 'address': 2148919376},
             {'label': 'rpg_text_enemy_2', 'address': 2148919416},
             {'label': 'rpg_text_enemy_3', 'address': 2148919456},
             {'label': 'D_8015E8C8', 'address': 2148919496}],
 'code_proofs': [{'function': 'rpg_message_show',
                  'assembly_path': 'asm/ovl/RPG_BAT/matchings/rpg_battle/hud_messages_and_swimming_effect/rpg_message_show.s',
                  'address': 2148841272,
                  'instruction_words': 25},
                 {'function': 'rpg_message_update',
                  'assembly_path': 'asm/ovl/RPG_BAT/matchings/rpg_battle/hud_messages_and_swimming_effect/rpg_message_update.s',
                  'address': 2148841372,
                  'instruction_words': 194},
                 {'function': 'rpg_message_format_damage',
                  'assembly_path': 'asm/ovl/RPG_BAT/matchings/rpg_battle/hud_messages_and_swimming_effect/rpg_message_format_damage.s',
                  'address': 2148847900,
                  'instruction_words': 153},
                 {'function': 'rpg_enemy_alien_chatter',
                  'assembly_path': 'asm/ovl/RPG_BAT/matchings/rpg_battle/enemy_alien/rpg_enemy_alien_chatter.s',
                  'address': 2148887744,
                  'instruction_words': 46},
                 {'function': 'rpg_stage_fight',
                  'assembly_path': 'asm/ovl/RPG_BAT/matchings/rpg_battle/battle_dispatch/rpg_stage_fight.s',
                  'address': 2148776208,
                  'instruction_words': 186}],
 'source_pins': {'asm/ovl/RPG_BAT/data/RPG_BAT_data.data.s': 'fcc246eee68d0e13f4275ab9f83333580b7bc10ec13eda17c7f4a38d27d4b25c',
                 'asm/ovl/RPG_BAT/matchings/rpg_battle/battle_dispatch/rpg_stage_fight.s': '44e90d408b398a957dffff7e06db10e72f69110c775107f15c87e3c2f0d56647',
                 'asm/ovl/RPG_BAT/matchings/rpg_battle/enemy_alien/rpg_enemy_alien_chatter.s': 'a314489ef4dc9f6a4290a6cac5f7eeed6f4b7fe9d02819c0a45ce2503cfeba31',
                 'asm/ovl/RPG_BAT/matchings/rpg_battle/hud_messages_and_swimming_effect/rpg_message_format_damage.s': '8f7a648d6b25ab2adda28048ae8f98bb86adfbb8d3ca03bc0ca761ae7e289999',
                 'asm/ovl/RPG_BAT/matchings/rpg_battle/hud_messages_and_swimming_effect/rpg_message_show.s': '8c45f5a264d903fe3d113ff5678a82dc9da9325b74446f60b6e7288b68ca4814',
                 'asm/ovl/RPG_BAT/matchings/rpg_battle/hud_messages_and_swimming_effect/rpg_message_update.s': '0ca3e47c12839a49150ef2dc61f104d2fe26606d5c74500574514751550af928',
                 'config/symbols_RPG_BAT.txt': 'b7ed898cf3d840d2a365856626fd9dc3eb7647054e65c2b899892bef751d3cb8',
                 'include/common.h': 'dc411aae03844dfa54650a626636ec55f2ce0c67f3ee099afb76e158ee0edfc1',
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
                 'include/game/schedule_records.h': 'c895d5dc8902d05039261caa24e3347d02ba5f4b3e1708a4ec855ae6a79311cd',
                 'include/game/system_data.h': '5754c2cbc7969b308dafd57593648ab1f96d7191e80813165b85f77325f7c48c',
                 'include/game/word_views.h': '8a21797a38272607105b2ceef422c337e928f0b0d066aff5c81dfc6ec8bb0498',
                 'include/game_enums.h': 'cf35db6941db5bdae992b0228040a77336dce7a2f951d2427b7e7a71cd204987',
                 'include/include_asm.h': 'f0b8655093a7dac35e22ae67a63743121f275cd7427e0f61f5e642303acc8371',
                 'include/libapi.h': 'e4c93af4e724109d86a3532e61cdd415c93bdca6d439149a393ebbf5a1a9065e',
                 'include/libgpu.h': 'f0197b81d8f9b4889c0abd2dd0070c536f7cdf5da09b74c3d5fbc10090fe5f27',
                 'include/libgte.h': '368db521cf1db72d862d6e9c1d23acdf745ec16dc844c95345993706fe6c1a84',
                 'include/main/actor.h': '4377eb95590cb89931e8a0bfb8b80440bfb312d8bbd1c67e6aceb34519545dba',
                 'include/main/audio.h': '16877729ae9cc510dd3b7c609c71bb1dd97b5dda46e17f8141f318bf6e952c9d',
                 'include/main/bg.h': 'bd778800b70dba2bb6b2811b79086fd3c391b2448dc2ecca808698c5100237bd',
                 'include/main/cd.h': '2175ff58a8673adbeb77cb78372f0517fa5913c77e363c277d0ecb766ced2b58',
                 'include/main/cursor.h': 'b4e359b7c6278fce81566e7042e872f2025d0bfb5d2ffa2c83595fb1e02c3164',
                 'include/main/draw.h': 'b57934cf272e8d85f7b5c43369307702181b026e202d4a04ac56d9904bf93b99',
                 'include/main/game_mode.h': '104f931cec5e4b9133baccca3eeea596d4966e46d24bf719a082a7931d6a8da5',
                 'include/main/girl.h': 'e605fa35dd623771ffcbd60070b7edef7a8beb07942ed19a826be7391c77a8fd',
                 'include/main/hud.h': 'dd5a7374a8469741052dc7abb8162fe98901eccbdaeb008f1aa97ecd48b84fd3',
                 'include/main/import_views/entry_ui.h': 'eac3a572e784c88406acc5b4c40c019df3888aadd6df5c6061baeb09bfa49129',
                 'include/main/import_views/entry_ui_scenes_school_events.h': '1efff14748e011b3c4980a3636735e0d90c3e4e6dd7af2597ba0beb13c454fbb',
                 'include/main/import_views/minigame.h': 'e26757bdafabd1b29988400d8908853934868eb3dedee7a420a2ac51510d11f7',
                 'include/main/import_views/scenes.h': 'e6e7abb06e10ecefdc007011ef75dc7e386b98344d1b6059542382812241b5ea',
                 'include/main/import_views/scenes_school_events.h': '887df254ca70274155d79a878a6f94a9f9c2e7ae32c7d2cd87f11f3f997452a4',
                 'include/main/layer.h': '2cca7cdd41d2a6ac4f8ce81adb3285241e802ea181906f4d5f086b969493eb06',
                 'include/main/menu.h': 'b171ce815adeee4fdb2c4f86258ebcc2b7c1b75eafc2f26ce7e9dd9494ca35fe',
                 'include/main/message.h': '71ce494eca28a75312cd49c2ba41fdfb8e1f9df7732063f374c6ecfad1fb9c06',
                 'include/main/pad.h': '66db19784afce6a71b067e93b7a50f56be2431bb5ad31ebba4fc24ceb7dcb19c',
                 'include/main/prim.h': 'a5820d7fa0051f4fe561274f5f7bc28f1df6cc87bfd76786a9c69f8a8a3ca732',
                 'include/main/sprite.h': '2b82233ad846ad0615e3dc667573984362a5518810b5b54ec973465f98994ffa',
                 'include/main/state.h': 'aafe390c388f5b2fd36e8a2f3b491cd516364a01c03f258f940cac77b319fd01',
                 'include/main/text.h': '3f79601270d9515b5714dc928f59196c1c84a7223f86321e4f49f23cc0ee5eb7',
                 'include/main/tpage.h': '277579106126d3d1e8b722dec269e7a7f9c8d47956ceac121253a5d54d74d4d0',
                 'include/main/vram.h': 'cbed858bdfd1e38a6711c3a798bdc773be6dc849863a597d9ffa062fd669f2f3',
                 'include/ovl/rpg_battle.h': '2970e816bb70c48cf8876261518624f92569c3aea8def9c86eee0e40e0a4fc9f',
                 'src/ovl/rpg_battle/battle_dispatch.c': '99bffd4f3f4817ac75af2d832fe0af7f528ad478ededff0c1efa6a6acf87e20f',
                 'src/ovl/rpg_battle/enemy_alien.c': '1f47f9dc6999db8753766e2c6db6ad00abee4bf7a255699fe8c5c1db223f9ec6',
                 'src/ovl/rpg_battle/enemy_dispatch.c': '0dd2b2c9aedef2611887123e4c03c6185b96f51cc231002debf54160a493a5ab',
                 'src/ovl/rpg_battle/hud_messages_and_swimming_effect.c': '68c44fa0ca57953986fd8db30e04ad092ce7ab24317b1a3875a7de3e9ae8b943'},
 'evidence_scope': 'Five original initialized mutable slots, potential installed-table reader only; runtime '
                   'contents and execution unproved.',
 'header_closure_scope': 'Local quoted include closure of reviewed C/header roots; conditional declarations '
                         'pinned without inferring execution.'}
