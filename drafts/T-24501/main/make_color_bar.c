/* T-24501 draft: full-TU resolved edit cost330, same instruction count. Uses
 * the Pix2Out type defined in src/main/8004AD60.c. Pointer and masked-color
 * range priorities differ; indexed and single-pixel loop forms did not help. */
void make_color_bar(void *arg0, s32 flag) {
    s32 i;
    s32 j;
    s32 k;
    Pix2Out *p;
    p = arg0;
    for (j = 0, i = -1; i < 31; i += 2, j += 2) {
        p[0].r1 = j;
        p[16].r1 = j;
        p[32].r1 = 0;
        p[48].r1 = 0;
        p[64].r1 = j;
        p[80].r1 = j;
        p[96].r1 = 0;
        p[112].r1 = 0;
        p[0].r0 = i;
        p[16].r0 = i;
        p[32].r0 = 0;
        p[48].r0 = 0;
        p[64].r0 = i;
        p[80].r0 = i;
        p[96].r0 = 0;
        p[112].r0 = 0;
        p++;
    }
    p = arg0;
    for (j = 0, i = -1; i < 31; i += 2, j += 2) {
        p[0].g1 = j;
        p[16].g1 = j;
        p[32].g1 = j;
        p[48].g1 = j;
        p[64].g1 = 0;
        p[80].g1 = 0;
        p[96].g1 = 0;
        p[112].g1 = 0;
        p[0].g0 = i;
        p[16].g0 = i;
        p[32].g0 = i;
        p[48].g0 = i;
        p[64].g0 = 0;
        p[80].g0 = 0;
        p[96].g0 = 0;
        p[112].g0 = 0;
        p++;
    }
    p = arg0;
    for (j = 0, i = -1; i < 31; i += 2, j += 2) {
        p[0].b1 = j;
        p[16].b1 = 0;
        p[32].b1 = j;
        p[48].b1 = 0;
        p[64].b1 = j;
        p[80].b1 = 0;
        p[96].b1 = j;
        p[112].b1 = 0;
        p[0].b0 = i;
        p[16].b0 = 0;
        p[32].b0 = i;
        p[48].b0 = 0;
        p[64].b0 = i;
        p[80].b0 = 0;
        p[96].b0 = i;
        p[112].b0 = 0;
        p++;
    }
    p = arg0;
    if (flag) {
        for (k = 0; k != 128; k++) {
            p[k].x1 = 1;
            p[k].x0 = 1;
        }
    } else {
        for (k = 0; k != 128; k++) {
            p[k].x1 = 0;
            p[k].x0 = 0;
        }
    }
}
