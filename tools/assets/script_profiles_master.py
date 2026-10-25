"""MASTER linked dialogue roots from original loader and initializer sources.

Eight files supply talk tables at 0x801E8000. Six class-specific initializers
share one runtime triad of globals; KOSIEN and TAI_MAS use a second triad.
TAI_MAS also reuses the same layout through a third initializer/global triad.
Group bounds come from the first linked command-stream address immediately
following the text root, checked against all parallel pointer slots. No NUL
scans discover layouts, and unclaimed bytes are not called non-text.
"""
from __future__ import annotations

import hashlib
import struct


def verify_boundaries(data: bytes, profile: dict) -> dict:
    """Validate pinned linked root extents, without searching for strings."""
    if profile.get("sha256") and hashlib.sha256(data).hexdigest() != profile["sha256"]:
        raise ValueError("MASTER original input differs from pinned profile")
    if "size" in profile and len(data) != profile["size"]:
        raise ValueError("MASTER original input size differs from pinned profile")
    base = profile["load_address"]
    evidence = []
    for section in profile["sections"]:
        count = section["group_count"]
        if not isinstance(count, int) or not 0 < count <= 4096:
            raise ValueError("MASTER group count is outside bounded limits")
        groups = {}
        for kind in ("text", "command", "voice"):
            root = section[kind + "_table"]
            offset = root - base
            if root % 4 or offset < 0 or offset + count * 4 > len(data):
                raise ValueError("MASTER root slots are outside the original input")
            pointers = list(struct.unpack_from("<" + "I" * count, data, offset))
            if any(not base <= pointer < base + len(data) for pointer in pointers):
                raise ValueError("MASTER root target is outside the original input")
            groups[kind] = pointers
        first_command = groups["command"][0]
        if first_command != min(groups["command"]) or first_command != section["text_table"] + count * 4:
            raise ValueError("MASTER text root does not end at the first linked command stream")
        evidence.append({"section": section["id"], "group_count": count,
                         "text_root_end_address": first_command,
                         "first_command_pointer_offset": section["command_table"] - base,
                         "parallel_root_slots_verified": count * 3})
    return {"status": "verified_linked_root_boundaries", "sections": evidence,
            "unclaimed_bytes_are_not_proven_nontext": True}

