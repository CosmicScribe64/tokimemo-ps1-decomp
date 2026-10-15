extern s8 D_80080714;
extern s8 D_800B086A;
extern s8 D_800B1888;
extern s32 D_800B18D0;
extern s32 D_800E9E50;
extern s32 D_800E9E94;
extern s32 D_800EC1B4;

void func_8010F340(void) {
    func_80012D2C(1, 0);
    func_8001886C(1);
    func_80010678();
    func_80018944(0);
    func_80012D40(1);
    func_8004CDDC();
    D_800B086A = 0x80;
    D_800F19AB = 0;
    D_80080714 = 0;
    D_800B1888 = 0;
    D_800B18D0 = 1;
    func_8001FD50();
    func_800201FC(1);
    func_8003535C();
    func_8003580C();
    D_801243B0 = D_800B0F2C;
    D_800EECB4 = (s32) D_800B0F30;
    D_80094730 = 0;
    D_801252E4 = 1;
    if (!(D_800B1AE4 & 3) && (D_800B0F30 != 2)) {
        D_801252E4 = 0;
    }
    D_801243B4 = 0;
    D_80124384 = 0;
    D_80124388 = 0;
    D_800B1746 = D_800B0F30;
    func_8004BC20((s32) D_800B0F30);
    D_800E9E63 = 0;
    D_800E9E94 |= 0x81000000;
    D_800E9E50 |= 0x81000000;
    *D_800EC190 |= 0x81000000;
    D_800EC1B4 |= 0x81000000;
    func_8004C360();
    func_80011DFC();
}
