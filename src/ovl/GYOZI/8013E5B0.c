#include "common.h"
#include "ovl/GYOZI.h"

typedef struct {
    void (*f[17])();
} FnTbl17; /* size 0x44 */
extern FnTbl17 D_80148180;

void func_8013E5B0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl17 tbl;

    tbl = D_80148180;
    idx = D_800F647A;
    tbl.f[idx](0x80);
}

/* FAKE: empty counter test steers allocation; the opaque call keeps only
 * the base argument to avoid a redundant stride reload. T-10850.
 * FAKE: the loop init, the empty test and the `do` share one source line (as1 schedules by line;
 * autotweak group, T-20830). T-21520 */
void func_8013E638(void)
{
    s16 i;
    i = 0; if (i && i) { } do
    {
        if ((D_800F53A0.girl[i].unk_10[1] & 1) && (D_800F53A0.unk_F2F != i))
        {
            D_800F53A0.girl[i].unk_0A += 1;
        }
        i += 1;
    }
    while (i < 0xB);
    D_800F53A0.girl[D_800F53A0.unk_F2F].unk_06 += 8;
    D_800F53A0.girl[D_800F53A0.unk_F2F].unk_0A += 8;
    func_8008FB00(&D_800F53A0);
    func_80050D60(0, 0);
    func_80081D30(1);
}

void func_8013E724(void) {
    s16 i;

    D_801480FC = D_8014808C;
    D_80148100 = D_801480C0;
    D_80148104 = D_801480F4;
    D_80148108 = 0;
    D_8014810C = 0;
    for (i = 0; i < 0xB; i++) {
        D_800D9388[i] = (D_800F53A0.girl[i].unk_10[1] & 1) >= 1;
    }
    func_8004DE1C();
}

void func_8013E7D8(void) {
    D_800F62CF = (u8) D_8012E66C;
    func_80090960(7, 0);
    func_8008F618(D_800F62CF);
    func_8004DE1C();
}

void func_8013E824(void) {
    s32 r;
    u8 s;

    r = func_8005E0E0(D_800F62CF);
    s = r & 0x7F;
    D_80148108 = 0;
    if (s >= 2U) {
        D_80148108 += 2;
    }
    if (s >= 3U) {
        D_80148108 += 2;
    }
    if (s == 4) {
        D_80148108 += 2;
    }
    switch (D_800F62CF) {
    case 0:
        D_801480FC = D_80148064;
        D_80148100 = D_80148098;
        D_80148104 = D_801480CC;
        break;
    case 1:
        D_801480FC = D_80148078;
        D_80148100 = D_801480AC;
        D_80148104 = D_801480E0;
        break;
    case 2:
        D_801480FC = D_80148068;
        D_80148100 = D_8014809C;
        D_80148104 = D_801480D0;
        break;
    case 3:
        D_801480FC = D_80148070;
        D_80148100 = D_801480A4;
        D_80148104 = D_801480D8;
        break;
    case 4:
        D_801480FC = D_80148088;
        D_80148100 = D_801480BC;
        D_80148104 = D_801480F0;
        break;
    case 5:
        D_801480FC = D_8014807C;
        D_80148100 = D_801480B0;
        D_80148104 = D_801480E4;
        break;
    case 6:
        D_801480FC = D_80148080;
        D_80148100 = D_801480B4;
        D_80148104 = D_801480E8;
        break;
    case 7:
        D_801480FC = D_80148074;
        D_80148100 = D_801480A8;
        D_80148104 = D_801480DC;
        break;
    case 8:
        D_801480FC = D_80148060;
        D_80148100 = D_80148094;
        D_80148104 = D_801480C8;
        break;
    case 9:
        D_801480FC = D_80148084;
        D_80148100 = D_801480B8;
        D_80148104 = D_801480EC;
        break;
    case 10:
        D_801480FC = D_80148090;
        D_80148100 = D_801480C4;
        D_80148104 = D_801480F8;
        break;
    }
    func_8004DE1C();
}
