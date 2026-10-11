#include "common.h"
#include "ovl/EVENT.h"

typedef struct {
    void (*f[39])();
} FnTbl39; /* size 0x9C */

typedef struct {
    void (*f[67])();
} FnTbl67; /* size 0x10C */

typedef struct {
    void (*f[46])();
} FnTbl46; /* size 0xB8 */

void func_801011A0(void) {
    D_801221D0 = 0x801CE0EC;
    D_801221D4 = 0x801CE0F0;
    D_801221D8 = 0x801CE110;
    D_801221DC = *(s16 *)0x801CE124;
    D_801221E0 = 0x801B0000;
    D_801221E4 = 0x801B2000;
    D_801221E8 = 0x801B6000;
    D_801221EC = 0x801BA000;
    D_801221F0 = 0x801BE000;
    D_801221F4 = 0x801C2000;
    D_801221F8 = 0x801C6000;
}

void func_80101250(void) {
    D_801221FC = 0x801D2230;
    D_80122200 = 0x801D2238;
    D_80122204 = 0x801D2278;
    D_80122208 = *(s16 *)0x801D2290;
    D_8012220C = 0x801B0000;
    D_80122210 = 0x801B2000;
    D_80122214 = 0x801B6000;
    D_80122218 = 0x801BA000;
    D_8012221C = 0x801BE000;
    D_80122220 = 0x801C2000;
    D_80122224 = 0x801C6000;
}

void func_80101300(void) {
    D_80122228 = 0x801D21D0;
    D_8012222C = 0x801D21D8;
    D_80122230 = 0x801D2218;
    D_80122234 = *(s16 *)0x801D2230;
    D_80122238 = 0x801B0000;
    D_8012223C = 0x801B2000;
    D_80122240 = 0x801B6000;
    D_80122244 = 0x801BA000;
    D_80122248 = 0x801BE000;
    D_8012224C = 0x801C2000;
    D_80122250 = 0x801C6000;
}

void func_801013B0(void) {
    D_80122254 = 0x801CE074;
    D_80122258 = 0x801CE078;
    D_8012225C = 0x801CE088;
    D_80122260 = *(s16 *)0x801CE094;
    D_80122264 = 0x801B0000;
    D_80122268 = 0x801B2000;
    D_8012226C = 0x801B6000;
    D_80122270 = 0x801BA000;
    D_80122274 = 0x801BE000;
    D_80122278 = 0x801C2000;
    D_8012227C = 0x801C6000;
}

void func_80101460(void) {
    D_80122280 = 0x801CE148;
    D_80122284 = 0x801CE14C;
    D_80122288 = 0x801CE170;
    D_8012228C = *(s16 *)0x801CE184;
    D_80122290 = 0x801B0000;
    D_80122294 = 0x801B2000;
    D_80122298 = 0x801B6000;
    D_8012229C = 0x801BA000;
    D_801222A0 = 0x801BE000;
    D_801222A4 = 0x801C2000;
    D_801222A8 = 0x801C6000;
}

void func_80101510(void) {
    switch (D_800EECB0) {
    case 1:
        func_801015B0();
        break;
    case 2:
        func_80101F24();
        break;
    case 3:
        func_80102118();
        break;
    case 4:
        func_801021FC();
        break;
    case 5:
        func_801022C0();
        break;
    default:
        func_80015FE0();
        break;
    }
}

void func_801015B0(void) {
    func_80078950("s%d ss%d\n", D_800B1AF5, D_800B1AF6);
    if (D_800B1AF5 == 0) {
        func_80101610();
        return;
    }
    func_80015FE0();
}

extern FnTbl39 D_801222AC;

void func_80101610(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl39 tbl;

    tbl = D_801222AC;
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
}

void func_80101684(void) {
    func_80015D28(0x3D, 0x801B0000, 0x8287);
    func_801011A0();
    func_80011DFC();
}

