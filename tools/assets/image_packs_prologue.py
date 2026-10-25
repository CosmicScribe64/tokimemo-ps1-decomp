"""Source-bound raw texture pages from four prologue NBN backgrounds.

The original NAME_ENT overlay reads six 0x4000-byte 8bpp pages followed by
256 BGR555 palette words. This reader maps the 128x120 native cells through the
lower-page V=128 upload and row*120 cell UVs. It does not choose the runtime
color-versus-grayscale palette slot or claim a composed scene.
"""
from copy import deepcopy
import hashlib
import re
import struct
from pathlib import Path

from .images import ImageFormatError, _color, _png

TRACK_SHA256 = "28c3c501b0c67b87ca1e30515da73619497d509ff6adc28ff722aa02897dfb72"
EXE_PATH = "SLPM_86.053"
EXE_SHA256 = "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae"
EXE_HELPER_OFFSET = 0x99A0
EXE_HELPER_SIZE = 0x274
EXE_HELPER_SHA256 = "9624e8e592ed00bfdf227796097a71e21d394d4679ccd6262452e28d4d58891c"
OVERLAY_PATH = "CDROM/EXEDIR/NAME_ENT.EXN"
OVERLAY_SHA256 = "f7384ad3617361282638ffd9bc1800bb6655d43ddee37ffe257de2ab485cd4a2"
OVERLAY_SIZE = 0x30000
OVERLAY_RECORD_LBA = 37129
OVERLAY_RECORD_OFFSET = 870
OVERLAY_LOAD_BASE = 0x80132000
SLOT_HELPER_OFFSET = 0x13FB0
SLOT_HELPER_SIZE = 0x128
SLOT_HELPER_SHA256 = "790ca273e08373ae91d101dac7e85650bd1c8c56cd89a23463998135e4c9efd0"
PROFILE_SCHEMA = "tokimemo.prologue-nbn-source-profile.v2"

