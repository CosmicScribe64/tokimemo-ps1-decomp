/* T-25608 block-copy state case (README.md). The 0x44-byte copy is a 12-byte loop; its end
 * register is the source register plus 2 (state F) or plus 3 (state B) in ugen's rotation
 * t6..t9, t0..t5. Build flags (tools/cc.py IDO_CFLAGS, cvt pass around uopt).
 * Case: store, then a `G >= 8` loop (`slti; beqz`).
 * Ucode before the copy: I C t[geq 8] M.
 * Measured: IDO 4.1 ugen F, IDO 5.3 ugen F; tools/ugen_pass.py rules: build B, rule A B,
 * rule A with ujp B, rule B B. */
typedef struct { int w[17]; } R;
extern unsigned char A[];
extern void f(void);
extern void f2(int);
extern int G;
#define COPY (*(R *)(A + 0x1a08) = *(R *)(A + 0x1980))
void test(unsigned char *p, int n) {
    p[4] = 8;
    do {
        f();
    } while (G >= 8);
    COPY;
}