void func_801016BC(void) {
    D_800B1746 = 1;
    func_80012D2C(1, 0);
    func_8001886C(1);
    func_80010678();
    func_80018944(0);
    func_80012D40(1);
    func_80017E50();
    func_8001FD50();
    func_80036884(D_8011F81C);
    D_800E96EF = 0;
    D_800E9733 = 0;
    D_80094714 = 0;
    D_80094718 = 0;
    func_80045B14();
    func_8003535C();
    func_8003580C();
    func_8004C360();
    func_8004BC20(D_800B1746);
    func_8004CDDC();
    func_800F6000();
    D_80120678 = D_80120330;
    D_8012067C = D_80120344;
    D_80120680 = D_80120358;
    func_80078970(D_80094784, "廊下");
    D_800B0A04[1].unk_02 += 1;
    D_800B0A04[1].unk_06 += 2;
    func_8004C250();
    func_80011DFC();
}

void func_801017FC(void) {
    func_801011A0();
    func_80012D64(D_801221E0, 0x11, 1, 2, 0);
    func_8004C46C(D_801221E4, D_801221E8, D_801221EC, D_801221F0, D_801221F4, D_801221F8);
    func_8004C6B0(D_801221D4, D_801221D8, D_801221D0, (s32) D_801221DC);
    func_80011DFC();
}

void func_801018A4(void) {
    D_80125320 = 3;
    D_80120678 = D_80120334;
    D_8012067C = D_80120348;
    D_80120680 = D_8012035C;
    func_80011DFC();
}

void func_801018FC(void) {
    func_80101250();
    func_80012D64(D_8012220C, 0x11, 1, 2, 0);
    func_8004C46C(D_80122210, D_80122214, D_80122218, D_8012221C, D_80122220, D_80122224);
    func_8004C6B0(D_80122200, D_80122204, D_801221FC, (s32) D_80122208);
    func_80011DFC();
}

void func_801019A4(void) {
    D_80120678 = D_80120338;
    D_8012067C = D_8012034C;
    D_80120680 = D_80120360;
    func_80101300();
    func_80012D64(D_80122238, 0x11, 1, 2, 0);
    func_8004C46C(D_8012223C, D_80122240, D_80122244, D_80122248, D_8012224C, D_80122250);
    func_8004C6B0(D_8012222C, D_80122230, D_80122228, D_80122234);
    D_800B0A04[1].unk_02 += 1;
    D_800B0A04[1].unk_06 += 2;
    D_800B0A04[1].unk_0A -= 0x14;
    func_8004C250();
    func_80011DFC();
}

void func_80101AB8(void) {
    D_80120678 = D_8012033C;
    D_8012067C = D_80120350;
    D_80120680 = D_80120364;
    func_801013B0();
    func_80012D64(D_80122264, 0x11, 1, 2, 0);
    func_8004C46C(D_80122268, D_8012226C, D_80122270, D_80122274, D_80122278, D_8012227C);
    func_8004C6B0(D_80122258, D_8012225C, D_80122254, D_80122260);
    D_800EAFD8 |= 0x80000000;
    D_800EAFB6 = 0;
    D_800B0A04[1].unk_02 += 2;
    D_800B0A04[1].unk_06 += 2;
    D_800B0A04[1].unk_0A -= 0x14;
    func_8004C250();
    func_80011DFC();
}

void func_80101BEC(void) {
    D_80120678 = D_80120340;
    D_8012067C = D_80120354;
    D_80120680 = D_80120368;
    func_80012D64(D_80122290, 0x11, 1, 2, 0);
    func_80012D64(D_80122290, 0x12, 1, 2, 1);
    func_8004C46C(D_80122294, D_80122298, D_8012229C, D_801222A0, D_801222A4, D_801222A8);
    func_8004C6B0(D_80122284, D_80122288, D_80122280, D_8012228C);
    D_800B0A04[1].unk_02 += 3;
    D_800B0A04[1].unk_06 += 2;
    D_800B0A04[1].unk_0A -= 0x14;
    func_8004C250();
    func_80011DFC();
}

