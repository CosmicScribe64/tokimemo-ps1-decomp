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
| ETC | `func_80142AE8` | promo | global loaded once into $v0 and compared in two loops, original frees $v0 for the second loop's end pointer; IDO needs a local copy (address in $v1) and takes $v0 for the loop temp (T-2040) |
| ETC | `func_801428EC` | regorder | u8 global loaded into $v0 and scaled by 0x10101 (shift/add chain) for a call argument, IDO takes $t7 for the load and $t8/$t9 for the chain (T-2040) |
| ETC | `func_80143F24` | promo | u8 global loaded once into $a1 and used in a store loop and after it (`D -= 1`); original also hoists the `lui` of two later globals above the loop (shared $v0/$v1), IDO keeps the value in $v1 and reloads the lui late (T-2040) |
| ETC | `func_80145AF8` | promo | u8 global loaded into $v0, used for a scale argument of the first call and spilled to the stack for the second call; IDO reloads it into $t9 and has a smaller frame (T-2040) |
| DATE | `func_80156570` | regorder | bit test of a global then `D = D + 1` on a u16: original skips one temporary (`sll t8` where IDO takes `t7`, later temps shifted by one) (T-2010) |
| DATE | `func_801565B4` | regorder | same bit test, inverted: original skips one temporary (T-2010) |
| DATE | `func_8015522C` | regorder | global compare, then `D = D + 1` on a u16: original skips one temporary (T-2010) |
| DATE | `func_80157184` | regorder | `get_g_zyotai_s(x) & 0x7F` compare, then u16 `D += 5`: original andi in $t7 (IDO $t6), later temps shifted (T-2010) |
| DATE | `func_801575CC` | regorder | same shape as `func_80157184` (T-2010) |
| DATE | `func_80157824` | regorder | same shape as `func_80157184` (T-2010) |
| DATE | `func_80149F48` | regorder | copy chain of globals: original loads the first value into $v0, IDO into $t6 (T-2010) |
| DATE | `func_80148924` | regorder | `s16` argument added to eight globals: original sign-extends into $v0 once (`sll t6; sra v0`), IDO drops the extension and uses $a0 (T-2010) |
| DATE | `func_8013DCC0` | regorder | nine stores of the constants 3, 4, 2: original loads each constant once ($t6, $t7, $t8) and reuses it; IDO with `-nokpicopt` re-loads it per store (T-2010) |
| SHOUGATU | `func_8013F190` | regorder | load from a global: original loads into a fresh temp (t0), IDO reuses the lui register (t9) (T-2060) |
| SHOUGATU | `func_801410B4` | regorder | `D |= 4` at entry: original lbu into t7 (t6 holds the lui), IDO reuses t6 (T-2060) |
| SHOUGATU | `func_8014131C` | regorder | `D |= 4` at entry: original lbu into t7 (t6 holds the lui), IDO reuses t6 (T-2060) |
| SHOUGATU | `func_8014147C` | regorder | `D |= 4` at entry: original lbu into t7 (t6 holds the lui), IDO reuses t6 (T-2060) |
| SHOUGATU | `func_801338DC` | regorder | `func_80051A68(..) & 0x7F` result: original skips one temp (andi t7 where IDO uses t6), or moves it through $v1 first (T-2060) |
| SHOUGATU | `func_8013E464` | regorder | `func_80051A68(..) & 0x7F` result: original skips one temp (andi t7 where IDO uses t6), or moves it through $v1 first (T-2060) |
| SHOUGATU | `func_8013E410` | regorder | `func_80051A68(..) & 0x7F` result: original skips one temp (andi t7 where IDO uses t6), or moves it through $v1 first (T-2060) |
| SHOUGATU | `func_801345C8` | regorder | `func_80051A68(..) & 0x7F` result: original skips one temp (andi t7 where IDO uses t6), or moves it through $v1 first (T-2060) |
| SHOUGATU | `func_801341E4` | regorder | `func_80051A68(..) & 0x7F` result: original skips one temp (andi t7 where IDO uses t6), or moves it through $v1 first (T-2060) |
| SHOUGATU | `func_801366A4` | regorder | `func_80051A68(..) & 0x7F` result: original skips one temp (andi t7 where IDO uses t6), or moves it through $v1 first (T-2060) |
| SHOUGATU | `func_80136340` | regorder | `func_80051A68(..) & 0x7F` result: original skips one temp (andi t7 where IDO uses t6), or moves it through $v1 first (T-2060) |
| SHOUGATU | `func_8013AE0C` | promo | two globals compared, original loads the u8 into $v1 first and keeps it across the branches, IDO $v0 and loads in the other order (T-2060) |
| SHOUGATU | `func_801401DC` | regorder | `s16 D += 0x200; if (D >= 0x1000)`: original keeps the sign-extended value in v0, IDO uses a second temp (t7) (T-2060) |
| SHOUGATU | `func_80138D2C` | promo | `D = N; f(N)`: original keeps the constant N in a0 for the store and the call argument, IDO loads it into a temp for the store and again into a0 (T-2060) |
| SHOUGATU | `func_80139CCC` | promo | `D = N; f(N)`: original keeps the constant N in a0 for the store and the call argument, IDO loads it into a temp for the store and again into a0 (T-2060) |
| SHOUGATU | `func_80139514` | promo | `D = N; f(N)`: original keeps the constant N in a0 for the store and the call argument, IDO loads it into a temp for the store and again into a0 (T-2060) |
| SHOUGATU | `func_801385AC` | promo | `D = N; f(N)`: original keeps the constant N in a0 for the store and the call argument, IDO loads it into a temp for the store and again into a0 (T-2060) |
| SHOUGATU | `func_8013A2F8` | promo | `D = N; f(N)`: original keeps the constant N in a0 for the store and the call argument, IDO loads it into a temp for the store and again into a0 (T-2060) |
| SHOUGATU | `func_8013F9C4` | regorder | `D != 0 && D != 7 && D != 9`: constants 7, 9 live in v1, a0 and the original writes `beq v0,v1`, IDO emits `beq v1,v0` (T-2060) |
| SHOUGATU | `func_8013AB5C` | promo | `D = N; f(N)` then `base[D * 0x38 + off] |= 4` (also `func_8013A688`): original keeps N in a0 for store and call argument, IDO loads it into a temp (T-2060) |
| SHOUGATU | `func_80134730` | promo | two straight-line `rec[i].f += k` with `i * 0x38`: original keeps 0x38 in a register (`li a0,56; multu`), IDO strength-reduces to shifts (also `func_80142784`, `func_80132E8C`) (T-2060) |
| SHOUGATU | `func_801339DC` | regorder | `switch (func_80051A68(..) & 0x7F)`: original result register one temp later than IDO (also `func_80134648`) (T-2060) |
| TT | `func_8013923C` | regorder | u32 temp from a u16 sum: original $t9 then $v1, IDO $v1 then $a0 (wave 2) |
| TT | `func_8013A040` | regorder | three loaded global pointers: original $t6/$v1/$v0 in that order, IDO $v0/$a0/$v1 (wave 2) |
| TT | `func_801480BC` | regorder | loop pointer and counter swapped: original pointer $s1 counter $s2, IDO the reverse (wave 2) |
| TT | `func_8014C5F8` | regorder | original zero in $a0 and the argument reloaded into $a2, IDO uses $zero and $v1 (wave 2) |
| TT | `func_8013C764` | regorder | u16 arguments copied back to $a0/$a1 in the original, IDO copies $a0 to $a2 (wave 2) |
| TT | `func_8014B274` | regorder | original keeps the global pointer in $s4 and the flag in $s3, IDO adds an $s3 copy of the flag (wave 2) |
| GEKO | `func_80133250` | regorder | call result `func_80051A68(..)` is copied to $v1, masked into $t6 and moved back to $v1; IDO masks straight into $v1 (T-2050) |
| GEKO | `func_801331A0` | regorder | same call-result copy `or v1,v0; andi t6,v1,0x7f; or v1,t6` as func_80133250; IDO andi v1,v0 (T-2050) |
| GEKO | `func_801339AC` | regorder | same call-result copy to $v1 then mask; IDO uses $v0/$t9 and sra instead of srl for a halfword field (T-2050) |
| GEKO | `func_80136F20` | regorder | same call-result copy to $v1 then mask into $t6 (T-2050) |
| GEKO | `func_80137040` | regorder | same call-result copy to $v1 then mask; also constant 6 kept in $a0 (T-2050) |
| GEKO | `func_8013BB4C` | promo | D_800E7384 loaded into $a0 and kept for compare and increment; IDO uses $v0 and recompares (T-2050) |
| GEKO | `func_8013375C` | regorder | call result masked into $t7 where IDO uses $t6 (one temp number later) (T-2050) |
| GEKO | `func_8013F800` | regorder | call result masked into $t7 where IDO uses $t6 (T-2050) |
| GYOZI | `func_8013BEA0` | regorder | pointer-table fill ending in `D_a = D_b` (s16 from another overlay): original `lui t9; lh t0,lo(t9)` (value in the next temp), IDO `lh t9,lo(t9)` and every later constant temp shifts by one (wave 2) |
| GYOZI | `func_8013C3B0` | regorder | same table-fill pattern as func_8013BEA0: load of the s16 into a fresh temp in the original, address register reused by IDO (wave 2) |
| GYOZI | `func_801418B0` | regorder | same table-fill pattern as func_8013BEA0 (wave 2) |
| GYOZI | `func_80143AD0` | regorder | same table-fill pattern as func_8013BEA0 (wave 2) |
| GYOZI | `func_80143BA0` | regorder | same table-fill pattern as func_8013BEA0 (wave 2) |
| GYOZI | `func_80143C70` | regorder | same table-fill pattern as func_8013BEA0 (wave 2) |
