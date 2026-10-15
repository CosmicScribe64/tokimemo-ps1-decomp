s32 make_three_select(str)
    u8 *str;
{
    u8 buf[3][0x40];
    s16 i;
    s16 n;
    s16 start;

    n = 0;
    start = 0;
    i = 0;
    for (;;) {
        if (str[i] == 'P' || str[i] == 'G') {
            i--;
        } else if (str[i] == 'q' || str[i] == ' ') {
            str[i] = 0;
            strncpy(buf[n++], &str[start], 0x40);
            start = i + 1;
            i--;
        } else if (str[i] == 0) {
            strncpy(buf[n], &str[start], 0x40);
            break;
        }
        i += 2;
    }
    sprintf(str, "%sq%sq%s", buf[D_800CA2A4[0]], buf[D_800CA2A4[1]], buf[D_800CA2A4[2]]);
}
