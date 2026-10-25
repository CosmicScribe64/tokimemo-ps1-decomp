"""Source-pinned runtime script patch contracts and unresolved writes (T-28001).

This ledger never changes game data. A protected byte slot is not a translation
boundary: changing its position or width requires changing the runtime patch.
Generic AST results are candidate writes, not proof of their ultimate target.
"""
from __future__ import annotations

import hashlib
import re
import struct
import subprocess
from pathlib import Path

from .text_source import REPOSITORY, _lex_c_tokens, _mask_c_comments, _mask_c_literals, _read_original, _string_bytes
from .text import tokenize_text

SOURCE_PINS = {
    "src/ovl/DATE/80149310.c": "d15bba6533f174b6be21f14af5d1c804c2e410738095e379d25d4d0bed8d476a",
    "src/ovl/DATE2/80137360.c": "31353b53f7c439fb31bef46e1d9b5df7ffbab45de359d510273652dd6f5dd7dd",
    "src/ovl/SHUGAKU/801378B0.c": "7535ca51fa77410ea4f6be8b1c01bc6a110d8c12cd86a9fe9782a690766720e1",
    "src/ovl/SHUGAKU/80136380.c": "75915b36e53189a482d1d894f9ddfbc6fa6c19c3849d4bdf82c26496fd974b80",
    "src/ovl/SHOUGATU/801330C0.c": "d18cbcb57fee183767219fb7c9e2e80e3ba53d4a08e3c2bc04e23dce247926fd",
    "src/ovl/SHOUGATU/80132550.c": "0ab133711eea89c078acde9f4e07fa06c72567df76d3cb998d734d9d27c0d21e",
    "src/ovl/SHOUGATU/80132000.c": "cfc9c16f59ad19735be9b29ec7b0aa026eb8ffd98c83122aebc66c6f4b252a92",
}
SHOUGATU_ROUTE = {'assembly_pins': {'asm/matchings/main/80042540/mode_stage_next.s': 'b4ff29353ec3e3d37bc62158fae8b2046d056bcd3e112397f440cfdd7a1c45be',
                   'asm/matchings/main/80042540/mode_step_next.s': 'ce766a77a179a3855b374688e31bbb6efe19b214ecdc72bf8cd439c538cda936',
                   'asm/matchings/main/8007C030/dec_init.s': 'e3c78833d1f144b0465720417cf3d24784b52696c15583bf49fe3a12e0f4eabe',
                   'asm/matchings/main/8007C030/func_8007C784.s': 'deb332d92d8e867fdda66559ae9e9686ec7b5554695deaa6962faee6b5f6b99e',
                   'asm/matchings/main/8007C030/normal_date_move_place.s': 'f2b0c58aa3c6beea4335151a5721dbb676f06ce1127866035a7eb037717ff020',
                   'asm/matchings/main/8007C030/normal_date_two_select.s': '294303264159fb4486afb1facf9ad07839ec3e99a1031380dcd1d0a733720350',
                   'asm/matchings/main/8007C030/portrait_show_checked.s': '35dc116a5e44d2a8336ededd69e164acc35198aacfbad2173ee5e7439a68f3ad',
                   'asm/matchings/main/8007C030/select_girl.s': '8386b1501cbb72902f6b3b7eb89ce9026307f4859ba8fd687b1faed68ae0808f',
                   'asm/matchings/main/8007C030/select_girl_init.s': 'f2fa351006542d744e5eed9ba2c655ad012cd7ff6f8ad648997cb93e7a0ec1d6',
                   'asm/matchings/main/8007C030/select_girl_main.s': '47e917bbaa95c778c381c5154c8991394c60b9e46718b2e7bc87cf758764f9ea',
                   'asm/matchings/main/8007C030/talk_run.s': '9ac545e8512ff28e3a2089839ed02caf124d1c60dca0c84422cfae1f2d9a3929',
                   'asm/matchings/main/8007C030/talk_speak_part.s': 'f481f88598f5d5af80e3e756b7c32d9b20005b13d6c58a04c8651718b8c1f2f1',
                   'asm/ovl/SHOUGATU/matchings/SHOUGATU/80132000/func_801322D4.s': '5b72954dfa49e307368c1789a8bacc61a9dc7076c7cc0b710c7c55ddbb89fcfa',
                   'asm/ovl/SHOUGATU/matchings/SHOUGATU/80132000/func_801323D4.s': '808db2a25c8cb94a3ba06556524a60fac277eaaba2f31a6e0b39cede0c27a084',
                   'asm/ovl/SHOUGATU/matchings/SHOUGATU/80132550/func_80132A34.s': '1e8b4c7c0bb74e092fecc27a423552a4652e6cb4ce33e44ab5278867f573fb10',
                   'asm/ovl/SHOUGATU/matchings/SHOUGATU/801330C0/func_801330C0.s': '77e37d187cb30971f73e676781e3921e239f8734a2063ce9fa7578cf8164c624',
                   'asm/ovl/SHOUGATU/matchings/SHOUGATU/801330C0/func_80133760.s': '6d404c93904c75283971143c5aa4b2b76002af7a4fa288b838cc071d7aa94698',
                   'asm/ovl/SHOUGATU/matchings/SHOUGATU/801330C0/func_80133874.s': '66edbc9645b6b8bd7d63ebe854596c4cd473deadee79b9acb69e5ef56400a88e',
                   'asm/ovl/SHOUGATU/matchings/SHOUGATU/801330C0/func_801338DC.s': '418f46eb7fb8d37031ffe1fb81aaf6a83f1fba182b85bccd822dbbaa0f063c84',
                   'asm/ovl/SHOUGATU/matchings/SHOUGATU/801330C0/func_80133948.s': '41913db475ca02d309b1e31fc013aeabf1968ca2538fc8893c09da4ec27438b4',
                   'asm/ovl/SHOUGATU/matchings/SHOUGATU/801330C0/func_8013397C.s': '23de3a0eee287f630866443c1bb15d97ee42b8f5ec3b0cb80c73645d703b9a32',
                   'asm/ovl/SHOUGATU/matchings/SHOUGATU/801330C0/func_80133B44.s': '3aeee223ce025d7fddd55fac2728fd8c2bb6198be754e80236517f2435d99f2c'},
 'assumptions': ['normal stage-2 dispatch entered through step 0 initialization',
                 'no external writes to mode stage/step, current girl, or SHOUGATU text root during this route',
                 'step 11 successful non-Shiori selection with b3 set maps the root and advances to 14',
                 'step 19 low-seven-bit girl status is not 4, bypassing stage-reset step 21',
                 'required talk FF, background/CD, portrait validation and menu guards complete on the '
                 'source-visible route'],
 'binary_pins': {'CDROM/EXEDIR/SHOUGATU.EXN': '84842bc3e8540ef9853d38ca04bb453eddb60a5a9b14786896ac618b3b84bb13',
                 'SLPM_86.053': 'bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae'},
 'data_assembly': {'address': 2148809668,
                   'label': 'D_80143BC4',
                   'path': 'asm/ovl/SHOUGATU/data/SHOUGATU_data.data.s',
                   'sha256': '48c01e8db891528d26df9c53e5b5d7ffbaa443a5b753cf18a07593dfa00b414a',
                   'word_count': 63,
                   'raw_sha256': 'b9b07ce40102146f0535de68ceaa9889733347555b49832138f7cf63591c2891',
                   'required_slots': {11: 0x80133760, 13: 0x80133874, 19: 0x801338DC, 21: 0x8013397C, 28: 0x80133B44}},
 'dispatch_source': 'CDROM/EXEDIR/SHOUGATU.EXN',
 'excluded_roots': {'0': 'stage reset clears step',
                    '12': 'assigned outside stage-2 table route',
                    '14': 'b3-zero fallback loops 12 to 13 to 9',
                    '2': 'Shiori stores gated off; same-girl early return loops 12 to 13 to 9'},
 'indices': [1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 13],
 'source_pins': {'src/main/80042540.c': 'd7a1ed706202c528e1a760b3f2f6a9efd973250dc66a508ba02ab552a8a2b443',
                 'src/main/8007C030.c': '59d58351fcf8ef55aee74518801ab8a090593e080b725fc5809007c7c8a6a865',
                 'src/ovl/SHOUGATU/80132D00.c': '4d46bf4acde6741e31c47c6b8ad20c4da25c8d17c9236fcbab635aaebc775868',
                 'src/ovl/SHOUGATU/80134120.c': '5db2bc1ef2b8a5f5147a8194d77cae08c6206a593ee4355643680ba596ea6216',
                 'src/ovl/SHOUGATU/80135B90.c': '4a271839d336352011c46b2f3b6b86d863e5f27aa342bc49a80480e818083602'},
 'units': {'CDROM/EXEDIR/SHOUGATU.EXN': {'file_bias': 0, 'load_address': 2148737024},
           'SLPM_86.053': {'file_bias': 2048, 'load_address': 2147749888}}}
