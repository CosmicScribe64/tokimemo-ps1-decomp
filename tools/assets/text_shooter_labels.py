"""Fail-closed native label/ranking input proofs; no glyph model or registry edits."""
from __future__ import annotations

import hashlib
import re
from collections.abc import Mapping
from pathlib import Path

from .text import TextFormatError, source_path
from .text_renderer_proof import _check_proofs
from .text_shooter import _freeze

SHOOTER_LABEL_READ_SPECS = _freeze({
    'shooter_draw_label': {'index': 0, 'category': 'display', 'renderer': None,
        'renderer_status': 'unimplemented_fixed_renderer', 'conditions': (),
        'read_role': 'display_string', 'callee_address': 0x801357B0},
    'shooter_draw_ranking_text': {'index': 0, 'category': 'display', 'renderer': None,
        'renderer_status': 'unimplemented_fixed_renderer', 'conditions': (),
        'read_role': 'display_string', 'callee_address': 0x801371B4},
})

_WORD = re.compile(r'/\*\s*([0-9A-Fa-f]+)\s+([0-9A-Fa-f]{8})\s+([0-9A-Fa-f]{8})\s*\*/\s*([^\s]+)')
_CALLER_SAVED = frozenset(range(1, 16)) | frozenset(range(24, 29)) | {31}
_REQUIRED_FUNCTIONS = frozenset({'shooter_draw_label', 'shooter_draw_ranking_text',
                                 'shooter_draw_ranking_glyph', 'shooter_draw_glyph'})


class _Failure(TextFormatError):
    pass


def _thaw(value):
    if isinstance(value, Mapping):
        return {key: _thaw(item) for key, item in value.items()}
    if isinstance(value, tuple):
        return [_thaw(item) for item in value]
    return value


def _file(root, relative):
    source_path(relative)
    path = (root / relative).resolve(strict=True)
    if not path.is_relative_to(root) or not path.is_file():
        raise _Failure('proof_path_escapes_selected_root')
    return path


def _written_registers(word):
    op, fn, rs, rt, rd = word >> 26, word & 63, word >> 21 & 31, word >> 16 & 31, word >> 11 & 31
    if op == 0:
        if fn == 9:
            return _CALLER_SAVED | ({rd} - {0})
        if fn in {0, 2, 3, 4, 6, 7, 16, 18, 32, 33, 34, 35, 36, 37, 38, 39, 42, 43}:
            return {rd} - {0}
        if fn in {8, 17, 19, 24, 25, 26, 27}:
            return set()
    elif op == 3:
        return _CALLER_SAVED
    elif op == 1:
        return _CALLER_SAVED if rt in {16, 17, 18, 19} else set()
    elif op in {2, 4, 5, 6, 7, 40, 41, 42, 43, 46}:
        return set()
    elif op in {8, 9, 10, 11, 12, 13, 14, 15, 32, 33, 34, 35, 36, 37, 38}:
        return {rt} - {0}
    elif op == 16 and rs == 0:
        return {rt} - {0}
    return None


def _control_edge(word, address):
    """Classify local controls; calls/dynamic transfers require separate models."""
    op, fn, rt = word >> 26, word & 63, word >> 16 & 31
    if op == 3 or op == 0 and fn in {8, 9} or op == 1 and rt in {16, 17, 18, 19}:
        raise _Failure('native_byte_path_crosses_unmodeled_call_or_dynamic_control')
    if op == 2:
        return ((address + 4) & 0xF0000000) | ((word & 0x3FFFFFF) << 2)
    if op in {1, 4, 5, 6, 7}:
        immediate = word & 0xFFFF
        return address + 4 + (immediate - 0x10000 if immediate & 0x8000 else immediate) * 4
    return None


