#ifndef OVL_TEL_H
#define OVL_TEL_H

#include "common.h"
#include "game.h"

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

#endif /* OVL_TEL_H */
void func_80132138(void);
void func_80134DA0(void);

#ifndef OVL_TEL_W9_2_TABLES_H
#define OVL_TEL_W9_2_TABLES_H
/* Signed halfword changes indexed by the status nibble. T-9720. */
extern s16 D_801444A0[];
#endif

#ifndef OVL_TEL_W9_2_BYTE_TABLES_H
#define OVL_TEL_W9_2_BYTE_TABLES_H
/* Per-girl byte penalties read by func_8013BE38, eleven entries, T-9720. */
extern u8 D_801444B8[11];
extern u8 D_801444C4[11];
#endif


#ifndef OVL_TEL_T10040
#define OVL_TEL_T10040
/* Dialogue pointer tables and packed word streams, T-10040. */
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

/* Forward declarations for the original object, T-10040. */
void func_8013226C(void);
void func_80132FC4(void);
void func_80133D54(void);
void func_801345C4(void);
void func_801352F0(void);
void func_80136724(void);


/* Dialogue text pointers and XA tables, T-10040. */
extern u8 *D_801410BC[];
extern u32 D_801410D4[];
extern u32 D_80141104[];
extern u8 *D_801410C8;
extern u8 *D_801410CC;

/* Forward calls within 80132000.c, T-10040. */
void func_8013586C(void);
void func_80135A44(void);
void func_80135C4C(void);
void func_80135E28(void);

/* Sixteen day/month entries, 0x40-byte tables at +0x0564/+0x05A4. */
extern s32 D_80140564[16];
extern s32 D_801405A4[16];
s32 func_8013655C(void);

/* Dialogue text pointers and XA tables, T-10040. */
extern u32 D_8014109C[];
extern u32 D_801410AC[];
extern u8 *D_80141094;
extern u8 *D_80141098;

/* Dialogue text pointers and XA tables, T-10040. */
extern u32 D_8014104C[];
extern u32 D_80141064[];
extern u8 *D_80141040;
extern u8 *D_80141044;
extern u8 *D_80141048;

/* T-10040: dialogue data and object-local forward declarations. */
extern u32 D_80144210[];
extern u32 D_80144240[];
extern u8 *D_801441F8;
extern u8 *D_801441FC;
extern u8 *D_80144200;
extern u8 *D_80144208;
extern u8 *D_8014420C;
extern u8 *D_80144204;
void func_801384F0(void);

/* T-10040: dialogue data and object-local forward declarations. */
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

/* T-10040: dialogue data and object-local forward declarations. */
extern u32 D_80144350[];
extern u32 D_80144380[];
extern u8 *D_80144338;
extern u8 *D_8014433C;
extern u8 *D_80144340;
extern u8 *D_80144348;
extern u8 *D_8014434C;
extern u8 *D_80144344;
void func_80138D20(void);

/* T-10040: dialogue data and object-local forward declarations. */
extern u32 D_801443C8[];
extern u32 D_801443F8[];
extern u8 *D_801443B0;
extern u8 *D_801443B4;
extern u8 *D_801443B8;
extern u8 *D_801443C0;
extern u8 *D_801443C4;
extern u8 *D_801443BC;
void func_80139064(void);

/* T-10040: dialogue data and object-local forward declarations. */
extern u32 D_80144440[];
extern u32 D_80144470[];
extern u8 *D_80144428;
extern u8 *D_8014442C;
extern u8 *D_80144430;
extern u8 *D_80144438;
extern u8 *D_8014443C;
extern u8 *D_80144434;
void func_801393A8(void);

/* T-10040: dialogue and XA data used by func_8013586C. */
extern u32 D_80141084[];
extern u8 *D_8014107C;
extern u8 *D_80141080;
void func_8013639C(void);

/* T-10040: dialogue tables for the record-counter state. */
extern u32 D_801441A8[];
extern u32 D_801441D0[];
extern u8 *D_80144194;
extern u8 *D_80144198;
extern u8 *D_8014419C;
extern u8 *D_801441A0;
extern u8 *D_801441A4;
void func_801380D8(void);
#endif

