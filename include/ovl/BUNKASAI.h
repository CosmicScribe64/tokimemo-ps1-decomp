#ifndef OVL_BUNKASAI_H
#define OVL_BUNKASAI_H

#include "common.h"
#include "game.h"

/* BUNKASAI overlay (load address 0x80132000): externs and types. */

/* T-24030: side-effect-only helpers called by func_80142700. */
void func_80142B98(void);
void func_801433C0(void);

extern u32 D_801604F0; /* three data pointers set by the 0x34-byte setters at the start */
extern u32 D_801604F4;
extern u32 D_801604F8;

extern s32 D_8015D774;

/* T-18700: 33-byte strings by school year (95..97) and D_80122CDC, copied to the stack and
 * passed to strcpy by func_80147F8C (DATE2 func_80137A2C form). */
typedef struct BunkasaiNameTable {
    /* 0x000 */ u8 s[3][3][33];
} BunkasaiNameTable; /* size 0x129 */
extern BunkasaiNameTable D_801606DC;
void func_80147F8C(void);

/* T-18700: text-pointer tables copied to the stack by func_8015AF90 (BUNKAKEN func_80152680 form). */
typedef struct BunkasaiTexts12 {
    /* 0x00 */ u8 *text[12];
} BunkasaiTexts12; /* size 0x30 */
typedef struct BunkasaiTexts11 {
    /* 0x00 */ u8 *text[11];
} BunkasaiTexts11; /* size 0x2C */
typedef struct BunkasaiTexts4 {
    /* 0x00 */ u8 *text[4];
} BunkasaiTexts4; /* size 0x10 */
extern BunkasaiTexts12 D_80160AE0;
extern BunkasaiTexts11 D_80160B10;
extern BunkasaiTexts4 D_80160B3C;
extern u8 D_80160B4C; /* frame counter of func_8015AF90 */
s32 func_8015B828(void);

/* T-18700: per-girl byte tables copied to the stack. func_80142700 reads one byte per girl
 * (0..10); func_80142BE8 picks one 13-by-6 table per school year and a column through the
 * six-byte selector. */
typedef struct BunkasaiGirlBytes {
    /* 0x00 */ u8 v[11];
} BunkasaiGirlBytes; /* size 0x0B */
typedef struct BunkasaiColumns {
    /* 0x00 */ u8 v[6];
} BunkasaiColumns; /* size 0x06 */
typedef struct BunkasaiGirlColumns {
    /* 0x00 */ u8 v[13][6];
} BunkasaiGirlColumns; /* size 0x4E */
extern BunkasaiGirlBytes D_80160520;
extern BunkasaiColumns D_8016052C;
extern BunkasaiGirlColumns D_80160534; /* first year */
extern BunkasaiGirlColumns D_80160584; /* second year */
extern BunkasaiGirlColumns D_801605D4; /* third year */

/* T-18700: eleven words (all 0x80) copied to the stack by func_801509A4, which fills it with the
 * indices of the girls whose Rec38 flag bit 1 is set and picks one at random. */
typedef struct BunkasaiGirlList {
    /* 0x00 */ s32 v[11];
} BunkasaiGirlList; /* size 0x2C */
extern BunkasaiGirlList D_80160AA4;