void func_80101D1C(void) {
    func_800469F4(0x4657);
    func_80011DFC();
}

void func_80101D44(void) {
    func_800469F4(0x3FDE);
    func_80011DFC();
}

void func_80101D6C(void) {
    func_800469F4(0x42D6);
    func_80011DFC();
}

void func_80101D94(void) {
    func_800469F4(0x474F);
    func_80011DFC();
}

void func_80101DBC(void) {
    func_800469F4(0x42A8);
    func_80011DFC();
}

void func_80101DE4(void) {
    func_800469F4(0x4804);
    func_80011DFC();
}

void func_80101E0C(void) {
    func_800469F4(0x451C);
    func_80011DFC();
}

void func_80101E34(void) {
    func_800469F4(0x44C7);
    func_80011DFC();
}

void func_80101E5C(void) {
    func_800469F4(0x47BD);
    func_80011DFC();
}

void func_80101E84(void) {
    func_800469F4(0x44BD);
    func_80011DFC();
}

void func_80101EAC(void) {
    func_800469F4(0x4779);
    func_80011DFC();
}

void func_80101ED4(void) {
    func_800469F4(0x4376);
    func_80011DFC();
}

void func_80101EFC(void) {
    func_800469F4(0x436D);
    func_80011DFC();
}

extern FnTbl67 D_80122348;

void func_80101F24(void) {
    s32 pad; /* FAKE: unused local, takes the 4 bytes above the table (T-3330 layout); real source unknown. T-4010 */
    FnTbl67 tbl;

    tbl = D_80122348;
    func_80078950(" %d %d\n", D_800B1AF6, (&D_801243A4)[D_800EECBC]);
    tbl.f[D_800B1AF6](0x80);
}

void func_80101FC8(void) {
    func_80015D28(0x45, 0x801B0000, 0x82C4);
    func_80101250();
    func_80011DFC();
}

void func_80102000(void) {
    if (D_800EECBC == 1) {
        func_8004A8EC(2);
        func_80033E88();
        func_80011EC4(0x2E);
        return;
    }
    D_800B0E31 = 1;
    D_80124394 = 3;
    D_80124396 = 3;
    D_80124398 = 3;
    D_801243A4 = 2;
    D_801243A6 = 2;
    D_801243A8 = 2;
    D_800B0A04[1].unk_02 -= 2;
    D_800B0A04[1].unk_06 -= 1;
    D_800B0A04[1].unk_0A += 0x14;
    func_8004C250();
    func_80011DFC();
}

void func_801020E4(void) {
    func_80078970(D_800947C4, "絶叫マシーンビビール");
    func_80011DFC();
}

extern FnTbl39 D_80122454;

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/801011A0", func_80102118);

void func_801021C4(void) {
    func_80015D28(0x45, 0x801B0000, 0x8309);
    func_80101300();
    func_80011DFC();
}

extern FnTbl46 D_801224F0;

void func_801021FC(void) {
    s32 pad; /* FAKE: unused local, takes the 4 bytes above the table (T-3330 layout); real source unknown. T-4010 */
    FnTbl46 tbl;

    tbl = D_801224F0;
    func_80078950("%d\n", D_800B1AF6);
    tbl.f[D_800B1AF6](0x80);
}

void func_80102288(void) {
    func_80015D28(0x3D, 0x801B0000, 0x834E);
    func_801013B0();
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/801011A0", func_801022C0);

void func_801023F4(void) {
    func_80015D28(0x3D, 0x801B0000, 0x838B);
    func_80101460();
    func_80011DFC();
}

void func_8010242C(void) {
    if (D_800EECBC != 0) {
        D_80094714 += 1;
    } else {
        D_800B0A04[1].unk_0C.b14 = 1;
    }
    func_80011DFC();
}

void func_80102484(void) {
    func_8011C6C4();
}
