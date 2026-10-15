extern ? D_800E6280;
extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern u8 D_800E62C1;
extern u16 D_800E6374;
extern u8 D_800E652A;
extern u8 D_800E71DE;
extern u8 D_800E71DF;

void func_8013B448(void) {
    u32 temp_t6;
    u8 temp_t8;
    void *var_t6;

    *(u8 *)((unsigned char *)(&D_800E6280 + (D_800E71DE * 8)) + (1886)) = (u8) D_800E71DF;
    *(s8 *)((unsigned char *)(&D_800E6280 + (D_800E71DE * 8)) + (1888)) = 0x15;
    *(u8 *)((unsigned char *)(&D_800E6280 + (D_800E71DE * 8)) + (1890)) = (u8) D_800E62BE;
    *(u8 *)((unsigned char *)(&D_800E6280 + (D_800E71DE * 8)) + (1891)) = (u8) D_800E62BF;
    *(u8 *)((unsigned char *)(&D_800E6280 + (D_800E71DE * 8)) + (1892)) = (u8) D_800E62C0;
    *(u8 *)((unsigned char *)(&D_800E6280 + (D_800E71DE * 8)) + (1893)) = (u8) D_800E62C1;
    temp_t6 = (u16) D_800E6374 >> 0xC;
    switch (temp_t6) {                              /* switch 1; irregular */
    case 7:                                         /* switch 1 */
        *(s8 *)((unsigned char *)(&D_800E6280 + (D_800E71DE * 8)) + (1887)) = 7;
        break;
    case 8:                                         /* switch 1 */
        *(s8 *)((unsigned char *)(&D_800E6280 + (D_800E71DE * 8)) + (1887)) = 8;
        break;
    case 9:                                         /* switch 1 */
        *(s8 *)((unsigned char *)(&D_800E6280 + (D_800E71DE * 8)) + (1887)) = 9;
        break;
    case 10:                                        /* switch 1 */
        *(s8 *)((unsigned char *)(&D_800E6280 + (D_800E71DE * 8)) + (1887)) = 0xA;
        break;
    }
    switch (D_800E71DF) {                           /* switch 2 */
    case 0:                                         /* switch 2 */
        *(s8 *)((unsigned char *)(&D_800E6280 + (D_800E71DE * 8)) + (1889)) = 1;
        break;
    case 1:                                         /* switch 2 */
        *(s8 *)((unsigned char *)(&D_800E6280 + (D_800E71DE * 8)) + (1889)) = 1;
        break;
    case 2:                                         /* switch 2 */
        *(s8 *)((unsigned char *)(&D_800E6280 + (D_800E71DE * 8)) + (1889)) = 1;
        break;
    case 3:                                         /* switch 2 */
        var_t6 = &D_800E6280 + (D_800E71DE * 8);
block_29:
        *(s8 *)((unsigned char *)var_t6 + (1889)) = 1;
        break;
    case 4:                                         /* switch 2 */
        if ((((u8) D_800E652A >> 4) == 7) && (((u16) D_800E6374 >> 0xC) == 7)) {
            *(s8 *)((unsigned char *)(&D_800E6280 + (D_800E71DE * 8)) + (1889)) = 5;
            *(s8 *)((unsigned char *)(&D_800E6280 + (D_800E71DE * 8)) + (1893)) = 0xFF;
        } else {
            *(s8 *)((unsigned char *)(&D_800E6280 + (D_800E71DE * 8)) + (1889)) = 1;
        }
        break;
    case 5:                                         /* switch 2 */
        *(s8 *)((unsigned char *)(&D_800E6280 + (D_800E71DE * 8)) + (1889)) = 1;
        break;
    case 6:                                         /* switch 2 */
        if (((u16) D_800E6374 >> 0xC) == 9) {
            *(s8 *)((unsigned char *)(&D_800E6280 + (D_800E71DE * 8)) + (1889)) = 1;
        } else {
            *(s8 *)((unsigned char *)(&D_800E6280 + (D_800E71DE * 8)) + (1889)) = 0;
        }
        break;
    case 7:                                         /* switch 2 */
        *(s8 *)((unsigned char *)(&D_800E6280 + (D_800E71DE * 8)) + (1889)) = 0;
        break;
    case 8:                                         /* switch 2 */
        *(s8 *)((unsigned char *)(&D_800E6280 + (D_800E71DE * 8)) + (1889)) = 1;
        break;
    case 9:                                         /* switch 2 */
        *(s8 *)((unsigned char *)(&D_800E6280 + (D_800E71DE * 8)) + (1889)) = 5;
        *(s8 *)((unsigned char *)(&D_800E6280 + (D_800E71DE * 8)) + (1893)) = 0xFF;
        break;
    case 10:                                        /* switch 2 */
        *(s8 *)((unsigned char *)(&D_800E6280 + (D_800E71DE * 8)) + (1889)) = 1;
        break;
    case 11:                                        /* switch 2 */
        *(s8 *)((unsigned char *)(&D_800E6280 + (D_800E71DE * 8)) + (1889)) = 1;
        break;
    case 12:                                        /* switch 2 */
        var_t6 = &D_800E6280 + (D_800E71DE * 8);
        goto block_29;
    }
    temp_t8 = (D_800E71DE + 1) & 0xFF;
    D_800E71DE = temp_t8;
    if (temp_t8 >= 0xFFU) {
        D_800E71DE = 0xFE;
    }
}
