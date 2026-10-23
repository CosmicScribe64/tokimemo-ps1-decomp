typedef int s32; typedef unsigned int u32; typedef short s16; typedef unsigned short u16; typedef unsigned char u8;
typedef struct { u8 a, b, c, fl; s16 x; } R;
typedef struct { s16 a, b, c; } P;
typedef struct { s32 attr; s16 x, y, w, h; } S;
extern void g(S *); extern R RR[]; extern u8 RB[];
