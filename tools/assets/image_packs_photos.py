"""Source-bound original LASTPHOT and STAFF2 photo pages (T-28002).

Native ETC LBA tables bind last photos. Literal ETC reads bind staff photos,
whose palette is a separately hashed source file. No filename-derived CLUT,
stitched background or runtime fade/composition is invented.
"""
from copy import deepcopy
import hashlib
from pathlib import Path
import re
import struct

from .images import ImageFormatError, _color, _png

ETC_BASE = 0x80132000
ETC_SIZE = 0x30000
ETC_SHA1 = '681c93b6e57c885f86e074c885a8bf6c3cca2319'
LAST_TABLE = 0x80150390
ALL_CLEAR_TABLE = 0x801503C4
STAFF_PALETTE_PATH = 'CDROM/STAFF2/STAF2COL.NBN'
STAFF_PALETTE_LBA = 0xB2CC
LAST_PATHS = {'CDROM/LASTPHOT/LASTP_' + suffix + '.NBN'
              for suffix in ('AS', 'HI', 'HU', 'IZ', 'KA', 'KG', 'KI', 'KO', 'KY', 'MI', 'NI', 'TA', 'YU')}
STAFF_PATHS = {'CDROM/STAFF2/STAF2' + suffix + '.NBN'
               for suffix in ('AS', 'HI', 'HU', 'IJ', 'KG', 'KO', 'KS', 'KT', 'KY', 'MK', 'NZ', 'SA', 'TA', 'YO')}
# Literal read destination order in src/ovl/misc_scenes/staff_roll_asset_load.c (default variant).
STAFF_LBAS = (0xB2DA, 0xB30E, 0xB2CD, 0xB31B, 0xB342, 0xB301, 0xB328,
              0xB2F4, 0xB2BF, 0xB335, 0xB34F, 0xB35C, 0xB2E7, 0xB369)


def _etc_spec():
    # Fixed original-profile facts; exporting saved sources needs no checkout,
    # mutable config files, compiler outputs or current symbol spelling.
    return {'sha1': ETC_SHA1, 'load_base': ETC_BASE, 'size': ETC_SIZE,
            'source_evidence_paths': ['config/overlays/ETC.yaml', 'config/symbols_ETC.txt']}


def _verify_entry(data, entry, *, path=None, size=None, lba=None):
    if (not isinstance(entry, dict) or not isinstance(entry.get('path'), str)
            or not isinstance(entry.get('location'), dict)
            or not isinstance(entry['location'].get('lba'), int)
            or isinstance(entry['location']['lba'], bool)
            or len(data) != entry.get('size') or hashlib.sha256(data).hexdigest() != entry.get('sha256')
            or path is not None and entry['path'] != path
            or size is not None and len(data) != size
            or lba is not None and entry['location']['lba'] != lba):
        raise ImageFormatError('photo source path/LBA/extent/hash does not match declared original input')


def _reference(entry):
    return {key: deepcopy(entry.get(key)) for key in ('path', 'size', 'sha256')} | {
        'location': {key: deepcopy(entry['location'][key]) for key in ('lba', 'extent_bytes') if key in entry['location']}}


def photo_profile_rows(overlay_data, *, overlay_entry):
    """Return native/literal LBA bindings after validating the original ETC pair."""
    spec = _etc_spec()
    _verify_entry(overlay_data, overlay_entry, path='CDROM/EXEDIR/ETC.EXN', size=spec['size'])
    if hashlib.sha1(overlay_data).hexdigest() != spec['sha1'] or spec['load_base'] != ETC_BASE:
        raise ImageFormatError('ETC native SHA1/load base differs from canonical configuration')
    if ALL_CLEAR_TABLE - ETC_BASE + 4 > len(overlay_data):
        raise ImageFormatError('ETC native photo LBA tables are truncated')
    lbas = struct.unpack_from('<13I', overlay_data, LAST_TABLE - ETC_BASE)
    all_clear = struct.unpack_from('<I', overlay_data, ALL_CLEAR_TABLE - ETC_BASE)[0]
    if any(not value for value in lbas) or not all_clear or all_clear in lbas:
        raise ImageFormatError('ETC photo LBA rows are missing or conflict across source layouts')
    rows = [{'kind': 'last_photo', 'profile_index': index, 'source_lba': lba,
             'native_lba_pointer_address': LAST_TABLE + index * 4,
             'read_sector_count': 0x21, 'runtime_load_address': 0x80180000,
             'page_count': 4, 'bits_per_pixel': 8, 'palette_source_offset': 0x10000,
             'palette_queue_mode': 3, 'clut_vram': [0, 0x1FF], 'grid': [2, 2]}
            for index, lba in enumerate(lbas)]
    rows.append({'kind': 'last_photo_all_clear', 'profile_index': None, 'source_lba': all_clear,
                 'native_lba_pointer_address': ALL_CLEAR_TABLE, 'read_sector_count': 0x31,
                 'runtime_load_address': 0x80180000, 'page_count': 6, 'bits_per_pixel': 8,
                 'palette_source_offset': 0x18000, 'palette_queue_mode': 3,
                 'clut_vram': [0, 0x1FF], 'grid': [3, 2]})
    rows.extend({'kind': 'staff2_photo', 'profile_index': index, 'source_lba': lba,
                 'native_lba_pointer_address': None, 'read_sector_count': 0xD,
                 'runtime_load_address': 0x801E8000 - index * 0x8000,
                 'page_count': 3, 'bits_per_pixel': 4, 'palette_source_offset': 0,
                 'palette_queue_mode': 1, 'clut_vram': [0, 0x1F1], 'grid': None}
                for index, lba in enumerate(STAFF_LBAS))
    for row in rows:
        row['read_sector_count_hex'] = hex(row['read_sector_count'])
        row['source_byte_count'] = row['read_sector_count'] * 2048
        row['overlay_source'] = _reference(overlay_entry)
        row['canonical_overlay_profile'] = {'sha1': spec['sha1'], 'load_base': ETC_BASE,
                                             'size': ETC_SIZE, 'last_lba_table_vma': LAST_TABLE,
                                             'all_clear_lba_vma': ALL_CLEAR_TABLE}
        row['source_evidence_paths'] = spec['source_evidence_paths']
        if row['kind'] == 'staff2_photo':
            row['runtime_variant'] = 'staff_roll_variant == 0'
    return rows


