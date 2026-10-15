#define MAIN_API_OVERRIDE_func_8004A8EC /* matched with void(s32) (main_api.h: void(u8)) */
#include "common.h"
#include "ovl/EVENT.h"

void func_8004A8EC(s32 arg0);

void func_8010FE10(void) {
    D_801251D0 = 0x801A427C;
    D_801251D4 = 0x801A4D2C;
    D_801251D8 = 0x801A43EC;
    D_801251DC = 0x801A4E9C;
    D_801251E0 = 0x801A47F0;
    D_801251E4 = 0x801A52A0;
    D_801251E8 = 0x801A4190;
    D_801251EC = 0x801A4C40;
    D_801251F0 = 0x801A4A04;
    D_801251F4 = 0x801A54B4;
    D_801251F8 = 0x801A4368;
    D_801251FC = 0x801A4E18;
    D_80125200 = 0x801A4430;
    D_80125204 = 0x801A4EE0;
    D_80125208 = 0x801A4684;
    D_8012520C = 0x801A5134;
    D_80125210 = 0x801A497C;
    D_80125214 = 0x801A542C;
    D_80125218 = 0x801A4238;
    D_8012521C = 0x801A4CE8;
    D_80125220 = 0x801A49C0;
    D_80125224 = 0x801A5470;
    D_80125228 = 0x801A41F4;
    D_8012522C = 0x801A4CA4;
    D_80125230 = 0x801A4064;
    D_80125234 = 0x801A4B14;
    D_80125238 = 0x801A4A28;
    D_8012523C = 0x801A54D8;
    D_80125240 = 0x801A4A6C;
    D_80125244 = 0x801A551C;
    D_80125248 = 0x801A4600;
    D_8012524C = 0x801A50B0;
    D_80125250 = 0x801A412C;
    D_80125254 = 0x801A4BDC;
    D_80125258 = 0x801A4324;
    D_8012525C = 0x801A4DD4;
    D_80125260 = 0x801A4474;
    D_80125264 = 0x801A4F24;
    D_80125268 = 0x801A4000;
    D_8012526C = 0x801A4AB0;
}

void func_80110094(void) {
    D_80125270 = 0x801E2820;
    D_80125274 = 0x801E2830;
    D_80125278 = 0x801E28B4;
    D_80125280 = 0x801A0000;
    D_80125284 = 0x801BA000;
    D_80125288 = 0x801D2200;
    D_8012528C = 0x801A2000;
    D_80125290 = 0x801A6000;
    D_80125294 = 0x801AA000;
    D_80125298 = 0x801AE000;
    D_8012529C = 0x801B2000;
    D_801252A0 = 0x801B6000;
    D_801252A4 = 0x801BA200;
    D_801252A8 = 0x801BE200;
    D_801252AC = 0x801C2200;
    D_801252B0 = 0x801C6200;
    D_801252B4 = 0x801CA200;
    D_801252B8 = 0x801CE200;
}

void func_801101B4(void) {
    D_801252BC = 0x801F0638;
    D_801252C0 = 0x801F0C34;
    D_801252C4 = 0x801F063C;
    D_801252C8 = 0x801F0C3C;
    D_801252CC = 0x801F071C;
    D_801252D0 = 0x801F0CEC;
    D_801252DC = 0x801E0000;
    D_801252E0 = 0x801E2000;
}

void func_80110234(void) {
    u8 v;

    func_80017E50();
    D_80094730 += D_8012531C * (u8)D_801252E8;
    D_80094734 = 0;
    v = func_8002328C(D_800B1746);
    D_80124344 = (((v & 0x7F) >= 2U) + ((v & 0x7F) >= 3U)) * (((*(s8 *)&D_801252E4) == 0) + 1) + D_801252E4 * 6 - ((*(s8 *)&D_801252E4) == 2) * 3;
    D_80124348 = 0;
    *(s16 *)&D_80124358 = 0;
    *(s16 *)&D_80124360 = 0;
    D_8012434C = 0;
    D_80124350 = 0;
    D_8012437C = 0;
    D_80124380 = 0;
    D_80124370 = 0;
    D_80124378 = 0;
    *(s16 *)&D_80124364 = 0;
    *(s16 *)&D_80124368 = 0;
    if (((s16)D_800B0898 == 6) && (D_800B0897 < 8)) {
        D_80125328 = 1;
    } else {
        D_80125328 = 0;
    }
    func_80017E50();
    func_80045B14();
    D_80094868 = 0;
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010FE10", func_801103A0);

void func_80111E60(void) {
    D_8009473C = D_8012421C;
    D_80094740 = D_80124288;
    D_80094744 = D_801242F4;
    D_800EECD0 = 0;
    D_8012531C = 0;
    func_80078970(D_80094784, "近所の公園");
    D_801252E8 = 2;
    D_801243A4 = 0;
    D_801243A6 = 0;
    D_800EECE4 = -1;
    func_800469F4(0x4264);
    func_80011DFC();
}

void func_80111F04(void) {
    EventStateView *g;
    u8 sp2B;
    s32 temp_v0;
    s32 temp_t5;

    g = (EventStateView *)&D_800B0860;

    D_8009473C = D_80124254;
    D_80094740 = D_801242C0;
    D_80094744 = D_8012432C;
    D_800EECD0 = 0;
    func_80078970(D_80094784, "きらめき公園");
    D_801252E8 = 3;
    sp2B = func_8002328C(g->unk_EE6);
    temp_v0 = func_8002336C(g->unk_EE6);
    if ((((g->unk_036 == 3) && (g->unk_037 >= 0x1AU)) || ((g->unk_036 == 4) && (g->unk_037 < 0xBU))) &&
        ((g->unk_5CA[0] != 0) || (g->unk_EE6 != 0)) &&
        ((D_800B0E43 != 0) || (g->unk_EE6 != 4) || (sp2B != 0x80) || (temp_v0 != 0))) {
        D_8012531C = 2;
        D_801252E8 = 3;
        func_80078970(D_800947C4, "桜");
        D_800EECE4 = -1;
    } else {
        if (!(g->unk_1A4[g->unk_EE6].pad10[4] & 1) &&
            ((g->unk_036 < 6U) || (g->unk_036 >= 9U) || (g->unk_5CA[0x42] != 0) || (sp2B != 0x80) || (temp_v0 != 0))) {
            D_8012531C = 0;
            func_80078970(D_800947C4, "並木道");
            D_800EECE4 = 0;
        } else {
            D_8012531C = 1;
            func_80078970(D_800947C4, "池");
            D_800EECE4 = 0;
        }
    }
    temp_t5 = D_8012531C * 3;
    D_8012439C = (s16) *((g->unk_EE6 * 9) + ((u8 *)D_80125268) + temp_t5);
    D_8012439E = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 9) + ((u8 *)D_80125268) + temp_t5) + (1));
    D_801243A0 = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 9) + ((u8 *)D_80125268) + temp_t5) + (2));
    D_8012438C = (s16) *((g->unk_EE6 * 9) + ((u8 *)D_8012526C) + temp_t5);
    D_8012438E = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 9) + ((u8 *)D_8012526C) + temp_t5) + (1));
    D_80124390 = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 9) + ((u8 *)D_8012526C) + temp_t5) + (2));
    func_800469F4(0x4277);
    func_80011DFC();
}

void func_8011220C(void) {
    EventStateView *g;
    u8 *r;
    s32 temp_t2;

    g = (EventStateView *)&D_800B0860;

    D_8009473C = D_80124248;
    D_80094740 = D_801242B4;
    D_80094744 = D_80124320;
    func_80078970(D_80094784, "水族館");
    D_801252E8 = 3;
    r = D_800B0860 + g->unk_EE6 * 0x34;
    if ((r[0x1BD] == 0) && ((g->unk_035 >= 0x61U) || ((g->unk_035 == 0x60) && (g->unk_036 >= 3U)))) {
        D_8012531C = 2;
        func_80078970(D_800947C4, "イルカショー");
        D_800EECE4 = 5;
    } else if (!(r[0x1BC] & 1)) {
        D_8012531C = 0;
        func_80078970(D_800947C4, "館内");
        D_800EECE4 = 4;
    } else {
        D_8012531C = 1;
        func_80078970(D_800947C4, "館内");
        D_800EECE4 = 4;
    }
    temp_t2 = D_8012531C * 3;
    D_8012439C = (s16) *((g->unk_EE6 * 9) + ((u8 *)D_80125250) + temp_t2);
    D_8012439E = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 9) + ((u8 *)D_80125250) + temp_t2) + (1));
    D_801243A0 = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 9) + ((u8 *)D_80125250) + temp_t2) + (2));
    D_8012438C = (s16) *((g->unk_EE6 * 9) + ((u8 *)D_80125254) + temp_t2);
    D_8012438E = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 9) + ((u8 *)D_80125254) + temp_t2) + (1));
    D_80124390 = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 9) + ((u8 *)D_80125254) + temp_t2) + (2));
    func_800469F4(0x44FD);
    func_80011DFC();
}

void func_80112460(void) {
    EventStateView *g;
    u8 *r;
    s32 temp_t9;
    u32 temp_hi;

    g = (EventStateView *)&D_800B0860;

    D_8009473C = D_8012420C;
    D_80094740 = D_80124278;
    D_80094744 = D_801242E4;
    func_80078970(D_80094784, "動物園");
    D_801252E8 = 3;
    r = D_800B0860 + g->unk_EE6 * 0x34;
    if ((r[0x1BF] == 0) && ((g->unk_035 >= 0x60U) || ((g->unk_035 == 0x5F) && (g->unk_036 >= 0xCU)))) {
        D_8012531C = 2;
        func_80078970(D_800947C4, "コアラ");
        D_800EECE4 = 7;
    } else if (!(r[0x1BE] & 1)) {
        D_8012531C = 0;
        func_80078970(D_800947C4, "園内");
        D_800EECE4 = 6;
    } else {
        D_8012531C = 1;
        func_80078970(D_800947C4, "園内");
        D_800EECE4 = 6;
    }
    if (g->unk_EE6 == 7) {
        D_800EECE4 = 6;
        func_80078970(D_800947C4, "園内");
        temp_hi = D_800B0860[g->unk_EE6 * 0x34 + 0x1BE] % 3U;
        if (temp_hi == 0) {
            D_8012531C = 0;
        } else if (temp_hi == 1) {
            D_8012531C = 1;
        } else {
            D_8012531C = 2;
        }
    }
    temp_t9 = D_8012531C * 3;
    D_8012439C = (s16) *((g->unk_EE6 * 9) + ((u8 *)D_801251E8) + temp_t9);
    D_8012439E = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 9) + ((u8 *)D_801251E8) + temp_t9) + (1));
    D_801243A0 = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 9) + ((u8 *)D_801251E8) + temp_t9) + (2));
    D_8012438C = (s16) *((g->unk_EE6 * 9) + ((u8 *)D_801251EC) + temp_t9);
    D_8012438E = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 9) + ((u8 *)D_801251EC) + temp_t9) + (1));
    D_80124390 = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 9) + ((u8 *)D_801251EC) + temp_t9) + (2));
    func_800469F4(0x44E9);
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010FE10", func_80112750);

void func_80112930(void) {
    EventStateView *g;
    s32 temp_v0;

    g = (EventStateView *)&D_800B0860;

    D_8009473C = D_8012422C;
    D_80094740 = D_80124298;
    D_80094744 = D_80124304;
    func_80078970(D_80094784, "プラネタリウム");
    D_801252E8 = 7;
    if (!(D_800B0860[g->unk_EE6 * 0x34 + 0x1C1] & 1)) {
        (*((u8 *)&D_80125318 + 4)) = 0;
        func_80078970(D_800947C4, "館内");
    } else {
        (*((u8 *)&D_80125318 + 4)) = 1;
        func_80078970(D_800947C4, "館内");
        if (g->unk_EE6 == 0xA) {
            D_80094730 = 1;
        }
    }
    D_800EECE4 = 9;
    temp_v0 = (*((u8 *)&D_80125318 + 4)) * 3;
    D_8012439C = (s16) *((g->unk_EE6 * 6) + ((u8 *)D_80125218) + temp_v0);
    D_8012439E = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 6) + ((u8 *)D_80125218) + temp_v0) + (1));
    D_801243A0 = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 6) + ((u8 *)D_80125218) + temp_v0) + (2));
    D_8012438C = (s16) *((g->unk_EE6 * 6) + ((u8 *)D_8012521C) + temp_v0);
    D_8012438E = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 6) + ((u8 *)D_8012521C) + temp_v0) + (1));
    D_80124390 = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 6) + ((u8 *)D_8012521C) + temp_v0) + (2));
    if ((D_800B0956 < 0x78) && ((*((u8 *)&D_80125318 + 4)) != 0)) {
        switch (g->unk_EE6) {
        case 0:
        case 3:
            D_8012439C = 4;
            D_8012438C = 4;
            break;
        case 2:
        case 6:
        case 8:
            D_8012439C = 4;
            D_8012438C = 3;
            break;
        case 1:
        case 4:
        case 5:
        case 9:
            D_8012438C = 3;
            D_8012439C = 3;
            break;
        case 7:
            D_80124390 = 3;
            D_801243A0 = 3;
            break;
        case 10:
            D_80124390 = 1;
            D_801243A0 = 1;
            break;
        }
    }
    func_800469F4(0x4552);
    func_80011DFC();
}

