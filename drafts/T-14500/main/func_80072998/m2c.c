extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern u8 D_800E6354;
extern u8 D_800E738D;

void func_80072998(void) {
    switch (D_800E738D) {                           /* irregular */
    case 0:
        cd_read_sectors(0x95U, 0x801A0000, 0x9B94);
        D_800E738D += 1;
        return;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_80068938(D_800E62BE, D_800E62BF, 1);
            if (((u8) D_800E62BF % 3U) == 0) {
                if ((D_800E62BF == 9) && (D_800E62C0 == 1) && (D_800E62BE == 0x60)) {
                    func_80042808();
                } else if (D_800E62BF == 9) {
                    func_8004E58C();
                    func_80066104((s32) "ダミー");
                    func_80065F34(0);
                    gnsx(&D_800E6354);
                    sndisp((s32) "明日から新学期だ）", 0, 0x1F);
                    k_disp_start(3);
                    func_80044750(0xC1);
                    func_800674B0();
                    func_8004284C();
                } else {
                    func_80044750(0xC1);
                    func_800674B0();
                    func_8004284C();
                }
                func_8006D00C();
                D_800B5BC8 = 0xEF;
                return;
            }
            func_80042808();
        } else {
            return;
        }
        break;
    }
}
