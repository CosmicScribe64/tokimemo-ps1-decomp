/* Synthetic aligned-copy diagnostic, not game code or a matching claim.
 * Both functions clear byte4 and copy the same aligned68-byte object.
 * The integer address roundtrip tests direct versus indirect store lowering.
 * Run tools/cc.py in Docker; compare relative source/end temporary selection.
 * T-24517 */
#include "common.h"

typedef union Copy68 {
    /* 0x00 */ u32 words[17];
    /* 0x00 */ u8 bytes[68];
} Copy68; /* size 0x44, alignment4 */

extern Copy68 synthetic_records[2];

void copy68_direct(void) {
    synthetic_records[0].bytes[4] = 0;
    synthetic_records[1] = synthetic_records[0];
}

void copy68_integer_view(void) {
    /* FAKE: address roundtrip isolates store lowering only. T-24517 */
    *(u8 *)(u32)&synthetic_records[0].bytes[4] = 0;
    synthetic_records[1] = synthetic_records[0];
}
