/* FAKE: the byte views of D_8013C584 keep each read in v1, and `idx` declared above the table puts
 * the table at sp+0x28, as in func_80137044 (T-18600). */
void func_80135CF4(void) {
    s32 idx; /* declared before the table: its stack slot sits above it */
    MasterHalfwords13 values;

    values = D_8013C6A4;
    func_80083808();
    switch (D_800E6280.unk_110A) {
    case 0:
        if (cd_read_sync_status() & 1) {
            func_800847B8((*(u8 *)&D_8013C584));
            idx = (*(u8 *)&D_8013C584);
            D_800CA148 = values.v[idx];
            if (((*(u8 *)&D_8013C584) == 4) && (((u16) D_800E6280.unk_0F4.h >> 0xC) == 6)) {
                D_800CA148 = 0x3C;
            }
            D_800CA14C = 0;
            addr_init_bustup();
            D_800E6280.unk_F5F = (*(u8 *)&D_8013C584);
            func_80083474();
            if ((*(u8 *)&D_8013C584) == 0xC) {
                func_80083440(0U);
            }
        }
        break;
    case 1:
        normal_date_move_place();
        break;
    case 2:
        if ((*(u8 *)&D_8013C584) == 0xB) {
            func_80044750(0x500);
            func_80047550();
        }
        func_8004500C(0, 0);
        func_8004284C();
        break;
    case 3:
        normal_date_speak();
        break;
    case 4:
        normal_date_girl_in();
        break;
    case 5:
        normal_date_speak();
        break;
    case 6:
        if ((*(u8 *)&D_8013C584) == 0xB) {
            if (((u32) (D_800E6280.unk_1BC[0xB].unk_0C.w << 0x14) >> 0x1D) == 0) {
                D_800CA148 = 0x1E;
            } else {
                D_800CA148 = 0x1F;
            }
            normal_date_girl_out();
        } else if ((*(u8 *)&D_8013C584) == 0xC) {
            D_800CA148 = 0x20;
            func_8004284C();
        } else {
            D_800CA148 = 0x1D;
            func_8004284C();
        }
        break;
    case 7:
        normal_date_speak();
        break;
    case 8:
        if ((*(u8 *)&D_8013C584) == 0xB) {
            func_8004284C();
        } else {
            normal_date_girl_out();
        }
        break;
    case 9:
        normal_date_move_place();
        break;
    case 10:
        func_8004500C(1, 0);
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}