PROFILES = [{'id': 'master-en-gp',
  'path': 'CDROM/MASTER/EN_GP.BIN',
  'sha256': 'a36d7e72d8e3f527b82e379702be37013c7214425f90c3c2e0b47e818d7b1eea',
  'size': 6144,
  'load_address': 0x801e8000,
  'iso_lba': 0xa9b4,
  'load_sectors': 3,
  'tournament_class': 1,
  'sections': [{'id': 'table-0',
                'text_table': 0x801e8a1c,
                'command_table': 0x801e8b58,
                'voice_table': 0x801e94b8,
                'group_count': 33,
                'boundary_evidence': 'text root ends at first linked command stream; parallel roots and end '
                                     'commands validated'}],
  'evidence': ['src/ovl/MASTER/80132000.c::func_80132000', 'src/ovl/MASTER/80132140.c::cd_read_sectors'],
  'source_pins': {'src/ovl/MASTER/80132140.c': 'cba0eb244838f33d1d4275ddbb9b61e8cd909e0878da8d2583c09e88f2cd5cde',
                  'src/ovl/MASTER/80132000.c': '9ac659af38b272185ea6b161ecaa96070c43ee46f50dc493aa8dd1a15426d68d'}},
 {'id': 'master-g-con',
  'path': 'CDROM/MASTER/G_CON.BIN',
  'sha256': 'e9a91d556935b2b9a8dfa84bbf1a50bb5ffc2667a8b69d3032a58891508ab6ce',
  'size': 6144,
  'load_address': 0x801e8000,
  'iso_lba': 0xa9b7,
  'load_sectors': 3,
  'tournament_class': 3,
  'sections': [{'id': 'table-0',
                'text_table': 0x801e89cc,
                'command_table': 0x801e8afc,
                'voice_table': 0x801e93e4,
                'group_count': 31,
                'boundary_evidence': 'text root ends at first linked command stream; parallel roots and end '
                                     'commands validated'}],
  'evidence': ['src/ovl/MASTER/80132000.c::func_80132034', 'src/ovl/MASTER/80132140.c::cd_read_sectors'],
  'source_pins': {'src/ovl/MASTER/80132140.c': 'cba0eb244838f33d1d4275ddbb9b61e8cd909e0878da8d2583c09e88f2cd5cde',
                  'src/ovl/MASTER/80132000.c': '9ac659af38b272185ea6b161ecaa96070c43ee46f50dc493aa8dd1a15426d68d'}},
 {'id': 'master-kosien',
  'path': 'CDROM/MASTER/KOSIEN.BIN',
  'sha256': 'b174b6340dd3fc1aac1460dd0e18025fd6a195e59ae8e4c8b4cdcaefca58518d',
  'size': 4096,
  'load_address': 0x801e8000,
  'iso_lba': 0xa9ba,
  'load_sectors': 2,
  'tournament_class': None,
  'sections': [{'id': 'table-0',
                'text_table': 0x801e8660,
                'command_table': 0x801e8740,
                'voice_table': 0x801e8af8,
                'group_count': 28,
                'boundary_evidence': 'text root ends at first linked command stream; parallel roots and end '
                                     'commands validated'}],
  'evidence': ['src/ovl/MASTER/80133680.c::func_80133680', 'src/ovl/MASTER/80133680.c::cd_read_sectors'],
  'original_renderer_anomalies': [{
      'section': 'table-0', 'group': 10, 'line': 0, 'command': 0,
      'text_offset': 464, 'text_length': 53, 'relative_offset': 52, 'byte': 0x27,
      'kind': 'odd_terminal_unrecognized_byte',
      'handler_role': 'command_zero_calls_talk_render_with_original_payload',
      'source_behavior': 'two_byte_scanner_skips_terminal_nul_and_can_read_uncopied_stack_bytes',
      'runtime_outcome': 'not_proven', 'repair_applied': False,
      'evidence': ['src/main/8007C030.c:1191-1210,1627-1648,1809-1810']}],
  'source_pins': {'src/ovl/MASTER/80133680.c': 'c0d0608578564a43a8bbc1a25862d627cc4677310191bfa74b68dbbf86bc74cd'}},
 {'id': 'master-mokuten',
  'path': 'CDROM/MASTER/MOKUTEN.BIN',
  'sha256': '8788d25a30276541b601a26fd704e5c0b2e4d217585725dd6d954925a7de05d1',
  'size': 6144,
  'load_address': 0x801e8000,
  'iso_lba': 0xa9bc,
  'load_sectors': 3,
  'tournament_class': 4,
  'sections': [{'id': 'table-0',
                'text_table': 0x801e898c,
                'command_table': 0x801e8ab8,
                'voice_table': 0x801e93a0,
                'group_count': 31,
                'boundary_evidence': 'text root ends at first linked command stream; parallel roots and end '
                                     'commands validated'}],
  'evidence': ['src/ovl/MASTER/80132000.c::func_80132068', 'src/ovl/MASTER/80132140.c::cd_read_sectors'],
  'source_pins': {'src/ovl/MASTER/80132140.c': 'cba0eb244838f33d1d4275ddbb9b61e8cd909e0878da8d2583c09e88f2cd5cde',
                  'src/ovl/MASTER/80132000.c': '9ac659af38b272185ea6b161ecaa96070c43ee46f50dc493aa8dd1a15426d68d'}},
 {'id': 'master-nobel',
  'path': 'CDROM/MASTER/NOBEL.BIN',
  'sha256': '345ef9c934f133a082c9a59d73674b5bd668c9c6800a57628a7411bbb02be9d6',
  'size': 6144,
  'load_address': 0x801e8000,
  'iso_lba': 0xa9bf,
  'load_sectors': 3,
  'tournament_class': 2,
  'sections': [{'id': 'table-0',
                'text_table': 0x801e896c,
                'command_table': 0x801e8aa0,
                'voice_table': 0x801e938c,
                'group_count': 32,
                'boundary_evidence': 'text root ends at first linked command stream; parallel roots and end '
                                     'commands validated'}],
  'evidence': ['src/ovl/MASTER/80132000.c::func_8013209C', 'src/ovl/MASTER/80132140.c::cd_read_sectors'],
  'source_pins': {'src/ovl/MASTER/80132140.c': 'cba0eb244838f33d1d4275ddbb9b61e8cd909e0878da8d2583c09e88f2cd5cde',
                  'src/ovl/MASTER/80132000.c': '9ac659af38b272185ea6b161ecaa96070c43ee46f50dc493aa8dd1a15426d68d'}},
 {'id': 'master-sui-con',
  'path': 'CDROM/MASTER/SUI_CON.BIN',
  'sha256': 'baced8aa0d4db30c75c3edd58759746dc0cc420babaa15980816ae403da1b8a6',
  'size': 6144,
  'load_address': 0x801e8000,
  'iso_lba': 0xa9c2,
  'load_sectors': 3,
  'tournament_class': 5,
  'sections': [{'id': 'table-0',
                'text_table': 0x801e8abc,
                'command_table': 0x801e8c00,
                'voice_table': 0x801e9608,
                'group_count': 33,
                'boundary_evidence': 'text root ends at first linked command stream; parallel roots and end '
                                     'commands validated'}],
  'evidence': ['src/ovl/MASTER/80132000.c::func_801320D0', 'src/ovl/MASTER/80132140.c::cd_read_sectors'],
  'source_pins': {'src/ovl/MASTER/80132140.c': 'cba0eb244838f33d1d4275ddbb9b61e8cd909e0878da8d2583c09e88f2cd5cde',
                  'src/ovl/MASTER/80132000.c': '9ac659af38b272185ea6b161ecaa96070c43ee46f50dc493aa8dd1a15426d68d'}},
 {'id': 'master-tai-mas',
  'path': 'CDROM/MASTER/TAI_MAS.BIN',
  'sha256': '4aa6835ceb4b9376eaadf15456cf338037ffdda57210d9c053fca4fd2e4114b1',
  'size': 22528,
  'load_address': 0x801e8000,
  'iso_lba': 0xa9c5,
  'load_sectors': 11,
  'tournament_class': None,
  'sections': [{'id': 'table-0',
                'text_table': 0x801eaa54,
                'command_table': 0x801eae50,
                'voice_table': 0x801ed1c4,
                'group_count': 106,
                'boundary_evidence': 'text root ends at first linked command stream; parallel roots and end '
                                     'commands validated'}],
  'evidence': ['src/ovl/MASTER/80133680.c::func_801336B4',
               'src/ovl/MASTER/80133680.c::cd_read_sectors',
               'src/ovl/MASTER/80138FC0.c::func_80138FC0',
               'src/ovl/MASTER/80139000.c:161-177'],
  'source_pins': {'src/ovl/MASTER/80133680.c': 'c0d0608578564a43a8bbc1a25862d627cc4677310191bfa74b68dbbf86bc74cd',
                  'src/ovl/MASTER/80138FC0.c': '980f68375572a0b1f4bc359f7f69018fbbc3a94ba721172c38b2e17e39d30ccc',
                  'src/ovl/MASTER/80139000.c': '6f4ac101b124df04823452407fa86f6d0c7bdbff223dc888ec85f1d2f1889c26'}},
 {'id': 'master-ueki',
  'path': 'CDROM/MASTER/UEKI.BIN',
  'sha256': '36847c809b037dbc08ef4b0cd88f12540c481518e10e36e4aaa0158fd31f05cc',
  'size': 6144,
  'load_address': 0x801e8000,
  'iso_lba': 0xa9de,
  'load_sectors': 3,
  'tournament_class': 0,
  'sections': [{'id': 'table-0',
                'text_table': 0x801e8950,
                'command_table': 0x801e8a84,
                'voice_table': 0x801e9370,
                'group_count': 32,
                'boundary_evidence': 'text root ends at first linked command stream; parallel roots and end '
                                     'commands validated'}],
  'evidence': ['src/ovl/MASTER/80132000.c::func_80132104', 'src/ovl/MASTER/80132140.c::cd_read_sectors'],
  'source_pins': {'src/ovl/MASTER/80132140.c': 'cba0eb244838f33d1d4275ddbb9b61e8cd909e0878da8d2583c09e88f2cd5cde',
                  'src/ovl/MASTER/80132000.c': '9ac659af38b272185ea6b161ecaa96070c43ee46f50dc493aa8dd1a15426d68d'}}]