SOURCE_PINS.update(SHOUGATU_ROUTE["source_pins"])
SHOUGATU_ROUTE["root_conditions"] = {
    1: ["current_girl == GIRL_YUKO"], 3: ["current_girl == GIRL_YUINA"],
    4: ["current_girl == GIRL_AYAKO"], 5: ["current_girl == GIRL_MIRA"],
    6: ["current_girl == GIRL_MIO"], 7: ["current_girl == GIRL_YUKARI"],
    8: ["current_girl == GIRL_NOZOMI", "(get_g_zyotai_s(GIRL_NOZOMI) & 0x7F) < 2 at root selection"],
    9: ["current_girl == GIRL_NOZOMI", "(get_g_zyotai_s(GIRL_NOZOMI) & 0x7F) >= 2 at root selection"],
    10: ["current_girl == GIRL_MEGUMI"], 11: ["current_girl == GIRL_SAKI"],
    13: ["current_girl == GIRL_YUMI"]}

MUTATION_SPECS = [
    {"id": "date-birthday-initial-glyph", "source": "src/ovl/DATE/80149310.c",
     "function": "func_80149B78", "array": "buf", "write_lines": [247, 248], "length": 2,
     "script": "CDROM/SERIFU/G_BIRTH.BIN", "initializer_source": "src/ovl/DATE/80149310.c",
     "initializer": "func_80149310", "indices": list(range(11)) + [12], "group": 14, "line": 0,
     "conditions": ["D_80122CDC == 0", "root selected by func_801496C4 current_girl switch"],
     "selection": "explicit_switch_roots_with_conditional_nozomi_alternative",
     "evidence": ["src/ovl/DATE/80149310.c:72-112,122-124,150-211,241-249"],
     "pins": ["src/ovl/DATE/80149310.c"]},
    {"id": "date2-birthday-initial-glyph", "source": "src/ovl/DATE2/80137360.c",
     "function": "func_801378A4", "array": "buf", "write_lines": [124, 125], "length": 2,
     "script": "CDROM/SERIFU/G_BIRTH.BIN", "initializer_source": "src/ovl/DATE2/80137360.c",
     "initializer": "func_80137360", "indices": [1], "group": 14, "line": 0,
     "conditions": ["D_80122CDC == 0", "talk_text_table selected as D_8013A774 by func_801376F0"],
     "selection": "explicit_fixed_root", "evidence": ["src/ovl/DATE2/80137360.c:7-47,67-84,118-126"],
     "pins": ["src/ovl/DATE2/80137360.c"]},
    {"id": "school-trip-initial-line-shortening", "source": "src/ovl/SHUGAKU/801378B0.c",
     "function": "func_8013799C", "array": "buf", "write_lines": [32, 33, 34], "length": 3,
     "script": "CDROM/SERIFU/SHUGAKU.BIN", "initializer_source": "src/ovl/SHUGAKU/80136380.c",
     "initializer": "func_80136380", "indices": [11], "group": 2, "line": 0,
     "conditions": ["game_state.player_flags.b[1] & 0xF", "game_state.substep == 0"],
     "selection": "explicit_fixed_root", "evidence": ["src/ovl/SHUGAKU/801378B0.c:25-35,136-141",
                                                         "src/ovl/SHUGAKU/80136380.c:4-43"],
     "pins": ["src/ovl/SHUGAKU/801378B0.c", "src/ovl/SHUGAKU/80136380.c"]},
    {"id": "new-year-selected-line-and-command", "source": "src/ovl/SHOUGATU/801330C0.c",
     "function": "func_80133B44", "array": "sp40", "write_lines": [291, 292, 293, 294], "length": 3,
     "script": "CDROM/SERIFU/SHOGATU.BIN", "initializer_source": "src/ovl/SHOUGATU/80132000.c",
     "initializer": "func_80132000", "runtime_root": "D_80143AF4", "group": 0, "line": 0,
     "command_value": 7,
     "conditions": ["game_state.current_girl != GIRL_SHIORI", "game_state.substep == 0"],
     "indices": SHOUGATU_ROUTE["indices"], "selector_contract": SHOUGATU_ROUTE,
     "selection": "source_proved_conditional_stage2_route",
     "evidence": ["src/ovl/SHOUGATU/801330C0.c:6-12,131-232,275-304",
                  "src/ovl/SHOUGATU/80132550.c:133-202", "src/ovl/SHOUGATU/80132000.c:52-79,89-96",
                  "src/main/80042540.c:146-173", "src/main/8007C030.c:1574-1576,2031-2044",
                  "asm/ovl/SHOUGATU/data/SHOUGATU_data.data.s:417-481"],
     "pins": ["src/ovl/SHOUGATU/801330C0.c", "src/ovl/SHOUGATU/80132550.c", "src/ovl/SHOUGATU/80132000.c"] + list(SHOUGATU_ROUTE["source_pins"])},
]
NAME_BUFFERS = {"name_code_player_form", "name_code_family", "name_code_given", "name_code_girl_short",
                "girl_name_buf", "D_800CA23C", "D_800CA248", "D_800CA250", "D_800CA25C", "D_800CA280"}