def _parse(data, *, source_entry, overlay_data, overlay_entry, palette_data=None, palette_entry=None):
    _verify_entry(data, source_entry)
    lba, path = source_entry['location']['lba'], source_entry['path']
    rows = [row for row in photo_profile_rows(overlay_data, overlay_entry=overlay_entry) if row['source_lba'] == lba]
    if len(rows) != 1:
        raise ImageFormatError('photo source LBA does not have one original caller profile')
    profile = rows[0]
    staff = profile['kind'] == 'staff2_photo'
    allowed = STAFF_PATHS if staff else ({'CDROM/LASTPHOT/ALL_C.NBN'}
                                       if profile['kind'] == 'last_photo_all_clear' else LAST_PATHS)
    if path not in allowed:
        raise ImageFormatError('photo source path differs from original caller family')
    if len(data) != profile['source_byte_count']:
        raise ImageFormatError('photo read count differs from original file extent')
    if staff:
        if not isinstance(palette_data, bytes):
            raise ImageFormatError('STAFF2 requires its original separately hashed palette file')
        _verify_entry(palette_data, palette_entry, path=STAFF_PALETTE_PATH, size=2048, lba=STAFF_PALETTE_LBA)
        source_palette = struct.unpack_from('<256H', palette_data)
        palette = source_palette[:16]
        palette_reference = _reference(palette_entry)
    else:
        source_palette = struct.unpack_from('<256H', data, profile['palette_source_offset'])
        palette = tuple(word | 0x8000 if word & 0x7FFF else word for word in source_palette)
        palette_reference = _reference(source_entry)
    return profile, source_palette, palette, palette_reference


def probe_photos(data, *, source_entry, overlay_data, overlay_entry, palette_data=None, palette_entry=None):
    """Validate exact original caller/native-table and palette bindings."""
    try:
        profile, _, _, _ = _parse(data, source_entry=source_entry, overlay_data=overlay_data,
                                  overlay_entry=overlay_entry, palette_data=palette_data, palette_entry=palette_entry)
    except (ImageFormatError, OSError) as error:
        return {'status': 'invalid', 'format': 'tokimemo-photo-pages', 'reason': str(error)}
    return {'status': 'supported', 'format': 'tokimemo-photo-pages', 'metadata': deepcopy(profile)}


def _extent(data, offset, size, status):
    return {'offset': offset, 'size': size, 'sha256': hashlib.sha256(data[offset:offset + size]).hexdigest(), 'status': status}