SOURCE_FILES = {
    "src/ovl/name_entry/prologue.c": "cce3a4abaf102b745e0b116fcd1331ec63b35234d45b5bab4cbd9c39fc10470e",
    "src/main/gpu_effects.c": "11a9e631c3f3604d28ca1149faf2a7c5fe4c0b2de2b8b5662862a43ab8ed70f7",
    "asm/ovl/NAME_ENT/matchings/name_entry/prologue/prologue_background_slot_set.s": "aca95845e63ae78b16c6548335a87f9bafa28e22751b187a3a10d5c76e11cd67",
    "asm/ovl/NAME_ENT/matchings/name_entry/prologue/prologue_line_03_run.s": "3b2a256788b087d0a565e05c26ac025f95a9b0c6d3b02a543ffb40b3ba62f339",
    "asm/ovl/NAME_ENT/matchings/name_entry/prologue/prologue_scene_05_transition_run.s": "75dfeee7ed283e0cd4f55a60bc57cff11fa9e1fd3880e122eff9f73f2486cfee",
    "asm/ovl/NAME_ENT/matchings/name_entry/prologue/prologue_yoshio_greeting_run.s": "798523f3dd6fa843e3aa3c503f4e0fe82d1f9489378ef213ba0894a9780625fa",
    "asm/ovl/NAME_ENT/matchings/name_entry/prologue/prologue_line_11_run.s": "bff954fd9f021e0c92bca28730483d2be5c66143682f3a821803ac7765c41fcf",
    "asm/matchings/main/gpu_effects/prim_bg_slot_setup.s": "a536fc074615e1d5c6152563ca824304d5d94a16ed3d5a3951c199dccf6e9e99",
    "src/main/actor_sprites.c": "ec04dd6c419b8079ef9430a484eb084c44d7a52029cc1c6ea359fd8571dab546",
    "src/main/frame_render_audio.c": "b0932ca59d48bec7b3a3be55b8188b09f55f15721dbe96d84de14f2a0526d08e",
    "asm/matchings/main/actor_sprites/load_tpage_buf_lock.s": "c899777fda156693ed4c36172eb2dc6e67fd50d912603afa310b46712e2504a5",
    "asm/matchings/main/frame_render_audio/csr_load_vram.s": "688c5bad65b667ecf84ba78ee717ccf57d977118a80ea3df12581cc4f7e49c9b",
    "asm/matchings/main/frame_render_audio/LoadSquare.s": "37438dbf5244a359351132769cad8e2ba52d3a89ff4e028d9f365fe09ccb8787",
    "config/overlays/NAME_ENT.yaml": "844d84467d937cd6a6af8cba884c4dbafde062ba898671f20ceadd7f8783809a",
}
SOURCE_ANCHORS = {
    "src/ovl/name_entry/prologue.c": (
        "cd_read_sectors_nowait(0x31U, 0x801A4000, 0xAA23)",
        "cd_read_sectors(0x31U, 0x801A4000, 0xAA54)",
        "cd_read_sectors(0x31U, 0x801A4000, 0xAA85)",
        "cd_read_sectors(0x31U, 0x801A4000, 0xAAB6)",
        "for (i = 0; i < 6; i++) { addr[i] = image_address + i * 0x4000; }",
        "prim_bg_slot_setup(addr, 0, 6, 3, 2, palette_row_offset + 0x1E0, -0xA0, -0x78, 8, 8)",
        "load_palette((u8 *)(image_address + 0x18000), 0xF, 1, 2, 0)",
    ),
    "src/main/gpu_effects.c": (
        "cellh = 0x78;", "bg->unk_08 = w << 7;", "bg->unk_0A = h * cellh;",
        "layer_bg_maps[slot].cellw = 0x80;", "layer_bg_maps[slot].cellh = cellh;",
        "layer_bg_maps[slot].ncellw = w;", "layer_bg_maps[slot].ncellh = h;",
    ),
    "src/main/actor_sprites.c": (
        "load_csr_tp(arg1, (u8)(arg0 & 0x1F), (s32)arg2, 0, 0, 0x80, 0x80);",
        "load_csr_tp(arg1, (u8)(arg0 & 0x1F), (s32)arg2, 0, 0x80, 0x80, 0x80);",
    ),
    "src/main/frame_render_audio.c": (
        "g->csr_queue[index].unk_0A = arg4;",
        "LoadSquare(base->csr_queue[var_s1].unk_08, base->csr_queue[var_s1].unk_0A, base->csr_queue[var_s1].unk_0C, base->csr_queue[var_s1].unk_0E, (void *) base->csr_queue[var_s1].source);",
        "y = base->csr_queue[var_s1].unk_0A + (((temp_v0 = temp_v0) & 0x10) * 0x10);",
    ),
    "asm/matchings/main/actor_sprites/load_tpage_buf_lock.s": ("glabel load_tpage_buf_lock",),
    "asm/matchings/main/frame_render_audio/csr_load_vram.s": ("glabel csr_load_vram",),
    "asm/matchings/main/frame_render_audio/LoadSquare.s": ("glabel LoadSquare",),
}

FILES = (
    {"path": "CDROM/PROLODIR/CUT_05.NBN", "lba": 0xAA23,
     "sha256": "34494d1b0c7305466b097d62c4c68b10486325d70e71cb0115bc58d335ebd87c",
     "record_lba": 43554, "record_offset": 96, "record_length": 60,
     "identifier": "CUT_05.NBN;1", "native_code_offset": 0x141C0,
     "native_code_bytes": "23aa0634", "caller": "prologue_line_03_run"},
    {"path": "CDROM/PROLODIR/CUT_07.NBN", "lba": 0xAA54,
     "sha256": "3fff84684395c29849d57652a111282820977b4ca5fd44006deab14b3eefa125",
     "record_lba": 43554, "record_offset": 156, "record_length": 60,
     "identifier": "CUT_07.NBN;1", "native_code_offset": 0x14794,
     "native_code_bytes": "54aa0634", "caller": "prologue_scene_05_transition_run"},
    {"path": "CDROM/PROLODIR/CUT_08.NBN", "lba": 0xAA85,
     "sha256": "444165354346eeac36f2614a3375bd24e3ff5b457924b1d81705b4e174c3e3ca",
     "record_lba": 43554, "record_offset": 216, "record_length": 60,
     "identifier": "CUT_08.NBN;1", "native_code_offset": 0x15234,
     "native_code_bytes": "85aa0634", "caller": "prologue_yoshio_greeting_run"},
    {"path": "CDROM/PROLODIR/CUT_09.NBN", "lba": 0xAAB6,
     "sha256": "9c4a73089b11368c754de62a546344668182fb2b37ba9862e5bed0263d9cbd0b",
     "record_lba": 43554, "record_offset": 276, "record_length": 60,
     "identifier": "CUT_09.NBN;1", "native_code_offset": 0x15714,
     "native_code_bytes": "b6aa0634", "caller": "prologue_line_11_run"},
)

