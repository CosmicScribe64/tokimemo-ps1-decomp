#include "psx_types.h"
MATRIX *func_8015A640(s32 angle, MATRIX *matrix) {
    s32 sine;
    s32 cosine;
    s32 trig;
    s16 a;
    s16 b;
    s16 c;
    s16 d;
    s16 e;
    s16 f;

    if (angle < 0) {
        trig = D_800DE500[(-angle) & 0xFFF];
        sine = -(s16)trig;
    } else {
        trig = D_800DE500[angle & 0xFFF];
        sine = (s16)trig;
    }
    cosine = trig >> 16;
    a = matrix->m[1][0];
    d = matrix->m[2][0];
    b = matrix->m[1][1];
    e = matrix->m[2][1];
    c = matrix->m[1][2];
    f = matrix->m[2][2];
    matrix->m[1][0] = (cosine * a - sine * d) >> 12;
    matrix->m[1][1] = (cosine * b - sine * e) >> 12;
    matrix->m[1][2] = (cosine * c - sine * f) >> 12;
    matrix->m[2][0] = (sine * a + cosine * d) >> 12;
    matrix->m[2][1] = (sine * b + cosine * e) >> 12;
    matrix->m[2][2] = (sine * c + cosine * f) >> 12;
    return matrix;
}
