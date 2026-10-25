"""Source-backed fixed-width text storage in selected overlay data sections.

The profiles below come from typed source declarations, named assembly labels,
and copy/index consumers.  This reader preserves every fixed slot byte.  It
only offers a bounded text view when a NUL occurs inside the declared slot;
full-width unterminated slots remain raw and explicitly unresolved as C strings.
"""
from __future__ import annotations

import math
import hashlib
import re
from pathlib import Path

from .text import TextFormatError, sha256, source_path
from .text_fixed import tokenize_fixed_text


OVERLAY_TABLE_PROFILES = [
    {
        "id": "bunkaken-name-grid", "path": "CDROM/EXEDIR/BUNKAKEN.EXN",
        "sha256": "950de4f402232a1a36242935ad18094b1a4327d7d8fc187a8ecdb7e090313952",
        "load_address": 0x80132000,
        "tables": [{"id": "name_grid", "role": "choice_name", "address": 0x8015633C, "count": 9,
                    "stride": 33, "dimensions": [3, 3],
                    "context": ["school_year_index", "choice_index"],
                    "trailing_assembly_bytes": 11}],
        "evidence": ["include/ovl/culture_festival_tour.h:141-144", "src/ovl/culture_festival_tour/birthday.c:697-700",
                     "asm/ovl/BUNKAKEN/data/BUNKAKEN_data.data.s:624-704"],
        "source_pins": {
            "include/ovl/culture_festival_tour.h": 'e9b17459c69738051ce67cfdc0b7c4b85a5e207397c2cef752cc5ac335a1184c',
            "src/ovl/culture_festival_tour/birthday.c": '0176ffb328e30078824fed736fe5ea64ea144389dd984fb569fa24f39e44bc55',
            "asm/ovl/BUNKAKEN/data/BUNKAKEN_data.data.s": '93268198669fd78c6f40df5159cd9cf1066ba8f28b2e3e04d72909ca4dc5a32d'},
    },
    {
        "id": "bunkasai-name-grid", "path": "CDROM/EXEDIR/BUNKASAI.EXN",
        "sha256": "381a936f10c244b21bd274eab74bda0f0388f33570947e473afbf46c31204b42",
        "load_address": 0x80132000,
        "tables": [{"id": "name_grid", "role": "choice_name", "address": 0x801606DC, "count": 9,
                    "stride": 33, "dimensions": [3, 3],
                    "context": ["school_year_index", "choice_index"],
                    "trailing_assembly_bytes": 11}],
        "evidence": ["include/ovl/culture_festival.h:21-24", "src/ovl/culture_festival/birthday_scene.c:481-486",
                     "asm/ovl/BUNKASAI/data/BUNKASAI_data.data.s:365-445"],
        "source_pins": {
            "include/ovl/culture_festival.h": '61f27dcbe4c215c7d9a9c2b336cb3c696e222b7cb83a5dd06201befd11c83c40',
            "src/ovl/culture_festival/birthday_scene.c": 'c1bc190efdf101e4602db21531cfb1a9192d698a0d4ef23432781044b40c4582',
            "asm/ovl/BUNKASAI/data/BUNKASAI_data.data.s": '1080f2936d11383ae5c2669f849c7764212c3f55038764a2e984e89f6e700fb6'},
    },
    {
        "id": "date2-menu-labels", "path": "CDROM/EXEDIR/DATE2.EXN",
        "sha256": "650795ceb419b8261e049dd9a84d4938417a78e7b1e0b4f3e48976a5000a50c8",
        "load_address": 0x80132000,
        "tables": [{"id": "menu_labels", "role": "menu_label", "address": 0x8013B634, "count": 4,
                    "stride": 21, "dimensions": [4], "context": ["label_index"]},
                   {"id": "gift_names", "role": "item_name", "address": 0x8013A878, "count": 99,
                          "stride": 33, "dimensions": [11, 3, 3],
                          "context": ["girl_index", "year_index", "choice_index"],
                          "trailing_assembly_bytes": 5,
                          "context_basis": "current_girl*0x129 + game_state.year*0x63 + choice*0x21 - 0x24BD"}],
        "evidence": ["include/ovl/amusement_park_date.h:255-257", "src/ovl/amusement_park_date/karaoke_scene_and_return.c:288-323",
                     "src/ovl/amusement_park_date/scene_choices_and_saved_return.c:151-161",
                     "asm/ovl/DATE2/data/amusement_park_date/karaoke_scene_and_return.data.s:335-359",
                     "asm/ovl/DATE2/data/DATE2_data_8013A600.data.s:593-1414"],
        "source_pins": {
            'include/ovl/amusement_park_date.h': '83dfd8ca41b79648d1de1347f79d2d38948f33e6f46ca199777af172b128e19d',
            'src/ovl/amusement_park_date/karaoke_scene_and_return.c': '95511808b1eb8afc1a4ebfe70636d1d41afbe44013b8567bcd305c513ab17bd5',
            'src/ovl/amusement_park_date/scene_choices_and_saved_return.c': '63949bff20a168de619c50001c75a4b2199d2cdf281b534aa514643e2f333b73',
            'asm/ovl/DATE2/data/amusement_park_date/karaoke_scene_and_return.data.s': '1847c421c53ea972e1b2c454cb231feaa3850378165a431402177ac8378c3684',
            "asm/ovl/DATE2/data/DATE2_data_8013A600.data.s": '4a36259f9f6f7b17873143ceaa618fb1684a5791cf1323fc8c7ee187a65a687a'},
    },
    {
        "id": "geko-place-and-encounter-names", "path": "CDROM/EXEDIR/GEKO.EXN",
        "sha256": "f7295203dc38417b8ff3c274833c06554dd072f5a6efcc74674cdaf38f06e5ad",
        "load_address": 0x80132000,
        "tables": [
            {"id": "place_names", "role": "place", "address": 0x80144D8C, "count": 12,
             "stride": 20, "dimensions": [12], "context": ["place_name_index"],
             "trailing_assembly_bytes": 4},
            {"id": "encounter_names", "role": "name", "address": 0x80147988, "count": 19,
             "stride": 33, "dimensions": [19], "context": ["encounter_name_index"],
             "trailing_assembly_bytes": 1},
            {"id": "gift_names", "role": "item_name", "address": 0x801451FC, "count": 99,
             "stride": 33, "dimensions": [11, 3, 3],
             "context": ["girl_index", "year_index", "choice_index"],
             "trailing_assembly_bytes": 1,
             "context_basis": "current_girl*0x129 + source_year_index*0x63 + choice*0x21"},
        ],
        "evidence": ['include/ovl/girl_encounters.h:773-777,820-834', 'src/ovl/girl_encounters/walk_home_followup.c:84-88',
                     'src/ovl/girl_encounters/invite.c:142-152,364', 'src/ovl/girl_encounters/birthday_and_shared_scene_flow.c:372-373,906-908,1331-1336',
                     "asm/ovl/GEKO/data/GEKO_data.data.s:394-458",
                     "asm/ovl/GEKO/data/GEKO_data_80146270.data.s:2905-3065",
                     'asm/ovl/GEKO/data/girl_encounters/birthday_and_shared_scene_flow.data.s:730-1550'],
        "source_pins": {
            'include/ovl/girl_encounters.h': '742b6a2a671e16db7dde65bd15bdfb988a9269da766b983690f4ac669fb8e6f7',
            'src/ovl/girl_encounters/walk_home_followup.c': 'ed78a5e1465c7089065b9b692074fbc5c20c3e7e0ce003915c7efd1586078bf9',
            'src/ovl/girl_encounters/invite.c': 'fdaffecbe35b1cf095f43fd3b89d3483394e6527bba1519352fbc48f181394d6',
            'src/ovl/girl_encounters/birthday_and_shared_scene_flow.c': 'df3b1a88843f3302096cdedf9987e95e23309badebae2808c1e74a57c4165a65',
            "asm/ovl/GEKO/data/GEKO_data.data.s": '5901fee184282440784268a775d72bc319febd9d65a3628be0eebb0d5b097f66',
            "asm/ovl/GEKO/data/GEKO_data_80146270.data.s": '9c4cf944f810f729a0ea36f6b29abfc290a73791733547bdb6d8179831b3edb6',
            'asm/ovl/GEKO/data/girl_encounters/birthday_and_shared_scene_flow.data.s': 'd83bffd6b7b7d453ba21e1a9264e899bdb16a83f3e97a1f8799e73301da6c73c'},
    },
    {
        "id": "kangei-club-names", "path": "CDROM/EXEDIR/KANGEI.EXN",
        "sha256": "63bd7fb18df21bb6f1be64b8bff162b2755b3037a434bca80d2a209fc1d14ca0",
        "load_address": 0x80132000,
        "tables": [
            {"id": "club_names_20", "role": "name", "address": 0x80139FAC, "count": 11,
             "stride": 20, "dimensions": [11], "context": ["club_index"]},
            {"id": "club_names_21", "role": "name", "address": 0x8013A088, "count": 11,
             "stride": 21, "dimensions": [11], "context": ["club_index"],
             "trailing_assembly_bytes": 1},
            {"id": "gift_names", "role": "item_name", "address": 0x8013A570, "count": 99,
             "stride": 33, "dimensions": [11, 3, 3],
             "context": ["girl_index", "year_index", "choice_index"],
             "context_basis": "current_girl*0x129 + year_index*0x63 + choice*0x21",
             "extent_basis": "C DateTxt size 0xCC3; the ASM dlabel continues beyond this bounded object"},
        ],
        "evidence": ['include/ovl/summer_camp.h:201-208', 'src/ovl/summer_camp/scene_pictures_and_effects.c:544-558',
                     'src/ovl/summer_camp/girl_script_and_choice_flow.c:10-12,210-228,640-645',
                     "asm/ovl/KANGEI/data/KANGEI_data_80139C80.data.s:446-567,1254-41869"],
        "source_pins": {
            'include/ovl/summer_camp.h': '240d9e66e213931e35b332112b5cb005887a3c41873a978ce064768b97e5b73a',
            'src/ovl/summer_camp/scene_pictures_and_effects.c': 'f15e662b9393879c9671d1a749704eb19245ae727a281bd9219441c778d782fc',
            'src/ovl/summer_camp/girl_script_and_choice_flow.c': 'f3cf06f7fe118d72577d3431462c3d9340e8602ae4a39804dc17a21c8a439d9b',
            "asm/ovl/KANGEI/data/KANGEI_data_80139C80.data.s": '132a051f74631f8718a9a5834b9bc4f1b8e384cfdd80ac9dfd9bc73f90fc0851'},
    },
    {
        "id": "shougatu-name-choices", "path": "CDROM/EXEDIR/SHOUGATU.EXN",
        "sha256": "84842bc3e8540ef9853d38ca04bb453eddb60a5a9b14786896ac618b3b84bb13",
        "load_address": 0x80132000,
        "tables": [{"id": "choice_names", "role": "choice_name", "address": 0x80143D88, "count": 3,
                    "stride": 9, "dimensions": [3], "context": ["choice_index"],
                    "trailing_assembly_bytes": 13},
                   {"id": "gift_names", "role": "item_name", "address": 0x801440C0, "count": 99,
                          "stride": 33, "dimensions": [11, 3, 3],
                          "context": ["girl_index", "year_index", "choice_index"],
                          "trailing_assembly_bytes": 13,
                          "context_basis": "current_girl*0x129 + game_state.year*0x63 + choice*0x21 - 0x2520"},
                   {"id": "fortune_result_names", "role": "fortune_result_name", "address": 0x80143E5C,
                    "count": 7, "stride": 5, "dimensions": [7],
                    "context": ["fortune_result_index"], "trailing_assembly_bytes": 1},
                   {"id": "fortune_labels", "role": "fortune_label", "address": 0x80143E80,
                    "count": 7, "stride": 5, "dimensions": [7],
                    "context": ["fortune_result_index"], "trailing_assembly_bytes": 13}],
        "evidence": ['include/ovl/new_year_christmas.h:682-686,931-936', 'src/ovl/new_year_christmas/shrine_actor_sequences.c:79-86',
                     'src/ovl/new_year_christmas/omikuji_draw_scene.c:176-185,306-324,362-375',
                     'src/ovl/new_year_christmas/scene_resource_tables.c:91-94,376-382',
                     "asm/ovl/SHOUGATU/data/SHOUGATU_data.data.s:551-564,682-711",
                     'asm/ovl/SHOUGATU/data/new_year_christmas/scene_resource_tables.data.s:557-1380'],
        "source_pins": {
            'include/ovl/new_year_christmas.h': '1ca1ad375392e0f0bafc1270b9686b1b9e846b0674f0a18ccf8fb0b911b30814',
            'src/ovl/new_year_christmas/shrine_actor_sequences.c': 'aab52d5d2fc9e910e695cc413e83205242d4b3e64bfd324c0f1a7e9d6491a69d',
            'src/ovl/new_year_christmas/omikuji_draw_scene.c': '90edf735f53cccffa04c1b56de92615c46674cb9b199159a4f9cf77f5d640fed',
            'src/ovl/new_year_christmas/scene_resource_tables.c': '2205afa7f916e2429275a50c9ed635b6366e1238d9d96dda97a64a6d1fe4621b',
            "asm/ovl/SHOUGATU/data/SHOUGATU_data.data.s": 'b78a107230c27b1ebd9299649c499f4d8122384c61e106fdcc7ce6494301dd10',
            'asm/ovl/SHOUGATU/data/new_year_christmas/scene_resource_tables.data.s': 'fcf36b1c7564456b5b28bf0f245ed87e3086587fd8d5908e105445b8ea89aa8e'},
    },
    {
        "id": "shugaku-gift-names", "path": "CDROM/EXEDIR/SHUGAKU.EXN",
        "sha256": "06f885c6038811fbbf2f4081d81a18b74cfec08c244e12004e4fa91a8e54cc14",
        "load_address": 0x80132000,
        "tables": [{"id": "gift_names", "address": 0x8013B1AC, "count": 99,
                    "role": "item_name",
                    "stride": 33, "dimensions": [11, 3, 3],
                    "context": ["girl_index", "year_index", "choice_index"],
                    "trailing_assembly_bytes": 1,
                    "context_basis": "current_girl*0x129 + year_index*0x63 + choice*0x21"}],
        "evidence": ['src/ovl/school_trip/birthday_detours_and_talk.c:13-14,268-271,363-368',
                     "asm/ovl/SHUGAKU/data/SHUGAKU_data.data.s:382-1202"],
        "source_pins": {
            'src/ovl/school_trip/birthday_detours_and_talk.c': 'c89cae4e6263b5a05e115595b9ef10bd767aa50c56ba7a97529c0cb2fb371515',
            "asm/ovl/SHUGAKU/data/SHUGAKU_data.data.s": '15eb6194d92f9b985c7173e0ef6c23159a50a444550fcc89a370b8c0f5af0525'},
    },
]


