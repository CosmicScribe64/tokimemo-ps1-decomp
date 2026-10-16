extern s16 D_800CA220;
extern s16 D_800CA226;
extern s16 D_800CA228;
extern s16 D_800CA230;
extern s16 D_800CA232;
extern s16 D_800E638E;
extern u8 D_800E71DF;
extern u8 D_800E738A;

void func_8013BEB8(void) {
    if (D_800E71DF != 6) {
        func_8004284C();
        D_800E738A += 8;
        return;
    }
    if (D_800E638E >= 0x96) {
        D_800CA148 += 2;
        func_80042940(0x2D);
        return;
    }
    *D_800CA22C = 2;
    D_800CA21E = 3;
    *D_800CA21C = 3;
    D_800CA230 = 2;
    *D_800CA234 = 2;
    D_800CA226 = 3;
    *D_800CA224 = 3;
    D_800CA232 = 2;
    D_800CA220 = 3;
    D_800CA228 = 3;
    func_8004284C();
}
