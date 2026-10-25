"""Source-backed fixed-width DATE lookup tables (T-28001).

This module preserves all source slots, including empty slots and padding.
It also preserves bytes following the declared slots within each assembly
label extent without assigning those bytes a purpose. Decoded views come from
the shared fixed-string parser; original bytes remain the authority.
"""
from __future__ import annotations

import hashlib

from .text import TextFormatError
from .text_tables import parse_dictionary, verify_table_source_proof


def verify_date_source_proof(data: bytes, repository, files_root, profile=None) -> dict:
    """Replay current DATE table types, index formulas and original code/data."""
    return verify_table_source_proof(data, DATE_TABLE_PROFILE if profile is None else profile,
                                     repository, files_root)


DATE_TABLE_PROFILE = {'id': 'date-place-and-gift-names',
 'path': 'CDROM/EXEDIR/DATE.EXN',
 'sha1': 'b5fb83738364a317a590735578f5878077fcda3d',
 'sha256': '35035e4e3ebdd9b3aebf8affd049554ad6745f76dc27cd697a7b646ce3438ebc',
 'load_address': 2148737024,
 'header_size': 0,
 'tables': [{'id': 'place_names',
             'address': 2148907484,
             'count': 22,
             'kind': 'fixed_strings',
             'stride': 33,
             'valid_count': 21,
             'context': 'place_index',
             'assembly_size': 728},
            {'id': 'gift_names',
             'address': 2148913416,
             'count': 99,
             'kind': 'fixed_strings',
             'stride': 33,
             'dimensions': [11, 3, 3],
             'context': ['girl_index', 'year_index', 'answer_index'],
             'assembly_size': 3272}],
 'evidence': ['src/ovl/date/place_scenes.c::date_meeting_place_check',
              'src/ovl/date/girl_birthday.c::date_gift_start',
              'src/ovl/date/girl_birthday.c::date_gift_name_set',
              'include/ovl/date.h:DatePlaceNameTable'],
 'source_pins': {'asm/ovl/DATE/data/date/girl_birthday.data.s': 'cdba1b804d09b63030b3c8e228941e981a58c2f49600d07c734a1a1d6f2b789c',
                 'asm/ovl/DATE/data/date/place_scenes.data.s': 'd41534eb229839b8a608a41f351207cf3f5f41b8ebaeee94ffd899ed490c851e',
                 'asm/ovl/DATE/matchings/date/girl_birthday/date_gift_name_set.s': '5b13f8956512de30ea415605167051c6bdc520e82a289ae01467cc63a6c97984',
                 'asm/ovl/DATE/matchings/date/girl_birthday/date_gift_start.s': '54505da75f08083b979ae125ad8d95b724ff3d1269029c1c44a59fe4c9c91b9e',
                 'asm/ovl/DATE/matchings/date/place_scenes/date_meeting_place_check.s': 'ff13b01b48a32144a4a820f0982221032247770b7bee14ea43bf64396df1b6d4',
                 'config/symbols_DATE.txt': '4a10c2e99c724884e90df6e11589cbe1059eba80ed133ee483e6f3b2d5914fe7',
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
                 'include/game/model_records.h': 'a8e261ca83f07087bb6b7a2e08e1a709d9cdbcb99ebede573c6a3fa84e92c1c0',
                 'include/game/param.h': '627d01af9cc16b4e3a2819610acf9ee1fd174af5ab768c803facf1a9ec86ba55',
                 'include/game/schedule_records.h': 'c895d5dc8902d05039261caa24e3347d02ba5f4b3e1708a4ec855ae6a79311cd',
                 'include/game/system_data.h': '5754c2cbc7969b308dafd57593648ab1f96d7191e80813165b85f77325f7c48c',
                 'include/game/word_views.h': '8a21797a38272607105b2ceef422c337e928f0b0d066aff5c81dfc6ec8bb0498',
                 'include/game_enums.h': 'cf35db6941db5bdae992b0228040a77336dce7a2f951d2427b7e7a71cd204987',
                 'include/include_asm.h': 'f0b8655093a7dac35e22ae67a63743121f275cd7427e0f61f5e642303acc8371',
                 'include/libapi.h': 'e4c93af4e724109d86a3532e61cdd415c93bdca6d439149a393ebbf5a1a9065e',
                 'include/libgpu.h': 'f0197b81d8f9b4889c0abd2dd0070c536f7cdf5da09b74c3d5fbc10090fe5f27',
                 'include/libgs.h': '466b559a2e91a91b60448325222f344d376c13d76227e6223379c3678615d10b',
                 'include/libgte.h': '368db521cf1db72d862d6e9c1d23acdf745ec16dc844c95345993706fe6c1a84',
                 'include/main/actor.h': '4377eb95590cb89931e8a0bfb8b80440bfb312d8bbd1c67e6aceb34519545dba',
                 'include/main/audio.h': '16877729ae9cc510dd3b7c609c71bb1dd97b5dda46e17f8141f318bf6e952c9d',
                 'include/main/bg.h': 'bd778800b70dba2bb6b2811b79086fd3c391b2448dc2ecca808698c5100237bd',
                 'include/main/cd.h': '2175ff58a8673adbeb77cb78372f0517fa5913c77e363c277d0ecb766ced2b58',
                 'include/main/date.h': '10742c4761e1cf8c6116eb034701322ab5b05ab1f7632830e53a1c74a9140316',
                 'include/main/day_event.h': '983a52118fe5973fa6b42b356f1033a4cb3df32748c07c9f0938ca4866ac355b',
                 'include/main/draw.h': 'b57934cf272e8d85f7b5c43369307702181b026e202d4a04ac56d9904bf93b99',
                 'include/main/event_log.h': '3dd48e44393b7d7a0671aad1a8eaf17b60e4e506befb8ba8a147f345fdcc8fa0',
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
                 'include/main/message.h': '71ce494eca28a75312cd49c2ba41fdfb8e1f9df7732063f374c6ecfad1fb9c06',
                 'include/main/name_code.h': 'c10233524227a0cb62a681c3cc3724a9c80068c19c9a46c1cfd778835168394b',
                 'include/main/pad.h': '66db19784afce6a71b067e93b7a50f56be2431bb5ad31ebba4fc24ceb7dcb19c',
                 'include/main/param.h': '9f58341a7b7905bfa5ac090cac1035647052704fedaf2b70422df4f1ffb2185e',
                 'include/main/portrait.h': 'a47fd133b66b7e6ac079e890c456ca5e0546cc0fff1eff244146d1a5b9731486',
                 'include/main/prim.h': 'a5820d7fa0051f4fe561274f5f7bc28f1df6cc87bfd76786a9c69f8a8a3ca732',
                 'include/main/scene.h': 'faf86435b5c4ae9aa47d2a59c2e7c412f02e8a93cbdae2b3a04f1a507bf80049',
                 'include/main/sprite.h': '2b82233ad846ad0615e3dc667573984362a5518810b5b54ec973465f98994ffa',
                 'include/main/state.h': 'aafe390c388f5b2fd36e8a2f3b491cd516364a01c03f258f940cac77b319fd01',
                 'include/main/talk.h': 'ea765428db83f6fda60eb89c7e753180ddfe2893cc9de0391b66b32dc2dadc22',
                 'include/main/text.h': '3f79601270d9515b5714dc928f59196c1c84a7223f86321e4f49f23cc0ee5eb7',
                 'include/main/tpage.h': '277579106126d3d1e8b722dec269e7a7f9c8d47956ceac121253a5d54d74d4d0',
                 'include/main/view.h': '910777739861b865eea1f03d5a232ba14717dc07904aede3770a48e89d8d3198',
                 'include/main/vram.h': 'cbed858bdfd1e38a6711c3a798bdc773be6dc849863a597d9ffa062fd669f2f3',
                 'include/ovl/date.h': 'f803d8d3a6fce96573a5a4d8b4e4edea8fb8cb80fabe57c0eded344403371080',
                 'src/ovl/date/girl_birthday.c': '04caabe18307cdcb14cbb0981a2d1e8aadb795e0eb732d69fae5e141a5d49ada',
                 'src/ovl/date/place_scenes.c': '80813d58b7f2662ca3c307a308469dd0f99aeb9f0ce152d2af6a201caebc93d8'},
 'layout_notes': {'place_names': '22 fixed slots are declared; place IDs 0..20 are valid and slot 21 is '
                                 'retained as reserved data.',
                  'gift_names': 'Address expression is current_girl*0x129 + year_index*0x63 + '
                                'answer_index*0x21; dimensions are 11x3x3.',
                  'year_index': 'date_school_year; date_gift_start sets year-95 from April, year-96 before '
                                'April.',
                  'original_limits': {'slot_stride': 33, 'place_count': 21, 'gift_count': 99}},
 'data_proofs': [{'label': 'date_place_names',
                  'address': 2148907484,
                  'length': 728,
                  'assembly_path': 'asm/ovl/DATE/data/date/place_scenes.data.s'},
                 {'label': 'date_gift_names',
                  'address': 2148913416,
                  'length': 3272,
                  'assembly_path': 'asm/ovl/DATE/data/date/girl_birthday.data.s'}],
 'source_checks': [{'path': 'include/ovl/date.h', 'contains': 'u8 names[22][33];'},
                   {'path': 'src/ovl/date/girl_birthday.c', 'contains': 'u8 text_bytes[0xCC3];'},
                   {'path': 'src/ovl/date/place_scenes.c', 'contains': 'place_names = date_place_names;'},
                   {'path': 'src/ovl/date/place_scenes.c',
                    'contains': 'place_names.names[game_state.appointment_place_1]'},
                   {'path': 'src/ovl/date/girl_birthday.c',
                    'contains': 'if (game->month >= 4) { date_school_year = game->year - 0x5F; } else { '
                                'date_school_year = game->year - 0x60; }'},
                   {'path': 'src/ovl/date/girl_birthday.c',
                    'contains': 'game_state.current_girl * 0x129 + date_school_year * 0x63 + talk_choice * '
                                '0x21'}],
 'code_proofs': [{'assembly_path': 'asm/ovl/DATE/matchings/date/place_scenes/date_meeting_place_check.s',
                  'source_path': 'CDROM/EXEDIR/DATE.EXN',
                  'source_sha256': '35035e4e3ebdd9b3aebf8affd049554ad6745f76dc27cd697a7b646ce3438ebc',
                  'load_address': 2148737024,
                  'header_size': 0},
                 {'assembly_path': 'asm/ovl/DATE/matchings/date/girl_birthday/date_gift_start.s',
                  'source_path': 'CDROM/EXEDIR/DATE.EXN',
                  'source_sha256': '35035e4e3ebdd9b3aebf8affd049554ad6745f76dc27cd697a7b646ce3438ebc',
                  'load_address': 2148737024,
                  'header_size': 0},
                 {'assembly_path': 'asm/ovl/DATE/matchings/date/girl_birthday/date_gift_name_set.s',
                  'source_path': 'CDROM/EXEDIR/DATE.EXN',
                  'source_sha256': '35035e4e3ebdd9b3aebf8affd049554ad6745f76dc27cd697a7b646ce3438ebc',
                  'load_address': 2148737024,
                  'header_size': 0}]}


