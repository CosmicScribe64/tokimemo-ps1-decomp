/* T-26000 block-copy state case (README.md). The 0x44-byte copy is a 12-byte loop; its end
 * register is the source register plus 2 (state F) or plus 3 (state B) in ugen's rotation
 * t6..t9, t0..t5. Build flags (tools/cc.py IDO_CFLAGS, cvt pass around uopt).
 * Case: `odd = G1 & 1; if (odd != 0 && G2 != 0)` (uopt writes `ldc 0; lod R; neq`, the
 * constant on the left).
 * Ucode before the copy: and str(R) f[0 neq R] f[neq 0] C S and str(R) f[neq 0] S.
 * Measured: IDO 4.1 ugen B, IDO 5.3 ugen B; tools/ugen_pass.py B (a compare with a non-constant
 * right operand sets B). */
typedef struct { int w[17]; } R;
typedef struct { unsigned a : 29; unsigned active : 1; unsigned c : 2; } BF;
extern unsigned char A[];
extern unsigned char *P;
extern BF *Q;
extern void f(void);
extern int G1, G2;
#define COPY (*(R *)(A + 0x1a08) = *(R *)(A + 0x1980))
void test(void) {
    int odd;

    odd = G1 & 1;
    if (odd != 0 && G2 != 0) {
        f();
        odd = G1 & 1;
        G2 = 0;
    }
    if (odd) G2 = 5;
    COPY;
}
