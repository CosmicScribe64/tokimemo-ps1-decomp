#include "common.h"
#include "ovl/BUNKASAI.h"

void func_80132000(void) {
    D_801604F0 = 0x801EC7F8;
    D_801604F4 = 0x801ECD50;
    D_801604F8 = 0x801F02A8;
}

void func_80132034(void) {
    D_801604F0 = 0x801EEAF0;
    D_801604F4 = 0x801EF1F4;
    D_801604F8 = 0x801F3C2C;
}

void func_80132068(void) {
    D_801604F0 = 0x801ED9E0;
    D_801604F4 = 0x801EDFE8;
    D_801604F8 = 0x801F17E0;
}

void func_8013209C(void) {
    D_801604F0 = 0x801ECDF4;
    D_801604F4 = 0x801ED474;
    D_801604F8 = 0x801F0A0C;
}

void func_801320D0(void) {
    D_801604F0 = 0x801EAE58;
    D_801604F4 = 0x801EB114;
    D_801604F8 = 0x801ECF80;
}

void func_80132104(void) {
    D_801604F0 = 0x801EF01C;
    D_801604F4 = 0x801EF838;
    D_801604F8 = 0x801F4768;
}

void func_80132138(void) {
    D_801604F0 = 0x801EAAE8;
    D_801604F4 = 0x801EAD98;
    D_801604F8 = 0x801ECA94;
}

void func_8013216C(void) {
    D_801604F0 = 0x801EEF50;
    D_801604F4 = 0x801EF630;
    D_801604F8 = 0x801F3EFC;
}

void func_801321A0(void) {
    D_801604F0 = 0x801EE578;
    D_801604F4 = 0x801EEC8C;
    D_801604F8 = 0x801F338C;
}

void func_801321D4(void) {
    D_801604F0 = 0x801EC030;
    D_801604F4 = 0x801EC440;
    D_801604F8 = 0x801EF008;
}

void func_80132208(void) {
    D_801604F0 = 0x801EB46C;
    D_801604F4 = 0x801EB784;
    D_801604F8 = 0x801EDBF0;
}

void func_8013223C(void) {
    D_801604F0 = 0x801EA968;
    D_801604F4 = 0x801EACA8;
    D_801604F8 = 0x801ECC70;
}

void func_80132270(void) {
    D_801604F0 = 0x801EB83C;
    D_801604F4 = 0x801EBC70;
    D_801604F8 = 0x801EE0F8;
}

void func_801322A4(void) {
    D_801604F0 = 0x801EBD80;
    D_801604F4 = 0x801EC234;
    D_801604F8 = 0x801EF02C;
}

void func_801322D8(void) {
    D_801604F0 = 0x801EBF6C;
    D_801604F4 = 0x801EC3C0;
    D_801604F8 = 0x801EEFA8;
}

void func_8013230C(void) {
    D_801604F0 = 0x801ED958;
    D_801604F4 = 0x801EDE80;
    D_801604F8 = 0x801F1C8C;
}

void func_80132340(void) {
    D_801604F0 = 0x801EA974;
    D_801604F4 = 0x801EAC48;
    D_801604F8 = 0x801ECD44;
}

void func_80132374(void) {
    D_801604F0 = 0x801ED184;
    D_801604F4 = 0x801ED664;
    D_801604F8 = 0x801F0F78;
}

void func_801323A8(void) {
    D_801604F0 = 0x801E92FC;
    D_801604F4 = 0x801E9598;
    D_801604F8 = 0x801EA298;
}

void func_801323DC(void) {
    D_801604F0 = 0x801E81BC;
    D_801604F4 = 0x801E81CC;
    D_801604F8 = 0x801E82CC;
}

void func_80132410(void) {
    D_801604F0 = 0x801E81CC;
    D_801604F4 = 0x801E8204;
    D_801604F8 = 0x801E82E4;
}

void func_80132444(void) {
    D_801604F0 = 0x801E8A10;
    D_801604F4 = 0x801E8B7C;
    D_801604F8 = 0x801E93D4;
}

/* object boundary: alignment padding of the original link */
INCLUDE_ASM("src/ovl/pad", pad_BUNKASAI_80132478);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80132480);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801325F4);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801328D4);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80132DB4);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801333CC);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80133990);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80133E18);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801343B4);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801347A8);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80134A90);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80134CF8);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801350A8);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80135448);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801354B0);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80135554);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80135914);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80135C4C);

void func_80136230(void) {
    func_800847B8(1);
    func_801320D0();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}

/* object boundary: alignment padding of the original link */
INCLUDE_ASM("src/ovl/pad", pad_BUNKASAI_80136294);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801362A0);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80136414);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801367A0);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80136AE8);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80136EC8);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80137304);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80137788);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80137DD8);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80138148);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80138388);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8013866C);

void func_80138A00(void) {
    func_800847B8(4);
    func_8013223C();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}

/* object boundary: alignment padding of the original link */
INCLUDE_ASM("src/ovl/pad", pad_BUNKASAI_80138A64);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80138A70);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80138BE4);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80138EF8);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801391B8);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80139504);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80139920);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80139D84);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8013A614);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8013A95C);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8013ABA0);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8013AE08);

