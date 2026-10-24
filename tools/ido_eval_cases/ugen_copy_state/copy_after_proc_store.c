/* T-25608 block-copy state case (README.md). The 0x44-byte copy is a 12-byte loop; its end
 * register is the source register plus 2 (state F) or plus 3 (state B) in ugen's rotation
 * t6..t9, t0..t5. Build flags (tools/cc.py IDO_CFLAGS, cvt pass around uopt).
 * Case: the previous procedure ends with a store; the copy is at entry.
 * Ucode before the copy: M.
 * Measured: IDO 4.1 ugen B, IDO 5.3 ugen B; tools/ugen_pass.py rules: build B, rule A B,
 * rule A with ujp B, rule B B. */
typedef struct { int w[17]; } R;
extern unsigned char A[];
extern void f(void);
extern void f2(int);
extern int G;
#define COPY (*(R *)(A + 0x1a08) = *(R *)(A + 0x1980))
void prev(unsigned char *p) {
    p[4] = 8;
}
void test(unsigned char *p, int n) {
    COPY;
}