PAGE_SIZE = 0x4000
PAGE_COUNT = 6
PAGE_WIDTH = 128
SOURCE_PAGE_HEIGHT = 128
ACTIVE_CELL_HEIGHT = 120
PALETTE_OFFSET = 0x18000
PALETTE_SIZE = 0x200
TRAILER_OFFSET = PALETTE_OFFSET + PALETTE_SIZE
FILE_SIZE = 0x18800


def _sha(data):
    return hashlib.sha256(data).hexdigest()


def _check_witnesses(repository_root):
    root = Path(repository_root).resolve()
    actual = {}
    for relative, expected_hash in SOURCE_FILES.items():
        data = (root / relative).read_bytes()
        if _sha(data) != expected_hash:
            raise ValueError("prologue source witness hash differs: " + relative)
        text = data.decode("utf-8")
        for anchor in SOURCE_ANCHORS.get(relative, ()):
            if anchor not in text:
                raise ValueError("prologue source witness anchor missing: " + relative)
        actual[relative] = expected_hash
    return actual


def _check_original_code(exe_data, overlay_data):
    if len(exe_data) < EXE_HELPER_OFFSET + EXE_HELPER_SIZE or _sha(exe_data) != EXE_SHA256:
        raise ValueError("original main executable does not match the pinned prologue helper witness")
    helper = exe_data[EXE_HELPER_OFFSET:EXE_HELPER_OFFSET + EXE_HELPER_SIZE]
    if _sha(helper) != EXE_HELPER_SHA256:
        raise ValueError("original prim_bg_slot_setup bytes differ")
    if len(overlay_data) != OVERLAY_SIZE or _sha(overlay_data) != OVERLAY_SHA256:
        raise ValueError("original NAME_ENT overlay differs from the pinned source witness")
    slot_helper = overlay_data[SLOT_HELPER_OFFSET:SLOT_HELPER_OFFSET + SLOT_HELPER_SIZE]
    if _sha(slot_helper) != SLOT_HELPER_SHA256:
        raise ValueError("original prologue_background_slot_set bytes differ")
    for row in FILES:
        start = row["native_code_offset"]
        if overlay_data[start:start + 4].hex() != row["native_code_bytes"]:
            raise ValueError("original prologue overlay LBA instruction differs: " + row["path"])
    return {"executable_sha256": EXE_SHA256,
            "geometry_helper": {"path": EXE_PATH, "file_offset": EXE_HELPER_OFFSET,
                                "size": EXE_HELPER_SIZE, "sha256": _sha(helper)},
            "overlay_sha256": OVERLAY_SHA256,
            "slot_helper": {"path": OVERLAY_PATH, "file_offset": SLOT_HELPER_OFFSET,
                            "size": SLOT_HELPER_SIZE, "sha256": _sha(slot_helper)},
            "native_lba_instruction_bytes_verified": [
                {"path": row["path"], "offset": row["native_code_offset"],
                 "bytes_hex": row["native_code_bytes"]} for row in FILES]}


def _check_asset_inventory(entries):
    by_path = {entry.get("path"): entry for entry in entries}
    checked = []
    for row in FILES:
        entry = by_path.get(row["path"])
        location = (entry or {}).get("location", {})
        if (entry is None or entry.get("kind") != "file"
                or entry.get("size") != FILE_SIZE or entry.get("sha256") != row["sha256"]
                or location.get("lba") != row["lba"]
                or location.get("extent_bytes") != FILE_SIZE
                or location.get("record_lba") != row["record_lba"]
                or location.get("record_offset") != row["record_offset"]
                or len(location.get("sectors", [])) != 49):
            raise ValueError("original prologue NBN inventory differs: " + row["path"])
        checked.append((row, entry))
    return checked


