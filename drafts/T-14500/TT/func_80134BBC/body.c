void func_80134BBC(void) {
    TtActor68 *object;
    s32 i;
    s32 n;
    u8 *g;

    object = (TtActor68 *)D_80158AA0;
    for (i = 0, n = 0x80; i != n; i++, object++) {
        if (object->unk_00 != 0) {
            if (object->unk_02 == 0xAD) {
                func_80134ADC(object);
            } else if (object->unk_02 == 0xAC) {
                func_801349B0(object);
            }
            func_80134924((u8 *)object);
            if (object->unk_02 == 0xAB) {
                g = D_80158A60;
                object->unk_3E = object->unk_41 / 12 + 0xF;
                object->unk_3E += *(u16 *)(g + 0xA) & 1;
                object->unk_41++;
                if (object->unk_41 >= 0x31) {
                    object->unk_41 = 0x30;
                }
            } else {
                func_80132000((u8 *)object + 0x38, (u8 *)object + 0x56, (u8 *)object + 0x57);
            }
        }
    }
}