#ifndef OVL_TEL_T10630_H
#define OVL_TEL_T10630_H
/* Four-byte calendar entry passed by value to func_8006B5BC. T-10630. */
typedef union TelDateEntry {
    /* 0x00 */ GsWord word;
    /* 0x00 */ struct {
        u32 place : 5;  /* bits 0..4 */
        u32 year : 7;   /* bits 5..11 */
        u32 month : 4;  /* bits 12..15 */
        u32 day : 5;    /* bits 16..20 */
        u32 unk_21 : 3;
        u32 unk_24 : 1;
        u32 girl : 4;   /* bits 25..28 */
        u32 active : 1; /* bit 29 */
        u32 unk_30 : 2;
    } bits;
} TelDateEntry; /* size 0x04 */
extern s32 D_80140550;
extern s32 D_80140560;
/* One 21-place byte row per girl. */
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
#endif

void func_8013D88C();

#ifndef OVL_TEL_T10860_H
#define OVL_TEL_T10860_H
/* Dialogue pointer and XA command tables, T-10860. */
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
#endif

#ifndef OVL_TEL_T10860_ADC_H
#define OVL_TEL_T10860_ADC_H
s32 func_8013ADC0(void);
s32 func_8013BDDC(void);
extern u8 *D_80144FB8;
extern u8 *D_80144FBC;
extern u8 *D_80144FC0;
extern u8 *D_80144FC4;
extern u32 D_80144FC8[];
extern u32 D_80144FE8[];
#endif

#ifndef OVL_TEL_T10860_REPLIES_H
#define OVL_TEL_T10860_REPLIES_H
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
#endif

#ifndef OVL_TEL_T10860_GREETING_TABLES_H
#define OVL_TEL_T10860_GREETING_TABLES_H
/* Text pointer loads and word-indexed XA tables in 80139AA4/80139FEC. */
extern u8 *D_8014544C;
extern u8 *D_80145450;
extern u8 *D_80144D6C[12];
extern u32 D_80144D9C[];
extern u32 D_80144DEC[];
#endif

#ifndef OVL_TEL_T10860_GREETING_TABLES2_H
#define OVL_TEL_T10860_GREETING_TABLES2_H
extern u8 *D_80144738[12];
extern u8 *D_80144AA4[12];
extern u32 D_80144768[];
extern u32 D_801447B8[];
extern u32 D_80144AD4[];
extern u32 D_80144B24[];
#endif

#ifndef OVL_TEL_T10860_REPLY_TABLES_13
#define OVL_TEL_T10860_REPLY_TABLES_13
/* T-10860: 13 pointer entries each, bounded by the following data symbol. */
extern u8 **D_80145C44[13];
extern u32 *D_80145C78[13];
extern u32 *D_80145CAC[13];
#endif

#ifndef OVL_TEL_T10860_A4FC_VIEW
#define OVL_TEL_T10860_A4FC_VIEW
/* T-10860: preceding dispatcher used the legacy integer return view. */
s32 func_8013A4FC(void);
#endif

#ifndef OVL_TEL_T10860_DISPATCH_DECLS
#define OVL_TEL_T10860_DISPATCH_DECLS
void func_80139AA4(void);
void func_80139FEC(void);
#endif

#ifndef OVL_TEL_RECORD_FLAGS_H
#define OVL_TEL_RECORD_FLAGS_H
/* T-10860: bits cleared in a Rec38 +0x0C flag word by func_8013BE38. */
typedef struct TelRecordFlags {
    /* 0x00, bits 0..11 */ u32 low : 12;
    /* 0x00, bit 12 */ u32 b12 : 1;
    /* 0x00, bit 13 */ u32 b13 : 1;
    /* 0x00, bits 14..15 */ u32 middle : 2;
    /* 0x00, bit 16 */ u32 b16 : 1;
    /* 0x00, bits 17..31 */ u32 high : 15;
} TelRecordFlags; /* size 0x04 */
#endif

#ifndef OVL_TEL_T24045_SIGN_DAYS
#define OVL_TEL_T24045_SIGN_DAYS
/* Star-sign boundaries copied as sixteen words by func_8013D03C. T-24045. */
typedef struct TelSignDays {
    /* 0x00 */ u32 w[16];
} TelSignDays; /* size 0x40 */
extern TelSignDays D_80146FD0;
#endif
