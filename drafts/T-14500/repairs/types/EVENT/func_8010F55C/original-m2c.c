? func_80111F04(?, u8);                             /* extern */
? func_8011220C(?, u8);                             /* extern */
? func_80112460(?, u8);                             /* extern */
? func_80112750(?, u8);                             /* extern */
? func_80112930(?, u8);                             /* extern */
? func_80112BEC(?, u8);                             /* extern */
? func_801130F0(?, u8);                             /* extern */
? func_80113300(?, u8);                             /* extern */
? func_80113520(?, u8);                             /* extern */
? func_801136FC(?, u8);                             /* extern */
? func_801138D8(?, u8);                             /* extern */
? func_801139CC(?, u8);                             /* extern */
? func_80113A40(?, u8);                             /* extern */
? func_80113CA0(?, u8);                             /* extern */
? func_80113FD4(?, u8);                             /* extern */
? func_801142E8(?, u8);                             /* extern */
? func_801144AC(?, u8);                             /* extern */
? func_801146B4(?, u8);                             /* extern */
? func_80114784(?, u8);                             /* extern */
? func_80114948(?, u8);                             /* extern */
extern ? D_801243B8;
extern ? func_80111E60;

void func_8010F55C(void) {
    ? (**temp_v0)(?, u8);
    ? (*var_v1)(?, u8);

    M2C_MEMCPY_ALIGNED(&sp30[0], &D_801243B8, 0xF0);
    temp_v0 = &(&sp30[0])[D_800B1AF6];
    *(s32 *)((unsigned char *)(&sp30[0]) + (240)) = (s32) *(s32 *)((unsigned char *)(&D_801243B8) + (240));
    var_v1 = *temp_v0;
    if (&func_80111E60 == var_v1) {
        switch (D_801243B0) {
        case 1:
            var_v1 = func_80111F04;
block_23:
            *temp_v0 = var_v1;
            break;
        case 2:
            var_v1 = func_801139CC;
            goto block_23;
        case 3:
            var_v1 = func_8011220C;
            goto block_23;
        case 4:
            var_v1 = func_80112460;
            goto block_23;
        case 5:
            var_v1 = func_80112750;
            goto block_23;
        case 6:
            var_v1 = func_80112930;
            goto block_23;
        case 7:
            var_v1 = func_80112BEC;
            goto block_23;
        case 8:
            var_v1 = func_801130F0;
            goto block_23;
        case 9:
            var_v1 = func_80113300;
            goto block_23;
        case 10:
            var_v1 = func_80113520;
            goto block_23;
        case 11:
            var_v1 = func_801136FC;
            goto block_23;
        case 12:
            var_v1 = func_801138D8;
            goto block_23;
        case 13:
            var_v1 = func_80113A40;
            goto block_23;
        case 14:
            var_v1 = func_80113CA0;
            goto block_23;
        case 15:
            var_v1 = func_80113FD4;
            goto block_23;
        case 16:
            var_v1 = func_801142E8;
            goto block_23;
        case 17:
            var_v1 = func_801144AC;
            goto block_23;
        case 18:
            var_v1 = func_801146B4;
            goto block_23;
        case 19:
            var_v1 = func_80114784;
            goto block_23;
        case 20:
            var_v1 = func_80114948;
            goto block_23;
        }
    }
    var_v1(0x80, D_800B1AF6);
}
