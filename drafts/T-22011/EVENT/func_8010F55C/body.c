/* T-22011 best form: 138 diff lines (about 69 rows) against the original; needs the 21 prototypes `void func_80111E60();` ... `void func_80114948();` (case targets, see the case list) and the tail FnTbl61 D_801243B8 table from EVENT.h. Not adopted. */
s32 func_8010F55C(void) {
    FnTbl61 tbl;
    void (**p)();
    void (*fn)();
    u8 idx;

    tbl = D_801243B8;
    idx = D_800B1AF6;
    p = &tbl.f[idx];
    fn = *p;
    if (fn == func_80111E60) {
        switch (D_801243B0) {
        case 1:
            fn = func_80111F04;
            *p = fn;
            break;
        case 2:
            fn = func_801139CC;
            *p = fn;
            break;
        case 3:
            fn = func_8011220C;
            *p = fn;
            break;
        case 4:
            fn = func_80112460;
            *p = fn;
            break;
        case 5:
            fn = func_80112750;
            *p = fn;
            break;
        case 6:
            fn = func_80112930;
            *p = fn;
            break;
        case 7:
            fn = func_80112BEC;
            *p = fn;
            break;
        case 8:
            fn = func_801130F0;
            *p = fn;
            break;
        case 9:
            fn = func_80113300;
            *p = fn;
            break;
        case 10:
            fn = func_80113520;
            *p = fn;
            break;
        case 11:
            fn = func_801136FC;
            *p = fn;
            break;
        case 12:
            fn = func_801138D8;
            *p = fn;
            break;
        case 13:
            fn = func_80113A40;
            *p = fn;
            break;
        case 14:
            fn = func_80113CA0;
            *p = fn;
            break;
        case 15:
            fn = func_80113FD4;
            *p = fn;
            break;
        case 16:
            fn = func_801142E8;
            *p = fn;
            break;
        case 17:
            fn = func_801144AC;
            *p = fn;
            break;
        case 18:
            fn = func_801146B4;
            *p = fn;
            break;
        case 19:
            fn = func_80114784;
            *p = fn;
            break;
        case 20:
            fn = func_80114948;
            *p = fn;
            break;
        }
    }
    fn(0x80, idx);
}
