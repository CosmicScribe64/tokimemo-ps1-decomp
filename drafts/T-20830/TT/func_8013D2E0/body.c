void func_8013D2E0(void) {
    u8 *c;
    TtDrawBuffer80B8 *p;
    TILE *v;

    c = D_80158A64;
    p = ((TtDrawSelect0C *)D_80158A68)->index + (TtDrawBuffer80B8 *)D_80158AB4;
    v = (TILE *)p->cursor;
    p->cursor = (DR_MODE *)(((TILE *)p->cursor) + 1);
    ((TtOtTag *)v)->len = 3, v->code = 0x61;
    v->r0 = c[0x13], v->g0 = c[0x14], v->b0 = c[0x15];
    v->x0 = -0x80, v->y0 = 8;
    v->w = 0x100, v->h = 0xE8;
    ((TtOtTag *)v)->addr = *(u32 *)&p->ordering[15];
    (&p->ordering[15])->addr = (u32)v;
}
