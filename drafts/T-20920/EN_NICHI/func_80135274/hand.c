s32 func_80135274(s32 arg0) {
    s32 dx;
    s32 dy;
    s32 dir;

    dx = D_8011ECF6 - D_8013984C[arg0].unk_14.h[1];
    dy = D_8011ECFA - D_8013984C[arg0].unk_18.h[1];
    switch (func_80134008(arg0)) {
    case 0:
        if (dy < dx) {
            dir = 0;
        } else {
            dir = 1;
        }
        if (dy <= -dx) {
            dir += 2;
        }
        break;
    case 1:
        if (D_8013984C[arg0].unk_14.h[1] < D_8011ECF6) {
            dir = 0;
        } else {
            dir = 1;
        }
        if (D_8011ECFA <= D_8013984C[arg0].unk_18.h[1]) {
            dir += 2;
        }
        break;
    case 2:
        if (-dx < dy) {
            dir = 0;
        } else {
            dir = 1;
        }
        if (dy <= dx) {
            dir += 2;
        }
        break;
    case 3:
        if (D_8013984C[arg0].unk_18.h[1] < D_8011ECFA) {
            dir = 0;
        } else {
            dir = 1;
        }
        if (D_8013984C[arg0].unk_14.h[1] <= D_8011ECF6) {
            dir += 2;
        }
        break;
    case 4:
        if (dx < dy) {
            dir = 0;
        } else {
            dir = 1;
        }
        if (-dx <= dy) {
            dir += 2;
        }
        break;
    case 5:
        if (D_8011ECF6 < D_8013984C[arg0].unk_14.h[1]) {
            dir = 0;
        } else {
            dir = 1;
        }
        if (D_8013984C[arg0].unk_18.h[1] <= D_8011ECFA) {
            dir += 2;
        }
        break;
    case 6:
        if (dy < -dx) {
            dir = 0;
        } else {
            dir = 1;
        }
        if (dx <= dy) {
            dir += 2;
        }
        break;
    case 7:
        if (D_8011ECFA < D_8013984C[arg0].unk_18.h[1]) {
            dir = 0;
        } else {
            dir = 1;
        }
        if (D_8011ECF6 <= D_8013984C[arg0].unk_14.h[1]) {
            dir += 2;
        }
        break;
    }
    return dir;
}