/* FAKE: a signed narrow constant preserves the shared multiply/compare
 * range. EventResultRow6 models the two signed three-halfword outputs.
 * T-10870: the final message helper takes only its message id. */
void func_80112BEC(void) {
    s16 three = 3;
    s32 choice_offset;
    EventStateView *g;

    g = (EventStateView *)&D_800B0860;
    D_8009473C = D_80124200;
    D_80094740 = D_8012426C;
    D_80094744 = D_801242D8;
    func_80078970(D_80094784, "美術館");
    D_801252E8 = three;
    if ((g->unk_035 == 0x61) && ((u8) g->unk_036 >= 9U) && ((u8) g->unk_036 < 0xCU)) {
        (*(u8 *)&D_8012531C) = 2;
        D_801252F4 = 1;
        func_80078970(D_800947C4, "ガーキー展");
        if (g->unk_1A4[g->unk_EE6].pad18[7] != 0) {
            (*(u8 *)&D_8012531C) = 5;
        }
        D_800EECE4 = 0xB;
        if (g->unk_EE6 == 7) {
            D_80094730 = 2;
        }
    } else if ((g->unk_035 == 0x61) && ((u8) g->unk_036 >= 3U) && ((u8) g->unk_036 < 6U)) {
        (*(u8 *)&D_8012531C) = three;
        D_801252F8 = 1;
        func_80078970(D_800947C4, "彫塑展");
        if (g->unk_1A4[g->unk_EE6].pad18[8] != 0) {
            (*(u8 *)&D_8012531C) = 5;
        }
        D_800EECE4 = 0xC;
        if (g->unk_EE6 == 7) {
            D_80094730 = 4;
        }
    } else if ((g->unk_035 == 0x5F) && ((u8) g->unk_036 >= 9U) && ((u8) g->unk_036 < 0xCU)) {
        (*(u8 *)&D_8012531C) = 4;
        D_801252FC = 1;
        func_80078970(D_800947C4, "有名画家展");
        if (g->unk_1A4[g->unk_EE6].pad18[9] != 0) {
            (*(u8 *)&D_8012531C) = 5;
        }
        D_800EECE4 = 0xD;
        if (g->unk_EE6 == 7) {
            D_80094730 = 4;
        }
    } else if (!(g->unk_1A4[g->unk_EE6].pad18[6] & 1)) {
        (*(u8 *)&D_8012531C) = 0;
        func_80078970(D_800947C4, "館内");
        D_800EECE4 = 0xA;
    } else {
        (*(u8 *)&D_8012531C) = 1;
        func_80078970(D_800947C4, "館内");
        D_800EECE4 = 0xA;
        if (g->unk_EE6 == 7) {
            D_80094730 = 2;
        }
    }
    choice_offset = (*(u8 *)&D_8012531C) * three;
    ((EventResultRow6 *)&D_8012439C)->value[0] = (s16) *(u8 *)((g->unk_EE6 * 0xF) + D_801251D0 + choice_offset);
    ((EventResultRow6 *)&D_8012439C)->value[1] = (s16) *(u8 *)((u8 *)((g->unk_EE6 * 0xF) + D_801251D0 + choice_offset) + (1));
    ((EventResultRow6 *)&D_8012439C)->value[2] = (s16) *(u8 *)((u8 *)((g->unk_EE6 * 0xF) + D_801251D0 + choice_offset) + (2));
    ((EventResultRow6 *)&D_8012438C)->value[0] = (s16) *(u8 *)((g->unk_EE6 * 0xF) + D_801251D4 + choice_offset);
    ((EventResultRow6 *)&D_8012438C)->value[1] = (s16) *(u8 *)((u8 *)((g->unk_EE6 * 0xF) + D_801251D4 + choice_offset) + (1));
    ((EventResultRow6 *)&D_8012438C)->value[2] = (s16) *(u8 *)((u8 *)((g->unk_EE6 * 0xF) + D_801251D4 + choice_offset) + (2));
    if ((*(u8 *)&D_8012531C) == 5) {
        switch (g->unk_EE6) {
        default:
            ((EventResultRow6 *)&D_8012438C)->value[0] = ((EventResultRow6 *)&D_8012438C)->value[1] = ((EventResultRow6 *)&D_8012438C)->value[2] = three;
            ((EventResultRow6 *)&D_8012439C)->value[0] = ((EventResultRow6 *)&D_8012439C)->value[1] = ((EventResultRow6 *)&D_8012439C)->value[2] = three;
            break;
        case 1: case 3: case 7: case 8:
            ((EventResultRow6 *)&D_8012438C)->value[0] = ((EventResultRow6 *)&D_8012438C)->value[1] = ((EventResultRow6 *)&D_8012438C)->value[2] = 4;
            ((EventResultRow6 *)&D_8012439C)->value[0] = ((EventResultRow6 *)&D_8012439C)->value[1] = ((EventResultRow6 *)&D_8012439C)->value[2] = 4;
            break;
        }
        if ((g->unk_EE6 == three) && (D_801252F4 != 0)) {
            ((EventResultRow6 *)&D_8012439C)->value[0] = ((EventResultRow6 *)&D_8012439C)->value[1] = ((EventResultRow6 *)&D_8012439C)->value[2] = three;
        }
        if (g->unk_EE6 == 0xA) {
            D_80094730 += 1;
        }
    }
    func_800469F4(0x445D);
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010FE10", func_801130F0);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010FE10", func_80113300);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010FE10", func_80113520);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010FE10", func_801136FC);

void func_801138D8(void) {
    EventStateView *g;

    g = (EventStateView *)&D_800B0860;
    D_8009473C = D_80124250;
    D_80094740 = D_801242BC;
    D_80094744 = D_80124328;
    if ((g->unk_035 >= 0x62U) || ((g->unk_035 == 0x61) && (g->unk_036 >= 3U))) {
        D_80125304 = 1;
    } else {
        D_80125304 = 0;
    }
    if ((g->unk_035 == 0x62) || ((g->unk_035 == 0x61) && (g->unk_036 >= 0xCU))) {
        D_80125308 = 1;
    } else {
        D_80125308 = 0;
    }
    D_801252E8 = 0;
    func_80078970(D_80094784, "遊園地");
    func_800469F4(0x429B);
    func_80011DFC();
}

void func_801139CC(void) {
    D_8009473C = D_80124238;
    D_80094740 = D_801242A4;
    D_80094744 = D_80124310;
    D_801252E8 = 0;
    func_80078970(D_80094784, "ショッピング街");
    func_800469F4(0x448C);
    func_80011DFC();
}

void func_80113A40(void) {
    EventStateView *g;
    s32 temp_t3;
    s8 v;

    g = (EventStateView *)&D_800B0860;

    D_8009473C = D_80124244;
    D_80094740 = D_801242B0;
    D_80094744 = D_8012431C;
    func_80078970(D_80094784, "スタジアム");
    func_80078970(D_800947C4, "スタジアム内");
    D_801252E8 = 3;
    if ((g->unk_036 == 2) || (g->unk_036 == 3) || (g->unk_036 == 0xC)) {
        v = 1;
    } else {
        v = 0;
    }
    D_8012532C = v;
    if (v != 0) {
        if (!(g->unk_1A4[g->unk_EE6].pad2E[1] & 1)) {
            D_8012531C = 2;
        } else {
            D_8012531C = 3;
        }
        D_800EECE4 = 0x1B;
    } else {
        if (!(g->unk_1A4[g->unk_EE6].pad2E[0] & 1)) {
            D_8012531C = 0;
        } else {
            D_8012531C = 1;
        }
        D_800EECE4 = 0x1A;
    }
    temp_t3 = D_8012531C * 3;
    D_8012439C = (s16) *((g->unk_EE6 * 0xC) + ((u8 *)D_80125248) + temp_t3);
    D_8012439E = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 0xC) + ((u8 *)D_80125248) + temp_t3) + (1));
    D_801243A0 = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 0xC) + ((u8 *)D_80125248) + temp_t3) + (2));
    D_8012438C = (s16) *((g->unk_EE6 * 0xC) + ((u8 *)D_8012524C) + temp_t3);
    D_8012438E = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 0xC) + ((u8 *)D_8012524C) + temp_t3) + (1));
    D_80124390 = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 0xC) + ((u8 *)D_8012524C) + temp_t3) + (2));
    func_800469F4(0x4564);
    func_80011DFC();
}

void func_80113CA0(void) {
    EventStateView *g;
    s32 temp_t3;
    u8 var_t1;

    g = (EventStateView *)&D_800B0860;

    D_8009473C = D_80124220;
    D_80094740 = D_8012428C;
    D_80094744 = D_801242F8;
    D_801252E8 = 0;
    if (g->unk_035 == 0x5F) {
        if (((u8) g->unk_036 >= 6U) && ((u8) g->unk_036 < 9U)) {
            D_80125314 = 0;
        } else if (((u8) g->unk_036 >= 9U) && ((u8) g->unk_036 < 0xCU)) {
            D_80125314 = 1;
        } else {
            D_80125314 = 2;
        }
    } else if (g->unk_035 == 0x60) {
        if ((g->unk_036 != 0) && ((u8)g->unk_036 < 3U)) {
            D_80125314 = 2;
        } else if (((u8) g->unk_036 >= 3U) && ((u8) g->unk_036 < 6U)) {
            D_80125314 = 3;
        } else if (((u8) g->unk_036 >= 6U) && ((u8) g->unk_036 < 9U)) {
            D_80125314 = 4;
        } else if (((u8) g->unk_036 >= 9U) && ((u8) g->unk_036 < 0xCU)) {
            D_80125314 = 5;
        } else {
            D_80125314 = 6;
        }
    } else {
        var_t1 = 0xA;
        if (g->unk_035 == 0x61) {
            if ((g->unk_036 != 0) && ((u8) g->unk_036 < 3U)) {
                D_80125314 = 6;
            } else if (((u8) g->unk_036 >= 3U) && ((u8) g->unk_036 < 6U)) {
                D_80125314 = 7;
            } else if (((u8) g->unk_036 >= 6U) && ((u8) g->unk_036 < 9U)) {
                D_80125314 = 8;
            } else if (((u8) g->unk_036 >= 9U) && ((u8) g->unk_036 < 0xCU)) {
                D_80125314 = 9;
            } else {
                D_80125314 = 0xA;
            }
        } else {
            D_80125314 = var_t1;
        }
    }
    D_800EECE4 = -1;
    func_80078970(D_80094784, "映画館");
    func_80078970(D_800947C4, "館内");
    temp_t3 = D_80125314 * 3;
    D_8012439C = (s16) *((g->unk_EE6 * 0x21) + ((u8 *)D_80125208) + temp_t3);
    D_8012439E = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 0x21) + ((u8 *)D_80125208) + temp_t3) + (1));
    D_801243A0 = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 0x21) + ((u8 *)D_80125208) + temp_t3) + (2));
    D_8012438C = (s16) *((g->unk_EE6 * 0x21) + ((u8 *)D_8012520C) + temp_t3);
    D_8012438E = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 0x21) + ((u8 *)D_8012520C) + temp_t3) + (1));
    D_80124390 = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 0x21) + ((u8 *)D_8012520C) + temp_t3) + (2));
    func_800469F4(0x4360);
    func_80011DFC();
}

