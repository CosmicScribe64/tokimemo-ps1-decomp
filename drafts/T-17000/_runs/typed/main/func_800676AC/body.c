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
    ((DisplayActor44 *)D_8011ECD0)[27].unk_01 = 1;
    ((DisplayActor44 *)D_8011ECD0)[27].unk_02 = 0x40;
    ((DisplayActor44 *)D_8011ECD0)[27].unk_38 = 0x40000000;
    ((DisplayActor44 *)D_8011ECD0)[27].unk_03 = 0x84;
    *(u8 **)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (1848)) = D_800C5FD4;
    *(u8 **)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (1852)) = D_800C6010;
    *(s32 **)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (1888)) = &D_800C5FCC;
    ((DisplayActor44 *)D_8011ECD0)[27].unk_14 = (s16) D_800C6014;
    ((DisplayActor44 *)D_8011ECD0)[27].unk_04 = 8;
    ((DisplayActor44 *)D_8011ECD0)[27].unk_05 = 1;
    ((DisplayActor44 *)D_8011ECD0)[27].unk_16 = 0;
    ((DisplayActor44 *)D_8011ECD0)[27].unk_18 = (s16) (D_800B5C08 - 1);
    func_80048F64(0x1C);
    ((DisplayActor44 *)D_8011ECD0)[28].unk_01 = 1;
    ((DisplayActor44 *)D_8011ECD0)[28].unk_02 = 0x40;
    ((DisplayActor44 *)D_8011ECD0)[28].unk_38 = 0x40000000;
    ((DisplayActor44 *)D_8011ECD0)[28].unk_03 = 0x84;
    *(u8 **)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (1916)) = D_800C5FD4;
    *(u8 **)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (1920)) = D_800C6010;
    *(s32 **)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (1956)) = &D_800C5FCC;
    ((DisplayActor44 *)D_8011ECD0)[28].unk_14 = (s16) D_800C6014;
    ((DisplayActor44 *)D_8011ECD0)[28].unk_04 = 8;
    ((DisplayActor44 *)D_8011ECD0)[28].unk_05 = 1;
    ((DisplayActor44 *)D_8011ECD0)[28].unk_16 = 0;
    ((DisplayActor44 *)D_8011ECD0)[28].unk_18 = 0xD;
}
