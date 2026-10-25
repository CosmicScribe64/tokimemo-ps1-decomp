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
  'evidence': ['src/ovl/club_master/club_master_resource_setup.c::master_set_award_text_assets_1', 'src/ovl/club_master/club_master_mode_stage_dispatch.c::cd_read_sectors'],
  'source_pins': {'src/ovl/club_master/club_master_mode_stage_dispatch.c': 'd0d97acea008846e1335f77f794721dae12c5f1c47a5e43ec7661731278f897f',
                  'src/ovl/club_master/club_master_resource_setup.c': '0d80fad5565aa21bb0ea9275f720859da836345499fe23d4ef7bd1fe5f2d7689'}},
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
  'evidence': ['src/ovl/club_master/club_master_resource_setup.c::master_set_award_text_assets_3', 'src/ovl/club_master/club_master_mode_stage_dispatch.c::cd_read_sectors'],
  'source_pins': {'src/ovl/club_master/club_master_mode_stage_dispatch.c': 'd0d97acea008846e1335f77f794721dae12c5f1c47a5e43ec7661731278f897f',
                  'src/ovl/club_master/club_master_resource_setup.c': '0d80fad5565aa21bb0ea9275f720859da836345499fe23d4ef7bd1fe5f2d7689'}},
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
  'evidence': ['src/ovl/club_master/club_tournament_scene.c::master_set_shiori_tournament_dialogue', 'src/ovl/club_master/club_tournament_scene.c::cd_read_sectors'],
  'original_renderer_anomalies': [{
      'section': 'table-0', 'group': 10, 'line': 0, 'command': 0,
      'text_offset': 464, 'text_length': 53, 'relative_offset': 52, 'byte': 0x27,
      'kind': 'odd_terminal_unrecognized_byte',
      'handler_role': 'command_zero_calls_talk_render_with_original_payload',
      'source_behavior': 'two_byte_scanner_skips_terminal_nul_and_can_read_uncopied_stack_bytes',
      'runtime_outcome': 'not_proven', 'repair_applied': False,
      'evidence': ['src/main/scene_library.c (historical pre-rename source lines 1191-1210,1627-1648,1809-1810; use pinned current source)']}],
  'source_pins': {'src/ovl/club_master/club_tournament_scene.c': 'e15133f761ecfa1b61977dcda8c902f6ce94dbbdbe54ed6da895f22953a77fe3'}},
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
  'evidence': ['src/ovl/club_master/club_master_resource_setup.c::master_set_award_text_assets_4', 'src/ovl/club_master/club_master_mode_stage_dispatch.c::cd_read_sectors'],
  'source_pins': {'src/ovl/club_master/club_master_mode_stage_dispatch.c': 'd0d97acea008846e1335f77f794721dae12c5f1c47a5e43ec7661731278f897f',
                  'src/ovl/club_master/club_master_resource_setup.c': '0d80fad5565aa21bb0ea9275f720859da836345499fe23d4ef7bd1fe5f2d7689'}},
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
  'evidence': ['src/ovl/club_master/club_master_resource_setup.c::master_set_award_text_assets_2', 'src/ovl/club_master/club_master_mode_stage_dispatch.c::cd_read_sectors'],
  'source_pins': {'src/ovl/club_master/club_master_mode_stage_dispatch.c': 'd0d97acea008846e1335f77f794721dae12c5f1c47a5e43ec7661731278f897f',
                  'src/ovl/club_master/club_master_resource_setup.c': '0d80fad5565aa21bb0ea9275f720859da836345499fe23d4ef7bd1fe5f2d7689'}},
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
  'evidence': ['src/ovl/club_master/club_master_resource_setup.c::master_set_award_text_assets_5', 'src/ovl/club_master/club_master_mode_stage_dispatch.c::cd_read_sectors'],
  'source_pins': {'src/ovl/club_master/club_master_mode_stage_dispatch.c': 'd0d97acea008846e1335f77f794721dae12c5f1c47a5e43ec7661731278f897f',
                  'src/ovl/club_master/club_master_resource_setup.c': '0d80fad5565aa21bb0ea9275f720859da836345499fe23d4ef7bd1fe5f2d7689'}},
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
  'evidence': ['src/ovl/club_master/club_tournament_scene.c::master_set_tournament_companion_dialogue',
               'src/ovl/club_master/club_tournament_scene.c::cd_read_sectors',
               'src/ovl/club_master/mastery_talk_tables.c::master_set_companion_actor_assets_d',
               'src/ovl/club_master/club_master_scene_and_tables.c (historical pre-rename source lines 161-177; use pinned current source)'],
  'source_pins': {'src/ovl/club_master/club_tournament_scene.c': 'e15133f761ecfa1b61977dcda8c902f6ce94dbbdbe54ed6da895f22953a77fe3',
                  'src/ovl/club_master/mastery_talk_tables.c': '4076baa8bfb48a2acda4534c3aced40d517e5d48181f38ab305430e4edb381c7',
                  'src/ovl/club_master/club_master_scene_and_tables.c': '74af1cebcdc4b51e9e4f1ed97d91decedb21fef56cd63ca27f1cf74c5ecdd773'}},
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
  'evidence': ['src/ovl/club_master/club_master_resource_setup.c::master_set_award_text_assets_0', 'src/ovl/club_master/club_master_mode_stage_dispatch.c::cd_read_sectors'],
  'source_pins': {'src/ovl/club_master/club_master_mode_stage_dispatch.c': 'd0d97acea008846e1335f77f794721dae12c5f1c47a5e43ec7661731278f897f',
                  'src/ovl/club_master/club_master_resource_setup.c': '0d80fad5565aa21bb0ea9275f720859da836345499fe23d4ef7bd1fe5f2d7689'}}]


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
  'source_pins': {'src/ovl/club_master/tournament_actor_resources.c': '993fe5ff8a31834cb804272af17327340f777a76caa0f91b83094b5e632c167d',
                  'src/ovl/club_master/mastery_scene_actor_resources.c': '2473d7a18d8fb427bf5eb1a77b5c0803832c99c72f2432be8f8640b7f548801d',
                  'src/ovl/club_master/club_tournament_scene.c': 'e15133f761ecfa1b61977dcda8c902f6ce94dbbdbe54ed6da895f22953a77fe3',
                  'src/ovl/club_master/club_master_scene_and_tables.c': '74af1cebcdc4b51e9e4f1ed97d91decedb21fef56cd63ca27f1cf74c5ecdd773'},
  'evidence': ['src/ovl/club_master/club_tournament_scene.c (historical pre-rename source lines 1415-1422,1820-1836; use pinned current source)',
               'src/ovl/club_master/club_master_scene_and_tables.c (historical pre-rename source lines 677-684,1014-1030; use pinned current source)',
               'src/ovl/club_master/tournament_actor_resources.c (historical pre-rename source lines 3-7; use pinned current source)',
               'src/ovl/club_master/mastery_scene_actor_resources.c (historical pre-rename source lines 3-7; use pinned current source)'],
  'limits': ['actor callback consumers establish these resource pointers, not whole-file text absence',
             'unreferenced bytes are not classified as nontext by this descriptor']}]