def verify_profile_source_pins(repository, profile: dict) -> dict:
    """Fail closed unless every profile source pin resolves within this repository."""
    repo = Path(repository).resolve(strict=True)
    if not repo.is_dir() or not isinstance(profile, dict):
        raise TextFormatError("source-pin verification requires a repository and profile")
    pins = profile.get("source_pins")
    if not isinstance(pins, dict) or not pins:
        raise TextFormatError("source-pin verification requires nonempty pinned inputs")
    verified = {}
    for relative, expected in pins.items():
        source_path(relative)
        if not isinstance(expected, str) or not re.fullmatch(r"[0-9a-f]{64}", expected):
            raise TextFormatError("source pin is not a lowercase SHA-256 digest")
        path = (repo / relative).resolve(strict=True)
        if not path.is_relative_to(repo) or not path.is_file():
            raise TextFormatError("source pin escapes the selected repository")
        actual = hashlib.sha256(path.read_bytes()).hexdigest()
        if actual != expected:
            raise TextFormatError("pinned source input differs from the selected repository")
        verified[relative] = actual
    return {"status": "verified_profile_source_pins", "profile_id": profile.get("id"),
            "source_pins_verified": len(verified), "source_pins": verified}


def _validate_profile(profile: dict) -> None:
    if not profile.get("path") or not isinstance(profile.get("load_address"), int):
        raise TextFormatError("overlay table profile lacks its source path or load address")
    for table in profile.get("tables", []):
        count, stride = table.get("count"), table.get("stride")
        dims = table.get("dimensions")
        if (not isinstance(table.get("address"), int) or not isinstance(count, int) or count <= 0 or
                not isinstance(stride, int) or stride <= 0 or not isinstance(dims, list) or
                any(not isinstance(value, int) or value <= 0 for value in dims) or
                math.prod(dims) != count):
            raise TextFormatError("overlay fixed-table bounds or dimensions are invalid")


