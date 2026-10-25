"""Source-backed fixed-width DATE lookup tables (T-28001).

This module preserves all source slots, including empty slots and padding.
It also preserves bytes following the declared slots within each assembly
label extent without assigning those bytes a purpose. Decoded views come from
the shared fixed-string parser; original bytes remain the authority.
"""
from __future__ import annotations

import hashlib

from .text import TextFormatError
from .text_tables import parse_dictionary


DATE_TABLE_PROFILE = {
    "id": "date-place-and-gift-names",
    "path": "CDROM/EXEDIR/DATE.EXN",
    "sha1": "b5fb83738364a317a590735578f5878077fcda3d",
    "sha256": "35035e4e3ebdd9b3aebf8affd049554ad6745f76dc27cd697a7b646ce3438ebc",
    "load_address": 0x80132000,
    "header_size": 0,
    "tables": [
        {"id": "place_names", "address": 0x8015B9DC, "count": 22,
         "kind": "fixed_strings", "stride": 33, "valid_count": 21,
         "context": "place_index", "assembly_size": 728},
        {"id": "gift_names", "address": 0x8015D108, "count": 99,
         "kind": "fixed_strings", "stride": 33, "dimensions": [11, 3, 3],
         "context": ["girl_index", "year_index", "answer_index"], "assembly_size": 3272},
    ],
    "evidence": [
        "src/ovl/DATE/80132000.c:121,4483-4488",
        "src/ovl/DATE/80149310.c:56-60,182-218,274-281",
        "include/ovl/DATE.h:1752-1755",
        "asm/ovl/DATE/data/DATE/80132000.data.s:908-1093",
        "asm/ovl/DATE/data/DATE/80149310.data.s:349-1170",
    ],
    "source_pins": {
        "src/ovl/DATE/80132000.c": "e7b119d8b13fe1f12f34689de912d228bc55bd71c9c8d62498eb6204d097ff0f",
        "src/ovl/DATE/80149310.c": "d15bba6533f174b6be21f14af5d1c804c2e410738095e379d25d4d0bed8d476a",
        "include/ovl/DATE.h": "07d37cd2d6fd34a896b442cbc359205bebb533d820d179deffb061cd77866846",
        "asm/ovl/DATE/data/DATE/80132000.data.s": "689b051eafdb7dd4b8847d15e7b8829da9997c80a0f0d82c5a783819ceeaa349",
        "asm/ovl/DATE/data/DATE/80149310.data.s": "33c4f9eb4dd93a71c7f24003c41f8177777fdfe1093990fc22241220e5605cae",
    },
    "layout_notes": {
        "place_names": "22 fixed slots are declared; place IDs 0..20 are valid and slot 21 is retained as reserved data.",
        "gift_names": "Address expression is current_girl*0x129 + year_index*0x63 + answer_index*0x21; dimensions are 11x3x3.",
        "year_index": "D_8015D090; set from the game year and month by func_80149310.",
        "original_limits": {"slot_stride": 33, "place_count": 21, "gift_count": 99},
    },
}


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
