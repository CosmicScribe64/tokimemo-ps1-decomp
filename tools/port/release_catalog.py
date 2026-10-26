#!/usr/bin/env python3
"""Release catalog and multi-release PS1 disc recognition for Tokimeki Memorial (T-29017).

Provides robust identification, validation, and content compatibility evaluation
for all 5 known Japanese PS1 releases of Tokimeki Memorial - Forever with You:
  - best_slpm_86053     (PlayStation the Best, SLPM-86053, 1997-12-25) [Target]
  - rev4_slps_00065_v143 (Rev 4 Ver 1.43, PSX.EXE, 1996-03-31)
  - rev2_slps_00065_v125 (Rev 2 Ver 1.25, PSX.EXE, 1995-11-17)
  - rev1_slps_00065_v110 (Rev 1 Ver 1.10, PSX.EXE, 1995-08-31)
  - shokai_slps_00064_v110 (Shokai Genteiban Ver 1.10, PSX.EXE, 1995-08-31)

Separates container recognition (directory, CUE, raw image, ISO, ZIP) from release
profile verification, and reports exact per-overlay compatibility against PlayStation
the Best without relaxing canonical hash checks.
"""

from __future__ import annotations

import dataclasses
import hashlib
import io
import json
import os
import re
import struct
import sys
import zipfile
from typing import Any, Dict, List, Optional, Tuple

SECTOR_RAW = 2352
SECTOR_ISO = 2048
MAX_DIR_DEPTH = 8

CANONICAL_TARGET_ID = "best_slpm_86053"
OVERLAY_NAMES = (
    "BUNKAKEN.EXN", "BUNKASAI.EXN", "BUNKA_SD.EXN", "DATE.EXN", "DATE2.EXN",
    "ENDING.EXN", "EN_NICHI.EXN", "ETC.EXN", "EVENT.EXN", "GEKO.EXN",
    "GYOZI.EXN", "KANGEI.EXN", "MASTER.EXN", "NAME_ENT.EXN", "O.BIN",
    "OLH.EXN", "OMIMAI.EXN", "OPTION.EXN", "RENSYU.EXN", "RPG_BAT.EXN",
    "SHOUGATU.EXN", "SHUGAKU.EXN", "TACO.EXN", "TAIIKU.EXN", "TEL.EXN",
    "TT.EXN", "VALEN.EXN"
)

# Overlays byte-identical across all 5 cataloged releases
COMMON_BYTE_IDENTICAL_OVERLAYS = ("EVENT.EXN", "GYOZI.EXN", "O.BIN")


@dataclasses.dataclass(frozen=True)
class ReleaseProfile:
    id: str
    short_id: str
    label: str
    boot_file: str
    exe_size: int
    exe_sha1: str
    entry_addr: int
    gp_addr: int
    text_addr: int
    text_size: int
    rodata_addr: int
    rodata_size: int
    volume_id: str
    volume_set_id: str
    pvd_created: str
    track1_size: int
    track1_sha1: str
    system_cnf_boot: str
    overlays: Dict[str, str]
    supported_for_content_extraction: bool
    changed_exe_functions_vs_best: int
    diagnostic_code: str