ROOT_SEEDS = {"talk_text_table", "talk_command_table", "D_800CA13C", "D_800CA140", "D_800CA2D0"}
WRITE_CALLS = {"strcpy", "strncpy", "strcat", "strncat", "sprintf", "snprintf", "vsprintf", "vsnprintf",
               "memcpy", "memmove", "memset", "get_p_name", "get_g_name"}
STRING_OUTPUT_CALLS = WRITE_CALLS - {"memcpy", "memmove", "memset"}


def _sha(data):
    return hashlib.sha256(data).hexdigest()


def _source_path(repo, relative):
    path = (repo / relative).resolve(strict=True)
    if not path.is_relative_to(repo) or not path.is_file():
        raise ValueError("mutation source is outside the selected repository")
    return path


def _function_body(text, name):
    tokens = list(_lex_c_tokens(text))
    code = _mask_c_literals(_mask_c_comments(text), tokens)
    match = re.search(r"\b" + re.escape(name) + r"\s*\([^;{}]*\)\s*\{", code)
    if match is None:
        raise ValueError("source function not found")
    start = code.find("{", match.start())
    depth = 1
    stop = start + 1
    while depth and stop < len(code):
        depth += (code[stop] == "{") - (code[stop] == "}")
        stop += 1
    if depth:
        raise ValueError("unbalanced source function")
    return code[start:stop], text.count("\n", 0, start) + 1


def _initializer(repo, spec):
    source = _source_path(repo, spec["initializer_source"])
    body, first_line = _function_body(source.read_text(), spec["initializer"])
    assignments = [(m.group(1), int(m.group(2), 16)) for m in
                   re.finditer(r"\b([A-Za-z_]\w*)\s*=\s*(0x[0-9A-Fa-f]+)\s*;", body)]
    if not assignments or len(assignments) % 3:
        raise ValueError("initializer is not a bounded parallel table triad")
    count = len(assignments) // 3
    triads = [tuple(assignments[i + count * column][1] for column in range(3)) for i in range(count)]
    return assignments[:count], triads


def _replacement(catalog, repo, spec):
    matches = [site for site in catalog.get("source_inventory", {}).get("c_literal_sites", [])
               if site["source_site"].get("path") == spec["source"] and
               site["source_site"].get("function") == spec["function"] and
               site["source_site"].get("array_symbol") == spec["array"]]
    if len(matches) != 1:
        raise ValueError("replacement requires one source-inventory literal site")
    site = matches[0]
    raw = bytes.fromhex(site["raw_hex"])
    capacity = site["source_site"].get("array_length")
    if capacity != len(raw) + 1 or spec["length"] > capacity:
        raise ValueError("replacement literal capacity does not establish requested bytes")
    text = _source_path(repo, spec["source"]).read_text()
    actual = []
    for token in _lex_c_tokens(text):
        if token["kind"] != "string" or text.count("\n", 0, token["start"]) + 1 != site["source_site"]["line"]:
            continue
        prefix = text[max(0, token["start"] - 120):token["start"]]
        if re.search(r"\b" + re.escape(spec["array"]) + r"\s*\[\s*" + str(capacity) + r"\s*\]\s*=\s*$", prefix):
            actual.append(_string_bytes(token["literal"]))
    if actual != [raw]:
        raise ValueError("replacement source literal does not agree with source-inventory bytes")
    return {"c_literal_site_id": site["id"], "source_path": spec["source"],
            "source_line": site["source_site"]["line"], "array_symbol": spec["array"],
            "array_capacity": capacity, "raw_hex": (raw + b"\0")[:spec["length"]].hex(),
            "includes_terminal_nul": spec["length"] == capacity}