def _verify_directory_record(source, row):
    entry = {"path": "<prologue-directory-record>", "kind": "file", "size": 2048,
             "location": {"lba": row["record_lba"], "extent_bytes": 2048}}
    sectors = list(source.iter_raw_sectors(entry, TRACK_SHA256))
    if (len(sectors) != 1 or sectors[0].get("form") != 1
            or not sectors[0].get("subheader_valid") or sectors[0].get("payload_size") != 2048):
        raise ValueError("prologue NBN directory record sector is not valid Form1")
    payload = sectors[0]["payload"]
    offset = row["record_offset"]
    length = payload[offset]
    record = payload[offset:offset + length]
    identifier_length = record[32] if len(record) >= 33 else 0
    identifier = record[33:33 + identifier_length].decode("ascii")
    lba = int.from_bytes(record[2:6], "little")
    size = int.from_bytes(record[10:14], "little")
    if (length != row["record_length"] or len(record) != length
            or identifier != row["identifier"] or lba != row["lba"]
            or int.from_bytes(record[6:10], "big") != row["lba"]
            or size != FILE_SIZE or int.from_bytes(record[14:18], "big") != FILE_SIZE):
        raise ValueError("prologue NBN ISO directory identity or extent differs: " + row["path"])
    return {"record_lba": row["record_lba"], "record_offset": offset,
            "record_length": length, "identifier": identifier, "extent_lba": lba,
            "extent_size_bytes": size, "sector_payload_sha256": _sha(payload)}


def _verify_overlay_record(source, entry):
    location = entry.get("location", {})
    if (location.get("record_lba") != OVERLAY_RECORD_LBA
            or location.get("record_offset") != OVERLAY_RECORD_OFFSET
            or location.get("lba") != 38378 or entry.get("size") != OVERLAY_SIZE):
        raise ValueError("original NAME_ENT directory entry differs")
    sector_entry = {"path": "<NAME_ENT-directory-record>", "kind": "file", "size": 2048,
                    "location": {"lba": OVERLAY_RECORD_LBA, "extent_bytes": 2048}}
    sectors = list(source.iter_raw_sectors(sector_entry, TRACK_SHA256))
    if (len(sectors) != 1 or sectors[0].get("form") != 1
            or not sectors[0].get("subheader_valid")):
        raise ValueError("NAME_ENT directory record sector is not valid Form1")
    payload = sectors[0]["payload"]
    offset = OVERLAY_RECORD_OFFSET
    length = payload[offset]
    record = payload[offset:offset + length]
    identifier_length = record[32] if len(record) >= 33 else 0
    identifier = record[33:33 + identifier_length].decode("ascii")
    if (identifier != "NAME_ENT.EXN;1" or int.from_bytes(record[2:6], "little") != 38378
            or int.from_bytes(record[6:10], "big") != 38378
            or int.from_bytes(record[10:14], "little") != OVERLAY_SIZE
            or int.from_bytes(record[14:18], "big") != OVERLAY_SIZE):
        raise ValueError("NAME_ENT ISO record identity or extent differs")
    return {"record_lba": OVERLAY_RECORD_LBA, "record_offset": offset,
            "record_length": length, "identifier": identifier,
            "extent_lba": 38378, "extent_size_bytes": OVERLAY_SIZE,
            "sector_payload_sha256": _sha(payload)}


