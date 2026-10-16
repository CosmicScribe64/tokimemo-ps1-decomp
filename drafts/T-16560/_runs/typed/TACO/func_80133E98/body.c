u8 func_80133E98(void) {
    u8 temp_t0;
    u8 temp_t0_2;
    u8 temp_t1;
    u8 temp_t2;
    u8 temp_t3;
    u8 temp_t4;
    u8 temp_t4_2;
    u8 temp_t5;
    u8 temp_t6;
    u8 temp_t7;
    u8 temp_t8;
    u8 temp_t8_2;
    u8 var_v0;

    var_v0 = D_800E6280.unk_110D;
    switch (var_v0) {
    case 0:
        func_800462C8(1U, 0x8018A000, 0xB3F2);
        func_800462C8(5U, 0x8018A800, 0xB413);
        func_800462C8(0x12U, 0x8018D000, 0xB438);
        temp_t8 = (D_800E6280.unk_110D + 2) & 0xFF;
        var_v0 = temp_t8;
        D_800E6280.unk_110D = temp_t8;
    default:
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        var_v0 = cd_read_sync_status();
        if (var_v0 & 1) {
            temp_t0 = (D_800E6280.unk_110D + 1) & 0xFF;
            var_v0 = temp_t0;
            D_800E6280.unk_110D = temp_t0;
        }
        break;
    case 5:
        func_800462C8(7U, 0x80196000, 0xB45B);
        D_800E6280.unk_110D += 1;
        func_800462C8(3U, 0x80199800, 0xB478);
        func_800462C8(2U, 0x8019B000, 0xB47B);
        temp_t2 = (D_800E6280.unk_110D + 1) & 0xFF;
        var_v0 = temp_t2;
        D_800E6280.unk_110D = temp_t2;
        break;
    case 6:
    case 7:
    case 8:
    case 9:
        var_v0 = cd_read_sync_status();
        if (var_v0 & 1) {
            temp_t4 = (D_800E6280.unk_110D + 1) & 0xFF;
            var_v0 = temp_t4;
            D_800E6280.unk_110D = temp_t4;
        }
        break;
    case 10:
        func_800462C8(1U, 0x8019C000, 0xB486);
        func_800462C8(2U, 0x8019C800, 0xB487);
        func_800462C8(4U, 0x8019D800, 0xB4A9);
        func_800462C8(6U, 0x8019F800, 0xB4AD);
        temp_t5 = (D_800E6280.unk_110D + 1) & 0xFF;
        var_v0 = temp_t5;
        D_800E6280.unk_110D = temp_t5;
        break;
    case 11:
    case 12:
    case 13:
    case 14:
        var_v0 = cd_read_sync_status();
        if (var_v0 & 1) {
            temp_t7 = (D_800E6280.unk_110D + 1) & 0xFF;
            var_v0 = temp_t7;
            D_800E6280.unk_110D = temp_t7;
        }
        break;
    case 15:
        func_800462C8(2U, 0x801A2800, 0xB4B3);
        func_800462C8(4U, 0x801A3800, 0xB4B5);
        func_800462C8(3U, 0x801A5800, 0xB4B9);
        func_800462C8(0xAU, 0x801A7000, 0xB515);
        temp_t8_2 = (D_800E6280.unk_110D + 1) & 0xFF;
        var_v0 = temp_t8_2;
        D_800E6280.unk_110D = temp_t8_2;
        break;
    case 16:
    case 17:
    case 18:
    case 19:
        var_v0 = cd_read_sync_status();
        if (var_v0 & 1) {
            temp_t0_2 = (D_800E6280.unk_110D + 1) & 0xFF;
            var_v0 = temp_t0_2;
            D_800E6280.unk_110D = temp_t0_2;
        }
        break;
    case 20:
        func_800462C8(1U, 0x801AC000, 0xB534);
        func_800462C8(3U, 0x801AC800, 0xB668);
        func_800462C8(3U, 0x801AE000, 0xB66B);
        func_800462C8(3U, 0x801AF800, 0xB66E);
        temp_t1 = (D_800E6280.unk_110D + 1) & 0xFF;
        var_v0 = temp_t1;
        D_800E6280.unk_110D = temp_t1;
        break;
    case 21:
    case 22:
    case 23:
    case 24:
        var_v0 = cd_read_sync_status();
        if (var_v0 & 1) {
            temp_t3 = (D_800E6280.unk_110D + 1) & 0xFF;
            var_v0 = temp_t3;
            D_800E6280.unk_110D = temp_t3;
        }
        break;
    case 25:
        func_800462C8(2U, 0x801B1000, 0xB671);
        func_800462C8(3U, 0x801B2000, 0xB673);
        func_800462C8(3U, 0x801B3800, 0xB676);
        func_800462C8(2U, 0x801B5000, 0xB679);
        temp_t4_2 = (D_800E6280.unk_110D + 1) & 0xFF;
        var_v0 = temp_t4_2;
        D_800E6280.unk_110D = temp_t4_2;
        break;
    case 26:
    case 27:
    case 28:
    case 29:
        var_v0 = cd_read_sync_status();
        if (var_v0 & 1) {
            temp_t6 = (D_800E6280.unk_110D + 1) & 0xFF;
            var_v0 = temp_t6;
            D_800E6280.unk_110D = temp_t6;
        }
        break;
    case 30:
        D_8015EDB4 = (TcActor *) D_8011ED58;
        D_800E7318 = 1;
        D_800E731C = 2;
        var_v0 = func_8004284C();
        break;
    }
    return var_v0;
}