def _target(record, section, data, base, length, command_value=None):
    text = record["text"]
    raw = bytes.fromhex(text["raw_hex"])
    offset = text["offset"]
    if (record.get("source_sha256") != _sha(data) or text.get("length") != len(raw) or
            data[offset:offset + len(raw) + 1] != raw + b"\0" or not 0 <= offset <= len(data) - length):
        raise ValueError("target text extent or patch bytes disagree with original input")
    group_slot = section["text_table"] - base + record["group"] * 4
    if not 0 <= group_slot <= len(data) - 4:
        raise ValueError("target group slot outside input")
    group_pointer = struct.unpack_from("<I", data, group_slot)[0]
    line_slot = group_pointer - base + record["line"] * 4
    if not 0 <= line_slot <= len(data) - 4:
        raise ValueError("target line slot outside input")
    pointer = struct.unpack_from("<I", data, line_slot)[0]
    if pointer != base + offset or line_slot != text["pointer_offset"] or text["pointer_value"] != pointer:
        raise ValueError("original linked tables do not resolve to target record")
    fits = length <= len(raw) + 1
    result = {"record_id": record["id"], "section": section["id"], "group": record["group"], "line": record["line"],
              "source_path": record["source_path"], "source_sha256": record["source_sha256"],
              "offset": offset, "relative_byte_offset": 0, "length": length,
              "original_raw_hex": data[offset:offset + length].hex(),
              "write_fits_exported_extent": fits, "exported_text_capacity": len(raw) + 1,
              "pointer_evidence": [{"offset": group_slot, "value": group_pointer}, {"offset": line_slot, "value": pointer}],
              "translation_requires_patch_aware_relocation": True}
    if command_value is not None:
        command = record["command"]
        command_group_slot = section["command_table"] - base + record["group"] * 4
        if not 0 <= command_group_slot <= len(data) - 4:
            raise ValueError("command group slot outside input")
        command_pointer = struct.unpack_from("<I", data, command_group_slot)[0]
        command_offset = command_pointer - base + record["line"]
        if (not 0 <= command_offset < len(data) or command_offset != command["offset"] or
                data[command_offset] != command["value"]):
            raise ValueError("command pointer or byte differs from original input")
        result["command_patch"] = {"offset": command_offset, "original_value": command["value"], "replacement_value": command_value,
                                   "pointer_evidence": {"offset": command_group_slot, "value": command_pointer}}
    return result


def _conditional_view(record, replacement, conditions, command_value=None):
    """Derive one bounded runtime C-string view without modifying its source."""
    original = bytes.fromhex(record["text"]["raw_hex"]) + b"\0"
    patch = bytes.fromhex(replacement["raw_hex"])
    if len(patch) > len(original):
        return {"status": "target_capacity_unproved"}
    storage = patch + original[len(patch):]
    terminator = storage.find(b"\0")
    if terminator < 0:
        return {"status": "original_terminator_overwritten", "storage_raw_hex": storage.hex()}
    raw = storage[:terminator]
    command = record["command"]["value"] if command_value is None else command_value
    return {"status": "conditional_derived_view", "source_record_id": record["id"],
            "conditions": list(conditions), "command_value": command,
            "source_storage_unchanged": True, "storage_raw_hex": storage.hex(),
            "ignored_storage_after_nul_hex": storage[terminator + 1:].hex(),
            "text": {"raw_hex": raw.hex(), "length": len(raw), "terminator_hex": "00",
                     **tokenize_text(raw, command)}}


def _selector_route(repo, inputs, spec):
    """Validate a reviewed conditional route against immutable source/code pins."""
    contract = spec["selector_contract"]
    if list(spec["indices"]) != list(contract["indices"]):
        raise ValueError("selector roots differ from the reviewed conditional route")
    for path, digest in contract["source_pins"].items():
        if _sha(_source_path(repo, path).read_bytes()) != digest:
            raise ValueError("selector route source differs from pinned proof")
    binaries = {}
    for path, digest in contract["binary_pins"].items():
        data, error = _read_original(inputs / path, inputs)
        if error or data is None or _sha(data) != digest:
            raise ValueError("selector original code input differs from pinned proof")
        binaries[path] = data
    functions = []
    instruction = re.compile(r"/\*\s*[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})\s+([0-9A-Fa-f]{8})\s*\*/\s*([^\s]+)")
    for path, digest in contract["assembly_pins"].items():
        assembly = _source_path(repo, path).read_bytes()
        if _sha(assembly) != digest:
            raise ValueError("selector assembly differs from pinned proof")
        source_path = contract["dispatch_source"] if "/ovl/" in path else "SLPM_86.053"
        unit = contract["units"][source_path]
        data = binaries[source_path]
        words = []
        for line, text in enumerate(assembly.decode("utf-8").splitlines(), 1):
            match = instruction.search(text)
            if match is None or match[3].startswith("."):
                continue
            pc, raw = int(match[1], 16), bytes.fromhex(match[2])
            offset = pc - unit["load_address"] + unit["file_bias"]
            if offset < unit["file_bias"] or data[offset:offset + 4] != raw:
                raise ValueError("selector instruction disagrees with original code")
            if words and pc != words[-1]["pc"] + 4:
                raise ValueError("selector function instruction extent is not contiguous")
            words.append({"pc": pc, "offset": offset, "raw_hex": raw.hex(), "assembly_line": line})
        if not words:
            raise ValueError("selector assembly has no verified instructions")
        functions.append({"assembly_path": path, "assembly_sha256": digest,
                          "source_path": source_path, "source_sha256": contract["binary_pins"][source_path],
                          "instructions": words})
    dispatch = contract["data_assembly"]
    assembly = _source_path(repo, dispatch["path"]).read_bytes()
    if _sha(assembly) != dispatch["sha256"]:
        raise ValueError("selector dispatch assembly differs from pinned proof")
    label = re.escape(dispatch["label"])
    match = re.search(r"(?m)^dlabel " + label + r"\s*$([\s\S]*?)^enddlabel " + label + r"\s*$", assembly.decode("utf-8"))
    if match is None:
        raise ValueError("selector dispatch label extent missing")
    words = []
    data = binaries[contract["dispatch_source"]]
    unit = contract["units"][contract["dispatch_source"]]
    for item in instruction.finditer(match[1]):
        if item[3] != ".word":
            raise ValueError("selector dispatch contains an unsupported initializer")
        address, raw = int(item[1], 16), bytes.fromhex(item[2])
        offset = address - unit["load_address"] + unit["file_bias"]
        if (address != dispatch["address"] + len(words) * 4 or offset < unit["file_bias"] or
                data[offset:offset + 4] != raw):
            raise ValueError("selector dispatch word disagrees with original input")
        words.append(int.from_bytes(raw, "little"))
    raw_words = b"".join(word.to_bytes(4, "little") for word in words)
    if len(words) != dispatch["word_count"] or _sha(raw_words) != dispatch["raw_sha256"]:
        raise ValueError("selector dispatch words differ from reviewed route")
    for slot, target in dispatch["required_slots"].items():
        if not 0 <= slot < len(words) or words[slot] != target:
            raise ValueError("selector dispatch semantic slot differs from reviewed route")
    return {"status": "source_proved_conditional_route", "runtime_reachability_proven": False,
            "indices": list(contract["indices"]), "assumptions": list(contract["assumptions"]),
            "root_conditions": dict(contract.get("root_conditions", {})),
            "excluded_roots": dict(contract["excluded_roots"]), "binary_pins": dict(contract["binary_pins"]),
            "source_pins": dict(contract["source_pins"]), "function_instruction_evidence": functions,
            "dispatch": {**dispatch, "words": words, "source_path": contract["dispatch_source"],
                         "source_sha256": contract["binary_pins"][contract["dispatch_source"]]}}


