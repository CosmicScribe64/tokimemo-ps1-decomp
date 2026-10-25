"""Verify a native byte-atlas text reader without modeling its glyph output.

The contract does not register an argument role. Callers must require verified
evidence before using the separately exposed model metadata. Runtime execution
and actual read length remain unproved.
"""
from __future__ import annotations

import hashlib
import re
from pathlib import Path
from types import MappingProxyType

from .text import TextFormatError, source_path

def _freeze(value):
    """Keep public proof metadata recursively immutable, including pin maps."""
    if isinstance(value, dict):
        return MappingProxyType({key: _freeze(item) for key, item in value.items()})
    if isinstance(value, (list, tuple)):
        return tuple(_freeze(item) for item in value)
    return value


SHOOTER_TEXT_READ_SPEC = _freeze({
    'index': 0, 'category': 'display', 'renderer': None,
    'renderer_status': 'unimplemented_fixed_renderer', 'conditions': (),
    'read_role': 'display_string', 'callee_address': 0x801432F0,
})

_INSTRUCTION = re.compile(r'/\*\s*([0-9A-Fa-f]+)\s+([0-9A-Fa-f]{8})\s+([0-9A-Fa-f]{8})\s*\*/\s*([^\s]+)')
_GLABEL = re.compile(r'^\s*glabel\s+(\w+)\s*$', re.M)


class _ProofFailure(TextFormatError):
    """Stable failure code, independent of source or input payload values."""


def _file(root: Path, relative: str) -> Path:
    source_path(relative)
    path = (root / relative).resolve(strict=True)
    if not path.is_relative_to(root) or not path.is_file():
        raise _ProofFailure('proof_path_outside_root_or_not_file')
    return path


def _verify(repository, files_root) -> dict:
    profile = SHOOTER_TEXT_CONTRACT
    required = {'source_pins', 'reader_facts', 'path', 'sha256', 'size', 'load_address',
                'header_size', 'assembly_path', 'function', 'entry_address', 'instruction_words'}
    if not required.issubset(profile) or not profile.get('source_pins') or not profile.get('reader_facts'):
        raise _ProofFailure('required_source_or_reader_proof_missing')
    if SHOOTER_TEXT_READ_SPEC.get('callee_address') != profile['entry_address']:
        raise _ProofFailure('read_spec_entry_binding_mismatch')
    repo, inputs = Path(repository).resolve(strict=True), Path(files_root).resolve(strict=True)
    sources = {}
    for relative, expected in profile['source_pins'].items():
        raw = _file(repo, relative).read_bytes()
        if hashlib.sha256(raw).hexdigest() != expected:
            raise _ProofFailure('reviewed_source_pin_mismatch')
        text = raw.decode('utf-8', errors='strict')
        if '\ufffd' in text or text.encode('utf-8') != raw:
            raise _ProofFailure('source_evidence_not_roundtrip_utf8')
        sources[relative] = text
    image = _file(inputs, profile['path']).read_bytes()
    if hashlib.sha256(image).hexdigest() != profile['sha256'] or len(image) != profile['size']:
        raise _ProofFailure('original_input_hash_or_size_mismatch')
    assembly = sources.get(profile['assembly_path'])
    if assembly is None:
        raise _ProofFailure('renderer_assembly_not_pinned')
    if _GLABEL.findall(assembly) != [profile['function']]:
        raise _ProofFailure('renderer_entry_label_missing_or_ambiguous')
    instructions = []
    words = {}
    for line_number, line in enumerate(assembly.splitlines(), 1):
        match = _INSTRUCTION.search(line)
        if match is None or match[4].startswith('.'):
            continue
        offset, address, raw = int(match[1], 16), int(match[2], 16), bytes.fromhex(match[3])
        if (offset != address - profile['load_address'] + profile['header_size'] or
                offset < profile['header_size'] or image[offset:offset + 4] != raw or
                instructions and address != instructions[-1]['address'] + 4):
            raise _ProofFailure('renderer_instruction_mismatch_or_discontinuity')
        words[address] = int.from_bytes(raw, 'little')
        instructions.append({'address': address, 'offset': offset,
            'raw_sha256': hashlib.sha256(raw).hexdigest(), 'assembly_line': line_number})
    if (not instructions or instructions[0]['address'] != profile['entry_address'] or
            len(instructions) != profile['instruction_words']):
        raise _ProofFailure('renderer_entry_or_instruction_extent_mismatch')
    for fact in profile['reader_facts']:
        word = words.get(fact['address'])
        if word is None:
            raise _ProofFailure('native_reader_instruction_fact_missing')
        decoded = {'opcode': word >> 26, 'rs': word >> 21 & 31,
                   'rt': word >> 16 & 31, 'rd': word >> 11 & 31,
                   'function': word & 63, 'immediate': word & 0xFFFF}
        if any(decoded.get(key) != value for key, value in fact.items() if key != 'address'):
            raise _ProofFailure('native_reader_instruction_fact_mismatch')
    # Recheck every pin after reading to avoid combining source revisions.
    for relative, expected in profile['source_pins'].items():
        if hashlib.sha256(_file(repo, relative).read_bytes()).hexdigest() != expected:
            raise _ProofFailure('source_evidence_changed_during_verification')
    start = instructions[0]['offset']
    length = len(instructions) * 4
    return {'verified': True, 'status': 'verified_source_and_original_native_reader',
        'unit': 'TACO', 'function': profile['function'], 'entry_address': profile['entry_address'],
        'read_spec': dict(SHOOTER_TEXT_READ_SPEC),
        'renderer_model_implemented': False, 'native_renderer_kind': 'byte_atlas_sprite_reader',
        'runtime_reachability_proven': False, 'runtime_read_length_proven': False,
        'source_pins_verified': len(sources), 'source_pins': dict(profile['source_pins']),
        'original_input': {'path': profile['path'], 'sha256': profile['sha256'], 'size': len(image)},
        'code_proof': {'assembly_path': profile['assembly_path'],
            'assembly_sha256': profile['source_pins'][profile['assembly_path']],
            'offset': start, 'length': length, 'instruction_words_verified': len(instructions),
            'code_sha256': hashlib.sha256(image[start:start + length]).hexdigest(),
            'instructions': instructions},
        'reader_instruction_facts_verified': len(profile['reader_facts']),
        'limits': ['Potential native display input only; execution and actual read length are unproved.',
                   'Glyph mapping and rendering model are unimplemented; no fixed-renderer equivalence is claimed.']}


