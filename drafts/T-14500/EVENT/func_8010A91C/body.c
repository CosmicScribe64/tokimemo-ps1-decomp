void func_8010A91C(void) {
    s16 var_v0;
    s32 temp_lo;
    u8 *temp_v1;

    D_800B1746 = 0xB;
    func_80012D2C(1, 0);
    func_8001886C(1);
    func_80010678();
    func_80018944(0);
    func_80012D40(1);
    func_80017E50();
    func_8001FD50();
    func_80036884(&D_8011FB10);
    D_800E96EF = 0;
    D_800E9733 = 0;
    D_80094714 = 0;
    D_80094718 = 0;
    func_80045B14();
    func_800F6000();
    func_8003535C();
    func_8003580C();
    func_8004C360();
    D_800F19AB = 0;
    func_80078970(D_80094748, D_800B0929);
    func_8004CDDC();
    D_80120678 = D_80120628;
    D_8012067C = D_80120648;
    D_80120680 = D_80120668;
    if (D_800EECB0 == 1) {
        D_800B096E -= 5;
        var_v0 = 0;
        do {
            temp_lo = var_v0 * 0x34;
            var_v0 += 1;
            temp_v1 = &D_800B0860[temp_lo];
            if (*(s32 *)((unsigned char *)temp_v1 + (432)) & 2) {
                *(s16 *)((unsigned char *)temp_v1 + (422)) = (s16) (*(s16 *)((unsigned char *)temp_v1 + (422)) - 1);
            }
        } while (var_v0 < 0xA);
        func_8004C250(D_800B0860, 0x34);
    }
    func_80011DFC();
}
