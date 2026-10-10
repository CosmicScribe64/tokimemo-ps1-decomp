#ifndef OVL_GEKO_H
#define OVL_GEKO_H

/* main_api.h overrides (T-3340, tools/sync_protos.py): the views this overlay was matched with. */
#define MAIN_API_OVERRIDE_D_801206DA /* matched as u8 (main_api.h: s8) */
#define MAIN_API_OVERRIDE_func_800847B8 /* implicit declaration, as matched; main_api.h has a prototype with narrow parameters */

#include "common.h"
#include "game.h"

/* Overlay-local externs (T-1300). */
void func_80141BC0();
extern u8 D_80145088;
extern u8 D_80144C58;
s32 func_801322E8(void);
void func_80134F84(void);
void func_80134D10(void);
extern s32 D_80145048;
extern s32 D_8014504C;
extern s32 D_80145050;
extern s32 D_80145054;
extern s32 D_80145058;
extern s32 D_8014505C;
extern s16 D_80145060;
extern s16 D_80145068;
void func_80137894(void);
extern s16 D_80144E8C;
extern s16 D_80144E90;
extern s32 D_80144E80;
extern s32 D_80144E84;
extern s32 D_80144E88;
extern s16 D_80145064;
extern s32 D_80146274;
extern s32 D_80146278;
extern s32 D_8014627C;
extern s16 D_801476D8;
extern s16 D_801476DC;
extern s32 D_801476CC;
extern s32 D_801476D0;
extern s32 D_801476D4;
void func_801429D4();

extern u8 D_801206DA;

extern s32 D_80146780;
extern s32 D_80146784;
extern s32 D_80146788;
extern s32 D_8014678C;
extern s32 D_80146790;
extern s32 D_80146794;
extern s32 D_80146798;

extern s32 D_80147300;
extern s32 D_80147304;
extern s32 D_80147308;
extern s32 D_8014730C;
extern s32 D_80147310;
extern s32 D_80147314;
extern s32 D_80147318;

extern s32 D_80146600;
extern s32 D_80146604;
extern s32 D_80146608;
extern s16 D_8014660C;
extern s32 D_80146610;
extern s32 D_80146614;
extern s32 D_80146618;
extern s32 D_8014661C;
extern s32 D_80146620;
extern s32 D_80146624;
extern s32 D_80146628;

extern s32 D_801469F0;
extern s32 D_801469F4;
extern s32 D_801469F8;
extern s16 D_801469FC;
extern s32 D_80146A00;
extern s32 D_80146A04;
extern s32 D_80146A08;
extern s32 D_80146A0C;
extern s32 D_80146A10;
extern s32 D_80146A14;
extern s32 D_80146A18;

extern s32 D_80146C90;
extern s32 D_80146C94;
extern s32 D_80146C98;
extern s16 D_80146C9C;
extern s32 D_80146CA0;
extern s32 D_80146CA4;
extern s32 D_80146CA8;
extern s32 D_80146CAC;
extern s32 D_80146CB0;
extern s32 D_80146CB4;
extern s32 D_80146CB8;

extern s32 D_80146F90;
extern s32 D_80146F94;
extern s32 D_80146F98;
extern s16 D_80146F9C;
extern s32 D_80146FA0;
extern s32 D_80146FA4;
extern s32 D_80146FA8;
extern s32 D_80146FAC;
extern s32 D_80146FB0;
extern s32 D_80146FB4;
extern s32 D_80146FB8;

extern s32 D_80147050;
extern s32 D_80147054;
extern s32 D_80147058;
extern s16 D_8014705C;
extern s32 D_80147060;
extern s32 D_80147064;
extern s32 D_80147068;
extern s32 D_8014706C;
extern s32 D_80147070;
extern s32 D_80147074;
extern s32 D_80147078;

extern s32 D_8014707C;
extern s32 D_80147080;
extern s32 D_80147084;
extern s16 D_80147088;
extern s32 D_8014708C;
extern s32 D_80147090;
extern s32 D_80147094;
extern s32 D_80147098;
extern s32 D_8014709C;
extern s32 D_801470A0;
extern s32 D_801470A4;

extern s32 D_8014731C;
extern s32 D_80147320;
extern s32 D_80147324;
extern s16 D_80147328;
extern s32 D_8014732C;
extern s32 D_80147330;
extern s32 D_80147334;
extern s32 D_80147338;
extern s32 D_8014733C;
extern s32 D_80147340;
extern s32 D_80147344;

extern s32 D_80147530;
extern s32 D_80147534;
extern s32 D_80147538;
extern s16 D_8014753C;
extern s32 D_80147540;
extern s32 D_80147544;
extern s32 D_80147548;
extern s32 D_8014754C;
extern s32 D_80147550;
extern s32 D_80147554;
extern s32 D_80147558;

extern s32 D_80146380;
extern s32 D_80146384;
extern s32 D_80146388;
extern s16 D_8014638C;
extern s32 D_80146390;
extern s32 D_80146394;
extern s32 D_80146398;
extern s32 D_8014639C;
extern s32 D_801463A0;
extern s32 D_801463A4;
extern s32 D_801463A8;
extern s32 D_801463AC;
extern s32 D_801463B0;

extern s32 D_80147630;
extern s32 D_80147634;
extern s32 D_80147638;
extern s32 D_8014763C;
extern s32 D_80147640;
extern s32 D_80147644;
extern s32 D_80147648;
extern s32 D_8014764C;
extern s32 D_80147650;
extern s32 D_80147654;
extern s32 D_80147658;
extern s32 D_8014765C;
extern s32 D_80147660;
extern s32 D_80147664;
extern s32 D_80147668;
extern s32 D_8014766C;
extern s32 D_80147670;
extern s32 D_80147674;
extern s32 D_80147678;
extern s32 D_8014767C;
extern s32 D_80147680;
extern s32 D_80147684;
extern s32 D_80147688;
extern s32 D_8014768C;
extern s32 D_80147690;
extern s32 D_80147694;
extern s32 D_80147698;
extern s32 D_8014769C;
extern s32 D_801476A0;
extern s32 D_801476A4;
extern s32 D_801476A8;
extern s32 D_801476AC;
extern s32 D_801476B0;
extern s32 D_801476B4;
extern s32 D_801476B8;
extern s32 D_801476BC;
extern s32 D_801476C0;
extern s32 D_801476C4;
extern s32 D_801476C8;

extern s16 D_80144C3C;

extern u8 D_80144C50;

extern s32 D_80144C30;
extern s32 D_80144C34;
extern s32 D_80144C38;
extern u8 D_80144C40;

extern u8 D_8014508C;
extern s8 D_80145070;
extern s32 D_80144FD8;
extern s32 D_8014500C;
extern s32 D_80145040;
extern s8 D_80144C54;
extern s8 D_8014507C;
extern u8 D_80145074;

void func_801394B0(void);
void func_8013C5B0(void);
void func_8013D1E0(void);
void func_8013DC70(void);
void func_8013EF30(void);

extern u8 D_801476E0;

void func_80137560(void);

/* defined in C in one object, called from another (T-0500) */
void func_80138AF8(void);
void func_8013C5EC(void);

void func_80140D80(void);
void func_80140E40(void);

#endif
