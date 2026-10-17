/* T-20710 block-copy state reproducer (wiki/data/true-gaps.md). The 0x44-byte copy is a
 * 12-byte loop; its end register is the source register plus 2 (state F) or plus 3 (state B)
 * in ugen's rotation t6..t9, t0..t5. Build flags (tools/cc.py IDO_CFLAGS).
 * Measured: IDO 5.3 B (src t7, end t0), IDO 5.2 B, IDO 4.1 B: a call leaves the entry state. */
typedef struct { int w[17]; } R;
extern unsigned char A[];
extern void f(int, int);
extern int G1, G2;
void test(unsigned char *p, int n) {
    f(G1, G2);
    *(R *)(A + 0x1a08) = *(R *)(A + 0x1980);
}
