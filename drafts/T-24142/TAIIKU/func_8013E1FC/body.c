/* NON_MATCHING: T-24142 hand-structured draft */
extern s32 D_80122DD4;
void func_8013E1FC(void) {
    s32 padt[3];
    s32 q[4];
    s32 p[4];
    s32 v;
    s32 ox;
    s32 oy;
    s32 i;
    s32 k;
    s32 cx;
    s32 cy;

    for (i = 0; i < 0x40; i++) {
        q[1] = q[0] = -0x140;
        q[3] = q[2] = -0xF0;
        p[0] = i << 3;
        p[1] = (i + 1) << 3;
        for (k = 0; k < 4; k++) {
            q[k] += *(s32 *)D_80122DD0;
        }
        for (k = 0; k < 2; k++) {
            p[k] += D_80122DD4;
        }
        for (k = 0; k < 2; k++) {
            p[k] = (func_800A0140(0x2AA) * p[k]) >> 12;
        }
        p[2] = p[0];
        p[3] = p[1];
        cx = (q[0] + q[2]) >> 1;
        cy = (q[1] + q[3]) >> 1;
        for (k = 0; k < 2; k++) {
            v = p[k << 1];
            if (v > 0 || p[(k << 1) + 1] > 0) {
                if (cx + (q[(k << 1) + 1] * 0) < q[k << 1]) {
                    q[k << 1] = q[k << 1] + v + cx;
                }
                if (q[k << 1] < cx) {
                    q[k << 1] = q[k << 1] - v + cx;
                }
                v = q[(k << 1) + 1];
                if (cy < v) {
                    v = v + p[(k << 1) + 1] + cy;
                    q[(k << 1) + 1] = v;
                }
                if (v < cy) {
                    q[(k << 1) + 1] = v - p[(k << 1) + 1] + cy;
                }
            }
        }
        for (k = 0; k < 4; k++) {
            q[k] = -q[k];
        }
        for (k = 0; k < 4; k++) {
            p[k] -= 0x18;
        }
        v = D_80149968.unk_1C[0] >> 2;
        for (k = 0; k < 4; k++) {
            q[k] -= v;
        }
        for (k = 0; k < 2; k++) {
            if ((q[k << 1] >= -0xA0 && q[k << 1] <= 0xA0 && p[k << 1] >= -0x17 && p[k << 1] <= 0x78) ||
                (q[(k << 1) + 1] >= -0xA0 && q[(k << 1) + 1] <= 0xA0 && p[(k << 1) + 1] >= -0x17 && p[(k << 1) + 1] <= 0x78)) {
                func_80049FF0((s16)q[k << 1], (s16)p[k << 1], (s16)q[(k << 1) + 1], (s16)p[(k << 1) + 1], 0xFFFFFF, 0xFFFFFF, 0xFA);
            }
        }
    }
    for (i = 0; i < 0x40; i++) {
        ox = -0x140;
        oy = 0;
        func_80086204(i * 0x10, &ox, &oy, &q[0], &p[0]);
        func_80086204((i + 1) * 0x10, &ox, &oy, &q[1], &p[1]);
        ox = -0xF0;
        oy = 0;
        func_80086204(i * 0x10, &ox, &oy, &q[2], &p[2]);
        func_80086204((i + 1) * 0x10, &ox, &oy, &q[3], &p[3]);
        for (k = 0; k < 4; k++) {
            q[k] += *(s32 *)D_80122DD0;
            p[k] += D_80122DD4;
        }
        for (k = 0; k < 4; k++) {
            p[k] = (func_800A0140(0x2AA) * p[k]) >> 12;
        }
        cx = (q[0] + q[2]) >> 1;
        cy = (q[1] + q[3]) >> 1;
        for (k = 0; k < 2; k++) {
            v = p[k << 1];
            if (v > 0 || p[(k << 1) + 1] > 0) {
                if (cx + (q[(k << 1) + 1] * 0) < q[k << 1]) {
                    q[k << 1] = q[k << 1] + v + cx;
                }
                if (q[k << 1] < cx) {
                    q[k << 1] = q[k << 1] - v + cx;
                }
                v = q[(k << 1) + 1];
                if (cy < v) {
                    v = v + p[(k << 1) + 1] + cy;
                    q[(k << 1) + 1] = v;
                }
                if (v < cy) {
                    q[(k << 1) + 1] = v - p[(k << 1) + 1] + cy;
                }
            }
        }
        for (k = 0; k < 4; k++) {
            q[k] = -q[k];
        }
        for (k = 0; k < 4; k++) {
            p[k] -= 0x18;
        }
        v = D_80149968.unk_1C[0] >> 2;
        for (k = 0; k < 4; k++) {
            q[k] -= v;
        }
        for (k = 0; k < 2; k++) {
            if ((q[k << 1] >= -0xA0 && q[k << 1] <= 0xA0 && p[k << 1] >= -0x17 && p[k << 1] <= 0x78) ||
                (q[(k << 1) + 1] >= -0xA0 && q[(k << 1) + 1] <= 0xA0 && p[(k << 1) + 1] >= -0x17 && p[(k << 1) + 1] <= 0x78)) {
                func_80049FF0((s16)q[k << 1], (s16)p[k << 1], (s16)q[(k << 1) + 1], (s16)p[(k << 1) + 1], 0xFFFFFF, 0xFFFFFF, 0xFA);
            }
        }
    }
}
