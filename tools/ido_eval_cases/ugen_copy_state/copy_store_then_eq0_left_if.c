/* T-26000 block-copy state case (README.md). The 0x44-byte copy is a 12-byte loop; its end
 * register is the source register plus 2 (state F) or plus 3 (state B) in ugen's rotation
 * t6..t9, t0..t5. Build flags (tools/cc.py IDO_CFLAGS, cvt pass around uopt).
 * Case: an indirect store, then `if ((G1 & 1) == 0) f();` (uopt writes `ldc 0; lod; ldc 1;
 * and; equ`, the constant on the left).
 * Ucode before the copy: istr f[0 equ and] C.
 * Measured: IDO 4.1 ugen F, IDO 5.3 ugen F; tools/ugen_pass.py F (a compare with a constant on
 * either side keeps the state). */
typedef struct { int w[17]; } R;
extern unsigned char A[];
extern unsigned char *P;
extern void f(void);
extern int G1;
#define COPY (*(R *)(A + 0x1a08) = *(R *)(A + 0x1980))
void test(void) {
    P[4] = 8;
    if ((G1 & 1) == 0) f();
    COPY;
}
