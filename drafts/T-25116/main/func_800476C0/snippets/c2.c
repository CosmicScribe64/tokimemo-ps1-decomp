#include "c0.h"
void f(R *rec, P *part, s32 n) {
  S spr; s32 i;
  for (i = 0; i < n; i++) {
    if (((R *)RB)[i].fl & 1) { spr.x = part->a; } else { spr.x = part->b; }
    if (((R *)RB)[i].fl & 4) { spr.y = part->c; }
    g(&spr);
  }
}
