void func_80134768(s16 x, s16 y, s32 speed, s32 angle) {
    TtActor68 *object;

    object = (TtActor68 *)func_80133BA4(D_80158AA0, D_80158AA0 + 0x3400);
    if (object != 0) {
        object->unk_00 = 1;
        object->unk_02 = 0xAB;
        object->unk_04 = 0x40;
        object->unk_08 = 0;
        object->unk_06 = 0;
        object->unk_0A = 0;
        object->unk_0F = 0;
        object->unk_0E = 0;
        object->unk_0D = 0;
        object->unk_10 = 1;
        object->unk_20 = x << 16;
        object->unk_24 = y << 16;
        object->unk_28 = func_800A0140(angle) * speed;
        object->unk_2C = func_800A0070(angle) * speed;
        object->unk_38 = D_80151A50;
        object->unk_3C = 0;
        object->unk_3D = 9;
        object->unk_3E = 0x14;
        object->unk_40 = 0x60;
        object->unk_41 = 0;
        object->unk_4C = (u8 *)&D_80155B30[5];
    }
}