def export_photos(data, exe, output_dir, asset_id, *, source_entry, overlay_data, overlay_entry,
                  palette_data=None, palette_entry=None):
    """Export actual-CLUT raw pages; no unproved background stitching or fades."""
    profile, raw_palette, palette, palette_reference = _parse(
        data, source_entry=source_entry, overlay_data=overlay_data, overlay_entry=overlay_entry,
        palette_data=palette_data, palette_entry=palette_entry)
    if not isinstance(asset_id, str) or not re.fullmatch(r'[A-Za-z0-9][A-Za-z0-9_.-]{0,159}', asset_id):
        raise ImageFormatError('unsafe photo asset_id')
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    images, files = [], []
    staff = profile['kind'] == 'staff2_photo'
    page_bytes = 0x2000 if staff else 0x4000
    for index in range(profile['page_count']):
        offset = index * page_bytes
        source = data[offset:offset + page_bytes]
        indices = [((source[pixel // 2] >> ((pixel & 1) * 4)) & 15) if staff else source[pixel]
                   for pixel in range(128 * 128)]
        words = [palette[value] for value in indices]
        variant = 'source-page-%d' % index
        png, mask = '%s.%s.png' % (asset_id, variant), '%s.%s.stp.png' % (asset_id, variant)
        for filename, content in ((png, _png(128, 128, b''.join(_color(word) for word in words), 4)),
                                  (mask, _png(128, 128, bytes(255 if word & 0x8000 else 0 for word in words), 1))):
            with (output_dir / filename).open('xb') as output:
                output.write(content)
            files.append(filename)
        if staff:
            slot, map_cell = 5 + index, None
        else:
            row, column = divmod(index, profile['grid'][0])
            slot = 9 + column + row * 32
            map_cell = {'column': column, 'row': row, 'u': 0, 'v': row * 120,
                        'width': 128, 'height': 120, 'tpage': 9 + column}
        images.append({'png': png, 'stp_png': mask, 'variant': variant, 'frame_index': None,
                       'preview_kind': 'source_texture_page', 'width': 128, 'height': 128, 'parts': [],
                       'bits_per_pixel': profile['bits_per_pixel'], 'source_sheet_offset': offset,
                       'source_sheet_size': page_bytes, 'source_sheet_sha256': hashlib.sha256(source).hexdigest(),
                       'source_region': {'x': 0, 'y': 0, 'width': 128, 'height': 128},
                       'source_palette_words': list(raw_palette[:16] if staff else raw_palette),
                       'preview_palette_words': list(palette), 'runtime_loaded_palette_row_words': list(raw_palette),
                       'palette_source_offset': profile['palette_source_offset'], 'palette_source': palette_reference,
                       'clut_vram': profile['clut_vram'], 'palette_queue_mode': profile['palette_queue_mode'],
                       'palette_status': 'established by original ETC caller upload and draw setup',
                       'runtime_load_address': profile['runtime_load_address'], 'upload_slot': slot,
                       'vram_rectangle_words': [(slot & 15) * 64, (slot & 16) * 16 + (slot & 32) * 4,
                                                32 if staff else 64, 128],
                       'runtime_bg_cell': map_cell,
                       'composition_status': 'native raw page only; background row120/upload128 or staff placement/fades are not composed'})
    pixel_end = profile['page_count'] * page_bytes
    tail_start = pixel_end if staff else profile['palette_source_offset'] + 0x200
    opaque = [_extent(data, tail_start, len(data) - tail_start, 'uninterpreted trailer outside pixel pages and loaded palette')]
    paired_extents = ([_extent(palette_data, 0x200, len(palette_data) - 0x200, 'uninterpreted paired-palette trailer'),
                       _extent(palette_data, 32, 480, 'loaded palette entries16..255 not used by these4bpp page previews')]
                      if staff else [])
    dependencies = [profile['overlay_source']] + ([palette_reference] if staff else [])
    metadata = {'source_sha256': hashlib.sha256(data).hexdigest(), 'source_size': len(data),
                'source_path_profile': source_entry['path'], 'caller_profile': profile,
                'source_dependencies': dependencies, 'bits_per_pixel': profile['bits_per_pixel'],
                'sheet_offsets': [i * page_bytes for i in range(profile['page_count'])],
                'sheet_bytes': page_bytes, 'sheet_size': [128, 128],
                'palette_source': palette_reference, 'palette_source_offset': profile['palette_source_offset'],
                'palette_source_size': 0x200, 'palette_queue_mode': profile['palette_queue_mode'],
                'paired_palette_read_sector_count': 1 if staff else None,
                'paired_palette_source_extents': paired_extents, 'uninterpreted_extents': opaque,
                'alpha_policy': 'BGR555 zero word transparent; nonzero opaque',
                'stp_policy': 'separate bit15 mask; mode1 preserves source words, mode3 sets STP on nonblack words',
                'coverage_policy': 'all caller-established raw photo pages; other source bytes/palettes and runtime composition remain unresolved',
                'hardware_raster_parity_verified': False, 'round_trip_verified': False,
                'source_evidence': ['src/ovl/misc_scenes/staff_roll_and_clear_save.c:last_photo_load_step,staff_roll_girl_textures_load,staff_roll_pictures_place',
                                    'src/ovl/misc_scenes/staff_roll_asset_load.c:staff_roll_load_step',
                                    'src/main/gpu_effects.c:prim_bg_slot_setup',
                                    'src/main/frame_render_audio.c:palette_load_vram',
                                    'config/symbols_ETC.txt:last_photo_lba,last_photo_all_clear_lba']}
    return {'status': 'exported', 'format': 'tokimemo-photo-pages', 'files': files, 'images': images, 'metadata': metadata}