void func_80113FD4(void) {
    EventStateView *g;
    s32 temp_t3;
    u8 var_t1;

    g = (EventStateView *)&D_800B0860;

    D_8009473C = D_80124208;
    D_80094740 = D_80124274;
    D_80094744 = D_801242E0;
    D_801252E8 = 0;
    if (g->unk_035 == 0x5F) {
        if (((u8) g->unk_036 >= 9U) && ((u8) g->unk_036 < 0xCU)) {
            D_80125318 = 2;
        } else {
            D_80125318 = 3;
        }
    } else if (g->unk_035 == 0x60) {
        if ((g->unk_036 != 0) && ((u8)g->unk_036 < 3U)) {
            D_80125318 = 3;
        } else if (((u8) g->unk_036 >= 3U) && ((u8) g->unk_036 < 6U)) {
            D_80125318 = 4;
        } else if (((u8) g->unk_036 >= 6U) && ((u8) g->unk_036 < 9U)) {
            D_80125318 = 5;
        } else if (((u8) g->unk_036 >= 9U) && ((u8) g->unk_036 < 0xCU)) {
            D_80125318 = 6;
        } else {
            D_80125318 = 7;
        }
    } else {
        var_t1 = 0xB;
        if (g->unk_035 == 0x61) {
            if ((g->unk_036 != 0) && ((u8) g->unk_036 < 3U)) {
                D_80125318 = 7;
            } else if (((u8) g->unk_036 >= 3U) && ((u8) g->unk_036 < 6U)) {
                D_80125318 = 8;
            } else if (((u8) g->unk_036 >= 6U) && ((u8) g->unk_036 < 9U)) {
                D_80125318 = 9;
            } else if (((u8) g->unk_036 >= 9U) && ((u8) g->unk_036 < 0xCU)) {
                D_80125318 = 0xA;
            } else {
                D_80125318 = 0xB;
            }
        } else {
            D_80125318 = var_t1;
        }
    }
    D_800EECE4 = -1;
    func_80078970(D_80094784, "コンサート会場");
    func_80078970(D_800947C4, "会場内");
    temp_t3 = D_80125318 * 3;
    D_8012439C = (s16) *((g->unk_EE6 * 0x24) + ((u8 *)D_801251E0) + temp_t3);
    D_8012439E = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 0x24) + ((u8 *)D_801251E0) + temp_t3) + (1));
    D_801243A0 = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 0x24) + ((u8 *)D_801251E0) + temp_t3) + (2));
    D_8012438C = (s16) *((g->unk_EE6 * 0x24) + ((u8 *)D_801251E4) + temp_t3);
    D_8012438E = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 0x24) + ((u8 *)D_801251E4) + temp_t3) + (1));
    D_80124390 = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 0x24) + ((u8 *)D_801251E4) + temp_t3) + (2));
    func_800469F4(0x43D6);
    func_80011DFC();
}

void func_801142E8(void) {
    EventStateView *g;
    s32 temp;

    g = (EventStateView *)&D_800B0860;

    D_8009473C = D_80124228;
    D_80094740 = D_80124294;
    D_80094744 = D_80124300;
    D_801252E8 = 3;
    func_80078970(D_80094784, "プール");
    func_80078970(D_800947C4, "プール内");
    if (!(D_800B0860[g->unk_EE6 * 0x34 + 0x1D4] & 1)) {
        D_8012531C = 0;
    } else {
        D_8012531C = 1;
    }
    D_800EECE4 = 0x1C;
    temp = D_8012531C * 3;
    D_8012439C = (s16) *((g->unk_EE6 * 6) + ((u8 *)D_80125210) + temp);
    D_8012439E = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 6) + ((u8 *)D_80125210) + temp) + (1));
    D_801243A0 = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 6) + ((u8 *)D_80125210) + temp) + (2));
    D_8012438C = (s16) *((g->unk_EE6 * 6) + ((u8 *)D_80125214) + temp);
    D_8012438E = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 6) + ((u8 *)D_80125214) + temp) + (1));
    D_80124390 = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 6) + ((u8 *)D_80125214) + temp) + (2));
    func_800469F4(0x452B);
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010FE10", func_801144AC);

void func_801146B4(void) {
    D_8009473C = D_80124210;
    D_80094740 = D_8012427C;
    D_80094744 = D_801242E8;
    func_80078970(D_80094784, "縁日");
    func_80078970(D_800947C4, "花火大会");
    D_801252E8 = 5;
    D_8012438C = 0;
    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    D_8012438E = (u8)func_8002328C(D_800B1746) & 0x7F;
    D_801243A4 = 0;
    D_801243A6 = 0;
    D_8012531C = 0;
    D_800EECE4 = -1;
    func_800469F4(0x425B);
    func_80011DFC();
}

void func_80114784(void) {
    EventStateView *g;
    s32 temp;

    g = (EventStateView *)&D_800B0860;

    D_8009473C = D_8012423C;
    D_80094740 = D_801242A8;
    D_80094744 = D_80124314;
    D_801252E8 = 3;
    func_80078970(D_80094784, "スケート場");
    func_80078970(D_800947C4, "スケートリンク");
    if (!(D_800B0860[g->unk_EE6 * 0x34 + 0x1D6] & 1)) {
        D_8012531C = 0;
    } else {
        D_8012531C = 1;
    }
    D_800EECE4 = 0x1E;
    temp = D_8012531C * 3;
    D_8012439C = (s16) *((g->unk_EE6 * 6) + ((u8 *)D_80125238) + temp);
    D_8012439E = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 6) + ((u8 *)D_80125238) + temp) + (1));
    D_801243A0 = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 6) + ((u8 *)D_80125238) + temp) + (2));
    D_8012438C = (s16) *((g->unk_EE6 * 6) + ((u8 *)D_8012523C) + temp);
    D_8012438E = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 6) + ((u8 *)D_8012523C) + temp) + (1));
    D_80124390 = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 6) + ((u8 *)D_8012523C) + temp) + (2));
    func_800469F4(0x44D8);
    func_80011DFC();
}

#ifdef NON_MATCHING
void func_80114948(void) {
    EventStateView *g;
    s32 temp;

    g = (EventStateView *)&D_800B0860;

    D_8009473C = D_80124240;
    D_80094740 = D_801242AC;
    D_80094744 = D_80124318;
    D_801252E8 = 3;
    func_80078970(D_80094784, "スキー場");
    func_80078970(D_800947C4, "ゲレンデ");
    if (!(D_800B0860[g->unk_EE6 * 0x34 + 0x1D7] & 1)) {
        D_8012531C = 0;
    } else {
        D_8012531C = 1;
    }
    D_800EECE4 = 0x1F;
    temp = D_8012531C * 3;
    D_8012439C = (s16) *((g->unk_EE6 * 6) + ((u8 *)D_80125240) + temp);
    D_8012439E = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 6) + ((u8 *)D_80125240) + temp) + (1));
    D_801243A0 = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 6) + ((u8 *)D_80125240) + temp) + (2));
    D_8012438C = (s16) *((g->unk_EE6 * 6) + ((u8 *)D_80125244) + temp);
    D_8012438E = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 6) + ((u8 *)D_80125244) + temp) + (1));
    D_80124390 = (s16) *(u8 *)((unsigned char *)((g->unk_EE6 * 6) + ((u8 *)D_80125244) + temp) + (2));
    func_800469F4(0x44C4);
    func_80011DFC();
}
#else
INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010FE10", func_80114948);
#endif

/* NON_MATCHING: T-10330, T-11600, 4 differing lines: base registers of D_80124394/D_8012439C swapped (t1/t2). regdiff: both
 * hoisted addresses have priority 2.5 and D_8012439C, used first by `v = *r`, is coloured first; v = *p = *r, *p = *r; v = *p,
 * array views and statement orders did not reverse it. The u8 copy of D_800B0F2C fixed the bne order (autotweak). */
#ifdef NON_MATCHING
void func_80114B0C(void) {
    u8 sel;
    s16 i;
    s16 *p;
    s16 *r;
    s16 *q;
    s16 v;
    u8 temp;

    sel = D_800B0F35[D_800B1746];
    for (i = 0; i < 3; i++) {
        p = &D_80124394 + i;
        r = &D_8012439C + i;
        q = &D_801243A4 + i;
        *p = v = *r;
        if (*p == 0) {
            *p = 1;
        }
        if (*p == 4) {
            *p = 3;
        }
        temp = D_800B0F2C;
        if (temp == sel) {
            *p = 3;
        }
        *q = v - 1;
        if (*q == -1) {
            *q = 0;
        }
        if (*q == 3) {
            *q = 2;
        }
    }
    func_80011DFC();
}
#else
INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010FE10", func_80114B0C);
#endif

