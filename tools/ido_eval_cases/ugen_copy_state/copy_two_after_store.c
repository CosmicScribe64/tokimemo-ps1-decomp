/* T-25608 block-copy state case (README.md). The 0x44-byte copy is a 12-byte loop; its end
 * register is the source register plus 2 (state F) or plus 3 (state B) in ugen's rotation
 * t6..t9, t0..t5. Build flags (tools/cc.py IDO_CFLAGS, cvt pass around uopt).
 * Case: store, then two copies (a copy keeps the state).
 * Ucode before the copy: I M M.
 * Measured: IDO 4.1 ugen FF, IDO 5.3 ugen FF; tools/ugen_pass.py rules: build FF, rule A FF,
 * rule A with ujp FF, rule B FF. */
typedef struct { int w[17]; } R;
extern unsigned char A[];
extern void f(void);
extern void f2(int);
extern int G;
#define COPY (*(R *)(A + 0x1a08) = *(R *)(A + 0x1980))
void test(unsigned char *p, int n) {
    p[4] = 8;
    COPY;
    *(R *)(A + 0x1b08) = *(R *)(A + 0x1c80);
}