def _context_for(table: dict, index: int) -> dict:
    if table["id"] == "place_names":
        valid = index < table["valid_count"]
        return {"place_index": index, "valid_place_id": valid,
                "slot_status": "valid_place" if valid else "reserved"}
    dimensions = table["dimensions"]
    answers = dimensions[2]
    years = dimensions[1]
    girl, remainder = divmod(index, years * answers)
    year, answer = divmod(remainder, answers)
    return {"girl_index": girl, "year_index": year, "answer_index": answer,
            "source_coordinates": [girl, year, answer]}


def parse_date_tables(data: bytes, profile: dict | None = None) -> dict:
    """Parse DATE fixed-width tables and attach source-derived index context."""
    profile = DATE_TABLE_PROFILE if profile is None else profile
    expected_sha256 = profile.get("sha256")
    if expected_sha256 and hashlib.sha256(data).hexdigest() != expected_sha256:
        raise TextFormatError("DATE table source differs from verified layout")
    for table in profile["tables"]:
        if table["id"] == "place_names":
            if not 0 <= table.get("valid_count", -1) <= table["count"]:
                raise TextFormatError("DATE place table lacks a bounded valid-slot count")
        elif table["id"] == "gift_names":
            dimensions = table.get("dimensions", [])
            if (len(dimensions) != 3 or any(not isinstance(value, int) or value <= 0 for value in dimensions) or
                    dimensions[0] * dimensions[1] * dimensions[2] != table["count"]):
                raise TextFormatError("DATE gift table dimensions do not match its slot count")
        else:
            raise TextFormatError("unsupported DATE fixed-table context")
    parsed = parse_dictionary(data, profile)
    tables = {table["id"]: table for table in profile["tables"]}
    for record in parsed["records"]:
        table = tables[record["table"]]
        record["context"] = _context_for(table, record["index"])
    parsed["schema"] = "tokimemo.date-tables.v1"
    extents = []
    for table in profile["tables"]:
        declared_size = table["count"] * table["stride"]
        extent_size = table.get("assembly_size", declared_size)
        offset = table["address"] - profile["load_address"] + profile["header_size"]
        if extent_size < declared_size or offset < 0 or offset + extent_size > len(data):
            raise TextFormatError("DATE table assembly extent outside source")
        extents.append({"id": table["id"], "address": table["address"], "offset": offset,
                        "declared_size": declared_size, "assembly_extent_size": extent_size,
                        "trailing_assembly_bytes_size": extent_size - declared_size,
                        "trailing_assembly_bytes_raw_hex": data[offset + declared_size:offset + extent_size].hex()})
    parsed["table_extents"] = extents
    parsed["coverage"]["empty_slots"] = sum(record["length"] == 0 for record in parsed["records"])
    parsed["coverage"]["empty_slots_by_table"] = {
        table_id: sum(record["table"] == table_id and record["length"] == 0
                      for record in parsed["records"])
        for table_id in tables
    }
    parsed["coverage"]["raw_slots_preserved"] = all(
        len(record.get("slot_raw_hex", "")) == 2 * tables[record["table"]]["stride"]
        for record in parsed["records"])
    return parsed
