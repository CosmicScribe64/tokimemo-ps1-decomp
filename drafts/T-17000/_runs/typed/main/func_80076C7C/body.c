void func_80076C7C(void) {
    u8 sp4C;
    u8 sp2C;

    switch (D_800E6280.unk_110D) {
    case 0:
        k_reset(1);
        k_sub_reset_point_set();
        k_sub_reset();
        gnsx(D_800E6280.unk_0D4);
        strcpy(&sp4C, "それじゃ、 ");
        strcat(&sp4C, D_800B342C[D_800E6280.unk_724]);
        strcat(&sp4C, "で、 ");
        get_g_name(&sp2C, (s32) ((u32) (D_800E6280.unk_54C[0].w * 8) >> 0x1C));
        strcat(&sp4C, &sp2C);
        strcat(&sp4C, "を待つか。）");
        sndisp((s32) &sp4C, 0, 0x1F);
        k_sub_disp_start(4);
        D_800E6280.unk_110D += 1;
        return;
    case 1:
        xa_wait();
        return;
    case 2:
        func_8004284C();
        return;
    }
}