def _verify_native_read(source, row, sector_count=49, declared_size=FILE_SIZE):
    native_entry = {"path": row["path"], "kind": "file", "size": declared_size,
                    "location": {"lba": row["lba"], "extent_bytes": declared_size}}
    sectors = list(source.iter_raw_sectors(native_entry, TRACK_SHA256))
    if len(sectors) != sector_count:
        raise ValueError("prologue native sector read length differs: " + row["path"])
    for index, sector in enumerate(sectors):
        if (sector.get("lba") != row["lba"] + index or sector.get("form") != 1
                or not sector.get("subheader_valid") or sector.get("payload_size") != 2048):
            raise ValueError("prologue native read contains an invalid Form1 sector: " + row["path"])
    return {"requested_sector_count": sector_count, "iso_extent_sector_count": sector_count,
            "sectors_after_iso_extent": 0,
            "raw_sector_extent_sha256": _sha(b"".join(s["raw"] for s in sectors)),
            "payload_extent_sha256": _sha(b"".join(s["payload"] for s in sectors)),
            "sectors": [{"lba": s["lba"], "form": s["form"],
                         "raw_sha256": s["raw_sha256"],
                         "payload_sha256": s["payload_sha256"]} for s in sectors]}


def build_profile_context(source, entries, repository_root):
    """Revalidate source, executable, overlay, directory records, and native reads."""
    source_files = _check_witnesses(repository_root)
    if len(FILES) != 4 or len({row["lba"] for row in FILES}) != 4:
        raise ValueError("trusted prologue profile must contain four distinct source LBAs")
    selected = _check_asset_inventory(entries)
    description = source.describe()
    if (description.get("format") != "mode2-2352"
            or description.get("source_sha256") != TRACK_SHA256):
        raise ValueError("original prologue MODE2 track differs")
    by_path = {entry.get("path"): entry for entry in entries}
    overlay_entry = by_path.get(OVERLAY_PATH)
    exe_entry = by_path.get(EXE_PATH)
    if (overlay_entry is None or overlay_entry.get("kind") != "file"
            or overlay_entry.get("size") != OVERLAY_SIZE or exe_entry is None
            or exe_entry.get("kind") != "file"):
        raise ValueError("original prologue overlay or main executable is absent")
    overlay_data = source.read(overlay_entry)
    exe_data = source.read(exe_entry)
    native_code = _check_original_code(exe_data, overlay_data)
    overlay_record = _verify_overlay_record(source, overlay_entry)
    overlay_read = _verify_native_read(source,
                                      {"path": OVERLAY_PATH, "lba": overlay_entry["location"]["lba"]},
                                      sector_count=96, declared_size=OVERLAY_SIZE)
    records = []
    for row, entry in selected:
        data = source.read(entry)
        if _sha(data) != row["sha256"]:
            raise ValueError("prologue NBN bytes differ from the pinned original: " + row["path"])
        record_proof = _verify_directory_record(source, row)
        read_proof = _verify_native_read(source, row)
        records.append({
            "path": row["path"], "lba": row["lba"], "caller": row["caller"],
            "source_sha256": row["sha256"], "source_size": FILE_SIZE,
            "iso_directory_record": record_proof,
            "native_instruction": {"overlay_file_offset": row["native_code_offset"],
                                   "bytes_hex": row["native_code_bytes"]},
            "native_read_extent": read_proof,
            "source": {"path": row["path"], "sha256": row["sha256"],
                       "size": FILE_SIZE, "location": {"lba": row["lba"]}},
        })
    return {"schema": PROFILE_SCHEMA, "source_files": source_files,
            "native_code": native_code, "overlay_directory_record": overlay_record,
            "overlay_native_read_extent": overlay_read, "source_assets": records,
            "texture_profile": {"page_count": PAGE_COUNT,
                                "page_source_bytes": PAGE_SIZE,
                                "source_page_dimensions": [PAGE_WIDTH, SOURCE_PAGE_HEIGHT],
                                "active_cell_dimensions": [PAGE_WIDTH, ACTIVE_CELL_HEIGHT],
                                "native_uv_mapping": {
                                    "upper_grid_row": {"cell_v": 0, "upload_v": 0,
                                                       "source_rows": [0, ACTIVE_CELL_HEIGHT]},
                                    "lower_grid_row": {"cell_v": ACTIVE_CELL_HEIGHT,
                                                       "lower_page_upload_v": SOURCE_PAGE_HEIGHT,
                                                       "source_spans": [
                                                           {"page_row": 0, "rows": [ACTIVE_CELL_HEIGHT, SOURCE_PAGE_HEIGHT]},
                                                           {"page_row": 1, "rows": [0, ACTIVE_CELL_HEIGHT - (SOURCE_PAGE_HEIGHT - ACTIVE_CELL_HEIGHT)]}]},
                                    "unused_source_spans": [{"page_row": 1,
                                                             "rows": [ACTIVE_CELL_HEIGHT - (SOURCE_PAGE_HEIGHT - ACTIVE_CELL_HEIGHT), SOURCE_PAGE_HEIGHT]}]},
                                "source_pixels_preserved_in_original": True,
                                "grid_columns": 3, "grid_rows": 2,
                                "palette_offset": PALETTE_OFFSET,
                                "palette_word_count": 256,
                                "uninterpreted_trailer_offset": TRAILER_OFFSET,
                                "uninterpreted_trailer_size": FILE_SIZE - TRAILER_OFFSET,
                                "palette_source": "embedded source NBN BGR555 CLUT",
                                "preview_palette": "source color palette only; runtime grayscale alternative is not emitted",
                                "bit15_mask_interpretation": "raw embedded CLUT bit 15 only; runtime STP behavior is unverified",
                                "runtime_scene_composition_claimed": False}}


