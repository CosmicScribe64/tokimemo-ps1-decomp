/* T-24037: source-only six-edit near miss; not a C match. */
void func_80102E54(void) {
    DisplayActor44 *m = (DisplayActor44 *)D_800E9620;
    s16 i;

    D_80120678 = D_80120450;
    D_8012067C = D_8012046C;
    D_80120680 = D_80120488;
    D_800B0A04[5].unk_02 += 2;
    D_800B0A04[5].unk_06 += 1;
    D_800B0A04[5].unk_0A -= 0x14;
    func_8004C250();
    func_80102610();
    func_80012D64(D_801226E8, 0x11, 1, 2, 0);
    func_8004C46C(D_801226EC, D_801226F0, D_801226F4, D_801226F8, D_801226FC, D_80122700);
    func_8004C6B0(D_801226DC, D_801226E0, D_801226D8, D_801226E4);
    m[96].unk_04 = m[97].unk_04 = 8;
    for (i = 0; i < 8; i++) {
        D_8012274C[i] = 0;
    }
    for (i = 0; i < 8; i++) {
        m[98 + i] = ((DisplayActor44 *)D_800EAFA0)[0];
        m[98 + i].unk_16 = 5;
        m[98 + i].unk_26 = D_80122754[i] - 0xA0;
        m[98 + i].unk_2A = D_80122764[i] - 0x78;
        D_80122774[i] = func_800789A0() % 120;
    }
    m[98].unk_16 = 4;
    func_80011DFC();
}
