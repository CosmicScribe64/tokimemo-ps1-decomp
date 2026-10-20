/* T-24301 draft: GsBG/GsMAP/GsCELL setup (types as in include/ovl/OPTION.h). The original multiplies
 * h * 0x78 with multu (the 0x78 constant is shared with map->cellh) and has a 0x70 frame. */
typedef struct BgCell8 {
    u8 u;
    u8 v;
    u16 cba;
    u16 flag;
    u16 tpage;
} BgCell8;

typedef struct BgMap10 {
    u8 cellw;
    u8 cellh;
    u16 ncellw;
    u16 ncellh;
    u8 pad_06[2];
    BgCell8 *base;
    u16 *index;
} BgMap10;

extern BgMap10 D_80122120[2];
extern BgCell8 D_80122140[2][64];
extern u16 D_80122540[2][64];

void func_8004A1A0(s32 *tex, s32 slot, s32 count, s16 w, s32 h, s32 clut, s32 x, s32 y, s32 flag, s32 tp) {
    SpriteSlot24 *bg;
    BgMap10 *map;
    BgCell8 *cells;
    u16 *index;
    s32 row;
    s32 col;
    s32 n;

    bg = &((SpriteSlot24 *)&D_801220D0)[slot];
    bg->unk_04 = x;
    map = &D_80122120[slot];
    bg->unk_00 = 0x01000000;
    bg->unk_0A = h * 0x78;
    bg->unk_08 = w << 7;
    bg->unk_14 = (u32)&D_80122120[slot];
    bg->unk_1C = 0x1000;
    bg->unk_1E = 0x1000;
    cells = D_80122140[slot];
    index = D_80122540[slot];
    bg->unk_0C = 0;
    bg->unk_0E = 0;
    bg->unk_10 = 0x80;
    bg->unk_11 = 0x80;
    bg->unk_12 = 0x80;
    bg->unk_18 = 0;
    bg->unk_1A = 0;
    bg->unk_20 = 0;
    map->cellw = 0x80;
    map->cellh = 0x78;
    map->ncellw = w;
    map->base = cells;
    map->index = index;
    map->ncellh = h;
    bg->unk_06 = y;
    for (row = 0; row < h; row++) {
        for (col = 0; col < w; col++) {
            load_tpage_buf_lock(tp + row * 32 + col, tex[row * w + col], 1);
        }
    }
    for (row = 0; row < h; row++) {
        for (col = 0; col < w; col++) {
            n = row * w + col;
            index[n] = n;
            cells[n].u = 0;
            cells[n].v = row * 0x78;
            cells[n].tpage = tp + col;
            cells[n].cba = func_8009ED74(0, clut);
            cells[n].flag = 0;
            if (n > 0x40) {
                printf("BG_ERROR:p_line.c \n");
            }
        }
    }
    D_800E6280.unk_10EC[slot] = flag;
}