# Built-in definitions matching config/versions.txt and wiki/versions.md
DEFAULT_PROFILES: Dict[str, ReleaseProfile] = {
    "best_slpm_86053": ReleaseProfile(
        id="best_slpm_86053",
        short_id="best",
        label="PlayStation the Best (SLPM-86053, 1997-12-25)",
        boot_file="SLPM_86.053",
        exe_size=667648,
        exe_sha1="e823bd844a8f8fa4d05483b59c66bc54b8393b26",
        entry_addr=0x800420D0,
        gp_addr=0x800EB670,
        text_addr=0x80041000,
        text_size=0xA2800,
        rodata_addr=0x800AF340,
        rodata_size=0x3EE0,
        volume_id="VX009J2",
        volume_set_id="TOKIMEKIMEMORIALBEST",
        pvd_created="19971225",
        track1_size=702072000,
        track1_sha1="00b595095e26108d0e07d76492a11aaf3f71ccf1",
        system_cnf_boot="cdrom:SLPM_86.053;1",
        overlays={
            "BUNKAKEN.EXN": "d423cb6d09b4b7e96e894168c0db4c7d15745b98",
            "BUNKASAI.EXN": "0307afff16d01bd32055ee2699bdf9a075d846ad",
            "BUNKA_SD.EXN": "173ac942ad4efbbbb176f8522fc7d273540169b0",
            "DATE.EXN": "b5fb83738364a317a590735578f5878077fcda3d",
            "DATE2.EXN": "0db71056d49f1bf0975a592ec7bbb4fd7894bd56",
            "ENDING.EXN": "6788c26fe3b93016b245e2f5837de12428603ddf",
            "EN_NICHI.EXN": "d1642c45b607242936873b52e81ac62c451b3e29",
            "ETC.EXN": "681c93b6e57c885f86e074c885a8bf6c3cca2319",
            "EVENT.EXN": "ff7184d7e92b7aff372450ac3b8c2bdd42e55a5e",
            "GEKO.EXN": "fa37853a9bd8df628c6313b0a4af8ec7568482cd",
            "GYOZI.EXN": "43f92523116e7b6cd772d4397712a52eeda89703",
            "KANGEI.EXN": "b94bbaceb7083c4b56d3b2ff56313cbcfbe6ac83",
            "MASTER.EXN": "7303c196a7a2fd8423da1e66e5a49511e9805bbc",
            "NAME_ENT.EXN": "e9a9a0f059dc0029e292363ecf09cffb4068dad9",
            "O.BIN": "d3814aac226cdc256e6cf1e046534d576d412a6c",
            "OLH.EXN": "3c6fd6ec737732f62c4fda4a655b8a5b3f48122b",
            "OMIMAI.EXN": "465f4abd096d90e78960fd0f99e983c122efe3c1",
            "OPTION.EXN": "3fd443d1e32404e5febd57d0586ef1c70ce8a811",
            "RENSYU.EXN": "67efd6e1b2ee454598096a74bd13dfeb0c15d27e",
            "RPG_BAT.EXN": "142d13e317aa7ae6def6a300085c38f645c19db6",
            "SHOUGATU.EXN": "d956b1df6fbf9d1f8261d7e1f98ea301f7eb66a9",
            "SHUGAKU.EXN": "e9c64e952a1ef5dfea5e9b3e6a659aa6ccb8eb8c",
            "TACO.EXN": "1419e93cbad6c0df790577701181be04e0747969",
            "TAIIKU.EXN": "306d1013b9be465819525d1fb4dc0e86b422a846",
            "TEL.EXN": "d37c3ae6beb7ab361f39366b8274cde0f7c96b5c",
            "TT.EXN": "89a81735c1e9d79d2562f4c6ce628a732e017cf2",
            "VALEN.EXN": "356aed2481987108f908d7013cc0392cea9dcc8c",
        },
        supported_for_content_extraction=True,
        changed_exe_functions_vs_best=0,
        diagnostic_code="OK_PLAYSTATION_THE_BEST",
    ),
    "rev4_slps_00065_v143": ReleaseProfile(
        id="rev4_slps_00065_v143",
        short_id="rev4",
        label="Rev 4 (SLPS-00065VER143, 1996-03-31)",
        boot_file="PSX.EXE",
        exe_size=667648,
        exe_sha1="60305396bde6f4dfb7187a328c320638aaa7df72",
        entry_addr=0x800420D0,
        gp_addr=0x800EB540,
        text_addr=0x80041000,
        text_size=0xA2800,
        rodata_addr=0x800AF1F0,
        rodata_size=0x3F00,
        volume_id="TOKIMEKIMEMORIAL",
        volume_set_id="SLPS00065VER143",
        pvd_created="19960331",
        track1_size=702104928,
        track1_sha1="0ec8e3b093291ac7ce3af2bb62beda5228f09435",
        system_cnf_boot="cdrom:PSX.EXE;1",
        overlays={
            "BUNKAKEN.EXN": "4c77071cf11040c4cb33f3353d9a225ec9b3e176",
            "BUNKASAI.EXN": "4b318992f46efc73da9406dc1768f825d13e64fc",
            "BUNKA_SD.EXN": "f397163aa23825d804406822e65b4a3584d7e887",
            "DATE.EXN": "b28842695a37b9c8e928a707ff31626428c8a6c2",
            "DATE2.EXN": "7a1023e8ced74c70a7d79648e34a68aa4e6a2ff7",
            "ENDING.EXN": "03d8d6ebe32d7dbfe0cf5da768648223f164f883",
            "EN_NICHI.EXN": "695c62988f8e7f4a2912c5f96148620f7e70a597",
            "ETC.EXN": "608cfecc4840824aca6fb85422f1cf522833e75f",
            "EVENT.EXN": "ff7184d7e92b7aff372450ac3b8c2bdd42e55a5e",
            "GEKO.EXN": "b3e80ba0821918158bbf9fc5e519cbd74248aa1c",
            "GYOZI.EXN": "43f92523116e7b6cd772d4397712a52eeda89703",
            "KANGEI.EXN": "4ad29ef6479b20f3376523998a13990d28860373",
            "MASTER.EXN": "e3355958f282c5781962d0373aadbeb69663b22f",
            "NAME_ENT.EXN": "c1ff1f0c54ee4aa0c3a5edd77436fa3ac6c6d2bf",
            "O.BIN": "d3814aac226cdc256e6cf1e046534d576d412a6c",
            "OLH.EXN": "31f78c9cbe6c44187e3906c66f97e64afd07d7fa",
            "OMIMAI.EXN": "ca6b083e3a88e9beeaf5a519eb9251d2c3ce9ce0",
            "OPTION.EXN": "01ed973104253038cf6855675ef65add00dc247c",
            "RENSYU.EXN": "c1886063e92f290bf6161f05b00259942573ae60",
            "RPG_BAT.EXN": "dee9b99e16366d90642655c991bc6f6528c48590",
            "SHOUGATU.EXN": "8184c901efe21ac016a601fc13442ccfae7a2b89",
            "SHUGAKU.EXN": "c7b99859cd8fb666a2ace1ee21b8a984270784ee",
            "TACO.EXN": "1e27c170e664674f094e7a637c99ec220a44487b",
            "TAIIKU.EXN": "24814174dc40cddd6be8b4ec71f6c81e8326f723",
            "TEL.EXN": "39d1762e099cc4af688825d784d1deb648613ae3",
            "TT.EXN": "01cd08db308cc057d3799d27c625f0872b5f35db",
            "VALEN.EXN": "fe76adefce3bafa05bf635fc1b778add2931eb88",
        },
        supported_for_content_extraction=False,
        changed_exe_functions_vs_best=46,
        diagnostic_code="ERR_REV4_UNSUPPORTED",
    ),
    "rev2_slps_00065_v125": ReleaseProfile(
        id="rev2_slps_00065_v125",
        short_id="rev2",
        label="Rev 2 (SLPS-00065VER125, 1995-11-17)",
        boot_file="PSX.EXE",
        exe_size=690176,
        exe_sha1="6c3eafe5df4a5dbf5dd596971752efaf7669150b",
        entry_addr=0x800420D0,
        gp_addr=0x800F0A60,
        text_addr=0x80041000,
        text_size=0xA8000,
        rodata_addr=0x800B57C0,
        rodata_size=0x3C30,
        volume_id="TOKIMEKIMEMORIAL",
        volume_set_id="SLPS00065VER125",
        pvd_created="19951117",
        track1_size=702104928,
        track1_sha1="f877c53d420d571694e38d4af4e9f4ea3e5f8b7e",
        system_cnf_boot="cdrom:PSX.EXE;1",
        overlays={
            "BUNKAKEN.EXN": "096e7dc1ec5635bd2fd8db8743412c1e64d54f8d",
            "BUNKASAI.EXN": "a7ae8c6d0a06f6d585225dd4c8704603793a83f6",
            "BUNKA_SD.EXN": "8059e596549c431d8b11fe27fe9569b53b18ab21",
            "DATE.EXN": "685b8683482b9c173e32c3c290311aaa30cf6152",
            "DATE2.EXN": "2d9e12021f487f23f38724beb8bff9073453b77d",
            "ENDING.EXN": "ae25332d91b944ec8b93cf9d2027a91311553923",
            "EN_NICHI.EXN": "6d6fc69a2a74fab8fc475399dddfd0213e867ec5",
            "ETC.EXN": "bc12103590960fed26c526dad2e89df783b6b418",
            "EVENT.EXN": "ff7184d7e92b7aff372450ac3b8c2bdd42e55a5e",
            "GEKO.EXN": "d632a63e25e27909f89c400205ed615e6a34f316",
            "GYOZI.EXN": "43f92523116e7b6cd772d4397712a52eeda89703",
            "KANGEI.EXN": "d8dc95ba7eb049446b3ea24b3b05c73b485e3d1e",
            "MASTER.EXN": "b7161e92b205c38e3985d4f99479a0f65ce528ad",
            "NAME_ENT.EXN": "7363ec4c2dfddcd9889698d53b105fc2e51cb2bc",
            "O.BIN": "d3814aac226cdc256e6cf1e046534d576d412a6c",
            "OLH.EXN": "201bdd4d28a35af2bf1ddfa6a43a8fc754aea92e",
            "OMIMAI.EXN": "cd704acef30bcce74e87fed94bdbcf305eb4f86f",
            "OPTION.EXN": "b777ca5c2bc9aba477c76cdff3f74b53885eaa24",
            "RENSYU.EXN": "d0a76d27a714fdbb79ffaf7cff262dc5c3275c51",
            "RPG_BAT.EXN": "0922d5ad21e0fc5a3ed6c344850198fabfcb5751",
            "SHOUGATU.EXN": "eb0bfab7a9b3756ae192505aaac8ccea19e6be3f",
            "SHUGAKU.EXN": "00039751a85b2fae45cc616feb078c8687276740",
            "TACO.EXN": "062fc4a4f12b802be95e114bc0af95644e6af82f",
            "TAIIKU.EXN": "f197252ba210676188b0c25bfb4fe94c677b209f",
            "TEL.EXN": "07f7c2e20bd72bf972247a8c4e269037da5ec295",
            "TT.EXN": "f84871bd7d71828c6074d97bc9ffa009946f6afb",
            "VALEN.EXN": "34d25cdad5ab06d36655731c5cf7ec69952d8890",
        },
        supported_for_content_extraction=False,
        changed_exe_functions_vs_best=475,
        diagnostic_code="ERR_REV2_UNSUPPORTED",
    ),
    "rev1_slps_00065_v110": ReleaseProfile(
        id="rev1_slps_00065_v110",
        short_id="rev1",
        label="Rev 1 (SLPS-00065VER110, 1995-08-31)",
        boot_file="PSX.EXE",
        exe_size=690176,
        exe_sha1="2d8d474816e4bf937b09dc3b017f550f7ce4e14b",
        entry_addr=0x800422A0,
        gp_addr=0x800F0D90,
        text_addr=0x80041000,
        text_size=0xA8000,
        rodata_addr=0x800B5A60,
        rodata_size=0x3CB0,
        volume_id="TOKIMEKIMEMORIAL",
        volume_set_id="SLPS00065VER110",
        pvd_created="19950831",
        track1_size=702104928,
        track1_sha1="7b2f72d1319db27d0700070aa0bffe886b5ef7a4",
        system_cnf_boot="cdrom:PSX.EXE;1",
        overlays={
            "BUNKAKEN.EXN": "7f8678a47b95798dcf9620175eda58a11d20d9da",
            "BUNKASAI.EXN": "8c317ed8683b81d2dbb77451773ebc5768aaa893",
            "BUNKA_SD.EXN": "dfa7510b8b682f0302020bdb3cfeb1ddb580df9f",
            "DATE.EXN": "4bc06fdf36925c3692c2aacadc44dd532ea60c2e",
            "DATE2.EXN": "fdd2ff364aba56def62aab545de4eb033b723b55",
            "ENDING.EXN": "10e07e33a5a6be2e312f9c9ab98a1330045c597e",
            "EN_NICHI.EXN": "3bc66b9e9307ed77646e9cac4f39d62153a546f4",
            "ETC.EXN": "efb00ffd5401961e6609fbf6825f8e711181674c",
            "EVENT.EXN": "ff7184d7e92b7aff372450ac3b8c2bdd42e55a5e",
            "GEKO.EXN": "099fbf4edb575a36b050e1f7ec8e7787122c734c",
            "GYOZI.EXN": "43f92523116e7b6cd772d4397712a52eeda89703",
            "KANGEI.EXN": "4b6ad2871d63227b720b160490cb0606f24cd6cc",
            "MASTER.EXN": "1e24be3bc092bd92d361951ad2bfcaf26913ec64",
            "NAME_ENT.EXN": "a99ea63d83f0ad87056d80cd944ea5331c5a68a8",
            "O.BIN": "d3814aac226cdc256e6cf1e046534d576d412a6c",
            "OLH.EXN": "e7f9354630836f4a7373a90b14bb5bedb087b7ac",
            "OMIMAI.EXN": "50ea25a5d994ee1b78de842b75487ce6c7c1cd3e",
            "OPTION.EXN": "c7d49aa4edd65b1d0784330266b263a72f982c7e",
            "RENSYU.EXN": "5a5daf1895ad7757f10876b4e9725882cf4e8b09",
            "RPG_BAT.EXN": "b06293eb9ce7be26bef06fc0d6d76ba067394fdf",
            "SHOUGATU.EXN": "278e922649fbd76a463c51e5a8dc6432d3bcdc25",
            "SHUGAKU.EXN": "2fc7ac1901cec1064532dbdd4e7fd52d8a6a53fd",
            "TACO.EXN": "6d8d6f806c5f026dc69ef6fa9ee3dabc3d029b8e",
            "TAIIKU.EXN": "a1aee09fbbd9db3e939caa4f6a9b91d419b03f0e",
            "TEL.EXN": "49cf0c40f7c6e7d4918ed0c440a67ad3d685c2e1",
            "TT.EXN": "e02ad145626b610b38f78178af1f8b55ee6808e6",
            "VALEN.EXN": "28cf2a154afd8553137aa3df59efa8bbb755c4a5",
        },
        supported_for_content_extraction=False,
        changed_exe_functions_vs_best=521,
        diagnostic_code="ERR_REV1_UNSUPPORTED",
    ),
    "shokai_slps_00064_v110": ReleaseProfile(
        id="shokai_slps_00064_v110",
        short_id="shokai",
        label="Shokai Genteiban (SLPS-00064VER110, 1995-08-31)",
        boot_file="PSX.EXE",
        exe_size=690176,
        exe_sha1="2d8d474816e4bf937b09dc3b017f550f7ce4e14b",
        entry_addr=0x800422A0,
        gp_addr=0x800F0D90,
        text_addr=0x80041000,
        text_size=0xA8000,
        rodata_addr=0x800B5A60,
        rodata_size=0x3CB0,
        volume_id="TOKIMEKIMEMORIAL",
        volume_set_id="SLPS00064VER110",
        pvd_created="19950831",
        track1_size=702104928,
        track1_sha1="899cc5c27bf073470de676c1208c5acfbcb1da3e",
        system_cnf_boot="cdrom:PSX.EXE;1",
        overlays={
            "BUNKAKEN.EXN": "7f8678a47b95798dcf9620175eda58a11d20d9da",
            "BUNKASAI.EXN": "8c317ed8683b81d2dbb77451773ebc5768aaa893",
            "BUNKA_SD.EXN": "dfa7510b8b682f0302020bdb3cfeb1ddb580df9f",
            "DATE.EXN": "4bc06fdf36925c3692c2aacadc44dd532ea60c2e",
            "DATE2.EXN": "fdd2ff364aba56def62aab545de4eb033b723b55",
            "ENDING.EXN": "10e07e33a5a6be2e312f9c9ab98a1330045c597e",
            "EN_NICHI.EXN": "3bc66b9e9307ed77646e9cac4f39d62153a546f4",
            "ETC.EXN": "efb00ffd5401961e6609fbf6825f8e711181674c",
            "EVENT.EXN": "ff7184d7e92b7aff372450ac3b8c2bdd42e55a5e",
            "GEKO.EXN": "099fbf4edb575a36b050e1f7ec8e7787122c734c",
            "GYOZI.EXN": "43f92523116e7b6cd772d4397712a52eeda89703",
            "KANGEI.EXN": "4b6ad2871d63227b720b160490cb0606f24cd6cc",
            "MASTER.EXN": "1e24be3bc092bd92d361951ad2bfcaf26913ec64",
            "NAME_ENT.EXN": "a99ea63d83f0ad87056d80cd944ea5331c5a68a8",
            "O.BIN": "d3814aac226cdc256e6cf1e046534d576d412a6c",
            "OLH.EXN": "e7f9354630836f4a7373a90b14bb5bedb087b7ac",
            "OMIMAI.EXN": "50ea25a5d994ee1b78de842b75487ce6c7c1cd3e",
            "OPTION.EXN": "c7d49aa4edd65b1d0784330266b263a72f982c7e",
            "RENSYU.EXN": "5a5daf1895ad7757f10876b4e9725882cf4e8b09",
            "RPG_BAT.EXN": "b06293eb9ce7be26bef06fc0d6d76ba067394fdf",
            "SHOUGATU.EXN": "278e922649fbd76a463c51e5a8dc6432d3bcdc25",
            "SHUGAKU.EXN": "2fc7ac1901cec1064532dbdd4e7fd52d8a6a53fd",
            "TACO.EXN": "6d8d6f806c5f026dc69ef6fa9ee3dabc3d029b8e",
            "TAIIKU.EXN": "a1aee09fbbd9db3e939caa4f6a9b91d419b03f0e",
            "TEL.EXN": "49cf0c40f7c6e7d4918ed0c440a67ad3d685c2e1",
            "TT.EXN": "e02ad145626b610b38f78178af1f8b55ee6808e6",
            "VALEN.EXN": "28cf2a154afd8553137aa3df59efa8bbb755c4a5",
        },
        supported_for_content_extraction=False,
        changed_exe_functions_vs_best=521,
        diagnostic_code="ERR_SHOKAI_UNSUPPORTED",
    ),
}


