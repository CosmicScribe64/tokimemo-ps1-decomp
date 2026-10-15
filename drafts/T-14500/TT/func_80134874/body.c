/* NON_MATCHING: T-10720, T-11620, 14 differing lines: constant 1 is in v1, original has it in a1 (T-0018 phantom registers, same as func_80134768); the lui of D_80155B4E is hoisted early and sh 0x58 late in the original. unk_60 before unk_4C gives the original's t3/t4/t5 (T-11620). */
void func_80134874(x, y)
s16 x;
s16 y;
{
    TtActor68 *object;
    u8 *source;

    source = D_80158A8C;
    object = (TtActor68 *)func_80133BA4(D_80158AA0, D_80158AA0 + 0x3400);
    if (object != 0) {
        object->unk_00 = 1;
        object->unk_02 = 0xAD;
        object->unk_04 = 0x20;
        object->unk_0A = 0;
        object->unk_08 = 0;
        object->unk_06 = 0;
        object->unk_0F = 0;
        object->unk_0E = 0;
        object->unk_0D = 0;
        object->unk_10 = 1;
        object->unk_20 = x << 16;
        object->unk_28 = 0;
        object->unk_24 = y << 16;
        object->unk_58 = 0;
        object->unk_2C = 0;
        object->unk_60 = *(s16 *)(source + 0x14);
        object->unk_4C = (u8 *)&D_80155B30[5];
    }
}
