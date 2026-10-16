void func_8005A668(void) {
    u8 temp_v1;
    u8 *temp_v0;

    back_clear_switch(0);
    func_80048E78();
    func_80041584();
    D_800E6280.unk_036 = 0;
    D_800E6280.unk_037 = 0;
    D_800E6280.unk_038 = 0;
    func_8004E58C();
    func_80066104((s32) "平日コマンド画面");
    gnsx(D_800E6280.unk_0D4);
    parameter_show_init();
    parameter_show_init();
    hizuke_init();
    message_window_init();
    parameter_disp_switch(1);
    tpage_buf_clear();
    temp_v0 = &D_800E6280 + D_800E6280.unk_040;
    temp_v1 = *(u8 *)((unsigned char *)temp_v0 + (101));
    if (temp_v1 == 5) {
        D_800E6280.unk_F6D = 0xF;
        func_8006509C();
        func_800654D0((s32) (u8) D_800E6280.unk_F6D);
        func_8005AC70(1);
        D_800E6280.unk_1093 = 0xE;
        icon_can_use_set(0xE, 1);
        sndisp((s32) "明日からテストだ）", 0, 0x1F);
    } else if ((temp_v1 == 8) && (((u16) D_800E6280.unk_0F4.h >> 0xC) != 0xB)) {
        D_800E6280.unk_F6D = 0xE;
        func_8006509C();
        func_800654D0((s32) (u8) D_800E6280.unk_F6D);
        func_8005AC70(1);
        icon_can_use_set(4, 1);
        D_800E6280.unk_1093 = 4;
        if ((((u16) D_800E6280.unk_0F4.h >> 0xC) == 6) && (D_800E6280.unk_03E == 0x61) && ((u32) (*(u16 *)((unsigned char *)(&D_800E6280) + (1670)) & 0xF) >= 8U)) {
            sndisp((s32) "明日から、念願の 全国高校野球大会だ）", 0, 0x1F);
        } else {
            sndisp((s32) "明日から夏合宿だ）", 0, 0x1F);
        }
    } else if ((*(u8 *)((unsigned char *)(temp_v0 - 1) + (107)) == 0xC) && ((u32) ((u16) D_800E6280.unk_0F4.h >> 0xC) < 6U)) {
        if ((D_800E6280.unk_0F4.b[1] & 0xF) != 3) {
            D_800E6280.unk_F6D = 0xE;
            func_8006509C();
            func_800654D0((s32) (u8) D_800E6280.unk_F6D);
            func_8005AC70(1);
            icon_can_use_set(4, 1);
            D_800E6280.unk_1093 = 4;
            sndisp((s32) "文化祭に向けて部活をせねば）", 0, 0x1F);
        } else {
            D_800E6280.unk_F6D = 0xE;
            func_8006509C();
            func_800654D0((s32) (u8) D_800E6280.unk_F6D);
            func_8005AC70(1);
            icon_can_use_set(7, 1);
            D_800E6280.unk_1093 = 7;
            func_8005AD1C();
            sndisp((s32) "文化祭に向けて部活をせねば でも病気で身体が動かない）", 0, 0x1F);
        }
    } else if (temp_v1 == 0xB) {
        D_800E6280.unk_F6D = 0xF;
        func_8006509C();
        func_800654D0((s32) (u8) D_800E6280.unk_F6D);
        func_8005AC70(1);
        D_800E6280.unk_1093 = 0xE;
        icon_can_use_set(0xE, 1);
        sndisp((s32) "明日から修学旅行だ）", 0, 0x1F);
    } else if (D_800E6280.unk_000 & 1) {
        D_800E6280.unk_F6D = 0xE;
        func_8006509C();
        func_800654D0((s32) (u8) D_800E6280.unk_F6D);
        func_8005AC70(1);
        icon_can_use_set(0, 1);
        icon_can_use_set(1, 1);
        icon_can_use_set(2, 1);
        D_800E6280.unk_1093 = 0;
        sndisp((s32) "一週間強制補習を しなければならない）", 0, 0x1F);
    } else if (((u8) D_800E6280.unk0 & 0xF) == 3) {
        D_800E6280.unk_F6D = 0xE;
        func_8006509C();
        func_800654D0((s32) (u8) D_800E6280.unk_F6D);
        func_8005AC70(1);
        icon_can_use_set(7, 1);
        D_800E6280.unk_1093 = 7;
        func_8005AD1C();
        sndisp((s32) "病気で身体が動かない）", 0, 0x1F);
    } else {
        D_800E6280.unk_F6D = 0xE;
        func_8006509C();
        func_800654D0((s32) (u8) D_800E6280.unk_F6D);
        func_8005AC70(0);
        D_800E6280.unk_1093 = D_800E6280.unk_F68.w & 0xF;
        sndisp((s32) "明日から何をしようかな？）", 0, 0x1F);
    }
    k_disp_start(4);
    func_8006BC28(0);
    if ((D_800E6280.unk_03E == 0x5F) && (D_800E6280.unk_03F == 4) && (D_800E6280.unk_040 == 4)) {
        icon_can_use_set(0xD, 0);
    }
    menu_bar_color(0x10, 0, 0, 8);
    func_8005AB4C();
}