def probe_prologue_pages(data, *, source_row):
    """Validate an exact fixed NBN family member and its embedded palette."""
    try:
        if (not isinstance(source_row, dict) or len(data) != FILE_SIZE
                or _sha(data) != source_row.get("source_sha256")
                or source_row.get("source_size") != FILE_SIZE):
            raise ImageFormatError("prologue source path, size, or hash differs from profile")
        palette_bytes = data[PALETTE_OFFSET:PALETTE_OFFSET + PALETTE_SIZE]
        if len(palette_bytes) != PALETTE_SIZE:
            raise ImageFormatError("prologue embedded palette is truncated")
        return {"status": "supported", "format": "tokimemo-prologue-native-cells",
                "metadata": {"source_path": source_row["path"],
                             "source_sha256": source_row["source_sha256"],
                             "page_count": PAGE_COUNT, "page_bytes": PAGE_SIZE,
                             "source_page_dimensions": [PAGE_WIDTH, SOURCE_PAGE_HEIGHT],
                             "active_cell_dimensions": [PAGE_WIDTH, ACTIVE_CELL_HEIGHT],
                             "grid": [3, 2], "palette_offset": PALETTE_OFFSET,
                             "palette_size": PALETTE_SIZE,
                             "palette_sha256": _sha(palette_bytes),
                             "palette_word_count": 256,
                             "palette_source": "embedded original BGR555 color CLUT",
                             "bit15_mask_interpretation": "raw source palette bit 15 diagnostic; runtime STP behavior is unverified",
                             "native_uv_mapping": "upper cells sample page rows 0..119; lower cells sample upper page rows 120..127 followed by lower page rows 0..111",
                             "source_pixels_preserved_in_original": True,
                             "uninterpreted_trailer": {"offset": TRAILER_OFFSET,
                                                        "size": FILE_SIZE - TRAILER_OFFSET,
                                                        "sha256": _sha(data[TRAILER_OFFSET:])},
                             "runtime_grayscale_variant_emitted": False,
                             "runtime_scene_composition_claimed": False}}
    except (ImageFormatError, TypeError, KeyError) as error:
        return {"status": "invalid", "format": "tokimemo-prologue-native-cells",
                "reason": str(error)}