void func_80114BFC(void) {
    if (D_8012531C == 0) {
        func_800469F4(0x4289);
    } else if (D_8012531C == 1) {
        func_800469F4(0x4280);
    } else {
        func_800469F4(0x4292);
    }
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010FE10", func_80114C5C);

void func_80115320(void) {
    func_800469F4(0x4506);
    func_80011DFC();
}

void func_80115348(void) {
    func_800469F4(0x44F3);
    func_80011DFC();
}

void func_80115370(void) {
    func_800469F4(0x4548);
    func_80011DFC();
}

void func_80115398(void) {
    func_800469F4(0x455C);
    func_80011DFC();
}

void func_801153C0(void) {
    if (D_801252F4 != 0) {
        func_800469F4(0x446F);
    } else if (D_801252F8 != 0) {
        func_800469F4(0x4479);
    } else if (D_801252FC != 0) {
        func_800469F4(0x4482);
    } else {
        func_800469F4(0x4466);
    }
    func_80011DFC();
}

void func_80115448(void) {
    func_800469F4(0x44BA);
    func_80011DFC();
}

void func_80115470(void) {
    if (D_800EECBC == 0) {
        func_800469F4(0x444B);
    } else {
        func_800469F4(0x4454);
    }
    func_80011DFC();
}

void func_801154B4(void) {
    func_800469F4(0x4593);
    func_80011DFC();
}

void func_801154DC(void) {
    func_800469F4(0x4589);
    func_80011DFC();
}

void func_80115504(void) {
    func_800469F4(0x42A5);
    func_80011DFC();
}

void func_8011552C(void) {
    u8 v;

    func_800433D0(0x200);
    func_80078970(D_80094784, "ヒーローショー");
    func_800469F4(0x42E4);
    if (!(D_800B0A04[D_800B1746].unk_27 & 1)) {
        v = 0;
    } else {
        v = 1;
    }
    D_80094730 += v * 2;
    D_800EECE4 = 0x13;
    D_8012531C = v;
    func_80011DFC();
}

/* s32 with no return (K&R): the stored D_800EECBC value stays in $v0 as the return register. */
s32 func_801155D4(void) {
    /* FAKE: unused words reserve top frame slots so s32 msg sits at sp+0x28 in a 0x50 frame; T-11110 */
    s32 pad[9];
    s32 msg;

    if (D_800EECBC == 0) {
        if ((D_80125308 != 0) || (D_80125304 != 0)) {
            msg = 0x42C9;
            D_80125320 = 3;
        } else {
            msg = 0x42AE;
            D_80125320 = 0;
        }
    }
    if (D_800EECBC == 1) {
        D_80094730 += 5;
        if (D_80125308 != 0) {
            msg = 0x42D2;
            D_80125320 = 4;
        } else {
            msg = 0x42B7;
            D_80125320 = 1;
        }
    }
    if (D_800EECBC == 2) {
        D_80094730 += 0xA;
        msg = 0x42C0;
        D_80125320 = 2;
    }
    func_800469F4(msg);
    func_80115C18();
    func_80114B0C();
    if ((D_800B1746 == 8) && ((u8)D_80125320 == 3) && (D_800B0BCF != 0)) {
        D_800EECBC = 1;
    } else {
        D_800EECBC = 0;
    }
}

void func_80115714(void) {
    func_80078970(D_800947C4, "ナイトパレード");
    func_800433D0(0x204);
    func_800469F4(0x42DB);
    (&D_80124394)[0] = (&D_80124394)[1] = (&D_80124394)[2] = 1;
    (&D_801243A4)[0] = (&D_801243A4)[1] = (&D_801243A4)[2] = 0;
    if (D_800B1746 == 2) {
        (&D_80124394)[0] = (&D_80124394)[1] = (&D_80124394)[2] = 3;
        (&D_801243A4)[0] = (&D_801243A4)[1] = (&D_801243A4)[2] = 2;
    }
    func_80011DFC();
}

void func_801157D4(void) {
    func_800469F4(0x429B);
    func_80078970(D_800947C4, "遊園地");
    func_80011DFC();
}

void func_80115810(void) {
    if ((u8) D_8012531C < 2U) {
        func_800433D0(0x200);
        func_800469F4(0x456D);
    } else {
        func_800433D0(0x201);
        func_800469F4(0x4576);
    }
    func_80011DFC();
}

void func_80115868(void) {
    func_800469F4(0x4369);
    func_80011DFC();
}

void func_80115890(void) {
    switch (D_80125314) {
    case 0:
        func_800469F4(0x4372);
        break;
    case 1:
        func_800469F4(0x437B);
        break;
    case 2:
        func_800469F4(0x4384);
        break;
    case 3:
        func_800469F4(0x438C);
        break;
    case 4:
        func_800469F4(0x4396);
        break;
    case 5:
        func_800469F4(0x439F);
        break;
    case 6:
        func_800469F4(0x43A7);
        break;
    case 7:
        func_800469F4(0x43B0);
        break;
    case 8:
        func_800469F4(0x43B9);
        break;
    case 9:
        func_800469F4(0x43C3);
        break;
    case 10:
        func_800469F4(0x43CD);
        break;
    }
    func_80011DFC();
}

void func_80115984(void) {
    func_800469F4(0x4360);
    func_80011DFC();
}

void func_801159AC(void) {
    func_800469F4(0x43E0);
    func_80011DFC();
}

void func_801159D4(void) {
    switch (D_80125318) {
    case 0:
        func_800469F4(0x4430);
        break;
    case 1:
        func_800469F4(0x43E9);
        break;
    case 2:
        func_800469F4(0x440E);
        break;
    case 3:
        func_800469F4(0x4420);
        break;
    case 4:
        func_800469F4(0x43F3);
        break;
    case 5:
        func_800469F4(0x4405);
        break;
    case 6:
        func_800469F4(0x4430);
        break;
    case 7:
        func_800469F4(0x4417);
        break;
    case 8:
        func_800469F4(0x43FC);
        break;
    case 9:
        func_800469F4(0x4430);
        break;
    case 10:
        func_800469F4(0x4428);
        break;
    case 11:
        func_800469F4(0x4439);
        break;
    }
    func_80011DFC();
}

void func_80115AD8(void) {
    func_800469F4(0x43D6);
    func_80011DFC();
}

void func_80115B00(void) {
    func_800469F4(0x4535);
    func_80011DFC();
}

void func_80115B28(void) {
    if ((D_800B1746 != 6) || (D_800B095E < 0x96)) {
        func_800469F4(0x4518);
    } else {
        func_800469F4(0x4522);
    }
    func_80011DFC();
}

void func_80115B88(void) {
    func_800469F4(0x44E0);
    func_80011DFC();
}

void func_80115BB0(void) {
    func_800469F4(0x44CE);
    func_80011DFC();
}

void func_80115BD8(void) {
    func_80015D28(0x86, 0x801B0000, 0xAC0C);
    func_800433D0(0x200);
    func_80110094();
    func_80011DFC();
}

void func_80115C18(void) {
    EventStateView *g;
    s32 temp_t0;
    s32 temp_t1;
    s32 temp_t1_2;
    s32 temp_t8;
    s32 temp_t9;

    g = (EventStateView *)D_800B0860;
    func_80078970(D_800947C4, "遊園地");
    if ((u8) D_80125320 == 0) {
        func_80078970(D_80094784, "ジェットコースター");
        if (!(D_800B0A04[g->unk_EE6].pad28[0] & 1)) {
            D_8012531C = 0;
        } else {
            D_8012531C = 1;
        }
        temp_t1 = D_8012531C * 3;
        ((EventResultRow6 *)&D_8012439C)->value[0] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125260 + temp_t1);
        ((EventResultRow6 *)&D_8012439C)->value[1] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125260 + temp_t1 + 1);
        ((EventResultRow6 *)&D_8012439C)->value[2] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125260 + temp_t1 + 2);
        ((EventResultRow6 *)&D_8012438C)->value[0] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125264 + temp_t1);
        ((EventResultRow6 *)&D_8012438C)->value[1] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125264 + temp_t1 + 1);
        ((EventResultRow6 *)&D_8012438C)->value[2] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125264 + temp_t1 + 2);
        D_800EECE4 = 0x14;
        return;
    }
    if ((u8) D_80125320 == 3) {
        func_80078970(D_80094784, "絶叫マシーンビビール");
        if (!(D_800B0A04[g->unk_EE6].pad28[3] & 1)) {
            D_8012531C = 0;
        } else {
            D_8012531C = 1;
        }
        temp_t0 = D_8012531C * 3;
        D_800EECE4 = 0x17;
        ((EventResultRow6 *)&D_8012439C)->value[0] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125260 + temp_t0 + 18);
        ((EventResultRow6 *)&D_8012439C)->value[1] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125260 + temp_t0 + 19);
        ((EventResultRow6 *)&D_8012439C)->value[2] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125260 + temp_t0 + 20);
        ((EventResultRow6 *)&D_8012438C)->value[0] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125264 + temp_t0 + 18);
        ((EventResultRow6 *)&D_8012438C)->value[1] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125264 + temp_t0 + 19);
        ((EventResultRow6 *)&D_8012438C)->value[2] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125264 + temp_t0 + 20);
        return;
    }
    if ((u8) D_80125320 == 1) {
        func_80078970(D_80094784, "ゴーストハウス");
        if (!(D_800B0A04[g->unk_EE6].pad28[1] & 1)) {
            D_8012531C = 0;
        } else {
            D_8012531C = 1;
        }
        temp_t8 = D_8012531C * 3;
        D_800EECE4 = 0x15;
        ((EventResultRow6 *)&D_8012439C)->value[0] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125260 + temp_t8 + 6);
        ((EventResultRow6 *)&D_8012439C)->value[1] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125260 + temp_t8 + 7);
        ((EventResultRow6 *)&D_8012439C)->value[2] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125260 + temp_t8 + 8);
        ((EventResultRow6 *)&D_8012438C)->value[0] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125264 + temp_t8 + 6);
        ((EventResultRow6 *)&D_8012438C)->value[1] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125264 + temp_t8 + 7);
        ((EventResultRow6 *)&D_8012438C)->value[2] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125264 + temp_t8 + 8);
        return;
    }
    if ((u8) D_80125320 == 4) {
        func_80078970(D_80094784, "バーチャルシップ");
        if (!(D_800B0A04[g->unk_EE6].pad28[4] & 1)) {
            D_8012531C = 0;
        } else {
            D_8012531C = 1;
        }
        temp_t1_2 = D_8012531C * 3;
        D_800EECE4 = 0x18;
        ((EventResultRow6 *)&D_8012439C)->value[0] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125260 + temp_t1_2 + 24);
        ((EventResultRow6 *)&D_8012439C)->value[1] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125260 + temp_t1_2 + 25);
        ((EventResultRow6 *)&D_8012439C)->value[2] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125260 + temp_t1_2 + 26);
        ((EventResultRow6 *)&D_8012438C)->value[0] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125264 + temp_t1_2 + 24);
        ((EventResultRow6 *)&D_8012438C)->value[1] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125264 + temp_t1_2 + 25);
        ((EventResultRow6 *)&D_8012438C)->value[2] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125264 + temp_t1_2 + 26);
        return;
    }
    func_80078970(D_80094784, "観覧車");
    if (!(D_800B0A04[g->unk_EE6].pad28[2] & 1)) {
        D_8012531C = 0;
    } else {
        D_8012531C = 1;
    }
    temp_t9 = D_8012531C * 3;
    D_800EECE4 = 0x16;
    ((EventResultRow6 *)&D_8012439C)->value[0] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125260 + temp_t9 + 12);
    ((EventResultRow6 *)&D_8012439C)->value[1] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125260 + temp_t9 + 13);
    ((EventResultRow6 *)&D_8012439C)->value[2] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125260 + temp_t9 + 14);
    ((EventResultRow6 *)&D_8012438C)->value[0] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125264 + temp_t9 + 12);
    ((EventResultRow6 *)&D_8012438C)->value[1] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125264 + temp_t9 + 13);
    ((EventResultRow6 *)&D_8012438C)->value[2] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125264 + temp_t9 + 14);
}

void func_8011630C(void) {
    if (D_80125300 == 0) {
        D_80094730 += 1;
        func_80011DFC();
        func_80011DFC();
        D_800EECBC = 0;
    }
    func_80011DFC();
}

s32 func_80116360(void) {
    if ((D_800B1746 == 0) && (D_80094730 == 0xA) && (D_800EECBC == 0)) {
        func_80011DFC();
        return 0;
    }
    if ((D_800B1746 == 1) || (D_800B1746 == 4) || (D_800B1746 == 9)) {
        if ((D_80094730 == 6) && (D_800EECBC == 0)) {
            func_80011DFC();
            return 0;
        }
    }
    if ((D_800B1746 == 0xA) && (D_80094730 == 6) && (D_800EECBC == 0)) {
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
        return 0;
    }
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
}

void func_80116460(void) {
    u8 temp_v0;

    temp_v0 = func_8002328C(D_800B1746);
    if ((D_800EECBC == 0) && ((D_800B1746 == 0) || (D_800B1746 == 7)) && (D_8012531C == 0)) {
        D_80094730 = D_80094730 + ((temp_v0 & 0x7F) >= 2U) + ((temp_v0 & 0x7F) >= 3U);
    } else {
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
    }
    func_80011DFC();
}

void func_8011651C(void) {
    u32 t;

    t = (u8) func_8002328C(D_800B1746) & 0x7F;
    D_80094730 = ((D_80094730 - (t >= 2U)) - (t >= 3U)) + 2;
    func_80011DFC();
}

void func_80116578(void) {
    func_80011DFC();
}

void func_80116598(void) {
    if (D_800B0956 < 0x78) {
        D_80094730 += 2;
    }
    func_80011DFC();
}

void func_801165DC(void) {
    if (D_8012531C == 5) {
        func_80118B4C();
        D_800EECBC = 0;
        return;
    }
    func_80011DFC();
}

void func_80116620(void) {
    u8 v;

    v = func_8002328C(D_800B1746) & 0x7F;
    if (D_8012531C == 5) {
        D_80094730 = D_80094730 + (v >= 2U) + (v >= 3U);
        func_80011DFC();
        D_800B1AF6 += 8;
        return;
    }
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010FE10", func_801166A8);

void func_80116888(void) {
    u32 v;

    if (D_80125328 == 0) {
        D_80094730 = 9;
        v = D_800B1746;
        if (v == 7) {
            D_80094730 = 0xA;
        } else if (v == 2 || v == 9) {
            D_80094730 = 8;
        } else if (v == 4 || v == 8 || v == 5 || v == 0xA) {
            D_80094730 = 7;
        }
        D_800B1AF6 += 0x14;
    }
    func_80011DFC();
}

void func_80116940(void) {
    EventStateView *g;

    g = (EventStateView *)D_800B0860;
    if (D_800EECBC == 1) {
        /* FAKE: evaluate cvt(D_800B1746) into $v0 before D_80094730 = 9; T-11110 */
        if (D_800B1746 + 0) {
        }
        D_80094730 = 9;
        if (D_800B1746 == 7) {
            D_80094730 = 0xA;
        } else if (D_800B1746 == 2 || D_800B1746 == 9) {
            D_80094730 = 8;
        } else if (D_800B1746 == 4 || D_800B1746 == 8 || D_800B1746 == 5 || D_800B1746 == 0xA) {
            D_80094730 = 7;
        }
        D_800B1AF6 += 0x11;
    } else if (D_800B1746 == 4 || D_800B1746 == 8 || D_800B1746 == 5 || D_800B1746 == 0xA) {
        D_800B1AF6 += 5;
    } else if (D_800B1746 == 9 && g->unk_5CA[0x3A] == 0) {
        D_80094730 += 1;
        D_800B1AF6 += 5;
    } else if (D_800B1746 == 9 && g->unk_5CA[0x3A] != 0) {
        D_800B1AF6 += 0x11;
    }
    func_80011DFC();
}

s32 func_80116AA8(void) {
    /* FAKE: unused word reserves top frame slot so s16 before sits at sp+0x2A; T-11110 */
    s32 pad;
    s16 before;

    if (D_800EECBC == 1) {
        if (D_800B1746 == 7) {
            before = D_800B1AF6;
            func_8004CF30();
            if (before != D_800B1AF6) {
                D_80094730 = 0xA;
                D_800B1AF6 += 0xB;
            }
            return 0;
        }
        D_80094730 = 9;
        if (D_800B1746 == 2) {
            D_80094730 = 8;
        } else if ((D_800B1746 == 4) || (D_800B1746 == 8) || (D_800B1746 == 5)) {
            D_80094730 = 7;
        }
        /* FAKE: dead test gives cvt(D_800B1AF6) a second use in this block so uopt allocates $v0; T-11110 */
        if ((s32)D_800B1AF6) {
        }
        D_800B1AF6 += 0xC;
    } else {
        D_8012530C = 1;
    }
    func_80011DFC();
}

void func_80116B94(void) {
    EventStateView *g;

    g = (EventStateView *)D_800B0860;
    func_80011DFC();
    g->unk_1296 += 0x16;
    ((EventResultRow6 *)&D_8012439C)->value[0] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125260 + D_8012531C * 3 + 30);
    ((EventResultRow6 *)&D_8012439C)->value[1] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125260 + D_8012531C * 3 + 31);
    ((EventResultRow6 *)&D_8012439C)->value[2] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125260 + D_8012531C * 3 + 32);
    ((EventResultRow6 *)&D_8012438C)->value[0] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125264 + D_8012531C * 3 + 30);
    ((EventResultRow6 *)&D_8012438C)->value[1] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125264 + D_8012531C * 3 + 31);
    ((EventResultRow6 *)&D_8012438C)->value[2] = (s16) *(g->unk_EE6 * 0x24 + (u8 *)D_80125264 + D_8012531C * 3 + 32);
    func_80114B0C();
}

