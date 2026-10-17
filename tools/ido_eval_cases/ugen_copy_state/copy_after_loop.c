/* T-20710 block-copy state reproducer (wiki/data/true-gaps.md). The 0x44-byte copy is a
 * 12-byte loop; its end register is the source register plus 2 (state F) or plus 3 (state B)
 * in ugen's rotation t6..t9, t0..t5. Build flags (tools/cc.py IDO_CFLAGS).
 * Measured: IDO 5.3 F (src t7, end t9), IDO 5.2 F, IDO 4.1 B (end t0). */
typedef struct { int w[17]; } R;
extern unsigned char A[];
void test(unsigned char *p, int n) {
    int i;
    for (i = 0; i < n; i++) {
        p[i] = 0;
    }
    *(R *)(A + 0x1a08) = *(R *)(A + 0x1980);
}
