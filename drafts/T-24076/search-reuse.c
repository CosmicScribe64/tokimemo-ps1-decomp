/* NON_MATCHING: T-24076 neutral unsigned index/code reuse, 90 resolved rows. */
void func_8013F054(void) {
    /* FAKE: the whole-GameState byte view at +0x756 covers index0 before the
     * known unk_75E subarray. The integer address cursor preserves the unsigned
     * boundary without forming a before-object pointer; original C type and
     * spelling are unproven. T-24076. */
    s32 index;
    u32 cursor;
    u32 search;
    s32 var_a3;
    u8 var_t0;
    s32 var_t1;
    GsRec75E *record;

    index = D_80150064;
    search = (u32) index;
    var_t0 = 0;
    var_a3 = 0;
    var_t1 = 0;
    if (D_800E6280.unk_1124 == 0) {
        if ((D_801500D8 == 1) || (D_801500D8 == 3)) {
            record = (GsRec75E *) ((u8 *) &D_800E6280 + 0x756 + index * 8);
            var_t1 = record->unk_00;
            var_a3 = record->unk_02;
            var_t0 = record->unk_01;
        } else if (((D_801500D8 == 0) || (D_801500D8 == 2)) && (index >= 0)) {
            cursor = (u32) &D_800E6280 + search * 8U;
            do {
                search = ((GsRec75E *) (cursor + 0x756))->unk_00;
                cursor -= 8;
                if (((search == 0xC) && (D_800E6280.unk_F5F == 0xC)) ||
                    ((search == 0xB) && (D_800E6280.unk_F5F == 0xB))) {
                    record = (GsRec75E *) (cursor + 0x75E);
                    var_a3 = record->unk_02;
                    var_t0 = record->unk_01;
                    var_t1 = search;
                    break;
                }
            } while (cursor >= (u32) &D_800E6280);
        }
    } else {
        record = (GsRec75E *) &D_800E7D10.unk_058[D_80150080 * 8];
        var_t1 = record->unk_00;
        var_a3 = record->unk_02;
        var_t0 = record->unk_01;
    }
    /* FAKE: scalar field-address views retain independent final selector reloads.
     * T-24076; the ordinary direct-field form CSEs the second read. */
    if ((*(u8 *) &D_800E6280.unk_F5F == 0xC) && (var_t1 == 0xC)) {
        if (var_a3 == 4) {
            func_8013F320(1);
        } else if (var_a3 == 6) {
            func_8013F320(2);
        } else if (var_a3 == 7) {
            func_8013F320(3);
        } else if ((var_a3 == 2) && (var_t0 == 0x4F)) {
            func_8013F320(5);
        } else if ((var_a3 == 2) && (var_t0 == 0x50)) {
            func_8013F320(4);
        } else if (var_a3 == 2) {
            if (var_t0 == 0x51) {
                func_8013F320(6);
            }
        }
    } else if ((*(u8 *) &D_800E6280.unk_F5F == 0xB) && (var_t1 == 0xB)) {
        if ((var_a3 == 2) && (var_t0 == 0x4A)) {
            func_8013F320(0);
        } else if (((var_a3 == 2) && (var_t0 == 0x4C)) || ((var_a3 == 0xB) && (var_t0 != 0x1E))) {
            func_8013F320(1);
        } else if (((var_a3 == 2) && (var_t0 == 0x4E)) ||
                   ((var_a3 == 0xB) && (var_t0 == 0x1E))) {
            func_8013F320(2);
        } else if (var_a3 == 0x12) {
            func_8013F320(3);
        }
    }
}