void func_8013B120(void) {
    func_800847B8(8);
    func_8013230C();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}

/* object boundary: alignment padding of the original link */
INCLUDE_ASM("src/ovl/pad", pad_BUNKASAI_8013B184);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8013B190);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8013B25C);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8013B688);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8013BB94);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8013C0AC);

void func_8013C3C4(void) {
    func_800847B8(2);
    func_80132138();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}

/* object boundary: alignment padding of the original link */
INCLUDE_ASM("src/ovl/pad", pad_BUNKASAI_8013C428);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8013C430);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8013C5A4);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8013CA64);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8013CCA8);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8013D1F8);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8013D8E0);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8013DE3C);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8013E59C);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8013EBDC);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8013EFB8);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8013F234);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8013F5D4);

void func_8013FA20(void) {
}

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8013FA28);

void func_8013FC58(void) {
    func_800847B8(3);
    func_8013216C();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}

void func_8013FCBC(void) {
    func_800847B8(3);
    func_801321A0();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}

void func_8013FD20(void) {
    func_800847B8(3);
    func_801321D4();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}

/* object boundary: alignment padding of the original link */
INCLUDE_ASM("src/ovl/pad", pad_BUNKASAI_8013FD84);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8013FD90);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8013FEB4);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801401E8);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80140558);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80140958);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80140F4C);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801417DC);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80141F70);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80142298);

void func_80142694(void) {
    func_800847B8(10);
    func_80132374();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}

/* object boundary: alignment padding of the original link */
INCLUDE_ASM("src/ovl/pad", pad_BUNKASAI_801426F8);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80142700);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80142B98);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80142BC8);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80142BE8);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801431D4);

void func_8014338C(void) {
    k_reset(0);
    func_8006612C(&D_8015D774);
    k_disp_start(1);
}

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801433C0);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80143770);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801438E4);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80143C38);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80143F90);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80144374);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801448D8);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80144D20);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80145334);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801456E0);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80145930);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80145C28);

void func_80145FC0(void) {
    func_800847B8(7);
    func_801322D8();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}

/* object boundary: alignment padding of the original link */
INCLUDE_ASM("src/ovl/pad", pad_BUNKASAI_80146024);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80146030);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80146100);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801464A8);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801466FC);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80146A78);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80146F38);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80146FA0);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80147214);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80147F8C);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80148040);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801481B0);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80148524);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80148800);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80148B84);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80148F98);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80149404);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80149A88);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80149E68);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8014A1AC);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8014A49C);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8014A778);

void func_8014AB54(void) {
    func_800847B8(0);
    func_80132000();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}

/* object boundary: alignment padding of the original link */
INCLUDE_ASM("src/ovl/pad", pad_BUNKASAI_8014ABB8);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8014ABC0);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8014AD34);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8014B15C);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8014B548);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8014B9D8);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8014BFE0);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8014C528);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8014CE94);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8014D240);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8014D558);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8014D8F0);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8014DD28);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8014DD90);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8014DF04);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8014E2AC);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8014E5EC);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8014E9D0);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8014EF24);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8014F36C);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8014FC70);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8014FFE4);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80150234);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8015052C);

void func_801508C4(void) {
    func_800847B8(6);
    func_801322A4();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}

/* object boundary: alignment padding of the original link */
INCLUDE_ASM("src/ovl/pad", pad_BUNKASAI_80150928);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80150930);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801509A4);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80150B7C);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80150F20);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80151064);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801511E0);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80151364);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801516B0);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80151A10);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80151F10);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801523A0);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801527A8);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80152F08);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80153260);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80153530);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801537BC);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80153A98);

void func_80153FA4(void) {
    func_800847B8(0);
    func_80132034();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}

/* object boundary: alignment padding of the original link */
INCLUDE_ASM("src/ovl/pad", pad_BUNKASAI_80154008);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80154018);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801541D8);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801548C8);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80154D64);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80155388);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80155B1C);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80156284);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801569BC);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8015706C);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80157860);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80157DD4);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_801580EC);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80158764);

void func_80158DD8(void) {
    func_800847B8(1);
    func_80132068();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}

void func_80158E3C(void) {
    func_800847B8(1);
    func_8013209C();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}

/* object boundary: alignment padding of the original link */
INCLUDE_ASM("src/ovl/pad", pad_BUNKASAI_80158EA0);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80158EB0);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80158FD4);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80159344);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80159698);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80159AD4);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_80159FBC);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8015A5DC);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8015A870);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8015AB58);

void func_8015AF20(void) {
    func_800847B8(9);
    func_80132340();
    D_800CA14C = 0;
    D_800CA160 = D_801604F0;
    D_800CA164 = D_801604F4;
    D_800CA168 = D_801604F8;
    func_8004284C();
}

/* object boundary: alignment padding of the original link */
INCLUDE_ASM("src/ovl/pad", pad_BUNKASAI_8015AF84);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8015AF90);

INCLUDE_ASM("asm/ovl/BUNKASAI/nonmatchings/BUNKASAI", func_8015B828);
