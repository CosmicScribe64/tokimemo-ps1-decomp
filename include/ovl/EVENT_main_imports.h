#ifndef OVL_EVENT_MAIN_IMPORTS_H
#define OVL_EVENT_MAIN_IMPORTS_H

#include "common.h"

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

void func_80045B14(void);

s32 func_80045FF4(void);

void func_800469F4();

void func_80046A7C();

void func_80046B88();

s32 func_80049B20();

void func_8004A2C4();

void func_8004A4D8();

void func_8004A734(void);

void func_8004A764();

void func_8004A7C4();

void func_8004A7F4();

#ifndef MAIN_API_OVERRIDE_func_8004A8EC
void func_8004A8EC();
#endif

void func_8004ACA8();

void func_8004AEB0();

void func_8004BB54();

s32 func_8004BC20(s32 arg0);

void func_8004C060();

void func_8004C250();

void func_8004C360();

void func_8004C46C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

#ifndef MAIN_API_OVERRIDE_func_8004C6B0
void func_8004C6B0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
#endif

void func_8004C808();

void func_8004C90C();

void func_8004C984();

void func_8004CA18();

void func_8004CA48();

void func_8004CDDC();

void func_8004CF30();

void func_8004A9CC();

#ifndef MAIN_API_OVERRIDE_func_8004AFB8
void func_8004AFB8();
#endif

void func_800651F8();

void func_80065234();

void func_80046600(void);

void func_800464E8(void);

/* Legacy EVENT renderer call-site views, T-24017. The packet pointers are
 * passed in a0/a1; return values are unused. Keep the unknown legacy names. */
void func_800674F4();

void func_800675A8();

void func_80067678();

void func_80067628();

#endif