def enrich_script_mutations(catalog, repository=REPOSITORY, files_root=None, *, specs=None, source_pins=None,
                            scan_candidates=True, candidate_files=None):
    """Attach validated patch slots and a separate unresolved source-write ledger."""
    repo = Path(repository).resolve(strict=True)
    inputs = Path(files_root or repo / "disc/files").resolve(strict=True)
    specs = MUTATION_SPECS if specs is None else specs
    pins = SOURCE_PINS if source_pins is None else source_pins
    rules, blocked = [], []
    for spec in specs:
        rule = {"id": spec["id"], "conditions": list(spec["conditions"]), "selection_status": spec["selection"],
                "source": {"path": spec["source"], "function": spec["function"], "write_lines": spec["write_lines"]},
                "evidence": spec.get("evidence", []), "targets": []}
        rules.append(rule)
        try:
            for path in spec["pins"]:
                if path not in pins or _sha(_source_path(repo, path).read_bytes()) != pins[path]:
                    raise ValueError("source differs from pinned mutation proof")
            rule["source_pins"] = {path: pins[path] for path in spec["pins"]}
            if "selector_contract" in spec:
                rule["selector_route"] = _selector_route(repo, inputs, spec)
                rule["conditions"].extend(rule["selector_route"]["assumptions"])
            roots, triads = _initializer(repo, spec)
            rule["replacement"] = _replacement(catalog, repo, spec)
            indices = spec.get("indices")
            if indices is None:
                indices = set()
                references = []
                root_map = {symbol: i for i, (symbol, _) in enumerate(roots)}
                for path in sorted((repo / Path(spec["source"]).parent).glob("*.c")):
                    source = path.read_text()
                    code = _mask_c_literals(_mask_c_comments(source), list(_lex_c_tokens(source)))
                    for match in re.finditer(r"\b" + re.escape(spec["runtime_root"]) + r"\s*=\s*([A-Za-z_]\w*)\s*;", code):
                        if match.group(1) in root_map:
                            index = root_map[match.group(1)]
                            indices.add(index)
                            references.append({"path": path.relative_to(repo).as_posix(),
                                               "line": source.count("\n", 0, match.start()) + 1, "table_index": index,
                                               "source_sha256": _sha(path.read_bytes())})
                indices = sorted(indices)
                rule["root_assignment_candidates"] = references
                blocked.append({"rule_id": spec["id"], "status": "runtime_root_selection_not_fully_proved"})
            files = [item for item in catalog["files"] if item["path"] == spec["script"] and "script" in item]
            if len(files) != 1:
                raise ValueError("required linked script is absent or ambiguous")
            parsed = files[0]["script"]
            data, error = _read_original(inputs / spec["script"], inputs)
            if error or data is None or _sha(data) != parsed["source"]["sha256"]:
                raise ValueError("script original input missing, unsafe, or checksum differs")
            layout = parsed["layout"]
            for index in indices:
                expected = triads[index]
                sections = [section for section in layout["sections"]
                            if tuple(section[key] for key in ("text_table", "command_table", "voice_table")) == expected]
                if len(sections) != 1:
                    raise ValueError("initializer root does not identify one exported section")
                section = sections[0]
                records = [record for record in parsed["records"] if record["section"] == section["id"] and
                           record["group"] == spec["group"] and record["line"] == spec["line"] and "text" in record]
                if len(records) != 1:
                    raise ValueError("patch target is not one exported text record")
                if records[0].get("source_path") != spec["script"]:
                    raise ValueError("patch target source path differs from the selected script")
                target = _target(records[0], section, data, layout["load_address"], spec["length"], spec.get("command_value"))
                target["initializer_table_index"] = index
                route_conditions = rule.get("selector_route", {}).get("root_conditions", {}).get(index, [])
                if route_conditions:
                    target["selector_conditions"] = list(route_conditions)
                if spec["selection"] != "source_assignment_union_requires_runtime_selector_proof":
                    target["conditional_runtime_view"] = _conditional_view(records[0], rule["replacement"],
                                                                             rule["conditions"] + list(route_conditions), spec.get("command_value"))
                    if target["conditional_runtime_view"]["status"] == "original_terminator_overwritten":
                        blocked.append({"rule_id": spec["id"], "record_id": target["record_id"],
                                        "status": "runtime_terminator_outside_exported_extent"})
                rule["targets"].append(target)
                if not target["write_fits_exported_extent"]:
                    blocked.append({"rule_id": spec["id"], "record_id": target["record_id"], "status": "target_allocation_capacity_unproved"})
            rule["status"] = "source_pinned_slots_validated"
        except (OSError, ValueError, KeyError, TypeError, IndexError) as exc:
            rule["status"] = "unresolved_mutation"
            blocked.append({"rule_id": spec["id"], "status": "mutation_proof_failed", "reason": str(exc)})
    candidate_report = inventory_mutation_candidates(repo, catalog, candidate_files) if scan_candidates else {
        "status": "not_requested", "candidates": [], "blocked": []}
    known_sites = {(spec["source"], line) for spec in specs for line in spec["write_lines"]}
    for candidate in candidate_report["candidates"]:
        candidate["known_patch_rule_ids"] = [spec["id"] for spec in specs
                                             if candidate["source_path"] == spec["source"] and candidate["line"] in spec["write_lines"]]
    unresolved = [candidate["id"] for candidate in candidate_report["candidates"]
                  if (candidate["source_path"], candidate["line"]) not in known_sites and
                  candidate["kind"] not in ("script_root_binding", "source_literal_initializer")]
    opaque_count = len(candidate_report.get("opaque_value_dependencies", []))
    address_count = len(candidate_report.get("unmodeled_address_dependencies", []))
    catalog["runtime_text_mutations"] = {"schema": "tokimemo.text.runtime-mutations.v1", "rules": rules,
                                         "source_write_inventory": candidate_report, "blocked": blocked + candidate_report["blocked"],
                                         "unresolved_candidate_ids": unresolved,
                                         "completeness": {"complete": False, "unresolved_writes_block_completion": True,
                                                          "opaque_dependencies_block_completion": True,
                                                          "unmodeled_address_dependencies_block_completion": True,
                                                          "unresolved_opaque_dependencies": opaque_count,
                                                          "unresolved_address_dependencies": address_count},
                                         "coverage": {"rules": len(rules), "target_views": sum(len(rule["targets"]) for rule in rules),
                                                      "unresolved_candidate_writes": len(unresolved),
                                                      "opaque_value_dependencies": opaque_count,
                                                      "unmodeled_address_dependencies": address_count},
                                         "limits": ["no_original_bytes_modified", "conditions_not_executed",
                                                    "generic_write_candidates_require_alias_and_state_resolution",
                                                    "assembly_only_mutations_not_enumerated"]}
    return catalog


