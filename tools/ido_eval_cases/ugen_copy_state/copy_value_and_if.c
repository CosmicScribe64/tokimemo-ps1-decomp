/* T-26000 block-copy state case (README.md). The 0x44-byte copy is a 12-byte loop; its end
 * register is the source register plus 2 (state F) or plus 3 (state B) in ugen's rotation
 * t6..t9, t0..t5. Build flags (tools/cc.py IDO_CFLAGS, cvt pass around uopt).
 * Case: `if (G1 & 1) f();` (`andi; beqz`), a branch on a value.
 * Ucode before the copy: lod ldc 1 and fjp C.
 * Measured: IDO 4.1 ugen F, IDO 5.3 ugen B; tools/ugen_pass.py B (a branch on a value keeps
 * the state). */
typedef struct { int w[17]; } R;
typedef struct { unsigned a : 29; unsigned active : 1; unsigned c : 2; } BF;
extern unsigned char A[];
extern unsigned char *P;
extern BF *Q;
extern void f(void);
extern int G1, G2;
#define COPY (*(R *)(A + 0x1a08) = *(R *)(A + 0x1980))
void test(void) {
    if (G1 & 1) f();
    COPY;
}
