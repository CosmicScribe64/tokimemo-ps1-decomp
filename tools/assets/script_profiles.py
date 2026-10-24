"""Bounded metadata for script containers whose talk tables are confirmed.

``CDROM/BUNKA/BUN_PC.BIN`` is 10,240 bytes with SHA-256
``6af0844f3d3dce2aa304de509307d40f70d8610fec7acedd2ce85b4c395a90e9``.
Its table triad and 83 contiguous groups are established by
``src/ovl/BUNKAKEN/80132000.c:148-152`` and the parsed-container evidence in
``wiki/spec/m-dates-b.md``. It is read from the extracted ``disc/files`` root.

``CDROM/SERIFU/P_BIRTH.BIN`` is 6,144 bytes with SHA-256
``76f6a93641fd9fd83aafab9be505906b6fa57905665013236a8484028d9a844e``.
Its load address and thirteen per-girl table triads come from
``src/ovl/OMIMAI/80132AD0.c:3-42,60-64,84-140``; the load is independently
confirmed at ``src/ovl/BUNKASAI/80148ED0.c:235`` and the extent is identified
as ``CDROM/SERIFU/P_BIRTH.BIN`` by the ISO inventory at LBA 0xAF43. The
contiguous group counts, in section order, are 3, 3, 1, 3, 3, 3, 3, 3, 3, 3,
3, 1, 3, validated from all three pointer tables in Docker.
"""

PROFILES = [
    {
        "id": "bunka-bun-pc",
        "path": "CDROM/BUNKA/BUN_PC.BIN",
        "sha256": "6af0844f3d3dce2aa304de509307d40f70d8610fec7acedd2ce85b4c395a90e9",
        "load_address": 0x801E8000,
        "sections": [
            {
                "id": "bun-pc",
                "text_table": 0x801E92FC,
                "command_table": 0x801E9598,
                "voice_table": 0x801EA298,
                "group_count": 83,
            },
        ],
        "evidence": [
            "src/ovl/BUNKAKEN/80132000.c",
            "wiki/spec/m-dates-b.md",
        ],
    },
    {
        "id": "omimai-p-birth",
        "path": "CDROM/SERIFU/P_BIRTH.BIN",
        "sha256": "76f6a93641fd9fd83aafab9be505906b6fa57905665013236a8484028d9a844e",
        "load_address": 0x801B0000,
        "sections": [
            {"id": "girl-0", "text_table": 0x801B00DC, "command_table": 0x801B00F4, "voice_table": 0x801B01A8, "group_count": 3},
            {"id": "girl-1", "text_table": 0x801B02AC, "command_table": 0x801B02C8, "voice_table": 0x801B0398, "group_count": 3},
            {"id": "girl-2", "text_table": 0x801B03C8, "command_table": 0x801B03D0, "voice_table": 0x801B03F0, "group_count": 1},
            {"id": "girl-3", "text_table": 0x801B04C4, "command_table": 0x801B04DC, "voice_table": 0x801B0590, "group_count": 3},
            {"id": "girl-4", "text_table": 0x801B0668, "command_table": 0x801B0680, "voice_table": 0x801B0734, "group_count": 3},
            {"id": "girl-5", "text_table": 0x801B0808, "command_table": 0x801B0820, "voice_table": 0x801B08D4, "group_count": 3},
            {"id": "girl-6", "text_table": 0x801B09DC, "command_table": 0x801B09F8, "voice_table": 0x801B0AE4, "group_count": 3},
            {"id": "girl-7", "text_table": 0x801B0BE8, "command_table": 0x801B0C00, "voice_table": 0x801B0CB4, "group_count": 3},
            {"id": "girl-8", "text_table": 0x801B0D8C, "command_table": 0x801B0DA4, "voice_table": 0x801B0E58, "group_count": 3},
            {"id": "girl-9", "text_table": 0x801B0F5C, "command_table": 0x801B0F78, "voice_table": 0x801B1064, "group_count": 3},
            {"id": "girl-10", "text_table": 0x801B1164, "command_table": 0x801B1180, "voice_table": 0x801B126C, "group_count": 3},
            {"id": "girl-11", "text_table": 0x801B12D8, "command_table": 0x801B12E0, "voice_table": 0x801B1338, "group_count": 1},
            {"id": "girl-12", "text_table": 0x801B1454, "command_table": 0x801B1470, "voice_table": 0x801B155C, "group_count": 3},
        ],
        "evidence": [
            "src/ovl/OMIMAI/80132AD0.c",
            "src/ovl/BUNKASAI/80148ED0.c",
            "wiki/spec/o-bunkasai.md",
        ],
    },
]

# Source metadata is split by independent analysis lane; consumers use one API.
from .script_profiles_bunka import PROFILES as BUNKA_PROFILES
from .script_profiles_dates import PROFILES as DATE_PROFILES
from .script_profiles_other import PROFILES as OTHER_PROFILES, NON_TEXT_PROFILES

PROFILES += BUNKA_PROFILES + DATE_PROFILES + OTHER_PROFILES
