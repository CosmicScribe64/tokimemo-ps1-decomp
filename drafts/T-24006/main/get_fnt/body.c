void get_fnt(u8 *dst, u8 *font, s32 glyph, s32 divisor, s32 size, u8 flags) {
    u8 buf[256];
    s32 row;
    s32 offset;
    s32 per_row;
    s32 flipped;
    s32 count;
    u8 *src;
    /* FAKE: unused pointer slots retained only for the exploratory frame. T-24006 */
    u8 *to;
    u8 *from;

    row = 0;
    if (size > 0) {
        per_row = 128 / size;
        offset = 0;
        do {
            memcpy(dst + offset / divisor, (glyph % per_row) * size / divisor + font + (((glyph / per_row) * size + row) << 7) / divisor, size / divisor);
            row++;
            offset += size;
        } while (row != size);
    }
    if (flags & 1) {
        memcpy(buf, dst, 256);
        flipped = size - 1;
        if (size > 0) {
            src = buf;
            count = 1;
            do {
                memcpy(dst + flipped * size, src, size);
                row = count;
                flipped--;
                src += size;
                count++;
            } while (row != size);
        }
    }
    if (flags & 2) {
        row = 0;
        memcpy(buf, dst, 256);
        for (row = 0; row < size; row++) {
            dst[row * size + 4] = buf[row * size + 3];
            dst[row * size + 5] = buf[row * size + 2];
            dst[row * size + 6] = buf[row * size + 1];
            dst[row * size + 7] = buf[row * size];
            dst[row * size] = buf[row * size + 7];
            dst[row * size + 1] = buf[row * size + 6];
            dst[row * size + 2] = buf[row * size + 5];
            dst[row * size + 3] = buf[row * size + 4];
        }
    }
}
