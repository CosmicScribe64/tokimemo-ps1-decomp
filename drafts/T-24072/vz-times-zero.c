/* NON_MATCHING: T-24072, private source-only diagnostic; production remains ASM. */
void func_8014354C(void) {
    s32 model;
    TaiikuBig *record;
    u32 *object;

    model = 0x801F9004;
    func_8009B3C0(model);
    model += 4;
    model += 4;
    /* FAKE: initializer/do line grouping reproduces setup order; original line layout unknown. T-24072; transferred from verified T-24064. */
    object = D_80122640; record = D_801227A0; do {
        func_8009B430(model, (u8 *)object, 0);
        object[1] = (u32)record;
        record++;
        object += 4;
        object[-4] = 0x80000000;
    } while (record != D_80122CA0);
    func_8009B430(model, (u8 *)D_80122640, 0);
    D_80122640[1] = (u32)D_801227A0;
    D_80122640[0] = 0;
    D_80150130 = -0x60;
    D_80150134 = 0x20;
    D_80150138 = -0x300;
    /* FAKE DIAGNOSTIC: test the family zero-store idiom without an aggregate alias. T-24072. */
    D_8015013C.vz *= 0;
    D_8015013C.vx = 0;
    D_8015013C.vy = 0;
}
