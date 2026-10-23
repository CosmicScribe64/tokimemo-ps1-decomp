/* T-25104 control draft, 7 rows (not the guard; the guard in src/main/8004C3F0.c has 4).
 * Same body as the guard plus two FAKE changes: padC after p (one more declared word) and
 * `if (&p) {}` at the end (p address-taken, so it is spilled at its declared home 0x6C).
 * Frame 0xA8 and every stack slot then match the original; left are the latch load of p in
 * a ugen temporary ($t4) where the original has a uopt colour ($a3), and the ugen ring shift
 * after it ($t5/$t6 against $t4/$t5 at the last goto_tpage). Paste over the guard body. */
u16 search_tpage_multi();
void mod_set_gpu(s32 idx, s32 frame, s32 x, s32 y, s32 cx, s32 cy, s32 pri) {
    s32 padA; /* FAKE: T-25006, padA and padB put row and mode at the original stack homes */
    s32 padB;
    u32 row;
    SPRT_8 *s;
    u32 col;
    u32 *data;
    u32 uv;
    u32 page;
    u32 clut;
    u16 tpage;
    u16 last;
    s32 bright;
    s32 mode;
    s32 ypos;
    u8 rows;
    u8 cols;
    ModRec14 *p;
    s32 padC;

    bright = func_8004901C();
    p = (ModRec14 *)(D_801217A0 + idx * 0x14);
    switch (p->unk_08) {
    case 0:
        mode = 1;
        break;
    case 1:
        mode = 2;
        break;
    case 2:
        mode = 2;
        break;
    case 3:
        mode = 1;
        break;
    case 4:
        mode = 1;
        break;
    case 5:
        mode = 1;
        break;
    default:
        mode = 1;
        break;
    }
    last = 0xFFFF;
    rows = p->unk_0D;
    cols = p->unk_0C;
    data = rows * frame * cols + p->unk_04;
    row = 0;
    if (rows != 0) {
        do {
            col = 0;
            if (cols != 0) {
                do {
                    uv = (*data & 0xFF0000) >> 16;
                    page = (*data & 0xFF000000) >> 24;
                    clut = (*data & 0xFF00) >> 8;
                    s = (SPRT_8 *)GetWorkBase(0x10, D_8011ECA0);
                    func_8009F07C(s);
                    SetSemiTrans(s, 0);
                    s->x0 = x + 0xA0 + col * 8;
                    ypos = row * 8 + y + 0x168;
                    s->y0 = ypos - D_8011ECA0 * 0xF0;
                    /* FAKE: T-25006, one line so as1 keeps the u0/v0 order */
                    s->u0 = (uv & 0xF) * 8; s->v0 = ((uv & 0xF0) >> 4) * 8;
                    s->b0 = bright;
                    s->g0 = bright;
                    s->r0 = bright;
                    tpage = search_tpage_multi(p->unk_00[page], mode);
                    if (last != tpage) {
                        if (last != 0xFFFF) {
                            goto_tpage((last & 0xF) << 6, (last & 0x10) << 4, pri + 1, 0, 0);
                        }
                        last = tpage;
                    }
                    s->clut = func_8009ED74(cx + clut * 16, cy);
                    AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + pri * 4 + 4, s);
                    col++;
                    data++;
                    cols = p->unk_0C;
                } while (col < cols);
                rows = p->unk_0D;
            }
            row++;
        } while (row < rows);
    }
    goto_tpage((last & 0xF) << 6, (last & 0x10) << 4, pri + 1, 0, 0);
    if (&p) {}
}
