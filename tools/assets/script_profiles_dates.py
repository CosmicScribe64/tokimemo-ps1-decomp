"""Verified linked-table layouts for the SERIFU DATE and EVENT scripts.

DATE profiles cover eleven files and use the overlay's date_fill_sections_*
initializers. EVENT profiles map the consumed talk-table assignments in the
girl-specific handlers to files through the DATE loader switch. Both sets load
at 0x80198000; pointer-table group extents were checked across each triad and
all sections were validated with parse_script. Checksums identify the exact
extracted files. Source evidence is recorded per profile.
"""

_DATE_FILES = [
    ("FU", "667597bc307eaa52ef3f11d83e6850048f9e94831578527b58f12cb48256c005", [
        (0x8019C1B8, 0x8019C1E8, 0x8019C350, 6),
        (0x801A33B0, 0x801A33F8, 0x801A3614, 9),
        (0x801A0540, 0x801A0588, 0x801A0750, 9),
        (0x801996C0, 0x80199708, 0x801998D0, 9),
        (0x8019E3BC, 0x8019E3EC, 0x8019E51C, 6),
        (0x8019DA2C, 0x8019DA9C, 0x8019DC94, 14),
        (0x801986D0, 0x80198768, 0x80198AFC, 19),
        (0x801A09D4, 0x801A0A04, 0x801A0B34, 6),
        (0x8019AEC0, 0x8019AF10, 0x8019B0DC, 10),
        (0x80198DCC, 0x80198DFC, 0x80198F2C, 6),
        (0x8019BDC8, 0x8019BDF8, 0x8019BF28, 6),
        (0x801A22B8, 0x801A2480, 0x801A2EB0, 57),
        (0x8019ED18, 0x8019EDB8, 0x8019F150, 20),
        (0x8019FEE4, 0x8019FF44, 0x801A01C0, 12),
        (0x8019CDF0, 0x8019CE28, 0x8019CF24, 7),
        (0x801991F0, 0x80199220, 0x80199318, 6),
        (0x8019D39C, 0x8019D3CC, 0x8019D4FC, 6),
        (0x8019E030, 0x8019E060, 0x8019E158, 6),
        (0x8019A73C, 0x8019A7B4, 0x8019AA3C, 15),
        (0x8019F41C, 0x8019F44C, 0x8019F57C, 6),
        (0x8019F7E0, 0x8019F810, 0x8019F940, 6),
    ]),
    ("KI", "221b58aa6bd996a64dbf61a0c08a5ed300b9003fe1f7f1187f9cc424649cf074", [
        (0x8019C7D8, 0x8019C808, 0x8019C970, 6),
        (0x801A353C, 0x801A35B4, 0x801A3938, 15),
        (0x801A05AC, 0x801A05F4, 0x801A07BC, 9),
        (0x801997D0, 0x80199838, 0x80199AB8, 13),
        (0x8019EAD0, 0x8019EB00, 0x8019EC30, 6),
        (0x8019E0A0, 0x8019E110, 0x8019E308, 14),
        (0x801986C8, 0x80198760, 0x80198AF4, 19),
        (0x801A0A50, 0x801A0A80, 0x801A0BB0, 6),
        (0x8019B548, 0x8019B598, 0x8019B764, 10),
        (0x80198D8C, 0x80198DBC, 0x80198EEC, 6),
        (0x8019C404, 0x8019C434, 0x8019C564, 6),
        (0x801A1FCC, 0x801A219C, 0x801A2C58, 57),
        (0x8019F354, 0x8019F3D4, 0x8019F698, 16),
        (0x8019FF48, 0x8019FFA8, 0x801A0208, 12),
        (0x8019D4D4, 0x8019D50C, 0x8019D608, 7),
        (0x801991A0, 0x801991D0, 0x801992C8, 6),
        (0x8019DA64, 0x8019DA94, 0x8019DBA8, 6),
        (0x8019E754, 0x8019E784, 0x8019E87C, 6),
        (0x8019AD68, 0x8019ADE0, 0x8019B068, 15),
        (0x8019F938, 0x8019F968, 0x8019FA98, 6),
    ]),
    ("HI", "7586d0ceb898e484a10c5e49f6b5501ec0eae55d5846ac4cd0f32c99839a5b52", [
        (0x8019BD30, 0x8019BD60, 0x8019BEC8, 6),
        (0x801A2014, 0x801A205C, 0x801A2278, 9),
        (0x8019F490, 0x8019F4D8, 0x8019F6A0, 9),
        (0x801994E0, 0x80199528, 0x801996F0, 9),
        (0x8019DB58, 0x8019DB88, 0x8019DCB8, 6),
        (0x8019D268, 0x8019D2D8, 0x8019D4D0, 14),
        (0x8019863C, 0x801986D4, 0x80198A68, 19),
        (0x8019F90C, 0x8019F93C, 0x8019FA6C, 6),
        (0x8019AD74, 0x8019ADC4, 0x8019AF90, 10),
        (0x80198CE0, 0x80198D10, 0x80198E40, 6),
        (0x8019B96C, 0x8019B99C, 0x8019BACC, 6),
        (0x801A0F20, 0x801A10EC, 0x801A1B70, 57),
        (0x8019E2B8, 0x8019E328, 0x8019E5AC, 14),
        (0x8019EDFC, 0x8019EE5C, 0x8019F0BC, 12),
        (0x8019C8D8, 0x8019C910, 0x8019CA0C, 7),
        (0x80199074, 0x801990A4, 0x80199180, 6),
        (0x8019CD88, 0x8019CDB8, 0x8019CECC, 6),
        (0x8019D7CC, 0x8019D7FC, 0x8019D8F4, 6),
        (0x8019A698, 0x8019A700, 0x8019A92C, 13),
        (0x8019E7D8, 0x8019E808, 0x8019E938, 6),
    ]),
    ("KA", "f7d3c4a7cf2d1c02c51fc981861bc7f414e1bfd41e9767a9116f9067c263540a", [
        (0x8019C09C, 0x8019C0CC, 0x8019C234, 6),
        (0x801A2A40, 0x801A2A88, 0x801A2CA4, 9),
        (0x8019FABC, 0x8019FB04, 0x8019FCCC, 9),
        (0x8019954C, 0x80199594, 0x8019975C, 9),
        (0x8019E114, 0x8019E144, 0x8019E274, 6),
        (0x8019D7D0, 0x8019D840, 0x8019DA38, 14),
        (0x8019864C, 0x801986E4, 0x80198A78, 19),
        (0x8019FF78, 0x8019FFA8, 0x801A00D8, 6),
        (0x8019ADBC, 0x8019AE0C, 0x8019AFD8, 10),
        (0x80198CFC, 0x80198D2C, 0x80198E5C, 6),
        (0x8019BCB4, 0x8019BCE4, 0x8019BE14, 6),
        (0x801A195C, 0x801A1B24, 0x801A2554, 57),
        (0x8019E8D4, 0x8019E944, 0x8019EBC8, 14),
        (0x8019F464, 0x8019F4C4, 0x8019F724, 12),
        (0x8019CCB8, 0x8019CCF0, 0x8019CDEC, 7),
        (0x801990B4, 0x801990E4, 0x801991DC, 6),
        (0x8019D234, 0x8019D264, 0x8019D378, 6),
        (0x8019DDB4, 0x8019DDE4, 0x8019DEC0, 6),
        (0x8019A668, 0x8019A6E0, 0x8019A968, 15),
        (0x8019EE5C, 0x8019EE8C, 0x8019EFBC, 6),
    ]),
    ("NI", "e34fc63acbbccb109b210a3027dbe65dba1da7331f24419a0271e97e23583097", [
        (0x8019C064, 0x8019C094, 0x8019C1FC, 6),
        (0x801A2A94, 0x801A2ADC, 0x801A2CF8, 9),
        (0x8019FF1C, 0x8019FF64, 0x801A012C, 9),
        (0x801995D8, 0x80199620, 0x801997E8, 9),
        (0x8019E0E0, 0x8019E110, 0x8019E240, 6),
        (0x8019D760, 0x8019D7D0, 0x8019D9C8, 14),
        (0x801986B4, 0x8019874C, 0x80198AE0, 19),
        (0x801A038C, 0x801A03BC, 0x801A04EC, 6),
        (0x8019ADC0, 0x8019AE10, 0x8019AFDC, 10),
        (0x80198D60, 0x80198D90, 0x80198EC0, 6),
        (0x8019BC98, 0x8019BCC8, 0x8019BDF8, 6),
        (0x801A19C4, 0x801A1B7C, 0x801A2588, 55),
        (0x8019E8EC, 0x8019E96C, 0x8019EC30, 16),
        (0x8019F880, 0x8019F8E0, 0x8019FB40, 12),
        (0x8019CC98, 0x8019CCD0, 0x8019CDCC, 7),
        (0x80199124, 0x80199154, 0x8019924C, 6),
        (0x8019D1D8, 0x8019D208, 0x8019D338, 6),
        (0x8019DDA4, 0x8019DDD4, 0x8019DECC, 6),
        (0x8019A664, 0x8019A6DC, 0x8019A964, 15),
        (0x8019EEA8, 0x8019EED8, 0x8019F008, 6),
        (0x8019F220, 0x8019F250, 0x8019F380, 6),
    ]),
    ("KO", "8c37969a2ce464e5467de7941f2a4350b56337bb3f7417a47cedb25a11e7c894", [
        (0x8019C29C, 0x8019C2CC, 0x8019C434, 6),
        (0x801A2F2C, 0x801A2F74, 0x801A3190, 9),
        (0x801A042C, 0x801A0474, 0x801A063C, 9),
        (0x80199578, 0x801995C0, 0x80199788, 9),
        (0x8019E4A0, 0x8019E4D0, 0x8019E600, 6),
        (0x8019DA04, 0x8019DA74, 0x8019DCA4, 14),
        (0x80198658, 0x801986F0, 0x80198A84, 19),
        (0x801A08A0, 0x801A08D0, 0x801A0A00, 6),
        (0x8019B04C, 0x8019B09C, 0x8019B268, 10),
        (0x80198D1C, 0x80198D4C, 0x80198E7C, 6),
        (0x8019BECC, 0x8019BEFC, 0x8019C02C, 6),
        (0x801A1E38, 0x801A1FF4, 0x801A2A1C, 55),
        (0x8019ED60, 0x8019EDE0, 0x8019F0C0, 16),
        (0x8019FE34, 0x8019FE94, 0x801A00F4, 12),
        (0x8019CF0C, 0x8019CF44, 0x8019D05C, 7),
        (0x801990D4, 0x80199104, 0x801991FC, 6),
        (0x8019D418, 0x8019D448, 0x8019D55C, 6),
        (0x8019E0BC, 0x8019E104, 0x8019E25C, 9),
        (0x8019A900, 0x8019A968, 0x8019AB94, 13),
        (0x8019F344, 0x8019F374, 0x8019F4A4, 6),
        (0x8019F784, 0x8019F7B8, 0x8019F920, 6),
    ]),
    ("KY", "24f09f4921c7a35ade1aaa9fb3a5f068a748129eefffb5f7c25bbd877259e07d", [
        (0x8019BD94, 0x8019BDC4, 0x8019BF2C, 6),
        (0x801A1FBC, 0x801A2004, 0x801A2220, 9),
        (0x8019F798, 0x8019F7E0, 0x8019F9A8, 9),
        (0x80199304, 0x8019934C, 0x80199514, 9),
        (0x8019DBC0, 0x8019DBF0, 0x8019DD20, 6),
        (0x8019D2C4, 0x8019D334, 0x8019D52C, 14),
        (0x801984E4, 0x8019857C, 0x80198910, 19),
        (0x8019FB88, 0x8019FBB8, 0x8019FCE8, 6),
        (0x8019ABAC, 0x8019ABFC, 0x8019ADC8, 10),
        (0x80198B4C, 0x80198B7C, 0x80198CAC, 6),
        (0x8019B9C8, 0x8019B9F8, 0x8019BB28, 6),
        (0x801A0F6C, 0x801A1134, 0x801A1B64, 57),
        (0x8019E344, 0x8019E3BC, 0x8019E67C, 15),
        (0x8019F180, 0x8019F1E0, 0x8019F440, 12),
        (0x8019C908, 0x8019C940, 0x8019CA3C, 7),
        (0x80198EAC, 0x80198EDC, 0x80198FD4, 6),
        (0x8019CDCC, 0x8019CDFC, 0x8019CF10, 6),
        (0x8019D820, 0x8019D850, 0x8019D980, 6),
        (0x8019A4C4, 0x8019A53C, 0x8019A7C4, 15),
        (0x8019E884, 0x8019E8B4, 0x8019E9E4, 6),
        (0x8019EBFC, 0x8019EC2C, 0x8019ED5C, 6),
    ]),
    ("KG", "6ba9533d0c937edb37568646809cf986cfcf8fad3318448a99fc304c7403038e", [
        (0x8019C708, 0x8019C744, 0x8019C900, 6),
        (0x801A34DC, 0x801A3524, 0x801A3740, 9),
        (0x801A048C, 0x801A04D4, 0x801A06B8, 9),
        (0x80199794, 0x801997DC, 0x801999A4, 9),
        (0x8019E4E0, 0x8019E520, 0x8019E690, 8),
        (0x8019DE70, 0x8019DEE0, 0x8019E0D8, 14),
        (0x801987B0, 0x80198868, 0x80198C7C, 23),
        (0x801A0A34, 0x801A0A7C, 0x801A0C60, 9),
        (0x8019B274, 0x8019B2DC, 0x8019B55C, 13),
        (0x80198F70, 0x80198FA0, 0x801990D0, 6),
        (0x8019C24C, 0x8019C284, 0x8019C3D4, 7),
        (0x801A2360, 0x801A2530, 0x801A2FD4, 58),
        (0x8019EDC4, 0x8019EE54, 0x8019F1AC, 18),
        (0x8019FDEC, 0x8019FE4C, 0x801A00AC, 12),
        (0x8019D354, 0x8019D38C, 0x8019D488, 7),
        (0x801992FC, 0x8019932C, 0x80199424, 6),
        (0x8019D8F8, 0x8019D928, 0x8019DA3C, 6),
        (0x8019E254, 0x8019E25C, 0x8019E27C, 1),
        (0x8019AA08, 0x8019AA80, 0x8019AD24, 15),
        (0x8019F450, 0x8019F480, 0x8019F5B0, 6),
        (0x8019F7F8, 0x8019F828, 0x8019F958, 6),
    ]),
    ("AS", "f2d9b1339cf4bd85b0200bf6d73a086d552f7976af2b396546c605d4e0d2afd7", [
        (0x8019BC2C, 0x8019BC5C, 0x8019BDC4, 6),
        (0x801A1BFC, 0x801A1C44, 0x801A1E60, 9),
        (0x8019F430, 0x8019F478, 0x8019F640, 9),
        (0x8019939C, 0x801993E4, 0x801995AC, 9),
        (0x8019DB44, 0x8019DB74, 0x8019DCA4, 6),
        (0x8019D284, 0x8019D2F4, 0x8019D4EC, 14),
        (0x80198548, 0x801985E0, 0x80198974, 19),
        (0x8019F79C, 0x8019F7C4, 0x8019F8B8, 5),
        (0x8019AA10, 0x8019AA60, 0x8019AC2C, 10),
        (0x80198BB0, 0x80198BE0, 0x80198D10, 6),
        (0x8019B844, 0x8019B874, 0x8019B9A4, 6),
        (0x801A0B24, 0x801A0CE0, 0x801A1778, 55),
        (0x8019E0E4, 0x8019E13C, 0x8019E328, 11),
        (0x8019EE34, 0x8019EE94, 0x8019F0F4, 12),
        (0x8019C828, 0x8019C860, 0x8019C95C, 7),
        (0x80198F58, 0x80198F88, 0x80199080, 6),
        (0x8019CD38, 0x8019CD68, 0x8019CE98, 6),
        (0x8019D82C, 0x8019D85C, 0x8019D954, 6),
        (0x8019A378, 0x8019A3E0, 0x8019A60C, 13),
        (0x8019E558, 0x8019E588, 0x8019E6B8, 6),
        (0x8019E8D4, 0x8019E904, 0x8019EA34, 6),
    ]),
    ("MI", "8974457edf3c29436b950aeb38d34708cfb0580184d5d7db719dc281b834040d", [
        (0x8019C19C, 0x8019C1CC, 0x8019C334, 6),
        (0x801A2524, 0x801A256C, 0x801A2788, 9),
        (0x8019FA88, 0x8019FAD0, 0x8019FC98, 9),
        (0x801994D8, 0x80199520, 0x801996E8, 9),
        (0x8019E1AC, 0x8019E1DC, 0x8019E30C, 6),
        (0x8019D794, 0x8019D804, 0x8019DA6C, 14),
        (0x8019867C, 0x80198714, 0x80198AA8, 19),
        (0x8019FF00, 0x8019FF30, 0x801A0060, 6),
        (0x8019AFD8, 0x8019B028, 0x8019B1F4, 10),
        (0x80198CF0, 0x80198D20, 0x80198E50, 6),
        (0x8019BDDC, 0x8019BE0C, 0x8019BF3C, 6),
        (0x801A14A4, 0x801A1668, 0x801A20E8, 56),
        (0x8019E918, 0x8019E998, 0x8019EC5C, 16),
        (0x8019F434, 0x8019F494, 0x8019F6F4, 12),
        (0x8019CCF0, 0x8019CD28, 0x8019CE40, 7),
        (0x8019909C, 0x801990CC, 0x801991C4, 6),
        (0x8019D244, 0x8019D274, 0x8019D388, 6),
        (0x8019DE50, 0x8019DE80, 0x8019DF78, 6),
        (0x8019A884, 0x8019A904, 0x8019ABAC, 16),
        (0x8019EEC0, 0x8019EEF0, 0x8019F020, 6),
    ]),
    ("SA", "6f0fe0aa594ea1debaa9545247ebd3b68daea9843ab952e3b74c37084929b7d3", [
        (0x8019BDBC, 0x8019BDEC, 0x8019BF54, 6),
        (0x801A2B84, 0x801A2BCC, 0x801A2DE8, 9),
        (0x8019FC30, 0x8019FC80, 0x8019FE68, 10),
        (0x801995C8, 0x80199610, 0x801997D8, 9),
        (0x8019DE7C, 0x8019DEAC, 0x8019DFDC, 6),
        (0x8019D474, 0x8019D4F4, 0x8019D764, 16),
        (0x8019868C, 0x8019872C, 0x80198AE0, 20),
        (0x801A00FC, 0x801A0134, 0x801A02A0, 7),
        (0x8019ABB4, 0x8019AC04, 0x8019ADD0, 10),
        (0x80198D9C, 0x80198DD4, 0x80198F24, 7),
        (0x8019B9E0, 0x8019BA10, 0x8019BB40, 6),
        (0x801A196C, 0x801A1B2C, 0x801A2654, 56),
        (0x8019E6BC, 0x8019E734, 0x8019E9D8, 15),
        (0x8019F5BC, 0x8019F61C, 0x8019F87C, 12),
        (0x8019C8FC, 0x8019C934, 0x8019CA30, 7),
        (0x80199134, 0x80199164, 0x8019925C, 6),
        (0x8019CE74, 0x8019CEA4, 0x8019CFB8, 6),
        (0x8019DB20, 0x8019DB50, 0x8019DC48, 6),
        (0x8019A4A4, 0x8019A510, 0x8019A758, 13),
        (0x8019EC48, 0x8019EC78, 0x8019EDA8, 6),
        (0x8019F024, 0x8019F054, 0x8019F184, 6),
    ]),
]

