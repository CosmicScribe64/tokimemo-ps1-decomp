/* T-24521: source-only reconstruction; does not match. */
void func_8013EC60(void) {
    s32 i;
    s32 middle0;
    s32 middle1;
    s32 x[4];
    s32 y[4];
    s32 row;
    s32 input_x;
    s32 input_y;
    s32 value;
    s32 camera_x;
    s32 camera_y;
    s32 *xp;
    s32 *yp;
    s32 angle;
    s32 first_x;
    s32 first_y;
    s32 next_angle;

    angle = 0;
    do {
        next_angle = angle + 1;
        x[1] = 320;
        x[3] = 240;
        y[2] = angle * 8;
        y[3] = next_angle * 8;
        y[1] = next_angle * 8;
        y[0] = angle * 8;
        x[2] = 240;
        x[0] = 320;
        angle = next_angle;
        for (i = 0; i < 4; i++) {
            input_x = x[i];
            input_y = y[i];
            Rot_2D(((TaiikuCamera0C *)D_80122DD0)->angle, &input_x, &input_y, &x[i], &y[i]);
        }
        camera_x = ((TaiikuCamera0C *)D_80122DD0)->x;
        camera_y = ((TaiikuCamera0C *)D_80122DD0)->y;
        for (i = 0; i < 4; i++) {
            x[i] += camera_x;
            y[i] += camera_y;
        }
        for (i = 0; i < 4; i++) {
            value = func_800A0140(0x2AA);
            y[i] = (y[i] * value) >> 12;
        }
        middle0 = (x[2] + x[0]) >> 1;
        middle1 = (x[3] + x[1]) >> 1;
        i = 0;
        row = 0;
        do {
            xp = x + row * 2;
            yp = y + row * 2;
            i++;
            first_y = yp[0];
            if (first_y > 0 || yp[1] > 0) {
                first_x = xp[0];
                if (middle0 < first_x) {
                    first_x = first_x + first_y + middle0;
                    xp[0] = first_x;
                }
                if (first_x < middle0) {
                    xp[0] = first_x - first_y + middle0;
                }
                value = xp[1];
                if (middle1 < value) {
                    value = value + yp[1] + middle1;
                    xp[1] = value;
                }
                if (value < middle1) {
                    xp[1] = value - yp[1] + middle1;
                }
            }
            row = i;
        } while (i < 2);
        for (i = 0; i < 4; i++) {
            x[i] = -x[i];
        }
        for (i = 0; i < 4; i++) {
            y[i] -= 24;
        }
        for (i = 0; i < 4; i++) {
            x[i] -= (s16)D_80149968.unk_1C[0] >> 2;
        }
        i = 0;
        row = 0;
        do {
            xp = x + row * 2;
            yp = y + row * 2;
            first_x = xp[0];
            if ((first_x >= -160 && first_x <= 160 && yp[0] >= -23 && yp[0] < 121) || (xp[1] >= -160 && xp[1] <= 160 && yp[1] >= -23 && yp[1] < 121)) {
                first_y = yp[0];
                func_80049FF0((s16)first_x, (s16)first_y, (s16)xp[1], (s16)yp[1], 0xFFFFFF, 0xFFFFFF, 250);
            }
            i++;
            row = i;
        } while (i != 2);
    } while (angle < 64);
    ((DisplayActor44 *)D_8011ECD0)[102].unk_26 = x[3] + 8;
    angle = 0;
    do {
        input_x = 0;
        input_y = -320;
        Rot_2D(((TaiikuCamera0C *)D_80122DD0)->angle + angle, &input_x, &input_y, &x[0], &y[0]);
        Rot_2D(((TaiikuCamera0C *)D_80122DD0)->angle + angle + 16, &input_x, &input_y, &x[1], &y[1]);
        input_x = 0;
        input_y = -240;
        Rot_2D(((TaiikuCamera0C *)D_80122DD0)->angle + angle, &input_x, &input_y, &x[2], &y[2]);
        Rot_2D(((TaiikuCamera0C *)D_80122DD0)->angle + angle + 16, &input_x, &input_y, &x[3], &y[3]);
        camera_x = ((TaiikuCamera0C *)D_80122DD0)->x;
        camera_y = ((TaiikuCamera0C *)D_80122DD0)->y;
        for (i = 0; i < 4; i++) {
            x[i] += camera_x;
            y[i] += camera_y;
        }
        for (i = 0; i < 4; i++) {
            value = func_800A0140(0x2AA);
            y[i] = (y[i] * value) >> 12;
        }
        middle0 = (x[2] + x[0]) >> 1;
        middle1 = (x[3] + x[1]) >> 1;
        i = 0;
        row = 0;
        do {
            xp = x + row * 2;
            yp = y + row * 2;
            i++;
            first_y = yp[0];
            if (first_y > 0 || yp[1] > 0) {
                first_x = xp[0];
                if (middle0 < first_x) {
                    first_x = first_x + first_y + middle0;
                    xp[0] = first_x;
                }
                if (first_x < middle0) {
                    xp[0] = first_x - first_y + middle0;
                }
                value = xp[1];
                if (middle1 < value) {
                    value = value + yp[1] + middle1;
                    xp[1] = value;
                }
                if (value < middle1) {
                    xp[1] = value - yp[1] + middle1;
                }
            }
            row = i;
        } while (i < 2);
        for (i = 0; i < 4; i++) {
            x[i] = -x[i];
        }
        for (i = 0; i < 4; i++) {
            y[i] -= 24;
        }
        for (i = 0; i < 4; i++) {
            x[i] -= (s16)D_80149968.unk_1C[0] >> 2;
        }
        i = 0;
        row = 0;
        do {
            xp = x + row * 2;
            yp = y + row * 2;
            first_x = xp[0];
            if ((first_x >= -160 && first_x <= 160 && yp[0] >= -23 && yp[0] < 121) || (xp[1] >= -160 && xp[1] <= 160 && yp[1] >= -23 && yp[1] < 121)) {
                first_y = yp[0];
                func_80049FF0((s16)first_x, (s16)first_y, (s16)xp[1], (s16)yp[1], 0xFFFFFF, 0xFFFFFF, 250);
            }
            i++;
            row = i;
        } while (i != 2);
        angle += 16;
    } while (angle != 0x400);
}