def inventory_mutation_candidates(repository, catalog, files=None):
    """Inventory C string-writing calls and writes through possible text aliases.

    Numeric address matches and lexical aliases select candidates only; they
    never prove that an assignment modifies a loaded script or language string.
    Structural value roles exclude array selectors and member-name tokens from
    alias seeds. Arithmetic, element types, and helper returns remain possible
    dependencies, with opaque return expressions recorded separately.
    """
    from pycparser import c_ast, c_parser
    from .text_ast import _GENERATOR, _ids, _redacted_expr
    from .text_renderers import known_calls, read_argument_specs
    def value_symbols(node):
        """Possible pointer/storage bases; selectors are not returned values."""
        if node is None:
            return set()
        if isinstance(node, c_ast.ID):
            return {node.name}
        if isinstance(node, c_ast.ArrayRef):
            return value_symbols(node.name)
        if isinstance(node, c_ast.StructRef):
            return value_symbols(node.name)
        if isinstance(node, c_ast.Cast):
            return value_symbols(node.expr)
        if isinstance(node, c_ast.UnaryOp):
            return set() if node.op in ("sizeof", "_Alignof") else value_symbols(node.expr)
        if isinstance(node, c_ast.TernaryOp):
            return value_symbols(node.iftrue) | value_symbols(node.iffalse)
        if isinstance(node, c_ast.ExprList):
            return value_symbols(node.exprs[-1]) if node.exprs else set()
        if isinstance(node, c_ast.BinaryOp):
            return value_symbols(node.left) | value_symbols(node.right)
        # Function return values have no proved relationship to their arguments.
        return set()

    def alias_dependencies(node):
        """Keep helper-return alias possibilities without seeding its scalar inputs."""
        dependencies = value_symbols(node)
        class Calls(c_ast.NodeVisitor):
            def visit_FuncCall(self, call):
                for argument in call.args.exprs if call.args else []:
                    dependencies.update(value_symbols(argument))
                self.generic_visit(call)
        Calls().visit(node)
        return dependencies

    def write_dependencies(node):
        """Retain opaque destination bases, without following selector operands."""
        if isinstance(node, (c_ast.ArrayRef, c_ast.StructRef)):
            return write_dependencies(node.name)
        if isinstance(node, (c_ast.Cast, c_ast.UnaryOp)):
            return write_dependencies(node.expr)
        return alias_dependencies(node) if isinstance(node, c_ast.FuncCall) else value_symbols(node)

    def expression_variables(node):
        """Variables used anywhere in an expression, excluding member/callee names."""
        if node is None:
            return set()
        if isinstance(node, c_ast.ID):
            return {node.name}
        if isinstance(node, c_ast.StructRef):
            return expression_variables(node.name)
        if isinstance(node, c_ast.FuncCall):
            return expression_variables(node.args)
        if isinstance(node, c_ast.UnaryOp) and node.op in ("sizeof", "_Alignof"):
            return set()
        return set().union(*(expression_variables(child) for _, child in node.children()))
    repo = Path(repository).resolve(strict=True)
    paths = sorted((repo / "src/main").rglob("*.c")) + sorted((repo / "src/ovl").rglob("*.c")) if files is None else [repo / path for path in files]
    root_values = {section[key] for file in catalog.get("files", []) for section in file.get("script", {}).get("layout", {}).get("sections", [])
                   for key in ("text_table", "command_table") if key in section}
    roots_by_unit = {}
    source_inputs = []
    source_read_blocked = []
    for path in paths:
        try:
            if not path.resolve(strict=True).is_relative_to(repo):
                raise ValueError("source escapes repository")
            source = path.read_text(encoding="utf-8")
            if "\ufffd" in source:
                raise UnicodeError("replacement character in source")
        except (OSError, ValueError, UnicodeError) as exc:
            source_read_blocked.append({"source_path": path.as_posix(), "status": "mutation_source_unreadable",
                                        "error_type": type(exc).__name__})
            continue
        code = _mask_c_literals(_mask_c_comments(source), list(_lex_c_tokens(source)))
        rel_parts = path.relative_to(repo).parts
        unit = "main" if rel_parts[:2] == ("src", "main") else rel_parts[2]
        roots = roots_by_unit.setdefault(unit, set(ROOT_SEEDS))
        source_inputs.append((path, source, code, unit))
        for match in re.finditer(r"\b([A-Za-z_]\w*)\s*=\s*(0x[0-9A-Fa-f]+)\s*;", code):
            if int(match.group(2), 16) in root_values:
                roots.add(match.group(1))
    while True:
        before = sum(len(roots) for roots in roots_by_unit.values())
        for _, _, code, unit in source_inputs:
            roots = roots_by_unit[unit]
            for match in re.finditer(r"\b([A-Za-z_]\w*)\s*=\s*([A-Za-z_]\w*)\s*;", code):
                if match.group(2) in roots:
                    roots.add(match.group(1))
        if sum(len(roots) for roots in roots_by_unit.values()) == before:
            break
    candidates, blocked, parsed_files, skipped_files, opaque_values = [], list(source_read_blocked), [], [], []
    address_dependencies = []
    generator = _GENERATOR()
    for path, source, code, unit in source_inputs:
        roots = roots_by_unit[unit]
        display_calls = {name for name in known_calls(unit)
                         if any(spec["role"] == "display" for spec in read_argument_specs(name, unit))}
        rel = path.relative_to(repo).as_posix()
        if files is None and not any(re.search(r"\b" + re.escape(name) + r"\b", code) for name in WRITE_CALLS | roots | NAME_BUFFERS | display_calls):
            skipped_files.append(rel)
            continue
        try:
            preprocessed = subprocess.run(["gcc", "-E", "-std=gnu89", "-DM2CTX", "-Iinclude", rel], cwd=repo,
                                          check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True).stdout
            tree = c_parser.CParser().parse(preprocessed, filename=rel)
        except Exception as exc:
            blocked.append({"source_path": rel, "status": "mutation_source_ast_unavailable", "error_type": type(exc).__name__})
            continue
        parsed_files.append(rel)
        current = {"function": None, "locals": set(), "conditions": []}

        def coordinate(node):
            if node.coord is None:
                return None
            location = Path(node.coord.file)
            if location.is_absolute():
                try:
                    location = location.relative_to(repo)
                except ValueError:
                    return None
            return node.coord.line if location.as_posix() == rel else None

        def emit(node, kind, target, value, call=None):
            line = coordinate(node)
            if line is None:
                return
            candidates.append({"id": f"mutation-candidate:{rel}:{line}:{len(candidates)}", "source_path": rel,
                               "source_sha256": _sha(source.encode()), "line": line, "function": current["function"],
                               "kind": kind, "callee": call, "destination_expression": _redacted_expr(target, generator),
                               "operator": getattr(node, "op", None),
                               "value_expression": _redacted_expr(value, generator),
                               "destination_symbols": sorted(_ids(target)),
                               "syntactic_guard_context": list(current["conditions"]),
                               "status": "candidate_not_proven_runtime_target"})

        def opaque_value(node, expression, role, destination=None):
            class Calls(c_ast.NodeVisitor):
                found = False
                def visit_FuncCall(self, call):
                    self.found = True
            visitor = Calls()
            visitor.visit(expression)
            if visitor.found and coordinate(node) is not None:
                opaque_values.append({"source_path": rel, "source_sha256": _sha(source.encode()),
                                      "line": coordinate(node), "function": current["function"],
                                      "role": role, "expression": _redacted_expr(expression, generator),
                                      "destination_expression": _redacted_expr(destination, generator) if destination is not None else None,
                                      "status": "opaque_return_value_not_proven_alias"})

        def address_dependency(node, destination):
            dependencies = ((expression_variables(destination) - write_dependencies(destination)) &
                            (roots | NAME_BUFFERS | current["locals"]))
            line = coordinate(node)
            if dependencies and line is not None:
                address_dependencies.append({"source_path": rel, "source_sha256": _sha(source.encode()),
                                             "line": line, "function": current["function"],
                                             "destination_expression": _redacted_expr(destination, generator),
                                             "text_derived_selector_symbols": sorted(dependencies),
                                             "syntactic_guard_context": list(current["conditions"]),
                                             "status": "unmodeled_address_dependency_not_proven_text_write"})

        class Visitor(c_ast.NodeVisitor):
            def visit_FuncDef(self, node):
                old = dict(current)
                current.update(function=node.decl.name, locals=set(), conditions=[])
                class Reads(c_ast.NodeVisitor):
                    def visit_FuncCall(self, call):
                        name = call.name.name if isinstance(call.name, c_ast.ID) else None
                        args = call.args.exprs if call.args else []
                        for spec in read_argument_specs(name, unit):
                            if spec["role"] == "display" and spec["index"] < len(args):
                                current["locals"].update(value_symbols(args[spec["index"]]))
                                opaque_value(call, args[spec["index"]], "display_argument")
                        if name in STRING_OUTPUT_CALLS and args:
                            current["locals"].update(value_symbols(args[0]))
                            opaque_value(call, args[0], "string_output_destination")
                        self.generic_visit(call)
                Reads().visit(node.body)
                alias_rows = []
                class Aliases(c_ast.NodeVisitor):
                    def visit_Assignment(self, assignment):
                        if isinstance(assignment.lvalue, c_ast.ID):
                            alias_rows.append((assignment.lvalue.name, alias_dependencies(assignment.rvalue)))
                            opaque_value(assignment, assignment.rvalue, "assignment_value", assignment.lvalue)
                        self.generic_visit(assignment)
                    def visit_Decl(self, declaration):
                        if declaration.name and declaration.init is not None:
                            alias_rows.append((declaration.name, alias_dependencies(declaration.init)))
                            opaque_value(declaration, declaration.init, "initializer_value", c_ast.ID(declaration.name))
                        self.generic_visit(declaration)
                Aliases().visit(node.body)
                while True:
                    before = len(current["locals"])
                    for name, identifiers in alias_rows:
                        if identifiers & (roots | NAME_BUFFERS | current["locals"]):
                            current["locals"].add(name)
                    if len(current["locals"]) == before:
                        break
                self.visit(node.body)
                current.update(old)
            def visit_If(self, node):
                self.visit(node.cond)
                condition = _redacted_expr(node.cond, generator)
                current["conditions"].append(condition)
                self.visit(node.iftrue)
                current["conditions"].pop()
                if node.iffalse:
                    current["conditions"].append("!(" + condition + ")")
                    self.visit(node.iffalse)
                    current["conditions"].pop()
            def visit_For(self, node):
                if node.init:
                    self.visit(node.init)
                if node.cond:
                    self.visit(node.cond)
                current["conditions"].append("for_condition:" + (_redacted_expr(node.cond, generator) if node.cond else "true"))
                self.visit(node.stmt)
                if node.next:
                    self.visit(node.next)
                current["conditions"].pop()
            def visit_While(self, node):
                self.visit(node.cond)
                current["conditions"].append("while_condition:" + _redacted_expr(node.cond, generator))
                self.visit(node.stmt)
                current["conditions"].pop()
            def visit_DoWhile(self, node):
                current["conditions"].append("do_while_post_test:" + _redacted_expr(node.cond, generator))
                self.visit(node.stmt)
                current["conditions"].pop()
                self.visit(node.cond)
            def visit_Switch(self, node):
                self.visit(node.cond)
                current["conditions"].append("switch:" + _redacted_expr(node.cond, generator))
                self.visit(node.stmt)
                current["conditions"].pop()
            def visit_Case(self, node):
                current["conditions"].append("case:" + _redacted_expr(node.expr, generator))
                for statement in node.stmts or []:
                    self.visit(statement)
                current["conditions"].pop()
            def visit_Default(self, node):
                current["conditions"].append("default_case:fallthrough_possible")
                for statement in node.stmts or []:
                    self.visit(statement)
                current["conditions"].pop()
            def visit_Assignment(self, node):
                identifiers = write_dependencies(node.lvalue)
                opaque_value(node, node.lvalue, "write_destination", node.lvalue)
                address_dependency(node, node.lvalue)
                if identifiers & (roots | NAME_BUFFERS | current["locals"]):
                    kind = "script_root_binding" if isinstance(node.lvalue, c_ast.ID) and node.lvalue.name in roots else "text_or_buffer_write"
                    emit(node, kind, node.lvalue, node.rvalue)
                self.generic_visit(node)
            def visit_UnaryOp(self, node):
                if node.op in ("++", "--", "p++", "p--"):
                    opaque_value(node, node.expr, "write_destination", node.expr)
                    address_dependency(node, node.expr)
                    if write_dependencies(node.expr) & (roots | NAME_BUFFERS | current["locals"]):
                        emit(node, "text_or_buffer_write", node.expr, node.expr)
                self.generic_visit(node)
            def visit_Decl(self, node):
                if current["function"] and node.init is not None and node.name in current["locals"]:
                    kind = "source_literal_initializer" if isinstance(node.init, c_ast.Constant) and node.init.type == "string" else "text_or_buffer_write"
                    emit(node, kind, c_ast.ID(node.name), node.init)
                self.generic_visit(node)
            def visit_FuncCall(self, node):
                name = node.name.name if isinstance(node.name, c_ast.ID) else None
                args = node.args.exprs if node.args else []
                if name in WRITE_CALLS and args:
                    emit(node, "string_or_byte_copy", args[0], node.args, name)
                    address_dependency(node, args[0])
                self.generic_visit(node)
        Visitor().visit(tree)
    return {"schema": "tokimemo.text.mutation-candidates.v1", "source_files_enumerated": len(source_inputs),
            "parsed_files": parsed_files, "files_skipped_by_candidate_prefilter": skipped_files,
            "candidates": candidates, "blocked": blocked,
            "opaque_value_dependencies": opaque_values,
            "unmodeled_address_dependencies": address_dependencies,
            "root_seed_count": sum(len(roots) for roots in roots_by_unit.values()), "limits": ["candidate_alias_closure_not_pointer_proof",
                                                       "structural_value_dependencies_exclude_selectors_not_full_c_type_or_control_flow",
                                                       "arithmetic_and_element_scalar_pointer_roles_remain_conservative",
                                                       "opaque_helper_results_are_unresolved_possible_aliases_not_proven_passthrough",
                                                       "text_derived_selector_dependencies_remain_unresolved_address_diagnostics",
                                                       "all_string_and_byte_copy_calls_are_candidates_not_necessarily_text",
                                                       "opaque_helpers_and_unseeded_byte_writes_require_further_analysis",
                                                       "syntactic_guards_are_not_resolved_path_predicates",
                                                       "case_fallthrough_and_label_goto_edges_not_resolved",
                                                       "control_conditions_recorded_without_execution"]}
