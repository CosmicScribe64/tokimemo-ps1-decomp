s32 func_800460FC(void) {
    u8 status[8];
    u8 local[4];
    s32 saved;
    u8 *index = &D_80125128;
    s32 retries;
    WordBytes4 *state;

    saved = D_801255D0.word;
    state = &D_801255D0;
    retries = 0;
    /* FAKE: reuse the zero retry value for the byte-clear chain while
     * investigating the original v0 stores. T-12010 */
    state->bytes[0] = state->bytes[1] = state->bytes[2] = retries;
    state->bytes[3] = 0;
    if (func_80087954(1, status) != 0) {
        while (func_80087C64(0x10, 0, state) == 0) {
            retries++;
            if (retries >= 16) {
                if (func_80046094() == 0x1B) {
                    D_8012512B = -1;
                    if ((*index) != 0) {
                        (*index) -= 1;
                    } else {
                        (*index) = 0x2F;
                    }
                }
                state->word = saved;
                break;
            }
        }
    }
    local[0] = state->bytes[0];
    local[1] = state->bytes[1];
    local[2] = state->bytes[2];
    retries = func_80087FF0(local);
    if (func_80046684(state->bytes[0]) >= 0x4B) return 0;
    if (func_80046684(state->bytes[1]) >= 0x3C) return 0;
    if (func_80046684(state->bytes[2]) >= 0x4B) return 0;
    if (retries > 0) return retries;
    return 0;
}
