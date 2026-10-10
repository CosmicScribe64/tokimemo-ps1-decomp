---
type: data
updated: 2026-10-09
sources: ["wiki/matching-notes.md", "tools/queue.py"]
---

# T-0018 cases (register promotion of globals and related register-order gaps)

Data file of [[tickets/T-1321-register-promotion-build-step]]. One row per function skipped (left `INCLUDE_ASM` or `NON_MATCHING`) because of the T-0018 gap ([[matching-notes]], section "Register promotion of globals"). `tools/queue.py --calibrate` reads this table to measure the blocker detector, so keep the format.

Rules for batch agents (decompile-workflow step 7): append one row per skipped case, never edit or delete a row.
- `file`: main address stem (`8005A0B0`) or overlay name (`TEL`), as in the `file` column of `tools/queue.py`.
- `function`: name in backticks.
- `category`: `promo` if the original keeps one global in one register across several blocks (loads it again after calls or branches, same register) and IDO uses other registers; `reverse` if the original loads the global again into fresh temporaries where IDO keeps it in a register (the opposite gap); `regorder` for any other T-0018 register-choice difference (temporary order, `$v0`/`$v1` of a value that is not a global reload).
- `symptom`: one short line: what the original has and what IDO emits.

| file | function | category | symptom |
|---|---|---|---|
| TEL | `func_8013A40C` | promo | compare chain on a global, original loads it into $v1, IDO $v0 (batch B) |
| OLH | `func_80136210` | promo | compare chain on a global, original loads it into $v1, IDO $v0 (batch B) |
| EN_NICHI | `func_8013216C` | promo | compare chain on a global, original loads it into $v1, IDO $v0 (batch B) |
| BUNKA_SD | `func_80134540` | promo | compare chain on a global, original loads it into $v1, IDO $v0, value returned (batch G) |
| BUNKA_SD | `func_80135160` | promo | compare chain on a global, original loads it into $v1, IDO $v0, value returned (batch G) |
| BUNKA_SD | `func_801359B0` | promo | compare chain on a global, original loads it into $v1, IDO $v0, value returned (batch G) |
| BUNKA_SD | `func_80136270` | promo | compare chain on a global, original loads it into $v1, IDO $v0, value returned (batch G) |
| BUNKA_SD | `func_80138C34` | promo | post-increment compare of a global, old value in $v1, compare result in $v0 (batch G) |
| DATE2 | `func_801329F0` | promo | post-increment compare of a global, old value in $v1, compare result in $v0 (batch G) |
| DATE2 | `func_80132A48` | promo | post-increment compare of a global, old value in $v1, compare result in $v0 (batch G) |
| DATE2 | `func_80132B08` | promo | post-increment compare of a global, old value in $v1, compare result in $v0 (batch G) |
| NAME_ENT | `func_80145A74` | promo | compare chain on a global, original loads it into $v1, IDO $v0 (batch K) |
| NAME_ENT | `func_80132134` | promo | compare chain on a global, original loads it into $v1, IDO $v0 (batch K) |
| NAME_ENT | `func_80144AD0` | promo | compare chain on a global, original loads it into $v1, IDO $v0 (batch K) |
| NAME_ENT | `func_8013C4A8` | promo | compare chain on a global, original loads it into $v1, IDO $v0 (batch K) |
| NAME_ENT | `func_80140CCC` | promo | compare chain on a global, original loads it into $v1, IDO $v0 (batch K) |
| 8005A0B0 | `pre_syogatu_init` | promo | compare chain on a global, original loads it into $v1, IDO $v0 (batch D) |
| 8005A0B0 | `uwasa_main` | promo | compare chain on a global, original loads it into $v1, IDO $v0 (batch D) |
| 8005A0B0 | `func_80061688` | promo | compare chain on a global, original loads it into $v1, IDO $v0 (batch D) |
| 8005A0B0 | `func_8005B798` | promo | compare chain on a global, original loads it into $v1, IDO $v0 (batch D) |
| 8005A0B0 | `func_8005A560` | promo | compare chain on a global, original loads it into $v1, IDO $v0 (batch D) |
| 8005A0B0 | `func_8005AE60` | promo | compare chain on a global, original loads it into $v1, IDO $v0 (batch D) |
| 80061710 | `func_800626B0` | promo | compare chain on a global, original loads it into $v1, IDO $v0 (batch D) |
| 8005A0B0 | `func_80060B78` | promo | compare chain on a global, original loads it into $v1, IDO $v0 (batch D) |
| 80061710 | `func_80062634` | promo | compare chain on a global, original loads it into $v1, IDO $v0 (batch D) |
| 80061710 | `func_80061EFC` | promo | compare chain on a global, original loads it into $v1, IDO $v0 (batch D) |
| 8006CB30 | `func_80072BC0` | promo | compare chain on a global, original loads it into $v1, IDO $v0, copy to $v0 in the first delay slot (batch E) |
| 8006CB30 | `func_800728A4` | promo | compare chain on a global, original loads it into $v1, IDO $v0, copy to $v0 in the first delay slot (batch E) |
| 8006CB30 | `func_800725B0` | promo | compare chain on a global, original loads it into $v1, IDO $v0, copy to $v0 in the first delay slot (batch E) |
| 8006CB30 | `func_800732F8` | promo | compare chain on a global, original loads it into $v1, IDO $v0, copy to $v0 in the first delay slot (batch E) |
| 8006CB30 | `func_800733D8` | promo | compare chain on a global, original loads it into $v1, IDO $v0, copy to $v0 in the first delay slot (batch E) |
| 8006CB30 | `func_80070F80` | promo | compare chain on a global, original loads it into $v1, IDO $v0, copy to $v0 in the first delay slot (batch E) |
| 8006CB30 | `func_80072944` | promo | compare chain on a global, original loads it into $v1, IDO $v0, copy to $v0 in the first delay slot (batch E) |
| 8006CB30 | `func_8006CDD4` | promo | compare chain on a global, original loads it into $v1, IDO $v0, copy to $v0 in the first delay slot (batch E) |
| 8006CB30 | `week_day_init` | promo | compare chain on a global, original loads it into $v1, IDO $v0, copy to $v0 in the first delay slot (batch E) |
| 8006CB30 | `vacation_day_init` | promo | compare chain on a global, original loads it into $v1, IDO $v0, copy to $v0 in the first delay slot (batch E) |
| SHUGAKU | `func_80134024` | promo | D++ compared in the same expression, old value in $v1 (batch I) |
| SHUGAKU | `func_80134D8C` | promo | D++ compared in the same expression, old value in $v1 (batch I) |
| KANGEI | `func_80133A14` | regorder | D++ compared in the same expression, old value in $v1 (batch I) |
| EN_NICHI | `func_80132DC4` | reverse | read-modify-write of a global: original reloads into $t6 each time, IDO promotes to $v0 (batch B) |
| 80079B10 | `normal_date_move_place` | promo | 0/1/default dispatch on a global: selector in $v1, IDO $v0 (batch F) |
| 80085E30 | `func_80086640` | promo | two live temporaries or post-inc/dec counter: loaded value in $v1, compare in $v0 (batch F) |
| 80079B10 | `don_wait` | promo | two live temporaries or post-inc/dec counter: loaded value in $v1, compare in $v0 (batch F) |
| 80079B10 | `k_disp_inc2` | promo | two live temporaries or post-inc/dec counter: loaded value in $v1, compare in $v0 (batch F) |
| 80079B10 | `normal_date_bg_fadeout` | regorder | $v1 where the original has $v0 after -nokpicopt (T-1200 follow-up) |
| BUNKA_SD | `func_80135440` | regorder | $v1 where the original has $v0 after -nokpicopt (T-1200 follow-up) |
| 80079B10 | `func_8007B5EC` | reverse | original reloads the counter after the early return, IDO keeps it in a register (batch F) |
| 80043510 | `func_800450F4` | regorder | result kept in $v1, original keeps $v0 (batch C) |
| 80043510 | `func_8004500C` | regorder | result kept in $v1, original keeps $v0 (batch C) |
| 80043510 | `func_80044C98` | regorder | result kept in $v1, original keeps $v0 (batch C) |
| 80043510 | `func_80044E8C` | regorder | result kept in $v1, original keeps $v0 (batch C) |
| 80041000 | `func_800422C8` | regorder | result kept in $v1, original keeps $v0 (batch C) |
| 80047550 | `GetWorkBase` | regorder | register choice of a long-lived variable (batch C) |
| 80059710 | `MouseState` | regorder | register choice of a long-lived variable (batch C) |
| 8004AD60 | `func_8004AD60` | regorder | register choice of a long-lived variable (batch C) |
| 80079B10 | `func_8007B358` | regorder | register order of two temporaries, $t6/$t7 swapped (batch F) |
| 80079B10 | `func_8007B3DC` | regorder | register order of two temporaries, $t6/$t7 swapped (batch F) |
| 80075320 | `func_80075FA0` | regorder | register order of two temporaries, $t6/$t7 swapped (batch F) |
| 80062CD0 | `func_8006C700` | regorder | register choice in leaf code (batch E) |
| 8006CB30 | `get_weekly_bg_sector` | regorder | register choice in leaf code (batch E) |
| 80062CD0 | `func_80066ACC` | regorder | register choice in leaf code (batch E) |
| 8006CB30 | `func_80074F24` | regorder | register choice in leaf code (batch E) |
| 8006CB30 | `func_80074D28` | regorder | register choice in leaf code (batch E) |
| DATE2 | `func_80132E5C` | regorder | t = D * 0x1400 kept in $v0 by the original, IDO uses $t6 (batch G) |
| DATE2 | `func_80132FB0` | regorder | t = D * 0x1400 kept in $v0 by the original, IDO uses $t6 (batch G) |
| DATE2 | `func_8013301C` | regorder | t = D * 0x1400 kept in $v0 by the original, IDO uses $t6 (batch G) |
| DATE2 | `func_80132EC8` | regorder | t = D * 0x1400 kept in $v0 by the original, IDO uses $t6 (batch G) |
| KANGEI | `func_80138320` | regorder | ugen temp skips $t6 after a call (batch I) |
| KANGEI | `func_801386C4` | regorder | ugen temp skips $t6 after a call (batch I) |
| TT | `func_8014742C` | regorder | D = x; return: $t7/$t8 swap (batch K) |
| TT | `func_8013ABB0` | regorder | global pointer used many times: original keeps it in $t6, IDO reloads (batch K) |
| TT | `func_8013AB50` | regorder | global pointer used many times: original keeps it in $t6, IDO reloads (batch K) |
| TT | `func_80132130` | regorder | global pointer used many times: original keeps it in $t6, IDO reloads (batch K) |
| EN_NICHI | `func_801383A0` | regorder | $v1/$a2 swap in the 0x44 multiply family (batch B) |
| TACO | `func_801368D8` | regorder | loaded byte kept in $v0 and bound moved with `move a1,t6`; IDO uses $v1 and a second andi (batch T-2030) |
| TACO | `func_80143F40` | regorder | s16 clamp chain: original keeps the value in $v0, IDO uses $v1 plus an extra shift pair (T-2030) |
| TACO | `func_80137CBC` | regorder | index param copied with `or t6,a0,zero` before the table lookup; IDO uses $a0 directly (T-2030) |
| TACO | `func_80136C60` | regorder | post-increment of a u8 global used twice: original reuses $v0 (`addiu v0,v0,1`), IDO uses $v1/$a0 (T-2030) |
| TACO | `func_8014EDCC` | reverse | constant 1 compared twice: original loads `li at,1` each time, IDO keeps it in $v0 (T-2030) |
| TACO | `func_8014488C` | regorder | constant 8 stored to two u8 globals: original loads `li v0,8` once and reuses it across a third store, IDO re-loads it into fresh temps (T-2030) |
