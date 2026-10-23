void mod_trans_vram(s32 idx, s32 frame, s32 page, s32 x, s32 y) {
    u32 col;
    u32 *data;
    u32 row;
    u32 uv;
    u32 bank;
    ModRec14 *p;
    u8 buf[256];
    u8 ratio;
    u8 rows;
    u8 cols;
    u32 xh;
    s32 ypos;
    s32 size;

    p = (ModRec14 *)(D_801217A0 + idx * 0x14);
    if (p->unk_0E[0] == 8) {
        size = 8;
    } else {
        size = 16;
    }
    switch (p->unk_08) {
    case 0:
        ratio = 1;
        break;
    case 1:
        ratio = 2;
        break;
    case 2:
        ratio = 2;
        break;
    case 3:
        ratio = 1;
        break;
    case 4:
        ratio = 1;
        break;
    case 5:
        ratio = 1;
        break;
    default:
        ratio = 1;
        break;
    }
    xh = x / 2;
    rows = p->unk_0D;
    cols = p->unk_0C;
    data = rows * frame * cols + p->unk_04;
    row = 0;
    if (rows != 0) {
        do {
            col = 0;
            if (cols != 0) {
                ypos = ((page & 0x10) << 4) + ((page & 0x20) << 2) + y + row * size;
                do {
                    uv = *data;
                    uv &= 0xFF0000;
                    bank = *data & 0xFF000000;
                    get_fnt(buf, (u8 *)p->unk_00[bank >> 24], uv >> 16, ratio, size, *data & 0xFF);
                    LoadSquare(((page & 0xF) << 6) + xh / ratio + (u32)((s32)(col * size) / 2) / ratio, ypos, (u32)(size / 2) / ratio, size, buf);
                    data++;
                    col++;
                    cols = p->unk_0C;
                } while (col < cols);
                rows = p->unk_0D;
            }
            row++;
        } while (row < rows);
    }
}
