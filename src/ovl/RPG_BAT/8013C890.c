#define RPG_BAT_OVERRIDE_func_8013C890 /* defined s32 without a return: v0 live at the exit keeps the lw ra delay slot (T-17113); callers see void */
#include "common.h"
#include "ovl/RPG_BAT.h"

s32 func_8013C890(void) {
    if (func_80144640() < D_8015EDC8 && !(D_8015ED98 & 0xFF) && !(D_8015EDC4 & 0xFF) && !(D_8015EE1C & 0xFF) &&
        !(D_8015EDF0 & 0xFF)) {
        D_8015EBDC = func_8013EA00(0x14, 0x28);
        D_8015EDC8 = 0;
        D_8015EDC4 |= func_80144B7C();
        if (!(D_8015EDC4 & 0xFF)) {
            D_8015EDC8 = func_8013EA00(0x3C, 0x5A);
        }
    }
    if (D_8015EDC4 & 0xFF) {
        switch (D_8015EDC4 & 0xFFFF0000) {
        case 0x10000:
            switch (D_8015EDC4 & 0xFFFF) {
            case 1:
                func_80152080();
                break;
            case 2:
                func_80145CC0();
                break;
            }
            break;
        case 0x20000:
            switch (D_8015EDC4 & 0xFFFF) {
            case 1:
                func_80152080();
                break;
            case 2:
                func_80146C30();
                break;
            }
            break;
        case 0x40000:
            switch (D_8015EDC4 & 0xFFFF) {
            case 1:
                func_80152080();
                break;
            case 2:
                func_80133108();
                break;
            }
            break;
        case 0x100000:
            switch (D_8015EDC4 & 0xFFFF) {
            case 1:
                func_80138120();
                break;
            case 2:
                func_801387F0();
                break;
            case 3:
                func_80138170();
                break;
            case 4:
                func_80138324();
                break;
            case 5:
                func_80138B9C();
                break;
            case 6:
                func_80138F6C();
                break;
            case 7:
                func_80139460();
                break;
            }
            break;
        case 0x200000:
        case 0x400000:
            switch (D_8015EDC4 & 0xFFFF) {
            case 1:
                func_80158A0C();
                break;
            case 2:
                func_80158C4C();
                break;
            case 3:
                func_8015906C();
                break;
            case 4:
                func_80159214();
                break;
            case 5:
                func_80159620();
                break;
            }
            break;
        case 0x8000000:
            switch (D_8015EDC4 & 0xFFFF) {
            case 1:
                func_8013FD8C();
                break;
            case 2:
                func_80140CC8();
                break;
            }
            break;
        case 0x80000000:
            switch (D_8015EDC4 & 0xFFFF) {
            case 1:
                func_80156D78();
                break;
            case 2:
                func_80157158();
                break;
            case 3:
                func_80157640();
                break;
            case 4:
                func_80156CC0();
                break;
            }
            break;
        }
    }
}
