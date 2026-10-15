typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct RECT {
               s16 x;
               s16 y;
               s16 w;
               s16 h;
} RECT;
typedef union WordBytes4 {
    u32 word;
    u8 bytes[4];
} WordBytes4;
extern WordBytes4 D_801255D0;
extern s16 D_80121428;
typedef struct DialogueVoice8 {
               s32 unk_00;
               s32 unk_04;
} DialogueVoice8;
typedef struct DialogueBytes16 {
               u8 value[16];
} DialogueBytes16;
typedef struct DialogueWork {
               s32 unk_00;
               s32 unk_04;
               s32 unk_08;
               s32 unk_0C;
               s32 unk_10;
               s32 unk_14;
               s32 unk_18;
               s32 unk_1C;
               s32 unk_20;
               s32 unk_24;
               s32 unk_28;
               s32 unk_2C;
               s32 unk_30;
               s32 unk_34;
               s32 unk_38;
               s32 unk_3C;
               s32 unk_40;
               s32 unk_44;
               s32 unk_48;
               s32 unk_4C;
               s32 unk_50;
               s32 unk_54;
               s32 unk_58;
               s32 unk_5C;
               s32 unk_60;
               s32 unk_64;
               s32 unk_68;
               s32 unk_6C;
               s32 unk_70;
               s32 unk_74;
               s32 unk_78;
} DialogueWork;
typedef struct DialogueTables {
               s16 *group;
               s16 *line;
               u8 ***text;
               u8 **command;
               DialogueVoice8 ***voice;
} DialogueTables;
typedef struct Rec34Flags {
    u32 pad : 12;
    u32 b12 : 1;
    u32 b13 : 1;
    u32 b14 : 1;
    u32 rest : 17;
} Rec34Flags;
typedef struct Rec34 {
               s16 unk_00;
               s16 unk_02;
               s16 unk_04;
               s16 unk_06;
               s16 unk_08;
               s16 unk_0A;
               Rec34Flags unk_0C;
               u8 pad10[7];
               u8 unk_17;
               u8 pad18[0xA];
               u8 unk_22;
               u8 pad23[4];
               u8 unk_27;
               u8 pad28[5];
               u8 unk_2D;
               u8 pad2E[6];
} Rec34;
typedef union GsWord {
               s32 w;
               u32 u;
               u16 h[2];
               u8 b[4];
} GsWord;
typedef union GsHalf {
               u16 h;
               u8 b[2];
} GsHalf;
typedef struct FlagsWork24 {
               s32 unk_00;
               u8 unk_04[0x10];
               u8 flags[14];
               u8 pad22[2];
} FlagsWork24;
typedef struct CharFlags {
               u32 b0 : 1;
               u32 b1 : 1;
               u32 b2 : 1;
               u32 b3 : 1;
               u32 b4 : 1;
               u32 b5 : 1;
               u32 b6 : 1;
               u32 b7 : 1;
               u32 b8 : 1;
               u32 f9 : 3;
               u32 b12 : 1;
               u32 b13 : 1;
               u32 b14 : 1;
               u32 rest : 17;
} CharFlags;
typedef union Rec38Flags {
               s32 w;
               u32 u;
               u16 h[2];
               u8 b[4];
               CharFlags f;
} Rec38Flags;
typedef struct Rec38 {
               s16 unk_00;
               s16 unk_02;
               s16 unk_04;
               s16 unk_06;
               s16 unk_08;
               s16 unk_0A;
               Rec38Flags unk_0C;
               GsWord unk_10;
               u8 unk_14[0x24];
} Rec38;
typedef struct Rec24 {
               s32 unk_00;
               s16 unk_04;
               s16 unk_06;
               s16 unk_08;
               u16 unk_0A;
               s16 unk_0C;
               u8 unk_0E;
               u8 unk_0F;
               s16 unk_10;
               s16 unk_12;
               u8 unk_14;
               u8 unk_15;
               u8 unk_16;
               u8 pad17;
               s16 unk_18;
               s16 unk_1A;
               s16 unk_1C;
               s16 unk_1E;
               s32 unk_20;
} Rec24;
typedef struct Work80125D10 {
               u32 unk_00;
               s32 unk_04;
               s32 unk_08;
               s32 unk_0C;
               s32 unk_10;
               s32 unk_14;
               s32 unk_18;
               s16 unk_1C;
               s16 unk_1E;
               s16 unk_20;
               s16 unk_22;
               u16 unk_24;
               s16 unk_26;
               s16 unk_28;
               s16 unk_2A;
               s16 unk_2C;
               s16 unk_2E;
               s16 unk_30;
               s16 unk_32;
               u16 unk_34;
               u16 unk_36;
               u16 unk_38;
               s16 unk_3A;
               s16 unk_3C;
               s16 unk_3E;
               u16 unk_40;
               u16 unk_42;
               u8 unk_44[4];
               u8 unk_48[8];
} Work80125D10;
typedef struct GsRec01C {
               s32 unk_00;
               s32 unk_04;
               s32 unk_08;
} GsRec01C;
typedef struct GsRec044 {
               u8 unk_00[0x20];
               u8 unk_20[0x20];
} GsRec044;
typedef struct GsRec0FC {
               s16 unk_00;
               s16 unk_02;
} GsRec0FC;
typedef struct GsRec66C {
               u16 unk_00;
               u8 unk_02;
               u8 unk_03;
} GsRec66C;
typedef struct GsRec69C {
               u8 unk_00;
               u8 unk_01;
               u8 unk_02;
               u8 unk_03;
} GsRec69C;
typedef struct GsRec75E {
               u8 unk_00;
               u8 unk_01;
               u8 unk_02;
               u8 unk_03;
               u8 unk_04;
               u8 unk_05;
               u8 unk_06;
               u8 unk_07;
} GsRec75E;
typedef struct GsRecF90 {
               s16 unk_00;
               s16 unk_02;
               u8 unk_04;
               u8 unk_05;
               u8 unk_06;
               u8 unk_07;
} GsRecF90;
typedef struct GsRec1328 {
               s32 unk_00;
               u8 unk_04;
               u8 unk_05;
               u8 unk_06;
               u8 unk_07;
} GsRec1328;
typedef struct GsRec142C {
               s32 unk_00;
               u8 unk_04;
               u8 unk_05;
               u8 unk_06;
               u8 unk_07;
               u16 unk_08;
               u16 unk_0A;
               u16 unk_0C;
               u16 unk_0E;
} GsRec142C;
typedef struct GameState {
                u16 unk_000;
                u16 unk_002;
                u16 unk_004;
                u8 unk_006[0xE];
                s16 unk_014[2];
                s16 unk_018[2];
                GsRec01C unk_01C[2];
                u8 unk_034;
                u8 unk_035;
                u8 unk_036;
                u8 unk_037;
                u8 unk_038;
                u8 unk_039;
                u8 unk_03A;
                u8 unk_03B;
                s8 unk_03C;
                u8 unk_03D;
                u8 unk_03E;
                u8 unk_03F;
                u8 unk_040;
                u8 unk_041;
                u16 unk_042;
                GsRec044 unk_044[2];
                u8 unk_0C4[0x10];
                u8 unk_0D4[8];
                u8 unk_0DC[8];
                u8 unk_0E4[0x10];
                GsHalf unk_0F4;
                GsHalf unk_0F6;
                s32 unk_0F8;
                GsRec0FC unk_0FC;
                GsRec0FC unk_100;
                GsRec0FC unk_104;
                GsRec0FC unk_108;
                GsRec0FC unk_10C;
                GsRec0FC unk_110;
                GsRec0FC unk_114;
                GsRec0FC unk_118;
                GsRec0FC unk_11C;
                u8 unk_120[0x9C];
                Rec38 unk_1BC[16];
                u8 unk_53C[0x10];
                GsWord unk_54C[8];
                u8 unk_56C[0xFC];
                u8 unk_668[4];
                GsRec66C unk_66C[12];
                GsRec69C unk_69C[32];
                u8 unk_71C;
                u8 unk_71D;
                u8 unk_71E;
                u8 unk_71F;
                u8 unk_720;
                u8 unk_721;
                u8 unk_722;
                u8 unk_723;
                u8 unk_724;
                u8 unk_725;
                u8 unk_726;
                u8 unk_727;
                u8 unk_728[4];
                u8 unk_72C[0x10];
                u8 unk_73C[0x10];
                u8 unk_74C[0x10];
                u8 unk_75C;
                u8 unk_75D;
                GsRec75E unk_75E[256];
                u8 unk_F5E;
                u8 unk_F5F;
                s8 unk_F60;
                s8 unk_F61;
                s8 unk_F62;
                s8 unk_F63;
                u8 unk_F64[4];
                GsWord unk_F68;
                s8 unk_F6C;
                s8 unk_F6D;
                s8 unk_F6E;
                u8 unk_F6F;
                s8 unk_F70;
                s8 unk_F71;
                u8 unk_F72;
                s8 unk_F73;
                u8 unk_F74;
                u8 unk_F75;
                u8 unk_F76;
                u8 unk_F77;
                u8 unk_F78;
                u8 unk_F79;
                s16 unk_F7A;
                s16 unk_F7C;
                u8 unk_F7E[2];
                s32 unk_F80;
                s32 unk_F84;
                s32 unk_F88;
                s32 unk_F8C;
                GsRecF90 unk_F90[32];
                 u8 unk_1090;
                 u8 unk_1091;
                 u8 unk_1092;
                 s8 unk_1093;
                 s8 unk_1094;
                 s8 unk_1095[0xD];
                 s8 unk_10A2;
                 u8 unk_10A3;
                 u8 unk_10A4;
                 u8 unk_10A5[0x43];
                 s32 unk_10E8;
                 u8 unk_10EC[4];
                 s32 unk_10F0;
                 s32 unk_10F4;
                 u32 unk_10F8;
                 s32 unk_10FC;
                 s32 unk_1100;
                 GsWord unk_1104;
                 u8 unk_1108;
                 u8 unk_1109;
                 u8 unk_110A;
                 s8 unk_110B;
                 s8 unk_110C;
                 u8 unk_110D;
                 u8 unk_110E;
                 u8 unk_110F;
                 u8 unk_1110;
                 u8 unk_1111;
                 u8 unk_1112;
                 u8 unk_1113;
                 u8 unk_1114;
                 u8 unk_1115;
                 u8 unk_1116;
                 u8 unk_1117;
                 u8 unk_1118;
                 u8 unk_1119;
                 s8 unk_111A;
                 u8 unk_111B;
                 u8 unk_111C;
                 u8 unk_111D;
                 u8 unk_111E[2];
                 s32 unk_1120;
                 u8 unk_1124;
                 u8 unk_1125[3];
                 s32 unk_1128[64];
                 s32 unk_1228[64];
                 GsRec1328 unk_1328[32];
                 u8 unk_1428;
                 u8 unk_1429;
                 u8 unk_142A[2];
                 GsRec142C unk_142C[33];
                 s32 unk_163C[128];
                 u8 unk_183C[0x200];
                 s32 unk_1A3C;
                 u8 unk_1A40[0xC];
                 s32 unk_1A4C;
                 u8 unk_1A50[0xC];
                 s32 unk_1A5C;
                 u8 unk_1A60[0xC];
                 s32 unk_1A6C;
                 u8 unk_1A70[0xC];
                 s32 unk_1A7C;
                 u8 unk_1A80[4];
                 s32 unk_1A84;
                 u8 unk_1A88[8];
} GameState;
extern s32 D_8007E7D0[];
extern s32 D_8007E810[];
extern s32 D_8007E850[];
extern s8 D_8008093C;
extern s32 D_80094710;
extern u16 D_80094714;
extern u16 D_80094718;
extern u8 *D_8009471C;
extern u8 *D_80094720;
extern s32 D_80094724;
extern s32 D_80094728;
extern s32 D_8009472C;
extern s16 D_80094730;
extern s16 D_80094734;
extern s32 D_8009473C;
extern s32 D_80094740;
extern s32 D_80094744;
extern u8 D_80094748[];
extern u8 D_80094764[];
extern u8 D_8009476C[];
extern u8 D_80094784[];
extern u8 D_800947C4[];
extern u8 D_80094818[];
extern u8 D_8009483C[];
extern s16 D_80094868;
extern u8 D_800AFDF0[];
extern u8 D_800AFF6C[];
extern u8 D_800AFF78[];
extern u8 D_800B00FC[];
extern u8 D_800B0194[];
extern u8 D_800B0860[];
extern u8 D_800B0896;
extern u8 D_800B0897;
extern u8 D_800B0898;
extern u8 D_800B0929[];
extern u32 D_800B0940;
extern s32 D_800B0948;
extern s16 D_800B094E;
extern s16 D_800B0956;
extern s16 D_800B095E;
extern s16 D_800B0966;
extern s16 D_800B096E;
extern Rec34 D_800B0A04[];
extern u8 D_800B0A14;
extern u8 D_800B0AFF;
extern s16 D_800B0B0E[];
extern u8 D_800B0B64[];
extern u8 D_800B0B6C[];
extern s16 D_800B0B76;
extern u8 D_800B0BCE;
extern u8 D_800B0BCF;
extern s8 D_800B0E31;
extern u8 D_800B0E42;
extern u8 D_800B0E43;
extern u8 D_800B0E6A;
extern u8 D_800B0F2C;
extern u8 D_800B0F2D;
extern u8 D_800B0F2E;
extern u8 D_800B0F2F;
extern u8 D_800B0F30;
extern u8 D_800B0F35[];
extern u8 D_800B1746;
extern s32 D_800B1764;
extern u8 D_800B188D[];
extern u32 D_800B1AE4;
extern s32 D_800B1AF0;
extern u8 D_800B1AF5;
extern u8 D_800B1AF6;
extern u8 D_800B1AF9;
extern s32 D_800B1C1C;
extern s32 D_800B1C70;
extern s32 D_800B1CF0;
extern u8 D_800B3220;
extern u8 *D_800B3288[];
extern u8 **D_800B32CC;
extern s32 D_800B3538[];
extern u8 *girl_short_name_table[];
extern u8 *D_800B3624;
extern u8 *D_800B3630;
extern u8 D_800B3644[3];
extern s32 D_800B3688[];
extern s32 D_800B36AC;
extern s32 D_800B36C8[];
extern s32 D_800B36EC;
extern s32 D_800B3708[];
extern s32 D_800B372C;
extern s32 **D_800B3748;
extern void *D_800B374C;
extern u8 D_800B3750[][3];
extern s32 D_800B37D4[];
extern u8 *club_name_table[];
extern s32 D_800B394C[];
extern s32 D_800B3A3C[];
extern s32 D_800B3B9C[];
extern u8 D_800B3C6C;
extern u8 D_800B3C70[];
extern u8 D_800B3C88[];
extern s8 D_800B3CA0;
extern u8 D_800B3CFC;
extern u8 D_800B3D24;
extern u8 D_800B3D40;
extern u8 D_800B3D44;
extern u8 D_800B3D48;
extern u8 D_800B3D54[];
extern u8 D_800B3D60;
extern s32 D_800B3D68;
extern s32 D_800B3D6C;
extern s32 D_800B3D70[4];
extern s32 D_800B3648[16];
extern u8 D_800B3D80;
extern u8 D_800B3D84[];
extern s8 D_800B3DB0;
extern u8 text_list[];
extern u8 D_800B3DC7[];
extern u8 D_800B3F58[];
typedef struct KWork {
              s16 count;
              s16 total;
              s16 mark;
              u8 speed;
              u8 pad_07;
              s16 pos;
              u8 unk_6A;
              u8 pad_0B;
} KWork;
extern KWork text_list_count;
extern s16 D_800B40F0[4];
extern u8 D_800B4100[13];
extern u8 D_800B4110[13];
extern u8 D_800B4120[13];
extern s8 D_800B4130[13][8];
extern u8 D_800B4198[7];
extern u8 D_800B41A0[];
extern u8 D_800B41B0[20][20];
extern u8 D_800B4341[];
extern s8 D_800B4349[11];
extern u8 D_800B4355[10];
extern u8 D_800B4361[10];
extern s8 D_800B436C[15];
extern s8 D_800B437C[15];
extern s32 D_800B58E4;
extern s32 D_800B58F8;
extern s32 D_800B58FC;
extern s32 D_800B5900;
extern s32 D_800B5920[4];
extern u8 D_800B5938[];
extern u8 D_800B5939;
extern u8 D_800B593C;
extern u8 D_800B5940;
extern s32 D_800B5944;
extern s32 D_800B5948;
extern s32 D_800B594C;
extern s32 D_800B5950[];
extern s32 D_800B5960[];
extern s16 D_800B59F0;
extern s16 D_800B59F4;
extern u8 D_800B5A60;
extern u8 D_800B5A64;
extern s16 D_800B5B24;
extern u32 D_800B5B28[4][8];
extern u32 D_800B5BA8[8];
extern u8 D_800B5BC8;
extern u8 D_800B5BCC;
extern u8 D_800B5BD0;
extern s8 D_800B5BD4;
extern s32 D_800B5BD8[];
extern s32 D_800B5BE8[];
extern s32 D_800B5BF8[];
extern s16 D_800B5C08;
extern u8 D_800B5C10[];
extern u8 D_800B5C2C[][17];
extern u8 D_800B5D0C[];
extern u8 D_800B5D1C[][17];
extern u8 D_800B5DFC[];
extern u8 D_800B5E0C[];
extern u8 D_800B5E25[];
extern u8 D_800B5E3E[];
extern u8 D_800B5E57[];
extern u8 D_800B5E70[];
extern u8 D_800B5E89[];
extern u8 D_800B5EA4[][3][36];
extern s32 D_800B66A4[];
extern s16 D_800B66E0[];
extern s16 D_800B66F4[];
extern u8 D_800B6708;
extern u8 D_800B6724[];
extern u8 D_800B6728[];
extern u8 D_800B672C[];
extern u8 D_800B6730[];
extern s8 D_800B6740;
extern s32 D_800B6748;
extern s32 D_800B674C;
extern s32 D_800B6750;
extern s32 D_800B6754;
extern s32 D_800B6758;
extern s32 D_800B6760[];
extern s32 D_800B6780[];
extern s32 D_800B6810[];
extern s32 D_800B69E0[];
extern u8 D_800B6D30;
extern u8 ovl_load_state;
extern u8 ovl_load_sectors;
extern s32 ovl_load_buf;
extern s32 ovl_load_lba;
extern u8 font_glyphs[];
extern s8 D_800C51C4;
extern s32 D_800C5FCC;
extern s32 D_800C5FD0;
extern u8 D_800C6220[];
extern u8 D_800C9730[];
extern s32 D_800C9734;
extern s32 D_800C9738;
extern s32 D_800C973C;
extern s32 D_800C974C;
extern s32 D_800C9750;
extern s32 D_800C9754;
extern s32 D_800C9758;
extern u8 sprite_frame_table[];
extern u8 D_800C9A14[];
extern s16 D_800C9A54;
extern u8 D_800C9A60[];
extern u8 D_800C9D60[];
extern u16 D_800C9F60[];
extern u16 D_800C9FF8[];
extern u16 D_800CA048[];
extern u16 D_800CA108[];
extern s32 D_800CA120;
extern s32 D_800CA124;
extern s32 D_800CA128;
extern s32 D_800CA130;
extern u8 *D_800CA134;
extern u8 *D_800CA138;
extern s32 D_800CA13C;
extern s32 D_800CA140;
extern s32 D_800CA144;
extern s16 D_800CA148;
extern s16 D_800CA14C;
extern s16 D_800CA150;
extern s16 D_800CA154;
extern u8 D_800CA158;
extern u8 D_800CA15C;
extern s32 D_800CA160;
extern s32 D_800CA164;
extern s32 D_800CA168;
extern u8 D_800CA16C[];
extern u8 D_800CA174[];
extern u8 D_800CA17C[];
extern u8 D_800CA188[];
extern u8 girl_name_buf[];
extern u8 D_800CA19C[];
extern u8 D_800CA1DC[];
extern s16 D_800CA21C[4];
extern s16 D_800CA21E;
extern s16 D_800CA224[4];
extern s16 D_800CA22C[4];
extern s16 D_800CA234[4];
extern s16 D_800CA236;
extern u8 D_800CA23C[];
extern u8 D_800CA250[];
extern u8 D_800CA25C[];
extern u8 D_800CA2A4[4];
extern u8 D_800CA2A5;
extern u8 D_800CA2A6;
extern u8 D_800CA2A8;
extern s16 D_800CA2AC;
extern u8 D_800CA2B0[];
extern s16 D_800CA2C0;
extern u8 D_800CA2C4;
extern s32 D_800CA2C8;
extern u8 D_800CA2CC;
extern s32 D_800CA2D0;
extern s32 D_800CA2D4;
extern s32 D_800CA2D8;
extern s16 D_800CA2DC;
extern s16 D_800CA2E0;
extern s16 D_800CA2E4;
extern s16 D_800CA2E8;
extern u8 D_800CA2EC;
extern s8 D_800CA2F0;
extern u8 D_800CA2F4;
extern u8 D_800CA2F8;
extern u8 D_800CA2FC;
extern s16 D_800CA300[12];
extern s16 D_800CA318[12];
extern s16 D_800CA348[12];
extern u8 D_800CA360;
extern s8 D_800CA364;
extern u8 D_800CA368;
extern DialogueBytes16 D_800CA36C;
extern u8 *D_800D9234;
extern u8 *D_800D9238;
extern s32 D_800D923C;
extern s32 D_800D9240;
extern s32 D_800D9244;
extern s16 D_800D9248;
extern s16 D_800D924C;
extern s16 D_800D9250;
extern s16 D_800D9254;
extern s32 D_800D9258;
extern s32 D_800D925C;
extern s32 D_800D9260;
extern u8 D_800D9280;
extern u8 D_800D9288;
extern s32 D_800D92A0;
extern s32 D_800D92E0;
extern u8 D_800D9328[];
extern u8 D_800D9334[];
extern u8 D_800D9388[];
extern s32 D_800DE500[];
extern s8 *D_800E36C0[];
extern s32 D_800E36C8[];
extern s32 D_800E36D0[];
extern s32 D_800E36D8;
extern s32 D_800E36DC;
extern s32 D_800E36E0;
extern s32 D_800E36E4;
extern u16 D_800E36E8;
extern u16 D_800E36EA;
extern s32 D_800E36F0;
extern s32 D_800E36F4;
extern GameState D_800E6280;
extern s32 D_800E7D10;
extern u8 D_800E7D11[];
extern u8 D_800E7D13[];
extern u8 D_800E7D14[];
extern s8 D_800E7D15;
extern s8 D_800E7D16;
extern u8 D_800E7D1F;
extern u8 D_800E7D2C;
extern u8 D_800E7D34;
extern u8 D_800E7D36;
extern u16 D_800E7D3C;
extern u16 D_800E7D3E;
extern u16 D_800E7D40;
extern u8 D_800E7D42;
extern u8 D_800E7D43;
extern u8 D_800E7D44;
extern u8 D_800E7D45;
extern u8 D_800E7D46;
extern u8 D_800E7D47;
extern u8 D_800E7D48;
extern u8 D_800E7D49;
extern u8 D_800E7D4A;
extern u8 D_800E7D4B;
extern s32 D_800E7D4C;
extern u8 D_800E7D4F;
extern s32 D_800E7D50;
extern u8 D_800E7D68;
extern s8 D_800E8BEE;
extern u8 D_800E8BEF;
extern u8 D_800E8BF0[];
extern u8 D_800E8C30[];
extern s32 D_800E8C70;
extern u8 *D_800E8C74;
extern s32 D_800E8C84;
extern u8 *D_800E8C88;
extern u8 D_800E8CA0[];
extern u8 D_800E90A0[];
extern u8 D_800E9620[];
extern u8 D_800E96AB;
extern s8 D_800E96EF;
extern s8 D_800E9733;
extern u8 D_800E9E5F;
extern u8 D_800E9E63;
extern s16 D_800E9E74;
extern u8 D_800EAFA0[];
extern s8 D_800EAFA2;
extern u8 D_800EAFA7;
extern s16 D_800EAFB6;
extern s16 D_800EAFB8;
extern s32 D_800EAFD8;
extern u8 D_800EAFEB;
extern s8 D_800EB029;
extern u8 D_800EB02A;
extern u8 D_800EB02B;
extern s8 D_800EB02C;
extern s8 D_800EB02D;
extern s8 D_800EB02E;
extern u8 D_800EB02F;
extern s16 D_800EB030;
extern s32 D_800EB034;
extern s32 D_800EB038;
extern s16 D_800EB03C;
extern s16 D_800EB03E;
extern s16 D_800EB040;
extern s16 D_800EB04E;
extern s16 D_800EB052;
extern s32 D_800EB05C;
extern s32 D_800EB060;
extern s8 D_800EB06B;
extern s8 D_800EB06D;
extern s8 D_800EB06E;
extern u8 D_800EB06F;
extern s8 D_800EB070;
extern s8 D_800EB071;
extern s8 D_800EB072;
extern u8 D_800EB073;
extern s32 D_800EB078;
extern s32 D_800EB07C;
extern s16 D_800EB080;
extern s16 D_800EB082;
extern s16 D_800EB084;
extern s16 D_800EB092;
extern s16 D_800EB096;
extern s32 D_800EB0A0;
extern s32 D_800EB0A4;
extern s8 D_800EB0AF;
extern u8 D_800EC190[];
extern s32 D_800EC2F8;
extern s16 D_800EC2FC;
extern s16 D_800EC2FE;
extern s16 D_800EC300;
extern s16 D_800EC302;
extern s16 D_800EC304;
extern s8 D_800EC306;
extern s8 D_800EC307;
extern s16 D_800EC308;
extern s16 D_800EC30A;
extern u8 D_800EC30C;
extern u8 D_800EC30D;
extern u8 D_800EC30E;
extern s16 D_800EC310;
extern s16 D_800EC312;
extern s16 D_800EC314;
extern s16 D_800EC316;
extern s32 D_800EC318;
extern s32 D_800EECB0;
extern s32 D_800EECB4;
extern s32 D_800EECBC;
extern s32 D_800EECC0;
extern s32 D_800EECC8;
extern s32 D_800EECCC;
extern s32 D_800EECD0;
extern s32 D_800EECD4;
extern s32 D_800EECE0;
extern s32 D_800EECE4;
extern s32 D_800EECE8;
extern s32 D_800EECEC;
extern s32 D_800EECF0;
extern u8 D_800F19AB;
extern u8 D_800F53DA;
extern u8 D_800F53DE;
extern u8 D_800F53DF;
extern u8 D_800F53E0;
extern u32 D_800F5488;
extern s32 D_800F5490;
extern s16 D_800F5496;
extern s16 D_800F549A;
extern s16 D_800F54A6;
extern s16 D_800F54AE;
extern s16 D_800F54B2;
extern s16 D_800F5556;
extern u8 D_800F55CA;
extern u8 D_800F5602;
extern u32 D_800F5638;
extern u8 D_800F563A;
extern s16 D_800F56DE;
extern s32 D_800F56E0;
extern s16 D_800F574E;
extern s32 D_800F5750;
extern u8 D_800F5833;
extern u8 D_800F5902;
extern u8 D_800F592F;
extern u8 D_800F593D;
extern u8 D_800F594E;
extern u8 D_800F594F;
extern u8 D_800F5A2D[];
extern u8 D_800F5AAC;
extern u8 D_800F5AAE;
extern u8 D_800F5AB1;
extern u8 D_800F5AB2;
extern u8 D_800F5ACD;
extern u8 D_800F62CF;
extern s8 D_800F6412;
extern s32 D_800F6458;
extern s32 D_800F6468;
extern s32 D_800F6474;
extern u8 D_800F6479;
extern u8 D_800F647A;
extern s32 D_800F65C0;
extern s32 D_800F6600;
extern s32 D_800F6680;
extern u8 D_800F7710[];
extern s32 D_8011ECA0;
extern s8 D_8011ECA4;
extern s32 D_8011ECA8;
extern s32 D_8011ECAC;
extern s32 D_8011ECB0;
extern s32 D_8011ECB4;
extern s32 D_8011ECB8;
extern s32 D_8011ECBC;
extern s32 D_8011ECC0;
extern s32 D_8011ECC4;
extern s32 D_8011ECC8;
extern u8 D_8011ECD0[];
extern u8 D_8011ECD3;
extern s16 D_8011ECE8;
extern s32 D_8011ECF4;
extern s16 D_8011ECF6;
extern s32 D_8011ECF8;
extern s16 D_8011ECFA;
extern u8 D_8011ED15;
extern u8 D_8011ED16;
extern u8 D_8011ED17;
extern u8 D_8011ED18;
extern u8 D_8011ED19;
extern u8 D_8011ED1B;
extern u8 *D_8011ED20;
extern u8 *D_8011ED24;
extern s16 D_8011ED28;
extern s16 D_8011ED2C;
extern s16 D_8011ED3A;
extern s16 D_8011ED3E;
extern u8 *D_8011ED48;
extern s32 D_8011ED4C;
extern u8 D_8011ED57;
extern u8 D_8011ED58[];
extern u8 D_8011ED59;
extern u8 D_8011ED5A;
extern u8 D_8011ED5B;
extern u8 D_8011ED5C;
extern u8 D_8011ED5D;
extern u8 *D_8011ED64;
extern u8 *D_8011ED68;
extern s16 D_8011ED70;
extern s16 D_8011ED7E;
extern s16 D_8011ED82;
extern u8 *D_8011ED8C;
extern s32 D_8011ED90;
extern u8 D_8011ED9C[];
extern s8 D_8011ED9F;
extern s8 D_8011EDE3;
extern u8 D_8011EFBC[];
extern u8 D_8011F044[];
extern u8 D_8011F0CD;
extern u8 D_8011F0CE;
extern u8 D_8011F0CF;
extern u8 D_8011F0D0;
extern u8 D_8011F0D1;
extern u8 *D_8011F0D8;
extern u8 *D_8011F0DC;
extern s16 D_8011F0E0;
extern s16 D_8011F0E2;
extern s16 D_8011F0E4;
extern s16 D_8011F0F2;
extern s16 D_8011F0F6;
extern u8 *D_8011F100;
extern s32 D_8011F104;
extern u8 D_8011F10F;
extern u8 D_8011F113;
extern u8 D_8011F3FF[];
extern u8 D_8011F414;
extern s32 D_8011F41C;
extern s16 D_8011F422;
extern s16 D_8011F426;
extern s16 D_8011F458;
extern s16 D_8011F466;
extern s16 D_8011F46A;
extern u8 D_8011F4CA;
extern u8 D_8011F4CB;
extern u8 D_8011F4CF;
extern s16 D_8011F4D0;
extern s16 D_8011F4DE;
extern s16 D_8011F4E0;
extern s16 D_8011F4EE;
extern s16 D_8011F4F2;
extern s8 D_8011F50D;
extern u8 D_8011F50E;
extern u8 D_8011F50F;
extern u8 D_8011F513;
extern s16 D_8011F522;
extern s16 D_8011F524;
extern s16 D_8011F532;
extern s16 D_8011F536;
extern u8 D_8011F553;
extern s16 D_8011FDFA;
extern s16 D_8011FE3E;
extern s16 D_8011FE82;
extern s16 D_8011FEC6;
extern s16 D_8011FF0A;
extern s16 D_8011FF4E;
extern s16 D_8011FF92;
extern s16 D_8011FFD6;
extern s16 D_8012001A;
extern s16 D_8012005E;
extern s16 D_801200A2;
extern s16 D_801200E6;
extern s16 D_8012012A;
extern s16 D_8012016E;
extern s16 D_801201B2;
extern s16 D_801201F6;
extern s16 D_8012023A;
extern s16 D_8012027E;
extern s16 D_801202C2;
extern s16 D_80120306;
extern s16 D_8012034A;
extern s16 D_8012038E;
extern s16 D_801203D2;
extern s16 D_80120416;
extern s16 D_8012045A;
extern s16 D_8012049E;
extern s16 D_801204E2;
extern s16 D_80120526;
extern s16 D_8012056A;
extern s16 D_8012059A[];
extern s16 D_801205AE;
extern s16 D_801205F2;
extern s16 D_80120636;
extern u8 D_80120650[];
extern u8 D_80120651;
extern u8 D_80120652;
extern u8 D_80120653;
extern u8 D_80120654;
extern u8 D_80120655;
extern s8 D_80120656;
extern u8 D_80120657;
extern s16 D_80120658;
extern u8 *D_8012065C;
extern u8 *D_80120660;
extern s16 D_80120664;
extern s16 D_80120666;
extern s16 D_80120668;
extern s16 D_8012066A;
extern s16 D_8012066C;
extern s16 D_80120674;
extern s16 D_80120676;
extern s16 D_80120678;
extern s16 D_8012067A;
extern u8 *D_80120684;
extern s32 D_80120688;
extern s8 D_80120693;
extern u8 D_80120695;
extern u8 D_80120696;
extern u8 D_80120697;
extern u8 D_80120698;
extern u8 D_80120699;
extern u8 D_8012069A;
extern u8 D_8012069B;
extern s32 D_801206A0;
extern s32 D_801206A4;
extern s16 D_801206A8;
extern s16 D_801206AA;
extern s16 D_801206AC;
extern volatile s16 D_801206B0;
extern s16 D_801206B8;
extern s16 D_801206BA;
extern s16 D_801206BC;
extern s16 D_801206BE;
extern s32 D_801206C8;
extern s32 D_801206CC;
extern u8 D_801206D7;
extern u8 D_801206D8[];
extern u8 D_801206D9;
extern s8 D_801206DA;
extern u8 D_801206DB;
extern s8 D_801206DC;
extern u8 D_801206DD;
extern s8 D_801206DE;
extern u8 D_801206DF;
extern s16 D_801206E0;
extern s32 D_801206E4;
extern s32 D_801206E8;
extern s16 D_801206EC;
extern s16 D_801206EE;
extern s16 D_801206F0;
extern s16 D_801206F2;
extern s16 D_801206F4;
extern s16 D_801206FE;
extern s16 D_80120702;
extern s32 D_8012070C;
extern s32 D_80120710;
extern s8 D_8012071B;
extern s8 D_8012071D;
extern u8 D_8012071E;
extern u8 D_8012071F;
extern s8 D_80120720;
extern s8 D_80120721;
extern s8 D_80120722;
extern u8 D_80120723;
extern s16 D_80120724;
extern s32 D_80120728;
extern s32 D_8012072C;
extern s16 D_80120730;
extern s16 D_80120732;
extern s16 D_80120734;
extern s16 D_80120742;
extern s16 D_80120746;
extern s32 D_80120750;
extern s32 D_80120754;
extern s8 D_8012075F;
extern s32 D_80120760;
extern u8 D_80120761;
extern u8 D_80120762;
extern u8 D_80120763;
extern u8 D_80120765;
extern u8 D_80120767;
extern s32 D_8012076C;
extern s32 D_80120770;
extern s16 D_80120776;
extern s16 D_80120778;
extern s16 D_80120786;
extern s16 D_8012078A;
extern s32 D_80120794;
extern s32 D_80120798;
extern u8 D_801207A5;
extern u8 D_801207A6;
extern u8 D_801207A7;
extern u8 D_801207A9;
extern s32 D_801207B0;
extern s32 D_801207B4;
extern s16 D_801207BA;
extern s16 D_801207CA;
extern s16 D_801207CE;
extern s32 D_801207D8;
extern s32 D_801207DC;
extern u8 D_801207E9;
extern u8 D_801207EA;
extern u8 D_801207EB;
extern u8 D_801207ED;
extern s32 D_801207F4;
extern s32 D_801207F8;
extern s16 D_801207FE;
extern s16 D_8012080E;
extern s16 D_80120812;
extern s32 D_8012081C;
extern s32 D_80120820;
extern u8 D_8012082D;
extern u8 D_8012082E;
extern u8 D_8012082F;
extern u8 D_80120831;
extern s32 D_80120838;
extern s32 D_8012083C;
extern s16 D_80120844;
extern s16 D_80120852;
extern s16 D_80120856;
extern s32 D_80120860;
extern s32 D_80120864;
extern s16 D_80120896;
extern s16 D_8012089A;
extern s16 D_801208CC;
extern s16 D_801208DE;
extern u8 D_801208FB;
extern s16 D_80120912[];
extern s16 D_80120922;
extern u8 D_8012093F;
extern s16 D_80120966;
extern s16 D_801209AA;
extern s16 D_801209EE;
extern s16 D_80120A32;
extern s16 D_80120A72;
extern s16 D_80120A76;
extern s16 D_80120AA8;
extern s16 D_80120AB6;
extern s16 D_80120ABA;
extern s16 D_80120AFA;
extern s16 D_80120AFE;
extern s16 D_80120B3E;
extern s16 D_80120B42;
extern u8 D_80120B5F;
extern s16 D_80120B82;
extern s16 D_80120B86;
extern u8 D_80120BA3;
extern s16 D_80120BC6;
extern s16 D_80120BCA;
extern s8 D_80120BE3;
extern u8 D_80120BE7;
extern s16 D_80120C0A;
extern s16 D_80120C0E;
extern u8 D_80120C2B;
extern s16 D_80120C4E;
extern s16 D_80120C52;
extern u8 D_80120C6F;
extern s16 D_80120C92;
extern s16 D_80120C96;
extern s16 D_80120CD6;
extern s16 D_80120CDA;
extern s16 D_80120D08[];
extern s16 D_80120D1A;
extern s16 D_80120D1E;
extern s16 D_80120D5E;
extern s16 D_80120D62;
extern s16 D_80120DA2;
extern s16 D_80120DA6;
extern s8 D_80120DC1;
extern s8 D_80120E05;
extern u8 D_80120E07;
extern s16 D_80120E1C;
extern u8 D_80120E4E;
extern s16 D_80120E60;
extern s8 D_80120E93;
extern s16 D_80120EE6;
extern s16 D_80120EF6;
extern s16 D_80120EFA;
extern s16 D_80120F2A;
extern s16 D_80120F3A;
extern s16 D_80120F3E;
extern s16 D_8012114C;
extern u8 D_8012117A;
extern u8 D_801211BE;
extern u8 D_80121202;
extern s16 D_8012121A;
extern s16 D_8012121C;
extern s32 D_80121238;
extern u8 D_80121246;
extern s32 D_8012127C;
extern s8 D_801212CB;
extern s8 D_80121311;
extern u8 D_80121313;
extern s8 D_80121317;
extern s16 D_80121328;
extern s8 D_80121353;
extern u8 D_80121357;
extern s16 D_8012136C;
extern s8 D_80121397;
extern s8 D_80121399;
extern u8 D_8012139B;
extern u8 D_8012139E;
extern s16 D_801213B0;
extern s8 D_801213DD;
extern u8 D_801213DF;
extern s16 D_801213F4;
extern s8 D_80121421;
extern u8 D_80121422;
extern u8 D_80121423;
extern u8 D_80121426;
extern s16 D_80121438;
extern u8 D_80121467;
extern s8 D_8012146B;
extern s32 D_8012149C;
extern s8 D_801214A7;
extern u8 D_801214AB;
extern s8 D_801214AF;
extern s32 D_801214E0;
extern s8 D_801214EB;
extern u8 D_801214EF;
extern s8 D_801214F3;
extern s32 D_80121524;
extern s8 D_8012152F;
extern u8 D_80121531;
extern u8 D_80121533;
extern s16 D_80121548;
extern s16 D_80121556;
extern s16 D_8012155A;
extern s16 D_8012158C;
extern s8 D_801215FD;
extern u8 D_801215FF;
extern s8 D_80121603;
extern s32 D_80121634;
extern s8 D_8012163F;
extern s8 D_80121647;
extern s8 D_80121685;
extern s8 D_8012168B;
extern u8 D_8012170F;
extern s16 D_80121726;
extern s16 D_80121728;
extern u8 D_80121750[];
extern u8 D_801217A0[];
extern Rec24 D_801217D0[];
extern s16 D_801217D6;
extern u8 D_801217E4;
extern u8 D_801217E5;
extern u8 D_801217E6;
extern s16 D_801217F8;
extern s16 D_801217FA;
extern s32 D_80121808;
extern u8 D_80121809;
extern u8 D_8012180A;
extern s32 D_80121818;
extern s16 D_8012181E;
extern s32 D_8012183C;
extern s16 D_80121842;
extern s32 D_80121860;
extern s16 D_80121864;
extern s16 D_80121866;
extern u8 D_80121874;
extern s32 D_80121884;
extern s16 D_8012188A;
extern s32 D_80121FD4;
extern s16 D_80121FD8;
extern s16 D_80121FDA;
extern s16 D_80121FDC;
extern s16 D_80121FDE;
extern s16 D_80121FE0;
extern s8 D_80121FE2;
extern s8 D_80121FE3;
extern s16 D_80121FE4;
extern s16 D_80121FE6;
extern u8 D_80121FE8;
extern u8 D_80121FE9;
extern u8 D_80121FEA;
extern s16 D_80121FEC;
extern s16 D_80121FEE;
extern s16 D_80121FF0;
extern s16 D_80121FF2;
extern s32 D_80121FF4;
extern u32 D_801220D0;
extern s16 D_801220D4;
extern s16 D_801220D6;
extern u8 D_801220E0;
extern u8 D_801220E1;
extern u8 D_801220E2;
extern s16 D_801220E8;
extern s16 D_801220EA;
extern s16 D_801220EC;
extern s16 D_801220EE;
extern s32 D_801220F0;
extern s32 D_801220F4;
extern u8 D_80122104;
extern u8 D_80122105;
extern u8 D_80122106;
extern s16 D_80122790;
extern s32 D_80122794;
extern s8 D_80122798;
extern s8 D_80122799;
extern s8 D_8012279A;
typedef struct TaiikuBig {
               s32 unk0;
               u8 unk4[0x1C];
               s32 unk20;
               u8 unk24[0x2C];
} TaiikuBig;
extern TaiikuBig D_801227A0[];
extern TaiikuBig D_80122CA0[];
extern u32 D_80122640[];
extern s32 D_80122740;
extern s32 D_80122744;
extern s32 D_80122748;
extern s32 D_8012274C;
extern s32 D_80122750;
extern s32 D_80122754;
extern s32 D_80122758;
extern s32 D_8012275C;
extern s32 D_80122760;
extern s32 D_80122764;
extern s32 D_80122768;
extern u8 D_8012276C;
extern u8 D_8012276D;
extern u8 D_8012276E;
extern s32 D_80122770;
extern s32 D_80122774;
extern s32 D_80122778;
extern u8 D_8012277C;
extern u8 D_8012277D;
extern u8 D_8012277E;
extern s32 D_80122780;
extern s32 D_80122784;
extern s32 D_80122788;
extern u8 D_8012278C;
extern u8 D_8012278D;
extern u8 D_8012278E;
extern u8 D_801227A4[];
extern s32 D_80122CD0;
extern s32 D_80122CD4;
extern s32 D_80122CDC;
extern s32 D_80122CE0;
extern s32 D_80122CE4;
extern s32 D_80122CE8;
extern s32 D_80122CEC;
extern s32 D_80122CF0;
extern s32 D_80122CF4;
extern s32 D_80122CF8;
extern s32 D_80122CFC;
extern s32 D_80122D00;
extern s32 D_80122D04;
extern s32 D_80122D08;
extern s32 D_80122D0C;
extern s32 D_80122D10;
extern s32 D_80122D14;
extern s32 D_80122D18;
extern s32 D_80122D1C;
extern s32 D_80122D20;
extern s32 D_80122D28;
extern s32 D_80122D2C;
extern s32 D_80122D30;
extern s32 D_80122D34;
extern s32 D_80122D38;
extern u32 D_80122D3C;
extern s32 D_80122D40;
extern s32 D_80122D44;
extern s32 D_80122D4C;
extern u8 D_80122DD0[];
extern s32 D_80122E94;
extern u32 D_80122E9C;
extern s32 D_80122EA0;
extern s32 D_80122EA4;
extern s32 D_80122EAC;
extern s32 D_80122EB8;
extern s32 D_80122EBC;
extern s32 D_80122EC0;
extern s32 D_80122EC4;
extern s32 D_80122EC8;
extern s32 D_80122ECC;
extern s32 D_801230D0;
extern s32 D_801230E0[];
extern s32 D_801230E4;
extern s32 D_801230F4;
extern u32 D_80123110;
extern u8 D_80123120[];
extern s32 D_80125120;
extern s32 D_80125124;
extern u8 D_80125128;
extern u8 D_80125129;
extern u8 cd_read_status;
extern s8 D_8012512B;
typedef struct Ring18 {
               u8 cmd;
               s32 arg;
               u8 data[16];
} Ring18;
extern Ring18 D_80125130[];
extern s32 D_801255B0;
extern u8 D_801255B4;
extern u8 D_801255B5;
extern u8 D_801255B8;
extern u8 D_801255C0[];
extern u8 D_801255C8[];
extern s32 D_801255D8;
extern u8 D_801255DC;
extern s16 D_801255E0;
extern s16 D_801255E2;
extern u32 D_801255F0[];
extern s32 D_80125BF4;
extern s32 D_80125BF8;
extern s32 D_80125BFC;
extern s32 D_80125C00;
extern s32 D_80125C04;
extern s32 D_80125C08;
extern s32 D_80125C0C;
extern u8 D_80125C10[];
extern s16 D_80125C50;
extern s16 D_80125C52;
extern u8 D_80125C54;
extern u8 D_80125C55;
extern u8 *D_80125C58;
extern u8 D_80125C60[];
extern u8 D_80125C90[];
extern s16 D_80125CA0;
extern s32 D_80125CA4;
extern s32 D_80125CA8;
extern s32 D_80125CAC;
extern s32 D_80125CB0;
extern u16 D_80125CC0;
extern Work80125D10 D_80125D10;
extern s32 D_80125D14;
extern s32 D_80125D18;
extern s32 D_80125D1C;
extern s32 D_80125D20;
extern s32 D_80125D24;
extern s32 D_80125D28;
extern s16 D_80125D4A;
extern s8 D_80125D5C;
extern u8 D_80125D5D;
extern s16 D_80125D38;
extern s16 D_80125D3A;
extern s16 D_80125D3C;
extern u16 D_80125D4C;
extern u16 D_80125D4E;
extern u16 D_80125D50;
extern u16 D_80125D52;
extern u8 D_80125D5E;
extern u8 D_80125D5F;
extern s32 D_80125D60[];
extern s32 D_80125D64;
extern s32 D_80125D68;
extern s32 D_80125D6C;
extern s32 D_80125D70[];
extern s32 D_80125D80[];
extern s32 D_80125D84;
extern s32 D_80125D88;
extern s32 D_80125D8C;
extern s32 D_80125D90;
extern s32 D_80125D94;
extern s32 D_80125D98;
extern s32 D_80125D9C;
extern u16 D_80125E60;
extern u16 D_80125E62;
extern s16 D_80125E64;
extern s16 D_80125E66;
extern s16 D_80125E68;
extern u8 D_80125E70[];
extern s16 D_80126080[];
extern s32 D_8012749C;
extern s32 D_801274A0;
extern s32 D_80129F10;
extern u8 D_80129F40[];
extern s32 D_800B699C[];
extern s8 D_80120E49;
extern u16 D_800B670A[];
extern u8 D_80089C84;
extern u8 D_800CA2BC;
extern s16 D_800EB042;
extern s16 D_800EB044;
extern u8 D_80123324[];
extern u8 D_800B0E72;
extern u32 D_800B5970[16];
extern u32 D_800B59B0[16];
extern u32 D_800B0C4C;
extern u8 D_800E94A0[];
extern u8 D_800FD4A0[];
extern s16 D_800B0962;
extern u8 D_800E7D4E;
extern s32 D_80122E98;
extern s32 D_80122EA8;
extern s32 D_80122EB0;
extern s32 D_80122EB4;
extern u8 D_8011EDE0[];
extern u8 D_8011EEF0[];
extern u8 *D_800B342C[];
extern u8 *D_800B3610;
extern u8 *D_800B361C;
extern u8 *D_800B3628;
extern s32 D_800EED08;
extern s32 D_801230D4;
extern s32 D_801230D8;
extern s16 D_800F54B6;
extern u8 D_800D9320[];
extern TaiikuBig D_80122840[];
extern TaiikuBig D_80122B10[];
extern u8 D_80122660[];
extern s16 D_800CA330[];
extern u32 D_800E3680[16];
extern s16 D_801217D4;
extern u8 D_800EC1C8;
extern u8 D_800EC1A4;
extern u8 D_800EC1A5;
extern u8 D_800EC1A6;
extern u8 D_800EC1C9;
extern u8 D_800EC1CA;
extern s32 D_80080710;
extern s16 D_800F1990[14];
extern u8 D_80120694[];
extern s32 D_80122CD8;
extern u8 D_800B3CF8;
extern s32 D_800EECC4;
void func_80010678();
void func_80012D2C();
void func_80012D40();
void func_80017E50();
void func_8001886C();
void func_80018944();
void func_8001FD50();
void func_8003535C();
void func_8003580C();
void func_80036884();
void func_80037A90();
void func_8003C2C0();
void func_800410AC(void);
void func_8004111C(void);
void func_80041168(s32 arg0);
void func_80041584(void);
void func_800415B4(s32 arg0, s32 arg1);
void func_8004164C(s32 arg0, s32 arg1);
void func_80041878(void);
void func_800418B0(void);
void func_800419FC(void);
void func_80041C2C(void);
void func_80041F48(void);
void func_80042058(void);
void func_80042134();
void func_800422C8(void);
void func_80042418(void);
void func_80042458(void);
void func_80042488(void);
s32 func_80042808(void);
s32 func_8004284C(void);
void main_state_set();
s32 func_80042908(s32 arg0);
s32 func_80042940(s32 arg0);
s32 func_80042960(void);
s32 func_80042AC8();
void func_80042C30(void);
void func_80042CB0();
s32 func_80043010(void);
void func_800430C0();
void func_800433D0(s32 arg0);
s32 func_80046590(s16 arg0, u8 arg1);
s32 search_load_tpage_buf_lock(s32 arg0, u8 arg1);
u8 *get_k_work(s32 arg0);
void glyph_expand_1bpp();
void draw2d3d(u8 arg0, u8 arg1);
void func_800438DC();
void back_clear_switch(s32 arg0);
void func_800438F0();
s32 func_80043914();
s32 load_palette();
s32 load_csr_tp();
void LoadSquare(u16 arg0, u16 arg1, u16 arg2, u16 arg3, void *arg4);
void func_8004435C(u16 arg0, u16 arg1, u16 arg2, u16 arg3, s32 arg4);
s32 func_80044750(s32 arg0);
s32 func_80044774(s32 arg0);
u8 func_8004480C(void);
void func_8004482C(void);
void func_80044890(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
s32 func_80044C98(void);
s32 func_80044E8C(void);
s32 func_8004500C();
s32 func_800450F4(s32 arg0, s32 arg1);
s32 func_800451D0();
void func_800452C4(void);
void func_80045414(s32 arg0, s32 arg1, u8 *arg2);
void func_80045B14(void);
s32 func_80045FF4(void);
u8 func_80046094(void);
u8 cd_read_sync_status(void);
u8 func_800460DC(void);
u8 func_800460EC(void);
s32 func_80046274(void);
void func_80046290(s32, s32, s32);
void func_800462BC(u8 arg0, s32 arg1, s32 *arg2);
void func_800462C8(u8 arg0, s32 arg1, s32 arg2);
void cd_read_sectors(u8 arg0, s32 arg1, s32 arg2);
s32 file_checksum_ok();
s32 func_80046500(void);
void func_800469F4();
void func_80046A7C();
void func_80046B88();
void func_80047550(void);
void func_80047560();
void tpage_buf_clear_all(void);
void func_80048390();
void tpage_buf_clear(void);
void _sys_default_tpage_set(void);
void load_tpage_buf_lock();
void func_80048DAC(s32 arg0);
void func_80048DD0(s32 arg0);
void func_80048E78(void);
void func_80048EB8(s32 arg0);
void func_80048F64(s32 arg0);
s32 GetWorkBase(s32 arg0, s32 arg1);
void *func_800490F0();
void safe_env(s32 arg0);
void dtd_on_tpage(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_800494BC();
void dtd_on(s32 arg0);
void func_8004955C();
void _sprite_set_light_effect1(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void func_80049A40(s16 a, s16 b, s16 c, s16 d, s32 e, s32 f, s32 g);
s32 func_80049B20();
void func_8004A1A0();
void func_8004A2C4();
void func_8004A4D8();
void func_8004A734(void);
void func_8004A764();
void func_8004A7C4();
void func_8004A7F4();
void func_8004A8EC();
void func_8004AC18(s32 arg0);
void func_8004ACA8();
void func_8004ACC8(s32 arg0);
void func_8004ADE4(void);
void func_8004AE28(s32 a0, s32 a1, s32 a2);
void func_8004AE54(s32 a0, s32 a1, s32 a2, s32 a3);
void func_8004AEB0();
void func_8004B19C(s32 a, s32 b, s32 c);
void func_8004B338(s32 arg0, s32 arg1, s32 arg2);
void func_8004B358(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_8004BB54();
s32 func_8004BC20(s32 arg0);
void func_8004C060();
void func_8004C250();
void func_8004C360();
void func_8004C46C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void func_8004C670();
void func_8004C6A0();
void func_8004C6B0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_8004C808();
void func_8004C90C();
void func_8004C984();
void func_8004CA18();
void func_8004CA48();
void func_8004CDDC();
void func_8004CF30();
void srn_vram_set(s32, s32, s32, s32);
void func_8004D320();
void func_8004DDD8();
void func_8004DE1C(void);
void func_8004DEAC();
void func_8004DEE4();
void srn_init(s32, u32 *, s32);
void func_8004E44C(s32 arg0, u32 *arg1, s32 arg2);
void func_8004E500(s32 arg0);
void func_8004E58C(void);
s32 func_8004E788(s16 x, s16 y, s32 c, s32 d, s32 e);
s32 set_kanji_string(s16 x, s16 y, u8 col, u8 *str, s32 arg4);
void func_8004E884();
void k_disp_start(s32 arg0);
void k_disp_switch(s32 arg0, s32 arg1);
void func_8004E9F4();
void k_reset(s32 arg0);
void func_8004EA98();
void k_sub_reset_point_set(void);
void func_8004EAAC();
void k_sub_reset(void);
void func_8004EAD4();
void k_sub_disp_start(s32 arg0);
s16 k_disp_goto_line_end(void);
void k_speed_set(u8 arg0);
u8 get_k_speed(void);
s32 k_disp_inc(void);
s32 check_end_k(void);
s32 func_8004ECB4();
s32 func_8004ECF0();
void func_8004EDE0();
void func_8004EDF4();
void func_8004EE18();
void func_8004F860();
void func_8004F870();
s8 menu_check();
s8 menu_check_1();
s8 menu_check_2();
void func_8004FC10();
void func_800504CC();
s32 func_80050AB8();
void func_80050B54();
void menu_bar_show();
void func_80050C24(s32 arg0, s32 arg1);
void x_taku_menu_set(s32 arg0, s32 arg1);
void func_80050D60();
void func_80050D78();
void x_taku_string_set(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_80050DFC(u8 *arg0);
void gnsx(u8 *arg0);
void func_80050E8C(u8 *arg0, s32 arg1, s32 arg2);
void sndisp();
void func_80051010();
void sndi(u8 *arg0, s32 arg1, s32 arg2);
s32 get_g_name(u8 *arg0, s32 arg1);
void func_80051508(u8 *arg0, s32 arg1);
void get_p_name();
s32 func_80051A68();
s32 get_g_zyotai_s(s32 arg0);
u32 func_80051B48();
s32 get_g_zyotai_h(s32 arg0);
void func_80051DBC();
void xa_wait(void);
void func_80051DD8();
void func_80052000();
void func_80052060();
u32 get_h_tokimeki(s32 arg0);
u32 get_h_yuukou(s32 arg0);
s32 birth_day_check_days(s32 arg0, s32 arg1, s32 arg2);
void func_800530E0();
s32 func_8005352C();
void func_80053650(s32 arg0, s32 arg1, s32 arg2);
s32 func_800536AC();
void card_ev_set(void);
void Sw_Start(void);
void Hw_Start(void);
u8 Sw_Test(void);
void Sw_Clear(void);
void Hw_Clear(void);
void func_80053CC0(void);
void func_80053CE0(void);
void func_80053D10(void);
s32 func_80053F04(void);
s32 func_80054140(void);
u8 func_80054284(void);
s32 func_800552DC(s32 arg0);
void func_80053EFC();
s32 func_80054388();
u8 func_8005448C(void);
void func_80054864();
void func_8005493C();
void func_800549E8();
s32 func_80054AF4(s32 arg0);
void *func_80054B60();
void func_80054C4C(void);
s32 func_80055A38(s32);
s32 func_80055AFC(s32 arg0);
void func_80056070(u8 *buf, s32 arg1);
void func_80056284(void);
void set_movie_offset(s16 arg0, s16 arg1);
void strKickCD(s32 arg0);
void func_80056BA8();
void func_80057390();
void set_dec_bri(u8 arg0);
s32 get_last_gamen_mode(void);
void dec_bg_reset(void);
void func_800573AC();
void dec_bg_show_switch(s32 arg0);
void dec_bg_show_set(s32 arg0, s32 arg1);
void func_80057418(s32, s32);
s32 dec_bg_cd_read(s32 arg0, s32 arg1);
s32 func_8005742C(s32 arg0, s32 arg1);
s32 func_8005751C();
void func_80057640();
s32 func_80057710(void);
void func_800578F4(s32 a);
void func_80057D28(s32);
void func_80058398();
void func_80058D20();
void initView(void);
void func_80058EB0();
void func_80058F0C(s32 *arg0);
void initModelingData_init(s32 *arg0);
s32 func_80058F9C(u32 *arg0, s32 arg1, u32 arg2);
void func_80059048();
void func_800590CC();
void func_800591D8(s32 arg0);
void func_80059308(void);
void func_80059688(s32 arg0);
void SenseMouse(u16 arg0, u16 arg1);
void SetMouse(s32 arg0, u32 arg1, u32 arg2);
void func_80059938(s32 port);
void func_80059B04();
void func_80059B40(void);
void func_80059BC0(void);
void func_80059BE8(void);
s32 func_80059E00(void);
s32 func_8005A1A0(void);
void func_8005A2A8(void);
void func_8005A37C(void);
void func_8005A3E8(void);
void func_8005A560(void);
void func_8005A668();
void func_8005ABC0();
void func_8005ABD0(void);
void func_8005AC70(s32 arg0);
void func_8005AD70(void);
void func_8005AE60();
void func_8005AEFC();
s32 func_8005AF28(void);
s32 func_8005AFC8(void);
s32 func_8005B040();
void func_8005B06C();
s32 func_8005B0F4();
s32 func_8005B1A8(void);
void func_8005B264();
s8 func_8005B28C();
s32 func_8005B2BC();
s32 func_8005B32C();
u32 func_8005B368();
void func_8005B39C(void);
s32 func_8005B43C();
void func_8005B798(void);
void func_8005B830(void);
void func_8005B8A0(void);
void func_8005B8E0(void);
void func_8005B908();
void func_8005BD20();
void func_8005C4CC(s32 arg0);
void func_8005D174(void);
s32 func_8005D448(void);
s32 func_8005BE8C(void);
void func_8005BCEC(void);
void func_8005C5BC(void);
void func_8005CF6C(void);
void func_8005CA94(void);
void func_8005C7C0(void);
void func_8005C7FC(void);
extern u8 D_800B59F8[];
extern u8 D_800B5A1C[];
void func_8005F09C(void);
s32 func_8005FDEC(void);
s32 func_8005DEA0(s32 arg0);
s32 func_8005E150(void);
s32 system_settings_init(void);
s32 func_800603C0(void);
void func_8005D1B0();
void func_8005D9B4();
s32 func_8005E0E0();
void join_club(void);
void join_club_init(void);
s32 join_club_select(void);
s32 join_club_message(void);
void join_club_exit(void);
s32 func_80060B24();
void func_80060B78(void);
void uwasa_exit0(void);
s32 uwasa0();
s32 pre_xmas_init(void);
s32 func_80060EA0();
s32 pre_syogatu_init();
s32 func_80061634();
void func_80061710(void);
void func_80061790(void);
void func_800618B0();
void func_80061EFC(void);
void func_8006211C(void);
void func_8006218C(void);
void func_80062210(void);
void func_800623E4(void);
void func_80062524(void);
void func_800625C0(void);
void func_800626B0(void);
void func_80062764(void);
void func_800627DC(void);
void func_80062CD0();
void func_80062DBC();
void func_800634FC();
void func_80063520(u32 arg0);
void func_80063668();
void bustup_wink(void);
void func_80063930(s32);
void parameter_disp_switch();
void parameter_show_init(void);
void parameter_show(void);
void func_8006492C(s32 arg0);
void hizuke_disp_switch(s32 arg0);
void func_800649D4();
void hizuke_init(void);
void func_80064DEC();
void hizuke_show(void);
void func_80064E48();
void message_disp_switch(s32 arg0);
void func_80064E84();
void message_window_init(void);
void func_80064F48();
void message_window_show(void);
void func_80064FA4(s32);
void icon_disp_switch(s32 a);
void icon_can_use_set(s32 idx, s32 on);
void func_8006509C(void);
void func_80065900(u8 arg0);
void func_80065B0C(s32 a);
void func_80065F34(s32 arg0);
void func_80066104();
void func_8006612C();
void func_80066334();
s32 func_80066A2C(void);
s32 func_80066A84(void);
void func_80066C08(s32 a);
void func_80067438(void);
void func_800674B0(void);
void func_8006764C(s32 arg0);
void func_800676AC();
void func_80067870(void);
void func_80067C28(void);
s32 func_80067DD4(void);
void func_80067E34(void);
void func_80067F04(void);
s16 func_80068898(s32 arg0, s32 arg1);
s16 func_800688F0(s32 arg0, s32 arg1);
void func_80068938(u8 arg0, u8 arg1, s32 arg2);
void func_80068EC0();
void cal_base_show(void);
void func_80069128();
void cal_sprite_init(void);
void func_800696DC(s32, s32);
void func_8006A044();
void func_8006A2CC();
void func_8006B0C8();
void club_list_menu(void);
void func_8006B900(void);
void func_8006BA40(void);
void func_8006BC28(s32 arg0);
void func_8006BD6C(s32 arg0);
s32 func_8006C334(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 func_8006C700(void);
void func_8006C848(s32 arg0);
u8 *func_8006CA9C(void);
u8 *func_8006CAE0(void);
void _schedule_init(void);
void last_date_spot_timer_dec(void);
void restore_bgm(void);
s32 func_8006CDD4(void);
void func_8006D038(void);
void func_8006D138(void);
void func_8006D6E0();
s32 func_8006EB64(u8 *arg0, u8 *arg1);
s32 func_8006EEC4(u8 *arg0, u8 *arg1);
void yuukou_down(void);
void syoushin_up(void);
void func_80070F80(void);
void func_80070FD0(void);
void func_8007132C(void);
void func_80072338(void);
void func_8007259C(u8 arg0);
void func_800726F0(void);
void func_80072734();
s32 func_80072944(void);
void func_80072998(void);
void func_80072B20(void);
s32 func_80072B5C(s32 arg0);
void func_80072C68(void);
void func_80072CA0(void);
void func_80072FE8();
void func_800737A0(void);
u8 week_day_init(void);
s32 get_weekly_bg_sector(void);
u8 func_80073AD8(void);
u8 func_800741B8(void);
u8 func_8007437C(void);
void func_800744A0();
s32 week_day_main0(void);
void week_day_exit0(void);
void func_800748B8();
void func_80074950();
void func_80074A14();
u8 vacation_day_init(void);
u8 func_80074DE0(void);
u8 func_80074F24(void);
u8 func_8007505C(void);
void func_800755B4(void);
void func_80075A64(void);
void func_80075BFC();
void func_80075C24(void);
s32 func_80075C84();
s32 func_80075FA0();
s32 func_80076000();
void func_80076450();
void func_80076630(void);
s32 func_80076B48(void);
void func_80076C7C(void);
void appointment_launch(void);
void func_8007739C(void);
void func_80077694(void);
void func_80077900(void);
void func_80077C50(void);
void func_80077E30(void);
void magazine_exit(void);
void holiday_tel(void);
void holiday_tel_init(void);
void holiday_tel_call(void);
void telephone_class_init(void);
void holiday_club_join_exit(void);
void func_80078950();
void func_80078970();
s32 func_800789A0();
void func_800789E0(void);
void func_80078A94();
void func_80078C48();
void ovl_load_poll(void);
void ovl_load_verify(void);
void func_80079070();
s32 func_80079524(void);
void func_80079B10(u16 arg0);
void func_80079C70(void);
void func_80079D28(s16 arg0);
s32 func_80079E00(s32 arg0);
void func_80079E9C(void);
void func_80079F00(void);
void func_8007A4DC();
void func_8007A50C(void);
void func_8007A5BC();
void func_8007A868(void);
void func_8007A924(s32 arg0);
void func_8007A98C();
void func_8007AB24(u16 arg0);
void func_8007ABE0();
void func_8007AD6C();
void func_8007AEC0(void);
void func_8007AF0C(void);
void func_8007B144();
void func_8007B2B4();
void func_8007B3DC(u16 arg0);
void func_8007B460(u16 arg0);
void func_8007B4E4(u16 arg0);
void func_8007B568(s32 arg0, s32 arg1);
void func_8007B5CC();
s32 func_8007B5EC(u16 a);
void func_8007B640(void);
void func_8007B6E0();
void func_8007B734();
void func_8007B7E0();
void func_8007B844(void);
void func_8007B8F8();
void func_8007B99C(u16 arg0);
void func_8007BA54(void);
void func_8007BAAC(void);
void func_8007BCFC();
void func_8007BDE8(void);
void func_8007BE94(s16 arg0);
void func_8007BF04(s16 arg0);
void func_8007BFA8();
void func_8007BFB8(void);
void SD_InitCDLevelInfo(void);
void SD_GetCDLevel();
void func_8007C6A8(s32 arg0);
void addr_init_bustup(void);
void func_8007C740();
void func_8007C8A4(void);
s32 func_8007C8D8();
s32 func_8007D8AC();
s32 func_8007E390(void);
void normal_date_girl_in();
s32 normal_date_girl_in_init(void);
s32 normal_date_girl_in_main(void);
void normal_date_girl_out(void);
void normal_date_girl_out_init(void);
s32 normal_date_girl_out_main(void);
s32 normal_date_bg_fadein(void);
void func_8007E934();
void normal_date_bg_fadeout(void);
s32 normal_date_bggirl_fadein(void);
s32 normal_date_bggirl_fadeout(void);
void normal_date_move_place(void);
void normal_date_move_place_init(void);
s32 normal_date_move_place_main(void);
void place_init(void);
void bg_read_sub2(s32 arg0);
void func_8007ED84();
void check_k_scroll();
void func_8007EDF8();
void wait_sub_sub(s16 arg0);
void func_80081128(void);
void dialogue_render(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, void *arg5, void *arg6, s32 arg7);
void func_80081D30();
s32 func_80082764();
s32 make_three_select();
void normal_date_three_select(void);
s32 normal_date_three_select_init(void);
s32 normal_date_three_select_main(void);
void normal_date_two_select(void);
void normal_date_two_select_init(void);
s32 normal_date_two_select_main(void);
void func_80083378(void);
void func_800833A0(void);
void func_800833F0(void);
void func_80083418(void);
void func_80083440(u8 arg0);
s32 func_80083474(void);
s32 func_80083808();
void func_80083A10();
void func_80083B24();
void read_bustup();
void func_800846C0();
void k_disp_inc2();
void func_800847B8(u8 arg0);
void select_girl_init(s32 arg0);
s32 select_girl_main(s32 arg0);
void func_80084C98();
void sprite_brightness(s16 idx, u8 val);
void check_para_limit(void);
void func_80084D3C();
void func_80084E4C(void);
s32 func_80084E90(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
s32 func_800850D4();
void func_80085234();
void func_80085368(void);
void func_800853FC(void);
void event_face0(void);
void event_face1(void);
void func_800854C8();
void _sprite_set_box_shade_tarao(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void func_80085678();
void func_8008585C();
void don_wait(void);
void normal_date_speak_1line();
void func_80085A60();
void normal_date_speak(void);
s32 func_80085B3C();
void func_80085CD4(u8 a);
void func_80085E30(s32 arg0, s32 arg1);
void Vblnk_Timer_Init(void);
s32 Vblnk_Timer(void);
void Scroll(s32, s32, s32, s32, s32);
void func_80085F0C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Rot_2D(s32 arg0, s32 *arg1, s32 *arg2, s32 *arg3, s32 *arg4);
void Default_Disp(void);
void func_80086424();
s32 func_8008667C(s32 a);
void func_800867CC(void);
void DecDCTReset(s32 mode);
void func_800869A4(s32 a);
void func_800869C8(s32 arg0);
void func_80086AB0(s32 arg0);
s32 func_80087954(s32 arg0, u8 *arg1);
s32 func_800879D0();
s32 func_80087C64();
s32 func_80087E4C(s32 arg0, u8 *arg1);
s32 func_80087E6C(s32 arg0, s32 arg1, s32 arg2);
void func_80088070();
s32 func_800880C0();
void func_80088150(u8 *p, s32 n);
void func_80088180(s32 a, s32 b, s32 c, s32 d, s32 e);
void func_80089200();
void func_80089244();
void D_800896E0();
void D_800898F0();
void D_80089B6C();
void D_80089BD4();
void func_8008A0D4(s32 arg0);
void func_8008A148();
void func_8008B750();
void func_8008B8C4();
void func_8008B904();
void func_8008BAA4();
void func_8008BE34();
void func_8008BE54();
void func_8008BEBC();
void func_8008BEE0();
void func_8008BFB0();
s32 func_8008C1B0();
s32 func_8008C620();
void SsVabTransCompleted();
void func_8008D610();
void func_8008E280();
void func_8008E2E0();
void func_8008E310();
void func_8008E408();
void func_8008E6AC();
void func_8008E8B4();
void func_8008F54C();
void func_8008F618();
void func_8008FB00();
void func_8008FC10();
void func_8008FD1C();
void func_8008FD68();
void func_8008FEB0();
void func_8008FED0();
void func_8008FEF0();
void func_8008FF5C();
void func_8008FF60();
void func_80090580(s16);
void func_8009068C();
void func_800907E0(s16);
void func_80090960();
void func_80090D20(void);
void func_80090D60();
void func_80090DB0();
void func_80090DF0();
void func_80090E30();
void func_80090E60(s16);
void func_800949B0();
void func_80097D90(s32 a, s32 b, s32 c, s32 d, s32 e);
s32 func_80098370(void);
void func_80098380(void);
void func_80098490(s32 a, s32 b, s32 c, s32 d);
void func_80098530(void);
void func_800985A0(void);
void func_80099540(s32 *p);
void func_80099E30(s32 a, void *p);
void func_8009AD30(s32 arg0);
void func_8009AD50(s32 arg0);
void func_8009AD60(s32 arg0);
s32 func_8009AD70();
void GsSetAmbient();
void func_8009B310(s32 arg0, s32 arg1, s32 arg2);
void func_8009B340();
void func_8009B3C0(s32 arg0);
void func_8009B430(s32 arg0, u8 *arg1, s32 arg2, s32 arg3);
void func_8009B560();
void func_8009C210(s32 arg0);
void func_8009C5E0();
void func_8009C674(s32 arg0);
void func_8009C7F8(RECT *rect, s32 arg1, s32 arg2, s32 arg3);
void func_8009C884(RECT *rect, void *arg1);
void func_8009C8E0(RECT *rect, void *arg1);
void func_8009C93C(RECT *rect, s32 arg1, s32 arg2);
void func_8009CC04();
void func_8009CCBC();
void func_8009CCF4();
void func_8009D10C();
void func_8009D294(s32 a, s32 b, s32 c, s32 d, s32 e);
s32 func_8009ECB0(s32 a, s32 b, s32 c, s32 d);
s32 func_8009ED74();
void AddPrim(void *ot, s32 prim);
void func_8009EED0();
void SetSemiTrans();
void func_8009EF84();
void SetShadeTex();
void func_8009F02C();
void func_8009F054();
void func_8009F0A4();
void func_8009F0B8(u8 *p);
void func_8009F0F4();
void InitGeom(void);
s32 func_800A0070(s32);
s32 rsin();
s32 func_800A0140(s32 a);
void func_800A09D0();
void func_800A0C64();
void func_800A0F4C();
void func_800AB984();
void func_800ABA38();
void func_800ABAB8();
void func_800AD950();
s32 InitCARD(s32 arg0);
s32 StartCARD(void);
void _bu_init(void);
s32 _card_auto(s32 arg0);
s32 _card_status(s32 chan);
void bzero(void *p, s32 n);
void func_800AE080(u8 *arg0, s32 arg1);
void func_800AE090(void *dst, void *src, s32 n);
void func_800AE0A0(void *arg0, s32 arg1, s32 arg2);
void memcpy();
void func_800AE0B0(void *arg0, ...);
s32 printf(u8 *fmt, ...);
s32 func_800AE0C0(s32);
s32 func_800AE0D0(void);
s32 rand();
s32 func_800BCE40();
s32 func_800AE0E0(void *arg0);
s32 strlen(u8 *s);
void func_800AE0F0(void *dst, void *src);
void strcpy(u8 *dst, u8 *src);
void strcat(u8 *dst, u8 *src);
void func_800AE120(s32 arg0);
s32 strcmp(u8 *a, u8 *b);
s32 SetSp();
void InitHeap();
s32 GetGp();
s32 OpenEvent(u32 desc, s32 spec, s32 mode, void (*func)(void));
s32 EnableEvent(s32 ev);
s32 SetRCnt(u32 spec, u32 target, u32 mode);
s32 StartRCnt(u32 spec);
void CloseEvent(s32 ev);
void InitPAD(u8 *buf1, s32 len1, u8 *buf2, s32 len2);
void StartPAD(void);
void ChangeClearPAD(s32 arg0);
s32 TestEvent(s32 ev);
s32 close(s32 fd);
s32 format(u8 *path);
s32 open(u8 *name, s32 mode);
s32 GetSp(void);
void func_800BCDF0();
void func_800BCE10();
s32 func_800BDC20();
void func_801040F0(void);
s32 func_80067DFC(void);
s32 func_80053418();
s32 func_800463E8(void);
s32 func_80049FF0();
void place_init2(void);
void normal_date_bg_out(void);
void func_8004A9CC();
void func_8004AFB8();
void func_800651F8();
void func_80065234();
void func_8006B5BC(GsWord arg0);
typedef struct CdLevelWork {
               u16 unk_00;
               u16 unk_02;
               u16 unk_04;
               u16 unk_06;
               u16 unk_08;
} CdLevelWork;
void read_bustup_uniform(void);
void set_tarao_sprt();
void set_tarao_bg(s32 arg0);
void SD_DetectCDPeak(u16 *l, u16 *r, u16 pos, u16 n);
void SD_CalcCDAve(s16 *left, s16 *right, u16 start, u16 count);
s32 func_8006F218(u8 *arg0, u8 *arg1);
s16 getCDlevel(void);
u8 *strncat(u8 *dst, u8 *src, s32 count);
void func_80046600(void);
void func_800464E8(void);
extern u8 D_800B0895;
extern u8 D_800B0F31;
extern u8 D_800B0F32;
extern u8 D_800B0F33;
extern u8 D_800B0F34;
extern s32 D_800B3D30;
extern s32 D_800B3D34;
extern s32 D_800B3D38;
extern s32 D_800B3D3C;
extern u8 D_800B3D20;
s32 func_8007E81C();
extern u8 D_800B3634;
extern u8 D_800B3638;
extern s16 D_80120BB8;
void func_8005C414(void);
void func_8005D56C(void);
s32 func_8005D804(void);
void func_8005D31C(void);
void func_8005C968(void);
s32 func_8005BAE0(void);
s32 func_8005BC38(void);
s32 func_8005BB84(void);
void func_8005E018(void);
void func_8005E2C0(void);
void func_8005E40C(void);
void func_8005E454(void);
void func_8005E5C8(void);
void func_8005E7A4(void);
void func_8005E7F0(void);
void func_8005E924(void);
void func_8005E9EC(void);
void func_8005EA38(void);
void func_8005EB74(void);
void func_8005EC78(void);
void func_8005ECB8(void);
void func_8005EE00(void);
void func_8005EF04(void);
void func_8005EF44(void);
void func_8005F22C(void);
void func_8005F26C(void);
void func_8005F478(void);
void func_8005FA20(void);
void func_8005F6E4(void);
void func_8005FDA0(void);
void func_8005FF68(void);
void func_80060104(void);
void func_80060504(void);
void func_80060150(void);
extern u8 D_801405E4;
extern u8 *D_80140AD8[];
extern u8 D_80140B04[];
extern u8 D_80140B5C[];
void func_80135FFC(void);
void func_801360E4(void);
extern u8 *D_80140D18[];
extern u8 D_80140D44[];
extern u8 D_80140D9C[];
void func_801361CC(void);
extern u8 *D_80140F0C[];
extern u8 D_80140F38[];
extern u8 D_80140F90[];
void func_801362B4(void);
void func_80132060(void);
void func_80134BE0(void);
void func_80132000(void);
s32 func_8013AA30(void);
s32 func_8013B54C(void);
void func_8013BF50(void);
s32 func_8013A40C(void);
s32 func_8013AC0C(void);
s32 func_8013A88C(void);
s32 func_8013A820(void);
s32 func_8013BFDC(void);
s32 func_8013BF70(void);
s32 func_8013C4D8(void);
s32 func_8013C46C(void);
s32 func_8013C9D4(void);
s32 func_8013C968(void);
s32 func_8013CBA0(void);
s32 func_8013CB34(void);
void func_8013A76C(void);
s32 func_8013A478(void);
void func_8013BE38(void);
s32 func_8013A79C(void);
void func_801396EC(void);
void func_80132138(void);
void func_80134DA0(void);
extern s16 D_801444A0[];
extern u8 D_801444B8[11];
extern u8 D_801444C4[11];
extern s32 D_80140554;
extern s32 D_80140558;
extern s32 D_8014055C;
extern u8 *D_801434D4[];
extern u32 D_80143500[];
extern u32 D_80143558[];
extern u8 *D_801437BC[];
extern u8 *D_80143958[];
extern u32 D_80143984[];
extern u32 D_801439DC[];
extern u8 *D_80143BA0[];
extern u32 D_80143BCC[];
extern u32 D_80143C24[];
extern u32 D_80144E3C[];
extern u32 D_80144E44[];
extern u32 D_80144E4C[];
extern u32 D_80144EAC[];
void func_8013226C(void);
void func_80132FC4(void);
void func_80133D54(void);
void func_801345C4(void);
void func_801352F0(void);
void func_80136724(void);
typedef struct TelRec44 {
               u8 pad00[3];
               u8 unk_03;
               u8 pad04[0x14];
               s16 unk_18;
               s16 unk_1A;
               s16 unk_1C;
               u8 pad1E[0x12];
               s32 unk_30;
               u8 pad34[0x10];
} TelRec44;
extern u8 *D_801410BC[];
extern u32 D_801410D4[];
extern u32 D_80141104[];
extern u8 *D_801410C8;
extern u8 *D_801410CC;
void func_8013586C(void);
void func_80135A44(void);
void func_80135C4C(void);
void func_80135E28(void);
extern s32 D_80140564[16];
extern s32 D_801405A4[16];
s32 func_8013655C(void);
extern u32 D_8014109C[];
extern u32 D_801410AC[];
extern u8 *D_80141094;
extern u8 *D_80141098;
extern u32 D_8014104C[];
extern u32 D_80141064[];
extern u8 *D_80141040;
extern u8 *D_80141044;
extern u8 *D_80141048;
extern u32 D_80144210[];
extern u32 D_80144240[];
extern u8 *D_801441F8;
extern u8 *D_801441FC;
extern u8 *D_80144200;
extern u8 *D_80144208;
extern u8 *D_8014420C;
extern u8 *D_80144204;
void func_801384F0(void);
extern u32 D_80144298[];
extern u32 D_801442E8[];
extern u8 *D_80144270;
extern u8 *D_80144274;
extern u8 *D_80144278;
extern u8 *D_80144280;
extern u8 *D_80144284;
extern u8 *D_80144288;
extern u8 *D_8014428C;
extern u8 *D_80144290;
extern u8 *D_80144294;
extern u8 *D_8014427C;
void func_8013883C(void);
extern u32 D_80144350[];
extern u32 D_80144380[];
extern u8 *D_80144338;
extern u8 *D_8014433C;
extern u8 *D_80144340;
extern u8 *D_80144348;
extern u8 *D_8014434C;
extern u8 *D_80144344;
void func_80138D20(void);
extern u32 D_801443C8[];
extern u32 D_801443F8[];
extern u8 *D_801443B0;
extern u8 *D_801443B4;
extern u8 *D_801443B8;
extern u8 *D_801443C0;
extern u8 *D_801443C4;
extern u8 *D_801443BC;
void func_80139064(void);
extern u32 D_80144440[];
extern u32 D_80144470[];
extern u8 *D_80144428;
extern u8 *D_8014442C;
extern u8 *D_80144430;
extern u8 *D_80144438;
extern u8 *D_8014443C;
extern u8 *D_80144434;
void func_801393A8(void);
extern u32 D_80141084[];
extern u8 *D_8014107C;
extern u8 *D_80141080;
void func_8013639C(void);
extern u32 D_801441A8[];
extern u32 D_801441D0[];
extern u8 *D_80144194;
extern u8 *D_80144198;
extern u8 *D_8014419C;
extern u8 *D_801441A0;
extern u8 *D_801441A4;
void func_801380D8(void);
typedef union TelDateEntry {
               GsWord word;
               struct {
        u32 place : 5;
        u32 year : 7;
        u32 month : 4;
        u32 day : 5;
        u32 unk_21 : 3;
        u32 unk_24 : 1;
        u32 girl : 4;
        u32 active : 1;
        u32 unk_30 : 2;
    } bits;
} TelDateEntry;
extern s32 D_80140550;
extern s32 D_80140560;
extern u8 D_80141134[][21];
extern u8 *D_801413C8[];
extern u32 D_801413F8[];
extern u32 D_80141458[];
extern u8 *D_80141680[];
extern u32 D_801416B0[];
extern u32 D_80141710[];
extern u8 *D_801418D0[];
extern u8 *D_8014197C[];
extern u32 D_801419AC[];
extern u32 D_80141A0C[];
extern u8 *D_80141AB0[];
extern u8 *D_80141D80[];
extern u32 D_80141DB4[];
extern u32 D_80141E1C[];
extern u8 *D_80141FE0[];
extern u8 *D_80142254[];
extern u32 D_80142288[];
extern u32 D_801422F0[];
extern u8 *D_80142570[];
extern u8 *D_80142798[];
extern u32 D_801427C8[];
extern u32 D_80142828[];
extern u8 *D_801429E0[];
extern u8 *D_80142B40[];
extern u32 D_80142B70[];
extern u32 D_80142BD0[];
extern u8 *D_80142CB0[];
extern u8 *D_80142E70[];
extern u32 D_80142EA0[];
extern u32 D_80142F00[];
extern u8 *D_8014306C[];
extern u8 *D_801431E8[];
extern u32 D_80143218[];
extern u32 D_80143278[];
extern u8 *D_8014333C[];
void func_8013D88C();
extern u8 *D_801450C8;
extern u8 *D_801450CC;
extern u8 *D_801450D0;
extern u8 *D_801450D4;
extern u8 *D_801450D8;
extern u8 *D_801450DC;
extern u8 *D_801450E0;
extern u8 *D_80145454;
extern u8 *D_80145458;
extern u8 *D_8014545C;
extern u8 *D_80145460;
extern u8 *D_80145464;
extern u8 *D_801452C8[10];
extern u32 D_801450E4[];
extern u32 D_8014511C[];
extern u32 D_801452F0[];
extern u32 D_80145340[];
extern u32 D_80145468[];
extern u32 D_801454A0[];
typedef struct {
                           u32 pad : 25;
                            u32 nib : 4;
                       u32 b29 : 1;
                            u32 rest : 2;
} TelEntryFlags;
s32 func_8013ADC0(void);
s32 func_8013BDDC(void);
extern u8 *D_80144FB8;
extern u8 *D_80144FBC;
extern u8 *D_80144FC0;
extern u8 *D_80144FC4;
extern u32 D_80144FC8[];
extern u32 D_80144FE8[];
extern u8 *D_80146904[12];
extern u8 *D_80146BF4[12];
extern u8 *D_80146EE4[12];
extern u32 D_80146934[];
extern u32 D_8014698C[];
extern u32 D_80146C24[];
extern u32 D_80146C7C[];
extern u32 D_80146F14[];
extern u32 D_80146F6C[];
extern u8 *D_80146008[12];
extern u8 *D_801462F0[12];
extern u8 *D_801465B0[12];
extern u32 D_80146038[];
extern u32 D_80146090[];
extern u32 D_80146320[];
extern u32 D_80146378[];
extern u32 D_801465E0[];
extern u32 D_80146638[];
s32 func_8013C3E0(void);
s32 func_8013C8DC(void);
extern u8 *D_8014544C;
extern u8 *D_80145450;
extern u8 *D_80144D6C[12];
extern u32 D_80144D9C[];
extern u32 D_80144DEC[];
extern u8 *D_80144738[12];
extern u8 *D_80144AA4[12];
extern u32 D_80144768[];
extern u32 D_801447B8[];
extern u32 D_80144AD4[];
extern u32 D_80144B24[];
extern u8 **D_80145C44[13];
extern u32 *D_80145C78[13];
extern u32 *D_80145CAC[13];
s32 func_8013A4FC(void);
void func_80139AA4(void);
void func_80139FEC(void);
typedef struct TelClubFlags {
                           u32 pad : 24;
                       u32 b24 : 1;
                            u32 rest : 7;
} TelClubFlags;
typedef struct TelRecordFlags {
                           u32 low : 12;
                       u32 b12 : 1;
                       u32 b13 : 1;
                            u32 middle : 2;
                       u32 b16 : 1;
                            u32 high : 15;
} TelRecordFlags;
void func_80132000(void) {
    switch (D_800E6280.unk_1109) {
    case 0:
        func_80132060();
        return;
    case 1:
        func_80134BE0();
        return;
    default:
        func_80046500();
        return;
    }
}
void func_80132060(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80132138();
        break;
    case 1:
        func_8013226C();
        break;
    case 2:
        func_80132FC4();
        break;
    case 3:
        func_80133D54();
        break;
    case 4:
        func_801345C4();
        break;
    }
    func_80066C08(1);
    func_800646CC();
    func_80064F48();
    func_80064DEC();
    func_80067870();
    func_80066334();
    func_8006BA40();
}
void func_80132138(void) {
    if (*(u8 *)&D_800E6280.unk_110D == 0) {
        func_80051010("Ｔｒｒｒｒ…", 0, 0x2F);
        func_80044750(0x508);
        func_8004EAD4(0);
        D_801230D0 = (func_800AE0D0() & 3) + 1;
        *(u8 *)&D_800E6280.unk_110D = 1;
    } else if (*(u8 *)&D_800E6280.unk_110D == 1) {
        if ((D_800E6280.unk_1104.u & 0x7F) == 1) {
            func_8004EAAC();
            func_80051010("Ｔｒｒｒｒ…", 0, 0x2F);
            func_8004EAD4(0);
            if (D_801230D0 != 0) {
                D_801230D0 -= 1;
            }
            if (D_801230D0 > 0) {
                func_80044750(0x508);
            }
        }
        if (D_801230D0 <= 0 || (D_800E6280.unk_F88 & 0x40)) {
            func_80044750(0x1508);
            func_80044750(0x50B);
            func_8004284C();
        }
    }
    func_80050B54(0);
    func_80069128();
    func_8004EC24();
}
void func_8013226C(void) {
    u8 sp3C[36];
    u8 sp2C[16];
    switch (D_800E6280.unk_110D) {
    case 0x0:
        if (D_800E6280.unk_1BC[13].unk_0C.b[3] == 0) {
            D_800E6280.unk_110D = 1;
        } else if ((appointment_find_today((s32) D_800E6280.unk_03E, (s32) D_800E6280.unk_03F, (s32) D_800E6280.unk_040) > 0) && (((u32) (D_800E6280.unk_54C[0].w * 8) >> 0x1C) == 0xA)) {
            D_800E6280.unk_110D = 0x30;
        } else if (get_g_zyotai_h(0xA) == 4) {
            D_800E6280.unk_110D = 0x30;
        } else if (D_800E6280.unk_1BC[13].unk_0C.b[3] & 1) {
            D_800E6280.unk_110D = 0x10;
        } else {
            D_800E6280.unk_110D = 0x30;
        }
        D_800E6280.unk_1BC[13].unk_0C.b[3] += 1;
        break;
    case 0x1:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("ハイ、早乙女です。", 0, 0xD);
        func_80052060(0x1E000048, 0x0D00004D, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x2:
        func_8005215C();
        break;
    case 0x3:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("もしもし、", 0, 0xF);
        func_800AE0F0(sp3C, D_800E6280.unk_0D4);
        func_800AE100(sp3C, "ですけど。");
        sndi(sp3C, 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x4:
        xa_wait();
        break;
    case 0x5:
        k_sub_reset();
        gnsx(D_800B3628);
        func_800AE0F0(sp3C, "おっ、");
        get_p_name(sp2C, 0xD);
        func_800AE100(sp3C, sp2C);
        func_800AE100(sp3C, "か。");
        sndi(sp3C, 0, 0xD);
        sndi("良くかけて来てくれたな。", 1, 0xD);
        func_80052060((s32) &D_80144E3C, (s32) &D_80144E44, 0, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x6:
        func_8005215C();
        break;
    case 0x7:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("女の子の事なら", 0, 0xF);
        sndi("任せておけって、", 1, 0xF);
        sndi("言ってたよな…。", 2, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x8:
        xa_wait();
        break;
    case 0x9:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("もちろん。", 0, 0xD);
        sndi("何について聞きたいんだ？", 1, 0xD);
        func_80052060(0x180000B8, 0x140000C1, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0xA:
        func_8005215C();
        break;
    case 0xB:
        D_800E6280.unk_110D = 0x20;
        break;
    case 0x10:
        k_sub_reset();
        gnsx(D_800B361C);
        sndi("ハイ、早乙女です。", 0, 0xA);
        func_80052060(0x01000143, 0x1E000149, -1, 0xA);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x11:
        func_8005215C();
        break;
    case 0x12:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("好雄君は、", 0, 0xF);
        sndi("いらっしゃいますか？", 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x13:
        xa_wait();
        break;
    case 0x14:
        k_sub_reset();
        gnsx(D_800B361C);
        sndi("あっ、お兄ちゃんですね。", 0, 0xA);
        sndi("ちょっと待ってて下さい。", 1, 0xA);
        sndi("おにーちゃーん、電話ー！", 2, 0xA);
        func_80052060(0x020001A9, 0x1F0001C4, -1, 0xA);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x15:
        func_8005215C();
        break;
    case 0x16:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("ハイ、早乙女です。", 0, 0xD);
        func_80052060(0x1E000048, 0x0D00004D, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x17:
        func_8005215C();
        break;
    case 0x18:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("もしもし", 0, 0xF);
        sndi(D_800E6280.unk_0D4, 1, 0xF);
        sndi("ですけど。", 2, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x19:
        xa_wait();
        break;
    case 0x1A:
        k_sub_reset();
        gnsx(D_800B3628);
        func_800AE0F0(sp3C, "なんだ、");
        func_800AE100(sp3C, D_800E6280.unk_0D4);
        func_800AE100(sp3C, "か。");
        sndi(sp3C, 0, 0xD);
        func_80052060(0x1E000056, 0x10000059, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x1B:
        func_8005215C();
        break;
    case 0x1C:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("何だその残念そうな声は。", 0, 0xF);
        sndi("俺からじゃ不満か？", 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x1D:
        xa_wait();
        break;
    case 0x1E:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("あたりまえのこと聞くなよ。", 0, 0xD);
        sndi("ところで", 1, 0xD);
        sndi("どうしたんだ今日は？", 2, 0xD);
        func_80052060(0x1400009A, 0x0C0000AB, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x1F:
        func_8005215C();
        break;
    case 0x20:
        k_sub_reset();
        x_taku_string_set("女の子の評価を聞く", "デートスポットを聞く", 0, 2);
        x_taku_menu_set(1, 2);
        k_sub_disp_start(2);
        D_800E6280.unk_110D += 1;
        break;
    case 0x21:
        menu_check(1, D_8011ECF6, D_8011ECFA);
        menu_bar_show(1);
        func_8004FC10(1);
        if (D_800E6280.unk_F88 & 0x20) {
            switch (D_800E6280.unk_1094) {
            case 0:
                D_800E6280.unk_110D = 0x22;
                break;
            case 1:
                func_80042940(2);
                break;
            }
        }
        break;
    case 0x22:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("女の子達が、", 0, 0xF);
        sndi("俺をどう思ってるか", 1, 0xF);
        sndi("知りたいんだ。", 2, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x23:
        xa_wait();
        break;
    case 0x24:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("評価か…。", 0, 0xD);
        sndi("ちょっと待ってくれよ。", 1, 0xD);
        func_80052060(0x19000085, 0x0D00008F, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x25:
        func_8005215C();
        break;
    case 0x26:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("女の子の、", 0, 0xD);
        sndi("お前に対する評価は…。", 1, 0xD);
        func_80052060(0x19000090, 0x0E00009C, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x27:
        func_8005215C();
        break;
    case 0x28:
        func_80042940(3);
        break;
    case 0x30:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("はい、早乙女です。", 0, 0xD);
        func_80052060(0x1E000048, 0x0D00004D, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x31:
        func_8005215C();
        break;
    case 0x32:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("もしもし、", 0, 0xF);
        func_800AE0F0(sp3C, D_800E6280.unk_0D4);
        func_800AE100(sp3C, "ですけど。");
        sndi(sp3C, 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x33:
        xa_wait();
        break;
    case 0x34:
        k_sub_reset();
        gnsx(D_800B3628);
        func_800AE0F0(sp3C, "なんだ、");
        func_800AE100(sp3C, D_800E6280.unk_0D4);
        func_800AE100(sp3C, "か。");
        sndi(sp3C, 0, 0xD);
        func_80052060(0x1E000056, 0x10000059, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x35:
        func_8005215C();
        break;
    case 0x36:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("何だその残念そうな声は。", 0, 0xF);
        sndi("俺からじゃ不満か？", 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x37:
        xa_wait();
        break;
    case 0x38:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("あたりまえのこと聞くなよ。", 0, 0xD);
        sndi("ところで、", 1, 0xD);
        sndi("どうしたんだ今日は？", 2, 0xD);
        func_80052060(0x1400009A, 0x0C0000AB, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x39:
        func_8005215C();
        break;
    case 0x3A:
        D_800E6280.unk_110D = 0x20;
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}
void func_80132FC4(void) {
    u8 sp3C[44];
    s32 var_a0;
    s32 var_a2;
    s32 var_v1;
    var_v1 = 0;
    var_a2 = 0;
    switch (*(u8 *)&D_800E6280.unk_110D) {
    case 0x0:
        for (var_a0 = 0; var_a0 < 21; var_a0++) {
            if (D_800E6280.unk_120[var_a0] & 0x80) {
                var_v1++;
            }
        }
        if (var_v1 != 0) {
            if (D_800E6280.unk_120[0] & 0x80) {
                var_v1--;
                if (var_v1 == 0) {
                    D_800E6280.unk_120[0] = 1;
                }
            }
            for (var_a0 = 1; var_a0 < 21; var_a0++) {
                if (D_800E6280.unk_120[var_a0] & 0x80) {
                    var_v1--;
                    if (var_v1 == 0) {
                        var_a2 = var_a0;
                        D_800E6280.unk_120[var_a0] = 1;
                    }
                }
            }
            if (var_a2 == 1) {
                D_800E6280.unk_110D = 0x10;
                ((CharFlags *)D_800E6280.unk_F64)->b1 = 1;
            } else if (var_a2 == 7) {
                D_800E6280.unk_110D = 0x20;
                ((CharFlags *)D_800E6280.unk_F64)->b2 = 1;
            } else if (var_a2 == 3) {
                D_800E6280.unk_110D = 0x30;
                ((CharFlags *)D_800E6280.unk_F64)->b3 = 1;
            } else if (var_a2 == 0xD) {
                D_800E6280.unk_110D = 0x40;
                ((CharFlags *)D_800E6280.unk_F64)->b4 = 1;
            } else if (var_a2 == 5) {
                D_800E6280.unk_110D = 0x50;
                ((CharFlags *)D_800E6280.unk_F64)->b5 = 1;
            } else if (var_a2 == 0xA) {
                D_800E6280.unk_110D = 0x60;
                ((CharFlags *)D_800E6280.unk_F64)->b6 = 1;
            } else if (var_a2 == 6) {
                D_800E6280.unk_110D = 0x70;
                ((CharFlags *)D_800E6280.unk_F64)->b7 = 1;
            } else if (var_a2 == 0xB) {
                ((CharFlags *)D_800E6280.unk_F64)->b8 = 1;
                D_800E6280.unk_110D = 0x80;
            } else {
                D_800E6280.unk_110D = 1;
            }
        } else {
            D_800E6280.unk_110D = 1;
        }
        break;
    case 0x1:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("いいデートスポットを", 0, 0xF);
        sndi("教えて欲しいんだ。", 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x2:
        xa_wait();
        break;
    case 0x3:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("悪いな。今は特に無いんだ。", 0, 0xD);
        sndi("別の事なら教えてやれるぜ。", 1, 0xD);
        func_80052060(0x140000D0, 0x0F0000E2, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x4:
        func_8005215C();
        break;
    case 0x5:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("女の子の情報でも", 0, 0x1F);
        sndi("聞いてみようかな）", 1, 0x1F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x6:
        xa_wait();
        break;
    case 0x7:
        k_sub_reset();
        func_80051BDC(1, &D_801230D8);
        k_disp_start(D_801230D8 + 1);
        D_800E6280.unk_110D += 1;
        break;
    case 0x8:
        menu_check(1, D_8011ECF6, D_8011ECFA);
        menu_bar_show(1);
        func_8004FC10(1);
        if (D_800E6280.unk_F88 & 0x20) {
            if (D_800E6280.unk_1094 == 0xB) {
                func_80042940(4);
            } else if (D_800E6280.unk_1094 == 0xA) {
                func_80042940(3);
                D_800E6280.unk_110D = 0x20;
            } else if ((D_800E6280.unk_1094 != -1) && (D_800E6280.unk_1BC[D_800E6280.unk_1094].unk_0C.f.b1)) {
                D_800E6280.unk_110D += 1;
            }
        }
        break;
    case 0x9:
        D_800E6280.unk_110D += 1;
        break;
    case 0xA:
        k_sub_reset();
        gnsx(D_800B3628);
        if (D_800E6280.unk_1094 == 0xB) {
            func_800AE0F0(sp3C, D_800B3624);
            func_800AE100(sp3C, "、");
            func_800AE100(sp3C, D_800B3624);
        } else {
            func_800AE0F0(sp3C, girl_short_name_table[D_800E6280.unk_1094]);
            func_800AE100(sp3C, "、");
            func_800AE100(sp3C, girl_short_name_table[D_800E6280.unk_1094]);
        }
        func_800AE100(sp3C, "‥‥と。");
        func_80052060((s32) &D_80144E4C, (s32) &D_80144EAC, (s32) D_800E6280.unk_1094, 0xD);
        sndi(sp3C, 0, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0xB:
        func_8005215C();
        break;
    case 0xC:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("あったあった。", 0, 0xD);
        sndi("耳かっぽじって", 1, 0xD);
        sndi("よく聞けよ。", 2, 0xD);
        func_80052060(0x19000064, 0x0A00006E, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0xD:
        func_8005215C();
        break;
    case 0xE:
        func_80042940(4);
        break;
    case 0x10:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("ちょっと遠いけど、", 0, 0xD);
        sndi("きらめき中央公園なんて", 1, 0xD);
        sndi("わりといい雰囲気だぜ。", 2, 0xD);
        func_80052060(0x15000001, 0x02000015, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x11:
        func_8005215C();
        break;
    case 0x12:
        D_800E6280.unk_110D = 0xF0;
        break;
    case 0x20:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("この間、", 0, 0xD);
        sndi("美術館が出来たんだよ。", 1, 0xD);
        sndi("わりといいらしいぜ。", 2, 0xD);
        func_80052060(0x15000016, 0x03000024, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x21:
        func_8005215C();
        break;
    case 0x22:
        D_800E6280.unk_110D = 0xF0;
        break;
    case 0x30:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("今、水族館が", 0, 0xD);
        sndi("人気あるみたいだぜ。", 1, 0xD);
        func_80052060(0x1900009D, 0x0F0000A7, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x31:
        func_8005215C();
        break;
    case 0x32:
        D_800E6280.unk_110D = 0xF0;
        break;
    case 0x40:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("スタジアムが出来たの", 0, 0xD);
        sndi("知ってたか？", 1, 0xD);
        func_80052060(0x190000A8, 0x100000AF, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x41:
        func_8005215C();
        break;
    case 0x42:
        D_800E6280.unk_110D = 0xF0;
        break;
    case 0x50:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("植物園なんて", 0, 0xD);
        sndi("いいんじゃないか？", 1, 0xD);
        func_80052060(0x190000B0, 0x110000BA, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x51:
        func_8005215C();
        break;
    case 0x52:
        D_800E6280.unk_110D = 0xF0;
        break;
    case 0x60:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("隣町のボーリング場なんて", 0, 0xD);
        sndi("どうだ？", 1, 0xD);
        func_80052060(0x1A000007, 0x0300000F, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x61:
        func_8005215C();
        break;
    case 0x62:
        D_800E6280.unk_110D = 0xF0;
        break;
    case 0x70:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("プラネタリウムなんて", 0, 0xD);
        sndi("暗くていいぜ。", 1, 0xD);
        func_80052060(0x1A000010, 0x0400001B, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x71:
        func_8005215C();
        break;
    case 0x72:
        D_800E6280.unk_110D = 0xF0;
        break;
    case 0x80:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("やっと、この街にも", 0, 0xD);
        sndi("カラオケが出来たんだよ。", 1, 0xD);
        func_80052060(0x1A00001C, 0x05000027, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0x81:
        func_8005215C();
        break;
    case 0x82:
        D_800E6280.unk_110D = 0xF0;
        break;
    case 0xF0:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("サンキュー。", 0, 0xF);
        sndi("いつも悪いな。", 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0xF1:
        xa_wait();
        break;
    case 0xF2:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("お前も", 0, 0xD);
        sndi("まめに情報誌でも読めよ。", 1, 0xD);
        func_80052060(0x1A000028, 0x06000032, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 0xF3:
        func_8005215C();
        break;
    case 0xF4:
        func_8004E58C();
        func_80048E78();
        set_dec_bri(0);
        func_80072B5C(0);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}
void func_80133D54(void) {
    u8 sp5C[44];
    u8 sp3C[32];
    TelRec44 *var_s0;
    s32 var_s1;
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004E58C();
        func_8013CCD0();
        k_sub_reset_point_set();
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("こんなとこだな。", 0, 0xD);
        func_80052060(0x1E00005E, 0x12000061, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 2:
        func_8005215C();
        break;
    case 3:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("女の子の情報でも", 0, 0x1F);
        sndi("聞いてみようかな）", 1, 0x1F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 4:
        xa_wait();
        break;
    case 5:
        k_sub_reset();
        func_80051BDC(1, &D_801230D8);
        k_disp_start(D_801230D8 + 1);
        D_800E6280.unk_110D += 1;
        break;
    case 6:
        menu_check(1, D_8011ECF6, D_8011ECFA);
        menu_bar_show(1);
        func_8004FC10(1);
        if ((D_800E6280.unk_F88 & 0x20) && (D_800E6280.unk_1094 != -1)) {
            if (D_800E6280.unk_1094 == 0xB) {
                D_800E6280.unk_110D += 1;
            } else if (D_800E6280.unk_1BC[D_800E6280.unk_1094].unk_0C.f.b1) {
                D_800E6280.unk_110D += 1;
            }
        }
        break;
    case 7:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        if (D_800E6280.unk_1094 == 0xB) {
            get_g_name(sp3C, 0xC);
        } else {
            get_g_name(sp3C, D_800E6280.unk_1094);
        }
        func_800AE0F0(sp5C, sp3C);
        func_800AE100(sp5C, "の");
        sndi(sp5C, 0, 0xF);
        sndi("ことについて", 1, 0xF);
        sndi("知りたいんだけど。", 2, 0xF);
        k_sub_disp_start(0);
        if (D_800E6280.unk_1094 == 0xA) {
            D_800E6280.unk_110D = 0x20;
        } else if (D_800E6280.unk_1094 == 0xB) {
            func_80042940(4);
        } else {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 8:
        xa_wait();
        break;
    case 9:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("あぁ、いいぜ。", 0, 0xD);
        sndi("ちょっと待ってくれよ。", 1, 0xD);
        func_80052060(0x180000C2, 0x150000C9, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 10:
        func_8005215C();
        break;
    case 11:
        k_sub_reset();
        gnsx(D_800B3628);
        if (D_800E6280.unk_1094 == 0xB) {
            func_800AE0F0(sp5C, D_800B3624);
            func_800AE100(sp5C, "、");
            func_800AE100(sp5C, D_800B3624);
        } else {
            func_800AE0F0(sp5C, girl_short_name_table[D_800E6280.unk_1094]);
            func_800AE100(sp5C, "、");
            func_800AE100(sp5C, girl_short_name_table[D_800E6280.unk_1094]);
        }
        func_800AE100(sp5C, "‥‥と。");
        sndi(sp5C, 0, 0xD);
        func_80052060((s32) &D_80144E4C, (s32) &D_80144EAC, (s32) D_800E6280.unk_1094, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 12:
        func_8005215C();
        break;
    case 13:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("あったあった。", 0, 0xD);
        sndi("耳かっぽじって", 1, 0xD);
        sndi("よく聞けよ。", 2, 0xD);
        func_80052060(0x19000064, 0x0A00006E, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 14:
        func_8005215C();
        break;
    case 15:
        func_80042940(4);
        break;
    case 32:
        xa_wait();
        break;
    case 33:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("何だ、お前。", 0, 0xD);
        sndi("優美に気があるのか？", 1, 0xD);
        func_80052060(0x1900006F, 0x0B00007A, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 34:
        func_8005215C();
        break;
    case 35:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("い、いや。", 0, 0xF);
        sndi("そう言う訳じゃないけど…。", 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 36:
        xa_wait();
        break;
    case 37:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("別に隠さなくてもいいよ。", 0, 0xD);
        sndi("へぇ、お前が優美をねぇ。", 1, 0xD);
        sndi("よし、特別に教えてやろう。", 2, 0xD);
        func_80052060(0x120000C7, 0x090000E5, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 38:
        func_8005215C();
        break;
    case 39:
        func_80042940(4);
        break;
    }
    var_s0 = &((TelRec44 *)D_8011ECD0)[0x61];
    var_s1 = 0x61;
    do {
        if (var_s0->unk_18 == 0x60) {
            var_s0->unk_1C = (s16) ((func_800A0070((D_800E6280.unk_1104.w + var_s1) << 6) / 3) + 0xAAA);
            var_s0->unk_1A = (s16) ((func_800A0070((D_800E6280.unk_1104.w + var_s1) * 0x5A) / 4) + 0xC00);
            var_s0->unk_30 = (s32) (func_800A0070(D_800E6280.unk_1104.w << 6) * 4);
            var_s0->unk_03 = (u8) (var_s0->unk_03 | 0x10);
        }
        var_s1 += 1;
        var_s0++;
    } while (var_s1 != 0x82);
    if (D_80120668 == 0x3F) {
        cal_base_show();
        return;
    }
    func_80069178();
}
void func_801345C4(void) {
    u8 state;
    switch (D_800E6280.unk_110D) {
    case 0:
        cd_read_sectors(0x26U, 0x801A0000, 0x9D5F);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        }
        break;
    case 2:
        func_8004E58C();
        func_8013D03C((s32) D_800E6280.unk_1094, (s32) &D_801230D4);
        k_sub_reset_point_set();
        k_sub_disp_start(0);
        if (D_800E6280.unk_1094 == 0xA) {
            D_800E6280.unk_110D = 0x20;
        }
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 3:
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 4:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("って、とこだな。", 0, 0xD);
        func_80052060(0x1E00005A, 0x1100005D, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 5:
        func_8005215C();
        break;
    case 6:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("サンキュー。", 0, 0xF);
        sndi("いつも悪いな。", 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 7:
        xa_wait();
        break;
    case 8:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("なーに、いいってことよ。", 0, 0xD);
        sndi("女の子の情報なら", 1, 0xD);
        sndi("俺に任せてくれよ。", 2, 0xD);
        func_80052060(0x140000BE, 0x0E0000CF, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 9:
        func_8005215C();
        break;
    case 10:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 11:
        xa_wait();
        break;
    case 12:
        func_8004E58C();
        func_80048E78();
        set_dec_bri(0);
        func_80072B5C(0);
        break;
    case 32:
        xa_wait();
        break;
    case 33:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("って、とこだな。", 0, 0xD);
        func_80052060(0x1E00005A, 0x1100005D, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 34:
        func_8005215C();
        break;
    case 35:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("サンキュー。", 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 36:
        xa_wait();
        break;
    case 37:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("なんなら、", 0, 0xD);
        sndi("俺が優美に", 1, 0xD);
        sndi("言ってやろうか？", 2, 0xD);
        func_80052060(0x1900007B, 0x0C000084, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 38:
        func_8005215C();
        break;
    case 39:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("だから、", 0, 0xF);
        sndi("そんなんじゃないって。", 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 40:
        xa_wait();
        break;
    case 41:
        k_sub_reset();
        gnsx(D_800B3628);
        sndi("まぁ、いいか。", 0, 0xD);
        sndi("じゃ、頑張れよ。", 1, 0xD);
        func_80052060(0x0A0000E3, 0x0E0000EA, -1, 0xD);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 42:
        func_8005215C();
        break;
    case 43:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 44:
        xa_wait();
        break;
    case 45:
        func_8004E58C();
        func_80048E78();
        set_dec_bri(0);
        func_80072B5C(0);
        break;
    }
    if (D_80120668 == 0x3F) {
        cal_base_show();
        return;
    }
    func_80069178();
}
void func_80134BE0(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0x0:
        func_80134DA0();
        break;
    case 0x1:
        func_80134FE4();
        break;
    case 0x2:
        func_80135FFC();
        break;
    case 0x3:
        func_801352F0();
        break;
    case 0x4:
        func_801360E4();
        break;
    case 0x8:
        func_8013586C();
        break;
    case 0x9:
        func_80135A44();
        break;
    case 0xA:
        func_80135C4C();
        break;
    case 0x5:
        func_801361CC();
        break;
    case 0x7:
        func_80135E28();
        break;
    case 0x6:
        func_801362B4();
        break;
    case 0x10:
        func_801396EC();
        break;
    case 0x30:
        func_80136724();
        break;
    case 0x20:
        func_8013639C();
        break;
    case 0x40:
        func_801380D8();
        break;
    case 0x41:
        func_801384F0();
        break;
    case 0x42:
        func_8013883C();
        break;
    case 0x43:
        func_80138D20();
        break;
    case 0x44:
        func_80139064();
        break;
    case 0x45:
        func_801393A8();
        break;
    }
    func_80066C08(1);
    func_800646CC();
    func_80064F48();
    func_80064DEC();
    func_80067870();
    func_8006BA40();
}
void func_80134DA0(void) {
    if (*(u8 *)&D_800E6280.unk_110D == 0) {
        func_80051010("Ｔｒｒｒｒ…", 0, 0x2F);
        func_80044750(0x508);
        func_8004EAD4(0);
        D_801230D0 = (func_800AE0D0() & 3) + 1;
        *(u8 *)&D_800E6280.unk_110D = 1;
        if (*(u8 *)&D_800E6280.unk_F5F == 0xB) {
            D_801405E4 = 0xC;
        } else {
            D_801405E4 = *(u8 *)&D_800E6280.unk_F5F;
        }
    } else if (*(u8 *)&D_800E6280.unk_110D == 1) {
        if ((D_800E6280.unk_1104.u & 0x7F) == 1) {
            func_8004EAAC();
            func_80051010("Ｔｒｒｒｒ…", 0, 0x2F);
            func_8004EAD4(0);
            if (D_801230D0 > 0) {
                func_80044750(0x508);
            }
            if (D_801230D0 != 0) {
                D_801230D0 -= 1;
            }
        }
        if (D_801230D0 <= 0 || (D_800E6280.unk_F88 & 0x40)) {
            func_80044750(0x1508);
            func_80044750(0x50B);
            if (appointment_find_today(*(u8 *)&D_800E6280.unk_03E, *(u8 *)&D_800E6280.unk_03F, *(u8 *)&D_800E6280.unk_040) != 0 &&
                *(u8 *)&D_800E6280.unk_F5F == (u32)(D_800E6280.unk_54C[0].w << 3) >> 0x1C) {
                func_80042940(0x10);
            } else if (*(u8 *)&D_800E6280.unk_03E == 0x62 && *(u8 *)&D_800E6280.unk_03F == 2 && *(u8 *)&D_800E6280.unk_040 == 0x16) {
                func_80042940(0x10);
            } else if (func_8013655C() == 0) {
                func_80042940(0x10);
            } else if (D_801405E4 == 0xC) {
                func_80042940(0x40);
            } else {
                func_8004284C();
            }
        }
    }
    func_80050B54(0);
    func_80069128();
    func_8004EC24();
}
INCLUDE_ASM("asm/ovl/TEL/nonmatchings/TEL/80132000",func_80134FE4);
void func_801352F0(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        gnsx(D_800B3610);
        sndi("ああ、脇役君ね。", 0, 7);
        func_80052060(0x05000000, 0x0100000B, -1, 7);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi("違うよ。", 0, 0xF);
        sndi("Pだよ。", 1, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        sndisp(D_801434D4[D_801405E4], 0, (s32) D_801405E4);
        func_80052060((s32) &D_80143500, (s32) &D_80143558, (s32) D_801405E4, (s32) D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 5:
        func_8005215C();
        break;
    case 6:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_801437BC[D_801405E4], 0, 0x1F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 7:
        xa_wait();
        break;
    case 8:
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        sndisp(D_80143958[D_801405E4], 0, (s32) D_801405E4);
        func_80052060((s32) &D_80143984, (s32) &D_801439DC, (s32) D_801405E4, (s32) D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 9:
        func_8005215C();
        break;
    case 10:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp("あっ、 ちょっと待って。", 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
    case 11:
        xa_wait();
        break;
    case 12:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp("俺は、とりあえず Gの機嫌を とることにした。）", 0, 0x1F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
    case 13:
        xa_wait();
        break;
    case 14:
        k_sub_reset();
        sndisp("　　　　　・ 　　　　・ 　　　　・", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 15:
        xa_wait();
        break;
    case 16:
        D_800E6280.unk_110D = 0x14;
        break;
    case 20:
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        sndisp(D_80143BA0[D_801405E4], 0, (s32) D_801405E4);
        func_80052060((s32) &D_80143BCC, (s32) &D_80143C24, (s32) D_801405E4, (s32) D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 21:
        func_8005215C();
        break;
    case 22:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 23:
        xa_wait();
        break;
    case 24:
        k_sub_reset();
        func_80044750(0x50C);
        gnsx(D_800E6280.unk_0D4);
        sndisp("良かった。 何とか、Gの機嫌も 直ったみたいだぞ。）", 0, 0x1F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 25:
        xa_wait();
        break;
    default:
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 = 0xF;
        D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 = 0xF;
        func_80048E78();
        func_8004E58C();
        set_dec_bri(0);
        func_80072B5C(0);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}
void func_8013586C(void) {
    u8 girl;
    u32 *table;
    switch (D_800E6280.unk_110D) {
    case 0:
        table = D_80141084;
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        sndisp(D_8014107C, 0, (s32) D_801405E4);
        girl = D_801405E4;
        func_80052060((s32) table, (s32) table, 0, (s32) girl);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_80141080, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        sndisp(D_80140AD8[D_801405E4], 0, (s32) D_801405E4);
        girl = D_801405E4;
        func_80052060((s32) D_80140B04, (s32) D_80140B5C, (s32) girl, (s32) girl);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 5:
        func_8005215C();
        break;
    default:
        func_80042940(0x20);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}
void func_80135A44(void) {
    u32 sp28;
    u32 temp_hi;
    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        temp_hi = (u32) ((u8) D_800E6280.unk_1BC[D_801405E4].unk_0C.b[3] >> 1) % 3U;
        sp28 = temp_hi;
        sndisp(D_801410BC[temp_hi], 0, (s32) D_801405E4);
        func_80052060((s32) &D_801410D4, (s32) &D_80141104, (s32) sp28, (s32) D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_801410C8, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        sndisp(D_801410CC, 0, (s32) D_801405E4);
        func_80052060((s32) &D_801410D4, (s32) &D_80141104, 4, (s32) D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 5:
        func_8005215C();
        break;
    default:
        func_80042940(0x20);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}
void func_80135C4C(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        sndisp(D_80141094, 0, (s32) D_801405E4);
        func_80052060((s32) &D_8014109C, (s32) &D_801410AC, 0, (s32) D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_80141098, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        sndisp(D_80140AD8[D_801405E4], 0, (s32) D_801405E4);
        func_80052060((s32) D_80140B04, (s32) D_80140B5C, (s32) D_801405E4, (s32) D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 5:
        func_8005215C();
        break;
    default:
        func_80042940(0x20);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}
void func_80135E28(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        sndisp(D_80141040, 0, (s32) D_801405E4);
        func_80052060((s32) &D_8014104C, (s32) &D_80141064, 0, (s32) D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_80141044, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        k_sub_reset();
        gnsx(girl_short_name_table[D_801405E4]);
        sndisp(D_80141048, 0, (s32) D_801405E4);
        func_80052060((s32) &D_8014104C, (s32) &D_80141064, 2, (s32) D_801405E4);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 5:
        func_8005215C();
        break;
    default:
        func_80042940(0x20);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}
void func_80135FFC(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_80050DFC(girl_short_name_table[D_801405E4]);
        func_80050E8C(D_80140AD8[D_801405E4], 0, D_801405E4);
        func_80052060(D_80140B04, D_80140B5C, D_801405E4, D_801405E4);
        func_8004EAD4(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    default:
        func_80042940(0x20);
        break;
    }
    func_80050B54(0);
    func_80069128();
}
void func_801360E4(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_80050DFC(girl_short_name_table[D_801405E4]);
        func_80050E8C(D_80140AD8[D_801405E4], 0, D_801405E4);
        func_80052060(D_80140B04, D_80140B5C, D_801405E4, D_801405E4);
        func_8004EAD4(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    default:
        func_80042940(0x20);
        break;
    }
    func_80050B54(0);
    func_80069128();
}
void func_801361CC(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_80050DFC(girl_short_name_table[D_801405E4]);
        func_80050E8C(D_80140D18[D_801405E4], 0, D_801405E4);
        func_80052060(D_80140D44, D_80140D9C, D_801405E4, D_801405E4);
        func_8004EAD4(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    default:
        func_80042940(0x20);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}
void func_801362B4(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_80050DFC(girl_short_name_table[D_801405E4]);
        func_80050E8C(D_80140F0C[D_801405E4], 0, D_801405E4);
        func_80052060(D_80140F38, D_80140F90, D_801405E4, D_801405E4);
        func_8004EAD4(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    default:
        func_80042940(0x20);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}
void func_8013639C(void) {
    u32 choice;
    switch (D_800E6280.unk_110D) {
    case 0:
        func_80067E34();
        cal_sprite_init();
        func_8013655C();
        func_8004E58C();
        set_kanji_string(-0x80, 0x32, 0, (s32) "デートする日を選んで下さい", 0);
        k_disp_start(1);
        D_800E6280.unk_110D += 1;
        func_80050B00(1, 0x2F7FAF, 5, 0);
        break;
    case 1:
        menu_check(1, D_8011ECF6, D_8011ECFA);
        func_8004FC10(1);
        menu_bar_show(1);
        choice = D_800E6280.unk_1094;
        if ((choice != -1) && (D_800E6280.unk_F88 & 0x20)) {
            func_80069218(0);
            D_80140558 = D_801405A4[D_800E6280.unk_1094];
            choice = D_800E6280.unk_1094;
            D_80140554 = D_80140564[choice];
            if ((D_800E6280.unk_03F == 0xC) && (D_80140558 == 1)) {
                D_8014055C = D_800E6280.unk_03E + 1;
            } else {
                D_8014055C = (s32) D_800E6280.unk_03E;
            }
            D_800E6280.unk_110D += 1;
        }
        break;
    default:
        func_80050B00(0x10, 0x2F2F7F, 3, 0);
        func_80042940(0x30);
        break;
    }
    func_80068EC0();
}
s32 func_8013655C(void) {
    s32 var_s6;
    s16 spEC[16];
    s16 spCC[16];
    s16 spAC[16];
    s16 sp8C[16];
    s32 var_a3;
    s32 var_t0;
    u8 temp_v0;
    u32 var_s0;
    var_s0 = D_800E6280.unk_040;
    var_t0 = 0;
    var_s6 = 0;
    var_a3 = 0;
    do {
        var_s0 += 1;
        if ((u8) D_800E6280.unk_044[0].unk_00[0] < var_s0) {
            var_s0 = 1;
            var_s6 = 1;
        }
        temp_v0 = D_800E6280.unk_044[var_s6].unk_00[var_s0];
        if ((temp_v0 == 1) || (temp_v0 == 2)) {
            if (appointment_find_today((s32) D_800E6280.unk_03E, D_800E6280.unk_03F + var_s6, (s32) var_s0) == 0) {
                (&spEC[0])[var_t0] = func_80068898(var_s6, (var_s0 - 1)) - 0x10;
                (&spCC[0])[var_t0] = func_800688F0(var_s6, (var_s0 - 1)) - 0x10;
                (&spAC[0])[var_t0] = 0x10;
                (&sp8C[0])[var_t0] = 0x10;
                D_80140564[var_t0] = (s32) var_s0;
                D_801405A4[var_t0] = var_s6 + D_800E6280.unk_03F;
                var_t0++;
            }
        }
        var_a3 += 1;
    } while (var_a3 != 0x1C);
    func_8004F870(1, var_t0 & 0xFF, &spEC[0], &spCC[0], &spAC[0], &sp8C[0]);
    D_800E6280.unk_1094 = 0;
    return var_t0;
}
extern s16 D_800E6392;
extern s16 D_800E6396;
extern s16 D_800E639A;
extern s16 D_800E639E;
extern s32 D_800E7208;
extern s32 D_800E7378;
extern s8 D_800E7313;
extern u32 D_800E7384;
extern u8 D_800E62BF;
extern u8 D_800E6354;
extern u8 D_800E71DF;
extern u8 D_800E738D;
s32 func_800AE100();
extern s32 sp;
void func_80136724(void) {
    s32 sp17C;
    u8 sp78[0x104];
    u8 sp68[16];
    s16 sp48[16];
    GsWord sp44;
    s32 temp_a0;
    s32 temp_a3;
    s32 temp_a3_2;
    s32 temp_a3_3;
    s32 temp_lo;
    s32 temp_t3;
    s32 temp_v0_2;
    s32 var_a2;
    s32 var_a2_2;
    s32 var_t2;
    s32 var_v1_2;
    u32 temp_t2;
    u32 var_v1;
    u8 *temp_a0_2;
    u8 *temp_a0_3;
    u8 *temp_a0_4;
    u8 temp_t2_2;
    u8 temp_t3_2;
    u8 temp_t4;
    u8 temp_t7;
    u8 temp_v0;
    u8 var_t1;
    cal_base_show();
    menu_bar_show(0);
    switch ((u8) D_800E738D) {
    case 0x0:
        func_8004E58C();
        func_80048E78();
        func_8013D88C(0, D_8014055C, D_80140558, D_80140554);
        k_sub_reset_point_set();
        set_kanji_string(-0x80, 0x32, 0, (s32) "デートする場所を選んで下さい", 0);
        k_disp_start(check_set_k());
        D_800E738D += 1;
        break;
    case 0x1:
        menu_check(0, D_8011ECF6, D_8011ECFA);
        func_8004FC10(0);
        if (((u32) D_800E7384 >= 0x11U) && (D_800E7208 & 0x20)) {
            if (D_800E7313 == 0xA) {
                func_80044750(0x50D);
                D_800E738D = 2;
            } else if (D_800E7313 >= 0) {
                D_800E738D = 4;
                D_80140550 = (s32) D_800E7313;
            }
        }
        break;
    case 0x2:
        func_8004E58C();
        func_80048E78();
        func_8013D88C(1, D_8014055C, D_80140558, D_80140554);
        k_sub_reset_point_set();
        set_kanji_string(-0x80, 0x32, 0, (s32) "デートする場所を選んで下さい", 0);
        k_disp_start(check_set_k());
        D_800E738D += 1;
        break;
    case 0x3:
        menu_check(0, D_8011ECF6, D_8011ECFA);
        func_8004FC10(0);
        menu_bar_show(0);
        if (((u32) D_800E7384 >= 0x11U) && (D_800E7208 & 0x20)) {
            if (D_800E7313 == 0xA) {
                func_80044750(0x50D);
                D_800E738D = 0;
            } else if (D_800E7313 >= 0) {
                D_800E738D = 4;
                if (D_800E7313 < 6) {
                    D_80140550 = D_800E7313 + 0xA;
                }
                if ((D_800E7313 == 6) || (D_800E7313 == 7)) {
                    if ((D_800E62BF == 6) || (D_800E62BF == 7) || (D_800E62BF == 8)) {
                        D_80140550 = D_800E7313 + 0xA;
                    } else {
                        D_80140550 = D_800E7313 + 0xD;
                    }
                }
                if (D_800E7313 == 9) {
                    D_80140550 = 0x12;
                }
            }
        }
        break;
    case 0x4:
        k_sub_reset();
        gnsx(&D_800E6354);
        var_a2 = D_80140558;
        if (var_a2 == 0xD) {
            var_a2 = 1;
        }
        D_80140558 = var_a2;
        func_800AE0F0(sp78, D_800B3288[var_a2]);
        func_800AE100(sp78, "月");
        temp_a3 = D_80140554;
        if (temp_a3 >= 0xA) {
            func_800AE100(sp78, D_800B3288[temp_a3 / 10]);
        }
        func_800AE100(sp78, D_800B3288[temp_a3 % 10]);
        func_800AE100(sp78, "日");
        func_800AE100(sp78, "に、 ");
        func_800AE100(sp78, D_800B342C[D_80140550]);
        func_800AE100(sp78, "へ 行かない？");
        sndisp(sp78, 0, 0xF);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x5:
        xa_wait();
        break;
    case 0x6:
        func_80053418((s32) &sp68, (s32) &sp48);
        temp_t2 = (u8) *(u8 *)(sp68 + D_800E71DF) >> 2;
        temp_t3 = (s16) D_800E6392 / 4;
        if (temp_t3 < (s32) temp_t2) {
            var_v1 = temp_t2;
        } else {
            var_v1 = (u32) temp_t3;
        }
        temp_v0 = (*D_80141134)[(D_800E71DF * 0x15) + D_80140550];
        if (temp_v0 != 0) {
            var_v1_2 = temp_v0 + var_v1;
        } else {
            var_v1_2 = 0;
        }
        if (D_800E6396 < 0x28) {
            var_v1_2 = (D_800E6396 + var_v1_2) - 0x28;
        }
        sp17C = var_v1_2;
        temp_a0 = func_800AE0D0() & 0xFF;
        temp_v0_2 = var_v1_2 - temp_a0;
        if (temp_a0 < var_v1_2) {
            D_800E738D = 0x20;
        } else {
            if (temp_v0_2 < 0x66) {
                D_800E738D = 0x40;
            } else if (temp_v0_2 < 0xB3) {
                D_800E738D = 0x60;
            } else {
                D_800E738D = 0x80;
            }
            D_800E639E += 9;
            D_800E639A += func_800AE0D0() & 7;
            parameter_show_init();
            parameter_show_init();
        }
        D_800E6280.unk_1BC[D_800E71DF].unk_06 += D_800E7378 & 3;
        D_800E6280.unk_1BC[D_800E71DF].unk_0A -= 0xC;
        if (D_800E6280.unk_1BC[D_800E71DF].unk_0A < 0) {
            D_800E6280.unk_1BC[D_800E71DF].unk_0A = 0;
        }
        if (D_800E71DF == 6) {
            if ((u32) (get_g_zyotai_s(D_800E71DF) & 0x7F) < 2U) {
                D_801405E4 = 0xB;
            } else {
                D_801405E4 = D_800E71DF;
            }
        } else {
            D_801405E4 = D_800E71DF;
        }
        break;
    case 0x20:
        k_sub_reset();
        sp44.h[0] = (D_80140550 & 0x1F) | (sp44.h[0] & 0xFFE0);
        sp44.h[0] = ((D_8014055C << 5) & 0xFE0) | (sp44.h[0] & 0xF01F);
        temp_t4 = (D_80140554 & 0x1F) | (sp44.b[2] & 0xFFE0);
        sp44.b[1] = (D_80140558 * 0x10) | (sp44.b[1] & 0xFF0F);
        sp44.b[2] = temp_t4;
        sp44.b[2] = (D_80140560 << 5) | (temp_t4 & 0x1F);
        temp_t2_2 = sp44.b[3] & 0xFFFE;
        temp_t7 = ((D_800E71DF * 2) & 0x1E) | (temp_t2_2 & 0xE1);
        sp44.b[3] = temp_t2_2;
        temp_t3_2 = temp_t7 | 0x20;
        sp44.b[3] = temp_t7;
        sp44.b[3] = temp_t3_2;
        sp44.b[3] = temp_t3_2 & 0x3F;
        func_8006B5BC(sp44);
        func_8006B0C8();
        gnsx(girl_short_name_table[D_800E71DF]);
        sndisp(D_801413C8[D_801405E4], 0, (s32) D_800E71DF);
        func_80052060((s32) D_801413F8, (s32) D_80141458, (s32) D_801405E4, (s32) D_800E71DF);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x21:
        func_8005215C();
        break;
    case 0x22:
        k_sub_reset();
        gnsx(&D_800E6354);
        func_800AE0F0(sp78, "それじゃ、");
        var_a2_2 = D_80140558;
        if (var_a2_2 == 0xD) {
            var_a2_2 = 1;
        }
        D_80140558 = var_a2_2;
        func_800AE100(sp78, D_800B3288[var_a2_2], var_a2_2);
        func_800AE100(sp78, "月");
        temp_a3_2 = D_80140554;
        if (temp_a3_2 >= 0xA) {
            func_800AE100(sp78, D_800B3288[temp_a3_2 / 10]);
        }
        func_800AE100(sp78, D_800B3288[temp_a3_2 % 10]);
        func_800AE100(sp78, "日");
        func_800AE100(sp78, "に、 ");
        func_800AE100(sp78, D_800B342C[D_80140550]);
        func_800AE100(sp78, "の前で 待ち合わせということで。");
        sndisp(sp78, 0, 0xF);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x23:
        xa_wait();
        break;
    case 0x24:
        k_sub_reset();
        gnsx(girl_short_name_table[D_800E71DF]);
        sndisp(D_80141680[D_801405E4], 0, (s32) D_800E71DF);
        func_80052060((s32) D_801416B0, (s32) D_80141710, (s32) D_801405E4, (s32) D_800E71DF);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x25:
        func_8005215C();
        break;
    case 0x26:
        k_sub_reset();
        gnsx(&D_800E6354);
        temp_a0_2 = D_801418D0[D_801405E4];
        if (*temp_a0_2 == 0x20) {
            D_800E738D = 0x2D;
        } else {
            sndisp(temp_a0_2, 0, 0xF);
            k_sub_disp_start(0);
            D_800E738D += 1;
        }
        break;
    case 0x27:
        xa_wait();
        break;
    case 0x28:
        k_sub_reset();
        gnsx(girl_short_name_table[D_800E71DF]);
        temp_a0_3 = D_8014197C[D_801405E4];
        if (*temp_a0_3 == 0x20) {
            D_800E738D = 0x2D;
        } else {
            sndisp(temp_a0_3, 0, (s32) D_800E71DF);
            k_sub_disp_start(0);
            D_800E738D += 1;
        }
        func_80052060((s32) D_801419AC, (s32) D_80141A0C, (s32) D_801405E4, (s32) D_800E71DF);
        break;
    case 0x29:
        func_8005215C();
        break;
    case 0x2A:
        k_sub_reset();
        gnsx(&D_800E6354);
        temp_a0_4 = D_80141AB0[D_801405E4];
        if (*temp_a0_4 == 0x20) {
            D_800E738D = 0x2D;
        } else {
            sndisp(temp_a0_4, 0, 0xF);
            k_sub_disp_start(0);
            D_800E738D += 1;
        }
        break;
    case 0x2B:
        xa_wait();
        break;
    case 0x2C:
        D_800E738D = 0x2D;
        break;
    case 0x2D:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x2E:
        xa_wait();
        break;
    case 0x2F:
        k_sub_reset();
        func_80044750(0x50C);
        gnsx(&D_800E6354);
        func_800AE0F0(sp78, "やったー。 ");
        func_800AE100(sp78, D_800B3288[D_80140558], D_80140558);
        func_800AE100(sp78, "月");
        temp_a3_3 = D_80140554;
        if (temp_a3_3 >= 0xA) {
            func_800AE100(sp78, D_800B3288[temp_a3_3 / 10]);
        }
        func_800AE100(sp78, D_800B3288[temp_a3_3 % 10]);
        func_800AE100(sp78, "日");
        func_800AE100(sp78, "が、楽しみだぜ。 ");
        sndisp(sp78, 0, 0xF);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x30:
        xa_wait();
        break;
    case 0x31:
        D_800E738D = 0xFF;
        break;
    case 0x40:
        k_sub_reset();
        gnsx(girl_short_name_table[D_800E71DF]);
        if ((D_800E71DF == 6) && ((u32) (get_g_zyotai_s(D_800E71DF) & 0x7F) < 2U)) {
            D_801405E4 = 0xB;
        }
        var_t1 = D_801405E4;
        if (D_800E71DF == 4) {
            temp_lo = (s32) (D_80140554 + 6) / 7;
            if ((temp_lo == 3) && (D_80140558 == D_800E62BF) && (D_800E6280.unk_044[0].unk_00[D_80140554] == 1)) {
                goto block_108;
            }
            if ((temp_lo == 3) && (D_80140558 != D_800E62BF)) {
                var_t2 = var_t1 * 4;
                if (D_800E6280.unk_044[1].unk_00[D_80140554] == 1) {
block_108:
                    var_t1 = 0xC;
                    goto block_109;
                }
            } else {
                goto block_109;
            }
        } else {
block_109:
            var_t2 = var_t1 * 4;
        }
        D_801405E4 = var_t1;
        sndisp(*(D_80141D80 + var_t2), 0, (s32) D_800E71DF);
        k_sub_disp_start(0);
        func_80052060((s32) D_80141DB4, (s32) D_80141E1C, (s32) D_801405E4, (s32) D_800E71DF);
        D_800E738D += 1;
        break;
    case 0x41:
        func_8005215C();
        break;
    case 0x42:
        k_sub_reset();
        gnsx(&D_800E6354);
        sndisp(D_80141FE0[D_801405E4], 0, 0xF);
        k_sub_disp_start(0);
        D_800E738D += 1;
    case 0x43:
        xa_wait();
        break;
    case 0x44:
        k_sub_reset();
        gnsx(girl_short_name_table[D_800E71DF]);
        sndisp(D_80142254[D_801405E4], 0, (s32) D_800E71DF);
        func_80052060((s32) D_80142288, (s32) D_801422F0, (s32) D_801405E4, (s32) D_800E71DF);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x45:
        func_8005215C();
        break;
    case 0x46:
        k_sub_reset();
        gnsx(&D_800E6354);
        sndisp(D_80142570[D_801405E4], 0, 0xF);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x47:
        xa_wait();
        break;
    case 0x48:
        if (D_800E71DF == 9) {
            D_800E738D = 0x4D;
        } else {
            k_sub_reset();
            func_80044750(0x50B);
            sndi("ガチャ", 0, 0x2F);
            k_sub_disp_start(0);
            D_800E738D += 1;
        }
        break;
    case 0x49:
        xa_wait();
        break;
    case 0x4A:
        k_sub_reset();
        gnsx(&D_800E6354);
        func_80044750(0x50C);
        if (D_800E71DF == 5) {
            sndisp("言葉うつっちゃったよ。）", 0, 0x1F);
        } else {
            sndisp("くそ、次こそ絶対 誘ってみせるぜ。）", 0, 0x1F);
        }
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x4B:
        xa_wait();
        break;
    case 0x4C:
        D_800E738D = 0xFF;
        break;
    case 0x4D:
        k_sub_reset();
        gnsx(girl_short_name_table[D_800E71DF]);
        sndisp("あの…。 また今度、 誘って下さい…。", 0, (s32) D_800E71DF);
        func_80052060(0x02000163, 0x19000170, -1, (s32) D_800E71DF);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x4E:
        func_8005215C();
        break;
    case 0x4F:
        k_sub_reset();
        gnsx(&D_800E6354);
        sndisp("うん、そうするよ。 じゃあ。", 0, 0xF);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x50:
        xa_wait();
        break;
    case 0x51:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x52:
        xa_wait();
        break;
    case 0x53:
        D_800E738D = 0x4A;
        break;
    case 0x60:
        k_sub_reset();
        gnsx(girl_short_name_table[D_800E71DF]);
        sndisp(D_80142798[D_801405E4], 0, (s32) D_800E71DF);
        func_80052060((s32) D_801427C8, (s32) D_80142828, (s32) D_801405E4, (s32) D_800E71DF);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x61:
        func_8005215C();
        break;
    case 0x62:
        k_sub_reset();
        gnsx(&D_800E6354);
        sndisp(D_801429E0[D_801405E4], 0, 0xF);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x63:
        xa_wait();
        break;
    case 0x64:
        k_sub_reset();
        gnsx(girl_short_name_table[D_800E71DF]);
        sndisp(D_80142B40[D_801405E4], 0, (s32) D_800E71DF);
        func_80052060((s32) D_80142B70, (s32) D_80142BD0, (s32) D_801405E4, (s32) D_800E71DF);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x65:
        func_8005215C();
        break;
    case 0x66:
        if (*D_80142CB0[D_801405E4] == 0x20) {
            D_800E738D = 0x68;
        } else {
            k_sub_reset();
            gnsx(&D_800E6354);
            sndisp(D_80142CB0[D_801405E4], 0, 0xF);
            k_sub_disp_start(0);
            D_800E738D += 1;
        }
        break;
    case 0x67:
        xa_wait();
        break;
    case 0x68:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x69:
        xa_wait();
        break;
    case 0x6A:
        k_sub_reset();
        func_80044750(0x50C);
        gnsx(&D_800E6354);
        sndisp("あーあ、間が悪かったかな。", 0, 0xF);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x6B:
        xa_wait();
        break;
    case 0x6C:
        D_800E738D = 0xFF;
        break;
    case 0x80:
        k_sub_reset();
        gnsx(girl_short_name_table[D_800E71DF]);
        sndisp(D_80142E70[D_801405E4], 0, (s32) D_800E71DF);
        func_80052060((s32) D_80142EA0, (s32) D_80142F00, (s32) D_801405E4, (s32) D_800E71DF);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x81:
        func_8005215C();
        break;
    case 0x82:
        k_sub_reset();
        gnsx(&D_800E6354);
        sndisp(D_8014306C[D_801405E4], 0, 0xF);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x83:
        xa_wait();
        break;
    case 0x84:
        k_sub_reset();
        gnsx(girl_short_name_table[D_800E71DF]);
        sndisp(D_801431E8[D_801405E4], 0, (s32) D_800E71DF);
        func_80052060((s32) D_80143218, (s32) D_80143278, (s32) D_801405E4, (s32) D_800E71DF);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x85:
        func_8005215C();
        break;
    case 0x86:
        if (*D_8014333C[D_801405E4] == 0x20) {
            D_800E738D = 0x88;
        } else {
            k_sub_reset();
            gnsx(&D_800E6354);
            sndisp(D_8014333C[D_801405E4], 0, 0xF);
            k_sub_disp_start(0);
            D_800E738D += 1;
        }
        break;
    case 0x87:
        xa_wait();
        break;
    case 0x88:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x89:
        xa_wait();
        break;
    case 0x8A:
        k_sub_reset();
        gnsx(&D_800E6354);
        func_80044750(0x50C);
        sndisp("やっぱり、だめか…。", 0, 0xF);
        k_sub_disp_start(0);
        D_800E738D += 1;
        break;
    case 0x8B:
        xa_wait();
        break;
    case 0x8C:
        D_800E738D = 0xFF;
        break;
    case 0xFD:
        xa_wait();
        break;
    case 0xFE:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        D_800E738D = 0xFD;
        break;
    default:
        func_8004E58C();
        func_80048E78();
        set_dec_bri(0);
        func_80072B5C(0);
        break;
    }
}
void func_801380D8(void) {
    Rec38 *girl;
    u8 state;
    u32 temp_hi;
    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        D_800E6280.unk_1BC[12].unk_0C.b[3] += 1;
        gnsx(D_800B3624);
        sndisp(D_80144194, 0, 0xC);
        func_80052060((s32) &D_801441A8, (s32) &D_801441D0, 0, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp("もしもし、 Pですけど。", 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        girl = &D_800E6280.unk_1BC[12];
        if ((u8) girl->unk_0C.b[3] >= 0x29U) {
            temp_hi = (u8) girl->unk_0C.b[3] % 3U;
            switch (temp_hi) {
            case 0:
                func_80042940(0x43);
                break;
            case 1:
                func_80042940(0x44);
                break;
            case 2:
                func_80042940(0x45);
                break;
            }
        } else if ((u8) girl->unk_0C.b[3] >= 0x1FU) {
            func_80042940(0x42);
        } else if ((u8) girl->unk_0C.b[3] >= 0x15U) {
            func_80042940(0x41);
        } else {
            D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        }
        break;
    case 5:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144198, 0, 0xC);
        func_80052060((s32) &D_801441A8, (s32) &D_801441D0, 1, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 6:
        func_8005215C();
        break;
    case 7:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_8014419C, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 8:
        xa_wait();
        break;
    case 9:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_801441A0, 0, 0xC);
        func_80052060((s32) &D_801441A8, (s32) &D_801441D0, 3, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 10:
        func_8005215C();
        break;
    case 11:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 12:
        xa_wait();
        break;
    case 13:
        k_sub_reset();
        func_80044750(0x50C);
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_801441A4, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D = (state = D_800E6280.unk_110D) + 1;
        break;
    case 14:
        xa_wait();
        break;
    case 15:
        func_8004E58C();
        func_80048E78();
        set_dec_bri(0);
        func_80072B5C(0);
        break;
    default:
        func_80042940(0x10);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}
void func_801384F0(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_801441F8, 0, 0xC);
        func_80052060((s32) &D_80144210, (s32) &D_80144240, 0, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_801441FC, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144200, 0, 0xC);
        func_80052060((s32) &D_80144210, (s32) &D_80144240, 2, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 5:
        func_8005215C();
        break;
    case 6:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi(D_80144204, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 7:
        xa_wait();
        break;
    case 8:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144208, 0, 0xC);
        func_80052060((s32) &D_80144210, (s32) &D_80144240, 4, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 9:
        func_8005215C();
        break;
    case 10:
        k_sub_reset();
        func_80044750(0x50B);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
    case 11:
        xa_wait();
        break;
    case 12:
        k_sub_reset();
        func_80044750(0x50C);
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_8014420C, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 13:
        xa_wait();
        break;
    case 14:
        func_8004E58C();
        func_80048E78();
        set_dec_bri(0);
        func_80072B5C(0);
        break;
    default:
        func_80042940(0x10);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}
void func_8013883C(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144270, 0, 0xC);
        func_80052060((s32) &D_80144298, (s32) &D_801442E8, 0, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_80144274, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144278, 0, 0xC);
        func_80052060((s32) &D_80144298, (s32) &D_801442E8, 2, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 5:
        func_8005215C();
        break;
    case 6:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi(D_8014427C, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 7:
        xa_wait();
        break;
    case 8:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144280, 0, 0xC);
        func_80052060((s32) &D_80144298, (s32) &D_801442E8, 4, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 9:
        func_8005215C();
        break;
    case 10:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_80144284, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
    case 11:
        xa_wait();
        break;
    case 12:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144288, 0, 0xC);
        func_80052060((s32) &D_80144298, (s32) &D_801442E8, 6, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 13:
        func_8005215C();
        break;
    case 14:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_8014428C, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 15:
        xa_wait();
        break;
    case 16:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144290, 0, 0xC);
        func_80052060((s32) &D_80144298, (s32) &D_801442E8, 8, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 17:
        func_8005215C();
        break;
    case 18:
        k_sub_reset();
        func_80044750(0x50C);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 19:
        xa_wait();
        break;
    case 20:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_80144294, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 21:
        xa_wait();
        break;
    case 22:
        func_8004E58C();
        func_80048E78();
        set_dec_bri(0);
        func_80072B5C(0);
        break;
    default:
        func_80042940(0x10);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}
void func_80138D20(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144338, 0, 0xC);
        func_80052060((s32) &D_80144350, (s32) &D_80144380, 0, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_8014433C, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144340, 0, 0xC);
        func_80052060((s32) &D_80144350, (s32) &D_80144380, 2, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 5:
        func_8005215C();
        break;
    case 6:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi(D_80144344, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 7:
        xa_wait();
        break;
    case 8:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144348, 0, 0xC);
        func_80052060((s32) &D_80144350, (s32) &D_80144380, 4, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 9:
        func_8005215C();
        break;
    case 10:
        k_sub_reset();
        func_80044750(0x50C);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
    case 11:
        xa_wait();
        break;
    case 12:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_8014434C, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 13:
        xa_wait();
        break;
    case 14:
        func_8004E58C();
        func_80048E78();
        set_dec_bri(0);
        func_80072B5C(0);
        break;
    default:
        func_80042940(0x10);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}
void func_80139064(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_801443B0, 0, 0xC);
        func_80052060((s32) &D_801443C8, (s32) &D_801443F8, 0, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_801443B4, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_801443B8, 0, 0xC);
        func_80052060((s32) &D_801443C8, (s32) &D_801443F8, 2, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 5:
        func_8005215C();
        break;
    case 6:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi(D_801443BC, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 7:
        xa_wait();
        break;
    case 8:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_801443C0, 0, 0xC);
        func_80052060((s32) &D_801443C8, (s32) &D_801443F8, 4, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 9:
        func_8005215C();
        break;
    case 10:
        k_sub_reset();
        func_80044750(0x50C);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
    case 11:
        xa_wait();
        break;
    case 12:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_801443C4, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 13:
        xa_wait();
        break;
    case 14:
        func_8004E58C();
        func_80048E78();
        set_dec_bri(0);
        func_80072B5C(0);
        break;
    default:
        func_80042940(0x10);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}
void func_801393A8(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144428, 0, 0xC);
        func_80052060((s32) &D_80144440, (s32) &D_80144470, 0, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_8005215C();
        break;
    case 2:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_8014442C, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        xa_wait();
        break;
    case 4:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144430, 0, 0xC);
        func_80052060((s32) &D_80144440, (s32) &D_80144470, 2, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 5:
        func_8005215C();
        break;
    case 6:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndi(D_80144434, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 7:
        xa_wait();
        break;
    case 8:
        k_sub_reset();
        gnsx(D_800B3624);
        sndisp(D_80144438, 0, 0xC);
        func_80052060((s32) &D_80144440, (s32) &D_80144470, 4, 0xC);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 9:
        func_8005215C();
        break;
    case 10:
        k_sub_reset();
        func_80044750(0x50C);
        sndi("ガチャ", 0, 0x2F);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
    case 11:
        xa_wait();
        break;
    case 12:
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        sndisp(D_8014443C, 0, 0xF);
        k_sub_disp_start(0);
        D_800E6280.unk_110D += 1;
        break;
    case 13:
        xa_wait();
        break;
    case 14:
        func_8004E58C();
        func_80048E78();
        set_dec_bri(0);
        func_80072B5C(0);
        break;
    default:
        func_80042940(0x10);
        break;
    }
    menu_bar_show(0);
    cal_base_show();
}
void func_801396EC(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_80044750(0x50C);
        func_80050DFC(D_800E6280.unk_0D4);
        func_80051010("あ〜あ、留守みたいだな。", 0, 0x1F);
        func_80051010("今日はあきらめるか……。）", 1, 0x1F);
        func_8004EAD4(0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_80051DBC();
        break;
    case 2:
        func_8004E58C();
        func_80048E78();
        main_state_set(0x46);
        break;
    }
    func_80050B54(0);
    func_80069128();
}