def verify_shooter_text_contract(repository, files_root) -> dict:
    """Return verified=False on missing/changed evidence, with no payload output.

    Expected source, input, encoding and proof failures return a structured
    blocker. This API never changes role registries or classifies records.
    """
    try:
        return _verify(repository, files_root)
    except _ProofFailure as exc:
        blocker = str(exc)
    except (OSError, UnicodeError, TextFormatError):
        blocker = 'source_or_original_input_missing_unreadable_or_invalid'
    except (KeyError, TypeError, ValueError):
        blocker = 'invalid_contract_descriptor'
    return {'verified': False, 'status': 'source_evidence_not_verified',
            'unit': 'TACO', 'function': 'shooter_draw_text', 'blockers': [blocker],
            'renderer_model_implemented': False, 'runtime_reachability_proven': False,
            'runtime_read_length_proven': False}


SHOOTER_TEXT_CONTRACT = _freeze({'unit': 'TACO',
 'function': 'shooter_draw_text',
 'path': 'CDROM/EXEDIR/TACO.EXN',
 'sha256': '1eb8bad166897063bbb9c4bb94dafa426d34b56db5aa55e1b4f9b8d998e29b80',
 'size': 196608,
 'load_address': 2148737024,
 'header_size': 0,
 'entry_address': 2148807408,
 'assembly_path': 'asm/ovl/TACO/matchings/shooter_3d/text_assets_and_camera/shooter_draw_text.s',
 'instruction_words': 161,
 'reader_facts': [{'address': 2148807452, 'opcode': 36, 'rs': 4, 'rt': 14, 'immediate': 0},
                  {'address': 2148807460, 'opcode': 0, 'rs': 4, 'rt': 0, 'rd': 17, 'function': 37},
                  {'address': 2148807620, 'opcode': 36, 'rs': 17, 'rt': 3, 'immediate': 0},
                  {'address': 2148807964, 'opcode': 36, 'rs': 17, 'rt': 13, 'immediate': 1},
                  {'address': 2148807968, 'opcode': 9, 'rs': 17, 'rt': 17, 'immediate': 1},
                  {'address': 2148807972, 'opcode': 5, 'rs': 13, 'rt': 0}],
 'source_pins': {'asm/ovl/TACO/matchings/shooter_3d/text_assets_and_camera/shooter_draw_text.s': '0c9b40a1a7b07ac2d1e6f96868599a7350a50d093b45ffb17ef227c9dc9dce93',
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
                 'src/ovl/shooter_3d/text_assets_and_camera.c': '9b69ba724f964b36654f1c7e8d14f086c84a88bf05dae992ab58356df81920ea'},
 'semantic_scope': 'Potential NUL-terminated byte-string sprite-atlas input; glyph rendering model '
                   'unimplemented.'})