void func_80116CB4(void) {
    if (D_80125304 != 0) {
        D_80094730 += 0x10;
    }
    if (D_80125308 != 0) {
        D_80094730 += 0x10;
    }
    func_80011DFC();
}

void func_80116D18(void) {
    if (D_800EECBC == 1) {
        D_800EECBC = D_800EECC8;
        D_80125310 = 0;
        func_80011DFC();
        D_800B1AF6 += 0xC;
        return;
    }
    if (D_800B0A04[D_800B1746].unk_2D != 0) {
        D_8012437C += 1;
    }
    func_80011DFC();
}

void func_80116DC0(void) {
    if (D_800B0A04[D_800B1746].unk_2D == 0) {
        D_8012437C += 1;
    }
    func_80011DFC();
}

void func_80116E20(void) {
    D_800B0860[D_800B1746 * 0x34 + 0x1D1] += 1;
    func_80011DFC();
    D_800B1AF6 += 7;
}

void func_80116E80(void) {
    D_80094730 += D_8012531C * 2;
    func_80011DFC();
}

void func_80116EBC(void) {
    func_80078970(D_800947C4, "遊園地内");
    func_80011DFC();
}

void func_80116EF0(void) {
    switch (D_800B1746) {
    case 5:
    case 9:
        if (D_8012439C == 3) {
            D_80094730 += 1;
        } else if (D_8012439C == 2) {
            D_80094730 += 2;
        }
        break;
    case 7:
        if (D_8012439C == 4) {
            D_80094730 += 1;
        } else if (D_8012439C == 3) {
            D_80094730 += 2;
        }
        break;
    default:
        if (D_8012439C == 4) {
            D_80094730 += 1;
        } else if (D_8012439C == 2) {
            D_80094730 += 2;
        }
        break;
    }
    func_80011DFC();
}

void func_80117004(void) {
    /* FAKE: (u32) casts give D_8012439C a CVT so uopt puts the register-held constant 3 first in bne; T-11110 */
    switch (D_800B1746) {
    case 0:
    case 1:
    case 7:
        if ((u32)D_8012439C == 4) {
            D_80094730 += 1;
        } else if ((u32)D_8012439C == 2) {
            D_80094730 += 2;
        }
        break;
    case 2:
    case 3:
    case 8:
    case 10:
        if ((u32)D_8012439C == 4) {
            D_80094730 += 1;
        } else if ((u32)D_8012439C == 3) {
            D_80094730 += 2;
        }
        break;
    case 4:
    case 6:
    case 9:
        if (((u32)D_8012439C == 3) && ((u32)D_8012439C == 2)) {
            D_80094730 += 1;
        } else if (((u32)D_8012439C == 3) && ((u32)D_8012439C == 3)) {
            D_80094730 += 2;
        }
        break;
    case 5:
        if ((u32)D_8012439C == 3) {
            D_80094730 += 1;
        } else if ((u32)D_8012439C == 2) {
            D_80094730 += 2;
        }
        break;
    }
    func_80011DFC();
}

/* The record index is read through the EventStateView field (D_800B1746 is its unk_EE6): an indirect load, so
 * IDO reloads it after the compare chain instead of keeping D_800B1746 in a register, as the original does. */
void func_80117178(void) {
    if (D_800B0895 == 0x5F) {
        if ((D_800B1746 == 2) || (D_800B1746 == 8) || (D_800B1746 == 0xA)) {
            D_80094730 = 1;
        }
        *(s16 *)(D_800B0860 + ((EventStateView *)D_800B0860)->unk_EE6 * 0x34 + 0x1A6) += 1;
        func_80011DFC();
    } else if (D_800B0895 == 0x60) {
        if ((D_800B1746 == 2) || (D_800B1746 == 8) || (D_800B1746 == 0xA)) {
            D_80094730 = 4;
        } else {
            D_80094730 = 6;
        }
        func_80011EC4(0x14);
    } else {
        if ((D_800B1746 == 2) || (D_800B1746 == 8) || (D_800B1746 == 0xA)) {
            D_80094730 = 9;
        } else {
            D_80094730 = 0xB;
        }
        *(s16 *)(D_800B0860 + ((EventStateView *)D_800B0860)->unk_EE6 * 0x34 + 0x1A6) += 1;
        func_80011EC4(0x1F);
    }
    func_8004C250();
}

void func_801172E4(void) {
    func_80011EC4(0x32);
}

void func_80117304(void) {
    s32 offset;
    u8 *first;
    u8 *second;

    offset = D_800B1746 * 3;
    /* The canonical word globals contain byte-table addresses. */
    first = (u8 *)(offset + D_801251F0);
    D_8012439C = (s16) first[0];
    D_8012439E = (s16) first[1];
    D_801243A0 = (s16) first[2];
    second = (u8 *)(offset + D_801251F4);
    D_8012438C = (s16) second[0];
    D_8012438E = (s16) second[1];
    D_80124390 = (s16) second[2];
    func_80114B0C();
}

void func_80117390(void) {
    D_80094730 += D_801243AC;
    switch (D_801243AC) {
    case 0:
        D_800B096E += 5;
        break;
    case 1:
        D_800B094E += 5;
        break;
    case 2:
        *(s16 *)(D_800B0860 + D_800B1746 * 0x34 + 0x1A6) -= 1;
        break;
    case 3:
        *(s16 *)(D_800B0860 + D_800B1746 * 0x34 + 0x1A6) += 1;
        break;
    }
    func_8004C250();
    func_80011DFC();
}

void func_8011749C(void) {
    D_800EECC8 = D_800EECBC;
    if (D_800B0896 == 8) {
        D_80125310 = 1;
        D_8012437C = 0;
        D_80124380 = 0;
        func_8004A8EC(func_8002336C(D_800B1746));
        func_80033E88();
        func_80011DFC();
        return;
    }
    func_80011DFC();
    D_800B1AF6 += 0x10;
}

/* s32 with no return (K&R): the decremented byte stays in $v0 as the return register. */
s32 func_80117538(void) {
    /* FAKE: unused word reserves top frame slot so before sits at sp+0x28; T-11360 */
    s32 pad;
    s32 before;

    /* FAKE: dead load at entry lets the cvt pass keep D_800B1AF6 in $v0; T-11360 */
    before = D_800B1AF6;
    switch (D_800EECC4) {
    case 0:
        before = D_800B1AF6;
        func_80046600();
        /* FAKE: (s32) cast gives the original operand order of the beq; T-11360 */
        if (before != (s32)D_800B1AF6) {
            D_800B1AF6 -= 1;
            D_800EECC4 += 1;
        }
        break;
    case 1:
        before = D_800B1AF6;
        func_800464E8();
        if (before != (s32)D_800B1AF6) {
            D_800B1AF6 -= 1;
            D_800EECC4 += 1;
        }
        break;
    default:
        func_80011DFC();
        break;
    }
}

