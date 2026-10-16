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
s32 func_80142028(void);
void func_801460D8(void);
void func_801462B4(void);
void func_80146624(void);
void func_80146FE0(void);
void func_80147590(void);
void func_80147BEC(void);
void func_8014826C(void);
void func_801488DC(void);
void func_80148C20(void);
void func_80148F08(void);
void func_80149340(void);
void func_80149840(void);
void func_80149E3C(void);
void func_8014A600(void);
void func_8014A848(void);
void func_8014A98C(void);
void func_8014AB18(void);
void func_8014AD44(void);
void func_8014AE2C(void);
extern s32 D_8014D0A8;
extern u8 D_800E8BEE;
void func_8013B77C(void);
void func_80142880(void);
void func_801455E4(s32 arg0);
void func_80145AC4(void);
void func_80145760(void);
void func_80145848(void);
void func_8014068C(void);
void func_80140BA4(void);
extern u8 D_8014BEB0[];
void func_80138F60(void);
void func_80139CB4(void);
extern s32 D_8014D0D4;
void func_8013B51C(s32 arg0);
void func_80132198(void);
void func_80132320(void);
void func_80144A34(void);
void func_80142A80(void);
extern u8 D_8014B824[];
extern u8 D_8014B838[];
void func_8013BB24(void);
void func_8013BE18(void);
void func_8013BF74(void);
extern u8 D_8014B7B4[];
extern u8 D_8014B7C8[];
void func_80144FDC(void);
void func_80145378(void);
void func_80144F98(void);
extern u8 D_8014CC74[];
extern u8 D_8014CC7C[];
extern u8 D_8014CC84[];
extern u8 D_8014BA60[];
extern u8 D_8014BA78[];
extern u8 D_8014BA88[];
extern u8 D_8014BA28[];
extern u8 D_8014BA44[];
extern u8 D_8014B998[];
extern u8 D_8014B9A4[];
void func_8013BB10(void);
void func_8013B1D8(void);
extern u8 D_8014BBD4[];
extern u8 D_8014BBEC[];
extern u8 D_8014BAA4[];
extern u8 D_8014BAC4[];
extern u8 D_8014BAE0[];
extern u8 D_8014BB8C[];
extern u8 D_8014BBA8[];
extern u8 D_8014BBC0[];
extern u8 D_8014BCB4[];
extern u8 D_8014BCCC[];
extern u8 D_8014BCE0[];
extern u8 D_8014BDE8[];
extern u8 D_8014BE00[];
extern u8 D_8014BE10[];
extern u8 D_80120721;
extern s32 D_8014CCB8;
extern s32 D_8014CCBC;
extern s32 D_8014CCC0;
extern s32 D_8014CCC4;
extern s32 D_8014CCCC;
extern s32 D_8014CCD0[];
extern u8 D_8014CE6C[];
extern u8 D_8014CEE4[];
extern u8 D_8014CF5C[];
void func_80132134(void);
void func_80132E80(void);
void func_80138488(void);
void func_80140D6C(void);
void func_80140E18(void);
void func_801454CC(void);
extern s32 D_8014D184;
void prologue_line_start(void);
void prologue_line_run(void);
extern s8 D_8014D180;
void func_80142E24();
void func_80143580();
s32 func_80140CCC(void);
extern s32 D_8014D170;
extern s32 D_8014D0E0;
extern s32 D_8014D0E4;
extern s32 D_8014D0E8;
extern s32 D_8014D0EC;
extern s32 D_8014D0F0;
void func_8013B59C(void);
extern u8 D_8014D070[];
extern u8 D_8014D078[];
extern u8 D_8014D080[];
extern s8 D_8014D08C;
extern s8 D_8014D090;
extern s8 D_8014D094;
extern s8 D_8014D098;
extern s8 D_8014D09C;
extern s8 D_8014D0A0;
extern s8 D_8014D0A4;
extern s32 D_8014CD28;
extern s32 D_8014CD2C;
extern s32 D_8014CD30;
extern s32 D_8014CD34;
extern s32 D_8014CD38;
extern s32 D_8014CD3C;
extern s32 D_8014CD40;
extern s32 D_8014CD48;
extern u8 D_8014D028[];
extern s32 D_8014CD44;
extern s32 D_8014CD4C;
extern s32 D_8014CD50;
extern s32 D_8014CD54;
extern s32 D_8014CD58;
extern s32 D_8014CD5C;
extern s32 D_8014CD60;
extern s32 D_8014CD64;
extern s32 D_8014CD68;
extern s32 D_8014CD6C;
extern s32 D_8014CD70;
extern s32 D_8014CD74;
extern s32 D_8014CD78;
extern s32 D_8014CD7C;
extern s32 D_8014CD80;
extern s32 D_8014CD84;
extern s32 D_8014CD88;
extern s32 D_8014CD8C;
extern s32 D_8014CD90;
extern s32 D_8014CD94;
extern s32 D_8014CD98;
extern s32 D_8014CD9C;
extern s32 D_8014CDA0;
extern s32 D_8014CDA4;
extern s32 D_8014CDA8;
extern s32 D_8014CDAC;
void func_80137AA0(void);
extern u8 D_8014CC70;
void func_801377E8();
extern s32 D_8014CC58;
extern s32 D_8014CC5C;
extern s32 D_8014CC50;
extern s32 D_8014CC54;
void func_8013635C();
void func_801372A0();
extern s32 D_8014CCC8;
void func_80134F6C();
void func_80137B84(s32 arg0, s32 arg1, s32 arg2);
void func_80137CA0();
typedef struct NameRecord44 {
               u8 pad00;
               u8 unk_01;
               u8 unk_02;
               u8 unk_03;
               u8 pad04;
               u8 unk_05;
               u8 pad06;
               u8 unk_07;
               u8 pad08[4];
               s32 unk_0C;
               s32 unk_10;
               u8 pad14[2];
               s16 unk_16;
               s16 unk_18;
               u8 pad1A[0xC];
               s16 unk_26;
               u8 pad28[2];
               s16 unk_2A;
               u8 pad2C[8];
               s32 unk_34;
               s32 unk_38;
               u8 pad3C[8];
} NameRecord44;
extern s32 D_8014CCAC;
extern s32 D_8014CCB4;
extern u8 D_8014CD04[];
extern s32 D_8014CC40;
extern s32 D_8014CC44;
extern s32 D_8014CC48;
extern s32 D_8014CC4C;
extern u8 *D_8014D1A0[4];
typedef struct NameProfileFlags {
               u32 month : 4;
               u32 day : 5;
               u32 blood_type : 2;
               u32 unk_0B : 21;
} NameProfileFlags;
void func_80145EA4(void);
void func_801478E0(void);
typedef union NameMotionWord {
               s32 word;
    struct {
                   s16 low;
                   s16 high;
    } half;
} NameMotionWord;
typedef struct NameMotionActor44 {
               u8 pad[3];
               u8 flags;
               u8 pad04[3];
               u8 phase;
               u8 pad08[0x1C];
               NameMotionWord x;
               NameMotionWord y;
               u8 pad2C[0x18];
} NameMotionActor44;
void func_801473AC(void);
typedef struct NameAssetActor44 {
               u8 pad00[0x34];
               s32 *image;
               u8 pad38[0x0C];
} NameAssetActor44;
s32 func_80147A08(void);
typedef struct NameCommandPair8 {
               s32 unk_00;
               s32 unk_04;
} NameCommandPair8;
extern NameCommandPair8 D_8014DA30[43];
extern NameCommandPair8 D_8014DB88[];
extern u8 D_8014DA04[44];
extern s32 D_8014D188;
void func_80146940(void);
void func_80146CB8(void);
extern s32 D_8014D1B0;
extern s32 D_8014D1B4[4];
typedef struct NameActorHead44 {
               u8 pad_00[8];
               s16 unk_08;
               u8 pad_0A[0x3A];
} NameActorHead44;
extern u8 *D_8014D958[43];
s32 func_8014977C(void);
typedef struct Entry8 {
               s16 unk_00;
               s16 unk_02;
               u8 unk_04;
               u8 unk_05;
               u8 unk_06;
               s8 unk_07;
} Entry8;
typedef struct SyncObj {
               u8 unk_00[0x10];
               Entry8 tbl[2];
               s32 idx;
               s16 unk_24;
               s16 unk_26;
               u8 unk_28[4];
               s32 flag;
} SyncObj;
typedef struct FileReq {
               u8 name[32];
               s32 retry;
} FileReq;
typedef struct MovieVlcState {
               s32 buffers[2];
               s32 index;
} MovieVlcState;
typedef struct DisplayActor44 {
               u8 unk_00;
               u8 unk_01;
               u8 unk_02;
               u8 unk_03;
               u8 unk_04;
               u8 unk_05;
               u8 unk_06;
               u8 unk_07;
               s16 unk_08;
               u8 pad_0A[2];
               u8 *unk_0C;
               u8 *unk_10;
               s16 unk_14;
               s16 unk_16;
               s16 unk_18;
               s16 unk_1A;
               u8 pad_1C[2];
               s16 unk_1E;
               u8 pad_20[2];
               s16 unk_22;
               u8 pad_24[2];
               s16 unk_26;
               u8 pad_28[2];
               s16 unk_2A;
               u8 pad_2C[2];
               s16 unk_2E;
               u8 pad_30[3];
               u8 unk_33;
               u8 *unk_34;
               s32 unk_38;
               u8 pad_3C[7];
               u8 unk_43;
} DisplayActor44;
typedef struct DisplayImagePart0E {
               s16 unk_00;
               s16 unk_02;
               s16 unk_04;
               s16 unk_06;
               s16 unk_08;
               s16 unk_0A;
               s16 unk_0C;
} DisplayImagePart0E;
typedef struct DisplayImage78 {
               s16 count;
               s16 origin_x;
               s16 origin_y;
               s16 unk_06;
               DisplayImagePart0E part[8];
} DisplayImage78;
typedef struct DisplayColorPair4 {
               u32 r0 : 5;
    u32 g0 : 5;
    u32 b0 : 5;
    u32 pad_0 : 1;
               u32 r1 : 5;
    u32 g1 : 5;
    u32 b1 : 5;
    u32 pad_1 : 1;
} DisplayColorPair4;
typedef struct SpriteSlot24 {
               u32 unk_00;
               s16 unk_04;
               s16 unk_06;
               s16 unk_08;
               s16 unk_0A;
               s16 unk_0C;
               s16 unk_0E;
               u8 unk_10;
               u8 unk_11;
               u8 unk_12;
               u8 pad_13;
               u32 unk_14;
               s16 unk_18;
               s16 unk_1A;
               s16 unk_1C;
               s16 unk_1E;
               u32 unk_20;
} SpriteSlot24;
typedef struct StrDecEnv30 {
               u8 *buffer0;
               u8 *buffer1;
               s32 unk_08;
               u8 *buffer2;
               RECT rect0;
               RECT rect1;
               s32 index;
               RECT rect2;
               s32 flag;
} StrDecEnv30;
typedef struct GsByteNibbles {
                         u8 low : 4;
                         u8 high : 4;
} GsByteNibbles;
typedef struct ScheduleFlags {
               u32 unk_00 : 18;
               u32 b18 : 1;
               u32 unk_19 : 2;
               u32 unk_21 : 3;
               u32 unk_24 : 8;
} ScheduleFlags;
typedef struct ScheduleClubFlags {
               u32 unk_00 : 25;
               u32 unk_25 : 2;
               u32 unk_27 : 5;
} ScheduleClubFlags;
typedef struct ScheduleEntryDate {
               u16 unk_00 : 12;
               u16 month : 4;
               u16 day : 5;
               u16 unk_15 : 11;
} ScheduleEntryDate;
typedef struct ScheduleEntryFlags {
               u32 unk_00 : 25;
               u32 unk_25 : 4;
               u32 active : 1;
               u32 unk_30 : 2;
} ScheduleEntryFlags;
typedef struct NameMetric4 {
               u8 unk_00[2];
               s16 unk_02;
} NameMetric4;
typedef struct NameMetricWork {
                s32 unk_000;
                u8 slot_status[16];
                u8 flags[16];
                u8 unk_024[2];
                u8 selected_slot;
                u8 unk_027[0x831];
                u8 year[16];
                u8 month[16];
                u8 day[16];
                u8 family_name[16][8];
                u8 given_name[16][8];
                u8 nickname[16][16];
                NameMetric4 metrics[9][16];
                u8 unk_CC8[0x200];
                u8 club[16];
} NameMetricWork;
typedef struct NameRecordF00 {
                u8 unk_000[0x205];
                u8 year;
                u8 month;
                u8 day;
                u8 unk_208[0x94];
                u8 family_name[8];
                u8 given_name[8];
                u8 nickname[16];
                u16 club;
                u8 unk_2BE[6];
                NameMetric4 metrics[9];
                u8 unk_2E8[0x3A0];
                u8 flags[14];
                u8 unk_696[0x86A];
} NameRecordF00;
typedef struct ScheduleGirlFlags {
               u32 unk_00 : 16;
               u32 b16 : 1;
               u32 b17 : 1;
               u32 b18 : 1;
               u32 unk_19 : 1;
               u32 unk_20 : 4;
               u32 unk_24 : 8;
} ScheduleGirlFlags;
typedef struct DisplayRecord44 {
               u8 unk_00;
               u8 unk_01;
               u8 unk_02;
               u8 unk_03;
               u8 unk_04;
               u8 unk_05;
               u8 unk_06;
               u8 unk_07;
               u8 unk_08[4];
               u8 *unk_0C;
               u8 *unk_10;
               s16 unk_14;
               s16 unk_16;
               s16 unk_18;
               u8 unk_1A[0xC];
               s16 unk_26;
               u8 unk_28[2];
               s16 unk_2A;
               u8 unk_2C[8];
               u8 *unk_34;
               s32 unk_38;
               u8 unk_3C[8];
} DisplayRecord44;
typedef struct CallPair8 {
               s32 unk_00;
               s32 unk_04;
} CallPair8;
void func_8013ACA4(void);
void func_80140044(void);
s32 func_801425C0(void);
extern s32 D_8014D0D0;
extern s32 D_8014D0D8;
extern s32 D_8014D0DC;
extern u8 D_8014CC90;
extern u8 D_8014CC94;
extern u8 D_8014CC98;
extern u8 D_8014CC9C;
extern u8 D_8014CCA0;
extern u8 D_8014CCA4;
void func_80136808(s32 arg0);
typedef struct NameKanaPages44 {
               u8 *text[17];
} NameKanaPages44;
extern u8 *D_8014CDB0[45];
extern u16 *D_8014CE64;
extern u8 **D_8014CE68;
void func_80134248(s32 arg0);
void func_80138F00(void);
extern s32 D_8014CC60;
extern s32 D_8014CC64;
void func_8013BB1C(void);
void func_8013B994(void);
extern s32 D_8014D0AC;
extern u8 D_8014D0B0;
extern u8 D_8014D0B4;
extern u8 D_8014D0B8;
extern u8 D_8014D0BC;
extern u8 D_8014D0C0;
extern u8 D_8014D0C4;
extern u8 D_8014D0C8;
extern u8 D_8014D0CC;
extern u8 D_8014CCE4[];
void func_80142560(void);
extern u8 D_8014CD10[];
extern s32 D_8014CC38;
extern s32 D_8014CC3C;
extern u8 D_8014CC7A;
extern u8 D_8014CC82;
extern u8 D_8014CC8E;
extern u8 D_8014CC7B;
extern u8 D_8014CC83;
extern u8 D_8014CC8F;
void func_8013A9C8(void);
void func_80142A3C(void);
s32 func_801429BC(void);
void func_80049A40();
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740",func_80139740);
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740",func_801398EC);
void func_80139AD8(s32 arg0, s32 arg1, s32 arg2) {
    u8 *p;
    p = (u8 *)D_801217D0 + arg0 * 36;
    p[0x14] = 0x80;
    p[0x15] = 0x80;
    p[0x16] = 0x80;
    *(s16 *)(p + 4) = arg1;
    *(s16 *)(p + 6) = arg2;
    *(s32 *)p = 0;
    *(s16 *)(p + 8) = 0x10;
    *(s16 *)(p + 0xA) = 0x10;
    p[0xE] = arg0 / 8 * 0x30 + D_800E6280.unk_10F8 / 32 % 3 * 0x10 + 0x80;
    p[0xF] = (arg0 & 7) * 0x10;
    *(s16 *)(p + 0xC) = 0x1F;
    *(s16 *)(p + 0x10) = arg0 * 0x10;
    *(s16 *)(p + 0x18) = 8;
    *(s16 *)(p + 0x1A) = 8;
    *(s16 *)(p + 0x12) = 0x1F8;
    D_800E6280.unk_10A5[arg0] = 2;
}
void func_80139BA0(void) {
    if (D_800E6280.unk_F88 & 0x10) {
        D_8014D0D4 += 1;
        return;
    }
    if ((D_800E6280.unk_F88 & 0x20) && (D_8011ECF6 < 0x20) && (D_8011ECF6 >= -0x3F) && (D_8011ECFA < 0x1C) && (D_8011ECFA >= 0xD)) {
        D_8014D0D4 += 1;
    }
}
void func_80139C24(void) {
    if (D_8014D0D4 < 0) {
        D_8014D0D4 += 3;
    }
    switch (D_8014D0D4 % 3) {
    case 0:
        func_80139F30();
        break;
    case 1:
        func_8013ACA4();
        break;
    case 2:
        func_8013A9C8();
        break;
    }
    func_80139BA0();
}
void func_80139CB4(void) {
    u8 sp30[0x108];
    if (D_800E8BEE != 0) {
        func_800AE090(sp30, D_800B3288[D_800E7D36], 5);
        func_800AE100(sp30, "番");
        func_8004E788(-0x20, -0x5C, 0xF, (s32) sp30, 0);
        func_8004E788(0x20, -0x4B, 0xF, (s32) ((NameMetricWork *)&D_800E7D10)->family_name[D_800E7D36], 0);
        func_8004E788(0x4E, -0x4B, 0xF, (s32) ((NameMetricWork *)&D_800E7D10)->given_name[D_800E7D36], 0);
        func_8004E788(0x20, -0x3B, 0xF, (s32) ((NameMetricWork *)&D_800E7D10)->nickname[D_800E7D36], 0);
        func_8004E788(0x20, -0x2B, 0xF, (s32) club_name_table[((NameMetricWork *)&D_800E7D10)->club[D_800E7D36]], 0);
        return;
    }
    func_800AE090(sp30, D_800B3288[D_800E6280.unk_1118], 5);
    func_800AE100(sp30, "番");
    func_8004E788(-0x20, -0x5C, 0xF, (s32) sp30, 0);
    func_8004E788(0x20, -0x4B, 0xF, (s32) ((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].family_name, 0);
    func_8004E788(0x4E, -0x4B, 0xF, (s32) ((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].given_name, 0);
    func_8004E788(0x20, -0x3B, 0xF, (s32) ((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].nickname, 0);
    func_8004E788(0x20, -0x2B, 0xF, (s32) club_name_table[((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].club >> 0xC], 0);
}
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740",func_80139F30);
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740",func_8013A9C8);
void func_8013ACA4(void) {
    if (D_800E8BEE != 0) {
        func_80139740(((NameMetricWork *)&D_800E7D10)->metrics[0][D_800E7D36].unk_02, 16, 30);
        func_80139740(((NameMetricWork *)&D_800E7D10)->metrics[1][D_800E7D36].unk_02, 16, 46);
        func_80139740(((NameMetricWork *)&D_800E7D10)->metrics[2][D_800E7D36].unk_02, 16, 62);
        func_80139740(((NameMetricWork *)&D_800E7D10)->metrics[3][D_800E7D36].unk_02, 16, 78);
        func_80139740(((NameMetricWork *)&D_800E7D10)->metrics[4][D_800E7D36].unk_02, 128, 14);
        func_80139740(((NameMetricWork *)&D_800E7D10)->metrics[5][D_800E7D36].unk_02, 128, 30);
        func_80139740(((NameMetricWork *)&D_800E7D10)->metrics[6][D_800E7D36].unk_02, 128, 46);
        func_80139740(((NameMetricWork *)&D_800E7D10)->metrics[7][D_800E7D36].unk_02, 128, 62);
        func_80139740(((NameMetricWork *)&D_800E7D10)->metrics[8][D_800E7D36].unk_02, 128, 78);
        return;
    }
    func_80139740(((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].metrics[0].unk_02, 16, 30);
    func_80139740(((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].metrics[1].unk_02, 16, 46);
    func_80139740(((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].metrics[2].unk_02, 16, 62);
    func_80139740(((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].metrics[3].unk_02, 16, 78);
    func_80139740(((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].metrics[4].unk_02, 128, 14);
    func_80139740(((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].metrics[5].unk_02, 128, 30);
    func_80139740(((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].metrics[6].unk_02, 128, 46);
    func_80139740(((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].metrics[7].unk_02, 128, 62);
    func_80139740(((NameRecordF00 *)D_80123120)[D_800E6280.unk_1117].metrics[8].unk_02, 128, 78);
}
extern s32 sp;
u8 func_8013AF78(s32 arg0) {
    s32 flags[3];
    s32 *var_v1;
    u8 temp_t6;
    u8 temp_t7;
    u8 temp_t8;
    u8 temp_t9;
    u8 var_v0;
    u8 var_v1_2;
    var_v1 = &flags[0];
    do {
        var_v1 += 4;
        *(s32 *)((unsigned char *)var_v1 + (-4)) = 0;
    } while ((u32) var_v1 < (u32) &flags[3]);
    temp_t8 = D_80120BE7 & 0x7F;
    temp_t6 = D_80120B5F & 0x7F;
    temp_t7 = D_80120BA3 & 0x7F;
    temp_t9 = D_80120C2B & 0x7F;
    var_v0 = temp_t9;
    var_v1_2 = temp_t7;
    if (arg0 == 0) {
        D_80120BE7 = temp_t8;
        D_80120B5F = temp_t6;
        D_80120BA3 = temp_t7;
        D_80120C2B = temp_t9;
        return 0U;
    }
    *(s32 *)((unsigned char *)(sp + (((s32) D_8014D0D4 % 3) * 4)) + (28)) = 1;
    D_80120BE7 = temp_t8;
    if (flags[0] != 0) {
        var_v1_2 = (var_v1_2 | 0x80) & 0xFF;
        D_80120BE7 = temp_t8 | 0x80;
    }
    D_80120B5F = temp_t6;
    if (flags[1] != 0) {
        D_80120B5F = temp_t6 | 0x80;
    }
    D_80120C2B = var_v0;
    D_80120BA3 = var_v1_2;
    if (flags[2] != 0) {
        var_v0 = (var_v0 | 0x80) & 0xFF;
        D_80120C2B = var_v0;
        D_80120BA3 = var_v1_2 | 0x80;
    }
    return var_v0;
}
s32 func_8013B0A0(void) {
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    a = 0;
    b = 0;
    if (D_800E6280.unk_1109 == 0) {
        return 0;
    }
    c = 0;
    d = 0;
    if (D_800E6280.unk_110A < 0xA0U) {
        a = 1;
    } else if (D_800E6280.unk_110A < 0xC0U) {
        b = 1;
    } else if (D_800E6280.unk_110A < 0xE0U) {
        c = 1;
    } else {
        d = 1;
    }
    if (a != 0) {
        D_80120650[0xCF] |= 0x80;
    } else {
        D_80120650[0xCF] &= ~0x80;
    }
    if (b != 0) {
        D_80120650[0x47] |= 0x80;
    } else {
        D_80120650[0x47] &= ~0x80;
    }
    if (c != 0) {
        D_80120650[3] |= 0x80;
    } else {
        D_80120650[3] &= ~0x80;
    }
    if (d != 0) {
        D_80120650[0x8B] |= 0x80;
    } else {
        D_80120650[0x8B] &= ~0x80;
    }
}
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740",func_8013B1D8);
void func_8013B51C(s32 arg0) {
    s16 x;
    s16 y;
    x = (arg0 / 7) * 0x1F - 0x82;
    *(s16 *)&D_8011ECD0[0x26] = x;
    y = (arg0 % 7) * 0x10 - 0xB;
    *(s16 *)&D_8011ECD0[0x2A] = y;
    D_8011ECD0[0x1F9F] |= 0x80;
    *(s16 *)&D_8011ECD0[0x1FC2] = x;
    *(s16 *)&D_8011ECD0[0x1FC6] = y;
}
void func_8013B59C(void) {
    func_8004E58C();
    func_80139CB4();
    k_sub_reset_point_set();
    k_sub_disp_start(0);
    D_8014D0D4 = 1;
    func_8013B51C(D_800E6280.unk_1118 - 1);
}
void func_8013B5F0(void) {
    if ((D_800E6280.unk_1109 != 0 || D_800E6280.unk_110A != 0) && D_8014D0E0 < 0x4001) {
        D_8014D0E0 = (D_8014D0E0 * 16 + 0x70) / 15;
        D_8014D0E4 += D_8014D0EC;
        D_8014D0E8 += D_8014D0F0;
        if (D_8014D0E4 >= 0xA1) {
            D_8014D0EC = -(func_800AE0D0() & 0xF);
        }
        if (D_8014D0E8 >= 0x79) {
            D_8014D0F0 = -(func_800AE0D0() & 0xF);
        }
        if (D_8014D0E4 < -0xA0) {
            D_8014D0EC = func_800AE0D0() & 0xF;
        }
        if (D_8014D0E8 < -0x78) {
            D_8014D0F0 = func_800AE0D0() & 0xF;
        }
    }
    func_80046AC8(0x1A, 0, 0x1F7, D_8014D0E0, D_8014D0E4, D_8014D0E8, 0x01DF7FCF, 2);
}
void func_8013B77C(void) {
    if (D_8014D0AC != 0) {
        func_800AE0B0("option  kidou back ");
        if (D_8014D0AC & 0x80) {
            k_speed_set(D_8014D0B0);
        }
        func_800AE0B0("kanji_speed%d \n", D_8014D0B0);
        if (D_8014D0B4 >= 3) {
            D_800E6280.unk_F6C = D_8014D0B4 % 3;
        }
        func_800AE0B0("icon_mode %d \n", D_8014D0B4);
        if (D_8014D0B8 >= 4) {
            D_800E6280.unk_1092 = D_8014D0B8 & 3;
        }
        func_800AE0B0("menu_mode %d \n", D_8014D0B8);
        if (D_8014D0AC & 8) {
            D_800E6280.unk_F6F = D_8014D0BC;
        }
        func_800AE0B0("cursor_speed %d \n", D_8014D0BC);
        if (D_8014D0C0 >= 3) {
            D_800E6280.unk_03C = D_8014D0C0 % 3;
        }
        func_800AE0B0("sound_mode %d \n", D_8014D0C0);
        if (D_8014D0AC & 0x40) {
            D_800E6280.unk_F70 = D_8014D0C4;
        }
        func_800AE0B0("click_mode %d \n", D_8014D0C4);
        if (D_8014D0AC & 0x20) {
            D_800E6280.unk_F73 = D_8014D0C8;
        }
        func_800AE0B0("click_speed %d \n", D_8014D0C8);
        if (D_8014D0AC & 0x10) {
            D_800E6280.unk_F75 = D_8014D0CC;
        }
        func_800AE0B0("mouse_ok %d \n", D_8014D0CC);
        if (*(u8 *)&D_800E6280.unk_F6C == 2) {
            D_800E6280.unk_F6D = 0xF;
        } else {
            D_800E6280.unk_F6D = 0xE;
        }
        func_8006509C();
        func_80064FA4(0);
    }
    D_8014D0A8 = 0;
    func_8013BB1C();
    func_8013B994();
}
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740",func_8013B994);
void func_8013BB10(void) {
    D_8014D0A8 = 0;
}
void func_8013BB1C(void) {
}
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740",func_8013BB24);
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740",func_8013BE18);
void func_8013BF74(void) {
    s32 i;
    s32 column, row;
    s16 x[16], y[16], width[16], height[16];
    s16 x2[4], y2[4], width2[4], height2[4];
    s32 addresses[6], addresses2[6];
    DisplayRecord44 *record;
    for (i = 0; i < 6; i++) {
        addresses[i] = 0x80182000 + i * 0x4000;
        addresses2[i] = 0x801A4000 + i * 0x4000;
    }
    for (i = 0; i < 14; i++) {
        column = i / 7;
        row = i % 7;
        x[i] = column * 32 - 144;
        y[i] = row * 16 - 18;
        width[i] = 32;
        height[i] = 16;
    }
    func_8004F870(0, 14, x, y, width, height);
    for (i = 0; i < 4; i++) {
        column = i & 1;
        row = i & 2;
        x2[i] = column * 44 - 144;
        y2[i] = row * 8 - 96;
        width2[i] = 40;
        height2[i] = 20;
    }
    func_8004F870(1, 4, x2, y2, width2, height2);
    D_800E6280.unk_1093 = 0;
    D_800E6280.unk_1094 = -1;
    D_8014D0D0 = 0;
    D_8014D0D4 = 1;
    D_8014D0D8 = 0;
    D_8014D0DC = 0;
    record = (DisplayRecord44 *)D_80120650;
    for (i = 0; i < 45; i++) {
        record[i].unk_18 = i;
        record[i].unk_01 = 5;
        record[i].unk_02 = 64;
        record[i].unk_38 = 0x01000000;
        record[i].unk_03 = 36;
        record[i].unk_0C = (u8 *)0x801C048C;
        record[i].unk_10 = (u8 *)0x801C0540;
        record[i].unk_34 = (u8 *)0x801C047C;
        record[i].unk_04 = 0;
        record[i].unk_3C[7] = 0;
        record[i].unk_05 = 128;
        record[i].unk_26 = -160;
        record[i].unk_2A = -120;
        record[i].unk_07 = 128;
    }
    for (i = 0; i < 11; i++) {
        record[i + 45].unk_01 = 4;
        record[i + 45].unk_02 = 64;
        record[i + 45].unk_38 = 0x40000000;
        record[i + 45].unk_03 = 36;
        record[i + 45].unk_0C = sprite_frame_table;
        record[i + 45].unk_10 = D_800C9A14;
        record[i + 45].unk_34 = D_800C9730;
        record[i + 45].unk_04 = 8;
        record[i + 45].unk_18 = 89;
        record[i + 45].unk_3C[7] = 0;
        record[i + 45].unk_05 = 1;
        record[i + 45].unk_26 = i / 4 * 65 - 15;
        record[i + 45].unk_2A = i % 4 * 16 + 36;
    }
    load_tpage_buf_lock(5, ((s32 *)D_80120684)[0], 1);
    load_tpage_buf_lock(6, ((s32 *)D_80120684)[1], 1);
    load_tpage_buf_lock(7, ((s32 *)D_80120684)[2], 1);
    load_tpage_buf_lock(8, ((s32 *)D_80120684)[3], 1);
    ((DisplayRecord44 *)D_8011ECD0)[114].unk_03 = 0;
    ((DisplayRecord44 *)D_8011ECD0)[100].unk_03 |= 0x80;
    ((DisplayRecord44 *)D_8011ECD0)[101].unk_03 |= 0x80;
    ((DisplayRecord44 *)D_8011ECD0)[102].unk_03 |= 0x80;
    record = &((DisplayRecord44 *)D_8011ECD0)[103];
    for (i = 0; i < 12; i++) {
        record[i].unk_03 |= 128;
    }
    if (D_800E8BEE != 0) {
        ((DisplayRecord44 *)D_8011ECD0)[126].unk_3C[7] = 0x11;
    } else {
        ((DisplayRecord44 *)D_8011ECD0)[127].unk_3C[7] = 0x11;
    }
    ((DisplayRecord44 *)D_8011ECD0)[126].unk_03 |= 0x80;
    ((DisplayRecord44 *)D_8011ECD0)[127].unk_03 |= 0x80;
    func_80043914(0x801A2000, 0x10, 1, 1, 0);
    func_80043914(0x801A2000, 0x11, 1, 1, 1);
    func_80043914(0x80180000, 0x1F, 1, 1, 0);
    func_8004A1A0((s32) addresses, 0, 6, 3, 2, 0x1FF, -0xA0, -0x78, 0xA, 0xD);
    func_8004A1A0((s32) addresses2, 1, 6, 3, 2, 0x1F0, -0xA0, -0x78, 8, 0xA);
    func_80048F64(0x72);
    func_80042808();
}
void func_8013C4A8(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013BB24();
        return;
    case 1:
        func_8013BE18();
        return;
    case 2:
        func_8013BF74();
        return;
    }
}
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata",D_8014B7B4);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata",D_8014B7C8);
void func_8013C510(void) {
    func_80056284();
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014B7B4, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014B7C8, 0);
    k_disp_start(2);
    func_8004284C();
}
void func_8013C580(void) {
    u8 sel;
    GameState *g;
    if (func_80053F04() != 0) {
        func_8006BD6C(1);
        func_80053D10();
        g = &D_800E6280;
        sel = g->unk_1115;
        if (sel == 1) {
            func_800AE0B0("EXIST CARD!");
            func_80042808();
        } else if (sel == 0xF1) {
            func_800AE0B0("FILE_SEL UNFORMAT FIND!");
            func_80042908(0xC0);
        } else if (sel == 0xFF) {
            func_800AE0B0("NO CARD!");
            func_8004284C();
        }
    } else if ((u32)D_800E6280.unk_1120 >= 0x201U) {
        func_8006BD6C(1);
        func_80053D10();
        func_800AE0B0("NO COMUNICATION!");
        func_8004284C();
    }
}
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata",D_8014B824);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata",D_8014B838);
void func_8013C664(void) {
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014B824, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014B838, 0);
    k_disp_start(2);
    func_8004284C();
}
void func_8013C6CC(void) {
    if (D_800E6280.unk_F88 & 0x860) {
        func_8004284C();
    }
}
void func_8013C6FC(void) {
    func_8004E58C();
    func_8004E788(-0x40, 0x20, 0xF, "新たにゲームを始めますか？", 0);
    func_8004E788(-0x40, 0x30, 0xF, D_800B3C70, 0);
    func_8004E788(0x14, 0x30, 1, func_8006CA9C(), 0);
    func_8004E788(-0x40, 0x40, 0xF, D_800B3C88, 0);
    func_8004E788(0x14, 0x40, 2, func_8006CAE0(), 0);
    func_8004E884(5);
    func_8004284C();
}
void func_8013C7C0(void) {
    if (D_800E6280.unk_F88 & 0x800) {
        func_8013B77C();
        main_state_set(0x14);
    } else if (D_800E6280.unk_F88 & 0x20) {
        func_8013B77C();
        main_state_set(0x14);
    } else if (D_800E6280.unk_F88 & 0x40) {
        func_8004284C();
    }
}
void func_8013C834(void) {
    func_8004E58C();
    func_8004E788(-0x40, 0x20, 0xF, "メモリーカードを", 0);
    func_8004E788(-0x40, 0x30, 1, "差し込み口１", 0);
    func_8004E788(-0x40, 0x30, 0xF, "　　　　　　にセットして、", 0);
    func_8004E788(-0x40, 0x40, 0xF, "　ボタンを押してください。", 0);
    func_8004E788(-0x40, 0x40, 1, func_8006CA9C(), 0);
    func_80053D10();
    func_8004E884(5);
    func_8004284C();
}
void func_8013C8FC(void) {
    if (D_800E6280.unk_F88 & 0x20) {
        func_80056284();
        func_80042940(0);
    }
}
void func_8013C934(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013C510();
        break;
    case 1:
        func_8013C580();
        break;
    case 2:
        func_8013C664();
        break;
    case 3:
        func_8013C6CC();
        break;
    case 4:
        func_8013C6FC();
        break;
    case 5:
        func_8013C7C0();
        break;
    case 6:
        func_8013C834();
        break;
    case 7:
        func_8013C8FC();
        break;
    }
}
s32 func_8013C9F0(void) {
    s32 pad[3];
    s32 count;
    s32 i;
    switch (D_800E6280.unk_110D) {
    case 0:
        func_80053CE0();
        D_800E6280.unk_110D++;
        break;
    case 1:
        if (func_80054140() == 2) {
            func_800AE0B0("SEARCH_CARD");
            D_800E6280.unk_110D++;
        } else if (D_800E6280.unk_1104.u >= 0x201U) {
            func_80056284();
            func_80042908(1);
            func_80053D10();
        }
        break;
    case 2:
        count = 0;
        for (i = 0; i < 14; i++) {
            if (D_800E7D11[i + 4] != 0) {
                count++;
            }
        }
        func_8004E58C();
        if (D_800E8BEE != 0) {
            func_8004284C();
        } else if (count != 0) {
            func_80042940(0x20);
        } else {
            func_80042940(0x40);
        }
        break;
    case 3:
        break;
    }
}
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740",func_8013CB44);
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740",func_8013CEC8);
void func_8013D054(void) {
    u8 state;
    switch (D_800E6280.unk_110D) {
    case 0:
        func_80053CE0();
        D_800E6280.unk_110D++;
        break;
    case 1:
        if (func_8005448C() == 4) {
            func_80053D10();
            D_800E6280.unk_110D++;
        } else if ((state = D_800E6280.unk_1115) == 0xF0) {
            func_80056284();
            func_80042908(1);
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (state == 0xD0)) {
            func_80053D10();
            func_80042908(0xFF);
        }
        break;
    case 2:
        if (func_80055A38(0) == 1) {
            D_800E6280.unk_110D++;
        } else {
            D_800E7D14[D_800E6280.unk_1118] = 0xFF;
            func_80042908(0xF0);
        }
        break;
    case 3:
        if (func_80055AFC(0) == 1) {
            func_8013BB10();
            func_800AE0B0("SYS0 DATA OK!");
            func_80053D10();
            func_80044750(0x501);
            func_80053CE0();
            D_800E7D36 = D_800E6280.unk_1118;
            func_80054E6C();
            D_800E6280.unk_110D++;
        } else {
            state = D_800E6280.unk_1115;
            func_800AE0B0("s->mem_stat %d \n", state);
            D_800E7D14[D_800E6280.unk_1118] = 0xFF;
            func_80042908(0xF0);
        }
        break;
    case 4:
        if (func_80054388() == 0x13) {
            func_80053D10();
            func_80042908(3);
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || ((state = D_800E6280.unk_1115) == 0xF0)) {
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    }
    func_8013B1D8();
}
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740",func_8013D2AC);
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740",func_8013D4CC);
void func_8013D62C(void) {
    s32 pad;
    u8 buf[0x100];
    func_8004E58C();
    func_800AE0F0(buf, D_800B3288[D_800E6280.unk_1118]);
    strcat(buf, "番アルバムを");
    func_8004E788(-0x40, 0x20, 0xF, buf, 0);
    func_8004E788(-0x40, 0x30, 0xF, "ロードしています。", 0);
    func_8004E884(2);
    func_8004284C();
}
s32 func_8013D6C0(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_80053CE0();
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (func_8005448C() == 4) {
            D_800E6280.unk_110D += 1;
        } else if (D_800E6280.unk_1115 == 0xF0) {
            func_80056284();
            func_80042908(1);
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (D_800E6280.unk_1115 == 0xD0)) {
            func_80053D10();
            func_80042908(0xFF);
        }
        break;
    case 2:
        if (func_80055A38(0) == 1) {
            func_8013B59C();
            func_80042940(0x21);
        } else {
            D_800E7D14[D_800E6280.unk_1118] = 0xFF;
            func_80042908(0xFF);
        }
        break;
    }
}
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata",D_8014B998);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata",D_8014B9A4);
void func_8013D7F4(void) {
    if (func_80055AFC(0) == 1) {
        func_8013BB10();
        func_800AE0B0(D_8014B998);
        func_80053D10();
        func_80044750(0x501);
        func_80042908(3);
    } else {
        func_800AE0B0(D_8014B9A4);
        func_80056284();
        func_80042908(1);
    }
    func_8013B1D8();
}
void func_8013D874(void) {
    func_8004E58C();
    func_8004E788(-0x40, 0x20, 0xF, "このゲームのデータがありません", 0);
    func_8004E788(-0x40, 0x30, 0xF, "新たにアルバムを作成し、", 0);
    func_8004E788(-0x40, 0x40, 0xF, "ゲームをプレイしますか？", 0);
    func_8004E788(-0x40, 0x50, 0xF, "　　はい…　　いいえ…　", 0);
    func_8004E788(6, 0x50, 1, func_8006CA9C(), 0);
    func_8004E788(0x5A, 0x50, 2, func_8006CAE0(), 0);
    func_8004E884(6);
    func_8004284C();
}
void func_8013D954(void) {
    if (D_800E6280.unk_F88 & 0x20) {
        func_80042940(0x44);
    } else if (D_800E6280.unk_F88 & 0x40) {
        func_8004284C();
    }
}
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata",D_8014BA28);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata",D_8014BA44);
void func_8013D9A0(void) {
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BA28, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014BA44, 0);
    k_disp_start(2);
    func_8004284C();
}
void func_8013DA08(void) {
    if (D_800E6280.unk_F88 & 0x820) {
        func_8013B77C();
        main_state_set(0x14);
    }
}
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata",D_8014BA60);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata",D_8014BA78);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata",D_8014BA88);
void func_8013DA40(void) {
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BA60, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014BA78, 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_8014BA88, 0);
    k_disp_start(3);
    func_8004284C();
}
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740",func_8013DAC4);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata",D_8014BAA4);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata",D_8014BAC4);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata",D_8014BAE0);
void func_8013DBD0(void) {
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BAA4, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014BAC4, 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_8014BAE0, 0);
    k_disp_start(3);
    func_8004284C();
}
void func_8013DC54(void) {
    if (D_800E6280.unk_F88 & 0x860) {
        func_8004284C();
    }
}
void func_8013DC84(void) {
    func_8004E58C();
    func_8004E788(-0x40, 0x20, 0xF, "システムファイルを作成しますか", 0);
    func_8004E788(-0x40, 0x30, 0xF, D_800B3C70, 0);
    func_8004E788(0x14, 0x30, 1, func_8006CA9C(), 0);
    func_8004E788(-0x40, 0x40, 0xF, D_800B3C88, 0);
    func_8004E788(0x14, 0x40, 2, func_8006CAE0(), 0);
    func_8004E884(5);
    func_8004284C();
}
void func_8013DD48(void) {
    if (D_800E6280.unk_F88 & 0x820) {
        func_80042940(0x4C);
    } else if (D_800E6280.unk_F88 & 0x40) {
        func_8004284C();
    }
}
s32 func_8013DD94(void) {
    u8 sp38[0x108];
    u8 temp_v0;
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004E58C();
        func_800AE0F0(sp38, D_800B32CC[D_800E6280.unk_1118 - 1]);
        func_800AE100(sp38, "番アルバムを作成しています。");
        func_8004E788(-0x40, 0x20, 0xF, (s32) sp38, 0);
        func_8004E884(2);
        temp_v0 = D_800E7D14[D_800E6280.unk_1118];
        if (temp_v0 == 0xFF) {
            D_800E6280.unk_110D += 1;
        } else if (temp_v0 == 1) {
            D_800E6280.unk_110D += 1;
        } else if (func_80054694((s32) D_800E6280.unk_1118) == 1) {
            D_800E6280.unk_110D += 1;
        } else {
            func_80042908(4);
        }
        break;
    case 1:
        func_80053CE0();
        func_80041878();
        func_80042058();
        D_800E6280.unk_110D += 1;
        break;
    case 2:
        if (func_80054590() == 0x14) {
            D_800E6280.unk_110D += 1;
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (D_800E6280.unk_1115 == 0xF0)) {
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    case 3:
        func_80053D10();
        func_8013B77C();
        main_state_set(0x14);
        break;
    }
}
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740",func_8013DF88);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata",D_8014BB8C);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata",D_8014BBA8);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata",D_8014BBC0);
void func_8013E30C(void) {
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BB8C, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014BBA8, 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_8014BBC0, 0);
    k_disp_start(3);
    func_8004284C();
}
void func_8013E390(void) {
    if (D_800E6280.unk_F88 & 0x860) {
        func_8004284C();
    }
    func_80142880();
}
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata",D_8014BBD4);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata",D_8014BBEC);
void func_8013E3C8(void) {
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BBD4, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014BBEC, 0);
    k_disp_start(2);
    func_8004284C();
}
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740",func_8013E430);
void func_8013E59C(void) {
    s32 pad;
    u8 buf[0x100];
    func_8004E58C();
    func_800AE0F0(buf, D_800B3288[D_800E6280.unk_1118]);
    strcat(buf, "番アルバムを");
    func_8004E788(-0x40, 0x20, 0xF, buf, 0);
    func_8004E788(-0x40, 0x30, 0xF, "やりなおします。", 0);
    func_8004E788(-0x40, 0x40, 0xF, D_800B3C70, 0);
    func_8004E788(0x14, 0x40, 1, func_8006CA9C(), 0);
    func_8004E788(-0x40, 0x50, 0xF, D_800B3C88, 0);
    func_8004E788(0x14, 0x50, 2, func_8006CAE0(), 0);
    func_8004E884(6);
    func_8004284C();
}
void func_8013E6A8(void) {
    if (D_800E6280.unk_F88 & 0x40) {
        func_80044750(0x501);
        func_80042940(0);
    } else if (D_800E6280.unk_F88 & 0x20) {
        if (D_800E8BEE) {
            func_80042940(0xA6);
        } else {
            func_80042940(0xA7);
        }
    }
}
typedef struct NameDate {
    u32 day : 4;
    u32 month : 5;
    u32 week : 2;
    u32 rest : 21;
} NameDate;
void func_8013E71C(void) {
    func_800AE0A0(D_8014D070, &D_800E6280.unk_0D4, 8);
    func_800AE0A0(D_8014D078, &D_800E6280.unk_0DC, 8);
    func_800AE0A0(D_8014D080, &D_800E6280.unk_0E4, 0xC);
    D_8014D08C = ((NameDate *)&D_800E6280.unk_0F8)->day - 1;
    D_8014D090 = ((NameDate *)&D_800E6280.unk_0F8)->month - 1;
    D_8014D094 = (((NameDate *)&D_800E6280.unk_0F8)->week + 3) & 3;
    D_8014D098 = ((NameDate *)&D_800E6280.unk_1BC[0].unk_10.w)->day - 1;
    D_8014D09C = ((NameDate *)&D_800E6280.unk_1BC[0].unk_10.w)->month - 1;
    D_8014D0A0 = (((NameDate *)&D_800E6280.unk_1BC[0].unk_10.w)->week + 3) & 3;
    D_8014D0A4 = 1;
}
void func_8013E80C(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_80053CE0();
        func_800AE0B0("NEW FILE %d \n", D_800E6280.unk_1118);
        D_800E6280.unk_110D += 1;
    default:
        break;
    case 1:
        if (func_8005448C() == 4) {
            func_80053D10();
            D_800E6280.unk_110D += 1;
        } else if (D_800E6280.unk_1115 == 0xF0) {
            func_80056284();
            func_80042908(1);
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (D_800E6280.unk_1115 == 0xD0)) {
            func_80053D10();
            func_80042908(0xFF);
        }
        break;
    case 2:
        if (func_80055A38(0) == 1) {
            D_800E6280.unk_110D += 1;
        } else {
            D_800E7D14[D_800E6280.unk_1118] = 0xFF;
            func_80042908(0xFF);
        }
        break;
    case 3:
        if (func_80055AFC(0) == 1) {
            func_8013E71C();
            func_8013BB10();
            D_800E6280.unk_110D += 1;
        } else {
            D_800E7D14[D_800E6280.unk_1118] = 0xFF;
            func_80042908(0xFF);
        }
        break;
    case 4:
        func_800AE0B0("%d file replay", D_800E6280.unk_1118);
        func_80041878();
        func_80042058();
        func_8013B77C();
        main_state_set(0x14);
        break;
    }
}
void func_8013E9FC(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_80053CE0();
        func_800AE0B0("NEW FILE %d \n", D_800E6280.unk_1118);
        D_800E6280.unk_110D += 1;
    default:
        break;
    case 1:
        if (func_8005448C() == 4) {
            func_80053D10();
            D_800E6280.unk_110D += 1;
        } else if (D_800E6280.unk_1115 == 0xF0) {
            func_80056284();
            func_80042908(1);
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (D_800E6280.unk_1115 == 0xD0)) {
            func_80053D10();
            func_80042908(0xFF);
        }
        break;
    case 2:
        if (func_80055A38(0) == 1) {
            D_800E6280.unk_110D += 1;
        } else {
            D_800E7D14[D_800E6280.unk_1118] = 0xFF;
            func_80042908(0xFF);
        }
        break;
    case 3:
        if (func_80055AFC(0) == 1) {
            func_8013E71C();
            func_8013BB10();
            D_800E6280.unk_110D += 1;
        } else {
            D_800E7D14[D_800E6280.unk_1118] = 0xFF;
            func_80042908(0xFF);
        }
        break;
    case 4:
        func_800AE0B0("%d file replay", D_800E6280.unk_1118);
        func_80041878();
        func_80042058();
        func_8013B77C();
        main_state_set(0x14);
        break;
    }
}
void func_8013EBEC(void) {
    u8 state;
    s32 result;
    u8 text[256];
    state = D_800E6280.unk_110D;
    switch (state) {
    case 0:
        func_8004E58C();
        func_800AE0F0(text, D_800B32CC[D_800E6280.unk_1118 - 1]);
        strcat(text, "番アルバム　を、");
        func_8004E788(-0x40, 0x20, 0xF, (s32) text, 0);
        func_8004E788(-0x40, 0x30, 0xF, (s32) "作成しています。", 0);
        func_8004E884(2);
        D_800E6280.unk_110D += 1;
    default:
        break;
    case 1:
        if ((result = func_80054694((s32) D_800E6280.unk_1118)) == 1) {
            D_800E6280.unk_110D += 1;
        } else {
            func_80042908(4);
        }
        break;
    case 2:
        func_80053CE0();
        func_80041878();
        func_80042058();
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        if ((result = func_80054590()) == 0x14) {
            D_800E7D14[D_800E6280.unk_1118] = 1;
            D_800E6280.unk_110D += 1;
        } else if (((u32) (*(u32 *)&D_800E6280.unk_1120) >= 0x201U) || (D_800E6280.unk_1115 == 0xF0)) {
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    case 4:
        func_80053D10();
        func_80053CE0();
        D_800E7D36 = D_800E6280.unk_1118;
        func_80054E6C();
        D_800E6280.unk_110D += 1;
        break;
    case 5:
        if ((result = func_80054388()) == 0x13) {
            func_80053D10();
            D_800E6280.unk_110D += 1;
        } else if (((u32) (*(u32 *)&D_800E6280.unk_1120) >= 0x201U) || (D_800E6280.unk_1115 == 0xF0)) {
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    case 6:
        func_8013B77C();
        main_state_set(0x14);
        break;
    }
}
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740",func_8013EE7C);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata",D_8014BCB4);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata",D_8014BCCC);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata",D_8014BCE0);
void func_8013F058(void) {
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BCB4, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014BCCC, 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_8014BCE0, 0);
    k_disp_start(3);
    func_8004284C();
}
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740",func_8013F0DC);
s32 func_8013F208(void) {
    s32 state;
    u8 mode;
    u8 text[0x100];
    state = D_800E6280.unk_110D;
    switch (state) {
    case 0:
        func_80053CE0();
        D_800E6280.unk_110D++;
    default:
        break;
    case 1:
        if (func_8005448C() == 4) {
            D_800E6280.unk_110D++;
        } else if ((mode = D_800E6280.unk_1115) == 0xF0) {
            func_80056284();
            func_80042908(1);
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (mode == 0xD0)) {
            func_80053D10();
            func_80042908(0xFF);
        }
        break;
    case 2:
        if (func_80055A38(0) == 1) {
            func_8013B59C();
            D_800E6280.unk_110D++;
        } else {
            D_800E7D14[D_800E6280.unk_1118] = 0xFF;
            func_80042908(0xFF);
        }
        break;
    case 3:
        if (func_80055AFC(0) == 1) {
            func_8013BB10();
            D_8014D0A8 = 1;
            func_800AE0B0("COPY LOAD SUM !\n");
            D_800E6280.unk_110D++;
        } else {
            D_800E7D14[D_800E6280.unk_1118] = 0xFF;
            func_80042908(0xFF);
        }
        break;
    case 4:
        func_8004E58C();
        func_800AE0F0(text, D_800B3288[D_800E6280.unk_1118]);
        func_800AE100(text, "番アルバムを");
        func_8004E788(-0x40, 0x20, 0xF, text, 0);
        func_8004E788(-0x40, 0x30, 0xF, "どのアルバムにコピーしますか？", 0);
        func_8004E788(-0x40, 0x40, 0xF, "カーソルで選んで下さい。", 0);
        func_8004E884(3);
        func_8004284C();
        break;
    }
}
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740",func_8013F454);
void func_8013F5B0(void) {
    u8 buf[0x108];
    func_8004E58C();
    func_800AE0F0(buf, D_800B3288[D_800E6280.unk_1118]);
    strcat(buf, "番アルバムに");
    func_8004E788(-0x40, 0x20, 0xF, buf, 0);
    func_8004E788(-0x40, 0x30, 0xF, "上書きしてもいいですか？", 0);
    func_8004E788(-0x40, 0x40, 0xF, D_800B3C70, 0);
    func_8004E788(0x14, 0x40, 1, func_8006CA9C(), 0);
    func_8004E788(-0x40, 0x50, 0xF, D_800B3C88, 0);
    func_8004E788(0x14, 0x50, 2, func_8006CAE0(), 0);
    func_8004E884(6);
    func_8004284C();
}
void func_8013F6BC(void) {
    if (D_800E6280.unk_F88 & 0x20) {
        func_80044750(0x501);
        func_8004284C();
    } else if (D_800E6280.unk_F88 & 0x40) {
        func_80042940(0);
        D_800E6280.unk_110D = 2;
    }
}
void func_8013F71C(void) {
    if (D_800E8BEE) {
        func_80042940(0xC7);
    } else {
        func_80042940(0xC8);
    }
}
s32 func_8013F758(void) {
    u8 sp38[0x108];
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004E58C();
        func_800AE0F0(sp38, D_800B3288[D_800E6280.unk_1118]);
        func_800AE100(sp38, "番アルバムに");
        func_8004E788(-0x40, 0x20, 0xF, (s32) sp38, 0);
        func_8004E788(-0x40, 0x30, 0xF, (s32) "コピーします。", 0);
        func_8004E884(2);
        func_80053CE0();
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (func_80054590() == 0x14) {
            D_800E7D14[D_800E6280.unk_1118] = 1;
            D_800E6280.unk_110D += 1;
            func_80053D10();
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (D_800E6280.unk_1115 == 0xF0)) {
            func_800AE0B0("NO MEM CARD \n");
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    case 2:
        func_80053D10();
        D_800E7D36 = D_800E6280.unk_1118;
        D_800E6280.unk_1118 = 0;
        func_80053CE0();
        func_80054E6C();
        D_800E6280.unk_110D += 1;
        break;
    case 3:
        if (func_80054388() == 0x13) {
            func_80053D10();
            func_80042940(0);
            D_800E6280.unk_110D = 2;
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (D_800E6280.unk_1115 == 0xF0)) {
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    }
}
s32 func_8013F984(void) {
    u8 sp38[0x108];
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004E58C();
        func_800AE0F0(sp38, D_800B3288[D_800E6280.unk_1118]);
        func_800AE100(sp38, "番アルバムに");
        func_8004E788(-0x40, 0x20, 0xF, (s32) sp38, 0);
        func_8004E788(-0x40, 0x30, 0xF, (s32) "コピーします。", 0);
        func_8004E884(2);
        func_80053CE0();
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (func_80054590() == 0x14) {
            D_800E7D14[D_800E6280.unk_1118] = 1;
            D_800E6280.unk_110D += 1;
            func_80053D10();
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (D_800E6280.unk_1115 == 0xF0)) {
            func_800AE0B0("NO MEM CARD \n");
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    case 2:
        func_80042940(0);
        D_800E6280.unk_110D = 2;
        break;
    }
}
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata",D_8014BDE8);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata",D_8014BE00);
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata",D_8014BE10);
void func_8013FB08(void) {
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BDE8, 0);
    set_kanji_string(-0x40, 0x30, 0xF, D_8014BE00, 0);
    set_kanji_string(-0x40, 0x40, 0xF, D_8014BE10, 0);
    k_disp_start(3);
    func_8004284C();
}
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740",func_8013FB8C);
void func_8013FCBC(void) {
    s32 pad;
    u8 buf[0x100];
    func_8004E58C();
    func_800AE0F0(buf, D_800B3288[D_800E6280.unk_1118]);
    strcat(buf, "番アルバムを");
    func_8004E788(-0x40, 0x20, 0xF, buf, 0);
    func_8004E788(-0x40, 0x30, 0xF, "本当に消去していいですか？", 0);
    func_8004E788(-0x40, 0x40, 0xF, D_800B3C70, 0);
    func_8004E788(0x14, 0x40, 1, func_8006CA9C(), 0);
    func_8004E788(-0x40, 0x50, 0xF, D_800B3C88, 0);
    func_8004E788(0x14, 0x50, 2, func_8006CAE0(), 0);
    func_8004E884(6);
    func_8004284C();
}
void func_8013FDC8(void) {
    if (D_800E6280.unk_F88 & 0x20) {
        func_80044750(0x501);
        if (D_800E8BEE) {
            func_80042940(0xE4);
        } else {
            func_80042940(0xE5);
        }
    } else if (D_800E6280.unk_F88 & 0x40) {
        func_80042940(0);
    }
}
void func_8013FE40(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        if (func_80054704((s32) D_800E6280.unk_1118) == 1) {
            func_800AE0B0("DELETE OK");
            D_800E7D14[D_800E6280.unk_1118] = 0;
            D_800E6280.unk_110D += 1;
        } else {
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    case 1:
        func_80053CE0();
        D_800E7D36 = D_800E6280.unk_1118;
        D_800E6280.unk_1118 = 0;
        D_800E6280.unk_110D += 1;
        break;
    case 2:
        if (func_80054388() == 0x13) {
            func_80053D10();
            func_80056284();
            func_80042908(1);
        } else if (((u32) D_800E6280.unk_1120 >= 0x201U) || (D_800E6280.unk_1115 == 0xF0)) {
            func_80053D10();
            func_80056284();
            func_80042908(1);
        }
        break;
    }
}
void func_8013FF98(void) {
    if (D_800E6280.unk_110D == 0) {
        if (func_80054704(D_800E6280.unk_1118) == 1) {
            func_800AE0B0("DELETE OK");
            D_800E7D14[D_800E6280.unk_1118] = 0;
            func_80042940(0);
            D_800E6280.unk_110D = 2;
            return;
        }
        func_80053D10();
        func_80056284();
        func_80042908(1);
    }
}
void func_80140024(void) {
    func_80042940(0);
}
void func_80140044(void) {
    u8 state;
    D_800E6280.unk_1104.w += 1;
    state = D_800E6280.unk_110A;
    switch (state) {
    case 0x0:
        func_8013C9F0();
        return;
    case 0x1:
        func_8013CB44();
        return;
    case 0x2:
        func_8013CEC8();
        return;
    case 0x3:
        func_8013D054();
        return;
    case 0x20:
        func_8013D2AC();
        return;
    case 0x21:
        func_8013D4CC();
        return;
    case 0x22:
        func_8013D62C();
        return;
    case 0x23:
        func_8013D6C0();
        return;
    case 0x24:
        func_8013D7F4();
        return;
    case 0x40:
        func_8013D874();
        return;
    case 0x41:
        func_8013D954();
        return;
    case 0x42:
        func_8013D9A0();
        return;
    case 0x43:
        func_8013DA08();
        return;
    case 0x44:
        func_8013DA40();
        return;
    case 0x45:
        func_8013DAC4();
        return;
    case 0x46:
        func_8013DBD0();
        return;
    case 0x47:
        func_8013DC54();
        return;
    case 0x48:
        func_8013DC84();
        return;
    case 0x49:
        func_8013DD48();
        return;
    case 0x4A:
        func_8013DD94();
        return;
    case 0x4C:
        func_8013DF88();
        return;
    case 0xA0:
        func_8013E30C();
        return;
    case 0xA1:
        func_8013E390();
        return;
    case 0xA2:
        func_8013E3C8();
        return;
    case 0xA3:
        func_8013E430();
        return;
    case 0xA4:
        func_8013E59C();
        return;
    case 0xA5:
        func_8013E6A8();
        return;
    case 0xA6:
        func_8013E80C();
        return;
    case 0xA7:
        func_8013E9FC();
        return;
    case 0xA8:
        func_8013EBEC();
        return;
    case 0xA9:
        func_8013EE7C();
        return;
    case 0xC0:
        func_8013F058();
        return;
    case 0xC1:
        func_8013F0DC();
        return;
    case 0xC2:
        func_8013F208();
        return;
    case 0xC3:
        func_8013F454();
        return;
    case 0xC4:
        func_8013F5B0();
        return;
    case 0xC5:
        func_8013F6BC();
        return;
    case 0xC6:
        func_8013F71C();
        return;
    case 0xC7:
        func_8013F758();
        return;
    case 0xC8:
        func_8013F984();
        return;
    case 0xE0:
        func_8013FB08();
        return;
    case 0xE1:
        func_8013FB8C();
        return;
    case 0xE2:
        func_8013FCBC();
        return;
    case 0xE3:
        func_8013FDC8();
        return;
    case 0xE4:
        func_8013FE40();
        return;
    case 0xE5:
        func_8013FF98();
        return;
    case 0xE6:
        func_80140024();
        return;
    default:
        func_8013B77C();
        main_state_set(0x10);
        return;
    }
}
s32 func_80140430(void) {
    u32 i;
    switch (D_800E6280.unk_110D) {
    case 0:
        i = D_800E6280.unk_1118 - 1;
        func_80046290(D_800B3A3C[i * 2], D_800B3A3C[i * 2 + 1], i % 13);
        func_80044750(0x300);
        func_800AE0B0("CALL XA");
        D_800E6280.unk_110D++;
        break;
    case 1:
        if (func_800460EC() & 4) {
            D_800E6280.unk_110D++;
        }
        break;
    case 2:
        if (func_800460EC() & 2) {
            D_800E6280.unk_110D++;
        }
        break;
    case 3:
        func_80053D10();
        if (D_800E8BEE != 0) {
            func_80042908(2);
            func_80042940(2);
        } else {
            func_80056284();
            func_80042908(1);
        }
        break;
    }
}
s32 func_80140574(void) {
    u32 i;
    switch (D_800E6280.unk_110D) {
    case 0:
        i = D_800E6280.unk_1118 - 1;
        func_80046290(D_800B394C[i * 2], D_800B394C[i * 2 + 1], i % 13);
        func_80044750(0x300);
        func_800AE0B0("CALL XA");
        D_800E6280.unk_110D++;
        break;
    case 1:
        if (func_800460EC() & 4) {
            D_800E6280.unk_110D++;
        }
        break;
    case 2:
        if (func_800460EC() & 2) {
            D_800E6280.unk_110D++;
        }
        break;
    case 3:
        func_80053D10();
        func_80056284();
        func_80042908(1);
        break;
    }
}
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740",func_8014068C);
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740",func_80140B20);
void func_80140BA4(void) {
    if (D_800E6280.unk_110D++ >= 0x41U) {
        func_8013B77C();
        func_80057390(0);
        if (D_800E6280.unk_03E == 0x62 && D_800E6280.unk_03F == 3 && D_800E6280.unk_040 == 1) {
            func_8013E71C();
            func_80041878();
            func_80041F48();
            D_800E6280.unk_F5E = 0;
            main_state_set(0x14);
        } else if (D_800E6280.unk_03E == 0x5F && D_800E6280.unk_03F == 4 && D_800E6280.unk_040 == 4) {
            func_8013E71C();
            func_80041878();
            func_80041F48();
            D_800E6280.unk_F5E = 0;
            main_state_set(0x14);
        } else {
            func_800674B0();
            main_state_set(0x34);
            func_80042908(0xFF);
        }
    }
    D_8011ECA4 = 1;
}
s32 func_80140CCC(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8014068C();
        break;
    case 1:
        func_80140BA4();
        break;
    }
}
INCLUDE_RODATA("asm/ovl/NAME_ENT/data/NAME_ENT/80139740.rodata",D_8014BEB0);
void func_80140D20(void) {
    func_8004E58C();
    set_kanji_string(-0x40, 0x20, 0xF, D_8014BEB0, 0);
    k_disp_start(2);
    func_8004284C();
}
void func_80140D6C(void)
{
  u8 *p;
  u8 *new_var;
  if (D_800E6280.unk_F88 & 0x860)
  {
    p = (u8 *) (&D_800E7D10); do
    {
      if (p[4] == 1)
      {
        break;
      }
      p++;
    }
    while (p != (&D_800E7D1F));
    new_var = ((u8 *) (&D_800E7D10)) - -15;
    if (p == new_var)
    {
      func_8004284C();
      return;
    }
    func_80042908(2);
    if (D_800E8BEE != 0)
    {
      func_80042940(1);
      return;
    }
    func_80042940(0x20);
  }
}
void func_80140E18(void) {
    func_8004E58C();
    func_8004E788(-0x40, 0x20, 0xF, "別のメモリーカードを差し込み口", 0);
    func_8004E788(-0x40, 0x30, 0xF, "１にセットするか、他のゲームの", 0);
    func_8004E788(-0x40, 0x40, 0xF, "データを消去してください。", 0);
    func_8004E884(2);
    func_8004284C();
}
void func_80140E9C(void) {
    if (D_800E6280.unk_F88 & 0x860) {
        func_80042908(1);
        func_80042940(6);
    }
}
void func_80140ED4(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80140D20();
        return;
    case 1:
        func_80140D6C();
        return;
    case 2:
        func_80140E18();
        return;
    case 3:
        func_80140E9C();
        return;
    }
}
void func_80140F54(void) {
    if (D_800E8BEE) {
        func_80042940(1);
    } else {
        func_80042940(2);
    }
}
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740",func_80140F90);
INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80139740",func_80141820);
s32 func_80141FA4(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80140F54();
        break;
    case 1:
        func_80140F90();
        break;
    case 2:
        func_80141820();
        break;
    }
    D_800E6280.unk_1092 = 0;
}
s32 func_80142028(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004E58C();
        func_8004E788(-0x40, 0x20, 0xF, "未初期化のメモリーカードです。", 0);
        func_8004E884(1);
        D_800E6280.unk_110D++;
    default:
        break;
    case 1:
        if (D_800E6280.unk_F88 & 0x860) {
            D_800E6280.unk_110D++;
        }
        break;
    case 2:
        func_8004E58C();
        func_8004E788(-0x40, 0x20, 0xF, "初期化していいですか？", 0);
        func_8004E788(-0x40, 0x30, 0xF, D_800B3C70, 0);
        func_8004E788(0x14, 0x30, 1, func_8006CA9C(), 0);
        func_8004E788(-0x40, 0x40, 0xF, D_800B3C88, 0);
        func_8004E788(0x14, 0x40, 2, func_8006CAE0(), 0);
        func_8004E884(5);
        D_800E6280.unk_110D++;
        break;
    case 3:
        if (D_800E6280.unk_F88 & 0x20) {
            if (func_80054108() == 1) {
                func_80042908(2);
            } else {
                func_80042908(1);
            }
        } else if (D_800E6280.unk_F88 & 0x40) {
            D_800E6280.unk_110D++;
        }
        break;
    case 4:
        func_8004E58C();
        func_8004E788(-0x40, 0x20, 0xF, "別のメモリーカードを", 0);
        func_8004E788(-0x40, 0x30, 1, "差し込み口１", 0);
        func_8004E788(-0x40, 0x30, 0xF, "　　　　　　にセットして、", 0);
        func_8004E788(-0x40, 0x40, 0xF, "　ボタンを押してください。", 0);
        func_8004E788(-0x40, 0x40, 1, func_8006CA9C(), 0);
        func_8004E884(5);
        D_800E6280.unk_110D++;
        break;
    case 5:
        if (D_800E6280.unk_F88 & 0x20) {
            func_80042908(1);
        }
        break;
    }
}
void func_801422F0(void) {
    if (D_800E6280.unk_110A == 0) {
        func_80142028();
    }
}
void func_8014231C(void) {
    s32 i;
    for (i = 0; i < 13; i++) {
        D_8011ECD0[i * 0x44 + 8707] &= 0x7F;
    }
    for (i = 0; i < 11; i++) {
        D_8011ECD0[i * 0x44 + 9591] &= 0x7F;
    }
    func_8013AF78(0);
    D_800E6280.unk_1100 += 1;
    switch (D_800E6280.unk_1109) {
    case 0x0:
        func_8013C4A8();
        break;
    case 0x1:
        func_8013C934();
        break;
    case 0x2:
        func_80140044();
        break;
    case 0x3:
        func_80140CCC();
        break;
    case 0x4:
        func_80140ED4();
        break;
    case 0xC0:
        func_801422F0();
        break;
    case 0xD0:
        func_80141FA4();
        break;
    case 0xF0:
        func_80140430();
        break;
    case 0xFF:
        func_80140574();
        break;
    }
    func_8013B5F0();
    func_801398EC();
    func_8004EC24();
    func_8006BA40();
    func_8013B0A0();
    func_801425C0();
    func_80142560();
    if (D_8014D0A8 != 0) {
        D_800E6280.unk_1092 = 0;
    }
}
void func_80142560(void) {
    if ((D_800E6280.unk_1109 | D_800E6280.unk_110A) != 0) {
        D_801230D0 -= 1;
    }
    if (D_801230D0 > 0) {
        func_8004AC18(D_801230D0);
    }
}
s32 func_801425C0(void) {
    s32 temp_t3;
    s32 temp_t7;
    s32 var_s0;
    for (var_s0 = 0; var_s0 < 14; var_s0++) {
        if (((NameMetricWork *)&D_800E7D10)->slot_status[var_s0 + 1] == 0) {
            func_80049A40(((var_s0 / 7) * 0x1F) - 0x90, ((var_s0 % 7) * 0x10) - 0x12, 0x1E, 0xF, 2, 0x808080, 0x81);
        }
        if (((NameMetricWork *)&D_800E7D10)->slot_status[var_s0 + 1] == 0xFF) {
            func_80049A40(((var_s0 / 7) * 0x1F) - 0x90, ((var_s0 % 7) * 0x10) - 0x12, 0x1E, 0xF, 2, 0x8080, 0x81);
        }
    }
    func_800494BC(0, 0, 2, 2, 0);
    if (D_800E6280.unk_1109 != 0) {
        if (D_800E8BEE != 0) {
            temp_t7 = D_800E6280.unk_10F8 & 0x1F;
            if (temp_t7 != 0) {
                if (temp_t7 == 0x10) {
                    ((DisplayRecord44 *)D_8011ECD0)[126].unk_03 &= 0x7F;
                }
            } else {
                ((DisplayRecord44 *)D_8011ECD0)[126].unk_03 |= 0x80;
            }
            ((DisplayRecord44 *)D_8011ECD0)[126].unk_3C[7] = 0x11;
            ((DisplayRecord44 *)D_8011ECD0)[127].unk_3C[7] = 0;
        } else {
            temp_t3 = D_800E6280.unk_10F8 & 0x1F;
            if (temp_t3 != 0) {
                if (temp_t3 == 0x10) {
                    ((DisplayRecord44 *)D_8011ECD0)[127].unk_03 &= 0x7F;
                }
            } else {
                ((DisplayRecord44 *)D_8011ECD0)[127].unk_03 |= 0x80;
            }
            ((DisplayRecord44 *)D_8011ECD0)[126].unk_3C[7] = 0;
            ((DisplayRecord44 *)D_8011ECD0)[127].unk_3C[7] = 0x11;
        }
    }
}
void func_80142880(void) {
    s8 *sel;
    menu_check(1, D_8011ECF6, D_8011ECFA);
    func_8004FC10(1);
    sel = &D_800E6280.unk_1094;
    if (D_800E6280.unk_F88 & 0x20) {
        switch (*sel) {
        case 0:
            func_80142A3C();
            func_80042940(0xC0);
            func_800AE0B0("0");
            break;
        case 1:
            func_80142A3C();
            func_80042940(0xA0);
            func_800AE0B0("1");
            break;
        case 2:
            func_80142A3C();
            func_80042940(0xE0);
            func_800AE0B0("2");
            break;
        case 3:
            func_80042940(0);
            func_800AE0B0("3");
            break;
        }
        func_800415B4(0, 0x40);
        if (D_800E6280.unk_1094 >= 0) {
            D_8014D0D4 = 1;
        }
    }
    if (func_801429BC() != 0) {
        func_800415B4(0, 0x40);
    }
}
s32 func_801429BC(void) {
    if (D_800E6280.unk_F88 & 0x20) {
        if (D_8011ECF6 < -0x50 && D_8011ECF6 >= -0x8F) {
            if (D_8011ECFA < -0x18 && D_8011ECFA >= -0x27) {
                func_80044750(0x50F);
                func_80042908(0xD0);
                return 1;
            }
        }
    }
    return 0;
}
void func_80142A3C(void) {
    s32 i;
    for (i = 0; i < 13; i++) D_801217D0[i].unk_00 = 0x80000000;
}