def _verify(repository, files_root):
    descriptor = SHOOTER_LABEL_CONTRACT
    if (not descriptor.get('source_pins') or not descriptor.get('proofs') or
            not descriptor.get('facts') or not descriptor.get('preserved_register_paths')):
        raise _Failure('required_native_label_proofs_missing')
    fact_sites = [(f['function_name'], f['address']) for f in descriptor['facts']]
    if len(set(fact_sites)) != len(fact_sites) or set(fact_sites) != _REQUIRED_FACT_SITES:
        raise _Failure('native_critical_fact_sites_missing_duplicated_or_unreviewed')
    if (set(descriptor['proofs']) != _REQUIRED_FUNCTIONS or
            {f['function_name'] for f in descriptor['facts']} != _REQUIRED_FUNCTIONS):
        raise _Failure('required_native_wrapper_or_primitive_helper_missing')
    flows = descriptor['preserved_register_paths']
    flow_ids = [(f['function_name'], f['register'], f['start_address'], f['end_address']) for f in flows]
    if len(set(flow_ids)) != len(flow_ids) or set(flow_ids) != _REQUIRED_FORWARDING_PATHS:
        raise _Failure('native_byte_forwarding_path_identity_missing_duplicated_or_unreviewed')
    if (len(flows) != 3 or {f['function_name'] for f in flows} !=
            _REQUIRED_FUNCTIONS - {'shooter_draw_glyph'} or any(f['register'] != 4 for f in flows)):
        raise _Failure('required_byte_argument_forwarding_paths_missing')
    repo, inputs = Path(repository).resolve(strict=True), Path(files_root).resolve(strict=True)
    sources = {}
    for relative, expected in descriptor['source_pins'].items():
        raw = _file(repo, relative).read_bytes()
        if hashlib.sha256(raw).hexdigest() != expected:
            raise _Failure('reviewed_source_or_header_pin_mismatch')
        text = raw.decode('utf-8', errors='strict')
        if '\ufffd' in text or text.encode('utf-8') != raw:
            raise _Failure('source_proof_not_roundtrip_utf8')
        sources[relative] = text
    for name, spec in SHOOTER_LABEL_READ_SPECS.items():
        if name not in descriptor['proofs'] or spec['callee_address'] != descriptor['proofs'][name]['entry_address']:
            raise _Failure('native_read_spec_entry_mismatch')
    proofs = _thaw(descriptor['proofs'])
    for proof in proofs.values():
        for field, digest in (('source', 'source_sha256'), ('assembly', 'assembly_sha256')):
            if descriptor['source_pins'].get(proof[field]) != proof[digest]:
                raise _Failure('native_function_evidence_not_in_reviewed_pins')
        _file(inputs, proof['binary'])
    replay = _check_proofs(repo, inputs, proofs)
    if not replay['verified']:
        raise _Failure('shared_original_source_code_replay_failed')
    words_by_function = {}
    for name, proof in proofs.items():
        assembly = sources[proof['assembly']]
        if re.findall(r'^\s*glabel\s+(\w+)\s*$', assembly, re.M) != [name]:
            raise _Failure('native_function_entry_label_mismatch')
        words = {}
        for match in _WORD.finditer(assembly):
            if match[4].startswith('.'):
                continue
            offset, address = int(match[1], 16), int(match[2], 16)
            if (offset != address - proof['load_address'] + proof['header_size'] or
                    words and address != next(reversed(words)) + 4):
                raise _Failure('native_function_instruction_address_discontinuity')
            words[address] = int.from_bytes(bytes.fromhex(match[3]), 'little')
        if not words or next(iter(words)) != proof['entry_address'] or len(words) != proof['verified_instruction_words']:
            raise _Failure('native_function_entry_or_word_extent_mismatch')
        words_by_function[name] = words
    for fact in descriptor['facts']:
        address = fact['address']
        word = words_by_function[fact['function_name']].get(address)
        if word is None:
            raise _Failure('native_byte_call_primitive_fact_missing')
        decoded = {'opcode': word >> 26, 'rs': word >> 21 & 31, 'rt': word >> 16 & 31,
                   'rd': word >> 11 & 31, 'function': word & 63, 'immediate': word & 0xFFFF}
        if word >> 26 == 3:
            decoded['callee_address'] = ((address + 4) & 0xF0000000) | ((word & 0x3FFFFFF) << 2)
        if word >> 26 in {1, 4, 5, 6, 7}:
            immediate = word & 0xFFFF
            decoded['branch_target'] = address + 4 + (immediate - 0x10000 if immediate & 0x8000 else immediate) * 4
        if any(decoded.get(key) != value for key, value in fact.items() if key not in {'function_name', 'address'}):
            raise _Failure('native_byte_call_primitive_fact_mismatch')
    for flow in descriptor.get('preserved_register_paths', ()):
        words = words_by_function[flow['function_name']]
        start, end = flow['start_address'], flow['end_address']
        if (start not in words or end not in words or end + 4 not in words or start >= end or
                words[end] >> 26 != 3 or words[start] >> 26 in {32, 33, 34, 35, 36, 37, 38}):
            raise _Failure('native_byte_argument_capture_or_load_delay_unmodeled')
        for address in range(flow['start_address'] + 4, flow['end_address'], 4):
            if address not in words:
                raise _Failure('native_byte_forwarding_path_missing')
            written = _written_registers(words[address])
            if written is None or flow['register'] in written:
                raise _Failure('native_byte_forwarding_register_overwritten_or_unmodeled')
            target = _control_edge(words[address], address)
            if target is not None and (flow.get('control_edges', {}).get(address) != target or
                    not start + 4 <= target <= end):
                raise _Failure('native_byte_forwarding_control_edge_not_reviewed')
        # JAL captures its argument after the delay instruction. The callee may
        # clobber caller-saved registers only after this argument is consumed.
        delay = words[end + 4]
        written = _written_registers(delay)
        if written is None or flow['register'] in written or _control_edge(delay, end + 4) is not None:
            raise _Failure('native_byte_argument_clobbered_in_call_delay_slot')
    for relative, expected in descriptor['source_pins'].items():
        if hashlib.sha256(_file(repo, relative).read_bytes()).hexdigest() != expected:
            raise _Failure('source_pins_changed_during_native_verification')
    return {'verified': True, 'status': 'verified_native_label_and_ranking_reader_contracts',
        'unit': 'TACO', 'read_specs': {name: dict(spec) for name, spec in SHOOTER_LABEL_READ_SPECS.items()},
        'source_pins_verified': len(sources), 'source_pins': dict(descriptor['source_pins']),
        'code_proof': replay, 'critical_facts_verified': len(descriptor['facts']),
        'byte_forwarding_paths_verified': len(descriptor.get('preserved_register_paths', ())),
        'source_reader_limits': {'shooter_draw_label': {'max_bytes': 21, 'loop_counter_step': 13},
                                 'shooter_draw_ranking_text': {'max_bytes': 21, 'loop_counter_step': 1}},
        'renderer_model_implemented': False, 'runtime_reachability_proven': False,
        'runtime_read_length_proven': False,
        'limits': ['Native byte-atlas primitives only; no fixed renderer or glyph equivalence.',
                   'NUL/21-byte source limits are provenance, not future translation layout limits.',
                   'Execution, chosen runtime paths and actual read lengths are unproved.']}


