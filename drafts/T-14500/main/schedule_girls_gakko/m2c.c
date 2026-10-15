extern ? D_800E6300;
extern u8 D_800E6375;

void schedule_girls_gakko(void) {
    u8 sp5F;
    u8 sp5E;
    GameState *var_s0;
    s32 var_s1;
    u8 temp_v0;

    var_s1 = 0;
    var_s0 = &D_800E6280;
    do {
        if (var_s0->unk_69C[0].unk_00 == 0x44) {
            temp_v0 = var_s0->unk_69C[1].unk_00;
            if ((temp_v0 == 0x33) || (temp_v0 == 0x32) || (temp_v0 == 0x47)) {
                goto block_15;
            }
            if ((D_800E6375 & 0xF) == 3) {
                goto block_15;
            }
            if (special_event_select((s32) &sp5F, (s32) &sp5E) != 0) {
                var_s0->unk_69C[0].unk_00 = 0x45;
                var_s0->unk_69C[0].unk_01 = sp5F;
                var_s0->unk_69C[0].unk_02 = sp5E;
            } else if ((date_sasoi_judge((s32) &sp5F) == 1) && (var_s1 == 0)) {
                var_s1 = (var_s1 + 1) & 0xFF;
                var_s0->unk_69C[0].unk_00 = 0x46;
                var_s0->unk_69C[0].unk_01 = sp5F;
            } else if (geko_judge((s32) &sp5F) != 0) {
                var_s0->unk_69C[0].unk_01 = sp5F;
            } else {
block_15:
                var_s0->unk_69C[0].unk_00 = 0xFA;
            }
        }
        var_s0 += 4;
    } while (var_s0 != &D_800E6300);
    func_8004284C();
}
