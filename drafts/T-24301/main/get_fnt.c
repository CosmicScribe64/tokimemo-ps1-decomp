/* T-24301 draft, ~200 rows: structure right (copy, vertical flip, 8-byte row reversal); the original
 * frame is 0x1A0 with the buffer at sp+0x8C, uses s0-s7 only, keeps the first loop counter itself and
 * unrolls the reversal loop by 2. */
void get_fnt(u8 *dst, u8 *src, s32 code, s32 div, s32 size, u8 flags) {
    u8 buf[0x100];
    s32 i;

    for (i = 0; i < size; i++) {
        memcpy(dst + i * size / div,
               src + code % (0x80 / size) * size / div + ((code / (0x80 / size)) * size + i) * 0x80 / div,
               size / div);
    }
    if (flags & 1) {
        memcpy(buf, dst, 0x100);
        for (i = 0; i < size; i++) {
            memcpy(dst + (size - 1 - i) * size, buf + i * size, size);
        }
    }
    if (flags & 2) {
        memcpy(buf, dst, 0x100);
        for (i = 0; i < size; i++) {
            dst[i * size + 4] = buf[i * size + 3];
            dst[i * size + 5] = buf[i * size + 2];
            dst[i * size + 6] = buf[i * size + 1];
            dst[i * size + 7] = buf[i * size + 0];
            dst[i * size + 0] = buf[i * size + 7];
            dst[i * size + 1] = buf[i * size + 6];
            dst[i * size + 2] = buf[i * size + 5];
            dst[i * size + 3] = buf[i * size + 4];
        }
    }
}
