void strCallback(void) {
    RECT rect;
    s32 odd;

    odd = D_800B5900 & 1;
    if (odd != 0 && D_80125C5C != 0) {
        func_80088210();
        odd = D_800B5900 & 1;
        D_80125C5C = 0;
    }
    rect = D_80125C58->dec.slice;
    if (odd != 0) {
        rect.x += (D_800B58F8 * 3 + 2) / 3;
        rect.y += (D_800B58FC * 3 + 2) / 3;
    } else {
        rect.x += D_800B58F8;
        rect.y += D_800B58FC;
    }
    func_8009C884(&rect, D_80125C58->dec.imgbuf);
    func_8009C674(0);
    D_80125C58->dec.slice.x += D_80125C58->dec.slice.w;
    if (D_80125C58->dec.slice.x < D_80125C58->dec.rect[D_80125C58->dec.rectid].x + D_80125C58->dec.rect[D_80125C58->dec.rectid].w) {
        func_80086944(D_80125C58->dec.imgbuf, D_80125C58->dec.slice.w * D_80125C58->dec.slice.h / 2);
        return;
    }
    D_80125C58->dec.isdone = 1;
    if (D_80125C58->dec.rectid != 0) {
        D_80125C58->dec.rectid = 0;
    } else {
        D_80125C58->dec.rectid = 1;
    }
    D_80125C58->dec.slice.x = D_80125C58->dec.rect[D_80125C58->dec.rectid].x;
    D_80125C58->dec.slice.y = D_80125C58->dec.rect[D_80125C58->dec.rectid].y;
}
