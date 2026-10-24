/* T-26000 block-copy state case (README.md). The 0x44-byte copy is a 12-byte loop; its end
 * register is the source register plus 2 (state F) or plus 3 (state B) in ugen's rotation
 * t6..t9, t0..t5. Build flags (tools/cc.py IDO_CFLAGS, cvt pass around uopt).
 * Case: the never-true arm of the FAKEs in main strCallback, func_8005938C and MASTER
 * func_801344E0, func_801363F4: `if ((unsigned)G < 0) { *(unsigned char *)0 = 0; }`. ugen
 * drops the arm (no sb in the object).
 * Ucode before the copy: f[les 0] istr.
 * Measured: IDO 4.1 ugen B, IDO 5.3 ugen B; tools/ugen_pass.py F (it counts the store although
 * ugen drops the arm; the FAKE arms rely on that). */
typedef struct { int w[17]; } R;
extern unsigned char A[];
extern unsigned char G3;
#define COPY (*(R *)(A + 0x1a08) = *(R *)(A + 0x1980))
void test(void) {
    if ((unsigned)G3 < 0) {
        *(unsigned char *)0 = 0;
    }
    COPY;
}
