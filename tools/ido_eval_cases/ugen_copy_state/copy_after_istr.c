/* T-20710 block-copy state reproducer (wiki/data/true-gaps.md). The 0x44-byte copy is a
 * 12-byte loop; its end register is the source register plus 2 (state F) or plus 3 (state B)
 * in ugen's rotation t6..t9, t0..t5. Build flags (tools/cc.py IDO_CFLAGS).
 * Measured: IDO 5.3 F (src t9, end t1), IDO 5.2 F, IDO 4.1 F: an indirect store gives F in all three. */
typedef struct { int w[17]; } R;
extern unsigned char A[];
void test(unsigned char *p, int n) {
    p[4] = 8;
    p[5] = 11;
    *(R *)(A + 0x1a08) = *(R *)(A + 0x1980);
}