# Additional DATE roots explicitly consumed through D_800CA13C/D_800CA140/
# D_800CA144 in the overlay. Their table globals sit at the tail of the
# initializer's parallel text/command/voice symbol ranges.
_DATE_EXTRA = {
    "FU": [
        ("D_8015BDE4", 0x8019D048, 0x8019D068, 0x8019D120, 4),
        ("D_8015BE18", 0x80199F04, 0x80199F94, 0x8019A3CC, 18),
        ("D_8015BE1C", 0x8019B658, 0x8019B6CC, 0x8019BAD8, 14),
        ("D_8015BE20", 0x8019C4B4, 0x8019C4DC, 0x8019C608, 5),
        ("D_8015BE24", 0x8019C94C, 0x8019C978, 0x8019CAF8, 5),
        ("D_8015BE28", 0x8019DD58, 0x8019DD78, 0x8019DDF8, 4),
    ],
    "KI": [
        ("D_8015BDE4", 0x8019D734, 0x8019D754, 0x8019D80C, 4),
        ("D_8015BE18", 0x8019A30C, 0x8019A3BC, 0x8019A9B4, 18),
        ("D_8015BE1C", 0x8019BCCC, 0x8019BD3C, 0x8019C12C, 14),
        ("D_8015BE20", 0x8019CB24, 0x8019CB50, 0x8019CCB4, 5),
        ("D_8015BE24", 0x8019D03C, 0x8019D068, 0x8019D204, 5),
        ("D_8015BE28", 0x8019E3F4, 0x8019E414, 0x8019E4CC, 4),
    ],
    "HI": [
        ("D_8015BDE4", 0x8019CB24, 0x8019CB44, 0x8019CBE0, 4),
        ("D_8015BE18", 0x80199DDC, 0x80199E78, 0x8019A358, 18),
        ("D_8015BE1C", 0x8019B35C, 0x8019B3CC, 0x8019B714, 14),
        ("D_8015BE20", 0x8019C008, 0x8019C030, 0x8019C15C, 5),
        ("D_8015BE24", 0x8019C4D8, 0x8019C504, 0x8019C684, 5),
        ("D_8015BE28", 0x8019D584, 0x8019D5A4, 0x8019D624, 4),
    ],
    "KA": [
        ("D_8015BDE4", 0x8019CF08, 0x8019CF28, 0x8019CFE0, 4),
        ("D_8015BE18", 0x80199DFC, 0x80199E8C, 0x8019A2C4, 18),
        ("D_8015BE1C", 0x8019B59C, 0x8019B610, 0x8019BA1C, 14),
        ("D_8015BE20", 0x8019C3B4, 0x8019C3DC, 0x8019C508, 5),
        ("D_8015BE24", 0x8019C878, 0x8019C8A4, 0x8019CA24, 5),
        ("D_8015BE28", 0x8019DAE8, 0x8019DB08, 0x8019DB88, 4),
    ],
    "NI": [
        ("D_8015BDE4", 0x8019CEDC, 0x8019CEFC, 0x8019CFB4, 4),
        ("D_8015BE18", 0x80199E30, 0x80199EC0, 0x8019A2F8, 18),
        ("D_8015BE1C", 0x8019B570, 0x8019B5E8, 0x8019BA10, 14),
        ("D_8015BE20", 0x8019C374, 0x8019C39C, 0x8019C4C8, 5),
        ("D_8015BE24", 0x8019C864, 0x8019C890, 0x8019CA10, 5),
        ("D_8015BE28", 0x8019DA88, 0x8019DAA8, 0x8019DB44, 4),
    ],
    "KO": [
        ("D_8015BDE4", 0x8019D160, 0x8019D180, 0x8019D238, 4),
        ("D_8015BE18", 0x80199FBC, 0x8019A064, 0x8019A5EC, 18),
        ("D_8015BE1C", 0x8019B81C, 0x8019B88C, 0x8019BC7C, 14),
        ("D_8015BE20", 0x8019C5CC, 0x8019C5F4, 0x8019C720, 5),
        ("D_8015BE24", 0x8019CAC8, 0x8019CAF8, 0x8019CC94, 5),
        ("D_8015BE28", 0x8019DD74, 0x8019DD94, 0x8019DE14, 4),
    ],
    "KY": [
        ("D_8015BDE4", 0x8019CB18, 0x8019CB38, 0x8019CBD4, 4),
        ("D_8015BE18", 0x80199BF8, 0x80199C98, 0x8019A148, 20),
        ("D_8015BE1C", 0x8019B31C, 0x8019B390, 0x8019B79C, 14),
        ("D_8015BE20", 0x8019C08C, 0x8019C0B4, 0x8019C1E0, 5),
        ("D_8015BE24", 0x8019C508, 0x8019C534, 0x8019C6B4, 5),
        ("D_8015BE28", 0x8019D5CC, 0x8019D5EC, 0x8019D66C, 4),
    ],
    "KG": [
        ("D_8015BDE4", 0x8019D5B0, 0x8019D5D0, 0x8019D688, 4),
        ("D_8015BE18", 0x8019A0E8, 0x8019A17C, 0x8019A640, 18),
        ("D_8015BE1C", 0x8019BB18, 0x8019BB8C, 0x8019BF98, 14),
        ("D_8015BE20", 0x8019CA7C, 0x8019CAA4, 0x8019CBD0, 5),
        ("D_8015BE24", 0x8019CF40, 0x8019CF6C, 0x8019D0EC, 5),
        ("D_8015BE28", 0x8019E180, 0x8019E1A0, 0x8019E220, 4),
    ],
    "AS": [
        ("D_8015BDE4", 0x8019CA64, 0x8019CA84, 0x8019CB3C, 4),
        ("D_8015BE18", 0x80199BB0, 0x80199C40, 0x8019A078, 18),
        ("D_8015BE1C", 0x8019B198, 0x8019B208, 0x8019B614, 14),
        ("D_8015BE20", 0x8019BF1C, 0x8019BF44, 0x8019C070, 5),
        ("D_8015BE24", 0x8019C418, 0x8019C444, 0x8019C5C4, 5),
        ("D_8015BE28", 0x8019D58C, 0x8019D5AC, 0x8019D62C, 4),
    ],
    "MI": [
        ("D_8015BDE4", 0x8019CF54, 0x8019CF74, 0x8019D02C, 4),
        ("D_8015BE18", 0x80199E40, 0x80199EF4, 0x8019A578, 18),
        ("D_8015BE1C", 0x8019B728, 0x8019B79C, 0x8019BBA8, 14),
        ("D_8015BE20", 0x8019C49C, 0x8019C4C4, 0x8019C5F0, 5),
        ("D_8015BE24", 0x8019C8FC, 0x8019C928, 0x8019CAA8, 5),
        ("D_8015BE28", 0x8019DB48, 0x8019DB68, 0x8019DC20, 4),
    ],
    "SA": [
        ("D_8015BDE4", 0x8019CB64, 0x8019CB84, 0x8019CC3C, 4),
        ("D_8015BE18", 0x80199D10, 0x80199DA0, 0x8019A184, 18),
        ("D_8015BE1C", 0x8019B320, 0x8019B394, 0x8019B7A0, 14),
        ("D_8015BE20", 0x8019C0D8, 0x8019C100, 0x8019C22C, 5),
        ("D_8015BE24", 0x8019C518, 0x8019C544, 0x8019C6C4, 5),
        ("D_8015BE28", 0x8019D834, 0x8019D854, 0x8019D928, 4),
    ],
}

