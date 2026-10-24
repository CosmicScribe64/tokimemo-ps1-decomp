/* T-25608 block-copy state case (README.md). The 0x44-byte copy is a 12-byte loop; its end
 * register is the source register plus 2 (state F) or plus 3 (state B) in ugen's rotation
 * t6..t9, t0..t5. Build flags (tools/cc.py IDO_CFLAGS, cvt pass around uopt).
 * Case: store inside a loop on `n`, then a second loop on `n` (the shape of main func_8006B0C8).
 * Ucode before the copy: ... I t ... f[geq 1]< C t M.
 * Measured: IDO 4.1 ugen B, IDO 5.3 ugen F; tools/ugen_pass.py rules: build F, rule A B,
 * rule A with ujp B, rule B B. */
typedef struct { int w[17]; } R;
extern unsigned char A[];
extern void f(void);
extern void f2(int);
extern int G;
#define COPY (*(R *)(A + 0x1a08) = *(R *)(A + 0x1980))
void test(unsigned char *p, int n) {
    int i;
    for (i = 0; i < n; i++) p[i] = 0;
    for (i = 0; i < n; i++) f2(i);
    COPY;
}
