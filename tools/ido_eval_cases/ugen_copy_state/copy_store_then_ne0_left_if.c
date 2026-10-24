/* T-26000 block-copy state case (README.md). The 0x44-byte copy is a 12-byte loop; its end
 * register is the source register plus 2 (state F) or plus 3 (state B) in ugen's rotation
 * t6..t9, t0..t5. Build flags (tools/cc.py IDO_CFLAGS, cvt pass around uopt).
 * Case: copy_ne0_left_if.c with an indirect store first, so the procedure is in F before
 * `if (odd != 0 && G2 != 0)` (uopt writes `ldc 0; lod R; neq`, the constant on the left).
 * Ucode before the copy: istr and str(R) f[0 neq R] f[neq 0] C S and str(R) f[neq 0] S.
 * Measured: IDO 4.1 ugen F, IDO 5.3 ugen B; tools/ugen_pass.py F (a compare with a constant on
 * either side keeps the state). */
typedef struct { int w[17]; } R;
extern unsigned char A[];
extern unsigned char *P;
extern void f(void);
extern int G1, G2;
#define COPY (*(R *)(A + 0x1a08) = *(R *)(A + 0x1980))
void test(void) {
    int odd;

    P[4] = 8;
    odd = G1 & 1;
    if (odd != 0 && G2 != 0) {
        f();
        odd = G1 & 1;
        G2 = 0;
    }
    if (odd) G2 = 5;
    COPY;
}