def verify_shooter_label_contract(repository, files_root):
    """Return a metadata-only verified flag; ordinary proof failures fail closed."""
    try:
        return _verify(repository, files_root)
    except _Failure as exc:
        blocker = str(exc)
    except (OSError, UnicodeError, TextFormatError):
        blocker = 'source_or_original_input_missing_unreadable_or_invalid'
    except (KeyError, TypeError, ValueError):
        blocker = 'invalid_native_label_contract_descriptor'
    return {'verified': False, 'status': 'source_evidence_not_verified', 'unit': 'TACO',
            'blockers': [blocker], 'renderer_model_implemented': False,
            'runtime_reachability_proven': False, 'runtime_read_length_proven': False}


SHOOTER_LABEL_CONTRACT = _freeze({'source_pins': {'asm/ovl/TACO/matchings/shooter_3d/name_entry_and_starfield/shooter_draw_glyph.s': '1c968855e5214f4da7e4de2d27bb8b314e3682201ac4da68c07273c1e77063b2',
                 'asm/ovl/TACO/matchings/shooter_3d/score_rankings/shooter_draw_ranking_glyph.s': 'b6b9832531e3b3e542f477747cf40c31510af8d098f0c658c90259fae82be7bf',
                 'asm/ovl/TACO/matchings/shooter_3d/score_rankings/shooter_draw_ranking_text.s': 'b0bde9fbf48082379a4ae79b85cbeaab4da6e58e52d9959ae523b8d58c802980',
                 'asm/ovl/TACO/matchings/shooter_3d/settings_menu/shooter_draw_label.s': 'a721d4e42619855c9752edb21c1c72420b2cb9286fd8426c39a29d918116f66f',
                 'config/symbols_TACO.txt': '7225f556be06d8528f639ae742efefed1574b69123e62ddae03c4297cb91dee2',
                 'include/common.h': 'dc411aae03844dfa54650a626636ec55f2ce0c67f3ee099afb76e158ee0edfc1',
                 'include/game/appointment.h': 'fefe3a4c97bbb2812989d326d065d3948cb42119ed37d0e4ecce393154e3ec2a',
                 'include/game/calendar.h': '44e07c9582c54980f4f8fe8c54adc061a0b9dcd68a5fd061014241ceb01974e9',
                 'include/game/club.h': '2bfd606956b1baf60ea2e370ad1f11297fd40e721d2303b46cd7915d2b46552d',
                 'include/game/day_event.h': 'efe0cdf794e1bf5f35bd5ff16feeecf3946f4695e98e066c6eca50effdf6c86d',
                 'include/game/draw_records.h': '77cf6a1567a9877f55fe0238566b08af034c921da8c16f503f2c115f68efdcb2',
                 'include/game/game_state.h': 'e58484d902e1c47696a3ae5086fa93626bebc107ab83cb0d8dc91cc0a3e4a6af',
                 'include/game/girl.h': '82c7a558ed145d60e82d57f3a83d331d7fee53a9b89a2edd7c56c284e5c09c58',
                 'include/game/layer.h': 'b0a33122c990abad593818fd9520ebd7f43250a4f93042492d9c12d851f97c29',
                 'include/game/menu_records.h': '1849d0f486659abea223b922fe0d4397e5133387af39f98c50d83780ca9f58fa',
                 'include/game/model_records.h': 'a8e261ca83f07087bb6b7a2e08e1a709d9cdbcb99ebede573c6a3fa84e92c1c0',
                 'include/game/param.h': '627d01af9cc16b4e3a2819610acf9ee1fd174af5ab768c803facf1a9ec86ba55',
                 'include/game/save_image.h': '2b51497cc1a103db1c91400a1b66c3a036bb634630fdad74e6821d8ec98f9a84',
                 'include/game/system_data.h': '5754c2cbc7969b308dafd57593648ab1f96d7191e80813165b85f77325f7c48c',
                 'include/game/word_views.h': '8a21797a38272607105b2ceef422c337e928f0b0d066aff5c81dfc6ec8bb0498',
                 'include/game_enums.h': 'cf35db6941db5bdae992b0228040a77336dce7a2f951d2427b7e7a71cd204987',
                 'include/include_asm.h': 'f0b8655093a7dac35e22ae67a63743121f275cd7427e0f61f5e642303acc8371',
                 'include/libapi.h': 'e4c93af4e724109d86a3532e61cdd415c93bdca6d439149a393ebbf5a1a9065e',
                 'include/libgpu.h': 'f0197b81d8f9b4889c0abd2dd0070c536f7cdf5da09b74c3d5fbc10090fe5f27',
                 'include/libgs.h': '466b559a2e91a91b60448325222f344d376c13d76227e6223379c3678615d10b',
                 'include/libgte.h': '368db521cf1db72d862d6e9c1d23acdf745ec16dc844c95345993706fe6c1a84',
                 'include/libspu.h': '0bb5c3088a838e98ef4f27c6e42e6f4f74cd7fa3943c4f0ee234e4c75fa2f3d8',
                 'include/main/actor.h': '4377eb95590cb89931e8a0bfb8b80440bfb312d8bbd1c67e6aceb34519545dba',
                 'include/main/audio.h': '16877729ae9cc510dd3b7c609c71bb1dd97b5dda46e17f8141f318bf6e952c9d',
                 'include/main/bg.h': 'bd778800b70dba2bb6b2811b79086fd3c391b2448dc2ecca808698c5100237bd',
                 'include/main/boot.h': '9393101d26e9ed59e96da16efbe8eae0abb9c7d2a8c5226047ce6301196763af',
                 'include/main/calendar.h': '8863720cc8b1d3cbb3b0cf536fc83b35bb313616434a31d6093ac6ed47cc2285',
                 'include/main/cd.h': '2175ff58a8673adbeb77cb78372f0517fa5913c77e363c277d0ecb766ced2b58',
                 'include/main/debug.h': '4565888d481634611fed69f0215e97299a49597561b7f9406b805aedf7e2459d',
                 'include/main/draw.h': 'b57934cf272e8d85f7b5c43369307702181b026e202d4a04ac56d9904bf93b99',
                 'include/main/game_mode.h': '104f931cec5e4b9133baccca3eeea596d4966e46d24bf719a082a7931d6a8da5',
                 'include/main/hud.h': 'dd5a7374a8469741052dc7abb8162fe98901eccbdaeb008f1aa97ecd48b84fd3',
                 'include/main/import_views/entry_ui.h': 'eac3a572e784c88406acc5b4c40c019df3888aadd6df5c6061baeb09bfa49129',
                 'include/main/import_views/entry_ui_scenes_school_events.h': '1efff14748e011b3c4980a3636735e0d90c3e4e6dd7af2597ba0beb13c454fbb',
                 'include/main/import_views/minigame.h': 'e26757bdafabd1b29988400d8908853934868eb3dedee7a420a2ac51510d11f7',
                 'include/main/import_views/scenes.h': 'e6e7abb06e10ecefdc007011ef75dc7e386b98344d1b6059542382812241b5ea',
                 'include/main/import_views/scenes_school_events.h': '887df254ca70274155d79a878a6f94a9f9c2e7ae32c7d2cd87f11f3f997452a4',
                 'include/main/layer.h': '2cca7cdd41d2a6ac4f8ce81adb3285241e802ea181906f4d5f086b969493eb06',
                 'include/main/memory_card.h': '7efb3e2cbfab8ca751ebb4dc907450c289d61231aa281c8ec840546e8406a23c',
                 'include/main/message.h': '71ce494eca28a75312cd49c2ba41fdfb8e1f9df7732063f374c6ecfad1fb9c06',
                 'include/main/prim.h': 'a5820d7fa0051f4fe561274f5f7bc28f1df6cc87bfd76786a9c69f8a8a3ca732',
                 'include/main/settings.h': '1c824ac1c30d076264b8a7d13ea97c11c395da9d9c6c77cb5c889568139b1aff',
                 'include/main/sprite.h': '2b82233ad846ad0615e3dc667573984362a5518810b5b54ec973465f98994ffa',
                 'include/main/state.h': 'aafe390c388f5b2fd36e8a2f3b491cd516364a01c03f258f940cac77b319fd01',
                 'include/main/text.h': '3f79601270d9515b5714dc928f59196c1c84a7223f86321e4f49f23cc0ee5eb7',
                 'include/main/tpage.h': '277579106126d3d1e8b722dec269e7a7f9c8d47956ceac121253a5d54d74d4d0',
                 'include/main/view.h': '910777739861b865eea1f03d5a232ba14717dc07904aede3770a48e89d8d3198',
                 'include/main/vram.h': 'cbed858bdfd1e38a6711c3a798bdc773be6dc849863a597d9ffa062fd669f2f3',
                 'include/ovl/shooter_3d.h': '2b3636574f817f18c24b9eb75efa17baf2dfd3bff5449c06c79c0c5a425fdfdb',
                 'include/psx_types.h': '2fcbf5a7b0db1a1ccd0bfdb691144ebe815b18a430ce92d3aaa2b4fc76223fc4',
                 'src/ovl/shooter_3d/name_entry_and_starfield.c': '0180d9615d1762f4be52757179b458f98f23d9bcf270f5e5b9be77ddad819475',
                 'src/ovl/shooter_3d/score_rankings.c': '8bd064894972c652f00d4eb0f421d350efc9124badc55a200114c96ddda58075',
                 'src/ovl/shooter_3d/settings_menu.c': '7c3ba5a6cf7ccb0c83aaf0466774dd830290da7b5ba3134dee0dd1511a64b7d1'},
 'proofs': {'shooter_draw_label': {'unit': 'TACO',
                                   'source': 'src/ovl/shooter_3d/settings_menu.c',
                                   'source_sha256': '7c3ba5a6cf7ccb0c83aaf0466774dd830290da7b5ba3134dee0dd1511a64b7d1',
                                   'assembly': 'asm/ovl/TACO/matchings/shooter_3d/settings_menu/shooter_draw_label.s',
                                   'assembly_sha256': 'a721d4e42619855c9752edb21c1c72420b2cb9286fd8426c39a29d918116f66f',
                                   'binary': 'CDROM/EXEDIR/TACO.EXN',
                                   'binary_sha256': '1eb8bad166897063bbb9c4bb94dafa426d34b56db5aa55e1b4f9b8d998e29b80',
                                   'entry_address': 2148751280,
                                   'load_address': 2148737024,
                                   'header_size': 0,
                                   'verified_instruction_words': 51,
                                   'window_offset': 14256,
                                   'window_size': 204,
                                   'window_sha256': '002c33e862d59e73b58fba6fc5db82efe895656be328973361beca8c8d18ae8c',
                                   'read_arguments': [{'index': 0, 'role': 'display', 'renderer': None}],
                                   'evidence': 'Reviewed native byte input and primitive construction; model '
                                               'unimplemented.'},
            'shooter_draw_ranking_text': {'unit': 'TACO',
                                          'source': 'src/ovl/shooter_3d/score_rankings.c',
                                          'source_sha256': '8bd064894972c652f00d4eb0f421d350efc9124badc55a200114c96ddda58075',
                                          'assembly': 'asm/ovl/TACO/matchings/shooter_3d/score_rankings/shooter_draw_ranking_text.s',
                                          'assembly_sha256': 'b0bde9fbf48082379a4ae79b85cbeaab4da6e58e52d9959ae523b8d58c802980',
                                          'binary': 'CDROM/EXEDIR/TACO.EXN',
                                          'binary_sha256': '1eb8bad166897063bbb9c4bb94dafa426d34b56db5aa55e1b4f9b8d998e29b80',
                                          'entry_address': 2148757940,
                                          'load_address': 2148737024,
                                          'header_size': 0,
                                          'verified_instruction_words': 44,
                                          'window_offset': 20916,
                                          'window_size': 176,
                                          'window_sha256': '7ccb3489b8dcfa59047b95b8be620ec98a093564ff4d2b30e4fc82162c559f07',
                                          'read_arguments': [{'index': 0,
                                                              'role': 'display',
                                                              'renderer': None}],
                                          'evidence': 'Reviewed native byte input and primitive '
                                                      'construction; model unimplemented.'},
            'shooter_draw_ranking_glyph': {'unit': 'TACO',
                                           'source': 'src/ovl/shooter_3d/score_rankings.c',
                                           'source_sha256': '8bd064894972c652f00d4eb0f421d350efc9124badc55a200114c96ddda58075',
                                           'assembly': 'asm/ovl/TACO/matchings/shooter_3d/score_rankings/shooter_draw_ranking_glyph.s',
                                           'assembly_sha256': 'b6b9832531e3b3e542f477747cf40c31510af8d098f0c658c90259fae82be7bf',
                                           'binary': 'CDROM/EXEDIR/TACO.EXN',
                                           'binary_sha256': '1eb8bad166897063bbb9c4bb94dafa426d34b56db5aa55e1b4f9b8d998e29b80',
                                           'entry_address': 2148757104,
                                           'load_address': 2148737024,
                                           'header_size': 0,
                                           'verified_instruction_words': 55,
                                           'window_offset': 20080,
                                           'window_size': 220,
                                           'window_sha256': 'd0c6b679421857a08f698d562aa1aec959a4a04b4923c59b4adb613b3b4d23ac',
                                           'read_arguments': [],
                                           'evidence': 'Reviewed native byte input and primitive '
                                                       'construction; model unimplemented.'},
            'shooter_draw_glyph': {'unit': 'TACO',
                                   'source': 'src/ovl/shooter_3d/name_entry_and_starfield.c',
                                   'source_sha256': '0180d9615d1762f4be52757179b458f98f23d9bcf270f5e5b9be77ddad819475',
                                   'assembly': 'asm/ovl/TACO/matchings/shooter_3d/name_entry_and_starfield/shooter_draw_glyph.s',
                                   'assembly_sha256': '1c968855e5214f4da7e4de2d27bb8b314e3682201ac4da68c07273c1e77063b2',
                                   'binary': 'CDROM/EXEDIR/TACO.EXN',
                                   'binary_sha256': '1eb8bad166897063bbb9c4bb94dafa426d34b56db5aa55e1b4f9b8d998e29b80',
                                   'entry_address': 2148766756,
                                   'load_address': 2148737024,
                                   'header_size': 0,
                                   'verified_instruction_words': 419,
                                   'window_offset': 29732,
                                   'window_size': 1676,
                                   'window_sha256': '509486d4fab32b0c86e513f4b78566304feb34716369be32a675b3ac93261de1',
                                   'read_arguments': [],
                                   'evidence': 'Reviewed native byte input and primitive construction; model '
                                               'unimplemented.'}},
 'facts': [{'function_name': 'shooter_draw_label',
            'address': 2148751320,
            'opcode': 36,
            'rs': 4,
            'rt': 14,
            'immediate': 0},
           {'function_name': 'shooter_draw_label',
            'address': 2148751336,
            'opcode': 4,
            'rs': 14,
            'rt': 0,
            'branch_target': 2148751440},
           {'function_name': 'shooter_draw_label',
            'address': 2148751344,
            'opcode': 0,
            'rs': 4,
            'rt': 0,
            'rd': 16,
            'function': 37},
           {'function_name': 'shooter_draw_label',
            'address': 2148751360,
            'opcode': 12,
            'rs': 14,
            'rt': 4,
            'immediate': 255},
           {'function_name': 'shooter_draw_label',
            'address': 2148751364,
            'opcode': 0,
            'rs': 0,
            'rt': 0,
            'rd': 17,
            'function': 37},
           {'function_name': 'shooter_draw_label',
            'address': 2148751396,
            'opcode': 3,
            'callee_address': 2148766756},
           {'function_name': 'shooter_draw_label',
            'address': 2148751404,
            'opcode': 9,
            'rs': 17,
            'rt': 17,
            'immediate': 13},
           {'function_name': 'shooter_draw_label',
            'address': 2148751408,
            'opcode': 10,
            'rs': 17,
            'rt': 1,
            'immediate': 273},
           {'function_name': 'shooter_draw_label',
            'address': 2148751412,
            'opcode': 9,
            'rs': 16,
            'rt': 16,
            'immediate': 1},
           {'function_name': 'shooter_draw_label',
            'address': 2148751416,
            'opcode': 4,
            'rs': 1,
            'rt': 0,
            'branch_target': 2148751440},
           {'function_name': 'shooter_draw_label',
            'address': 2148751424,
            'opcode': 36,
            'rs': 16,
            'rt': 4,
            'immediate': 0},
           {'function_name': 'shooter_draw_label',
            'address': 2148751428,
            'opcode': 0,
            'rs': 0,
            'rt': 0,
            'rd': 0,
            'function': 0},
           {'function_name': 'shooter_draw_label',
            'address': 2148751432,
            'opcode': 5,
            'rs': 4,
            'rt': 0,
            'branch_target': 2148751372},
           {'function_name': 'shooter_draw_ranking_text',
            'address': 2148757976,
            'opcode': 36,
            'rs': 4,
            'rt': 14,
            'immediate': 0},
           {'function_name': 'shooter_draw_ranking_text',
            'address': 2148757992,
            'opcode': 4,
            'rs': 14,
            'rt': 0,
            'branch_target': 2148758076},
           {'function_name': 'shooter_draw_ranking_text',
            'address': 2148757996,
            'opcode': 0,
            'rs': 0,
            'rt': 0,
            'rd': 17,
            'function': 37},
           {'function_name': 'shooter_draw_ranking_text',
            'address': 2148758000,
            'opcode': 0,
            'rs': 4,
            'rt': 0,
            'rd': 16,
            'function': 37},
           {'function_name': 'shooter_draw_ranking_text',
            'address': 2148758012,
            'opcode': 12,
            'rs': 14,
            'rt': 4,
            'immediate': 255},
           {'function_name': 'shooter_draw_ranking_text',
            'address': 2148758032,
            'opcode': 3,
            'callee_address': 2148757104},
           {'function_name': 'shooter_draw_ranking_text',
            'address': 2148758040,
            'opcode': 9,
            'rs': 17,
            'rt': 17,
            'immediate': 1},
           {'function_name': 'shooter_draw_ranking_text',
            'address': 2148758044,
            'opcode': 10,
            'rs': 17,
            'rt': 1,
            'immediate': 21},
           {'function_name': 'shooter_draw_ranking_text',
            'address': 2148758048,
            'opcode': 9,
            'rs': 16,
            'rt': 16,
            'immediate': 1},
           {'function_name': 'shooter_draw_ranking_text',
            'address': 2148758052,
            'opcode': 4,
            'rs': 1,
            'rt': 0,
            'branch_target': 2148758076},
           {'function_name': 'shooter_draw_ranking_text',
            'address': 2148758060,
            'opcode': 36,
            'rs': 16,
            'rt': 4,
            'immediate': 0},
           {'function_name': 'shooter_draw_ranking_text',
            'address': 2148758064,
            'opcode': 0,
            'rs': 0,
            'rt': 0,
            'rd': 0,
            'function': 0},
           {'function_name': 'shooter_draw_ranking_text',
            'address': 2148758068,
            'opcode': 5,
            'rs': 4,
            'rt': 0,
            'branch_target': 2148758020},
           {'function_name': 'shooter_draw_ranking_glyph',
            'address': 2148757116,
            'opcode': 12,
            'rs': 4,
            'rt': 14,
            'immediate': 255},
           {'function_name': 'shooter_draw_ranking_glyph',
            'address': 2148757124,
            'opcode': 0,
            'rs': 14,
            'rt': 0,
            'rd': 4,
            'function': 37},
           {'function_name': 'shooter_draw_ranking_glyph',
            'address': 2148757300,
            'opcode': 3,
            'callee_address': 2148766756},
           {'function_name': 'shooter_draw_glyph',
            'address': 2148766772,
            'opcode': 12,
            'rs': 4,
            'rt': 17,
            'immediate': 255},
           {'function_name': 'shooter_draw_glyph',
            'address': 2148766820,
            'opcode': 9,
            'rs': 0,
            'rt': 4,
            'immediate': 40},
           {'function_name': 'shooter_draw_glyph',
            'address': 2148766824,
            'opcode': 3,
            'callee_address': 2147782896},
           {'function_name': 'shooter_draw_glyph',
            'address': 2148766832,
            'opcode': 0,
            'rs': 2,
            'rt': 0,
            'rd': 16,
            'function': 37},
           {'function_name': 'shooter_draw_glyph',
            'address': 2148766836,
            'opcode': 3,
            'callee_address': 2148134976},
           {'function_name': 'shooter_draw_glyph',
            'address': 2148766840,
            'opcode': 0,
            'rs': 2,
            'rt': 0,
            'rd': 4,
            'function': 37},
           {'function_name': 'shooter_draw_glyph',
            'address': 2148768380,
            'opcode': 3,
            'callee_address': 2148134608},
           {'function_name': 'shooter_draw_glyph',
            'address': 2148768384,
            'opcode': 0,
            'rs': 16,
            'rt': 0,
            'rd': 5,
            'function': 37},
           {'function_name': 'shooter_draw_glyph',
            'address': 2148768404,
            'opcode': 3,
            'callee_address': 2147783868},
           {'function_name': 'shooter_draw_glyph',
            'address': 2148767200,
            'opcode': 41,
            'rs': 16,
            'rt': 31,
            'immediate': 8}],
 'preserved_register_paths': [{'function_name': 'shooter_draw_label',
                               'register': 4,
                               'start_address': 2148751360,
                               'end_address': 2148751396,
                               'control_edges': {}},
                              {'function_name': 'shooter_draw_ranking_text',
                               'register': 4,
                               'start_address': 2148758012,
                               'end_address': 2148758032,
                               'control_edges': {}},
                              {'function_name': 'shooter_draw_ranking_glyph',
                               'register': 4,
                               'start_address': 2148757124,
                               'end_address': 2148757300,
                               'control_edges': {2148757136: 2148757172,
                                                 2148757148: 2148757164,
                                                 2148757164: 2148757176,
                                                 2148757252: 2148757268,
                                                 2148757280: 2148757296}}],
 'semantic_scope': 'Two NUL/max21 byte input loops feed native textured-quad helpers; glyph model '
                   'unimplemented.'})