def parse_overlay_fixed_tables(data: bytes, profile: dict) -> dict:
    """Preserve source-declared fixed slots and decode only bounded NUL strings."""
    _validate_profile(profile)
    digest = sha256(data)
    if profile.get("sha256") and digest != profile["sha256"]:
        raise TextFormatError("overlay table source differs from verified layout")
    records = []
    extents = []
    base = profile["load_address"]
    for table in profile["tables"]:
        start = table["address"] - base
        slot_size = table["count"] * table["stride"]
        extent_size = slot_size + table.get("trailing_assembly_bytes", 0)
        if start < 0 or extent_size < slot_size or start + extent_size > len(data):
            raise TextFormatError("overlay table extent outside original source")
        extents.append({"id": table["id"], "address": table["address"], "offset": start,
                        "slot_bytes": slot_size, "declared_extent_bytes": extent_size,
                        "trailing_assembly_bytes": data[start + slot_size:start + extent_size].hex()})
        for index in range(table["count"]):
            offset = start + index * table["stride"]
            slot = data[offset:offset + table["stride"]]
            nul = slot.find(b"\0")
            record = {"id": f"overlay-table:{profile['path']}:{table['id']}:{index:02d}",
                      "source_path": profile["path"], "source_sha256": digest,
                      "table": table["id"], "index": index, "slot_offset": offset,
                      "offset": offset,
                      "address": table["address"] + index * table["stride"],
                      "stride": table["stride"], "slot_raw_hex": slot.hex(),
                      "terminator_offset": nul if nul >= 0 else None,
                      "bytes_after_terminator_hex": slot[nul + 1:].hex() if nul >= 0 else None,
                      "string_extent_status": "nul_bounded_within_slot" if nul >= 0 else "no_nul_within_slot",
                      "text": None, "role": table.get("role")}
            if nul >= 0:
                raw = slot[:nul]
                record["length"] = len(raw)
                record["content_raw_hex"] = raw.hex()
                # The C consumers copy these fields as strings, but the table
                # declaration alone does not prove the later rendering API.
                view = tokenize_fixed_text(raw, "source_fixed_width_storage")
                if view["source_decoded_text"] is None:
                    record["string_extent_status"] = "nul_bounded_invalid_shift_jis"
                else:
                    record["text"] = view
            else:
                record["length"] = None
            dims = table["dimensions"]
            coords = []
            remainder = index
            for dimension in reversed(dims):
                coords.append(remainder % dimension)
                remainder //= dimension
            coords.reverse()
            names = table.get("context", [])
            if len(names) == len(coords):
                record["context"] = dict(zip(names, coords))
                record["context"]["source_coordinates"] = coords
            records.append(record)
    return {"schema": "tokimemo.overlay-fixed-tables.v1", "profile": profile,
            "source": {"path": profile["path"], "size": len(data), "sha256": digest},
            "table_extents": extents, "records": records,
            "coverage": {"tables": len(extents), "slots": len(records),
                         "nul_bounded_slots": sum(r["terminator_offset"] is not None for r in records),
                         "unterminated_slots": sum(r["terminator_offset"] is None for r in records),
                         "empty_slots": sum(r["terminator_offset"] == 0 for r in records),
                         "all_slot_bytes_preserved": all(len(r["slot_raw_hex"]) == 2 * r["stride"] for r in records)}}
