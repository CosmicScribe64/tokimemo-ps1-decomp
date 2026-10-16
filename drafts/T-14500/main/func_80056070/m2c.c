extern u8 D_800B3938;

void func_80056070(u8 *buf, s32 arg1) {
    s32 var_v1;
    u8 *var_v0;

    var_v0 = buf;
    var_v1 = 0;
    do {
        var_v1 += 4;
        var_v0->unk0 = 0;
        *(s8 *)((unsigned char *)var_v0 + (1)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (2)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (3)) = 0;
        var_v0 += 4;
    } while (var_v1 != 0x14);
    strcpy(buf, &D_800B3938);
    switch (arg1) {
    case 0:
        strcat(buf, "0");
        return;
    case 1:
        strcat(buf, "1");
        break;
    case 2:
        strcat(buf, "2");
        break;
    case 3:
        strcat(buf, "3");
        break;
    case 4:
        strcat(buf, "4");
        break;
    case 5:
        strcat(buf, "5");
        break;
    case 6:
        strcat(buf, "6");
        break;
    case 7:
        strcat(buf, "7");
        break;
    case 8:
        strcat(buf, "8");
        break;
    case 9:
        strcat(buf, "9");
        break;
    case 10:
        strcat(buf, "A");
        break;
    case 11:
        strcat(buf, "B");
        break;
    case 12:
        strcat(buf, "C");
        break;
    case 13:
        strcat(buf, "D");
        break;
    case 14:
        strcat(buf, "E");
        break;
    case 15:
        strcat(buf, "F");
        break;
    default:
        return;
    }
}
