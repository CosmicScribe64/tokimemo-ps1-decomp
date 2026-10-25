"""Source-backed fixed-width text storage in selected overlay data sections.

The profiles below come from typed source declarations, named assembly labels,
and copy/index consumers.  This reader preserves every fixed slot byte.  It
only offers a bounded text view when a NUL occurs inside the declared slot;
full-width unterminated slots remain raw and explicitly unresolved as C strings.
"""
from __future__ import annotations

import math

from .text import TextFormatError, sha256
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
        "evidence": ["include/ovl/BUNKAKEN.h:141-144", "src/ovl/BUNKAKEN/80148ED0.c:697-700",
                     "asm/ovl/BUNKAKEN/data/BUNKAKEN_data.data.s:624-704"],
        "source_pins": {
            "include/ovl/BUNKAKEN.h": "455abe59a952b2099bbff684e58211e6fd6c51cc709e53747ac347af877adab4",
            "src/ovl/BUNKAKEN/80148ED0.c": "de43a6aa04d0f88657ed9ff5e85652e884b750864b0858e5f8840a17679a328e",
            "asm/ovl/BUNKAKEN/data/BUNKAKEN_data.data.s": "7d37ad0c77a339a431c333e25246b00c1d69ff21adfcdb8d67671fa0da8859cf"},
    },
    {
        "id": "bunkasai-name-grid", "path": "CDROM/EXEDIR/BUNKASAI.EXN",
        "sha256": "381a936f10c244b21bd274eab74bda0f0388f33570947e473afbf46c31204b42",
        "load_address": 0x80132000,
        "tables": [{"id": "name_grid", "role": "choice_name", "address": 0x801606DC, "count": 9,
                    "stride": 33, "dimensions": [3, 3],
                    "context": ["school_year_index", "choice_index"],
                    "trailing_assembly_bytes": 11}],
        "evidence": ["include/ovl/BUNKASAI.h:21-24", "src/ovl/BUNKASAI/80146FA0.c:481-486",
                     "asm/ovl/BUNKASAI/data/BUNKASAI_data.data.s:365-445"],
        "source_pins": {
            "include/ovl/BUNKASAI.h": "602e171bfda712152dd912a3be38dfc315535683c7210484b0992591c4839378",
            "src/ovl/BUNKASAI/80146FA0.c": "5bb03625b73fca9de0b01b64dec5ad2956637ff3ac2b999da7f07473969d5b6b",
            "asm/ovl/BUNKASAI/data/BUNKASAI_data.data.s": "cef8bd186765df95c6967bad3cb54b4b718bda2ee7daee98930ba2431ffdb202"},
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
        "evidence": ["include/ovl/DATE2.h:255-257", "src/ovl/DATE2/80137D50.c:288-323",
                     "src/ovl/DATE2/80137360.c:151-161",
                     "asm/ovl/DATE2/data/DATE2/80137D50.data.s:335-359",
                     "asm/ovl/DATE2/data/DATE2_data_8013A600.data.s:593-1414"],
        "source_pins": {
            "include/ovl/DATE2.h": "a83c5907e9826e6580ff451b24849c7ab6076942180f77092728700b97cc82af",
            "src/ovl/DATE2/80137D50.c": "5184b05be033f4ae9ba608b68c51433bbf903406581c4c22349f9eeaf17ec718",
            "src/ovl/DATE2/80137360.c": "31353b53f7c439fb31bef46e1d9b5df7ffbab45de359d510273652dd6f5dd7dd",
            "asm/ovl/DATE2/data/DATE2/80137D50.data.s": "deed180c5a8806f4b5cc3acd7afa67e97a0306f9c0200d9bd879acafd53f5dfe",
            "asm/ovl/DATE2/data/DATE2_data_8013A600.data.s": "17a44d6faecaf08945880287f31a3d06ac3f93f41da66da4c74b0d7bfc2dfbb7"},
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
        "evidence": ["include/ovl/GEKO.h:773-777,820-834", "src/ovl/GEKO/801338F0.c:84-88",
                     "src/ovl/GEKO/80142760.c:142-152,364", "src/ovl/GEKO/80134D10.c:372-373,906-908,1331-1336",
                     "asm/ovl/GEKO/data/GEKO_data.data.s:394-458",
                     "asm/ovl/GEKO/data/GEKO_data_80146270.data.s:2905-3065",
                     "asm/ovl/GEKO/data/GEKO/80134D10.data.s:730-1550"],
        "source_pins": {
            "include/ovl/GEKO.h": "4fb9e4c2d8a7680981a78548747dd2fb17d1fe8286a887233dede34dc7533518",
            "src/ovl/GEKO/801338F0.c": "4cc0c5388fa02c2ff871c883cd961870253c7677f05638d7f19e4a56689088d6",
            "src/ovl/GEKO/80142760.c": "52bcb794c462ebee3677782fe0df5803622403a577848b49a1b419dc155e712c",
            "src/ovl/GEKO/80134D10.c": "6d40808acb4ea61ddad7b819344a028c85881e10866e3387093e56d55b8faf01",
            "asm/ovl/GEKO/data/GEKO_data.data.s": "8e6a1b808743449e9a69d3ba42c3e1529e1c4b072246138d3846cbc4525dcac9",
            "asm/ovl/GEKO/data/GEKO_data_80146270.data.s": "78bbb92da182f7da1843e8a69d21046fbb4492e86754d6a2a7b26d9a25e44415",
            "asm/ovl/GEKO/data/GEKO/80134D10.data.s": "c72484534ff46a933ec8036ef82e9501e6b9c08379aee40067ed28f11fdf77e9"},
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
        "evidence": ["include/ovl/KANGEI.h:201-208", "src/ovl/KANGEI/80135F10.c:544-558",
                     "src/ovl/KANGEI/80137B90.c:10-12,210-228,640-645",
                     "asm/ovl/KANGEI/data/KANGEI_data_80139C80.data.s:446-567,1254-41869"],
        "source_pins": {
            "include/ovl/KANGEI.h": "c9a60d7fa93128455b5136ea9965d80095091c5f87d56da52f02a6d7a7e1b6be",
            "src/ovl/KANGEI/80135F10.c": "1ead52f8fc63c33e3d9602a3a8922089bdabbcfc6f9e38ff4211525bd231bcd3",
            "src/ovl/KANGEI/80137B90.c": "853387092cc5bc1a92ec989ed9058cebe40556601fee2205f0acc878dc2d56ee",
            "asm/ovl/KANGEI/data/KANGEI_data_80139C80.data.s": "c379f30c618bd1e4c6c1dea71ecba35ff5caa4aaae0cad91607c4cf27bd72280"},
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
        "evidence": ["include/ovl/SHOUGATU.h:682-686,931-936", "src/ovl/SHOUGATU/80134120.c:79-86",
                     "src/ovl/SHOUGATU/80134890.c:176-185,306-324,362-375",
                     "src/ovl/SHOUGATU/80135B90.c:91-94,376-382",
                     "asm/ovl/SHOUGATU/data/SHOUGATU_data.data.s:551-564,682-711",
                     "asm/ovl/SHOUGATU/data/SHOUGATU/80135B90.data.s:557-1380"],
        "source_pins": {
            "include/ovl/SHOUGATU.h": "86375fbbaf07ec10d8c84a07d10dba8758f543c98633abd9939c3a5348d4825e",
            "src/ovl/SHOUGATU/80134120.c": "5db2bc1ef2b8a5f5147a8194d77cae08c6206a593ee4355643680ba596ea6216",
            "src/ovl/SHOUGATU/80134890.c": "44aca46d20edcae1bcc0792a5c32bcb1a95efb0d535608ce0948471975b9f154",
            "src/ovl/SHOUGATU/80135B90.c": "4a271839d336352011c46b2f3b6b86d863e5f27aa342bc49a80480e818083602",
            "asm/ovl/SHOUGATU/data/SHOUGATU_data.data.s": "48c01e8db891528d26df9c53e5b5d7ffbaa443a5b753cf18a07593dfa00b414a",
            "asm/ovl/SHOUGATU/data/SHOUGATU/80135B90.data.s": "69cc20ab32b7a80b65f9443649664ba3894c042cfe0891111484aff60487d9a4"},
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
        "evidence": ["src/ovl/SHUGAKU/80132000.c:13-14,268-271,363-368",
                     "asm/ovl/SHUGAKU/data/SHUGAKU_data.data.s:382-1202"],
        "source_pins": {
            "src/ovl/SHUGAKU/80132000.c": "ece491d53bc997330ded2b721511d93ee62efc7937a2040819070bcc210b5aca",
            "asm/ovl/SHUGAKU/data/SHUGAKU_data.data.s": "71712bad2939f9fe709295868f7678caacc524e12e6e322b95abeef93198d77b"},
    },
]


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