RESOURCE_PROFILES = [{'id': 'master-tear-actor-resource',
  'path': 'CDROM/MASTER/TEAR.BIN',
  'sha256': '04ebe53ee599942009ac0c8c72de7f322b653661c8f035a71cd45b3feb8672ce',
  'size': 28672,
  'iso_lba': 0xa9d0,
  'load_address': 0x80197000,
  'load_sectors': 14,
  'status': 'source_actor_resource_not_linked_talk_profile',
  'actor_resource_roots': [0x8019dcfc, 0x8019dd00, 0x8019ddc0],
  'actor_halfword_address': 0x8019ddf8,
  'source_pins': {'src/ovl/MASTER/80138EC0.c': '04a9d078e011df2d376efb41a1d04cead39ce3170028229cef7fb241e105c365',
                  'src/ovl/MASTER/80138F70.c': '267e520ff912bc17fbd9d067a641833d957ec464363401086be22c764c9160a1',
                  'src/ovl/MASTER/80133680.c': 'c0d0608578564a43a8bbc1a25862d627cc4677310191bfa74b68dbbf86bc74cd',
                  'src/ovl/MASTER/80139000.c': '6f4ac101b124df04823452407fa86f6d0c7bdbff223dc888ec85f1d2f1889c26'},
  'evidence': ['src/ovl/MASTER/80133680.c:1415-1422,1820-1836',
               'src/ovl/MASTER/80139000.c:677-684,1014-1030',
               'src/ovl/MASTER/80138EC0.c:3-7',
               'src/ovl/MASTER/80138F70.c:3-7'],
  'limits': ['actor callback consumers establish these resource pointers, not whole-file text absence',
             'unreferenced bytes are not classified as nontext by this descriptor']}]