class DiscReader:
    """Seekable reader over cooked ISO (2048) or raw MODE2/2352 disc images."""

    def __init__(self, f: Any, is_mode2: bool) -> None:
        self.f = f
        self.is_mode2 = is_mode2
        self.sector_size = SECTOR_RAW if is_mode2 else SECTOR_ISO

    def read_sector(self, lba: int) -> Tuple[bytes, bool]:
        """Return (payload, is_form2) for one sector."""
        if not self.is_mode2:
            self.f.seek(lba * SECTOR_ISO)
            return self.f.read(SECTOR_ISO), False
        self.f.seek(lba * SECTOR_RAW + 16)
        sub = self.f.read(8)
        if len(sub) < 8:
            return b"", False
        if sub[:4] != sub[4:]:  # no subheader (directory sector quirk)
            self.f.seek(lba * SECTOR_RAW + 16)
            return self.f.read(2048), False
        if sub[2] & 0x20:  # Form 2
            return self.f.read(2324), True
        return self.f.read(2048), False

    def read_dir(self, lba: int, size: int) -> List[Tuple[str, int, int, int]]:
        data = b"".join(self.read_sector(lba + i)[0] for i in range((size + 2047) // 2048))
        pos, out = 0, []
        while pos < size:
            if pos >= len(data):
                break
            ln = data[pos]
            if ln == 0:
                pos = (pos // 2048 + 1) * 2048
                continue
            rec = data[pos : pos + ln]
            if len(rec) < 33:
                pos += ln
                continue
            r_lba = struct.unpack("<I", rec[2:6])[0]
            r_size = struct.unpack("<I", rec[10:14])[0]
            flags = rec[25]
            name = rec[33 : 33 + rec[32]].decode("latin1", errors="replace").split(";")[0]
            out.append((name, r_lba, r_size, flags))
            pos += ln
        return out

    def read_file(self, lba: int, size: int) -> bytes:
        out = bytearray()
        i = 0
        while len(out) < size:
            payload, form2 = self.read_sector(lba + i)
            if form2 or not payload:
                break
            out += payload
            i += 1
        return bytes(out[:size])

    def index_tree(self, root_lba: int, root_size: int) -> Dict[str, Tuple[int, int]]:
        file_map: Dict[str, Tuple[int, int]] = {}

        def walk(cur_lba: int, cur_size: int, prefix: str, depth: int) -> None:
            if depth > MAX_DIR_DEPTH:
                return
            for name, r_lba, r_size, flags in self.read_dir(cur_lba, cur_size):
                if name in ("", "\x00", "\x01"):
                    continue
                p = prefix + name.upper()
                if flags & 2:
                    walk(r_lba, r_size, p + "/", depth + 1)
                else:
                    file_map[p] = (r_lba, r_size)

        walk(root_lba, root_size, "", 0)
        return file_map


class ReleaseCatalog:
    """Catalog of known releases and multi-release verifier."""

    def __init__(self, profiles: Optional[Dict[str, ReleaseProfile]] = None) -> None:
        self.profiles = dict(profiles or DEFAULT_PROFILES)
        self._sha1_to_profile_ids: Dict[str, List[str]] = {}
        for pid, prof in self.profiles.items():
            self._sha1_to_profile_ids.setdefault(prof.exe_sha1, []).append(pid)

    @classmethod
    def load_from_versions_txt(cls, path: str) -> ReleaseCatalog:
        """Parse config/versions.txt and construct a populated ReleaseCatalog."""
        if not os.path.isfile(path):
            raise FileNotFoundError(f"versions database not found: {path}")

        vers_meta: Dict[str, Dict[str, Any]] = {}
        exes_meta: Dict[str, Dict[str, Any]] = {}
        ovls_meta: Dict[str, Dict[str, str]] = {}

        with open(path, "r", encoding="utf-8") as f:
            for line in f:
                line = line.strip()
                if not line or line.startswith("#"):
                    continue
                p = line.split()
                if p[0] == "version":
                    # version <id> <label> <volume_id> <pvd_created> <boot_file> <track1_size> <track1_sha1> <volume_set_id>
                    vers_meta[p[1]] = {
                        "label": p[2].replace("_", " "),
                        "volume_id": p[3],
                        "pvd_created": p[4],
                        "boot_file": p[5],
                        "track1_size": int(p[6]),
                        "track1_sha1": p[7],
                        "volume_set_id": p[8] if len(p) > 8 else "",
                    }
                elif p[0] == "exe":
                    # exe <id> <size> <sha1> <entry> <gp> <text_addr> <text_size> <rodata_addr> <rodata_size>
                    exes_meta[p[1]] = {
                        "size": int(p[2]),
                        "sha1": p[3],
                        "entry": int(p[4], 16),
                        "gp": int(p[5], 16),
                        "text_addr": int(p[6], 16),
                        "text_size": int(p[7], 16),
                        "rodata_addr": int(p[8], 16),
                        "rodata_size": int(p[9], 16),
                    }
                elif p[0] == "ovl":
                    # ovl <id> <NAME.EXN|O.BIN> <sha1>
                    ovls_meta.setdefault(p[1], {})[p[2]] = p[3]

        id_map = {
            "best": "best_slpm_86053",
            "rev4": "rev4_slps_00065_v143",
            "rev2": "rev2_slps_00065_v125",
            "rev1": "rev1_slps_00065_v110",
            "shokai": "shokai_slps_00064_v110",
        }
        diff_functions = {"best": 0, "rev4": 46, "rev2": 475, "rev1": 521, "shokai": 521}
        diag_codes = {
            "best": "OK_PLAYSTATION_THE_BEST",
            "rev4": "ERR_REV4_UNSUPPORTED",
            "rev2": "ERR_REV2_UNSUPPORTED",
            "rev1": "ERR_REV1_UNSUPPORTED",
            "shokai": "ERR_SHOKAI_UNSUPPORTED",
        }

        profiles: Dict[str, ReleaseProfile] = {}
        for short_id, canon_id in id_map.items():
            vm = vers_meta.get(short_id, {})
            em = exes_meta.get(short_id, {})
            ovl = ovls_meta.get(short_id, {})
            boot_f = vm.get("boot_file", "PSX.EXE" if short_id != "best" else "SLPM_86.053")
            sys_boot = f"cdrom:{boot_f};1"
            prof = ReleaseProfile(
                id=canon_id,
                short_id=short_id,
                label=vm.get("label", canon_id),
                boot_file=boot_f,
                exe_size=em.get("size", 667648),
                exe_sha1=em.get("sha1", ""),
                entry_addr=em.get("entry", 0),
                gp_addr=em.get("gp", 0),
                text_addr=em.get("text_addr", 0),
                text_size=em.get("text_size", 0),
                rodata_addr=em.get("rodata_addr", 0),
                rodata_size=em.get("rodata_size", 0),
                volume_id=vm.get("volume_id", ""),
                volume_set_id=vm.get("volume_set_id", ""),
                pvd_created=vm.get("pvd_created", ""),
                track1_size=vm.get("track1_size", 0),
                track1_sha1=vm.get("track1_sha1", ""),
                system_cnf_boot=sys_boot,
                overlays=dict(ovl),
                supported_for_content_extraction=(short_id == "best"),
                changed_exe_functions_vs_best=diff_functions.get(short_id, 0),
                diagnostic_code=diag_codes.get(short_id, "ERR_UNKNOWN_EXECUTABLE"),
            )
            profiles[canon_id] = prof

        return cls(profiles)

    def identify_executable(
        self,
        exe_name: str,
        exe_size: int,
        exe_sha1: str,
        cnf_text: Optional[str] = None,
        vol_id: Optional[str] = None,
        vol_set_id: Optional[str] = None,
    ) -> Dict[str, Any]:
        """Identify release profile strictly from executable hash, size, and disc metadata."""
        matching_pids = self._sha1_to_profile_ids.get(exe_sha1.lower(), [])

        if not matching_pids:
            return {
                "release_id": "unknown",
                "canonical_profile": "unknown",
                "label": "Unknown PS1 Executable",
                "supported_for_content_extraction": False,
                "boot_target_valid": False,
                "diagnostic_code": "ERR_UNKNOWN_EXECUTABLE",
                "diagnostic": (
                    f"Unknown PS1 executable '{exe_name}' (size {exe_size} bytes, "
                    f"SHA1 {exe_sha1[:8]}). Expected PlayStation the Best (SLPM-86053 / VX009J2, "
                    f"SHA1 {DEFAULT_PROFILES['best_slpm_86053'].exe_sha1[:8]})."
                ),
            }

        target_pid = matching_pids[0]
        # Disambiguate rev1 vs shokai
        if len(matching_pids) > 1:
            set_id = (vol_set_id or "").upper()
            cnf = (cnf_text or "").upper()
            if "SLPS00064" in set_id or "SLPS-00064" in cnf or "SLPS-00067" in cnf:
                target_pid = "shokai_slps_00064_v110"
            elif "SLPS00065" in set_id:
                target_pid = "rev1_slps_00065_v110"
            else:
                # Extracted directory without PVD volume set ID
                target_pid = "rev1_or_shokai_v110"

        if target_pid == "rev1_or_shokai_v110":
            prof = self.profiles["rev1_slps_00065_v110"]
            boot_valid = self._check_boot_target("PSX.EXE", cnf_text)
            return {
                "release_id": "rev1",
                "canonical_profile": "rev1_or_shokai_v110",
                "label": "Rev 1 (also labeled v1.1) / Shokai Genteiban (1995-08-31)",
                "supported_for_content_extraction": False,
                "boot_target_valid": boot_valid,
                "diagnostic_code": "ERR_REV1_OR_SHOKAI_UNSUPPORTED",
                "diagnostic": (
                    f"Detected Rev 1 / Shokai Genteiban with PSX.EXE (SHA1 {exe_sha1[:8]}). "
                    f"This release differs in 521 functions, older PsyQ 3.x libraries, and moved LBAs. "
                    f"This port requires PlayStation the Best (SLPM-86053 / VX009J2, 1997-12-25) per wiki/versions.md."
                ),
            }

        profile = self.profiles[target_pid]
        boot_valid = self._check_boot_target(profile.boot_file, cnf_text)

        # Build diagnostic string
        diagnostic = ""
        diagnostic_code = profile.diagnostic_code

        if profile.short_id == "best":
            if not boot_valid and cnf_text:
                diagnostic_code = "ERR_INVALID_BOOT_TARGET"
                diagnostic = (
                    f"SYSTEM.CNF boot target mismatch for PlayStation the Best: "
                    f"expected {profile.system_cnf_boot}"
                )
        elif profile.short_id == "rev4":
            diagnostic = (
                f"Detected Rev 4 (SLPS-00065VER143, 1996-03-31) with PSX.EXE (SHA1 {exe_sha1[:8]}). "
                f"This release differs in 46 functions and shifted data symbols (rodata at 0x800AF1F0). "
                f"This port requires PlayStation the Best (SLPM-86053 / VX009J2, 1997-12-25) per wiki/versions.md."
            )
        elif profile.short_id == "rev2":
            diagnostic = (
                f"Detected Rev 2 (SLPS-00065VER125, 1995-11-17) with PSX.EXE (SHA1 {exe_sha1[:8]}). "
                f"This release differs in 475 functions and older SDK libraries. "
                f"This port requires PlayStation the Best (SLPM-86053 / VX009J2, 1997-12-25) per wiki/versions.md."
            )
        elif profile.short_id in ("rev1", "shokai"):
            diagnostic = (
                f"Detected {profile.label} with PSX.EXE (SHA1 {exe_sha1[:8]}). "
                f"This release differs in 521 functions, older PsyQ 3.x libraries, and moved LBAs. "
                f"This port requires PlayStation the Best (SLPM-86053 / VX009J2, 1997-12-25) per wiki/versions.md."
            )

        return {
            "release_id": profile.short_id,
            "canonical_profile": profile.id,
            "label": profile.label,
            "supported_for_content_extraction": profile.supported_for_content_extraction,
            "boot_target_valid": boot_valid,
            "diagnostic_code": diagnostic_code,
            "diagnostic": diagnostic,
        }

    def _check_boot_target(self, expected_exe: str, cnf_text: Optional[str]) -> bool:
        if not cnf_text:
            return True
        m = re.search(r"BOOT\s*=\s*cdrom:\\?([^;\s\r\n]+)", cnf_text, re.IGNORECASE)
        if m:
            found_boot = m.group(1).strip()
            return found_boot.upper() == expected_exe.upper()
        return False

    def evaluate_content_compatibility(
        self,
        identified_profile_id: str,
        found_overlays: Dict[str, str],
    ) -> Dict[str, Any]:
        """Separates container/release recognition from exact content extraction compatibility."""
        target_profile = self.profiles[CANONICAL_TARGET_ID]
        identified = self.profiles.get(identified_profile_id)

        per_overlay: Dict[str, Dict[str, Any]] = {}
        identical_count = 0

        for ovl in OVERLAY_NAMES:
            actual_sha = found_overlays.get(ovl, "")
            target_sha = target_profile.overlays.get(ovl, "")
            expected_sha = identified.overlays.get(ovl, "") if identified else ""
            is_identical_to_target = (actual_sha.lower() == target_sha.lower()) if actual_sha else False
            if is_identical_to_target:
                identical_count += 1
            per_overlay[ovl] = {
                "present": bool(actual_sha),
                "actual_sha1": actual_sha,
                "target_sha1": target_sha,
                "expected_for_release_sha1": expected_sha,
                "identical_to_best_target": is_identical_to_target,
            }

        is_best = (identified_profile_id == CANONICAL_TARGET_ID)
        status = "FULLY_COMPATIBLE" if is_best else f"INCOMPATIBLE_{identified_profile_id.upper()}"

        summary = (
            f"All 27 overlays match PlayStation the Best target."
            if is_best
            else f"Identified release has {27 - identical_count} overlay(s) incompatible with PlayStation the Best."
        )

        return {
            "status": status,
            "supported_for_content_extraction": is_best,
            "identical_overlay_count": identical_count,
            "total_overlay_count": len(OVERLAY_NAMES),
            "common_identical_overlays": list(COMMON_BYTE_IDENTICAL_OVERLAYS),
            "per_overlay": per_overlay,
            "summary": summary,
        }

    def verify_source(self, path: str) -> Dict[str, Any]:
        """Inspect and verify a path (directory, cue, raw image, iso, zip)."""
        if not os.path.exists(path):
            return {
                "container": {"type": "missing", "valid": False, "error": f"Path not found: {path}"},
                "release": {"release_id": "unknown", "diagnostic_code": "ERR_SOURCE_NOT_FOUND"},
                "compatibility": {"supported_for_content_extraction": False},
            }

        if os.path.isdir(path):
            return self._verify_directory(path)

        ext = os.path.splitext(path)[1].lower()
        if ext == ".zip":
            return self._verify_zip(path)
        elif ext == ".cue":
            return self._verify_cue(path)
        elif ext in (".bin", ".iso", ".img"):
            return self._verify_disc_image(path)

        return {
            "container": {"type": "unsupported", "valid": False, "error": f"Unsupported container format: {ext}"},
            "release": {"release_id": "unknown", "diagnostic_code": "ERR_UNSUPPORTED_CONTAINER"},
            "compatibility": {"supported_for_content_extraction": False},
        }

    def _verify_directory(self, dir_path: str) -> Dict[str, Any]:
        cnf_text = ""
        for name in ("SYSTEM.CNF", "system.cnf"):
            p = os.path.join(dir_path, name)
            if os.path.isfile(p):
                with open(p, "r", encoding="latin1", errors="replace") as f:
                    cnf_text = f.read()
                break

        exe_name = ""
        for candidate in ("SLPM_86.053", "slpm_86.053", "PSX.EXE", "psx.exe"):
            p = os.path.join(dir_path, candidate)
            if os.path.isfile(p):
                exe_name = os.path.basename(p).upper()
                exe_path = p
                break

        if not exe_name:
            return {
                "container": {"type": "directory", "valid": False, "error": "No boot executable found in directory"},
                "release": {"release_id": "unknown", "diagnostic_code": "ERR_NO_BOOT_EXECUTABLE"},
                "compatibility": {"supported_for_content_extraction": False},
            }

        with open(exe_path, "rb") as f:
            exe_data = f.read()
        exe_sha1 = hashlib.sha1(exe_data).hexdigest()

        # Collect overlays in CDROM/EXEDIR or exedir
        overlays: Dict[str, str] = {}
        for base in (
            os.path.join(dir_path, "CDROM", "EXEDIR"),
            os.path.join(dir_path, "cdrom", "exedir"),
            dir_path,
        ):
            if os.path.isdir(base):
                for fname in os.listdir(base):
                    up = fname.upper()
                    if up.endswith(".EXN") or up == "O.BIN":
                        p = os.path.join(base, fname)
                        if os.path.isfile(p):
                            with open(p, "rb") as f:
                                overlays[up] = hashlib.sha1(f.read()).hexdigest()

        release_info = self.identify_executable(
            exe_name=exe_name,
            exe_size=len(exe_data),
            exe_sha1=exe_sha1,
            cnf_text=cnf_text,
        )

        compat = self.evaluate_content_compatibility(
            release_info.get("canonical_profile", "unknown"),
            overlays,
        )

        return {
            "container": {
                "type": "directory",
                "valid": True,
                "path": dir_path,
                "boot_executable": exe_name,
                "overlays_found": len(overlays),
            },
            "release": release_info,
            "compatibility": compat,
        }

    def _verify_cue(self, cue_path: str) -> Dict[str, Any]:
        with open(cue_path, "r", encoding="latin1", errors="replace") as f:
            content = f.read()
        m = re.search(r'FILE\s+"([^"]+)"', content, re.IGNORECASE)
        if not m:
            return {
                "container": {"type": "cue", "valid": False, "error": "No FILE entry found in CUE sheet"},
                "release": {"release_id": "unknown", "diagnostic_code": "ERR_INVALID_CUE"},
                "compatibility": {"supported_for_content_extraction": False},
            }
        track1_rel = m.group(1)
        track1_path = os.path.join(os.path.dirname(cue_path), track1_rel)
        if not os.path.isfile(track1_path):
            return {
                "container": {"type": "cue", "valid": False, "error": f"Track file not found: {track1_path}"},
                "release": {"release_id": "unknown", "diagnostic_code": "ERR_TRACK_NOT_FOUND"},
                "compatibility": {"supported_for_content_extraction": False},
            }
        res = self._verify_disc_image(track1_path)
        res["container"]["type"] = "cue"
        res["container"]["cue_path"] = cue_path
        res["container"]["has_audio_track"] = "TRACK 02 AUDIO" in content
        return res

    def _verify_disc_reader(
        self,
        dr: DiscReader,
        container_type: str,
        path: str,
        extra: Optional[Dict[str, Any]] = None,
    ) -> Dict[str, Any]:
        pvd, _ = dr.read_sector(16)
        if len(pvd) < 2048 or pvd[1:6] != b"CD001":
            return {
                "container": {"type": container_type, "valid": False, "error": "Missing ISO9660 PVD at sector 16"},
                "release": {"release_id": "unknown", "diagnostic_code": "ERR_INVALID_ISO_PVD"},
                "compatibility": {"supported_for_content_extraction": False},
            }

        vol_id = pvd[40:72].decode("latin1", errors="replace").strip()
        vol_set_id = pvd[190:318].decode("latin1", errors="replace").strip()
        created = pvd[813:829].decode("latin1", errors="replace").strip()

        root_rec = pvd[156:190]
        root_lba = struct.unpack("<I", root_rec[2:6])[0]
        root_size = struct.unpack("<I", root_rec[10:14])[0]

        iso_map = dr.index_tree(root_lba, root_size)

        cnf_text = ""
        if "SYSTEM.CNF" in iso_map:
            lba, sz = iso_map["SYSTEM.CNF"]
            cnf_text = dr.read_file(lba, sz).decode("latin1", errors="replace")

        exe_name = ""
        for cand in ("SLPM_86.053", "PSX.EXE"):
            if cand in iso_map:
                exe_name = cand
                break

        if not exe_name:
            return {
                "container": {"type": container_type, "valid": False, "error": "Boot executable not found in disc image"},
                "release": {"release_id": "unknown", "diagnostic_code": "ERR_NO_BOOT_EXECUTABLE"},
                "compatibility": {"supported_for_content_extraction": False},
            }

        elba, esize = iso_map[exe_name]
        exe_data = dr.read_file(elba, esize)
        exe_sha1 = hashlib.sha1(exe_data).hexdigest()

        overlays: Dict[str, str] = {}
        for ovl in OVERLAY_NAMES:
            for prefix in ("CDROM/EXEDIR/", "EXEDIR/", ""):
                key = prefix + ovl
                if key in iso_map:
                    olba, osize = iso_map[key]
                    odata = dr.read_file(olba, osize)
                    if odata:
                        overlays[ovl] = hashlib.sha1(odata).hexdigest()
                    break

        release_info = self.identify_executable(
            exe_name=exe_name,
            exe_size=len(exe_data),
            exe_sha1=exe_sha1,
            cnf_text=cnf_text,
            vol_id=vol_id,
            vol_set_id=vol_set_id,
        )

        compat = self.evaluate_content_compatibility(
            release_info.get("canonical_profile", "unknown"),
            overlays,
        )

        container_dict = {
            "type": container_type,
            "valid": True,
            "path": path,
            "volume_id": vol_id,
            "volume_set_id": vol_set_id,
            "pvd_created": created,
            "boot_executable": exe_name,
            "overlays_found": len(overlays),
        }
        if extra:
            container_dict.update(extra)

        return {
            "container": container_dict,
            "release": release_info,
            "compatibility": compat,
        }

    def _verify_disc_image(self, img_path: str) -> Dict[str, Any]:
        file_size = os.path.getsize(img_path)
        with open(img_path, "rb") as f:
            head = f.read(12)
            is_mode2 = head == b"\x00" + b"\xff" * 10 + b"\x00"
            f.seek(0)
            dr = DiscReader(f, is_mode2)
            return self._verify_disc_reader(
                dr,
                "raw_image" if is_mode2 else "iso",
                img_path,
                {"file_size": file_size, "sector_size": dr.sector_size},
            )

    def _verify_zip(self, zip_path: str) -> Dict[str, Any]:
        try:
            with zipfile.ZipFile(zip_path, "r") as z:
                names = z.namelist()
                name_map = {n.upper(): n for n in names}

                # 1. Check if zip contains extracted files directly
                boot_name = ""
                for cand in ("SLPM_86.053", "PSX.EXE"):
                    if cand in name_map:
                        boot_name = cand
                        break

                if boot_name:
                    real_boot = name_map[boot_name]
                    exe_data = z.read(real_boot)
                    cnf_text = ""
                    if "SYSTEM.CNF" in name_map:
                        cnf_text = z.read(name_map["SYSTEM.CNF"]).decode("latin1", errors="replace")

                    overlays: Dict[str, str] = {}
                    for ovl in OVERLAY_NAMES:
                        for prefix in ("CDROM/EXEDIR/", "EXEDIR/", ""):
                            key = prefix + ovl
                            if key in name_map:
                                overlays[ovl] = hashlib.sha1(z.read(name_map[key])).hexdigest()
                                break

                    release_info = self.identify_executable(
                        exe_name=boot_name,
                        exe_size=len(exe_data),
                        exe_sha1=hashlib.sha1(exe_data).hexdigest(),
                        cnf_text=cnf_text,
                    )
                    compat = self.evaluate_content_compatibility(
                        release_info.get("canonical_profile", "unknown"),
                        overlays,
                    )
                    return {
                        "container": {
                            "type": "zip_files",
                            "valid": True,
                            "path": zip_path,
                            "file_count": len(names),
                            "boot_executable": boot_name,
                            "overlays_found": len(overlays),
                        },
                        "release": release_info,
                        "compatibility": compat,
                    }

                # 2. Check if zip holds a track image (.bin / .iso)
                track_member = None
                cue_member = next((n for n in sorted(names) if n.lower().endswith(".cue")), None)
                if cue_member:
                    cue_text = z.read(cue_member).decode("latin1", errors="replace")
                    m = re.search(r'FILE\s+"([^"]+)"', cue_text, re.IGNORECASE)
                    if m:
                        track_name = os.path.basename(m.group(1)).lower()
                        for n in names:
                            if os.path.basename(n).lower() == track_name:
                                track_member = n
                                break
                if not track_member:
                    for n in sorted(names):
                        if n.lower().endswith((".bin", ".iso", ".img")):
                            track_member = n
                            break

                if track_member:
                    with z.open(track_member) as tf:
                        head = tf.read(12)
                        is_mode2 = head == b"\x00" + b"\xff" * 10 + b"\x00"
                        tf.seek(0)
                        dr = DiscReader(tf, is_mode2)
                        return self._verify_disc_reader(
                            dr,
                            "zip_image",
                            zip_path,
                            {
                                "track_member": track_member,
                                "cue_member": cue_member,
                                "is_mode2": is_mode2,
                            },
                        )

                return {
                    "container": {"type": "zip", "valid": False, "error": "ZIP contains neither game files nor disc track"},
                    "release": {"release_id": "unknown", "diagnostic_code": "ERR_EMPTY_OR_INVALID_ZIP"},
                    "compatibility": {"supported_for_content_extraction": False},
                }

        except zipfile.BadZipFile as e:
            return {
                "container": {"type": "zip", "valid": False, "error": f"Bad ZIP archive: {e}"},
                "release": {"release_id": "unknown", "diagnostic_code": "ERR_BAD_ZIP"},
                "compatibility": {"supported_for_content_extraction": False},
            }

    def _read_iso_directory_tree(
        self,
        f: Any,
        lba: int,
        size: int,
        sector_size: int,
        is_mode2: bool,
    ) -> Dict[str, Tuple[int, int]]:
        result: Dict[str, Tuple[int, int]] = {}

        def walk(cur_lba: int, cur_size: int, prefix: str, depth: int):
            if depth > MAX_DIR_DEPTH:
                return
            sectors_needed = (cur_size + 2047) // 2048
            data = bytearray()
            for i in range(sectors_needed):
                f.seek((cur_lba + i) * sector_size + (16 if is_mode2 else 0))
                sec = f.read(2048)
                data.extend(sec)

            pos = 0
            while pos < cur_size:
                if pos >= len(data):
                    break
                rec_len = data[pos]
                if rec_len == 0:
                    pos = (pos // 2048 + 1) * 2048
                    continue
                rec = data[pos:pos + rec_len]
                if len(rec) < 33:
                    pos += rec_len
                    continue
                r_lba = struct.unpack("<I", rec[2:6])[0]
                r_size = struct.unpack("<I", rec[10:14])[0]
                flags = rec[25]
                name_len = rec[32]
                name = rec[33:33 + name_len].decode("latin1", errors="replace").split(";")[0]
                pos += rec_len

                if name in ("\x00", "\x01"):
                    continue

                full_path = prefix + name.upper()
                if flags & 2:
                    walk(r_lba, r_size, full_path + "/", depth + 1)
                else:
                    result[full_path] = (r_lba, r_size)

        walk(lba, size, "", 0)
        return result

    def _read_extent(
        self,
        f: Any,
        lba: int,
        size: int,
        sector_size: int,
        is_mode2: bool,
    ) -> bytes:
        out = bytearray()
        cur_lba = lba
        while len(out) < size:
            f.seek(cur_lba * sector_size + 16 if is_mode2 else cur_lba * sector_size)
            if is_mode2:
                sub = f.read(8)
                # Form 1 payload is at +24
                f.seek(cur_lba * sector_size + (24 if sub[:4] == sub[4:] else 16))
            chunk = f.read(min(2048, size - len(out)))
            if not chunk:
                break
            out.extend(chunk)
            cur_lba += 1
        return bytes(out[:size])


def identify_release(source_path: str, versions_txt: Optional[str] = None) -> Dict[str, Any]:
    """Convenience entry point for release verification."""
    if versions_txt and os.path.isfile(versions_txt):
        catalog = ReleaseCatalog.load_from_versions_txt(versions_txt)
    else:
        catalog = ReleaseCatalog()
    return catalog.verify_source(source_path)


def main() -> int:
    import argparse
    parser = argparse.ArgumentParser(description="Multi-Release PS1 Disc Catalog & Recognition Tool")
    parser.add_argument("source", help="Path to extracted directory, CUE sheet, disc image, or ZIP archive")
    parser.add_argument("--versions", default="config/versions.txt", help="Path to config/versions.txt")
    parser.add_argument("--json", action="store_true", help="Output raw JSON verification report")
    args = parser.parse_args()

    v_path = args.versions if os.path.isfile(args.versions) else None
    res = identify_release(args.source, v_path)

    if args.json:
        print(json.dumps(res, indent=2))
        return 0 if res["release"]["diagnostic_code"] != "ERR_UNKNOWN_EXECUTABLE" else 1

    # Human-readable report
    c = res.get("container", {})
    r = res.get("release", {})
    comp = res.get("compatibility", {})

    print(f"Container: {c.get('type')} (Valid: {c.get('valid')})")
    print(f"Release:   {r.get('label')} [{r.get('canonical_profile')}]")
    print(f"Code:      {r.get('diagnostic_code')}")
    if r.get("diagnostic"):
        print(f"Details:   {r.get('diagnostic')}")
    print(f"Supported for content extraction: {comp.get('supported_for_content_extraction')}")
    print(f"Status:    {comp.get('status')}")
    if "identical_overlay_count" in comp:
        print(f"Overlays:  {comp.get('identical_overlay_count')}/{comp.get('total_overlay_count')} identical to target")

    return 0 if r.get("diagnostic_code") != "ERR_UNKNOWN_EXECUTABLE" else 1


if __name__ == "__main__":
    sys.exit(main())
