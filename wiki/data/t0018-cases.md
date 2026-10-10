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

Update (T-1321): `tools/cvt_pass.py` now reproduces most `promo` rows (switch or compare chain on an unsigned global, reloads after calls); see [[matching-notes]], "Unsigned-load conversion pass (T-1321)". The rows stay as the test set; T-3000 retries them.

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
| EVENT | `func_8011A2C4` | regorder | `&&` chain of `== 1` compares: original re-materialises `li at,1` per compare, IDO keeps the constant in $v0 (batch EVENT) |
| EVENT | `func_8011A4A4` | regorder | `&&` chain of `== 1` compares: original re-materialises `li at,1` per compare, IDO keeps the constant in $v0 (batch EVENT) |
| EVENT | `func_80119600` | regorder | `&&` chain of `== 1` compares: original re-materialises `li at,1` per compare, IDO keeps the constant in $v1 (batch EVENT) |
| EVENT | `func_8011A144` | regorder | compare chain on a u8 global: original loads it into $t6, IDO $v0 and a different block order (batch EVENT) |
| EVENT | `func_8011A328` | regorder | compare chain on a u8 global: original loads it into $t6, IDO $v0 and a different block order (batch EVENT) |
| EVENT | `func_8011B5A8` | regorder | seven byte stores of 0x80: original keeps one `li v0,128` for all stores, IDO re-loads a fresh temporary per store (batch EVENT) |
| EVENT | `func_80119C9C` | regorder | nine halfword stores of 3, 4, 2: original reuses one register per constant ($t6, $t7, $t8) across the function, IDO re-loads each (batch EVENT) |
| EVENT | `func_80102C0C` | regorder | one constant (8) stored to two byte globals: original `li v0,8` for both stores, IDO a fresh temporary per store (batch EVENT) |
| EVENT | `func_8010AC40` | regorder | one constant (8) stored to two byte globals: original `li v0,8` for both stores, IDO a fresh temporary per store (batch EVENT) |
| EVENT | `func_80119074` | promo | `u8 == u8` compare chain: original keeps the first global in $v0 and loads it first, IDO loads the second operand first (batch EVENT) |
| EVENT | `func_80119518` | regorder | compare chain on a u8 global: original swaps the $v0/$v1 operands of the first `bne` (batch EVENT) |
| RPG_BAT | `func_8013F350` | promo | returns a global (or the old value of another) kept in $v0 across three blocks; IDO keeps it in $v1 and moves it to $v0 at the end (wave 2) |
| RPG_BAT | `func_8013D1D0` | regorder | first loaded global in $v0, IDO uses $a0 (wave 2) |
| RPG_BAT | `func_8013D290` | regorder | first loaded global in $v0, IDO uses $a0 (wave 2) |
| RPG_BAT | `func_80144B7C` | regorder | masked global loaded into $v0, IDO $v1 (wave 2) |
| RPG_BAT | `func_801515F0` | regorder | masked global loaded into $v0, IDO $v1 (wave 2) |
| RPG_BAT | `func_8013D030` | regorder | temporaries one register later than the original after the first global load (wave 2) |
| RPG_BAT | `func_8013C32C` | regorder | callee-saved register order: original keeps a global in $s4, IDO $s5 (wave 2) |
| RPG_BAT | `func_8015A744` | regorder | callee-saved registers $s2/$s3 swapped, $v1 vs $v0 for a loaded halfword (wave 2) |
| TAIIKU | `func_801339C4` | regorder | compare chain on a global: original loads it into $v1, IDO $v0 (wave 2) |
| TAIIKU | `func_80146A60` | regorder | global loaded into $v0 and tested several times, IDO uses $v1 (wave 2) |
| SHUGAKU | `func_8013A274` | regorder | two sequential `if (D == k)` tests on a `u8` global around a call: original loads it into $t6 and again into $t7, IDO uses $v0 (T-2100) |
| SHUGAKU | `func_80138DD8` | regorder | constant 1 hoisted into $v0 for three compares; last compare is `bne t8,v0` in the original, `bne v0,t8` built (T-2100) |
| ENDING | `func_80137574` | regorder | `D \|= 3` on a `u8` global: original computes `ori t7` then `andi v0,t7,0xff` into $v0 before the `sb`, IDO emits `ori` into another temp with no `andi` (T-2100) |
| 80085E30 | `func_80086640` | regorder | swap of two s16 globals: original loads the first into $v1, IDO into $t6 (T-2090) |
| 8004AD60 | `func_8004AD60` | regorder | clamp of a u8 global: original `li $v0,8`, IDO `li $t8,8` (T-2090) |
| 8004F870 | `func_8005352C` | regorder | `u16 >> 12` range test: original temp in $v1 (srl), IDO $t6 (T-2090) |
| 8006CB30 | `get_weekly_bg_sector` | regorder | two dependent byte loads: original $v1/$a0, IDO $t6/$v1 (T-2090) |
| 80062CD0 | `func_8006C700` | regorder | bit-field extract of one global: original $v0/$a0/$a1/$v1, IDO $v0/$v1/$a0/$a1 (T-2090) |
| 80062CD0 | `func_8006C934` | promo | global kept in $v0 across both branches and stored in the branch delay slot; IDO reloads (T-2090) |
| 80079B10 | `func_8007B734` | promo | two read-modify-write globals: original loads them into $v0/$v1 right after the call and stores last, IDO sinks both loads (T-2090) |
| 80079B10 | `func_8007A868` | promo | same as `func_8007B734` (loads of $v1/$a0/$v0 hoisted above nine stores) (T-2090) |
| 80079B10 | `func_80079F00` | promo | read-modify-write of one global, original keeps the loaded value in $t7 across the next test and stores it in the branch delay slot (T-2090) |
| 80079B10 | `func_80083628` | regorder | call result masked with 0x7F: original `move $v1,$v0; andi $t6,$v1`, IDO `andi $v1,$v0` (T-2090) |
| 80079B10 | `func_800836A0` | regorder | same `& 0x7F` temp ($t8 vs $t7) and loads sunk differently (T-2090) |
| 80079B10 | `func_8007BF04` | regorder | s16 parameter read once into $v0 and compared twice, IDO reloads it from the home slot (T-2090) |
| 80079B10 | `normal_date_bg_fadein` | regorder | clamped u8 global: original keeps the value in $v0, IDO $v1 (T-2090) |
| 80079B10 | `normal_date_bg_fadeout` | regorder | same as `normal_date_bg_fadein` (T-2090) |
| 80079B10 | `func_8007B358` | regorder | `lhu` temp is $t7 in the original ($t6 holds the `lui`), IDO reuses $t6; parameter copies in $a2/$a3 (T-2090) |
| 80079B10 | `func_8007B3DC` | regorder | same as `func_8007B358` (T-2090) |
| 80079B10 | `func_8007B460` | regorder | same as `func_8007B358` (T-2090) |
| 80079B10 | `func_80085368` | regorder | s16 loop counter passed to a call: original passes $s0 and re-normalizes after the increment, IDO sign-extends at the call (T-2090) |
| 80079B10 | `func_800853FC` | regorder | same as `func_80085368` (T-2090) |
| 800451D0 | `func_8004636C` | regorder | 16-bit checksum loop: sum in $v0, counter $v1, limit $a2 in the original, IDO allocates $a2/$a3/$v0 (T-2090) |
| 80062CD0 | `func_8006B014` | regorder | counting loop over a flag word: word in $v0 and argument in $a0 in the original, IDO moves the argument to $a2 and loads into $a0 (T-2090) |
| 8006CB30 | `func_8006CE84` | regorder | loop with a base pointer and a constant bound: original $s1 base / $s2 bound, IDO swaps them (T-2090) |
| 80059710 | `MouseState` | regorder | pointer table entry: original $v0 (address) / $a2 (entry) / $v1 (result), IDO $a2/$v1/$v0 (T-2090) |
| 80043510 | `load_csr_ab` | regorder | the two stack arguments are loaded before the stores and the index sits in $v1, IDO uses $t9 and loads them late (T-2090) |
| 80043510 | `load_csr_tp` | regorder | same as `load_csr_ab` (T-2090) |
| 80058D20 | `initCoordinate` | regorder | pointer loop: original emits the end-pointer `addiu` before the start-pointer `addiu` (T-2090) |
| 80061710 | `func_800618B0` | regorder | same as `initCoordinate` (T-2090) |
| 80062CD0 | `hizuke_disp_switch` | regorder | unrolled 4x byte loop: original loads the first element first, IDO schedules it last (T-2090) |
| 80062CD0 | `parameter_disp_switch` | regorder | same as `hizuke_disp_switch` (T-2090) |
| 80079B10 | `vram_bustup_clear` | regorder | s16 loop: original loads the constant after the base address and schedules the `addu` after the compare (T-2090) |
| 80079B10 | `SD_CalcCDAve` | regorder | loop with u16 temporaries: base pointer $a1 and value $a0 in the original, IDO swaps them (T-2090) |
| 80079B10 | `SD_DetectCDPeak` | regorder | same family as `SD_CalcCDAve` (T-2090) |
| 80079B10 | `func_8007B2B4` | regorder | switch variable after a read-modify-write store: original reuses $v0, IDO allocates $v1 (T-2090) |
| 80079B10 | `func_8007A6AC` | regorder | struct fields loaded into $t7/$a0/$v1 and spilled; IDO uses $t6/$a0/$a2 (T-2090) |
| DATE | `func_8015A3B0` | promo | switch (s32 global D_80122CD0, two cases): original selector in $v1, IDO $v0 (u32 cast no help) (T-4060) |
| 800674B0 | `func_80067DD4` | regorder | `if (!(D & 1)) D = (D or 3) & 0xFF` on u8: original keeps the load in $v0 and stores `andi v0,t7,0xFF`; IDO drops the andi or promotes to $v1 (T-4060) |
| 800674B0 | `func_8006C934` | promo | s16 global compared, then `D += 1` / `D = 0x48` in separate arms: original keeps D in $v0, increments with sll/sra and stores in the delay slot of `jr`; IDO reloads/adds without the sign extension (T-4060) |
| 800674B0 | `func_80067DFC` | regorder | same shape as `func_80067DD4` on D_80120652 (T-4060) |
| 80085E30 | `func_80086640` | regorder | swap of two s16 globals (temp = A; A = B; B = temp): original loads B into $v1, IDO uses $t6 (T-4060) |
| DATE | `func_8014E38C` | promo | `D_800E7384 += 1` then unsigned compare with 0x10: original keeps the global in $v1, IDO uses $v0 (T-4060) |
| SHOUGATU | `func_80138158` | promo | `D_800E7384 += 1; if (D_800E7384 == 1)`: original keeps the global in $v1 (xori/sltiu compare), IDO uses $v0 (T-4060) |
| ETC | `func_801428EC` | regorder | call argument `u8 * 0x10101 + 0x141414`: original loads the u8 global into $v0 and computes in $t6/$v0, IDO uses $t7/$t8/$t9 (T-4060) |
| SHOUGATU | `func_801388F0` | regorder | `rec[idx].b or 4` on a byte record field then a string copy: original loads into $t0, ori into $t1, constant into $t2 (a temp skipped); IDO uses $t9/$t0/$t1 (T-4060) |
| SHOUGATU | `func_80138964` | regorder | same as `func_801388F0` with the constant 0x17 (T-4060) |
| SHOUGATU | `func_80138E60` | regorder | same as `func_801388F0` with the constant 8 (T-4060) |
| SHOUGATU | `func_80138ED4` | regorder | same as `func_801388F0` with the constant 0xB (T-4060) |
| GYOZI | `func_8013B6CC` | regorder | `girl[i].unk_10[0] or 4` then a string copy: same skipped-temp pattern as SHOUGATU `func_801388F0` (T-4060) |
| GYOZI | `func_8013B740` | regorder | same as `func_8013B6CC` with the constant 0x17 (T-4060) |
| BUNKA_SD | `func_8013261C` | regorder | `if (A != 4 && B == 0x14)`: original compares `bne v0,t0` (constant 0x14 kept in $v0 across both compares), IDO emits `bne t0,v0` (T-4060) |
| RPG_BAT | `func_80144640` | regorder | switch on `D & 0xFFFF0000` (compare chain, 8 cases): same chain order as the original, but the selector sits in $v1 (original $v0, loaded into $v0 and copied with `or v0,t6,zero` for the default) (T-4060) |
| TT | `func_8013BE08` | regorder | switch on `arg1 & 0xFFFF` (5 cases 40..44, jump table): original keeps the masked selector in $t6 and stores it to the record, IDO puts it in $v0; case constants load in another order (T-4060) |
| DATE | `func_801573F0` | regorder | three u8 `+= 0x10` and a compare of the last: original takes $t7/$t8/$t9/$t0 and $t6 for the final `andi`, IDO starts at $t6 (T-4040) |
| DATE | `func_801572E4` | regorder | `D_a = D_b = D_c = 0x7F` byte stores: original loads 127 once into $v0 and stores it three times, IDO loads it per store ($t2/$t3/$t4); also hoists the `lbu` of the second masked global (T-4040) |
| TT | `func_801473E8` | regorder | pointer loaded from a global and used for 12 stores with `p += 0x208` in the middle: original keeps the pointer in $v0 the whole way (load and add in one register), IDO loads into $t6 and adds into $v0 (T-4040) |
| TT | `func_80149B20` | regorder | two global pointers, 15 halfword/word/byte stores through `p + 0x10` and `a += 0x10CC`: original keeps them in $v0/$v1 with the constant in $a0, IDO uses $t6/$v1/$t9 and $a1 (T-4040) |
| TACO | `func_8013D2A0` | regorder | ring-buffer slot (`D % 16`, 0x14-byte records) returned at the end: original keeps the slot in $t3 and reloads the s16 counter after the record stores (old value compared with 0x11); IDO keeps the counter in $v0/$v1 and the slot in $a3 (T-4040) |
| TT | `func_801473BC` | regorder | global pointer loaded and used for 6 byte stores with `p += 0x208` in the middle: original loads straight into $v0 and adds in place (`nop` after the `lw`), IDO loads into $t6 and adds into $v0 (T-4040) |
| SHOUGATU | `func_8013AE0C` | regorder | `(u8)D_800E652A >> 4` compared with `(u16)D_800E6374 >> 12`: original loads the u8 global into $v1 (srl $v0), IDO loads it into $v0 and shifts into $t6 (T-4040) |
| DATE | `func_80152C3C` | regorder | `if (a == 7 && D == 1 && E == 1)`: original rebuilds the constant 1 in $at for each compare, IDO loads it once into $v0 and reuses it (T-4040) |
| GEKO | `func_80140EBC` | regorder | `if (D_800E7384++ == 1)` (post-increment of the u32 selector): original materialises `(old ^ 1) < 1` in $v0 and branches with `beqz $v0`, IDO branches on the xori result ($v0 vs $t7 for the sltiu in the `!(x ^ 1)` form) (T-4040) |
| SHOUGATU | `func_80139C10` | regorder | same post-increment compare as GEKO `func_80140EBC` (T-4040) |
| SHOUGATU | `func_80137940` | regorder | `if (D_800E7384++ >= 0x400)`: original loads the selector into $v1 and builds `sltiu v0; xori v0,1`, IDO loads into $v0 and branches on `sltiu $at` (T-4040) |
| KANGEI | `func_80138944` | regorder | `(u16)D >> 12 == (u8)D2[idx * 0x38] >> 4`: original loads the u8 index global into $t6 and the table byte into $t0, IDO uses $v0/$t9 and loads the halfword first (T-4040) |
| TACO | `func_801421AC` | regorder | `v == 3 \|\| v == 4` on three s16 fields: code identical but every `beq`/`bne` has the constant register ($a0/$a1) as `rs` and the field ($v1) as `rt` in the original; IDO emits them the other way round, whatever the operand order, `switch` or `if` form (T-4040) |
| TACO | `func_801420DC` | regorder | same swapped `beq`/`bne` register operands as `func_801421AC` (T-4040) |
| GEKO | `func_80141514` | regorder | three byte stores `D1 = 8; D2 = 0x4D; D3 = 8`: original loads 8 once into $v0 and stores it twice, IDO loads a fresh temp per store (T-4040) |
| TT | `func_8014C5F8` | regorder | slot setup after `func_8014AF00(..)`: original keeps `arg0` in $a2 and builds a zero in $a0 for the two byte stores `p[0xE]`, `p[0xD]`, then reuses $a0 for the call argument; IDO stores $zero and reloads `arg0` into $v1 (T-4040) |
| TT | `func_80132000` | regorder | animation-frame step with a base pointer and a derived pointer (`q = p + n * 3`, later `q = p`, `q += 2`): original has p in $v1 and q in $v0, IDO swaps them (rest of the function matches) (T-4040) |
| GEKO | `func_80140FD0` | regorder | bit-field extraction `(D << 20) >> 29` of a u32 global used three times plus a read-modify-write of `D_800E6280[0x431]`: original keeps the field in $v0 and the array base in $v1 (`addiu v1,v1,%lo`), IDO uses $v1 and folds the offset into `lbu %lo(D+0x431)` (T-4040) |
| TT | `func_801475C8` | regorder | loop over six 0x68-byte slots with a call: the loop bound 6 and the constant 0x40 live in $s2/$s3; the original has 6 in $s2 and 0x40 in $s3, IDO swaps them for every loop form tried (T-4040) |
| TT | `func_8014A814` | regorder | unrolled loop (12 slots, 0x54 bytes each) storing five shared constants (12, 0x40, 0x3C87, 0xA, 3): original keeps them in $t2..$a2, IDO in $t0..$a0 (two registers lower; same descending order) (T-4040) |
| TAIIKU | `func_80141964` | regorder | four 8-byte s16 arrays on the stack filled with shared constants (-0x80, 0xFC, 0x10 three times each): original allocates the shared constants to $t6/$t2/$t3 and the single-use ones to $t7-$t9, IDO hands out registers in first-use order; layout matches only with one extra leading 4-byte local (T-4040) |
| TT | `func_80149400` | regorder | state step with a mask table lookup, a call in the middle and a pointer copy kept across it: original keeps the mask in $v1 and the pointer copy in $v0 (with `sw $t6,0x28($sp)` around the call), IDO takes $a0/$a1/$a2 for the same values (T-4040) |
| TACO | `func_80143574` | regorder | record setup with a call result kept in a spilled local (`temp_v0`, `sp28`) and the third argument split into bytes: frame matches with two pointer locals, but the original loads `arg2` into $t0 and shifts into $t7/$t9, IDO uses $v1/$t6/$t7 for the whole body (T-4040) |
| TT | `func_8014B088` | regorder | sibling of the matched `func_8014B4A4` (six slots): the constant 0x40 is shared between the `sh` store and the call, original passes the $s5 copy with `andi a1,s5,0xffff`, IDO folds it to `li a1,64` for every form tried (T-4040) |
| SHOUGATU | `func_80139E54` | regorder | `D_800E6280[idx * 0x38 + 0x1C8] \|= 4` then two stores and a string call: original takes $t0/$t1/$t2 for the byte, the `ori` and the constant (skipping $t9), IDO takes $t9/$t0/$t1 (T-4040) |
| SHOUGATU | `func_80132E8C` | regorder | three straight-line `D_800E6280 + idx * 0x38 + off` read-modify-writes: original keeps 0x38 in $a0 (`li; multu`) and the base in $v1 across them, IDO shifts and subtracts per access (T-0018 row family `idx * 0x38`) (T-4040) |
| KANGEI | `func_80138470` | regorder | two `D_800E6280 + D_800E71DF * 0x38` read-modify-writes and a later `func_80084D3C(0x38)`: original reloads the index and multiplies twice with 0x38 kept in $a0 (`li; multu`), IDO merges the two address computations and uses shifts (T-4040) |
| TACO | `func_8013546C` | regorder | s8 global read, `+= 1` twice under two flag bits, clamp to 0..4, flags global reloaded after a call: original holds the s8 value in $v0 (`lb v0`, `addiu v0,v0,1`, `sll/sra`), IDO loads it into $a0 and moves it, for s8, s32 and char locals (T-4040) |
| DATE | `func_801378BC` | regorder | global index in $t6 (load, shifts, base) where IDO promotes it to $v0; same shape in `func_801381B4`, `func_801386C8` (T-4020) |
| DATE | `func_801381B4` | regorder | same shape as `func_801378BC` (T-4020) |
| DATE | `func_801386C8` | regorder | same shape as `func_801378BC` (T-4020) |
| DATE | `func_8013BEB8` | regorder | constants 2/3 stored to ten globals: original loads each constant once and reuses it, IDO reloads per store (T-4020) |
| DATE | `func_8013FA6C` | regorder | constant 0x80 stored to eight byte globals: original keeps it in $v0, IDO loads it per store (T-4020) |
| DATE | `func_8013A2E0` | regorder | two statements `A = B; C = D;` in a row: original loads D after the first store, IDO hoists the load above it (T-4020) |
| DATE | `func_80139154` | promo | address high half of D_800E71DF kept in $v1 across a call and reused for the reload; IDO uses a fresh register (T-4020) |
| DATE | `func_8013A898` | regorder | two `idx * 0x38` in straight line: original keeps 0x38 in a register (li; multu), IDO uses shifts (T-4020) |
| DATE | `func_80139944` | regorder | six `idx * 0x24` accesses: original keeps 0x24 in $v1 and uses multu, IDO uses shifts (T-4020) |
| DATE | `func_8013F544` | regorder | three `idx * 0x38` accesses: original keeps 0x38 in a register (li; multu), IDO uses shifts (T-4020) |
| DATE | `func_8013D23C` | regorder | constant 1 shared by two compares: second `bne` has the operands as ($v0,$v1) in the original, ($v1,$v0) in IDO; `1U` on one side removes the sharing instead (T-4020) |
| DATE | `func_8013D2C8` | regorder | same shape as `func_8013D23C` (T-4020) |
| DATE | `func_80136F2C` | regorder | loop over two s16 arrays: hoisted base addresses in $t0/$t1 are swapped (T-4020) |
| DATE | `func_8013D9E8` | regorder | three read-modify-write statements on adjacent globals: original loads after the earlier store, IDO hoists (T-4020) |
| DATE | `func_80140D38` | regorder | u8 counter with wrap: original keeps the sum in $v0 and the masked copy in $t8, IDO uses $t8/$v1 (T-4020) |
| DATE | `func_80145DF0` | regorder | read-modify-write before a call: original schedules the `sb` one slot later (T-4020) |
| DATE | `func_80149268` | regorder | struct copy loop: original keeps the entry pointer in $v0 and copies it to the two walkers $t2/$t3, IDO derives both from $t3 (T-4020) |
| DATE | `func_8013D174` | reverse | D_800E62BF read twice in the original (once per compare), IDO keeps one copy in $v1 (T-4020) |
| DATE | `func_801395B4` | regorder | `lui $v0` for D_800E71DE sits one slot earlier in the original (before the store to D_800CA148) (T-4020) |
| DATE | `func_8013E6A4` | regorder | u8 local spilled across two calls: original at sp+0x2B, IDO at sp+0x2F (one more word local above it in the original) (T-4020) |
| DATE | `func_80138418` | regorder | dead store of D_800E738A to a stack local kept in the original, IDO removes it (T-4020) |
| SHUGAKU | `func_80135994` | regorder | consecutive `A = B; C = D;` byte copies: original loads D after the first store, IDO hoists the load (T-4020) |
| SHUGAKU | `func_8013A9E0` | regorder | same family as `func_80135994` (T-4020) |
| SHUGAKU | `func_80135A80` | regorder | return value of the last of four byte copies: original loads D_800B593C with `lui $v0`, IDO uses `$v1` as address register (T-4020) |
| SHUGAKU | `func_80135B68` | regorder | `return -0x81` after the last halfword store: original puts the store in the `jr` delay slot, IDO puts the `addiu $v0` there (T-4020) |
| SHUGAKU | `func_801385A8` | promo | s16 global loaded into $v0 before a branch with three calls and reloaded after them into $v0; IDO spills it to the stack (T-4020) |
| SHUGAKU | `func_80138DD8` | regorder | `bne` operand order of a compare against a constant held in $v0 (T-4020) |
| SHUGAKU | `func_8013A980` | regorder | epilogue `lw $ra` is duplicated into a branch delay slot in the original (T-4020) |
| SHUGAKU | `func_8013A274` | regorder | global loaded into $t6/$t7 for two compares around a call, IDO uses $v0 twice (T-4020) |
| SHUGAKU | `func_80138320` | promo | switch on an s32 global, compare chain on $v1 in the original and $v0 in IDO (T-4020) |
| SHUGAKU | `func_80135DB8` | regorder | two `idx * 0x38` with the constant shared with the call argument 0x38: original li; multu, IDO shifts (T-4020) |
| 80042540 | `func_80042808` | regorder | `return 0` after the last store: original puts the store in the `jr` delay slot, IDO puts `or $v0,$zero,$zero` there (T-4020) |
| 80042540 | `func_8004284C` | regorder | same shape as `func_80042808` (T-4020) |
| 80042540 | `func_80042908` | regorder | same shape as `func_80042808` (T-4020) |
| 80042540 | `func_80042940` | regorder | same shape as `func_80042808` (T-4020) |
| GYOZI | `func_80142D40` | promo | index global re-read after the table call into $v1, IDO uses a temporary; compare operand order differs (T-4020) |
| GYOZI | `func_80134D9C` | regorder | three `idx * 0x38` accesses with 0x38 shared with the call argument: original li; multu, IDO shifts (T-4020) |
| TACO | `func_80136C60` | regorder | u8 counter times 3 with multu: original loads into $v0 and keeps 3 in $v1 and the new value in $v0, IDO uses $v1/$a0 (T-4020) |
| TT | `func_80142DA8` | regorder | RECT stores: original stores the halfword at sp+0x2C after the other fields, IDO stores it earlier (T-4020) |
| TT | `func_80146BE0` | regorder | unrolled byte clear: pointer in $v1, counter $v0, bound $a0 in the original; IDO picks $a0/$v1/$v0 (T-4020) |
| TT | `func_80146C14` | regorder | same shape as `func_80146BE0` (T-4020) |
| TT | `func_80148210` | regorder | same shape as `func_80146BE0` (T-4020) |
| TT | `func_80148714` | regorder | u16 parameter reduced modulo 100 in place: original keeps it in $a0 and the divisor 10 in $v1, IDO uses $v1/$a2 (T-4020) |
| BUNKA_SD | `func_80134418` | regorder | constant 0x80 for a loop store and a global store: original reuses $v1 (the old counter), IDO uses $v0 and moves the pointer to $v1 (T-4020) |
| EVENT | `func_8010564C` | regorder | loop with two calls: the constants 84, 130, 8, 0x24 are kept in saved registers and used as division operands in the original; IDO uses immediates (T-4020) |
| DATE | `func_80149AC0` | regorder | `func_80051A68(..) & 0x7F` result and every later temporary one register later than IDO (known wave-2 shape) (T-4010) |
| ENDING | `func_80139EC4` | promo | u8 global `D_80121874` loaded into `$a1` (also the call argument) and re-loaded into `$a1` in the loop; IDO uses `$v0` for the first load and a different chain (T-4010) |
| OPTION | `func_80139F8C` | regorder | s32 global `D_800E7208` in `$v1` for two masked compares, u8 `D_8013D3A8` loaded to `$t1` then re-loaded into `$a0`; IDO uses `$v0` and one load (T-4010) |
| OPTION | `func_80139F04` | regorder | clamp of s32 global `D_8013E730`: original loads it into `$v0` both times, IDO takes `$a1` for the first load and copies (T-4010) |
| EVENT | `func_80100DD8` | regorder | `D_80094714 += D_800EECBC` then the same global as 5th argument: original loads it after the `lhu`, IDO hoists the `lw` above it (T-4010) |
| GEKO | `func_80140040` | regorder | loop with constants `0x44`, `0x1E`, `-0x81`, `-1`, `1` held in `$a1,$a2,$t1,$a3,$t0` for the whole function; IDO keeps them as immediates or other registers and uses other temporaries (T-4010) |
| DATE | `func_801511B4` | promo | u8 global `D_800B593C` loaded once into `$v0` through `lui $v1` and kept for four stores; IDO uses `lui $v0; lbu $v0` (T-4010) |
| ENDING | `func_80139F3C` | promo | u8 global `D_80121874` kept in `$a1` across the tests, the loop and the final shifts (re-loaded into `$a1`); IDO uses `$s0/$v0/$v1` (T-4010) |
| ENDING | `func_8013B510` | regorder | `u32` loop counter: original does `addiu s0,s0,1; andi t0,s0,0xff; ...; move s0,t0` with the next value in `$t0`, IDO takes `$a0` (the call-argument register) (T-4010) |
| OPTION | `func_80139878` | promo | u8 global `D_800E62BA` loaded into `$v1` (+4) and the masked copy in `$v0`, then reused for the compare and the `0x80` store; IDO swaps `$v0/$v1` and the later temporaries (T-4010) |
| TT | `func_801364EC` | regorder | switch on a `u16` field: the original reuses the selector register `$v0` for the later `lhu` of the counter, IDO takes `$a0` (everything else matches) (T-4010) |
| 80047550 | `func_80047560` | promo | u8 global `D_801255DC` kept in `$a0` for the loop bound, the `rsin` result math and the `+4` store (re-loaded into `$a0` after the call); IDO uses `$a3/$a1/$t9` and a different temp chain (T-4010) |
| RPG_BAT | `func_8013D1D0` | regorder | `D_80120E07 &= ~0x80` in the else branch: original materialises `-0x81` in `$v1` (`and t6,t5,v1`), IDO folds it to `andi 0xFF7F` for every spelling tried (T-4010) |
| 8007C030 | `normal_date_move_place` | regorder | switch on lw global (also tried u32 and if-chain): original selector in $v1, IDO $v0 (T-4080) |
| GYOZI | `func_801399C0` | regorder | `D_800F5750 \|= 2`: original loads into $t8 with base $t7, IDO reuses $t7 as destination (T-4080) |
| 8007C030 | `normal_date_girl_in` | regorder | same shape as `normal_date_move_place`: switch on lw global D_800E7384, original selector in $v1, IDO $v0 (T-4080) |
| GEKO | `func_80135A6C` | regorder | `if (D++ == 1 && ...)` on lw global: original keeps it in $v1 and materialises the compare (`xori; sltiu 1; beqz`), IDO branches with `xori; bnez` (T-4080) |
| 8007C030 | `normal_date_bg_fadein` | regorder | u8 global `+4`, clamp, store, compare: original keeps the masked temporary in $t7 and copies it to $v0, IDO propagates one register (T-4080) |
| 8007C030 | `normal_date_bg_fadeout` | regorder | same shape as `normal_date_bg_fadein` with `-4` (T-4080) |
| NAME_ENT | `func_80140B20` | regorder | u8 global `-2` stored to six bytes and back: original loads the global into $a0 and reuses it, IDO uses $v1/$v0/$a1 (T-4080) |
| 8007C030 | `normal_date_girl_suddenin` | regorder | seven byte stores of 0x80: original keeps the constant in $v1/$v0 with a `move`, IDO emits a fresh `li` per store; chained assignments do not help (T-4080) |
| TACO | `func_801368D8` | regorder | `u8` parameter used as modulus (`(u32)(*p + n + 1) % n`): IDO adds `move v0,a1` before the zero check and takes $v1 for the loaded byte (T-4080) |
| GEKO | `func_80136F20` | regorder | `g = func_80051A68(x) & 0x7F` then compare chain: original `move v1,v0; andi t6,v1,0x7F; ...; move v1,t6` (copy kept), IDO folds to `andi v1,v0,0x7F` (T-4080) |
| GEKO | `func_80137040` | regorder | same `& 0x7F` copy shape as `func_80136F20` (T-4080) |
| GEKO | `func_801331A0` | regorder | same `& 0x7F` copy shape as `func_80136F20` (T-4080) |
| 8007C030 | `func_80083628` | regorder | same `get_g_zyotai_h(x) & 0x7F` copy shape as GEKO `func_80136F20` (T-4080) |
| TAIIKU | `func_80136DA4` | regorder | `v = D_8014929C + D_80149218`: original loads the first global straight into the result register $v1, IDO loads into $t6 and adds into $v1 (T-4080) |
| GYOZI | `func_8013CAA4` | regorder | a flag OR on `D_800F53A0.girl[D_800F62CF]` before a strcpy of a literal: code identical but the original numbers the temporaries $t0..$t2 where IDO uses $t9,$t0,$t1 (one unsigned-index temporary more consumed in the original) (T-4080) |
| 8007C030 | `select_girl` | regorder | same shape as `normal_date_move_place`: switch on lw global D_800E7384, original selector in $v1, IDO $v0 (T-4080) |
| 8007C030 | `select_girl2` | regorder | identical to `select_girl` (T-4080) |
| NAME_ENT | `func_8013DAC4` | promo | signed byte global D_800E7313 compared with -1 twice and read again after a call: original keeps it in $v1 with -1 in $a1, IDO loads into $v0/$a0 (T-4080) |
| BUNKA_SD | `func_801362EC` | promo | unsigned byte global D_800E738A tested in two range checks around a call: original keeps it in $a1 (loaded once, reloaded after the call), IDO uses $a0 or spills a local (T-4080) |