def export_prologue_pages(data, output_dir, asset_id, *, source_row):
    """Export the six cells using the proven upload-V and cell-UV mapping."""
    if (not isinstance(asset_id, str)
            or not re.fullmatch(r"[A-Za-z0-9][A-Za-z0-9_.-]{0,127}", asset_id)
            or asset_id in (".", "..")):
        raise ImageFormatError("asset_id must be one safe filename component")
    inspection = probe_prologue_pages(data, source_row=source_row)
    if inspection["status"] != "supported":
        raise ImageFormatError(inspection.get("reason", "unsupported prologue NBN"))
    palette = struct.unpack_from("<256H", data, PALETTE_OFFSET)
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    images, files = [], []
    palette_sha = _sha(data[PALETTE_OFFSET:PALETTE_OFFSET + PALETTE_SIZE])

    def page_rows(page_index, first, last):
        start = page_index * PAGE_SIZE
        return data[start + first * PAGE_WIDTH:start + last * PAGE_WIDTH]

    for index in range(PAGE_COUNT):
        row, col = divmod(index, 3)
        if row == 0:
            upper_page = data[col * PAGE_SIZE:(col + 1) * PAGE_SIZE]
            spans = [{"page_index": col, "page_sha256": _sha(upper_page),
                      "source_region": [0, 0, PAGE_WIDTH, ACTIVE_CELL_HEIGHT]}]
            indices = page_rows(col, 0, ACTIVE_CELL_HEIGHT)
            unused_spans = []
        else:
            upper_index, lower_index = col, col + 3
            upper_page = data[upper_index * PAGE_SIZE:(upper_index + 1) * PAGE_SIZE]
            lower_page = data[lower_index * PAGE_SIZE:(lower_index + 1) * PAGE_SIZE]
            upper_rows = SOURCE_PAGE_HEIGHT - ACTIVE_CELL_HEIGHT
            lower_rows = ACTIVE_CELL_HEIGHT - upper_rows
            spans = [
                {"page_index": upper_index, "page_sha256": _sha(upper_page),
                 "source_region": [0, ACTIVE_CELL_HEIGHT, PAGE_WIDTH, upper_rows]},
                {"page_index": lower_index, "page_sha256": _sha(lower_page),
                 "source_region": [0, 0, PAGE_WIDTH, lower_rows]},
            ]
            indices = (page_rows(upper_index, ACTIVE_CELL_HEIGHT, SOURCE_PAGE_HEIGHT)
                       + page_rows(lower_index, 0, lower_rows))
            unused_spans = [{"page_index": lower_index,
                             "source_region": [0, lower_rows, PAGE_WIDTH,
                                               SOURCE_PAGE_HEIGHT - lower_rows]}]
        rgba = b"".join(_color(palette[value]) for value in indices)
        bit15 = bytes(255 if palette[value] & 0x8000 else 0 for value in indices)
        name = "%s.cell-%d" % (asset_id, index)
        png, bit15_png = name + ".png", name + ".clut-bit15.png"
        for filename, content in ((png, _png(PAGE_WIDTH, ACTIVE_CELL_HEIGHT, rgba, 4)),
                                  (bit15_png, _png(PAGE_WIDTH, ACTIVE_CELL_HEIGHT, bit15, 1))):
            with (output_dir / filename).open("xb") as stream:
                stream.write(content)
            files.append(filename)
        images.append({"png": png, "clut_bit15_png": bit15_png, "cell_index": index,
                       "variant": "native-cell-%d" % index,
                       "grid_row": row, "grid_column": col,
                       "preview_kind": "native_uv_mapped_texture_cell",
                       "width": PAGE_WIDTH, "height": ACTIVE_CELL_HEIGHT,
                       "source_texture_dimensions": [PAGE_WIDTH, SOURCE_PAGE_HEIGHT],
                       "source_spans": spans, "unused_source_spans_for_cell": unused_spans,
                       "whole_original_retained": True,
                       "palette_source_offset": PALETTE_OFFSET,
                       "palette_source_sha256": palette_sha, "palette_words": list(palette),
                       "palette_status": "embedded source color CLUT; runtime grayscale alternative not rendered",
                       "clut_bit15_status": "raw source CLUT bit-15 diagnostic; runtime STP behavior unverified"})
    return {"status": "exported", "format": "tokimemo-prologue-native-cells",
            "files": files, "images": images,
            "metadata": {**inspection["metadata"],
                         "caller_profile": deepcopy(source_row),
                         "alpha_policy": "BGR555 zero word transparent; all other words opaque",
                         "clut_bit15_policy": "separate raw source CLUT bit-15 diagnostic; runtime STP behavior is unverified",
                         "hardware_raster_parity_verified": False,
                         "round_trip_verified": False,
                         "source_bytes_unmodified": True,
                         "native_uv_mapping_applied": True}}