PROFILES = [
    {
        "id": "date-" + suffix.lower(),
        "path": "CDROM/SERIFU/DATE_" + suffix + ".BIN",
        "sha256": digest,
        "load_address": 0x80198000,
        "sections": [
            {
                "id": "section-%02d" % index,
                "text_table": text_table,
                "command_table": command_table,
                "voice_table": voice_table,
                "group_count": group_count,
            }
            for index, (text_table, command_table, voice_table, group_count)
            in enumerate(sections)
        ] + [
            {
                "id": "initializer-" + symbol,
                "text_table": text_table,
                "command_table": command_table,
                "voice_table": voice_table,
                "group_count": group_count,
            }
            for symbol, text_table, command_table, voice_table, group_count
            in _DATE_EXTRA[suffix]
        ],
        "evidence": [
            "src/ovl/DATE/80132000.c",
            "src/ovl/DATE/80141EE0.c",
        ],
    }
    for suffix, digest, sections in _DATE_FILES
]

# EVENT scene scripts selected by the DATE overlay. The event dispatch source
# selects these per girl (including the separate Miharu event); each section
# below is the matching D_8015E208/20C/210 assignment triple from its handler.
# Group counts are the equal contiguous pointer-table extents validated against
# all three tables and parse_script. EVENT_KO's final voice table has one extra
# in-bounds pointer beyond its parallel text/command tables; the latter bound
# the supported section at ten groups.
_EVENT_FILES = [
    ("FU", "5e586ed80d1dbc54a2658b283d415c6a5225e0726e1f67f8e42f8bd72e9fc396", [
        (0x80198A78, 0x80198AB4, 0x80198CE0, 6),
        (0x80198FFC, 0x80199028, 0x80199230, 4),
        (0x801994DC, 0x80199510, 0x801996CC, 6),
        (0x801998FC, 0x80199920, 0x80199AEC, 3),
    ], "8014F0F0.c"),
    ("KI", "c06133d6b7f5c208af4594cf3bfa3e6d6539b1dd98744c360b10bc25f9d6e543", [
        (0x801987A8, 0x801987E4, 0x801989A4, 7),
        (0x80198C40, 0x80198C60, 0x80198DBC, 3),
        (0x80198FA0, 0x80198FBC, 0x80199134, 3),
        (0x80199424, 0x80199464, 0x80199678, 7),
    ], "80154260.c"),
    ("HI", "74bd7975541d1a358a9c7cb8a189dab927dceed038c0c0584e81894683f29fe3", [
        (0x8019869C, 0x801986B8, 0x801987DC, 3),
        (0x801989DC, 0x801989FC, 0x80198BAC, 3),
        (0x80198DD0, 0x80198DF0, 0x80198F4C, 3),
        (0x8019915C, 0x80199180, 0x801992C4, 4),
    ], "80150010.c"),
    ("KA", "5308f57f9529146e82396e624be585a00d2f9542c7f58a872fc582c9fa1b33c6", [
        (0x8019889C, 0x801988D4, 0x80198A5C, 7),
        (0x80198D54, 0x80198D88, 0x80198F78, 5),
        (0x80199154, 0x80199178, 0x801992BC, 4),
        (0x801996D4, 0x80199714, 0x80199A20, 6),
    ], "80151460.c"),
    ("NI", "04c949e94c0f88072727378aaabbdeb77c0fbf4398dda42d3b56cb92dc1325e5", [
        (0x8019932C, 0x80199360, 0x80199664, 4),
        (0x80199A80, 0x80199ACC, 0x80199D90, 9),
        (0x80199F68, 0x80199F90, 0x8019A10C, 4),
        (0x8019A474, 0x8019A4B8, 0x8019A73C, 7),
    ], "801586A0.c"),
    ("KO", "97b744302a2dc823c300c1ddb049faa21ce73faf95c235634f7a4af327791882", [
        (0x80198C9C, 0x80198CBC, 0x80198E50, 3),
        (0x8019920C, 0x80199240, 0x80199484, 5),
        (0x801997C0, 0x801997E0, 0x801999C8, 3),
        (0x80199DC0, 0x80199E1C, 0x8019A0AC, 10),
    ], "801552B0.c"),
    ("KY", "fb8fac05a3a717d2054f783ca1fc4605f2e6bd24b35708521fa894d3fcace41e", [
        (0x80198C3C, 0x80198CAC, 0x801991DC, 10),
        (0x80199520, 0x8019955C, 0x801997C0, 6),
        (0x80199CA4, 0x80199CF0, 0x8019A0C0, 6),
        (0x8019A4D8, 0x8019A518, 0x8019A840, 6),
    ], "80156600.c"),
    ("KG", "d25348ecf4e3ee4d927da5127bcb5c6ce1d7047be644fcf35bcb622d2701804c", [
        (0x801986D0, 0x80198704, 0x801988BC, 5),
        (0x80198A44, 0x80198A6C, 0x80198B98, 5),
        (0x80198FF4, 0x80199040, 0x80199388, 7),
        (0x801995F8, 0x8019961C, 0x80199804, 3),
        (0x80199958, 0x8019997C, 0x80199A88, 4),
    ], "80152FC0.c"),
    ("AS", "cae1f9ff6de82a4a4775ef4de116f6e54862f2aa05c673cc78cb9856e418324c", [
        (0x801987BC, 0x801987E4, 0x8019897C, 4),
        (0x80198B9C, 0x80198BBC, 0x80198D34, 3),
        (0x80198E94, 0x80198EB8, 0x80198FFC, 4),
        (0x801991B8, 0x801991E0, 0x80199340, 4),
        (0x80199608, 0x80199640, 0x80199850, 6),
    ], "8014CC40.c"),
    ("MI", "82050013e7b7c3e7df23d71dbb82a93da924c39990d3ef360a01260a39e666ac", [
        (0x80198DBC, 0x80198DFC, 0x8019902C, 7),
        (0x80199540, 0x801995CC, 0x80199A18, 16),
        (0x80199BD8, 0x80199BF8, 0x80199D38, 3),
        (0x80199F04, 0x80199F2C, 0x8019A0E0, 4),
    ], "80157880.c"),
    ("SA", "343e2944f5bdbfe9621dd48032adb6dc691a577df31f1c36c168fecbcd84fba0", [
        (0x80199418, 0x8019943C, 0x801995D0, 3),
        (0x801998C8, 0x801998F4, 0x80199B34, 4),
        (0x80199EF4, 0x80199F24, 0x8019A180, 4),
        (0x8019A58C, 0x8019A5C8, 0x8019A848, 6),
    ], "801594B0.c"),
    ("OT", "614245bd4d450bf97261bc3f39499fc966c7ad202a30f7fe34379496d49b535a", [
        (0x8019A0E0, 0x8019A0F8, 0x8019A174, 3),
    ], "8015A190.c"),
]

