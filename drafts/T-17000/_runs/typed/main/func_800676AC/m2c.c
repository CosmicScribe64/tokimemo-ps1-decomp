void func_800676AC(s16 arg0) {
    if ((arg0 == 0) || (arg0 >= 0xE)) {
        D_80125C50 = 0x78;
        D_80125C52 = -0x37;
        D_80125C54 = 0xF;
        D_80125C55 = 8;
        D_800B5C08 = 0;
        func_8006764C(0);
        return;
    }
    D_800B5C08 = arg0;
    func_80048F64(0x1B);
    *(s8 *)((unsigned char *)D_8011ECD0 + (1837)) = 1;
    *(s8 *)((unsigned char *)D_8011ECD0 + (1838)) = 0x40;
    *(s32 *)((unsigned char *)D_8011ECD0 + (1892)) = 0x40000000;
    *(s8 *)((unsigned char *)D_8011ECD0 + (1839)) = 0x84;
    *(u8 **)((unsigned char *)D_8011ECD0 + (1848)) = D_800C5FD4;
    *(u8 **)((unsigned char *)D_8011ECD0 + (1852)) = D_800C6010;
    *(s32 **)((unsigned char *)D_8011ECD0 + (1888)) = &D_800C5FCC;
    *(s16 *)((unsigned char *)D_8011ECD0 + (1856)) = (s16) D_800C6014;
    *(s8 *)((unsigned char *)D_8011ECD0 + (1840)) = 8;
    *(s8 *)((unsigned char *)D_8011ECD0 + (1841)) = 1;
    *(s16 *)((unsigned char *)D_8011ECD0 + (1858)) = 0;
    *(s16 *)((unsigned char *)D_8011ECD0 + (1860)) = (s16) (D_800B5C08 - 1);
    func_80048F64(0x1C);
    *(s8 *)((unsigned char *)D_8011ECD0 + (1905)) = 1;
    *(s8 *)((unsigned char *)D_8011ECD0 + (1906)) = 0x40;
    *(s32 *)((unsigned char *)D_8011ECD0 + (1960)) = 0x40000000;
    *(s8 *)((unsigned char *)D_8011ECD0 + (1907)) = 0x84;
    *(u8 **)((unsigned char *)D_8011ECD0 + (1916)) = D_800C5FD4;
    *(u8 **)((unsigned char *)D_8011ECD0 + (1920)) = D_800C6010;
    *(s32 **)((unsigned char *)D_8011ECD0 + (1956)) = &D_800C5FCC;
    *(s16 *)((unsigned char *)D_8011ECD0 + (1924)) = (s16) D_800C6014;
    *(s8 *)((unsigned char *)D_8011ECD0 + (1908)) = 8;
    *(s8 *)((unsigned char *)D_8011ECD0 + (1909)) = 1;
    *(s16 *)((unsigned char *)D_8011ECD0 + (1926)) = 0;
    *(s16 *)((unsigned char *)D_8011ECD0 + (1928)) = 0xD;
}
