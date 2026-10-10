#include "common.h"
#include "ovl/SHOUGATU.h"

void func_800438DC();
void func_800438F0();
void func_80048390();
void func_800649D4();
void func_80064E84();
void func_8007C740();
void func_80084E4C();
void func_8008585C();
extern s32 D_800E7368;
extern s8 D_800E7322;
extern u8 D_800E62BA;
extern u8 D_800B593C;
extern u8 D_800B5940;

void func_80138D2C(void) {
    func_800438DC(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    func_800438F0(1);
    func_80048390();
    func_8004E58C();
    D_800E7322 = 0;
    D_800E7368 = 1;
    func_8008585C();
    D_800E62BA = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    func_8007C740();
    func_800649D4();
    func_80064E84();
    func_80084E4C();
    D_800E71DF = 3;
    func_800847B8(3);
    func_80136F90();
    D_80144DFC = D_80144D98;
    D_80144E00 = D_80144DBC;
    D_80144E04 = D_80144DE0;
    func_8004284C();
}