# Independent canonical site/path identities reject dropped or duplicated proof facts.
_REQUIRED_FACT_SITES = frozenset({
    ('shooter_draw_glyph', 0x80139434),
    ('shooter_draw_glyph', 0x80139464),
    ('shooter_draw_glyph', 0x80139468),
    ('shooter_draw_glyph', 0x80139470),
    ('shooter_draw_glyph', 0x80139474),
    ('shooter_draw_glyph', 0x80139478),
    ('shooter_draw_glyph', 0x801395e0),
    ('shooter_draw_glyph', 0x80139a7c),
    ('shooter_draw_glyph', 0x80139a80),
    ('shooter_draw_glyph', 0x80139a94),
    ('shooter_draw_label', 0x801357d8),
    ('shooter_draw_label', 0x801357e8),
    ('shooter_draw_label', 0x801357f0),
    ('shooter_draw_label', 0x80135800),
    ('shooter_draw_label', 0x80135804),
    ('shooter_draw_label', 0x80135824),
    ('shooter_draw_label', 0x8013582c),
    ('shooter_draw_label', 0x80135830),
    ('shooter_draw_label', 0x80135834),
    ('shooter_draw_label', 0x80135838),
    ('shooter_draw_label', 0x80135840),
    ('shooter_draw_label', 0x80135844),
    ('shooter_draw_label', 0x80135848),
    ('shooter_draw_ranking_glyph', 0x80136e7c),
    ('shooter_draw_ranking_glyph', 0x80136e84),
    ('shooter_draw_ranking_glyph', 0x80136f34),
    ('shooter_draw_ranking_text', 0x801371d8),
    ('shooter_draw_ranking_text', 0x801371e8),
    ('shooter_draw_ranking_text', 0x801371ec),
    ('shooter_draw_ranking_text', 0x801371f0),
    ('shooter_draw_ranking_text', 0x801371fc),
    ('shooter_draw_ranking_text', 0x80137210),
    ('shooter_draw_ranking_text', 0x80137218),
    ('shooter_draw_ranking_text', 0x8013721c),
    ('shooter_draw_ranking_text', 0x80137220),
    ('shooter_draw_ranking_text', 0x80137224),
    ('shooter_draw_ranking_text', 0x8013722c),
    ('shooter_draw_ranking_text', 0x80137230),
    ('shooter_draw_ranking_text', 0x80137234),
})

_REQUIRED_FORWARDING_PATHS = frozenset({
    ('shooter_draw_label', 4, 0x80135800, 0x80135824),
    ('shooter_draw_ranking_glyph', 4, 0x80136e84, 0x80136f34),
    ('shooter_draw_ranking_text', 4, 0x801371fc, 0x80137210),
})
