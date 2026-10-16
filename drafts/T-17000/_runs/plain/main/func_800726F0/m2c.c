extern u8 D_800E62BD;
extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern u8 D_800E6354;
extern u8 D_800E738A;

void func_800726F0(void) {
    switch (D_800E738A) {                           /* irregular */
    case 0:
        func_8004E58C();
        func_80066104((s32) "ダミー");
        func_80065F34(0);
        gnsx(&D_800E6354);
        if ((D_800E62BD != 0) && (D_800E62C0 == 2)) {
            D_800E62BD = 0;
            sndisp((s32) "明日から新学期だ）", 0, 0x1F);
        } else if (D_800E62BD != 0) {
            D_800E62BD = 0;
            sndisp((s32) "明日から新学期だ）", 0, 0x1F);
        } else if (D_800E62BF == 7) {
            sndisp((s32) "明日から夏休みだ）", 0, 0x1F);
            D_800E62BD = 1;
        } else if (D_800E62BF == 0xC) {
            sndisp((s32) "明日から冬休みだ）", 0, 0x1F);
            D_800E62BD = 2;
        } else if (D_800E62BF == 3) {
            sndisp((s32) "明日から春休みだ）", 0, 0x1F);
            D_800E62BD = 3;
        } else {
            D_800E62BD = 0;
        }
        func_800674B0();
        k_disp_start(3);
        func_8004284C();
        return;
    case 1:
        if (func_80044E8C() == 1) {
            func_8004500C(0, 0);
            func_80042808();
            return;
        }
        return;
    default:
        func_80046500();
        break;
    }
}
