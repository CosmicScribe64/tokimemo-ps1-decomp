/* T-26000 block-copy state case (README.md). The 0x44-byte copy is a 12-byte loop; its end
 * register is the source register plus 2 (state F) or plus 3 (state B) in ugen's rotation
 * t6..t9, t0..t5. Build flags (tools/cc.py IDO_CFLAGS, cvt pass around uopt).
 * Case: control for copy_dropped_arm_store.c, the same store in an arm ugen keeps:
 * `if (G1 < 0) { *(unsigned char *)0 = 0; }`.
 * Ucode before the copy: f[les 0] istr.
 * Measured: IDO 4.1 ugen F, IDO 5.3 ugen F; tools/ugen_pass.py F. */
typedef struct { int w[17]; } R;
extern unsigned char A[];
extern int G1;
#define COPY (*(R *)(A + 0x1a08) = *(R *)(A + 0x1980))
void test(void) {
    if (G1 < 0) {
        *(unsigned char *)0 = 0;
    }
    COPY;
}
