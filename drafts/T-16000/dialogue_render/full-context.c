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
typedef struct GsDateBits {
               u32 month : 4;
    u32 day : 5;
    u32 rest : 23;
} GsDateBits;
typedef union GsWord {
               GsDateBits date;
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
               u32 f15 : 5;
    u32 f20 : 4;
    u32 rest : 8;
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
extern u8 D_801210F2;
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
extern u8 D_8012139A;
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
extern u8 D_801215BA;
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
void func_8006CD14(void);
void schdeule_girls_param(void);
void func_80070ECC(void);
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
s32 select_girl_main(s32 arg0);
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
void func_80041840(void);
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
s32 func_80049FF0(s16 x0, s16 y0, s16 x1, s16 y1, s32 arg4, s32 arg5, s32 arg6);
s32 SsVabTransCompleted();
s32 dialogue_render();
void select_girl_init();
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800C9F60);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800C9FF8);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA048);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA108);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA120);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA124);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA128);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA130);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA134);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA138);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA13C);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA140);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA144);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA148);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA14C);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA160);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA164);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA168);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA16C);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA174);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA17C);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA188);
INCLUDE_RODATA("asm/data/main/8007C030.data",girl_name_buf);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA19C);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA1DC);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA23C);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA248);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA250);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA25C);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA280);
u8 D_800CA2A4[4] = { 0, 1, 2, 0 };
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA2A8);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA2AC);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA2B0);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA2BC);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA2C0);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA2C4);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA2C8);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA2F0);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA300);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA318);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA330);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA348);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA360);
INCLUDE_RODATA("asm/data/main/8007C030.data",D_800CA36C);
void func_80084C98(s32 idx, u8 val);
extern s32 D_80122CD0[];
extern s32 D_80122D48;
void SD_GetCDLevel(CdLevelWork *arg0) {
    s32 shift;
    u32 left;
    u32 right;
    arg0->unk_00 = 0U;
    arg0->unk_02 = 0U;
    if (((u32) D_80125D14 >> 0x18) == 0) {
        if (!(D_80125D10.unk_00 & 0x4000) && (func_80094B80(0) == 1)) {
            if (SsVabTransCompleted(0) == 1) {
                func_80094C60(0);
                func_80094C90((s32) D_80126080, 0x200);
                func_80094B80(1);
                func_80094C60(0x400);
                func_80094C90((s32) &D_80126080[0x200], 0x200);
                func_80094B80(1);
                switch (arg0->unk_08 & 0xF) {
                case 1:
                    SD_DetectCDPeak(&arg0->unk_00, &arg0->unk_02, 0, (s32) arg0->unk_06);
                    break;
                case 2:
                    SD_CalcCDAve(&arg0->unk_00, &arg0->unk_02, 0, (s32) arg0->unk_06);
                    break;
                }
                left = arg0->unk_00;
                right = arg0->unk_02;
                if ((arg0->unk_08 & 0xF0) == 0x20) {
                    left = ((u32) (left * left) >> 0xF);
                    right = ((u32) (right * right) >> 0xF);
                }
                switch (arg0->unk_08 & 0xF00) {
                case 0x100:
                    if (left < 0x3333U) {
                        left *= 4;
                    } else {
                        left = (left >> 2) + 0xC000;
                    }
                    if (right < 0x3333U) {
                        right *= 4;
                    } else {
                        right = (right >> 2) + 0xC000;
                    }
                    break;
                case 0x400:
                    if (left < 0x9999U) {
                        left = ((u32) (left * 4) / 6U);
                    } else {
                        left = ((u32) (left * 6) >> 2) + 0xFFFF7FFF;
                    }
                    if (right < 0x9999U) {
                        right = ((u32) (right * 4) / 6U);
                    } else {
                        right = ((u32) (right * 6) >> 2) + 0xFFFF7FFF;
                    }
                    break;
                case 0x800:
                    if (left < 0xB333U) {
                        left = ((u32) (left * 2) / 7U);
                    } else {
                        left = ((u32) (left * 8) / 3U) + 0xFFFE5555;
                    }
                    if (right < 0xB333U) {
                        right = ((u32) (right * 2) / 7U);
                    } else {
                        right = ((u32) (right * 8) / 3U) + 0xFFFE5555;
                    }
                    break;
                }
                shift = 0x10 - arg0->unk_04;
                arg0->unk_00 = (left >> shift) & 0xFFFF;
                arg0->unk_02 = (right >> shift) & 0xFFFF;
            }
        }
    }
}
void SD_DetectCDPeak(u16 *l, u16 *r, u16 pos, u16 n) {
    u16 i;
    u16 step;
    s16 a;
    s16 b;
    i = 0;
    if (n != 0) {
        step = 0x100 / n;
        do {
            a = D_80126080[pos];
            b = D_80126080[pos + 0x200];
            if (a & 0x8000) {
                a = ~(a - 1);
            }
            if (b & 0x8000) {
                b = ~(b - 1);
            }
            if (*l < a) {
                *l = a;
            }
            if (*r < b) {
                *r = b;
            }
            pos += step;
            i++;
        } while (i < n);
    }
    *l *= 2;
    *r *= 2;
}
void SD_CalcCDAve(s16 *left, s16 *right, u16 start, u16 count) {
    s32 left_sum;
    s32 right_sum;
    u16 index;
    s16 left_sample;
    s16 right_sample;
    left_sum = 0;
    right_sum = 0;
    for (index = 0; index < count; index++) {
        left_sample = D_80126080[start];
        right_sample = D_80126080[start + 0x200];
        if (left_sample & 0x8000) {
            left_sample = ~(left_sample - 1);
        }
        left_sum += left_sample;
        if (right_sample & 0x8000) {
            right_sample = ~(right_sample - 1);
        }
        right_sum += right_sample;
        start += (u16)(256U / count);
    }
    *left = ((u32)(left_sum * 2) / count) & 0xFFFF;
    *right = ((u32)(right_sum * 2) / count) & 0xFFFF;
}
s16 getCDlevel(void) {
    if (D_80125D10.unk_00 & 0x400) {
        return (u32)(D_80125E60 + D_80125E62) >> 1;
    }
    return 0;
}
void addr_init_weekly_sd(s32 arg0) {
    u16 *p;
    D_80125CA4 = 0x801E0000;
    p = D_800C9F60 + arg0 % 25 * 3;
    D_80125CB0 = p[0] + 0x801E0000;
    D_80125CA8 = p[1] + 0x801E0000;
    D_80125CAC = p[2] + 0x801E0000;
}
void addr_init_holiday_sd(s32 arg0) {
    u16 *p;
    D_80125CA4 = 0x801E0000;
    p = D_800C9FF8 + arg0 % 13 * 3;
    D_80125CB0 = p[0] + 0x801E0000;
    D_80125CA8 = p[1] + 0x801E0000;
    D_80125CAC = p[2] + 0x801E0000;
}
void addr_init_club_sd(s32 arg0) {
    u16 *p;
    D_80125CA4 = 0x801E0000;
    p = D_800CA048 + arg0 % 32 * 3;
    D_80125CB0 = p[0] + 0x801E0000;
    D_80125CA8 = p[1] + 0x801E0000;
    D_80125CAC = p[2] + 0x801E0000;
}
void addr_init_else_sd(s32 arg0) {
    u16 *p;
    D_80125CA4 = 0x801E0000;
    p = D_800CA108 + arg0 % 3 * 3;
    D_80125CB0 = p[0] + 0x801E0000;
    D_80125CA8 = p[1] + 0x801E0000;
    D_80125CAC = p[2] + 0x801E0000;
}
void addr_init_bustup(void) {
    D_800CA120 = 0x80180084;
    D_800CA124 = 0x80180098;
    D_800CA128 = 0x8018009C;
    D_800CA130 = 0x801800AC;
}
void func_8007C784(void) {
    s32 pad;
    u8 sp2B;
    u8 sp2A;
    u8 sp29;
    if (cd_read_sync_status() & 1) {
        func_8004284C();
        return;
    }
    if ((u32)D_800E6280.unk_1104.w++ >= 0x401) {
        func_800452C4();
        sp29 = D_800B5939;
        sp2A = *(u8 *)&D_800B5948;
        sp2B = D_800B593C;
        dec_bg_reset();
        *(u8 *)&D_800B5948 = sp2A;
        D_800B593C = sp2B;
        D_800B5939 = sp29;
        D_800E6280.unk_110A -= 1;
    }
}
void func_8007C844(void) {
    D_80122CF8 = 1;
    D_800B593C = 0x80;
    func_8004284C();
}
void func_8007C878(void) {
    D_80122CF8 = 0;
    D_800B593C = 0;
    func_8004284C();
}
void func_8007C8A4(void) {
    if (func_8007C8D8() == 1) {
        func_8004284C();
    }
}
s32 func_8007C8D8(void) {
    s16 top_y;
    s16 bottom_y;
    s16 left_x;
    s16 outer;
    GameState *g;
    s32 width;
    g = &D_800E6280;
    srand(g->unk_10F8);
    if (g->unk_1104.u == 0) {
        D_80122D3C = g->unk_03F % 7U;
    }
    if (D_80122D3C != 6) {
        D_80122CF8 = 2;
    }
    D_800B593C += 8;
    if ((u8)D_800B593C >= 0x80U) {
        D_800B593C = 0x80;
    }
    switch (D_80122D3C) {
    case 1:
        for (outer = g->unk_1104.u * 5; outer < 0x50; outer++) {
            func_80049FF0(-0xA0, -0x29 - outer, 0xA0, -0x29 - outer, 0, 0, 0xD);
            func_80049FF0(-0xA0, outer - 0x29, 0xA0, outer - 0x29, 0, 0, 0xD);
        }
        break;
    case 2:
        for (outer = g->unk_1104.u; outer < 0x10; outer++) {
            for (left_x = 0; left_x < 10; left_x++) {
                top_y = left_x * 16 + outer - 0x78;
                func_80049FF0(-0xA0, top_y, 0xA0, top_y, 0, 0, 0xD);
            }
        }
        break;
    case 3:
        for (outer = g->unk_1104.u << 5; outer < 0x1E0; outer++) {
            if (outer < 0xA0) {
                func_80049FF0(outer - 0xa0, -0x78, -0xA0, outer - 0x78, 0, 0, 0xD);
            } else if (outer < 0x140) {
                func_80049FF0(outer - 0xa0, -0x78, outer - 0x140, 0x27, 0, 0, 0xD);
            } else {
                func_80049FF0(0xA0, outer - 0x1b8, outer - 0x140, 0x27, 0, 0, 0xD);
            }
        }
        break;
    case 4:
        for (outer = g->unk_1104.u << 6; outer < 0x1E0; outer += 4) {
            if (outer < 0xA0) {
                func_80049FF0(outer - 0xa0, -0x78, -0xA0, outer - 0x78, 0, 0, 0xD);
            } else if (outer < 0x140) {
                func_80049FF0(outer - 0xa0, -0x78, outer - 0x140, 0x27, 0, 0, 0xD);
            } else {
                func_80049FF0(0xA0, outer - 0x1b8, outer - 0x140, 0x27, 0, 0, 0xD);
            }
        }
        for (outer = g->unk_1104.u << 6; outer < 0x1E0; outer += 4) {
            if (outer < 0xA0) {
                func_80049FF0(outer - 0x9f, -0x78, -0xA0, outer - 0x77, 0, 0, 0xD);
            } else if (outer < 0x140) {
                func_80049FF0(outer - 0x9f, -0x78, outer - 0x13f, 0x27, 0, 0, 0xD);
            } else {
                func_80049FF0(0xA0, outer - 0x1b7, outer - 0x13f, 0x27, 0, 0, 0xD);
            }
        }
        if ((g->unk_1104.u << 6) < 0x1E0U) left_x = 0;
        else left_x = (g->unk_1104.u << 6) - 0x1E0;
        for (outer = left_x; outer < 0x1E0; outer += 4) {
            if (outer < 0xA0) {
                func_80049FF0(outer - 0x9e, -0x78, -0xA0, outer - 0x76, 0, 0, 0xD);
            } else if (outer < 0x140) {
                func_80049FF0(outer - 0x9e, -0x78, outer - 0x13e, 0x27, 0, 0, 0xD);
            } else {
                func_80049FF0(0xA0, outer - 0x1b6, outer - 0x13e, 0x27, 0, 0, 0xD);
            }
        }
        for (outer = left_x; outer < 0x1E0; outer += 4) {
            if (outer < 0xA0) {
                func_80049FF0(outer - 0x9d, -0x78, -0xA0, outer - 0x75, 0, 0, 0xD);
            } else if (outer < 0x140) {
                func_80049FF0(outer - 0x9d, -0x78, outer - 0x13d, 0x27, 0, 0, 0xD);
            } else {
                func_80049FF0(0xA0, outer - 0x1b5, outer - 0x13d, 0x27, 0, 0, 0xD);
            }
        }
        break;
    case 5:
        for (outer = g->unk_1104.u * 10; outer < 0xA0; outer++) {
            width = 0xA0 - outer;
            left_x = width - 0xA0;
            top_y = (width / 2) - 0x78;
            func_80049FF0(left_x, top_y, (0x9F - width), top_y, 0, 0, 0xD);
            bottom_y = 0x27 - (width / 2);
            func_80049FF0(left_x, bottom_y, (0x9F - width), bottom_y, 0, 0, 0xD);
            func_80049FF0(left_x, top_y, left_x, bottom_y, 0, 0, 0xD);
            func_80049FF0(0xA0-width, top_y, (0x9F - width), bottom_y, 0, 0, 0xD);
        }
        break;
    case 6:
        if (g->unk_1104.u < 0x10U) set_tarao_bg(g->unk_1104.u >> 1);
        else set_tarao_bg(8);
        break;
    default:
        for (outer = 0; outer < 0xF; outer++) {
            if ((outer << 5) < 0xA0) {
                for (left_x = g->unk_1104.u * 2; left_x < 0x20; left_x++) {
                    func_80049FF0((outer << 5)+left_x-0xA0, -0x78, -0xA0, (outer << 5)+left_x-0x78, 0, 0, 0xD);
                }
            } else if ((outer << 5) < 0x140) {
                for (left_x = g->unk_1104.u * 2; left_x < 0x20; left_x++) {
                    func_80049FF0((outer << 5)+left_x-0xA0, -0x78, (outer << 5)+left_x-0x140, 0x28, 0, 0, 0xD);
                }
            } else {
                for (left_x = g->unk_1104.u * 2; left_x < 0x20; left_x++) {
                    func_80049FF0(0xA0, (outer << 5)+left_x-0x1B8, (outer << 5)+left_x-0x140, 0x28, 0, 0, 0xD);
                }
            }
        }
        break;
    }
    g->unk_1104.u++;
    if (g->unk_1104.u >= 16U) {
        D_80122CF8 = 2;
        return 1;
    }
    return 0;
}
void set_tarao_bg(s32 arg0) {
    s16 row;
    s16 col;
    s32 x;
    s32 y;
    s32 row_y;
    s32 offset;
    for (row = 0; row < 20; row++) {
        row_y = row * 8;
        for (col = 0; col < 16; col += 2) {
            x = col * 10;
            offset = (col % 2) * 8;
            y = row_y + offset;
            set_tarao_sprt(x, y, 10, arg0, x, y, 12, 128, 128, 128);
        }
        for (col = 1; col < 16; col += 2) {
            x = col * 10;
            offset = (col % 2) * 8;
            y = row_y + offset - arg0;
            set_tarao_sprt(x, y, 10, arg0, x, y, 12, 128, 128, 128);
        }
    }
    if (D_800B5939 != 0) {
        goto_tpage(0x140, 0x100, 12, 0, 2);
    } else {
        goto_tpage(0x2C0, 0, 12, 0, 2);
    }
    for (row = 0; row < 20; row++) {
        row_y = row * 8;
        for (col = 16; col < 32; col += 2) {
            x = col * 10;
            offset = (col % 2) * 8;
            y = row_y + offset;
            set_tarao_sprt(x, y, 10, arg0, x - 128, y, 12, 128, 128, 128);
        }
        for (col = 17; col < 32; col += 2) {
            x = col * 10;
            offset = (col % 2) * 8;
            y = row_y + offset - arg0;
            set_tarao_sprt(x, y, 10, arg0, x - 128, y, 12, 128, 128, 128);
        }
    }
    if (D_800B5939 != 0) {
        goto_tpage(0x1E0, 0x100, 12, 0, 2);
    } else {
        goto_tpage(0x360, 0, 12, 0, 2);
    }
}
void set_tarao_sprt(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)
s16 arg0;
s16 arg1;
s16 arg2;
s16 arg3;
u8 arg4;
u8 arg5;
s16 arg6;
u8 arg7;
u8 arg8;
u8 arg9;
{
    u8 *p;
    p = (u8 *)GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(p);
    SetSemiTrans(p, 0);
    SetShadeTex(p, 0);
    p[4] = arg7;
    p[5] = arg8;
    p[6] = arg9;
    *(s16 *)(p + 8) = arg0;
    *(s16 *)(p + 0xA) = arg1;
    *(s16 *)(p + 0x10) = arg2;
    *(s16 *)(p + 0x12) = arg3;
    p[0xC] = arg4;
    p[0xD] = arg5;
    if (D_8011ECA0 == 0) {
        *(s16 *)(p + 0xA) += 0xF0;
    }
    AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + arg6 * 4, p);
}
void set_tarao_rect(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, u8 arg5, u8 arg6, u8 arg7) {
    u8 *p;
    p = (u8 *)GetWorkBase(0x10, D_8011ECA0);
    func_8009F0F4(p);
    SetSemiTrans(p, 0);
    SetShadeTex(p, 0);
    p[4] = arg5;
    p[5] = arg6;
    p[6] = arg7;
    *(s16 *)(p + 8) = arg0;
    *(s16 *)(p + 0xA) = arg1;
    *(s16 *)(p + 0xC) = arg2;
    *(s16 *)(p + 0xE) = arg3;
    if (D_8011ECA0 == 0) {
        *(s16 *)(p + 0xA) += 0xF0;
    }
    AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + arg4 * 4, (s32)p);
}
void normal_date_bg_out(void) {
    if (func_8007D8AC() == 1) {
        func_8004284C();
    }
}
s32 func_8007D8AC(void) {
    s16 line_y;
    s16 bottom_y;
    s16 inner;
    s16 outer;
    s32 limit;
    s32 coordinate;
    D_800B593C -= 8;
    if ((u8)D_800B593C >= 0x81U) {
        D_800B593C = 0;
    }
    if (D_800E6280.unk_1104.u == 0) {
        D_80122D3C = (u8) D_800E6280.unk_03F % 7U;
    }
    if (D_80122D3C == 6) {
        D_80122CF8 = 0;
    }
    switch (D_80122D3C) {
    case 1:
        limit = (s16)(((GsWord *)&D_800E6280.unk_1104)->u * 5);
        if (limit > 0x50) limit = 0x50;
        for (outer=0; outer<limit; outer++) {
            func_80049FF0(-0xA0, outer-0x78, 0xA0, outer-0x78, 0, 0, 6);
            func_80049FF0(-0xA0, 0x27-outer, 0xA0, 0x27-outer, 0, 0, 6);
        }
        break;
    case 2:
        limit = (s16) D_800E6280.unk_1104.h[0];
        if (limit >= 0x11) {
            limit = 0x10;
        }
        outer = 0;
        if (limit > 0) {
            do {
                inner = 0;
                do {
                line_y = (inner * 0x10 - outer) - 0x69;
                func_80049FF0(-0xA0, line_y, 0xA0, line_y, 0, 0, 6);
                inner += 1;
                } while (inner < 0xA);
                outer += 1;
            } while (outer < limit);
        }
        break;
    case 3:
        limit = (s16)(D_800E6280.unk_1104.u << 5);
        if (limit >= 0x1E1) {
            limit = 0x1E0;
        }
        outer = 0;
        if (limit > 0) {
            do {
                if (outer >= 0x141) {
                    func_80049FF0(((0x1DF - outer) - 0xA0), -0x78, -0xA0, ((0x1DF - outer) - 0x78), 0, 0, 0xD);
                } else if (outer >= 0xA1) {
                    func_80049FF0(((0x1DF - outer) - 0xA0), -0x78, ((0x1DF - outer) - 0x140), 0x27, 0, 0, 0xD);
                } else {
                    func_80049FF0(0xA0, ((0x1DF - outer) - 0x1B8), ((0x1DF - outer) - 0x140), 0x27, 0, 0, 0xD);
                }
                outer += 1;
            } while (outer < limit);
        }
        break;
    case 4:
        limit = (s16)(D_800E6280.unk_1104.u << 6);
        if (limit >= 0x1E1) {
            limit = 0x1E0;
        }
        outer = 0;
        if (limit > 0) {
            do {
                if (outer >= 0x141) {
                    func_80049FF0(((0x1DF - outer) - 0xA0), -0x78, -0xA0, ((0x1DF - outer) - 0x78), 0, 0, 0xD);
                } else if (outer >= 0xA1) {
                    func_80049FF0(((0x1DF - outer) - 0xA0), -0x78, ((0x1DF - outer) - 0x140), 0x27, 0, 0, 0xD);
                } else {
                    func_80049FF0(0xA0, ((0x1DF - outer) - 0x1B8), ((0x1DF - outer) - 0x140), 0x27, 0, 0, 0xD);
                }
                outer += 4;
            } while (outer < limit);
            outer = 0;
        }
        if (limit > 0) {
            do {
                if (outer >= 0x141) {
                    func_80049FF0(((0x1DF - outer) - 0x9F), -0x78, -0xA0, ((0x1DF - outer) - 0x77), 0, 0, 0xD);
                } else if (outer >= 0xA1) {
                    func_80049FF0(((0x1DF - outer) - 0x9F), -0x78, ((0x1DF - outer) - 0x13F), 0x27, 0, 0, 0xD);
                } else {
                    func_80049FF0(0xA0, ((0x1DF - outer) - 0x1B7), ((0x1DF - outer) - 0x13F), 0x27, 0, 0, 0xD);
                }
                outer += 4;
            } while (outer < limit);
            outer = 0;
        }
        limit = (s16)((D_800E6280.unk_1104.u << 6) - 0x1E0);
        if (limit >= 0x1E1) {
            limit = 0x1E0;
        }
        if (limit > 0) {
            do {
                if (outer >= 0x141) {
                    func_80049FF0(((0x1DF - outer) - 0x9E), -0x78, -0xA0, ((0x1DF - outer) - 0x76), 0, 0, 0xD);
                } else if (outer >= 0xA1) {
                    func_80049FF0(((0x1DF - outer) - 0x9E), -0x78, ((0x1DF - outer) - 0x13E), 0x27, 0, 0, 0xD);
                } else {
                    func_80049FF0(0xA0, ((0x1DF - outer) - 0x1B6), ((0x1DF - outer) - 0x13E), 0x27, 0, 0, 0xD);
                }
                outer += 4;
            } while (outer < limit);
            outer = 0;
        }
        if (limit > 0) {
            do {
                if (outer >= 0x141) {
                    func_80049FF0(((0x1DF - outer) - 0x9D), -0x78, -0xA0, ((0x1DF - outer) - 0x75), 0, 0, 0xD);
                } else if (outer >= 0xA1) {
                    func_80049FF0(((0x1DF - outer) - 0x9D), -0x78, ((0x1DF - outer) - 0x13D), 0x27, 0, 0, 0xD);
                } else {
                    func_80049FF0(0xA0, ((0x1DF - outer) - 0x1B5), ((0x1DF - outer) - 0x13D), 0x27, 0, 0, 0xD);
                }
                outer += 4;
            } while (outer < limit);
        }
        break;
    case 5:
        limit = (s16)(D_800E6280.unk_1104.u * 0xA);
        if (limit >= 0xA1) {
            limit = 0xA0;
        }
        outer = 0;
        if (limit > 0) {
            do {
                inner = outer - 0xA0;
                coordinate = outer / 2;
                line_y = coordinate - 0x78;
                func_80049FF0(inner, line_y, (0x9F - outer), line_y, 0, 0, 0xD);
                bottom_y = 0x27 - coordinate;
                func_80049FF0(inner, bottom_y, (0x9F - outer), bottom_y, 0, 0, 0xD);
                func_80049FF0(inner, line_y, inner, bottom_y, 0, 0, 0xD);
                func_80049FF0((0xA0 - outer), line_y, (0x9F - outer), bottom_y, 0, 0, 0xD);
                outer += 1;
            } while (outer < limit);
        }
        break;
    case 6:
        if ((u32) D_800E6280.unk_1104.u < 0x10U) {
            set_tarao_bg(8 - ((u32) D_800E6280.unk_1104.u >> 1));
        } else {
            set_tarao_bg(0);
        }
        break;
    default:
        limit = (s16)(D_800E6280.unk_1104.u * 2);
        if (limit >= 0x21) {
            limit = 0x20;
        }
        for (outer = 0; outer < 15; outer++) {
            inner = 0;
            if ((outer << 5) < 0xA0) {
                coordinate = (outer << 5) + 0x1F;
                if (limit > 0) {
                    do {
                        func_80049FF0(((coordinate - inner) - 0xA0), -0x78, -0xA0, ((coordinate - inner) - 0x78), 0, 0, 0xD);
                        inner += 1;
                    } while (inner < limit);
                }
            } else if ((outer << 5) < 0x140) {
                coordinate = (outer << 5) + 0x1F;
                if (limit > 0) {
                    do {
                        func_80049FF0(((coordinate - inner) - 0xA0), -0x78, ((coordinate - inner) - 0x140), 0x28, 0, 0, 0xD);
                        inner += 1;
                    } while (inner < limit);
                }
            } else {
                coordinate = (outer << 5) + 0x1F;
                if (limit > 0) {
                    do {
                        func_80049FF0(0xA0, ((coordinate - inner) - 0x1B8), ((coordinate - inner) - 0x140), 0x28, 0, 0, 0xD);
                        inner += 1;
                    } while (inner < limit);
                }
            }
        }
        break;
    }
    D_800E6280.unk_1104.u++;
    if (D_800E6280.unk_1104.u >= 0x12U) {
        D_80122CF8 = 0;
        return 1;
    }
    return 0;
}
void normal_date_girl_in(void) {
    switch (D_800E6280.unk_1104.u) {
    case 0:
        normal_date_girl_in_init();
        break;
    case 1:
        normal_date_girl_in_main();
        break;
    default:
        func_80046500();
        break;
    }
    func_80057D28(1);
}
s32 normal_date_girl_in_init(void) {
    D_8011ECD0[0x843] = 0;
    D_801217D0[0].unk_14 = D_801217D0[0].unk_15 = D_801217D0[0].unk_16 = 0;
    D_801217D0[1].unk_14 = D_801217D0[1].unk_15 = D_801217D0[1].unk_16 = 0;
    D_800B5940 = 0;
    D_80122CF8 = 0;
    D_80122CF4 = 1;
    D_800E6280.unk_1104.w += 1;
}
s32 normal_date_girl_in_main(void) {
    if (D_800B5940 < 0x80) {
        D_800B5940 += 8;
    } else {
        func_8004284C();
        D_80122CF8 = 1;
        D_801217D0[0].unk_00 &= 0x7FFFFFFF;
        D_801217D0[1].unk_00 &= 0x7FFFFFFF;
        return 0;
    }
    D_801217D0[0].unk_14 = D_801217D0[0].unk_15 = D_801217D0[0].unk_16 = D_801217D0[1].unk_14 = D_801217D0[1].unk_15 = D_801217D0[1].unk_16 = D_800B5940;
    D_8011ECD0[0x843] = D_800B5940;
}
s32 normal_date_girl_suddenin(void) {
    D_80122CF4 = 1;
    D_8011ECD0[0x843] = 0x80;
    D_801217D0[0].unk_14 = D_801217D0[0].unk_15 = D_801217D0[0].unk_16 = 0x80;
    D_801217D0[1].unk_14 = D_801217D0[1].unk_15 = D_801217D0[1].unk_16 = 0x80;
    func_8004284C();
}
void normal_date_girl_out(void) {
    switch (D_800E6280.unk_1104.u) {
    case 0:
        normal_date_girl_out_init();
        break;
    case 1:
        normal_date_girl_out_main();
        break;
    default:
        func_80046500();
        break;
    }
    func_80057D28(1);
}
void normal_date_girl_out_init(void) {
    D_800B5940 = 0x80;
    D_80122CF8 = 0;
    D_800E6280.unk_1104.w = D_800E6280.unk_1104.w + 1;
}
s32 normal_date_girl_out_main(void) {
    if (D_800B5940 >= 8) {
        D_800B5940 -= 8;
    } else {
        D_8011ECD0[0x83F] &= ~0x80;
        D_8011ECD0[0x7FB] &= ~0x80;
        func_8004284C();
        D_801217D0[0].unk_00 |= 0x80000000;
        D_801217D0[1].unk_00 |= 0x80000000;
        D_801217D0[0].unk_14 = D_801217D0[0].unk_15 = D_801217D0[0].unk_16 = 0x80;
        D_801217D0[1].unk_14 = D_801217D0[1].unk_15 = D_801217D0[1].unk_16 = 0x80;
        D_80122CF4 = 0;
        D_80122CF8 = 1;
        return 0;
    }
    D_801217D0[0].unk_14 = D_801217D0[0].unk_15 = D_801217D0[0].unk_16 = D_801217D0[1].unk_14 = D_801217D0[1].unk_15 = D_801217D0[1].unk_16 = D_800B5940;
    D_8011ECD0[0x843] = D_800B5940;
}
s32 normal_date_girl_suddenout(void) {
    D_8011ECD0[0x83F] &= ~0x80;
    D_8011ECD0[0x7FB] &= ~0x80;
    D_801217D0[0].unk_00 |= 0x80000000;
    D_801217D0[1].unk_00 |= 0x80000000;
    D_801217D0[0].unk_14 = D_801217D0[0].unk_15 = D_801217D0[0].unk_16 = 0x80;
    D_801217D0[1].unk_14 = D_801217D0[1].unk_15 = D_801217D0[1].unk_16 = 0x80;
    D_80122CF4 = 0;
    D_80122CF8 = 1;
    func_8004284C();
}
s32 normal_date_bg_fadein(void) {
    D_80122CF8 = 1;
    D_800B593C += 4;
    if (D_800B593C >= 0x80) {
        D_800B593C = 0x80;
    }
    if (D_800B593C >= 0x7D) {
        D_800B593C = 0x80;
        func_8004284C();
    }
}
s32 normal_date_bggirl_fadein(void) {
    u8 x;
    D_8011ECD0[0x83F] |= 0x80;
    normal_date_bg_fadein();
    x = D_800B593C;
    D_8011ECD0[0x843] = x;
    D_801217D0[0].unk_00 &= 0x7FFFFFFF;
    D_801217D0[1].unk_00 &= 0x7FFFFFFF;
    D_801217D0[0].unk_14 = D_801217D0[0].unk_15 = D_801217D0[0].unk_16 = x;
    D_801217D0[1].unk_14 = D_801217D0[1].unk_15 = D_801217D0[1].unk_16 = x;
    D_80122CF4 = 1;
}
void normal_date_bg_fadeout(void) {
    D_80122CF8 = 1;
    D_800B593C -= 4;
    if (D_800B593C >= 0x81) {
        D_800B593C = 0;
    }
    if (D_800B593C < 4) {
        D_80122CF8 = 0;
        D_800B593C = 0;
        func_8004284C();
    }
}
s32 normal_date_bggirl_fadeout(void) {
    u8 x;
    if (D_800B593C < 8) {
        D_80122CF4 = 0;
        D_80122CF8 = 0;
        D_8011ECD0[0x83F] &= ~0x80;
        D_8011ECD0[0x7FB] &= ~0x80;
        D_801217D0[0].unk_14 = D_801217D0[0].unk_15 = D_801217D0[0].unk_16 = 0x80;
        D_801217D0[1].unk_14 = D_801217D0[1].unk_15 = D_801217D0[1].unk_16 = 0x80;
        D_801217D0[0].unk_00 |= 0x80000000;
        D_801217D0[1].unk_00 |= 0x80000000;
    }
    normal_date_bg_fadeout();
    x = D_800B593C;
    D_8011ECD0[0x843] = x;
    D_801217D0[0].unk_14 = D_801217D0[0].unk_15 = D_801217D0[0].unk_16 = x;
    D_801217D0[1].unk_14 = D_801217D0[1].unk_15 = D_801217D0[1].unk_16 = x;
}
void normal_date_move_place(void) {
    switch (D_800E6280.unk_1104.u) {
    case 0:
        normal_date_move_place_init();
        return;
    case 1:
        normal_date_move_place_main();
        return;
    default:
        func_80046500();
        return;
    }
}
void normal_date_move_place_init(void) {
    k_reset(1);
    D_800B5940 = 0x80;
    D_80122CF8 = 0;
    D_800E6280.unk_1104.w = D_800E6280.unk_1104.w + 1;
    func_80058398(1);
}
s32 normal_date_move_place_main(void) {
    if (D_800B5940 >= 8) {
        D_800B5940 -= 8;
    } else {
        func_80058398(1);
        dec_bg_show_switch(1);
        func_8004284C();
        D_801217D0[0].unk_00 |= 0x80000000;
        D_801217D0[1].unk_00 |= 0x80000000;
        D_801217D0[0].unk_14 = D_801217D0[0].unk_15 = D_801217D0[0].unk_16 = 0x80;
        D_801217D0[1].unk_14 = D_801217D0[1].unk_15 = D_801217D0[1].unk_16 = 0x80;
        D_80122CF8 = 1;
        return 0;
    }
    D_801217D0[0].unk_14 = D_801217D0[0].unk_15 = D_801217D0[0].unk_16 = D_801217D0[1].unk_14 = D_801217D0[1].unk_15 = D_801217D0[1].unk_16 = D_800B5940;
    func_80058398(1);
}
void place_init(void) {
    k_reset(0);
    func_8006612C(D_800CA19C);
    func_8004284C();
}
void place_init2(void) {
    k_reset(0);
    func_8006612C(D_800CA1DC);
    func_8004284C();
}
void change_dec_bg(void) {
    dec_bg_show_switch(1);
    func_8004284C();
}
s32 dec_init(void) {
    if ((((u32 *)D_80125C04)[0] & 0xFFFF0000) != 0x38000000 || (((u32 *)D_80125C04)[1] & 0xFF010000) != 0x10000) {
        D_800B5920[2] = 0;
        D_800B5920[3] = 0;
        func_8004284C();
        D_800E6280.unk_110A -= 3;
        return 0;
    }
    func_8005751C(1);
    func_8004284C();
}
void bg_read_sub2(s32 arg0) {
    s32 t;
    D_800B3D60 = 0;
    t = dec_bg_cd_read(arg0, 1);
    if (t == D_800B5939) {
        func_8004284C();
        func_8004284C();
        func_8004284C();
    } else if (t == 1 - D_800B5939) {
        func_8004284C();
        func_8004284C();
    }
}
void check_k_scroll(void) {
    s32 sp38;
    Entry8 *sp48;
    s16 temp_v1;
    s32 temp_a0;
    s32 temp_v0;
    s32 var_a0;
    s32 var_s0;
    u32 temp_s2;
    Entry8 *temp_v0_2;
    Entry8 *temp_v0_3;
    get_ksys();
    temp_v0 = get_now_k();
    temp_s2 = temp_v0 & 0xFF;
    sp48 = (Entry8 *) get_k_work(temp_v0);
    temp_a0 = (var_a0 = temp_s2 + 1);
    sp38 = temp_a0;
    get_k_work(temp_a0);
    if (sp48->unk_02 >= 0x60) {
        var_a0 = sp38;
        if (D_800CA2BC != 0) {
            var_s0 = 1;
            if (temp_s2 >= 2U) {
                do {
                    temp_v0_2 = (Entry8 *) get_k_work(var_s0);
                    temp_v1 = temp_v0_2->unk_02;
                    if ((u32) temp_v1 == 0x30U) {
                        temp_v0_2->unk_07 = 0;
                    } else {
                        temp_v0_2->unk_02 = (s16) (temp_v1 - 0x10);
                    }
                    var_s0 = (s32) ((s16) (var_s0 + 1));
                }
                while (((u32) var_s0) < temp_s2);
            }
            D_800CA2BC = 0;
            sp48->unk_02 = 0x50;
            sp48->unk_07 = 1;
            var_a0 = sp38;
        }
    }
    temp_v0_3 = (Entry8 *) get_k_work(var_a0);
    if (temp_v0_3->unk_02 >= 0x60) {
        D_800CA2BC = 1;
        temp_v0_3->unk_07 = 0;
    }
}
void wait_sub_sub(s16 arg0) {
    s32 t = D_800E6280.unk_1104.w + 1;
    D_800E6280.unk_1104.w = t;
    if ((u32)t >= (u32)arg0) {
        func_8004284C();
    }
}
INCLUDE_ASM("asm/nonmatchings/main/8007C030",dialogue_speak_part);
void func_80081128(void) {
    if (D_800CA2AC > 0x200) {
        if (D_800E6280.unk_F88 & 0x600060) {
            func_80044750(0xB4);
            D_800CA2A8 = 0;
            D_800CA2AC = 0;
            D_800CA2C4 = 1;
        }
    }
}
extern u8 D_800B2228[3];
extern u8 D_800B222C[3];
extern u8 D_800B2230[3];
extern u8 D_800B2234[3];
extern u8 D_800B2238[3];
extern u8 D_800CA248[];
extern u8 D_800CA280[];
s32 dialogue_render(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, void *arg5, void *arg6, s32 arg7) {
    typedef struct { u8 b[3]; } MainCopy3;
    s16 sp2F2;
    s16 sp2E6;
    u8 sp2A4[64];
    u8 sp28C[24];
    u8 sp8C[512];
    u8 sp84[3];
    u8 sp80[3];
    u8 sp7C[3];
    u8 sp78[3];
    u8 sp74[3];
    u8 sp70[3];
    s32 sp68;
    s32 sp64;
    u8 *sp60;
    s16 temp_v0;
    s16 temp_v0_5;
    s16 temp_v1_3;
    s16 temp_v1_4;
    s16 var_a0;
    s16 var_s0;
    s16 var_s0_10;
    s16 var_s0_11;
    s16 var_s0_12;
    s16 var_s0_13;
    s16 var_s0_14;
    s16 var_s0_15;
    s16 var_s0_16;
    s16 var_s0_17;
    s16 var_s0_18;
    s16 var_s0_19;
    s16 var_s0_20;
    s16 var_s0_21;
    s16 var_s0_22;
    s16 var_s0_23;
    s16 var_s0_2;
    s16 var_s0_3;
    s16 var_s0_4;
    s16 var_s0_5;
    s16 var_s0_6;
    s16 var_s0_7;
    s16 var_s0_8;
    s16 var_s0_9;
    s16 var_s3;
    s16 var_s4;
    s16 var_s5;
    s16 var_s5_2;
    s32 var_t8_2;
    s32 var_t9_2;
    u8 *temp_a1;
    u8 *temp_a1_2;
    u8 *temp_s1;
    u8 *temp_s1_2;
    u8 *temp_t6;
    u8 *temp_t6_2;
    u8 *temp_t6_4;
    u8 *temp_t6_5;
    u8 *temp_t6_6;
    u8 *temp_t6_7;
    u8 *temp_t6_8;
    u8 *temp_t7;
    u8 *temp_t7_2;
    u8 *temp_t7_4;
    u8 *temp_t7_5;
    u8 *temp_t8;
    u8 *temp_t8_2;
    u8 *temp_t8_4;
    u8 *temp_t8_5;
    u8 *temp_t8_7;
    u8 *temp_t9_2;
    u8 *temp_t9_3;
    u8 *temp_t9_5;
    u8 *temp_t9_6;
    u8 *temp_t9_7;
    u8 *temp_t9_8;
    u8 *temp_v1;
    u8 *temp_v1_2;
    u8 *var_t6;
    u8 *var_t6_2;
    u8 *var_t6_3;
    u8 *var_t7;
    u8 *var_t7_2;
    u8 *var_t7_3;
    u8 *var_t7_4;
    u8 *var_t8;
    u8 *var_t8_3;
    u8 *var_t9;
    u8 *var_t9_3;
    u8 *var_t9_4;
    u8 temp_t6_3;
    u8 temp_t7_3;
    u8 temp_t7_6;
    u8 temp_t7_7;
    u8 temp_t8_3;
    u8 temp_t8_6;
    u8 temp_t8_8;
    u8 temp_t8_9;
    u8 temp_t9;
    u8 temp_t9_4;
    u8 temp_v0_2;
    u8 temp_v0_3;
    u8 temp_v0_4;
    u8 temp_v0_6;
    *(MainCopy3 *)sp80 = *(MainCopy3 *)&D_800B2228;
    *(MainCopy3 *)sp7C = *(MainCopy3 *)&D_800B222C;
    *(MainCopy3 *)sp78 = *(MainCopy3 *)&D_800B2230;
    *(MainCopy3 *)sp74 = *(MainCopy3 *)&D_800B2234;
    *(MainCopy3 *)sp70 = *(MainCopy3 *)&D_800B2238;
    if (*(u8 *)arg3 == 0) {
        return 0;
    }
    strcpy(sp84, "（");
    strcpy(sp8C, (u8 *) arg3);
    D_80122D1C = 1;
    temp_v0 = strlen(sp8C);
    var_a0 = 0;
    if (temp_v0 > 0) {
        do {
            temp_v1 = &(sp8C)[var_a0];
            temp_v0_2 = *temp_v1;
            if ((temp_v0_2 == 0x6D) || (temp_v0_2 == 0x4D) || (temp_v0_2 == 0x41) || (temp_v0_2 == 0x57) || (temp_v0_2 == 0x4F) || (temp_v0_2 == 0x46) || (temp_v0_2 == 0x59) || (temp_v0_2 == 0x53) || (temp_v0_2 == 0x50) || (temp_v0_2 == 0x70) || (temp_v0_2 == 0x47) || (temp_v0_2 == 0x20) || (temp_v0_2 == 0x71)) {
                var_a0 -= 1;
            } else if (((temp_v0_2 != sp80) || (*(u8 *)((unsigned char *)temp_v1 + (1)) != sp80[1])) && ((temp_v0_2 != sp7C) || (*(u8 *)((unsigned char *)temp_v1 + (1)) != sp7C[1])) && ((temp_v0_2 != sp78) || (*(u8 *)((unsigned char *)temp_v1 + (1)) != sp78[1])) && ((temp_v0_2 != sp74) || (*(u8 *)((unsigned char *)temp_v1 + (1)) != sp74[1])) && ((temp_v0_2 != sp70) || (*(u8 *)((unsigned char *)temp_v1 + (1)) != sp70[1]))) {
                D_80122D1C = 0;
            }
            var_a0 += 2;
        } while (var_a0 < temp_v0);
    }
    var_s0 = 0;
    sp2E6 = 0;
    var_s3 = 0;
    var_s4 = 0;
    sp2F2 = 0;
loop_32:
    temp_v1_2 = &(sp8C)[sp2F2];
    temp_v0_3 = *temp_v1_2;
    if (temp_v0_3 == 0x6D) {
block_43:
        sp2F2 -= 1;
block_44:
block_45:
        sp2F2 += 2;
        goto loop_32;
    }
    if (temp_v0_3 == 0x4D) {
        goto block_43;
    }
    if (temp_v0_3 == 0x41) {
        goto block_43;
    }
    if (temp_v0_3 == 0x57) {
        goto block_43;
    }
    if (temp_v0_3 == 0x4F) {
        goto block_43;
    }
    if (temp_v0_3 == 0x46) {
        goto block_43;
    }
    if (temp_v0_3 == 0x59) {
        goto block_43;
    }
    if (temp_v0_3 == 0x53) {
        goto block_43;
    }
    if (temp_v0_3 == 0x50) {
        goto block_43;
    }
    if (temp_v0_3 == 0x70) {
        goto block_43;
    }
    if (temp_v0_3 == 0x47) {
        goto block_43;
    }
    if (temp_v0_3 == 0x20) {
block_49:
        *temp_v1_2 = 0;
        if ((u8) arg7 != 0) {
            set_kanji_string((s16) ((s16) arg0 - 0xE), (s16) ((s16) arg1 + sp2E6), (u8) arg2, (u8 *) arg6, arg4);
        }
        temp_a1 = &(sp8C)[var_s0];
        sp64 = (s32) (s16) ((s16) arg1 + sp2E6);
        sp60 = temp_a1;
        strncpy((s32) sp2A4, (s32) temp_a1, 0x40);
        temp_v1_3 = sp2F2 - var_s0;
        var_s5 = temp_v1_3;
        if (temp_v1_3 > 0) {
            do {
                temp_s1 = &(sp2A4)[var_s3];
                temp_v0_4 = *temp_s1;
                var_s0_2 = 0;
                if (temp_v0_4 == 0x50) {
                    var_t7 = D_800CA17C;
                    if (strlen(D_800CA17C) > 0) {
                        do {
                            temp_t6 = &temp_s1[var_s0_2];
                            var_s0_2 += 1;
                            *temp_t6 = *var_t7;
                            var_t7 = &D_800CA17C[var_s0_2];
                        } while (var_s0_2 < strlen(D_800CA17C));
                    }
                    strncpy((s32) &temp_s1[var_s0_2], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_2) + 0x40);
                    var_s5 = (var_s5 + strlen(D_800CA17C)) - 1;
                    var_s3 = (var_s3 + strlen(D_800CA17C)) - 2;
                    var_s4 -= 1;
                } else {
                    var_s0_3 = 0;
                    if (temp_v0_4 == 0x53) {
                        var_t9 = D_800CA23C;
                        if (strlen(D_800CA23C) > 0) {
                            do {
                                temp_t8 = &temp_s1[var_s0_3];
                                var_s0_3 += 1;
                                *temp_t8 = *var_t9;
                                var_t9 = &D_800CA23C[var_s0_3];
                            } while (var_s0_3 < strlen(D_800CA23C));
                        }
                        strncpy((s32) &temp_s1[var_s0_3], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_3) + 0x40);
                        var_s5 = (var_s5 + strlen(D_800CA23C)) - 1;
                        var_s3 = (var_s3 + strlen(D_800CA23C)) - 2;
                        var_s4 -= 1;
                    } else {
                        var_s0_4 = 0;
                        if (temp_v0_4 == 0x47) {
                            if (strlen(girl_name_buf) > 0) {
                                do {
                                    temp_t7 = &temp_s1[var_s0_4];
                                    temp_t9 = girl_name_buf[var_s0_4];
                                    var_s0_4 += 1;
                                    *temp_t7 = temp_t9;
                                } while (var_s0_4 < strlen(girl_name_buf));
                            }
                            strncpy((s32) &temp_s1[var_s0_4], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_4) + 0x40);
                            var_s5 = (var_s5 + strlen(girl_name_buf)) - 1;
                            var_t9_2 = (var_s4 - 1) << 0x10;
                            var_s3 = (var_s3 + strlen(girl_name_buf)) - 2;
                            goto block_96;
                        }
                        var_s0_5 = 0;
                        if (temp_v0_4 == 0x70) {
                            var_t8 = D_800CA16C;
                            if (strlen(D_800CA16C) > 0) {
                                do {
                                    temp_t9_2 = &temp_s1[var_s0_5];
                                    var_s0_5 += 1;
                                    *temp_t9_2 = *var_t8;
                                    var_t8 = &D_800CA16C[var_s0_5];
                                } while (var_s0_5 < strlen(D_800CA16C));
                            }
                            strncpy((s32) &temp_s1[var_s0_5], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_5) + 0x40);
                            var_s5 = (var_s5 + strlen(D_800CA16C)) - 1;
                            var_s3 = (var_s3 + strlen(D_800CA16C)) - 2;
                            var_s4 -= 1;
                        } else if (temp_v0_4 == 0x59) {
                            var_s0_6 = 0;
                            strcpy(sp28C, "好雄");
                            var_t7_2 = sp28C;
                            if (strlen(sp28C) > 0) {
                                do {
                                    temp_t6_2 = &temp_s1[var_s0_6];
                                    var_s0_6 += 1;
                                    *temp_t6_2 = *var_t7_2;
                                    var_t7_2 = &(sp28C)[var_s0_6];
                                } while (var_s0_6 < strlen(sp28C));
                            }
                            strncpy((s32) &temp_s1[var_s0_6], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_6) + 0x40);
                            var_s5 = (var_s5 + strlen(sp28C)) - 1;
                            var_s3 = (var_s3 + strlen(sp28C)) - 2;
                            var_s4 -= 1;
                        } else if (temp_v0_4 == 0x46) {
                            var_s0_7 = 0;
                            sprintf((s32) sp28C, (s32) "%s%s", (s32) D_800CA16C, (s32) D_800CA174);
                            var_t9_3 = sp28C;
                            if (strlen(sp28C) > 0) {
                                do {
                                    temp_t8_2 = &temp_s1[var_s0_7];
                                    var_s0_7 += 1;
                                    *temp_t8_2 = *var_t9_3;
                                    var_t9_3 = &(sp28C)[var_s0_7];
                                } while (var_s0_7 < strlen(sp28C));
                            }
                            strncpy((s32) &temp_s1[var_s0_7], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_7) + 0x40);
                            var_s5 = (var_s5 + strlen(sp28C)) - 1;
                            var_s3 = (var_s3 + strlen(sp28C)) - 2;
                            var_s4 -= 1;
                        } else {
                            if (temp_v0_4 == 0x6D) {
                                var_s0_8 = 0;
                                strcpy(sp28C, D_800CA174);
                                var_t6 = sp28C;
                                if (strlen(sp28C) > 0) {
                                    do {
                                        temp_t7_2 = &temp_s1[var_s0_8];
                                        var_s0_8 += 1;
                                        *temp_t7_2 = *var_t6;
                                        var_t6 = &(sp28C)[var_s0_8];
                                    } while (var_s0_8 < strlen(sp28C));
                                }
                                strncpy((s32) &temp_s1[var_s0_8], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_8) + 0x40);
                                var_s5 = (var_s5 + strlen(sp28C)) - 1;
                                var_t9_2 = (var_s4 - 1) << 0x10;
                                var_s3 = (var_s3 + strlen(sp28C)) - 2;
                                goto block_96;
                            }
                            var_s0_9 = 0;
                            if (temp_v0_4 == 0x4F) {
                                if (strlen(&D_800CA248) > 0) {
                                    do {
                                        temp_t9_3 = &temp_s1[var_s0_9];
                                        temp_t6_3 = (&D_800CA248)[var_s0_9];
                                        var_s0_9 += 1;
                                        *temp_t9_3 = temp_t6_3;
                                    } while (var_s0_9 < strlen(&D_800CA248));
                                }
                                strncpy((s32) &temp_s1[var_s0_9], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_9) + 0x40);
                                var_s5 = (var_s5 + strlen(&D_800CA248)) - 1;
                                var_s3 = (var_s3 + strlen(&D_800CA248)) - 2;
                                var_s4 -= 1;
                            } else {
                                var_s0_10 = 0;
                                if (temp_v0_4 == 0x57) {
                                    if (strlen(D_800CA250) > 0) {
                                        do {
                                            temp_t6_4 = &temp_s1[var_s0_10];
                                            temp_t8_3 = D_800CA250[var_s0_10];
                                            var_s0_10 += 1;
                                            *temp_t6_4 = temp_t8_3;
                                        } while (var_s0_10 < strlen(D_800CA250));
                                    }
                                    strncpy((s32) &temp_s1[var_s0_10], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_10) + 0x40);
                                    var_s5 = (var_s5 + strlen(D_800CA250)) - 1;
                                    var_s3 = (var_s3 + strlen(D_800CA250)) - 2;
                                    var_s4 -= 1;
                                } else {
                                    var_s0_11 = 0;
                                    if (temp_v0_4 == 0x41) {
                                        if (strlen(D_800CA25C) > 0) {
                                            do {
                                                temp_t8_4 = &temp_s1[var_s0_11];
                                                temp_t7_3 = D_800CA25C[var_s0_11];
                                                var_s0_11 += 1;
                                                *temp_t8_4 = temp_t7_3;
                                            } while (var_s0_11 < strlen(D_800CA25C));
                                        }
                                        strncpy((s32) &temp_s1[var_s0_11], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_11) + 0x40);
                                        var_s5 = (var_s5 + strlen(D_800CA25C)) - 1;
                                        var_s3 = (var_s3 + strlen(D_800CA25C)) - 2;
                                        var_s4 -= 1;
                                    } else {
                                        var_s0_12 = 0;
                                        if (temp_v0_4 == 0x4D) {
                                            if (strlen(&D_800CA280) > 0) {
                                                do {
                                                    temp_t7_4 = &temp_s1[var_s0_12];
                                                    temp_t9_4 = (&D_800CA280)[var_s0_12];
                                                    var_s0_12 += 1;
                                                    *temp_t7_4 = temp_t9_4;
                                                } while (var_s0_12 < strlen(&D_800CA280));
                                            }
                                            strncpy((s32) &temp_s1[var_s0_12], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_12) + 0x40);
                                            var_s5 = (var_s5 + strlen(&D_800CA280)) - 1;
                                            var_t9_2 = (var_s4 - 1) << 0x10;
                                            var_s3 = (var_s3 + strlen(&D_800CA280)) - 2;
block_96:
                                            var_s4 = (s16) (var_t9_2 >> 0x10);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                var_s3 += 2;
                var_s4 += 2;
            } while (var_s3 < var_s5);
            var_s3 = 0;
            var_s4 = 0;
        }
        if ((u8) arg7 != 0) {
            strcat(sp2A4, (u8 *) arg5);
        }
        if ((sp2A4[0] == sp84[0]) && (sp2A4[1] == sp84[1])) {
            set_kanji_string((s16) ((s16) arg0 - 0xE), (s16) sp64, (u8) arg2, sp2A4, arg4);
        } else {
            set_kanji_string((s16) arg0, (s16) sp64, (u8) arg2, sp2A4, arg4);
        }
        sp2E6 += 0x10;
        var_s0 = sp2F2 + 1;
        sp2F2 -= 1;
        goto block_44;
    }
    if (temp_v0_3 == 0x71) {
        goto block_49;
    }
    if (temp_v0_3 != 0) {
        goto block_45;
    }
    if ((u8) arg7 != 0) {
        set_kanji_string((s16) ((s16) arg0 - 0xE), (s16) ((s16) arg1 + sp2E6), (u8) arg2, (u8 *) arg6, arg4);
    }
    temp_v0_5 = (s16) arg1 + sp2E6;
    temp_a1_2 = &(sp8C)[var_s0];
    sp64 = (s32) temp_v0_5;
    sp60 = temp_a1_2;
    sp68 = (s32) temp_v0_5;
    strncpy((s32) sp2A4, (s32) temp_a1_2, 0x40);
    temp_v1_4 = sp2F2 - var_s0;
    var_s5_2 = temp_v1_4;
    if (temp_v1_4 > 0) {
        do {
            temp_s1_2 = &(sp2A4)[var_s3];
            temp_v0_6 = *temp_s1_2;
            var_s0_13 = 0;
            if (temp_v0_6 == 0x50) {
                var_t7_3 = D_800CA17C;
                if (strlen(D_800CA17C) > 0) {
                    do {
                        temp_t8_5 = &temp_s1_2[var_s0_13];
                        var_s0_13 += 1;
                        *temp_t8_5 = *var_t7_3;
                        var_t7_3 = &D_800CA17C[var_s0_13];
                    } while (var_s0_13 < strlen(D_800CA17C));
                }
                strncpy((s32) &temp_s1_2[var_s0_13], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_13) + 0x40);
                var_s5_2 = (var_s5_2 + strlen(D_800CA17C)) - 1;
                var_s3 = (var_s3 + strlen(D_800CA17C)) - 2;
                var_s4 -= 1;
            } else {
                var_s0_14 = 0;
                if (temp_v0_6 == 0x53) {
                    var_t6_2 = D_800CA23C;
                    if (strlen(D_800CA23C) > 0) {
                        do {
                            temp_t9_5 = &temp_s1_2[var_s0_14];
                            var_s0_14 += 1;
                            *temp_t9_5 = *var_t6_2;
                            var_t6_2 = &D_800CA23C[var_s0_14];
                        } while (var_s0_14 < strlen(D_800CA23C));
                    }
                    strncpy((s32) &temp_s1_2[var_s0_14], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_14) + 0x40);
                    var_s5_2 = (var_s5_2 + strlen(D_800CA23C)) - 1;
                    var_s3 = (var_s3 + strlen(D_800CA23C)) - 2;
                    var_s4 -= 1;
                } else {
                    var_s0_15 = 0;
                    if (temp_v0_6 == 0x47) {
                        if (strlen(girl_name_buf) > 0) {
                            do {
                                temp_t6_5 = &temp_s1_2[var_s0_15];
                                temp_t8_6 = girl_name_buf[var_s0_15];
                                var_s0_15 += 1;
                                *temp_t6_5 = temp_t8_6;
                            } while (var_s0_15 < strlen(girl_name_buf));
                        }
                        strncpy((s32) &temp_s1_2[var_s0_15], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_15) + 0x40);
                        var_s5_2 = (var_s5_2 + strlen(girl_name_buf)) - 1;
                        var_t8_2 = (var_s4 - 1) << 0x10;
                        var_s3 = (var_s3 + strlen(girl_name_buf)) - 2;
                        goto block_154;
                    }
                    var_s0_16 = 0;
                    if (temp_v0_6 == 0x70) {
                        var_t7_4 = D_800CA16C;
                        if (strlen(D_800CA16C) > 0) {
                            do {
                                temp_t8_7 = &temp_s1_2[var_s0_16];
                                var_s0_16 += 1;
                                *temp_t8_7 = *var_t7_4;
                                var_t7_4 = &D_800CA16C[var_s0_16];
                            } while (var_s0_16 < strlen(D_800CA16C));
                        }
                        strncpy((s32) &temp_s1_2[var_s0_16], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_16) + 0x40);
                        var_s5_2 = (var_s5_2 + strlen(D_800CA16C)) - 1;
                        var_s3 = (var_s3 + strlen(D_800CA16C)) - 2;
                        var_s4 -= 1;
                    } else if (temp_v0_6 == 0x59) {
                        var_s0_17 = 0;
                        strcpy(sp28C, "好雄");
                        var_t6_3 = sp28C;
                        if (strlen(sp28C) > 0) {
                            do {
                                temp_t9_6 = &temp_s1_2[var_s0_17];
                                var_s0_17 += 1;
                                *temp_t9_6 = *var_t6_3;
                                var_t6_3 = &(sp28C)[var_s0_17];
                            } while (var_s0_17 < strlen(sp28C));
                        }
                        strncpy((s32) &temp_s1_2[var_s0_17], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_17) + 0x40);
                        var_s5_2 = (var_s5_2 + strlen(sp28C)) - 1;
                        var_s3 = (var_s3 + strlen(sp28C)) - 2;
                        var_s4 -= 1;
                    } else if (temp_v0_6 == 0x46) {
                        var_s0_18 = 0;
                        sprintf((s32) sp28C, (s32) "%s%s", (s32) D_800CA16C, (s32) D_800CA174);
                        var_t8_3 = sp28C;
                        if (strlen(sp28C) > 0) {
                            do {
                                temp_t7_5 = &temp_s1_2[var_s0_18];
                                var_s0_18 += 1;
                                *temp_t7_5 = *var_t8_3;
                                var_t8_3 = &(sp28C)[var_s0_18];
                            } while (var_s0_18 < strlen(sp28C));
                        }
                        strncpy((s32) &temp_s1_2[var_s0_18], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_18) + 0x40);
                        var_s5_2 = (var_s5_2 + strlen(sp28C)) - 1;
                        var_s3 = (var_s3 + strlen(sp28C)) - 2;
                        var_s4 -= 1;
                    } else {
                        if (temp_v0_6 == 0x6D) {
                            var_s0_19 = 0;
                            strcpy(sp28C, D_800CA174);
                            var_t9_4 = sp28C;
                            if (strlen(sp28C) > 0) {
                                do {
                                    temp_t6_6 = &temp_s1_2[var_s0_19];
                                    var_s0_19 += 1;
                                    *temp_t6_6 = *var_t9_4;
                                    var_t9_4 = &(sp28C)[var_s0_19];
                                } while (var_s0_19 < strlen(sp28C));
                            }
                            strncpy((s32) &temp_s1_2[var_s0_19], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_19) + 0x40);
                            var_s5_2 = (var_s5_2 + strlen(sp28C)) - 1;
                            var_t8_2 = (var_s4 - 1) << 0x10;
                            var_s3 = (var_s3 + strlen(sp28C)) - 2;
                            goto block_154;
                        }
                        var_s0_20 = 0;
                        if (temp_v0_6 == 0x4F) {
                            if (strlen(&D_800CA248) > 0) {
                                do {
                                    temp_t9_7 = &temp_s1_2[var_s0_20];
                                    temp_t7_6 = (&D_800CA248)[var_s0_20];
                                    var_s0_20 += 1;
                                    *temp_t9_7 = temp_t7_6;
                                } while (var_s0_20 < strlen(&D_800CA248));
                            }
                            strncpy((s32) &temp_s1_2[var_s0_20], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_20) + 0x40);
                            var_s5_2 = (var_s5_2 + strlen(&D_800CA248)) - 1;
                            var_s3 = (var_s3 + strlen(&D_800CA248)) - 2;
                            var_s4 -= 1;
                        } else {
                            var_s0_21 = 0;
                            if (temp_v0_6 == 0x57) {
                                if (strlen(D_800CA250) > 0) {
                                    do {
                                        temp_t6_7 = &temp_s1_2[var_s0_21];
                                        temp_t8_8 = D_800CA250[var_s0_21];
                                        var_s0_21 += 1;
                                        *temp_t6_7 = temp_t8_8;
                                    } while (var_s0_21 < strlen(D_800CA250));
                                }
                                strncpy((s32) &temp_s1_2[var_s0_21], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_21) + 0x40);
                                var_s5_2 = (var_s5_2 + strlen(D_800CA250)) - 1;
                                var_t8_2 = (var_s4 - 1) << 0x10;
                                var_s3 = (var_s3 + strlen(D_800CA250)) - 2;
                                goto block_154;
                            }
                            var_s0_22 = 0;
                            if (temp_v0_6 == 0x41) {
                                if (strlen(D_800CA25C) > 0) {
                                    do {
                                        temp_t9_8 = &temp_s1_2[var_s0_22];
                                        temp_t7_7 = D_800CA25C[var_s0_22];
                                        var_s0_22 += 1;
                                        *temp_t9_8 = temp_t7_7;
                                    } while (var_s0_22 < strlen(D_800CA25C));
                                }
                                strncpy((s32) &temp_s1_2[var_s0_22], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_22) + 0x40);
                                var_s5_2 = (var_s5_2 + strlen(D_800CA25C)) - 1;
                                var_s3 = (var_s3 + strlen(D_800CA25C)) - 2;
                                var_s4 -= 1;
                            } else {
                                var_s0_23 = 0;
                                if (temp_v0_6 == 0x4D) {
                                    if (strlen(&D_800CA280) > 0) {
                                        do {
                                            temp_t6_8 = &temp_s1_2[var_s0_23];
                                            temp_t8_9 = (&D_800CA280)[var_s0_23];
                                            var_s0_23 += 1;
                                            *temp_t6_8 = temp_t8_9;
                                        } while (var_s0_23 < strlen(&D_800CA280));
                                    }
                                    strncpy((s32) &temp_s1_2[var_s0_23], (s32) (&sp60[var_s4] + 1), (-var_s3 - var_s0_23) + 0x40);
                                    var_s5_2 = (var_s5_2 + strlen(&D_800CA280)) - 1;
                                    var_t8_2 = (var_s4 - 1) << 0x10;
                                    var_s3 = (var_s3 + strlen(&D_800CA280)) - 2;
block_154:
                                    var_s4 = (s16) (var_t8_2 >> 0x10);
                                }
                            }
                        }
                    }
                }
            }
            var_s3 += 2;
            var_s4 += 2;
        } while (var_s3 < var_s5_2);
    }
    if (*(u8 *)arg5 != 0) {
        strcat(sp2A4, (u8 *) arg5);
    }
    D_800CA2C0 = sp68 + 0x10;
    if ((sp2A4[0] == sp84[0]) && (sp2A4[1] == sp84[1])) {
        return set_kanji_string((s16) ((s16) arg0 - 0xE), (s16) sp64, (u8) arg2, sp2A4, arg4);
    } else {
        return set_kanji_string((s16) arg0, (s16) sp64, (u8) arg2, sp2A4, arg4);
    }
}
s32 func_80082764(arg0, arg1, arg2)
u8 arg0;
u8 arg1;
u8 arg2;
{
    DialogueTables *tables;
    DialogueWork *dialogue;
    s16 group;
    s32 result;
    u8 command;
    tables = (DialogueTables *)&D_800CA134;
    dialogue = (DialogueWork *)D_80122CD0;
    if (dialogue->unk_30 != 0) {
        if ((check_end_k() != 0) && ((dialogue->unk_20 == 0) || ((dialogue->unk_20 == 1) && (func_800460EC() != 4)) || ((*(u8 *)&D_800E6280.unk_03C) == 0))) {
            dialogue->unk_30 = 0;
            D_800CA2AC = 0;
        } else if ((D_800E6280.unk_F88 & 0x400040) && ((*(u8 *)&D_800E6280.unk_03C) == 2)) {
            func_80044750(0xB4);
            dialogue->unk_30 = 2;
            D_800CA2AC = 0;
            D_800CA2C4 = 1;
        }
        if ((check_end_k() != 0) && (dialogue->unk_30 == 2) && (func_800460EC() != 4)) {
            dialogue->unk_30 = 0;
            D_800CA2AC = 0;
        }
    } else if (dialogue->unk_48 != 0) {
        if ((check_end_k() != 0) && ((dialogue->unk_20 == 0) || ((dialogue->unk_20 == 1) && (func_800460EC() != 4)) || ((*(u8 *)&D_800E6280.unk_03C) == 0))) {
            k_reset(1);
            dialogue->unk_48 = 0;
            D_800CA2AC = 0;
        }
    } else if (dialogue->unk_14 != 0) {
        if ((D_800E6280.unk_F88 & 0x400040) && ((*(u8 *)&D_800E6280.unk_03C) == 2)) {
            func_80044750(0xB4);
            D_800CA2C4 = 1;
            dialogue->unk_14 = 2;
            D_800CA2AC = 0;
        }
        if ((check_end_k() != 0) && (dialogue->unk_14 == 2) && (func_800460EC() != 4)) {
            k_reset(1);
            dialogue->unk_14 = 0;
            D_8011ED5B &= 0xFF7F;
            D_800CA2AC = 0;
            dialogue->unk_20 = 0;
            return 1;
        }
        if ((D_800E6280.unk_F88 & 0x200020) && (check_end_k() != 0) && ((dialogue->unk_20 == 0) || (dialogue->unk_20 == 0xA) || ((dialogue->unk_20 == 1) && (func_800460EC() != 4)) || ((*(u8 *)&D_800E6280.unk_03C) == 0))) {
            k_reset(1);
            dialogue->unk_14 = 0;
            D_8011ED5B &= 0xFF7F;
            D_800CA2AC = 0;
            dialogue->unk_20 = 0;
            return 1;
        }
        if ((check_end_k() != 0) && ((dialogue->unk_20 == 0) || (dialogue->unk_20 == 0xA) || ((dialogue->unk_20 == 1) && (func_800460EC() != 4)) || ((*(u8 *)&D_800E6280.unk_03C) == 0))) {
            D_8011ED5B |= 0x80;
            func_8006C934();
        } else {
            D_8011ED5B &= 0x7F;
        }
    } else {
        D_800CA2C8 = 0;
        D_800E6280.unk_1104.w = 0;
        group = *tables->group;
        result = dialogue_speak_part(tables->text[group][*tables->line], (s32) tables->command[group][*tables->line], (s32) arg0, tables->voice, (s32) group, (s32) (*tables->line), (s32) arg2);
        if ((arg1 != 0) && (D_800CA2A8 == 0) && ((result & 0xFF) != 0x64)) {
            *tables->line;
            if (tables->command[*tables->group][*tables->line] == 0xFF) {
                *tables->group = (s16) (*tables->group + 1);
                *tables->line = 0;
            } else {
                *tables->line = (s16) (*tables->line + 1);
            }
        }
    }
    if ((dialogue->unk_30 != 0) || (dialogue->unk_14 != 0) || (dialogue->unk_48 != 0)) {
        if ((check_end_k() == 0) && ((*(u8 *)&D_800E6280.unk_03C) == 0)) {
            command = tables->command[*tables->group][*tables->line - 1];
            if (((command == 1) || (command == 2) || (command == 5) || (command == 0xA) || (command == 0xB) || (command == 0xC) || (command == 0xD)) && (dialogue->unk_2C == 0)) {
                if ((dialogue->unk_24 != 0) && (dialogue->unk_4C == 0) && (dialogue->unk_50 == 0)) {
                    bustup_speech();
                } else if ((dialogue->unk_34 != 0) && (dialogue->unk_40 == 0) && (dialogue->unk_4C == 0)) {
                    func_80067DFC();
                }
            }
        }
    }
    return 0;
}
INCLUDE_ASM("asm/nonmatchings/main/8007C030",make_three_select);
INCLUDE_ASM("asm/nonmatchings/main/8007C030",junban_init);
void normal_date_three_select(void) {
    switch (D_800E6280.unk_1104.u) {
    case 0:
        normal_date_three_select_init();
        return;
    case 1:
        normal_date_three_select_main();
        return;
    default:
        func_80046500();
        return;
    }
}
s32 normal_date_three_select_init(void) {
    s32 pad;
    s16 x[3];
    s16 y[3];
    s16 w[3];
    s16 h[3];
    x[0] = x[1] = x[2] = -0x80;
    y[0] = 0x30;
    y[1] = 0x40;
    y[2] = 0x50;
    w[0] = w[1] = w[2] = 0xFC;
    h[0] = h[1] = h[2] = 0x10;
    menu_set(1, 3, x, y, w, h);
    D_800E6280.unk_1094 = 0;
    D_80122D30 = 1;
    D_800E6280.unk_1104.w++;
}
s32 normal_date_three_select_main(void) {
    menu_check(1, D_8011ECF6, D_8011ECFA);
    func_8004FC10(1);
    menu_bar_show(1);
    if (D_800E6280.unk_F88 & 0x200020) {
        if (check_end_k() != 0) {
            if (D_800E6280.unk_F90[1].unk_06 != 0) {
                D_80122CDC = D_800CA2A4[0];
            } else if (D_800E6280.unk_F90[3].unk_06 != 0) {
                D_80122CDC = D_800CA2A4[1];
            } else if (D_800E6280.unk_F90[5].unk_06 != 0) {
                D_80122CDC = D_800CA2A4[2];
            } else {
                return 0;
            }
            D_8011ECD3 &= 0xFF7F;
            D_80122D30 = 0;
            k_reset(1);
            func_8004284C();
        }
    }
}
void normal_date_two_select(void) {
    switch (D_800E6280.unk_1104.u) {
    case 0:
        normal_date_two_select_init();
        return;
    case 1:
        normal_date_two_select_main();
        return;
    default:
        func_80046500();
        return;
    }
}
void normal_date_two_select_init(void) {
    s32 pad;
    s16 x[2];
    s16 y[2];
    s16 w[2];
    s16 h[2];
    x[0] = x[1] = -0x80;
    y[0] = 0x30;
    y[1] = 0x40;
    w[0] = w[1] = 0xFC;
    h[0] = h[1] = 0x10;
    menu_set(1, 2, x, y, w, h);
    D_800E6280.unk_1094 = 0;
    D_80122D30 = 1;
    D_800E6280.unk_1104.w++;
}
s32 normal_date_two_select_main(void) {
    menu_check(1, D_8011ECF6, D_8011ECFA);
    func_8004FC10(1);
    menu_bar_show(1);
    if (D_800E6280.unk_F88 & 0x200020) {
        if (check_end_k() != 0) {
            if (D_800E6280.unk_F90[1].unk_06 != 0) {
                D_80122CDC = 0;
            } else if (D_800E6280.unk_F90[3].unk_06 != 0) {
                D_80122CDC = 1;
            } else {
                return 0;
            }
            D_8011ECD3 &= 0xFF7F;
            D_80122D30 = 0;
            k_reset(1);
            func_8004284C();
        }
    }
}
void func_80083338(void) {
    func_80083440(get_g_zyotai_h(D_800E6280.unk_F5F) & 0x7F);
    func_8004284C();
}
void func_80083378(void) {
    func_80083440(0);
    func_8004284C();
}
void func_800833A0(void) {
    func_80083440(1);
    func_8004284C();
}
void func_800833C8(void) {
    func_80083440(2);
    func_8004284C();
}
void func_800833F0(void) {
    func_80083440(3);
    func_8004284C();
}
void func_80083418(void) {
    func_80083440(4);
    func_8004284C();
}
void func_80083440(u8 arg0) {
    func_80085CD4(arg0);
    func_800634FC(arg0);
}
s32 func_80083474(void) {
    u8 var_v1;
    if ((((s32 *)D_800CA120)[0] != 0x801820AC || ((s32 *)D_800CA120)[1] != 0x801860AC ||
         ((s32 *)D_800CA120)[2] != 0x8018A0AC || ((s32 *)D_800CA120)[3] != 0x8018E0AC ||
         ((s32 *)D_800CA120)[4] != 0x801920AC || ((s32 *)D_800CA128)[0] != 0x8018005C ||
         ((s32 *)D_800CA128)[1] != 0x80180070 || *(s32 *)D_800CA124 != 0x80180000) &&
        D_800E6280.unk_1108 >= 0x50 && D_800E6280.unk_1108 < 0x70) {
        D_800E6280.unk_110A -= 3;
        func_8004284C();
        return 0;
    }
    if (D_800E6280.unk_1108 >= 0x50 && D_800E6280.unk_1108 < 0x70 && file_checksum_ok(0x2D, 0x80180000) == 0) {
        func_8004284C();
        D_800E6280.unk_110A -= 3;
        return 0;
    }
    var_v1 = get_g_zyotai_h((s32) D_800E6280.unk_F5F) & 0x7F;
    if ((u8) D_800E6280.unk_F5F >= 0xCU) {
        var_v1 = 0;
    }
    func_80062DBC(D_800CA130, D_800CA120, D_800CA124, D_800CA128, var_v1);
    func_8004284C();
}
void func_80083628(void) {
    u8 var_v1;
    var_v1 = get_g_zyotai_h((s32) D_800E6280.unk_F5F) & 0x7F;
    if ((u8) D_800E6280.unk_F5F >= 0xCU) {
        var_v1 = 0;
    }
    func_80062DBC(D_800CA130, D_800CA120, D_800CA124, D_800CA128, var_v1);
    func_8004284C();
}
s32 func_800836A0(void) {
    if ((((s32 *)D_800CA120)[0] != 0x801820AC) || (((s32 *)D_800CA120)[1] != 0x801860AC) || (((s32 *)D_800CA120)[2] != 0x8018A0AC) || (((s32 *)D_800CA120)[3] != 0x8018E0AC) || (((s32 *)D_800CA120)[4] != 0x801920AC)) {
        D_800E6280.unk_110A -= 4;
        return 0;
    }
    if (file_checksum_ok(0x2D, 0x80180000) == 0) {
        func_8004284C();
        D_800E6280.unk_110A -= 5;
        return 0;
    }
    func_80062DBC(D_800CA130, D_800CA120, D_800CA124, D_800CA128, (u8)get_g_zyotai_h((s32) D_800E6280.unk_F5F) & 0x7F);
    D_8011ECD0[31 * 0x44 + 7] = 0;
    *(s32 *)&D_8011ECD0[31 * 0x44 + 0x38] |= 0x81000000;
    *(s32 *)&D_8011ECD0[30 * 0x44 + 0x38] |= 0x81000000;
    D_801217D0->unk_00 |= 0x81000000;
    D_801217D0[1].unk_00 |= 0x81000000;
    func_8004284C();
}
s32 func_80083808(void) {
    if (D_80122CF4 != 0) {
        D_8011ECD0[0x83F] |= 0x80;
        D_8011ECD0[0x7FB] |= 0x80;
        D_801217D0[0].unk_00 &= 0x7FFFFFFF;
        D_801217D0[1].unk_00 &= 0x7FFFFFFF;
        func_80063668();
        if (D_80122D20 != 0) {
            D_801217D0[0].unk_0F = 0;
        }
        if (D_800E6280.unk_10F8 % 240 == 0) {
            bustup_wink();
        }
    } else if (D_80122D04 != 0) {
        D_8011ECD0[0x83F] &= ~0x80;
        D_8011ECD0[0x7FB] &= ~0x80;
        D_801217D0[0].unk_00 |= 0x80000000;
        D_801217D0[1].unk_00 |= 0x80000000;
        if (D_80122D10 == 1) {
            if (D_800E6280.unk_10F8 % 240 == 0) {
                func_80067DFC();
            }
        } else if (D_80122D10 == 0) {
            func_80067C28();
        }
        if (D_800E6280.unk_10F8 % 240 == 0 && D_80122D0C == 0) {
            func_80067DD4();
        }
    } else {
        D_8011ECD0[0x83F] &= ~0x80;
        D_8011ECD0[0x7FB] &= ~0x80;
        D_801217D0[0].unk_00 |= 0x80000000;
        D_801217D0[1].unk_00 |= 0x80000000;
    }
}
void func_80083A10(void) {
    if (D_80122CF8 != 0) {
        func_800578F4(1);
    }
    if (D_80122D2C == 0 && D_80122D04 == 0 && (D_800E6280.unk_F80 & 0xF000F000)) {
        D_8011ECD0[3] |= 0x80;
    }
    if (D_80122D04 != 0 || D_80122D2C != 0) {
        D_8011ECD0[3] &= 0x7F;
    }
    if (D_80122D30 == 1) {
        D_8011ECD0[3] |= 0x80;
    }
    if (D_8011ECD0[3] & 0x80) {
        func_8006BA40();
    }
}
void hizuke_hide(void) {
    hizuke_disp_switch(0);
    func_8004284C();
}
void hizuke_appear(void) {
    hizuke_disp_switch(1);
    func_8004284C();
}
void read_bustup(void) {
    switch (D_800E6280.unk_F5F) {
    case 0:
        if (D_800E6280.unk_03F >= 3U && D_800E6280.unk_03F < 6U) {
            func_80062CD0(0x55B2);
        } else if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 9U) {
            func_80062CD0(0x55DF);
        } else if (D_800E6280.unk_03F >= 9U && D_800E6280.unk_03F < 12U) {
            func_80062CD0(0x560C);
        } else {
            func_80062CD0(0x5639);
        }
        break;
    case 1:
        if (D_800E6280.unk_03F >= 3U && D_800E6280.unk_03F < 6U) {
            func_80062CD0(0x5D9B);
        } else if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 9U) {
            func_80062CD0(0x5DC8);
        } else if (D_800E6280.unk_03F >= 9U && D_800E6280.unk_03F < 12U) {
            func_80062CD0(0x5DF5);
        } else {
            func_80062CD0(0x5E22);
        }
        break;
    case 2:
        if (D_800E6280.unk_03F >= 3U && D_800E6280.unk_03F < 6U) {
            func_80062CD0(0x5288);
        } else if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 9U) {
            func_80062CD0(0x52B5);
        } else if (D_800E6280.unk_03F >= 9U && D_800E6280.unk_03F < 12U) {
            func_80062CD0(0x52E2);
        } else {
            func_80062CD0(0x530F);
        }
        break;
    case 3:
        if (D_800E6280.unk_03F >= 3U && D_800E6280.unk_03F < 6U) {
            func_80062CD0(0x5909);
        } else if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 9U) {
            func_80062CD0(0x5936);
        } else if (D_800E6280.unk_03F >= 9U && D_800E6280.unk_03F < 12U) {
            func_80062CD0(0x5963);
        } else {
            func_80062CD0(0x5990);
        }
        break;
    case 4:
        if (D_800E6280.unk_03F >= 3U && D_800E6280.unk_03F < 6U) {
            func_80062CD0(0x6719);
        } else if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 9U) {
            func_80062CD0(0x6746);
        } else if (D_800E6280.unk_03F >= 9U && D_800E6280.unk_03F < 12U) {
            func_80062CD0(0x6773);
        } else {
            func_80062CD0(0x67A0);
        }
        break;
    case 5:
        if (D_800E6280.unk_03F >= 3U && D_800E6280.unk_03F < 6U) {
            func_80062CD0(0x6011);
        } else if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 9U) {
            func_80062CD0(0x603E);
        } else if (D_800E6280.unk_03F >= 9U && D_800E6280.unk_03F < 12U) {
            func_80062CD0(0x606B);
        } else {
            func_80062CD0(0x6098);
        }
        break;
    case 6:
        if (D_800E6280.unk_03F >= 3U && D_800E6280.unk_03F < 6U) {
            func_80062CD0(0x6287);
        } else if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 9U) {
            func_80062CD0(0x62B4);
        } else if (D_800E6280.unk_03F >= 9U && D_800E6280.unk_03F < 12U) {
            func_80062CD0(0x62E1);
        } else {
            func_80062CD0(0x630E);
        }
        break;
    case 7:
        if (D_800E6280.unk_03F >= 3U && D_800E6280.unk_03F < 6U) {
            func_80062CD0(0x5B52);
        } else if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 9U) {
            func_80062CD0(0x5B7F);
        } else if (D_800E6280.unk_03F >= 9U && D_800E6280.unk_03F < 12U) {
            func_80062CD0(0x5BAC);
        } else {
            func_80062CD0(0x5BD9);
        }
        break;
    case 8:
        if (D_800E6280.unk_03F >= 3U && D_800E6280.unk_03F < 6U) {
            func_80062CD0(0x503F);
        } else if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 9U) {
            func_80062CD0(0x506C);
        } else if (D_800E6280.unk_03F >= 9U && D_800E6280.unk_03F < 12U) {
            func_80062CD0(0x5099);
        } else {
            func_80062CD0(0x50C6);
        }
        break;
    case 9:
        if (D_800E6280.unk_03F >= 3U && D_800E6280.unk_03F < 6U) {
            func_80062CD0(0x64FD);
        } else if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 9U) {
            func_80062CD0(0x652A);
        } else if (D_800E6280.unk_03F >= 9U && D_800E6280.unk_03F < 12U) {
            func_80062CD0(0x6557);
        } else {
            func_80062CD0(0x6584);
        }
        break;
    case 10:
        if (D_800E6280.unk_03F >= 3U && D_800E6280.unk_03F < 6U) {
            func_80062CD0(0x698F);
        } else if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 9U) {
            func_80062CD0(0x69BC);
        } else if (D_800E6280.unk_03F >= 9U && D_800E6280.unk_03F < 12U) {
            func_80062CD0(0x69E9);
        } else {
            func_80062CD0(0x6A16);
        }
        break;
    case 13:
        func_80062CD0(0x6C8C);
        break;
    }
    func_8004284C();
}
void read_bustup_uniform(void) {
    switch (D_800E6280.unk_F5F) {
    case 0:
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            func_80062CD0(0x541D);
        } else {
            func_80062CD0(0x544A);
        }
        break;
    case 1:
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            func_80062CD0(0x5D14);
        } else {
            func_80062CD0(0x5D41);
        }
        break;
    case 2:
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            func_80062CD0(0x5201);
        } else {
            func_80062CD0(0x522E);
        }
        break;
    case 3:
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            func_80062CD0(0x5882);
        } else {
            func_80062CD0(0x58AF);
        }
        break;
    case 4:
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            func_80062CD0(0x6692);
        } else {
            func_80062CD0(0x66BF);
        }
        break;
    case 5:
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            func_80062CD0(0x5F8A);
        } else {
            func_80062CD0(0x5FB7);
        }
        break;
    case 6:
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            func_80062CD0(0x6200);
        } else {
            func_80062CD0(0x622D);
        }
        break;
    case 7:
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            func_80062CD0(0x5AF8);
        } else {
            func_80062CD0(0x5B25);
        }
        break;
    case 8:
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            func_80062CD0(0x4FE5);
        } else {
            func_80062CD0(0x5012);
        }
        break;
    case 9:
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            func_80062CD0(0x64A3);
        } else {
            func_80062CD0(0x64D0);
        }
        break;
    case 10:
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            func_80062CD0(0x6908);
        } else {
            func_80062CD0(0x6935);
        }
        break;
    case 12:
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            func_80062CD0(0x57FB);
        } else {
            func_80062CD0(0x5828);
        }
        break;
    case 11:
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            func_80062CD0(0x6B51);
        } else {
            func_80062CD0(0x6B7E);
        }
        break;
    case 13:
        if (D_800E6280.unk_03F >= 6U && D_800E6280.unk_03F < 10U) {
            func_80062CD0(0x6C32);
        } else {
            func_80062CD0(0x6C5F);
        }
        break;
    case 14:
        func_80062CD0(0x6B24);
        break;
    }
    func_8004284C();
}
void read_bustup_swimsuit(void) {
    switch (D_800E6280.unk_F5F) {
    case 0:
        func_80062CD0(0x5666);
        break;
    case 1:
        func_80062CD0(0x5E4F);
        break;
    case 2:
        func_80062CD0(0x533C);
        break;
    case 3:
        func_80062CD0(0x59BD);
        break;
    case 4:
        func_80062CD0(0x67CD);
        break;
    case 5:
        func_80062CD0(0x60C5);
        break;
    case 6:
        func_80062CD0(0x633B);
        break;
    case 7:
        func_80062CD0(0x5C06);
        break;
    case 8:
        if (D_800E6280.unk_03E == 0x5F) {
            func_80062CD0(0x50F3);
        } else if (D_800E6280.unk_03E == 0x60) {
            func_80062CD0(0x5120);
        } else {
            func_80062CD0(0x514D);
        }
        break;
    case 9:
        func_80062CD0(0x65B1);
        break;
    case 10:
        func_80062CD0(0x6A43);
        break;
    }
    func_8004284C();
}
void bustup_return(void) {
    read_bustup();
}
void return_step(void) {
    main_state_set(D_800E6280.unk_720);
    func_80042908(D_800E6280.unk_721);
    func_80042940(D_800E6280.unk_722);
}
void k_disp_inc2(void) {
    s32 pad;
    u8 sp2B;
    if (D_80122CFC-- < 1) {
        D_80122CFC = 0;
        sp2B = get_k_speed();
        if (D_800E6280.unk_F80 & 0x600060) {
            k_speed_set(0);
        }
        k_disp_inc();
        k_speed_set(sp2B);
    }
}
void yosi_trans(void) {
    func_8004500C(0, 0x200);
    func_8004284C();
}
void change_yoshio(void) {
    D_800E6280.unk_F5F = 0xD;
    func_8004284C();
}
void change_girl(void) {
    D_800E6280.unk_F5F = D_800E6280.unk_75D;
    func_8004284C();
}
void func_800847B8(u8 arg0) {
    s32 t;
    strcpy(D_800CA16C, D_800E6280.unk_0D4);
    strcpy(D_800CA174, D_800E6280.unk_0DC);
    if (arg0 < 0xE) {
        t = arg0;
        get_p_name(D_800CA17C, t);
        strcpy(D_800CA188, girl_short_name_table[arg0]);
        get_g_name(girl_name_buf, t);
    }
}
void select_girl(void) {
    switch (D_800E6280.unk_1104.u) {
    case 0:
        select_girl_init(0);
        return;
    case 1:
        select_girl_main(0);
        return;
    default:
        func_80046500();
        return;
    }
}
void select_girl2(void) {
    switch (D_800E6280.unk_1104.u) {
    case 0:
        select_girl_init(1);
        return;
    case 1:
        select_girl_main(1);
        return;
    default:
        func_80046500();
        return;
    }
}
void select_girl_init(arg0)
u8 arg0;
{
    s16 i;
    s32 length;
    s32 result;
    u8 label[8];
    for (i = 0; i < 11; i++) {
        D_800CA300[i] = ((i / 3) << 6) - 0x80;
        D_800CA318[i] = ((i % 3) * 16) + 0x30;
        if (D_800CA2B0[i] != 0) {
            strcpy(label, girl_short_name_table[i]);
            result = set_kanji_string(D_800CA300[i], D_800CA318[i], 0, label, 0);
            length = strlen(label);
            D_800CA330[i] = (length / 2) * 14;
        } else {
            D_800CA330[i] = 0;
        }
        D_800CA348[i] = 16;
    }
    D_800CA300[11] = 0x40;
    D_800CA318[11] = 0x50;
    D_800CA330[11] = 0x2A;
    D_800CA348[11] = 0x10;
    if ((u8)arg0 != 0) D_800CA330[11] = 0;
    if ((u8)arg0 == 0) result = set_kanji_string(0x40, 0x50, 0, "やめる", 0);
    k_disp_start(result);
    menu_set(1, (0xC - (u8)arg0) & 0xFF, D_800CA300, D_800CA318, D_800CA330, D_800CA348);
    for (i = 0; i < 11; i++) {
        if (D_800CA330[i] != 0) {
            D_800E6280.unk_1094 = i;
            break;
        }
    }
    D_80122D30 = 1;
    D_800E6280.unk_1104.w++;
}
s32 select_girl_main(s32 arg0) {
    s16 i;
    u32 n;
    menu_check(1, D_8011ECF6, D_8011ECFA);
    func_8004FC10(1);
    menu_bar_show(1);
    if (D_800E6280.unk_F88 & 0x200020) {
        n = 0xC - (u8)arg0;
        for (i = 0; i < n; i++) {
            if (((u8 *)&D_800E6280)[i * 16 + 0xF9E] != 0 && D_800CA330[i] != 0) {
                D_80122CDC = i;
                D_8011ECD3 &= 0xFF7F;
                D_80122D30 = 0;
                k_reset(1);
                func_8004284C();
                return 0;
            }
        }
    }
}
void sprite_brightness(s16 idx, u8 val) {
    u8 *p = (u8 *)D_801217D0 + idx * 36;
    p[0x16] = val;
    p[0x15] = val;
    p[0x14] = val;
}
void day_plus(void) {
    s32 sp28;
    u8 sp2B;
    D_800CA2F0 = 0;
    sp2B = D_8011F113;
    func_80072338();
    hizuke_init();
    if (sp2B & 0x80) {
        hizuke_disp_switch(1);
    } else {
        hizuke_disp_switch(0);
    }
    hizuke_show();
    func_8004284C();
}
void check_para_limit(void) {
    s16 i;
    for (i = 0; i < 11; i++) {
        if (D_800E6280.unk_1BC[i].unk_02 < 0) {
            *(s32 *) &D_800E6280.unk_1BC[i].unk_00 = 0;
        }
        if (D_800E6280.unk_1BC[i].unk_02 >= 0x80) {
            D_800E6280.unk_1BC[i].unk_02 = 0x80;
        }
        if (D_800E6280.unk_1BC[i].unk_06 < 0) {
            *(s32 *) &D_800E6280.unk_1BC[i].unk_04 = 0;
        }
        if (D_800E6280.unk_1BC[i].unk_06 >= 0x80) {
            D_800E6280.unk_1BC[i].unk_06 = 0x80;
        }
        if (D_800E6280.unk_1BC[i].unk_0A < 0) {
            *(s32 *) &D_800E6280.unk_1BC[i].unk_08 = 0;
        }
        if (D_800E6280.unk_1BC[i].unk_0A >= 0xFF) {
            D_800E6280.unk_1BC[i].unk_0A = 0xFF;
        }
    }
    for (i = 0; i < 9; i++) {
        if ((&D_800E6280.unk_0FC)[i].unk_02 < 0) {
            ((s32 *) &D_800E6280.unk_0FC)[i] = 0;
        }
        if ((&D_800E6280.unk_0FC)[i].unk_02 >= 0x3E7) {
            ((s32 *) &D_800E6280.unk_0FC)[i] = 0x3E70000;
        }
    }
}
void func_80084E4C(void) {
    func_8006C848(0);
}
void func_80084E6C(void) {
    D_80122D20 = 0;
    func_8004284C();
}
s32 func_80084E90(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s16 i;
    D_80122D04 = 0;
    D_80122D0C = 0;
    D_80122D10 = 0;
    LoadSquare(0x140, 0x80, 0x40, 0x80, (void *)arg0);
    LoadSquare(0x180, 0x80, 0x40, 0x80, (void *)arg1);
    LoadSquare(0x1C0, 0x80, 0x40, 0x80, (void *)arg2);
    LoadSquare(0x200, 0x80, 0x40, 0x80, (void *)arg3);
    LoadSquare(0x240, 0x80, 0x40, 0x80, (void *)arg4);
    LoadSquare(0x280, 0x80, 0x40, 0x80, (void *)arg5);
    D_800E6280.unk_1228[37] = 1;
    D_800E6280.unk_1228[38] = 1;
    D_800E6280.unk_1228[39] = 1;
    D_800E6280.unk_1228[40] = 1;
    D_800E6280.unk_1228[41] = 1;
    D_800E6280.unk_1228[42] = 1;
    for (i = 0x3A; i < 0x40; i++) {
        D_801217D0[i].unk_0C = i - 0x35;
        D_801217D0[i].unk_00 = 0xC1000000;
        *(u16 *)&D_801217D0[i].unk_08 = 0x80;
        D_801217D0[i].unk_0A = 0x80;
        D_801217D0[i].unk_0E = 0;
        D_801217D0[i].unk_0F = 0x80;
        D_801217D0[i].unk_10 = 0;
        D_801217D0[i].unk_12 = 0x1F1;
        D_801217D0[i].unk_14 = 0;
        D_801217D0[i].unk_15 = 0;
        D_801217D0[i].unk_16 = 0;
        D_801217D0[i].unk_18 = 0;
        D_801217D0[i].unk_1A = 0;
        D_801217D0[i].unk_1C = 0x1000;
        D_801217D0[i].unk_1E = 0x1000;
        D_801217D0[i].unk_20 = 0;
        D_801217D0[i].unk_04 = ((i - 0x3A) % 3) * 128 - 0xA0;
        D_801217D0[i].unk_06 = ((i - 0x3A) / 3) * 128 - 0x78;
        D_800E6280.unk_10A5[i] = 0xB;
    }
    D_801217D0[60].unk_08 = D_801217D0[63].unk_08 = 0x40;
    D_801217D0[61].unk_0A = D_801217D0[62].unk_0A = D_801217D0[63].unk_0A = 0x20;
}
s32 func_800850D4(arg0, arg1, arg2, arg3)
    s32 arg0;
    s32 arg1;
    s32 arg2;
    s16 arg3;
{
    s16 i;
    DisplayActor44 *p;
    if (arg0 == 0) {
        func_80048F64(0x60);
        func_80048F64(0x61);
        return 0;
    }
    for (i = 0; i < 2; i++) {
        func_80048F64(i + 0x60);
        p = &((DisplayActor44 *)D_8011ECD0)[i + 96];
        p->unk_01 = 0xA;
        p->unk_02 = 0;
        p->unk_38 = 0x41000000;
        p->unk_03 = 4;
        p->unk_0C = (u8 *)arg0;
        p->unk_10 = (u8 *)arg1;
        p->unk_34 = (u8 *)arg2;
        p->unk_16 = 0;
        p->unk_18 = 0;
        p->unk_06 = 2;
        p->unk_43 = 0x11;
        p->unk_26 = -0xA0;
        p->unk_2A = -0x78;
        p->unk_07 = 0;
        p->unk_05 = 0;
        p->unk_04 = 0;
        p->unk_14 = arg3;
    }
    ((DisplayActor44 *)D_8011ECD0)[96].unk_02 = 4;
    if (arg3 >= 2) {
        D_800CA360 = 0;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = 2;
    } else {
        ((DisplayActor44 *)D_8011ECD0)[96].unk_03 = 0;
    }
}
void func_80085234(arg0)
    u8 arg0;
{
    s16 i;
    u8 t;
    if (arg0) {
        if (D_8011ECD0[0x1983] != 0) {
            t = D_800B5A60;
            D_800B5A60 = D_800CA360;
            D_800CA360 = t;
            D_8011ECD0[0x1983] |= 0x80;
        }
        if (D_8011ECD0[0x19C7] != 0) {
            D_8011ECD0[0x19C7] |= 0x80;
        }
        for (i = 0x3A; i < 0x40; i++) {
            D_801217D0[i].unk_00 &= 0x7FFFFFFF;
        }
    } else {
        D_800B5A60 = D_800CA360;
        if (D_8011ECD0[0x1983] != 0) {
            D_8011ECD0[0x1983] &= ~0x80;
        }
        if (D_8011ECD0[0x19C7] != 0) {
            D_8011ECD0[0x19C7] &= ~0x80;
        }
        for (i = 0x3A; i < 0x40; i++) {
            D_801217D0[i].unk_00 |= 0x80000000;
        }
    }
}
void func_80085368(void) {
    s16 var_s0;
    normal_date_bg_fadein();
    if (D_800B593C == 4) {
        func_80085234(1);
    }
    D_80122D04 = 1;
    func_80065F34(0);
    D_80120657 = D_800B593C;
    var_s0 = 0x3A;
    D_8012069B = D_800B593C;
    do {
        func_80084C98(var_s0, D_800B593C);
        var_s0 += 1;
    } while (var_s0 < 0x40);
}
void func_800853FC(void) {
    s16 var_s0;
    u8 temp_s0;
    u8 state;
    temp_s0 = D_800E6280.unk_110A;
    normal_date_bg_fadeout();
    state = D_800E6280.unk_110A;
    if (state != temp_s0) {
        func_80085234(0);
        D_80122D04 = 0;
        func_80065F34(1);
    }
    D_80120657 = D_800B593C;
    var_s0 = 0x3A;
    D_8012069B = D_800B593C;
    do {
        func_80084C98(var_s0, D_800B593C);
        var_s0 += 1;
    } while (var_s0 < 0x40);
}
void event_face0(void) {
    D_80120666 = 2;
    D_801206AA = 0;
    func_8004284C();
}
void event_face1(void) {
    D_80120666 = 3;
    D_801206AA = 1;
    func_8004284C();
}
void event_face2(void) {
    D_80120666 = 6;
    D_801206AA = 4;
    func_8004284C();
}
void event_face3(void) {
    D_80120666 = 7;
    D_801206AA = 5;
    func_8004284C();
}
void event_face_kuchi0(void) {
    D_80120666 = 2;
    func_8004284C();
}
void event_face_kuchi1(void) {
    D_80120666 = 3;
    func_8004284C();
}
void event_kuchi_ani_no(void) {
    D_80122D10 = 0;
    D_80120652 = 4;
    func_8004284C();
}
void event_kuchi_ani_yes(void) {
    D_80122D10 = 1;
    D_80120652 = 0;
    func_8004284C();
}
void ev_me_on(void) {
    D_80122D0C = 0;
    D_80120696 = 5;
    func_8004284C();
}
void ev_me_on_continue(void) {
    D_80122D0C = 1;
    D_80120696 = 5;
    func_8004284C();
}
INCLUDE_ASM("asm/nonmatchings/main/8007C030",_sprite_set_box_shade_tarao);
void func_8008585C(void) {
    s16 i;
    for (i = 5; i < 31;) {
        D_80122CD0[i] = 0;
        i++;
    }
    D_800CA2AC = 0;
    func_8006BC28(0);
    D_8011ECD3 &= 0xFF7F;
    k_reset(0);
    func_8006612C((s32) "　");
    func_80065F34(0);
    func_8006764C(0);
}
void don_init2(void) {
    func_80047550();
    func_8004284C();
}
void don_wait(void) {
    if ((u32)D_800E6280.unk_1104.w++ >= 0x81U) {
        k_reset(1);
        func_8004284C();
    }
}
void normal_date_speak_1line(void) {
    D_800CA134 = (u8 *)&D_800CA148;
    D_800CA138 = (u8 *)&D_800CA14C;
    D_800CA13C = D_800CA160;
    D_800CA140 = D_800CA164;
    D_800CA144 = D_800CA168;
    if (func_80082764(D_80122CDC, 1, 0) == 1) {
        func_8004284C();
    }
}
void normal_date_speak_012(void) {
    D_800CA134 = &D_800CA148;
    D_800CA138 = &D_800CA14C;
    D_800CA13C = D_800CA160;
    D_800CA140 = D_800CA164;
    D_800CA144 = D_800CA168;
    func_80082764(0xFF, 1, 0);
}
void normal_date_speak(void) {
    D_800CA134 = &D_800CA148;
    D_800CA138 = &D_800CA14C;
    D_800CA13C = D_800CA160;
    D_800CA140 = D_800CA164;
    D_800CA144 = D_800CA168;
    func_80082764(D_80122CDC, 1, 0);
}
void vram_bustup_clear(void) {
    s16 i;
    func_80048F64(0x1F);
    func_80048F64(0x1E);
    for (i = 5; i < 0xA; i++) {
        D_800E6280.unk_1228[i] = -1;
    }
    func_8004284C();
}
s32 func_80085B3C(arg0, arg1)
    u8 arg0;
    u8 arg1;
{
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_00 = D_800E6280.unk_F5F;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_02 = arg0;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_01 = arg1;
    if (D_80122D48 == 0 || D_80122D48 >= 7) {
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = get_g_zyotai_h(D_800E6280.unk_F5F);
    } else {
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = D_80122D48 - 1;
    }
    D_80122D48 = 0;
    if (D_800E6280.unk_F5F == 0xC) {
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 0;
    }
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_04 = D_800E6280.unk_03E;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_05 = D_800E6280.unk_03F;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_06 = D_800E6280.unk_040;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_07 = D_800E6280.unk_041;
    if (++D_800E6280.unk_F5E >= 0xFF) {
        D_800E6280.unk_F5E = 0xFE;
    }
}
void func_80085CD4(u8 a) {
    switch (a) {
    case 0:
        D_801217D0[0].unk_0C = 7;
        D_801217D0[0].unk_0E = 0x30;
        D_801217D0[1].unk_0C = 7;
        D_801217D0[1].unk_0E = 0;
        break;
    case 1:
        D_801217D0[0].unk_0C = 7;
        D_801217D0[0].unk_0E = 0x70;
        D_801217D0[1].unk_0C = 7;
        D_801217D0[1].unk_0E = 0x40;
        break;
    case 2:
        D_801217D0[0].unk_0C = 8;
        D_801217D0[0].unk_0E = 0x30;
        D_801217D0[1].unk_0C = 8;
        D_801217D0[1].unk_0E = 0;
        break;
    case 3:
        D_801217D0[0].unk_0C = 8;
        D_801217D0[0].unk_0E = 0x70;
        D_801217D0[1].unk_0C = 8;
        D_801217D0[1].unk_0E = 0x40;
        break;
    case 4:
        D_801217D0[0].unk_0C = 9;
        D_801217D0[0].unk_0E = 0x30;
        D_801217D0[1].unk_0C = 9;
        D_801217D0[1].unk_0E = 0;
        break;
    case 5:
        D_801217D0[0].unk_0C = 9;
        D_801217D0[0].unk_0E = 0x70;
        D_801217D0[1].unk_0C = 9;
        D_801217D0[1].unk_0E = 0x40;
        break;
    }
}
