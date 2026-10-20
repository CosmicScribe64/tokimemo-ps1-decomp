/* Source-only NON_MATCHING draft, T-24522: 53 literal / 53 aligned rows.
 * 376 words against target375; loop start/quotient range and pointer operands differ. */
void func_8015528C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    s32 *work;
    s32 *progress;
    s32 *phase;
    s32 end_candidate;
    s32 phase_sum;
    s32 progress_raw;
    s32 start;
    s32 end;
    s32 i;
    s32 color_phase;

    /* FAKE: T-24522, unresolved work-array view across scalar declarations. */
    work = &D_8015EBAC;
    progress = &work[arg6];
    progress_raw = *progress;
    if (arg3 < (progress_raw / 10)) {
        phase = &work[arg7];
        *progress = progress_raw - (arg3 * 0xA);
        *phase -= 1;
        if (*phase < 0) {
            *phase = 2;
        }
    }
    phase = &work[arg7];
    i = 0;
    do {
        start = (s32) *progress / 10;
        phase_sum = *phase + i;
        end_candidate = start + (i * arg3);
        if (end_candidate < arg0) {
            start += (i - 1) * arg3;
            end = end_candidate;
            /* FAKE: T-24522, equivalent remainder expression keeps arm ownership. */
            color_phase = (phase_sum % 3) * 1;
            if (start < 0) {
                goto block_9;
            }
        } else {
            start += (i - 1) * arg3;
            end = arg0;
            i = 0x64;
            color_phase = phase_sum % 3;
            if (start < 0) {
block_9:
                start = 0;
            }
        }
        switch (color_phase) {
        case 0:
            func_80155868(start, (s32) (arg1 * start) / arg0, (s32) (arg2 * start) / arg0, end, (s32) (arg1 * end) / arg0, (s32) (arg2 * end) / arg0, arg8 << 0x10, arg8 << 8, 4);
            break;
        case 1:
            func_80155868(start, (s32) (arg1 * start) / arg0, (s32) (arg2 * start) / arg0, end, (s32) (arg1 * end) / arg0, (s32) (arg2 * end) / arg0, arg8 << 8, arg8, 4);
            break;
        case 2:
            func_80155868(start, (s32) (arg1 * start) / arg0, (s32) (arg2 * start) / arg0, end, (s32) (arg1 * end) / arg0, (s32) (arg2 * end) / arg0, arg8, arg8 << 0x10, 4);
            break;
        }
        i += 1;
    } while (i < 0x32);
    dtd_on_tpage(0, 0, 4, arg5 - 4, 0);
    *progress += arg4;
}
