#include "ovl/OMIMAI.h"

void func_801320A0(void) {
    D_80134A60[0] = 0x801970EC;
    D_80134A60[1] = 0x8019727C;
    D_80134A60[2] = 0x80197434;
    D_80134A60[3] = 0x8019763C;
    D_80134A60[4] = 0x8019784C;
    D_80134A60[5] = 0x801979F4;
    D_80134A60[6] = 0x80197BC4;
    D_80134A60[7] = 0x80197DA8;
    D_80134A60[8] = 0x80197F40;
    D_80134A60[9] = 0x80198450;
    D_80134A60[10] = 0x801987C8;
    D_80134A60[11] = 0x801980C0;
    D_80134A90[0] = 0x801970F8;
    D_80134A90[1] = 0x80197288;
    D_80134A90[2] = 0x80197440;
    D_80134A90[3] = 0x80197648;
    D_80134A90[4] = 0x80197858;
    D_80134A90[5] = 0x80197A00;
    D_80134A90[6] = 0x80197BD0;
    D_80134A90[7] = 0x80197DB4;
    D_80134A90[8] = 0x80197F4C;
    D_80134A90[9] = 0x80198468;
    D_80134A90[10] = 0x801987D8;
    D_80134A90[11] = 0x801980D8;
    D_80134AC0[0] = 0x80197188;
    D_80134AC0[1] = 0x80197318;
    D_80134AC0[2] = 0x801974D0;
    D_80134AC0[3] = 0x80197710;
    D_80134AC0[4] = 0x801978E8;
    D_80134AC0[5] = 0x80197A90;
    D_80134AC0[6] = 0x80197C98;
    D_80134AC0[7] = 0x80197E44;
    D_80134AC0[8] = 0x80197FDC;
    D_80134AC0[9] = 0x801985F8;
    D_80134AC0[10] = 0x801988D8;
    D_80134AC0[11] = 0x801981A8;
}

typedef struct {
    void (*f[25])();
} FnTbl25; /* size 0x64 */
extern FnTbl25 D_80134AF0;

void func_801322E4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl25 tbl;

    tbl = D_80134AF0;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
    if (D_800E6280.unk_110A < 0xEU) {
        func_80066C08(1);
        parameter_show();
        return;
    }
    func_80066C08(2);
}

void func_80132398(void) {
    if ((D_800E6280.unk_040 == ((u32)(D_800E6280.unk_0F8 << 0x17) >> 0x1B)) && (D_800E6280.unk_03F == (D_800E6280.unk_0F8 & 0xF))) {
        func_80042808();
        return;
    }
    func_8004284C();
}

s32 func_801323F8(void) {
    if (D_800E6280.unk_110D == 0) {
        func_800674B0();
        D_800E6280.unk_110D += 1;
    }
    if (D_800E6280.unk_110D == 1) {
        if (D_800E6280.unk_1104.u++ >= 0x400U) {
            func_800452C4();
            func_8004482C();
            D_800E6280.unk_110D = 0;
        }
        if (func_80044E8C() == 1) {
            D_800E6280.unk_110D += 1;
        } else {
            return 0;
        }
    }
    return func_80072B5C(1);
}

void func_801324BC(void) {
    func_80044890(1, 0xBF98, 0xBF79, D_800B3688[D_800E6280.unk_75D], D_800B36C8[D_800E6280.unk_75D], D_800B3708[D_800E6280.unk_75D]);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

void func_80132544(void) {
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    if ((u32) D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

void func_801325C4(void) {
    func_80044750(0x200);
    func_8004284C();
}

void func_801325EC(void) {
    cd_read_sectors(4, 0x80197000, 0xAF3F);
    func_801320A0();
    func_80044750(0x603);
    func_8004284C();
}

void func_80132630(void) {
    func_8004E58C();
    k_reset(1);
    addr_init_bustup();
    func_80084E4C();
    func_8004284C();
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7368;
extern s8 D_800E62BA;
extern s8 D_800E7322;
extern u16 D_800E6374;
extern u8 D_800E652A;
extern u8 D_800E69DD;
extern u8 D_800E71DF;
void func_8013285C(void);
/* end bulk_m2c declarations */
void func_80132670(void) {
    u8 temp_a0;

    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    func_800AE0F0(D_800CA19C, "玄関");
    D_800E7322 = 0;
    D_800E7368 = 1;
    func_8008585C();
    D_800E62BA = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    hizuke_init();
    message_window_init();
    func_80084E4C();
    D_800CA148 = 0;
    D_800CA14C = 0;
    temp_a0 = D_800E69DD & 0xFF;
    if ((D_800E69DD == 4) && (((u16) D_800E6374 >> 0xC) == ((u8) D_800E652A >> 4))) {
        D_800CA148 = 1;
    }
    D_800E71DF = temp_a0;
    func_800847B8(temp_a0);
    func_8013285C();
    func_8004284C();
}

void func_80132788(void) {
    bg_read_sub2(0x4144);
    func_80085B3C(0xF, 0);
    func_8004284C();
}

void func_801327BC(void) {
    func_800847B8(D_800E6280.unk_75D);
    D_800E6280.unk_F5F = 0xE;
    func_800AE0F0(D_800CA188, "母親");
    D_800CA148 = (u32)D_800E6280.unk_75D > 0;
    D_800CA14C = 0;
    func_801320A0();
    D_800CA160 = D_80134A8C;
    D_800CA164 = D_80134ABC;
    D_800CA168 = D_80134AEC;
    func_8004284C();
}

void func_8013285C(void) {
    switch (D_800E6280.unk_F5F) {
    case 0:
        D_800CA160 = D_80134A60[1];
        D_800CA164 = D_80134A90[1];
        D_800CA168 = D_80134AC0[1];
        return;
    case 1:
        D_800CA160 = D_80134A60[5];
        D_800CA164 = D_80134A90[5];
        D_800CA168 = D_80134AC0[5];
        return;
    case 2:
        D_800CA160 = D_80134A60[2];
        D_800CA164 = D_80134A90[2];
        D_800CA168 = D_80134AC0[2];
        return;
    case 3:
        D_800CA160 = D_80134A60[3];
        D_800CA164 = D_80134A90[3];
        D_800CA168 = D_80134AC0[3];
        return;
    case 4:
        D_800CA160 = D_80134A60[9];
        D_800CA164 = D_80134A90[9];
        D_800CA168 = D_80134AC0[9];
        return;
    case 5:
        D_800CA160 = D_80134A60[6];
        D_800CA164 = D_80134A90[6];
        D_800CA168 = D_80134AC0[6];
        return;
    case 6:
        D_800CA160 = D_80134A60[7];
        D_800CA164 = D_80134A90[7];
        D_800CA168 = D_80134AC0[7];
        return;
    case 7:
        D_800CA160 = D_80134A60[4];
        D_800CA164 = D_80134A90[4];
        D_800CA168 = D_80134AC0[4];
        return;
    case 8:
        D_800CA160 = D_80134A60[0];
        D_800CA164 = D_80134A90[0];
        D_800CA168 = D_80134AC0[0];
        return;
    case 9:
        D_800CA160 = D_80134A60[8];
        D_800CA164 = D_80134A90[8];
        D_800CA168 = D_80134AC0[8];
        return;
    case 10:
        D_800CA160 = D_80134A60[10];
        D_800CA164 = D_80134A90[10];
        D_800CA168 = D_80134AC0[10];
    }
}