# The initializer in 8014B1F0 assigns 79 parallel pointer-table triples. The
# handler files above consume 47 of them directly; these additional triples
# also form bounded scripts in the one EVENT container into which their tables
# resolve and are retained as initializer-indexed roots.
_EVENT_EXTRA = {
    "FU": [
        (6, 0x80198390, 0x801983C0, 0x8019861C, 4),
        (7, 0x801986F4, 0x8019870C, 0x801987A4, 3),
        (12, 0x80199D78, 0x80199DB8, 0x8019A020, 7),
        (13, 0x8019A584, 0x8019A5D8, 0x8019AA8C, 7),
    ],
    "KI": [(37, 0x80198308, 0x80198330, 0x8019851C, 4)],
    "HI": [
        (14, 0x801982E8, 0x80198314, 0x801984E8, 5),
        (19, 0x80199604, 0x80199644, 0x80199890, 7),
        (20, 0x80199B18, 0x80199B38, 0x80199CCC, 3),
    ],
    "KA": [
        (24, 0x801983D0, 0x801983FC, 0x8019863C, 4),
        (29, 0x80199D8C, 0x80199DF0, 0x8019A050, 12),
    ],
    "NI": [
        (61, 0x801987E8, 0x80198850, 0x80198E04, 8),
        (66, 0x8019ABDC, 0x8019AC40, 0x8019AFE8, 10),
    ],
    "KO": [
        (42, 0x801983B8, 0x801983E0, 0x801985E4, 3),
        (43, 0x8019881C, 0x8019883C, 0x80198998, 3),
        (48, 0x8019A30C, 0x8019A344, 0x8019A500, 6),
    ],
    "KY": [(49, 0x80198374, 0x801983A0, 0x801985A8, 4)],
    "KG": [
        (30, 0x8019829C, 0x801982D0, 0x80198470, 6),
        (36, 0x80199D60, 0x80199DB4, 0x8019A044, 10),
    ],
    "AS": [(0, 0x80198304, 0x80198340, 0x801985A4, 6)],
    "MI": [
        (54, 0x80198418, 0x8019845C, 0x80198784, 6),
        (55, 0x801989A4, 0x801989BC, 0x80198B14, 2),
        (60, 0x8019A37C, 0x8019A3B8, 0x8019A5CC, 7),
    ],
    "SA": [
        (67, 0x80198784, 0x801987F8, 0x80198CF4, 11),
        (68, 0x80198FB4, 0x80198FE0, 0x801991E8, 4),
        (73, 0x8019AB0C, 0x8019AB4C, 0x8019ADD0, 7),
    ],
    "OT": [
        (21, 0x8019894C, 0x801989C4, 0x80198F14, 11),
        (22, 0x80199068, 0x80199098, 0x80199190, 6),
        (23, 0x801994E4, 0x80199524, 0x801997F8, 6),
        (74, 0x80199B14, 0x80199B6C, 0x80199DE4, 11),
        (75, 0x80199F30, 0x80199F50, 0x8019A05C, 4),
        (77, 0x8019A628, 0x8019A690, 0x8019AB34, 10),
        (78, 0x8019AFF8, 0x8019B06C, 0x8019B494, 14),
    ],
}

PROFILES.extend(
    {
        "id": "event-" + suffix.lower(),
        "path": "CDROM/SERIFU/EVENT_" + suffix + ".BIN",
        "sha256": digest,
        "load_address": 0x80198000,
        "sections": [
            {
                "id": "scene-%02d" % index,
                "text_table": text_table,
                "command_table": command_table,
                "voice_table": voice_table,
                "group_count": group_count,
            }
            for index, (text_table, command_table, voice_table, group_count)
            in enumerate(sections)
        ] + [
            {
                "id": "initializer-%02d" % initializer_index,
                "text_table": text_table,
                "command_table": command_table,
                "voice_table": voice_table,
                "group_count": group_count,
            }
            for initializer_index, text_table, command_table, voice_table, group_count
            in _EVENT_EXTRA.get(suffix, [])
        ],
        "evidence": [
            "src/ovl/DATE/8014B1F0.c",
            "src/ovl/DATE/" + source,
        ],
    }
    for suffix, digest, sections, source in _EVENT_FILES
)