/* defined in C in one object, called from another (T-0500) */
void func_80132000(void);
void func_80132034(void);
void func_80132068(void);
void func_8013209C(void);
void func_801320D0(void);
void func_80132104(void);
void func_80132138(void);
void func_8013216C(void);
void func_801321A0(void);
void func_801321D4(void);
void func_80132208(void);
void func_8013223C(void);
void func_80132270(void);
void func_801322A4(void);
void func_801322D8(void);
void func_8013230C(void);
void func_80132340(void);
void func_80132374(void);
extern s32 D_80160640;
extern s32 D_80160644;
extern s32 D_80160648;
extern s32 D_8016064C;
extern s32 D_80160650;
extern s32 D_80160654;
extern s32 D_80160658;
extern s32 D_8016065C;
extern s32 D_80160660;
extern s32 D_80160664;
extern s32 D_80160668;
extern s32 D_8016066C;
extern s32 D_80160670;
extern s32 D_80160674;
extern s32 D_80160678;
extern s32 D_8016067C;
extern s32 D_80160680;
extern s32 D_80160684;
extern s32 D_80160688;
extern s32 D_8016068C;
extern s32 D_80160690;
extern s32 D_80160694;
extern s32 D_80160698;
extern s32 D_8016069C;
extern s32 D_801606A0;
extern s32 D_801606A4;
extern s32 D_801606A8;
extern s32 D_801606AC;
extern s32 D_801606B0;
extern s32 D_801606B4;
extern s32 D_801606B8;
extern s32 D_801606BC;
extern s32 D_801606C0;
extern s32 D_801606C4;
extern s32 D_801606C8;
extern s32 D_801606CC;
extern s32 D_801606D0;
extern s32 D_801606D4;
extern s32 D_801606D8;


/* T-11190: declarations added by tools/bulk_m2c.py. */
void func_80136230(void);

/* T-11190: declarations added by tools/bulk_m2c.py. */
void func_80146F38(void);

/* T-11190: declarations added by tools/bulk_m2c.py. */
void func_8013C3C4(void);

#endif

#ifndef OVL_BUNKASAI_MODE_T9740
#define OVL_BUNKASAI_MODE_T9740
void func_8013B190(void);
#endif

#ifndef BUNKASAI_TABLE40_T9940
#define BUNKASAI_TABLE40_T9940
typedef struct BunkasaiTable40 {
    /* 0x00 */ s32 unk_00[4][4];
} BunkasaiTable40; /* size 0x40; copied by func_8015B828 */
extern BunkasaiTable40 D_80160B50;
#endif


#ifndef OVL_BUNKASAI_T9960
#define OVL_BUNKASAI_T9960

void func_8013B120(void);
void func_801508C4(void);
#endif


#ifndef OVL_BUNKASAI_T10040
#define OVL_BUNKASAI_T10040
/* Byte timer: lbu/sb in func_801527A8, T-10040. */
extern u8 D_80160AD0;

/* Forward declarations for the original object, T-10040. */
void func_801527A8(void);
void func_80153FA4(void);
void func_801541D8(void);
void func_80154D64(void);
void func_80155388(void);
void func_80155B1C(void);
void func_80156284(void);
void func_801569BC(void);
void func_8015706C(void);
void func_801580EC(void);
void func_80158764(void);
void func_80158DD8(void);
void func_80158E3C(void);
void func_80159FBC(void);
void func_8015AF20(void);

/* Forward calls for guarded dispatchers, T-10040. */
void func_80157DD4(void);
void func_80159344(void);
void func_80159698(void);
void func_80159AD4(void);
void func_8015A5DC(void);
void func_8015A870(void);
void func_8015AB58(void);

void func_801511E0(void);

void func_80151F10(void);

void func_80154018(void);

void func_801548C8(void);

void func_80158EB0(void);

void func_80158FD4(void);

void func_80151364(void);
#endif

/* T-10630: shared dialogue continuation in the 8013C430 object. */
void func_8013FCBC(void);
void func_8013FC58(void);
void func_8013FD20(void);


/* T-10630: scene helper has no return value; callers use its side effects. */
void func_801431D4(void);


#ifndef OVL_BUNKASAI_T10830_H
#define OVL_BUNKASAI_T10830_H
/* Side-effect-only scene table setters defined later in their objects. */
void func_80135448(void);
void func_80142694(void);
void func_80145FC0(void);
void func_8014DD28(void);
#endif
#ifndef OVL_BUNKASAI_T10860_H
#define OVL_BUNKASAI_T10860_H
void func_80149404(void);
void func_80137788(void);
void func_8014AB54(void);
#endif

/* T-10860: forward call into the owned 801362A0 object. */
void func_80138A00(void);