void func_8011760C(void) {
    u8 v;

    v = func_8002328C(D_800B1746) & 0x7F;
    D_80094730 = ((v >= 2U) + (v >= 3U)) * 2;
    D_80094734 = 0;
    D_8012439E = 0;
    D_8012439C = 0;
    D_801243A0 = 0;
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010FE10", func_8011767C);

s32 func_80117A10(void) {
    if ((D_801252E4 != 0) || (D_801243B4 == 1)) {
        func_80011DFC();
        func_80011DFC();
        return 0;
    }
    return func_80117A68();
}

s32 func_80117A68(void) {
    u32 r;

    D_8009471C = (u8 *)&D_80124344;
    D_80094720 = (u8 *)&D_80124348;
    D_80094724 = D_8012425C;
    D_80094728 = D_801242C8;
    D_8009472C = D_80124334;
    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    r = (u8)func_8002328C(D_800B1746) & 0x7F;
    if (D_801252E4 == 2 && D_80124348 == 2 && r >= 2U) {
        func_8004A8EC(3);
        func_80033E88();
    }
    return func_80049B20(D_800EECBC, 1, 0);
}

s32 func_80117B30(void) {
    if (D_800B1AF0++ == 0) {
        if (D_800B0896 == 3 && D_800B0897 == 0x15) {
            D_8012434C = 0;
            goto block_26;
        }
        if (D_800B0895 == 0x5F && D_800B0896 == 7 && D_800B0897 == 0x18) {
            D_8012434C = 1;
            goto block_26;
        }
        if (D_800B0895 == 0x60 && D_800B0896 == 7 && D_800B0897 == 0x1C) {
            D_8012434C = 1;
            goto block_26;
        }
        if (D_800B0895 == 0x61 && D_800B0896 == 7 && D_800B0897 == 0x1B) {
            D_8012434C = 1;
            goto block_26;
        }
        if (D_800B0896 == 9 && D_800B0897 == 0x17) {
            if (D_800B1AE4 & 1) {
                D_8012434C = 2;
            } else {
                D_8012434C = 3;
            }
            goto block_26;
        }
        if (D_800B0896 == 0xC && D_800B0897 == 0x17) {
            D_8012434C = 4;
            goto block_26;
        }
        func_80011DFC(D_800B0895);
        return 0;
    }
block_26:
    D_8009471C = (u8 *)&D_8012434C;
    D_80094720 = (u8 *)&D_80124350;
    D_80094724 = D_80124260;
    D_80094728 = D_801242CC;
    D_8009472C = D_80124338;
    return func_80049B20(D_800EECBC, 1, 0);
}

void func_80117CEC(void) {
    s16 sp2E;

    D_8009471C = (u8 *) &sp2E;
    D_80094720 = (u8 *) &(&D_8012439C)[D_800EECBC];
    D_80094724 = D_801241D8;
    D_80094728 = D_801241E8;
    sp2E = 0;
    D_8009472C = D_801241F8;
    if (func_80049B20(D_800EECBC, 0, 0) != 0) {
        func_80011DFC();
    }
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010FE10", func_80117D80);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010FE10", func_80118010);

s32 func_801182B8(void) {
    u8 x;
    u32 t;

    x = func_8002328C(D_800B1746);
    if (D_800B1AF9++ == 0) {
        t = x & 0x7F;
        D_8012435C = (t >= 2U) + (t >= 3U) + (t >= 4U);
    }
    D_8009471C = (u8 *) &D_8012435C;
    D_80094720 = (u8 *) &D_80124360;
    D_80094724 = D_80124268;
    D_80094728 = D_801242D4;
    D_8009472C = D_80124340;
    return func_80049B20(D_800EECBC, 1, 0);
}

void func_8011838C(void) {
    D_8009471C = (u8 *) &D_80124364;
    D_80094720 = (u8 *) &D_80124368;
    D_80094724 = D_80124264;
    D_80094728 = D_801242D0;
    D_8009472C = D_8012433C;
    if (func_80049B20(D_800EECBC, 0, 0) != 0) {
        func_80011DFC();
    }
}

void func_80118418(void) {
    if (D_800B1AF0++ == 0) {
        D_80124370 += D_80125314;
    }
    D_8009471C = &D_8012436C;
    D_80094720 = (u8 *)&D_80124370;
    D_80094724 = D_801241DC;
    D_80094728 = D_801241EC;
    D_8009472C = D_801241FC;
    if (func_80049B20(D_800EECBC, 0, 0) != 0) {
        func_80011DFC();
    }
}

void func_801184DC(void) {
    if (D_800B1AF0++ == 0) {
        D_80124378 += D_80125318;
    }
    D_8009471C = &D_80124374;
    D_80094720 = (u8 *)&D_80124378;
    D_80094724 = D_801241D0;
    D_80094728 = D_801241E0;
    D_8009472C = D_801241F0;
    if (func_80049B20(D_800EECBC, 0, 0) != 0) {
        func_80011DFC();
    }
}

void func_801185A0(void) {
    D_8009471C = &D_8012437C;
    D_80094720 = &D_80124380;
    D_80094724 = D_80124224;
    D_80094728 = D_80124290;
    D_8009472C = D_801242FC;
    func_80049B20(0xFF, 1, 0);
}

void func_80118618(void) {
    D_8009471C = &D_80124384;
    D_80094720 = &D_80124388;
    D_80094724 = D_801241D4;
    D_80094728 = D_801241E4;
    D_8009472C = D_801241F4;
    func_80049B20(0xFF, 1, 0);
}

void func_80118690(void) {
    D_8009471C = (u8 *)&D_80094730;
    if (D_800B1746 == 0 && (D_800B0940 >> 0x1C) == 6 && D_8012531C == 0 && D_80094734 == 0) {
        D_80094734 += 1;
    }
    D_80094720 = (u8 *)&D_80094734;
    D_80094724 = D_8009473C;
    D_80094728 = D_80094740;
    D_8009472C = D_80094744;
    if (func_80049B20(D_800EECBC, 0, 0) != 0) {
        func_80011DFC();
    }
}

void func_80118764(void) {
    if (D_800EECBC == 2) {
        if ((D_800B1746 == 2) || (D_800B1746 == 3)) {
            func_80011DFC();
            return;
        }
        if (D_800B1746 == 8) {
            func_8004A7F4();
            return;
        }
        if ((D_800B1746 == 5) && (D_800B0A04[5].unk_17 == 0)) {
            func_80011DFC();
            return;
        }
        if ((D_800B1746 == 5) && (D_800B0A04[5].unk_17 != 0)) {
            D_80094730 += 1;
            func_8004A7C4();
            return;
        }
        func_8004A7C4();
        return;
    }
    func_80011DFC();
}

void func_80118848(void) {
    func_80011DFC();
}

#ifdef NON_MATCHING
/* NON_MATCHING: T-11110, 8 lines: chained halfword store scheduling and $v0 vs $t6 for third chain */
void func_80118868(void) {
    if (D_800B1746 != 6) {
        func_80011DFC();
        D_800B1AF6 += 8;
        return;
    }
    if (D_800B095E >= 0x96) {
        D_80094730 += 3;
        func_80011DFC();
        D_800B1AF6 += 8;
        return;
    }
    (&D_801243A4)[0] = (&D_8012439C)[2] = (&D_8012439C)[0] = (&D_8012439C)[3] = 2;
    (&D_8012438C)[0] = (&D_8012438C)[1] = (&D_8012438C)[2] = 3;
    (&D_80124394)[0] = (&D_80124394)[1] = (&D_80124394)[2] = 3;
    func_80011DFC();
}
#else
INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010FE10", func_80118868);
#endif

void func_80118950(void) {
    if (D_801243B0 == 0x12) {
        func_80118A60();
        func_80011DFC();
    } else if (D_801243B0 == 0x14) {
        func_801189B4();
        func_80011DFC();
    }
    func_80011DFC();
}

void func_801189B4(void) {
    switch (D_800B1746) {
    case 0:
        func_800335E0(0x55BA);
        return;
    case 4:
        func_800335E0(0x64D8);
        return;
    case 5:
        func_800335E0(0x5E84);
        return;
    case 6:
        func_800335E0(0x60CD);
        return;
    case 7:
        func_800335E0(0x5B5A);
        return;
    case 8:
        func_800335E0(0x50CE);
        return;
    default:
        func_800335E0(0x6721);
        return;
    }
}

void func_80118A60(void) {
    switch (D_800B1746) {
    case 0:
        func_800335E0(0x5614);
        break;
    case 1:
        func_800335E0(0x5C68);
        break;
    case 2:
        func_800335E0(0x51DC);
        break;
    case 3:
        func_800335E0(0x593E);
        break;
    case 4:
        func_800335E0(0x6532);
        break;
    case 5:
        func_800335E0(0x5DD0);
        break;
    case 6:
        func_800335E0(0x6127);
        break;
    case 7:
        func_800335E0(0x5BE1);
        break;
    case 8:
        func_800335E0(0x4F93);
        break;
    case 9:
        func_800335E0(0x6316);
        break;
    case 10:
        func_800335E0(0x666D);
        break;
    }
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010FE10", func_80118B4C);

void func_80118DAC(s16 arg0, s16 *arg1, s16 *arg2) {
    s16 i;
    s16 j;
    s32 row;
    EventTableE7 table;

    table = D_8012536C;
    if (arg0 > 0) {
        i = 0;
positive_loop:
        if (D_800B0962 >= (0x64 - (i * 0x19))) {
            *arg1 = 5 - i;
        } else {
            i += 1;
            if (i < 5) {
                goto positive_loop;
            }
        }
        if (D_800B0962 == 0) {
            *arg1 = 0;
        }
        row = D_800B1746;
        *arg2 = 5 - table.unk_00[row][D_801243B0];
        return;
    }
    j = 0;
negative_loop:
    if (D_800B0962 >= (0x64 - (j * 0x19))) {
        *arg1 = j + 3;
    } else {
        j += 1;
        if (j < 5) {
            goto negative_loop;
        }
    }
    if (D_800B0962 == 0) {
        *arg1 = 8;
    }
    row = D_800B1746;
    *arg2 = table.unk_00[row][D_801243B0] + 3;
}

void func_80118F40(void) {
    func_8004A8EC((&D_8012438C)[D_800EECBC]);
    func_80011DFC();
}

void func_80118F80(void) {
    func_8004A8EC((&D_80124394)[D_800EECBC]);
    func_80033E88();
    func_80011DFC();
}

void func_80118FC8(void) {
    func_8004A8EC((&D_80124394)[D_800EECBC]);
    func_80011DFC();
}

void func_80119008(void) {
    if (D_800B1746 == 2) {
        func_8004A7C4();
        return;
    }
    if ((D_800B1746 == 1) && (D_800B0A04[1].unk_2D == 0)) {
        func_8004A734();
        return;
    }
    func_8004A764();
}

void func_80119074(void) {
    if ((D_800B0F32 == D_800B0F31) && (D_800B0F33 == D_800B0F31) && (D_800B0F34 == D_800B0F31) && ((u8) D_800B0895 >= 0x60U)) {
        func_801190F0();
        return;
    }
    func_80011DFC();
}

void func_801190F0(void) {
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010FE10", func_80119110);

void func_80119370(void) {
    if ((((EventStateView *)&D_800B0860)->unk_036 == ((EventBirthday4 *)&D_800B0A04[D_800B1746].pad10[0])->month) &&
        (D_800B0897 == ((EventBirthday4 *)&D_800B0A04[D_800B1746].pad10[0])->day)) {
        func_80011E28(0x50);
        func_80011E8C(2);
        return;
    }
    if ((D_800B0896 == ((EventBirthday4 *)&D_800B0948)->month) &&
        (D_800B0897 == ((EventBirthday4 *)&D_800B0948)->day)) {
        func_80011E28(0x50);
        func_80011E8C(3);
        return;
    }
    func_80011DFC();
}

void func_80119440(void) {
    s32 one; /* FAKE: keeps the constant 1 in a register shared by the compare and the store (permuter) */

    if (1 == D_800B1746) {
        if ((one = 1) == D_8012531C) {
            if (D_800B0896 >= 6 && D_800B0896 < 9) {
                D_80094730 += 9;
                D_8012439C = one;
                D_8012439E = 3;
                D_801243A0 = 2;
                D_8012438C = 2;
                D_8012438E = 3;
                D_80124390 = 3;
            }
        } else if (D_8012531C == 2) {
            D_80094730 += 9;
        }
    }
    func_80011DFC();
}

void func_80119518(void) {
    u8 sel; /* local copy of the selector: the original compares the register as the first operand (T-10400) */

    if (D_800B1746 == 1) {
        sel = D_8012531C;
        if (sel == 0) {
            D_80094730 = 4;
        }
        if (sel == 1) {
            D_80094730 = 7;
        }
        if (sel == 2) {
            D_80094730 = 0xA;
        }
        if ((D_800B0896 >= 6) && (D_800B0896 < 9)) {
            D_80094730 = 0;
            D_8012439C = 1;
            D_8012439E = 4;
            D_801243A0 = 2;
            D_8012438C = 2;
            D_8012438E = 3;
            D_80124390 = 3;
        }
        func_80114B0C();
        return;
    }
    func_80011DFC();
}

s32 func_80119600(void) {
    if ((D_800B1746 == 1) && (D_800B0896 >= 6) && (D_800B0896 < 9) && (D_800EECBC == 1)) {
        func_80011DFC();
        return 0;
    }
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
}

void func_80119688(void) {
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
}

s32 func_801196B8(void) {
    /* FAKE: label stops cvt_pass entry strip so uopt puts 1 ($a0) first in bne; T-11110 */
entry:;
    if ((D_800B1746 == 1) && (*(u8 *)&D_80125320 == 3)) {
        if (((u8)D_800B0E31 == 0) && ((D_800B0896 < 3) || (D_800B0896 >= 6))) {
            D_80094730 += 1;
            (&D_8012439C)[0] = (&D_8012439C)[1] = (&D_8012439C)[2] = 3;
            (&D_8012438C)[0] = (&D_8012438C)[1] = (&D_8012438C)[2] = 3;
            func_80114B0C();
            func_8004A8EC(3);
            func_80033E88();
            return 0;
        }
        if ((u8)D_800B0E31 == 1) {
            (&D_8012439C)[0] = (&D_8012439C)[1] = (&D_8012439C)[2] = 4;
            (&D_8012438C)[0] = (&D_8012438C)[1] = (&D_8012438C)[2] = 4;
            func_80114B0C();
            func_8004A8EC(4);
            func_80033E88();
            return 0;
        }
    } else if ((D_800B1746 == 8) && (*(u8 *)&D_80125320 == 2) && (D_800B0BCE != 0)) {
        D_80094730 += 3;
    }
    func_80011DFC();
}

void func_80119824(void) {
    if (D_800B1746 == 8 && (u32)(u8)D_80125320 == 3 && D_800B0BCF != 0) {
        D_80094734 = 2;
    }
    func_8004CF30();
    if (D_800B1746 == 8 && (u32)(u8)D_80125320 == 3) {
        if (D_800B0BCF == 0 && D_80094734 == 3) {
            func_800201FC(1);
            D_80094730 += 1;
            D_80094734 = 0;
            func_80011DFC();
        }
    }
}

void func_801198E8(void) {
    s32 v2;
    u8 v;

    /* FAKE: dead load before the first label lets cvt_pass strip the CVT of D_80125320, so it goes first in bne; T-11600 */
    v = D_80125320;
    /* FAKE: label stops cvt_pass entry strip so uopt puts 1 ($a0) first in bne; T-11110 */
entry:;
    if ((D_800B1746 == 1) && (D_80125320 == 3)) {
        if (((u8)D_800B0E31 == 0) && ((D_800B0896 < 3) || (D_800B0896 >= 6))) {
            D_800B1AF6 += 0xD;
        }
        if ((u8)D_800B0E31 == 1) {
            D_800B1AF6 += 0xD;
        }
    } else if ((D_800B1746 == 4) && (D_80125320 == 3) && (D_800B0AFF != 0)) {
        D_800B1AF6 += 6;
    } else if ((D_800B1746 == 7) && (D_80125320 == 1)) {
        v = func_8002328C(7);
        v2 = func_8002336C(7);
        if ((v == 0x80) && (v2 == 0)) {
            D_800B1AF6 += 0xA;
        } else {
            D_800B1AF6 += 0xB;
        }
    } else if ((D_800B1746 == 8) && (D_80125320 == 2) && (D_800B0BCE != 0)) {
        D_800B1AF6 += 6;
    }
    func_80011DFC();
}

void func_80119A60(void) {
    if (D_800B1746 == 2) {
        func_8004A8EC(4);
        func_80033E88();
        D_80094730 = 5;
        D_800B0A04[0].unk_02 += 1;
        D_800B0A04[0].unk_06 += 1;
        D_800B0A04[0].unk_0A -= 0x14;
        func_8004C250();
        func_80011EC4(0x5C);
        return;
    }
    func_801103A0();
}

void func_80119AFC(void) {
    if ((D_800B1746 == 2) || (D_800B1746 == 8) || (D_800B1746 == 5) || (D_800B1746 == 0xA)) {
        D_800B1AF6 += 5;
    }
    func_80011DFC();
}

void func_80119B58(void) {
    func_80011EC4(0x3A);
}

/* s32 with no return (K&R): the decremented copy `cur` stays in $v0 as the return register. */
s32 func_80119B78(void) {
    /* FAKE: unused word reserves top frame slot so before sits at sp+0x28; T-11600 */
    s32 pad;
    s32 before;
    u8 cur;

    if ((D_800B1746 == 6) && (D_800B0A04[6].unk_17 != 0) && (D_800EECBC == 2)) {
        D_80094730 = 0;
        D_800B1AF6 -= 5;
        func_80011DFC();
        return;
    }
    if ((D_800B1746 == 8) && (D_800EECBC == 2)) {
        before = D_800B1AF6;
        func_8004CF30();
        if (before != D_800B1AF6) {
            cur = D_800B1AF6;
            D_80094730 = 0;
            D_800B1AF6 = cur -= 5;
        }
    }
}

void func_80119C40(void) {
    if ((D_800B1746 == 6) && (D_800B0A04[6].unk_17 != 0) && (D_800EECBC == 2)) {
        D_800EECBC = 3;
    }
    func_80011DFC();
}

void func_80119C9C(void) {
    (&D_80124394)[0] = (&D_80124394)[1] = (&D_80124394)[2] = 3;
    (&D_8012439C)[0] = (&D_8012439C)[1] = (&D_8012439C)[2] = 4;
    (&D_801243A4)[0] = (&D_801243A4)[1] = (&D_801243A4)[2] = 2;
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
}

void func_80119D2C(void) {
    if ((D_800B1746 == 7) && (D_800EECBC == 0) && (D_8012531C == 0)) {
        func_80011DFC();
        return;
    }
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
}

s32 func_80119DA0(void) {
    if ((D_800B1746 == 7) && (D_800EECBC == 1) && ((D_8012531C == 2) || (D_8012531C == 0))) {
        func_8004A7F4();
        return 0;
    }
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
}

void func_80119E2C(void) {
    if (D_8012531C == 0) {
        func_8004A7F4();
        return;
    }
    func_80118B4C();
}

void func_80119E68(void) {
    if ((D_800B1746 == 7) && (D_8012531C == 0)) {
        D_80094730 += D_800EECBC;
        func_80011DFC();
        return;
    }
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
}

void func_80119EDC(void) {
    if ((D_800B1746 == 7) && (D_8012531C == 0)) {
        func_80011DFC();
        func_80011DFC();
        func_80011DFC();
        return;
    }
    func_80011DFC();
}

void func_80119F40(void) {
    if ((D_800B1746 == 7) && (D_800EECBC == 0) && (D_8012531C == 1)) {
        func_8004CF30();
        return;
    }
    func_80011DFC();
}

void func_80119FA4(void) {
    if ((D_800B1746 == 7) && (D_800EECBC == 0) && (D_8012531C == 1)) {
        func_8004A7F4();
        return;
    }
    func_80011DFC();
}

void func_8011A008(void) {
    if ((D_800B1746 == 7) && (D_800EECBC == 0) && (D_8012531C == 1)) {
        func_8004A764();
        return;
    }
    func_80011DFC();
}

void func_8011A06C(void) {
    if ((D_800B1746 == 7) && (D_800EECBC == 2) && (D_8012531C == 1)) {
        func_8004CF30();
        return;
    }
    func_80011DFC();
}

void func_8011A0D0(void) {
    func_8004CF30();
}

void func_8011A0F0(void) {
    if ((D_800B1746 == 8) && (D_800B0A04[8].unk_22 != 0)) {
        D_800B1AF6 += 0x19;
    }
    func_80011DFC();
}

void func_8011A144(void) {
    if (D_800B1746 == 5 && D_8012531C == 0 && D_800EECBC == 2) {
        func_8004CF30();
    }
    if (D_800B1746 == 5 && D_8012531C == 1 && D_800EECBC == 0) {
        func_8004CF30();
        return;
    }
    func_80011DFC();
}

void func_8011A1E4(void) {
    if (D_800B1746 == 5 && D_8012531C == 0 && D_800EECBC == 2) {
        if (D_800B1AF0++ == 0) {
            func_8004A8EC(3);
            func_80033E88();
        }
        func_8004CF30();
        return;
    }
    func_80011DFC();
}

void func_8011A270(void) {
    if ((D_800B1746 == 9) && (D_800EECBC == 2)) {
        func_8004CF30();
        return;
    }
    func_80011DFC();
}

void func_8011A2C4(void) {
    if ((D_800B1746 == 0xA) && (D_800EECBC == 1) && (D_8012531C == 1)) {
        func_8004CF30();
        return;
    }
    func_80011DFC();
}

void func_8011A328(void) {
    if (D_800B1746 == 0xA && D_800EECBC == 0 && D_8012531C == 0) {
        func_8004CF30();
    }
    if (D_800B1746 == 0xA && D_800EECBC == 0 && D_8012531C == 1 && D_800B0956 < 0x78) {
        func_8004CF30();
        return;
    }
    func_80011DFC();
}

void func_8011A3DC(void) {
    if ((D_800B1746 == 0xA) && (D_800EECBC == 2) && (D_8012531C == 4)) {
        func_8004CF30();
        return;
    }
    func_80011DFC();
}

void func_8011A440(void) {
    if ((D_800B1746 == 0xA) && (D_800EECBC == 0) && (D_8012531C == 0)) {
        func_8004CF30();
        return;
    }
    func_80011DFC();
}

void func_8011A4A4(void) {
    if ((D_800B1746 == 0xA) && (D_800EECBC == 1) && (D_8012531C == 1)) {
        func_8004CF30();
        return;
    }
    func_80011DFC();
}

void func_8011A508(void) {
    if (D_800B1746 == 0xA) {
        if (D_800EECBC == 1) {
            if ((u8)D_80125320 == 2) {
                if (D_8012531C == 1) {
                    if (D_800B1AF0++ == 0) {
                        D_80094730 = 0x37;
                    }
                    func_8004CF30();
                    return;
                }
            }
        }
    }
    func_80011DFC();
}

void func_8011A5A0(void) {
    if ((D_800B1746 == 5) || (D_800B1746 == 7)) {
        func_8004A764();
        return;
    }
    func_8004A734();
}

void func_8011A5EC(void) {
    if (D_800B1746 == 9) {
        func_8004A734();
        return;
    }
    func_8004A764();
}

void func_8011A62C(void) {
    switch (D_800B0F30) {
    case 0:
        func_8010BC54();
        func_80015D28(0x17, 0x80198000, 0x9ED8);
        break;
    case 1:
        func_8010C168();
        func_80015D28(0x18, 0x80198000, 0x9F31);
        break;
    case 2:
        func_8010C64C();
        func_80015D28(0x15, 0x80198000, 0x9EEF);
        break;
    case 3:
        func_8010CB30();
        func_80015D28(0x16, 0x80198000, 0x9F04);
        break;
    case 4:
        func_8010D014();
        func_80015D28(0x16, 0x80198000, 0x9F8A);
        break;
    case 5:
        func_8010D528();
        func_80015D28(0x17, 0x80198000, 0x9F49);
        break;
    case 6:
        func_8010DA3C();
        func_80015D28(0x15, 0x80198000, 0x9F60);
        break;
    case 7:
        func_8010DF50();
        func_80015D28(0x17, 0x80198000, 0x9F1A);
        break;
    case 8:
        func_8010E464();
        func_80015D28(0x13, 0x80198000, 0x9EC5);
        break;
    case 9:
        func_8010E948();
        func_80015D28(0x15, 0x80198000, 0x9F75);
        break;
    default:
        func_8010EE2C();
        func_80015D28(0x16, 0x80198000, 0x9FA0);
        break;
    }
    func_80011DFC();
}

void func_8011A7FC(void) {
    func_80015D28(2, 0x80197000, 0xA0E0);
    func_8010BB90();
    func_80011DFC();
}

void func_8011A838(void) {
    func_80015D28(3, 0x801A4000, 0xA143);
    func_8010FE10();
    func_80011DFC();
}

s32 func_8011A874(void) {
    if (D_8012531C == 1) {
        switch (D_800B1746) {
        case 0:
        case 3:
        case 4:
        case 5:
        case 8:
        case 9:
            func_80011DFC();
            return 0;
        }
    }
    return func_80045FF4();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010FE10", func_8011A8E4);

typedef struct {
    u16 v[21];
} U16x21; /* size 0x2A */
extern U16x21 D_801254D8;

void func_8011AB00(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    U16x21 tbl;

    tbl = D_801254D8;
    if (func_8001496C() == 1) {
        idx = D_801243B0;
        func_800433D0(tbl.v[idx]);
        func_80011DFC();
    }
}

void func_8011AB94(void) {
    if ((u8) D_800B1AF6 >= 0x14U) {
        func_800433D0(0x203);
    } else if (D_80125324 == 0) {
        func_800433D0(0x200);
    } else if (D_80125324 == 1) {
        func_800433D0(0x201);
    } else {
        func_800433D0(0x202);
    }
    func_80011DFC();
}

s32 func_8011AC18(void) {
    /* FAKE: unused word reserves top frame slot so EventTable2D6 tbl sits at sp+0x2C in a 0x308 frame; T-11110 */
    s32 pad;
    EventTable2D6 tbl;

    tbl = D_80125504;
    if (((D_800B0F2D == D_800B0F2C) && (D_801252E4 == 1)) || (D_800B0F2E == 0xFF)) {
        D_800B1AF6 += 0x11;
        return;
    }
    if (D_800B0F2E == D_800B0F2C) {
        func_80078970(D_80094818, tbl.unk_00[D_800B0F2D]);
    } else {
        func_80078970(D_80094818, tbl.unk_00[(s32)D_800B0F2E]);
    }
    if (D_800B0F2F != 0xFF) {
        D_80124384 = 3;
        if (D_800B0F2F == D_800B0F2C) {
            func_80078970(D_8009483C, tbl.unk_00[D_800B0F2D]);
        } else {
            func_80078970(D_8009483C, tbl.unk_00[(s32)D_800B0F2F]);
        }
    }
    func_80011DFC();
}

void func_8011AD7C(void) {
    u8 v;
    u8 sel;

    /* FAKE: dead load before the first branch lets cvt_pass strip the CVTs of D_801252E4, so uopt keeps it in $a1; T-11600 */
    sel = D_801252E4;
    if ((D_800B0F2D == D_800B0F2C) && (D_800EECBC == 0)) {
        D_801243B4 = 1;
        D_80124344 += 1;
        D_800B1AF6 += 0xD;
        return;
    }
    if ((D_800B0F2D != D_800B0F2C) && (D_800EECBC == 0)) {
        D_800B1AF6 += 0x1E;
        return;
    }
    D_80124384 += D_800EECBC;
    D_801252E4 = 2;
    v = func_8002328C(D_800B1746) & 0x7F;
    D_80124344 = ((v >= 2) + (v >= 3)) * ((D_801252E4 == 0) + 1) + D_801252E4 * 6 - (D_801252E4 == 2) * 3;
    if (D_800EECBC == 1) {
        if (D_800B0F2E == D_800B0F2C) {
            D_801243B0 = D_800B0F2D;
        } else {
            D_801243B0 = D_800B0F2E;
        }
    }
    if (D_800EECBC == 2) {
        if (D_800B0F2F == D_800B0F2C) {
            D_801243B0 = D_800B0F2D;
        } else {
            D_801243B0 = D_800B0F2F;
        }
    }
    func_80011DFC();
}

void func_8011AF20(void) {
    if (D_800B0F2D != D_801243B0) {
        D_80124384 = 7;
        D_800B1AF6 += 0x14;
        return;
    }
    func_80011DFC();
    func_80011DFC();
    *(s16 *)(D_800B0860 + D_800B1746 * 0x34 + 0x1AE) += 8;
    func_8004C250();
}

void func_8011AFBC(void) {
    if (D_800B0F2F != 0xFF) {
        func_8004A2C4();
        return;
    }
    func_8004A4D8();
}

void func_8011AFFC(void) {
    if ((D_801252E4 == 0) && (D_801243B4 == 0)) {
        func_80046B88(0x80);
        return;
    }
    func_80011DFC();
}

void func_8011B04C(void) {
    func_80045B14();
    func_80011DFC();
}

void func_8011B074(void) {
    D_80124384 = 7;
    func_80011DFC();
}

void func_8011B09C(void) {
    u8 *q;

    if (D_800B0966 < 0x1E || (D_800B0966 < 0x23 && D_800B1746 == 7)) {
        D_80124344 = 0xC;
        /* FAKE: D_800B1AF6 (= D_800B1746 + 0x3B0) is updated through D_800B1746 so that as1 keeps the next load of D_800B1746 after the store; real source unknown. T-4050 */
        (&D_800B1746)[0x3B0] += 0xF;
        /* FAKE: the record address goes through (u32) so IDO does not keep D_800B1746 in a register across the halfword stores (the original reloads it each time); real source unknown. T-4050 */
        q = (u8 *)((u32)D_800B0860 + D_800B1746 * 0x34);
        *(s16 *)(q + 0x1A6) -= 1;
        q = (u8 *)((u32)D_800B0860 + D_800B1746 * 0x34);
        *(s16 *)(q + 0x1AA) -= 1;
        q = (u8 *)((u32)D_800B0860 + D_800B1746 * 0x34);
        *(s16 *)(q + 0x1AE) += 5;
        func_8004C250(0x34);
        return;
    }
    func_80011DFC();
}

void func_8011B190(void) {
    s16 i;
    EventParticleView44 *p;

    if ((u8)((EventMovieWork *)D_800F1990)->unk_19 == 1) {
        for (i = 11; i < 16; i++) {
            ((EventEffectTables *)D_800B0860)->unk_13A8[i] = -1;
            ((EventEffectTables *)D_800B0860)->unk_13A8[i + 32] = -1;
        }
        ((EventMovieWork *)D_800F1990)->unk_06 = 0;
    } else {
        for (i = 21; i < 26; i++) {
            ((EventEffectTables *)D_800B0860)->unk_13A8[i] = -1;
            ((EventEffectTables *)D_800B0860)->unk_13A8[i + 16] = -1;
        }
        ((EventMovieWork *)D_800F1990)->unk_04 = 0;
    }
    D_80125864 = 0x80;
    D_800EECF0 = 1;
    D_8012585C = 0;
    D_80125860 = 1;
    func_80110094();
    func_80012D64(D_80094710, 15, 1, 2, 0);
    ((EventParticleView44 *)D_800E9620)[31].unk_43 = 15;
    ((Rec24 *)D_800EC190)[0].unk_12 = 0x1EF;
    ((Rec24 *)D_800EC190)[1].unk_12 = 0x1EF;
    for (i = 16; i < 32; i++) {
        func_80012D64(D_80125280, i & 0xFF, 1, 0, i - 16);
    }
    func_80012D64(D_80125284, 14, 1, 0, 0);
    func_80012D64(D_80125288, 13, 1, 0, 0);
    for (i = 0; i < 10; i++) {
        func_800189C4(i + 96);
        p = (EventParticleView44 *)D_800E9620 + i;
        p[96].unk_01 = 13;
        p[96].unk_02 = 1;
        p[96].unk_38 = 0x1000000;
        p[96].unk_03 = 4;
        p[96].unk_0C = D_80125274;
        p[96].unk_10 = D_80125278;
        p[96].unk_34 = D_80125270;
        p[96].unk_14 = D_8012527C;
        p[96].unk_16 = 3;
        p[96].unk_18 = 0;
        p[96].unk_06 = 1;
        p[96].unk_43 = 0;
        p[96].unk_24.h[1] = (func_800789A0() % 256) - 0x80;
        p[96].unk_28.h[1] = 0xA0;
        p[96].unk_07 = 0;
        p[96].unk_05 = 0x80;
        p[96].unk_04 = 8;
        p[96].unk_2C = (func_800789A0() % 0x8000) - 0x4000;
        D_801257DC[i] = ((u32)-(func_800789A0() % 9) << 16) - 0x280000;
    }
    ((EventParticleView44 *)D_800E9620)[96].unk_03 |= 0x80;
    func_8004C46C(D_8012528C, D_80125290, D_80125294, D_80125298, D_8012529C, D_801252A0);
    for (i = 0; i < 6; i++) {
        ((Rec24 *)D_800EC190)[i + 4].unk_00 &= 0x7FFFFFFF;
        ((Rec24 *)D_800EC190)[i + 4].unk_12 = 0x1EE;
        ((EventEffectTables *)D_800B0860)->unk_1031[i] = 12;
    }
    for (i = 0; i < 6; i++) {
        ((Rec24 *)D_800EC190)[i + 4].unk_14 = ((Rec24 *)D_800EC190)[i + 4].unk_15 = ((Rec24 *)D_800EC190)[i + 4].unk_16 = 0x80;
    }
    func_80011DFC();
}

void func_8011B588(void) {
    func_80046B88(0x3E8);
}

/* FAKE: the first chain is dead (uopt drops its stores as overwritten); it moves the shared `li v0,0x80` above the second `lui $at` as in the original. Twin of DATE func_8013FA6C, T-9380 */
void func_8011B5A8(void) {
    (&D_800EC1C8)[0] = (&D_800EC1C9)[0] = (&D_800EC1CA)[0] = (&D_800EC1A4)[0] = (&D_800EC1A5)[0] = (&D_800EC1A6)[0] = (&D_800E9E63)[0] = 0x80;
    D_800EECD0 = 1;
    (&D_800EC1C8)[0] = (&D_800EC1C9)[0] = (&D_800EC1CA)[0] = (&D_800EC1A4)[0] = (&D_800EC1A5)[0] = (&D_800EC1A6)[0] = (&D_800E9E63)[0] = 0x80;
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010FE10", func_8011B60C);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010FE10", func_8011B848);

void func_8011BDC8(void) {
    func_80015D28(0x22, 0x801E0000, 0xABEA);
    func_801101B4();
    func_80011DFC();
}

void func_8011BE00(void) {
    s32 drift;
    s16 size_variant;
    s16 i;
    s32 scale;
    s32 divisor;
    s32 random_span;
    s32 choice;
    GsWord *position;

    if (D_8012531C == 2) {
        func_80012D64(D_801252DC, 0x12, 1, 2, 0);
    } else {
        func_80012D64(D_801252E0, 0x12, 1, 2, 0);
    }
    for (i = 0; i < 0x14; i++) {
        func_800189C4(i + 0x62);
        ((EventParticleView44 *) D_800E9620)[i + 98].unk_38 = 0x01000000;
        ((EventParticleView44 *) D_800E9620)[i + 98].unk_01 = 0xD;
        ((EventParticleView44 *) D_800E9620)[i + 98].unk_02 = 1;
        ((EventParticleView44 *) D_800E9620)[i + 98].unk_03 = 4;
        if (D_8012531C == 2) {
            ((EventParticleView44 *) D_800E9620)[i + 98].unk_0C = (s32) D_801252C4;
            ((EventParticleView44 *) D_800E9620)[i + 98].unk_10 = (s32) D_801252CC;
            ((EventParticleView44 *) D_800E9620)[i + 98].unk_34 = (s32) D_801252BC;
        } else {
            ((EventParticleView44 *) D_800E9620)[i + 98].unk_0C = (s32) D_801252C8;
            ((EventParticleView44 *) D_800E9620)[i + 98].unk_10 = (s32) D_801252D0;
            ((EventParticleView44 *) D_800E9620)[i + 98].unk_34 = (s32) D_801252C0;
        }
        ((EventParticleView44 *) D_800E9620)[i + 98].unk_16 = (s16) (func_800789A0() % 8);
        ((EventParticleView44 *) D_800E9620)[i + 98].unk_43 = 0x12;
        choice = func_800789A0() % 2;
        scale = 0x1000 / ((s32) ((((s16) choice) * 2) + 2));
        size_variant = (s16) choice;
        ((EventParticleView44 *) D_800E9620)[i + 98].unk_1A = (s16) scale;
        ((EventParticleView44 *) D_800E9620)[i + 98].unk_1C = (s16) scale;
        ((EventParticleView44 *) D_800E9620)[i + 98].unk_07 = 0x80;
        position = &D_80125868[i];
        ((EventParticleView44 *) D_800E9620)[i + 98].unk_24.h[1] = (s16) ((func_800789A0() % 300) - 0x96);
        position->w = (s32) ((EventParticleView44 *) D_800E9620)[i + 98].unk_24.w;
        random_span = func_800789A0();
        divisor = size_variant + 1;
        drift = ((s32) (((((func_800789A0() % 2) * 2) - 1) * (random_span % 8)) + 8)) / divisor;
        position->h[1] = (s16) (((s16) position->h[1]) + drift);
        ((EventParticleView44 *) D_800E9620)[i + 98].unk_28.h[1] = (s16) ((func_800789A0() % 160) - 0x78);
        ((EventParticleView44 *) D_800E9620)[i + 98].unk_30 = (s32) (0x8000 / divisor);
    }

    func_80011DFC();
}

void func_8011C11C(void) {
    s16 i;

    for (i = 0; i < 20; i++) {
        D_800E9620[i * 0x44 + 0x1A0B] &= 0x7F;
    }
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010FE10", func_8011C164);

void func_8011C3F4(void) {
    u32 v;

    v = func_8002328C(D_800B1746);
    if (D_800B1746 == 7 && v >= 3U && D_801252E4 == 2) {
        func_80011EC4(0x39);
        D_801243B0 += 1;
        return;
    }
    func_80011DFC();
}

/* s32 with no return (K&R): the incremented byte stays in $v0 as the return register. */
s32 func_8011C470(void) {
    /* FAKE: unused word reserves top frame slot so before sits at sp+0x28; T-11600 */
    s32 pad;
    s32 before;
    u8 sel;

    before = D_800B1AF6;
    /* FAKE: dead load before the first call lets cvt_pass strip the CVT of D_80125320, so it goes first in bne; T-11600 */
    sel = D_80125320;
    func_8004CF30();
    if ((D_800B1746 == 9) && (D_80125320 == 9)) {
        if (D_800B0BCF == 0) {
            func_8004A8EC(3);
        } else {
            func_8004A8EC(4);
        }
        func_80033E88();
        /* FAKE: (s32) cast gives the original operand order of the beq; T-11600 */
        if (before != (s32)D_800B1AF6) {
            D_800B1AF6 += 5;
        }
    }
}

void func_8011C514(void) {
    if (D_800B0A04[D_800B1746].unk_0C.b12 || D_800B0A04[D_800B1746].unk_0C.b13) {
        func_80011DFC();
        return;
    }
    func_80011DFC();
    func_80011DFC();
}

s32 func_8011C588(void) {
    EventStateView *g;
    u8 v;

    g = (EventStateView *)&D_800B0860;
    v = func_8002328C(D_800B1746);
    D_80124354 = g->unk_035 * 3 + ((v & 0x7F) >= 2U) + ((v & 0x7F) >= 3U) - 0x111;
    D_8009471C = (u8 *)&D_80124354;
    D_80094720 = &D_80124358;
    D_80094724 = D_80124258;
    D_80094728 = D_801242C4;
    D_8009472C = D_80124330;
    return func_80049B20(D_800EECBC, 1, 0);
}
