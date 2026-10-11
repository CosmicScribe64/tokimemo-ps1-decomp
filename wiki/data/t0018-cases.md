---
type: data
updated: 2026-10-10
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

Update (T-5010): `tools/cvt_pass.py` is in the build, with an entry rule (only globals touched before the first call or branch) and a compare rule; unit-private selectors that the original keeps in `$v0` are written as a switch on a `FAKE` local copy. Batch guidance and numbers: [[matching-notes]], "Selector register rule (T-5010)". Keep recording rows as before: they stay the test set.

Update (T-7000): the build runs IDO in K&R mode with the widening of narrow globals ([[matching-notes]], "K&R promotion rules (T-7000)"). Rows now matched with plain C: TACO `func_8014EDCC`, EVENT `func_8011A2C4`, `func_8011A4A4`, `func_80119600`, `func_80102C0C`, DATE `func_8013BDB4`, TT `func_8013C764`, main `func_8007B5EC`, `SD_DetectCDPeak`. The `li at,k` per compare and the K&R parameter copies are no longer T-0018 shapes; U1 selectors after a call (main `func_800626B0`, `func_8005AE60`) still are.

Update (T-9000): re-tested under K&R with the wave agents' drafts ([[matching-notes]], "T-0018 rows under K&R: re-test, clusters, pass verdict (T-9000)"). Rows now matched in plain C: GYOZI `func_8013B6CC`, `func_8013B740`, `func_8013CAA4`, `func_8013BC40`, `func_8013BCB4`, `func_8013B644`, SHOUGATU `func_80138E60`, `func_80138ED4`, `func_80139E54` (bit-field flag store); SHUGAKU `func_80134024`, `func_8013A980`, GEKO `func_8013E56C`, `func_80140EBC`, SHOUGATU `func_80138158`, DATE `func_80148924`, TACO `func_8014E1A0`, OLH `func_80136210` (`s32` without a return value); EVENT `func_8010AC40` (chain assignment); RPG_BAT `func_80144640`, DATE `func_80145DF0` (draft unchanged). RPG_BAT `func_80142544` matches as `s32` but waits for the `D_8012121C` declaration. Before recording a new row, try `s32` without a return value and a bit-field store ([[decompile-workflow]]).

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
| EVENT | `func_8010A790` | regorder | `u8` global loaded, clamped (`if (v >= 2) v = 1`), used twice after: original keeps it in $v0, IDO $v1 (T-4090) |
| EVENT | `func_8010B614` | regorder | `D = 4; f(4);` keeps the constant in $a0 for the store in the original, IDO loads it into $t6 again (also with `f(D = 4)`) (T-4090) |
| EVENT | `func_8010B678` | regorder | four `D += 0x40` on neighbouring halfword globals: original interleaves the loads differently (third and fourth load before the second store) (T-4090) |
| GYOZI | `func_80135D64` | promo | `(u32)D_800F563A >> 4` compared twice: original loads the `u8` into $v1 and shifts into $v0, IDO loads into $v0 and shifts into $t7 (T-4090) |
| GYOZI | `func_801361D4` | regorder | `D_800F62CF = D_800F5ACD; f(D_800F5ACD)`: original `lui a0; lbu a0`, IDO `lui v0; lbu a0,0(v0)` (also as `f(D = x)` and via a `u8` local) (T-4090) |
| GYOZI | `func_801427E4` | promo | `s16` global `+= 0x200` then compared: original sign-extends into the same register ($v0 -> $t6 -> $v0, stores and compares $v0), IDO uses a fresh $t7 (also with a local) (T-4090) |
| DATE | `func_8014FD30` | promo | `if (D == 0) {..calls..} if (D++ == 0x80) f();` on an `s32` global: original keeps it in $v1, materialises `sltiu v0,v0,1` and branches on it; IDO branches on the `xori` (also with `u32`) (T-4090) |
| 80079B10 | `func_8007BE94` | regorder | `(u16)D == (s16)arg` branch: operands of the `bne` come in the other order (D first in the original) for every spelling tried (T-4090) |
| 80079B10 | `func_8007B64C` | regorder | `u16` count read once into $v0, `return 0` path: IDO hoists `move v0,zero` and puts the count in $v1 (T-4090) |
| 80079B10 | `func_80079C70` | regorder | loop over four flag bytes: the three mask constants (0x0F000000, 0xF0000000, 0x01000000) come in registers a2/a3/a0 in the original, IDO loads them in another order (T-4090) |
| 80079B10 | `func_8007A50C` | regorder | `u16 t = D44 + D48` operands are loaded in the other order and the `s16` difference gets its own sign-extension temps (T-4090) |
| EN_NICHI | `func_80133924` | regorder | loop with `i != 10` and `D == 7`: loop bound and the constant 7 are kept in $s2/$s3, the original has 10 in $s2 and 7 in $s3, IDO the reverse for every spelling tried (also `for`) (T-4090) |
| TAIIKU | `func_801448D4` | regorder | clamp loop over three 0x24-byte records with a local `s32[3]`: all code matches, the address temporaries of `tbl[i]` are $t3/$t4 in IDO and $t4/$t5 in the original (T-4090) |
| EN_NICHI | `func_80134784` | regorder | pointer loop `p += 0x30; while (p != end)` around a 5-entry switch: all code matches, IDO emits the `lui` of the end address and of the pointer in the other order (the pointer gets $v0 or the end address does, never the original pair $v1/$v0) (T-4090) |
| EN_NICHI | `func_80135A48` | regorder | ring-buffer insert (`p = &tbl[idx]; if (*p <= 0 || *p >= 10) {..}`): IDO keeps `arg1` in $a1 and uses other temps than the original ($a2/$a3 copies of the arguments); permuter best score 660 (T-4090) |
| SHUGAKU | `func_801350F4` | regorder | two `D_800E6280 + D_800E71DF * 0x38` record updates: the original keeps 0x38 in $a0 (also the call argument) and multiplies twice with `multu`, IDO shifts (T-4090) |
| DATE | `func_8014F370` | regorder | four-case `switch (D_80122CD0)` (`s32` global) calling one function per case: original selector in $v1, IDO $v0, also with `u32`, a `u32` override and an `s32` return type (T-4090) |
| 80043510 | `load_palette` | regorder | `if (D_800E76A8 < 0x20) { rec = D_800E6280 + D_800E76A8 * 8; ..4 stores..; D_800E76A8++; return 1; } return 0;` with `u8` parameters: IDO keeps the counter in $v1 and spills `arg0`; permuter best 60 (T-4090) |
| EVENT/800FD2F0 | `func_800FD360` | regorder | 3-case switch on an `s32` global: selector in $v1 in the original, IDO $v0; `u32` override and an if-chain do not help (T-4050) |
| EVENT/800F9680 | `func_800FA194` | regorder | two 4..0x12 loops over 0x24-byte records (`s16` counter, array base, 0x24 and mask constants in registers): original colours a2/v1/a0/a3, IDO a1/a0/v1/a2; permuter best 380 (T-4050) |
| EVENT/800F9680 | `func_800FA0D0` | regorder | `s16` loop over 0x24-byte records with a promoted `u8` global (`lbu a1`, base a2, constant a3, counter a0): IDO loads it into $a0 and copies to $a3; original frame is 8 bytes larger (T-4050) |
| GYOZI/80140740 | `func_80140780` | regorder | function-pointer table call whose result is returned (`ret` local, `s32`): original keeps `ret` in $v0 through the branches, IDO puts it in $v1 and copies to $v0 at the end (T-4050) |
| GYOZI/8013B920 | `func_8013BB0C` | regorder | `D_800F62CF = 3; f(3)` keeps the constant in $a0 for both the store and the call; IDO emits two `li` (same as the recorded wave-2 GYOZI init sequences) (T-4050) |
| TEL/801397D0 | `func_80139E3C` | regorder | `(D << 3) >> 28` index into a 0x38-byte table and an s16 table, two read-modify-write halfwords: everything matches except the `srl` result goes to $t7 where the original reuses $v0 (T-4050) |
| OPTION/8013A550 | `func_8013A550` | regorder | two flag-driven +1/-1 wrap blocks with an early `return arg1`: only difference is branch delay slots, original keeps `nop` and fills the final `jr` with `or v0,a0`, IDO copies the `move v0,a0` into the three branch delay slots (T-4050) |
| GYOZI/8013A920 | `func_8013AC38` | regorder | init sequence `D_800F5ACD = 8; D_800F62CF = 8; f(8)`: constant shared in one register in the original (same family as `func_8013BB0C`), not tried again (T-4050) |
| 80041000 | `func_800415B4` | regorder | `for (i = a; i < b; i++)` clearing an 18-store 0x24-byte record plus one byte of another table: original keeps one plain loop (guard, then `bne v0,a1`), IDO unrolls by 4 with a run-time remainder in indexed, pointer and do/while spellings (T-4050) |
| TACO/80151C30 | `func_80152110` | regorder | two 7-step loops calling a 5-argument function: registers and structure match, but the original fills the `jal` delay slot with the stack-argument `sw zero,0x10(sp)` after `move a3,zero`, IDO stores it earlier and fills with `move a3` (T-4050) |
| DATE/801552B0 | `func_801563A8` | reverse | function-pointer table call after `if (D < 0xB) f(); if (D == 0xB) g();`: the original reloads the `u8` global into a fresh temporary for each test and for the table index; IDO keeps it in $v0 (T-4050) |
| EVENT/800F9680 | `func_800FA6A0` | promo | function-pointer table call, then the same `u8` global is reloaded to index the table again and compare the entry with two function addresses: original loads it into $v1, IDO into $t4 (T-4050) |
| DATE/801586A0 | `func_8015908C` | reverse | function-pointer table call after `if (D < 8 && D2 == 0x80) f();`: original loads `D_800E738A` into $t1 for the compare and again for the index; IDO keeps one load in $v0 (same family as `func_801563A8`, `func_801591BC`) (T-4050) |
| DATE/801586A0 | `func_801591BC` | reverse | same shape as `func_8015908C` (compare of the table index global with a flag byte, then the table call), not tried separately (T-4050) |
| TACO/80137A60 | `func_80137CBC` | regorder | `p = D_8015F440 + arg0` (8-byte pairs) passed to a call: original starts with `move t6,a0` and builds the address from t6/t8, IDO uses $a0 directly and different temporaries (T-4050) |
| TEL/801397D0 | `func_8013BE38` | regorder | find-entry loop + three flag-byte clears + second loop: loops and masks match (bit-field tests, indexed 0x38 form) but the original frame is 8 bytes larger and its two `lbu` temporaries are t5/t3, IDO t3/t5, with the `sb` order of the clears off (T-4050) |
| GYOZI/8013A920 | `func_8013AA48` | regorder | `if (D++ == 1 && ...)` on an `s32` global: original materialises the compare (`xori v0,v1,1; sltiu v0,v0,1; beqz v0`), IDO branches on the `xori` result (`bnez v0`); `1U`, swapped operands, `^ 1) == 0` and a bool local do not change it (T-4050) |
| EVENT/8010FE10 | `func_8011A508` | regorder | `&&` chain of four compares (two against 1) then `D++ == 0`: original uses `li at,1` before each `== 1`; IDO shares the constant in $v0 and swaps the operand order of the last compare (known EVENT shape: constant per compare) (T-4050) |
| EVENT/800FD2F0 | `func_800FD588` | promo | `r = D % 60; if (r == 0) { f(); r = D % 60; } if (r == 0x1E) ...` on a `u32` global: original loads the global into $v1 and keeps the remainder in $v0; IDO swaps the two (T-4050) |
| EVENT/8010FE10 | `func_80119440` | regorder | two `== 1` tests (first `bne v1,t6`, second `bne v0,v1` with the constant kept in $v1): everything matches except the operand order of the second `bne`; source operand order, `1U` and casts do not change it (T-4050) |
| EVENT/8010FE10 | `func_8011B09C` | reverse | three read-modify-write halfwords at `D_800B0860 + D_800B1746 * 0x34 + off`: original reloads `D_800B1746` and multiplies again for each (shared base in $v1, 0x34 in $a0), IDO computes the record address once (T-4050) |
| EVENT/8010FE10 | `func_80119824` | regorder | compare chain with `== 3` twice: original uses `li at,3` before each compare, IDO keeps 3 in $v0 and swaps the operand order (same family as `func_8011A508`) (T-4050) |
| EVENT/8010FE10 | `func_80118868` | regorder | ten halfword stores of 2 and 3: the original loads each constant once and reuses the register, IDO loads per store (known EVENT shape) (T-4050) |
| EVENT/8010FE10 | `func_80117A68` | regorder | `&&` chain with two `== 2` compares: original loads `li at,2` before each, IDO keeps 2 in $v1 and swaps the operand order (same family as `func_8011A508`) (T-4050) |
| GYOZI/8013E5B0 | `func_8013E638` | reverse | `s16` loop over the 0x38-byte girl records comparing the index with the `u8` global `D_800F62CF`: original reloads the global inside the loop (`lui t9` per pass) and uses $v1 for the counter, IDO hoists it into $a2 (T-4050) |
| ETC/80143240 | `func_80143240` | regorder | four-way manual body over two byte tables with early `return 0` (i += 4, p += 4, `bne v0,a1` with a1 = 0xD): IDO unrolls/rewrites the loop (`li a0,4; beqz a0`) where the original keeps a plain loop (same family as `func_800415B4`) (T-4050) |
| VALEN/80132760 | `func_80133670` | regorder | long `&&` chain (bit-field compares, call, three table compares) matches after fixing operand orders, except the call argument: original loads `D_800E71DF` through a `lui v0` hoisted into the previous branch delay slot (`lbu a0,lo(v0)`), IDO uses `lui a0; lbu a0` (T-4050) |
| GYOZI/8013B920 | `func_8013BC40` | regorder | `girl[D_800F62CF].unk_10[0] or-assign 4; D_801474A8 = 8; D_801474AC = 0; strcpy(..., "...")`: all instructions match but every temporary from the `lbu` on is one register lower in IDO ($t9 where the original has $t0); permuter best 30 needs `& 0xFFFFFFFFFFFFFFFF` junk (T-4050) |
| GYOZI/8013B920 | `func_8013BCB4` | regorder | same as `func_8013BC40` (T-4050) |
| GYOZI/80135900 | `func_80135988` | regorder | everything matches except one store: the original stores `v` to `D_80145EB4` a second time inside `if (D_80145EC8 == 0)` (after the `lbu` of `D_800F62CF`), IDO drops the redundant store (T-4050) |
| DATE2/80132DE0 | `func_801338F8` | regorder | two blocks `v = (u8)f(x) & 0x7F; D += 10 (v >= 2); t = D; if (v >= 3) t += 10; D = t; if (v == 4) D = t + 10` on an s16 table entry: with the `(u8)` cast the temporaries and moves match, only the s16 local `t` is in $a0 instead of $v0 (T-4050) |
| 80041000 | `func_80041F48` | reverse | five pointer loops over D_800E6280 (end-pointer compares) match structurally, but IDO hoists the base address of D_800E6280 into $a2 for all loops where the original rebuilds `lui/addiu` in each (T-4050) |
| TT/8013F040 | `func_8013F878` | reverse | animation counters at a `u8 *` record (`p[0x7A] += 1; if (p[0x7A] >= 0x34U) p[0x7A] = 0x2E; p[0x8E] = p[0x7A]` x3, bit tests of the u16 at +0x364): all stores, constants and temporaries match, but the original reloads the u16 field (`lhu v1,0x364`) at the end of the then-block and in both arms of the last test, IDO keeps it in $a0 (extra `move a0,v1`) (T-4050) |
| EVENT/8010FE10 | `func_80119370` | reverse | two bit-field compares of D_800B0896/D_800B0897 against a record word and a global word: IDO keeps D_800B0896 in $v1 for both ifs, the original loads it into a temporary for each (T-4050) |
| DATE2/80132DE0 | `func_80136794` | regorder | sibling of `func_801366E8` with an extra bit-field update of D_800E699E and a call argument: after fixing operand orders three small differences remain: the call argument load uses `lui a1; lbu a0,lo(a1)`, and the loads of D_800E738A/D_800E699E are placed differently around the stores (T-4050) |
| EVENT/800F9680 | `func_800FAE6C` | regorder | a 0x44-byte block copy (`D_800EB028 = D_800EAFA0`) between two call sequences: all registers match, but IDO hoists the last `lw t4,4(t1)` above the preceding `sw at,0(t0)` of the copy tail where the original keeps them in order (T-4050) |
| TACO/80151C30 | `func_801522B0` | regorder | two byte counters through pointer arguments with a `30..34` range test and two call sites: original keeps only `arg0`, `arg1` in $s0/$s1 (the first `lbu` is a $v0 temporary, `a2`/`a3` stay in their stack home slots), IDO takes a third saved register for the loaded value in every spelling tried (T-4050) |
| DATE | `func_8015522C` | regorder | `u8 D |= 0x40` in an else branch: original lui t9, lbu t0, ori t1 (no reuse); IDO reuses t9 for the load (w3-3, T-4030) |
| SHUGAKU | `func_80134024` | promo | `if (D++ == 0x3C)` on an s32 global: original loads it into $v1, `xori v0,v1,0x3c; sltiu v0,v0,1; addiu v1,v1,1; beqz`; IDO emits `xori; bnez` without the sltiu (w3-3, T-4030) |
| NAME_ENT | `func_80134110` | promo | range checks on two s16 globals, early-out `or v0,v1,zero` return of the first global in $v1 (implicit int, no return): IDO keeps it in $v0 and takes other registers (w3-3, T-4030) |
| 80059710 | `MouseState` | regorder | pointer array element loaded into $a2, return value 0 kept in $v1 (`move v0,v1` at the end), element address in $v0 reused for reloads; IDO uses $v1/$v0/$a2 in another order (w3-3, T-4030) |
| TT | `func_8013C764` | regorder | two narrow (masked 0xFFFF) arguments copied back to $a0/$a1 (`or a1,t7` then `or a0,t6`); IDO moves the first one to $a2 or swaps the copy order (w3-3, T-4030) |
| SHOUGATU | `func_801345C8` | regorder | switch/compare chain (0,1 skip; 2 and default) on `f() & 0x7F`: original keeps the selector in $t7, IDO puts it in $v1 (w3-3, T-4030) |
| TACO | `func_80133410` | regorder | 16-record loop `p[i].unk13 &= ~0x20` unrolled 4x: same code, but IDO schedules the first record's load/and after the other three (w3-3, T-4030) |
| DATE | `func_801551D0` | promo | `if (++D < 0x10) return 0;` (s32 return, global incremented): original keeps the global in $v1 (v0 is the return value), IDO uses $v0 (w3-3, T-4030) |
| SHOUGATU | `func_8013315C` | regorder | two indexed loads through the same `D_80143B00 * 4` offset: the original also leaves a copy `or v0,t6,zero` of the CSE temp and schedules differently; everything else (post-increment compare, string) matches (w3-3, T-4030) |
| SHOUGATU | `func_80137DE4` | promo | if-chain on an `s16` global (3/0xB, then 5/0xD) after a call: original loads it into $v1, IDO uses $v0 (w3-3, T-4030) |
| KANGEI | `func_80134120` | regorder | two `D_800E71DF != 0xFF` tests: the original keeps the constant 0xFF in $v0 for both and loads the global into $t6; IDO uses `li at` and $v0 for the load (w3-3, T-4030) |
| TT | `func_8013D2E0` | regorder | GPU TILE packet + addPrim with bitfield tag: with a bitfield `TtTag` the code is right but the original keeps the packet pointer in $a1 / buffer in $v1 and stores `code` before `len` after loading 3 first; IDO swaps registers and order (w3-3, T-4030) |
| RPG_BAT | `func_80144B7C` | regorder | switch on `D & 0xFFFF0000` with default returning the selector: original keeps the loaded word in $v0 and the masked selector in $t6 (`move v0,t6` in the first delay slot), IDO loads into $v1 (w3-3, T-4030) |
| GEKO | `func_80144198` | regorder | month/day style helper with a 24-byte local s16 table: the original frame has only the table (no scalar slots, argument kept in $a2); with the five scalar locals IDO spills the argument and moves the table (w3-3, T-4030) |
| TAIIKU | `func_80140738` | regorder | GsMapModelingData/GsLinkObject4 pair with `arg0 += 4` kept across both calls: the original has the two spilled locals adjacent at sp+0x30/0x2C and spills the incoming argument home at 0x3C; IDO leaves a 4-byte hole between them whatever the declaration order (w3-3, T-4030) |
| GYOZI | `func_8014494C` | regorder | u32 counter `D++ == 0` loaded into $a0 (`sltiu v1,a0,1; addiu a0,a0,1`) before two struct copies, IDO $v1/$v0; the first copy's address registers also differ (T-4070) |
| GYOZI | `func_80137188` | promo | `u8` selector from `D & 7` (`v ? v-1 : v`) kept in $v0 across a call and two reloads, IDO $v1 (T-4070) |
| SHOUGATU | `func_801407C0` | promo | after the table call the original reloads `D_800E738A` into $v1 while the first copy stays in $t1, IDO spills or reuses one register (T-4070) |
| BUNKASAI | `func_8013B190` | promo | switch selector `D_80122EB8` in $v1, IDO $v0 (the `D \|= 4` entry is a bit-field, matched; T-4070) |
| TT | `func_80134768` | regorder | temporaries after the two `func_800A0140/70` calls start at $t2 in the original, IDO $t4 (T-4070) |
| TT | `func_80133C1C` | regorder | constant 1 stored as `sb` and `sh` shares $v0 in the original, IDO loads it twice (T-4070) |
| OPTION | `func_8013357C` | regorder | dead `li v0,0x60` ahead of a 64-record loop in the original, IDO has none (T-4070) |
| OPTION | `func_801387E4` | regorder | unsigned byte global read into $v0 before `addiu sp` and returned after `-= 2`, IDO loads it into $a0 (T-4070) |
| 800737A0 | `get_weekly_bg_sector` | promo | `u8` global index kept in $v1 and the table value in $a0 across the `!= 4` test; IDO uses $t6/$v1 (T-4100) |
| GEKO/80139910 | `func_8013A5E8` | promo | `s32` global read, compared with 0, reloaded after a call and compared with 0x40 (`xori; sltiu`): original keeps it in $v1, IDO uses $v0 or spills it (T-4100) |
| GEKO/8013DF70 | `func_8013E56C` | promo | `D_800E7384++ == 1` followed by a `u16` compare: original `xori; sltiu; beqz`, IDO `xori; bnez` (`++ == 0` and `++ >= N` do match, see matching-notes) (T-4100) |
| DATE/801594B0 | `func_8015A114` | promo | same shape as GEKO `func_8013E56C` (T-4100, not built separately) |
| ETC/80132000 | `func_8013275C` | promo | two-case dispatch on `u8 D_800E738A`: selector in $v1 in the original, $v0 in IDO (`switch`, `if` chain, local copy, `s32` return all tried) (T-4100) |
| ETC/80132000 | `func_80132148` | promo | same shape as `func_8013275C` (T-4100, asm read) |
| ETC/80132000 | `func_80132204` | promo | same shape as `func_8013275C` (T-4100, asm read) |
| ETC/80132000 | `func_80132290` | promo | same shape as `func_8013275C` (T-4100, asm read) |
| ETC/80132000 | `func_80132670` | promo | same shape as `func_8013275C`, one call with an argument (T-4100, asm read) |
| ETC/80132000 | `func_801326FC` | promo | same shape as `func_8013275C` on `D_800E7389` (T-4100, asm read) |
| ETC/80132000 | `func_80132098` | promo | four-case compare chain on `D_800E7389`, selector in $v1 (T-4100, asm read) |
| ETC/80132000 | `func_801323D0` | promo | four-case compare chain on `D_800E7389`, selector in $v1 (T-4100, asm read) |
| GEKO/8013AF80 | `func_8013B030` | promo | `s32 D_80122CD0` compare chain (1, 7, default): original $v1, IDO $v0; `switch`, `if` chain, `u32` override and local copy give the same $v0 (T-4100) |
| GEKO/8013C980 | `func_8013CA30` | promo | same shape as GEKO `func_8013B030` (cases 1 and 8; T-4100, asm read) |
| GEKO/8013DF70 | `func_8013E040` | promo | same shape as GEKO `func_8013B030` (cases 1 and 2; T-4100, asm read) |
| 80075320 | `func_80075468` | promo | two-case dispatch on `u8 D_800E738A`, selector in $v1 (T-4100, asm read) |
| 80075320 | `func_80077BE0` | promo | three-case dispatch, selector in $v1 (T-4100, asm read) |
| 80075320 | `holiday_club` | promo | four-case dispatch, selector in $v1 plus a copy in $v0 (T-4100, asm read) |
| 80075320 | `func_80076EC0` | promo | four-case dispatch, selector in $v1 (T-4100, asm read) |
| 80075320 | `func_8007894C` | promo | four-case dispatch, selector in $v1 (T-4100, asm read) |
| 80075320 | `func_80075FA0` | promo | same as `get_weekly_bg_sector` (index in $v1, value in $a0) (T-4100, asm read) |
| TAIIKU/801452D0 | `func_80146B8C` | promo | `u32` global read once into $v1 for two range tests and a `u8` global in $v0; IDO swaps them and emits a second `lui $at` for the paired stores (T-4100) |
| 80062CD0 | `parameter_change` | regorder | `*(s32 *)(p + 0xFC) += arg1; if (< 0)`: the sum lives in the temp $t9 in the original, IDO keeps it in the variable register $v1 (pointer, local and struct forms tried) (T-4100) |
| KANGEI/80135F10 | `func_80137544` | reverse | `D_80120657 -= 8` before a counted loop and a test after it: IDO keeps the `u8` value in $a2 across the loop, the original reloads it (T-4100) |
| KANGEI/80135F10 | `func_80137484` | reverse | same as `func_80137544` with `+= 8` (T-4100) |
| KANGEI/80135F10 | `func_8013614C` | regorder | `u8` table value in $v1 and the character index in a temp in the original; IDO promotes the index to $v1 and puts the table value in $a1 (T-4100) |
| TACO/80149F90 | `func_8014EDCC` | regorder | constant 1 stored with `sb` and compared with the argument: the original compares against `li at,1`, IDO shares one register for both (T-3001 family) (T-4100) |
| TACO/80149F90 | `func_8014B2A4` | regorder | constant 1 kept in $v1 for the compares and the store in the original; IDO loads it per use (T-3001 family) (T-4100) |
| EVENT/801024B0 | `func_80102C0C` | regorder | one `li v0,8` shared by two byte stores in the original, IDO loads the constant per store (T-3001 family) (T-4100) |
| ENDING/80133C10 | `func_80134B18` | regorder | `u8` local kept in $v1 behind a temp (`andi t6; move v1,t6`), IDO uses $v0/$a0 (T-4100) |
| TACO/80149F90 | `func_8014BC80` | regorder | counted loop with argument copy and table pointer: original counter $s0, copy $s1, pointer $s2; all declaration orders give another assignment (T-4100) |
| TACO/80149F90 | `func_801511C0` | regorder | same family as `func_8014BC80` (T-4100) |
| DATE2/80137360 | `func_8013775C` | regorder | two `idx * 0x38` uses with `li 0x38; multu` in a register, IDO shifts (known gap, T-4100) |
| GYOZI/801372B0 | `func_80137BEC` | regorder | same as DATE2 `func_8013775C` (T-4100) |
| GYOZI/8013A140 | `func_8013A4E8` | regorder | same as DATE2 `func_8013775C` (T-4100) |
| TAIIKU/801452D0 | `func_80146A60` | promo | `s32` global `D_800E7200` tested with two masks (0xA, 0x5) in several blocks: original keeps it in $v0 and the 0xA mask in $v1, IDO swaps them (`u32` override too) (T-4100) |
| RPG_BAT/801504E0 | `func_80150CD8` | promo | `s32` state `D_8015EC74` switched (cases 0, 1, 2) and reloaded for `+= 1` in each case: original keeps it in $v1, IDO uses $v0 and loads the constant 1 into $v1 (T-4100) |
| BUNKASAI/801354B0 | `func_801354B0` | promo | `s32` selector `D_80122EB8` (cases 0, 1): original $v1, IDO $v0 (T-4100) |
| 80075320 | `func_80077E30` | promo | five-entry jump table on `u8 D_800E738D` (cases 0 to 3 + `case 4:` with default), selector in $v1 and reused for `+= 1` in case 1, reloaded into $v1 in case 2; IDO $v0 and $t0 (T-4100) |
| GEKO | `func_80135A6C` | regorder | post-increment compare `if (D_800E7384++ == 1)`: original materialises `(old ^ 1) < 1` (xori; sltiu) in $v0 before `beqz`, IDO uses `xori; bnez` (T-6020) |
| GEKO | `func_80137040` | regorder | `temp = f() & 0x7F` compare chain: original keeps the call result in a copy (`move v1,v0; andi t7,v1,0x7f`), IDO folds it; same for `func_80136F20` (T-6020) |
| TACO | `func_8013587C` | promo | switch on `u8` `D_800E738D` (cases 0, 1, 2) with `+= 1` in cases 0 and 1: original copies the selector (`move v0,v1`), IDO does not (T-6020) |
| BUNKA_SD | `func_801389DC` | promo | `u8` `D_800E738D` tested `== 0` / `== 1` and incremented in the first branch: original keeps the old value in $v0 and does `addiu v0; andi t6; move v0,t6; sb t6`, IDO `addiu t6; sb` (T-6020) |
| NAME_ENT | `func_80145378` | promo | switch on `u8` `D_800E738D` (cases 0 to 3, case 2 falls into 3): original compares the selector in $v1 and copies it to $v0 for the later cases (`move v0,v1`), IDO does not; rest of the body matches (T-6020) |
| 80061710 | `func_800626B0` | promo | `switch` on `s32` `D_801230D0` (cases 0, 1, default) after a call: original loads the selector into $v1, IDO into $v0 (T-6020) |
| SHOUGATU/8013A950 | `func_8013AE0C` | promo | `u8` field (`D_800E652A`, `>> 4`) kept in $v1 and shifted into $v0 for two compares (== other nibble, == 7); IDO uses $t6 temps for the shifted value, local or inline (T-6060) |
| SHUGAKU/80138A60 | `func_80138DD8` | regorder | three-test `&&` chain on `u8` globals with constant 1 shared in $v0: original third test is `bne t8,v0` (variable first), IDO emits `bne v0,t8`; no operand order or cast changes it (T-6060) |
| RPG_BAT/80144010 | `func_80144640` | promo | 8-case compare chain on `s32 D_8015EDC4 & 0xFFFF0000` returned in the default case: original loads D into $v0 and copies the masked value to $v0 in the first delay slot (default is a bare `jr`); IDO loads into $v1 and copies after the chain (T-6060) |
| SHUGAKU/80132000 | `func_80132BCC` | promo | `u8` field compared with 6 and passed as the call argument (twice, plus a second field): original loads it once into $a0 and passes $a0 unchanged; IDO loads into $a1/$v0 and moves/`andi`s it to $a0 (T-6060) |
| EVENT/8010FE10 | `func_80119440` | regorder | `if (u8 == 1) {...} else if (u8 == 2)` after another u8 test with constant 1 shared in $v1: original second test is `bne v0,v1` (variable first), IDO emits `bne v1,v0`; operand order, `1 ==`, and switch do not change it (T-6060) |
| GEKO/8013DF70 | `func_8013E1A0` | regorder | `u8` field RMW (`b[1] \|= 0x40`) in the else arm: original hoists the `lui t0` into the branch delay slot and loads into $t1 (t0 stays live), IDO reuses $t0 for the loaded byte (T-6060) |
| 80075320 | `func_80076B48` | promo | switch on `u8 D_800E738D` (cases 0-2) then inner switch on `s8 D_800E7314` inside case 2: outer matches as implicit-int, inner selector is a plain $v0 in the original but $v1 plus a copy to $v0 with IDO (T-6060) |
| SHOUGATU/80137BB0 | `func_80137DE4` | promo | `s16 D_80144E0C` (unit-private) tested against 3, 0xB, 5, 0xD in an if/else-if chain: original loads it into $v1, IDO uses $v0 (also needs the `func_80083440(s32)` override) (T-6060) |
| SHUGAKU/80132000 | `func_801322F0` | promo | `u8` field saved before a call (spill slot sp+0x28 with a dummy local above it) and compared with +3/+4 after: original keeps it in $v1 (loaded before and after the call), IDO uses $v0 and puts the final store in a different slot order (T-6060) |
| EVENT/8010FE10 | `func_80119370` | reverse | two `if (u8 D_800B0896 == ...)` blocks with early returns: original loads the `u8` global again in the second block (different temp), IDO shares the first load in $v1 across both (T-6060) |
| 80042540 | `func_80042960` | promo | jump-table switch on `u8` field (`(u32)` selector gives the right sltiu/table code, 48 case labels): original keeps the selector in $v1, IDO in $v0 (T-6060) |
| 80042540 | `func_80042808` | regorder | `u8 += 1` then stores of 0 to the neighbouring state bytes, `return 0` implicit: original emits `or v0,zero,zero` before the last `lui at; sb zero` and fills the delay slot with the sb, IDO puts the `move v0,zero` in the delay slot; same for `func_80042908`, `func_8004284C`, `func_80042940` (T-6060) |
| GEKO/8013DF70 | `func_8013E56C` | regorder | `if (D++ == 1)` on a `GsWord` field: original `xori v0,v1,1; sltiu v0,v0,1; beqz v0`, IDO `xori; bnez` (T-6060) |
| ETC/8013FF80 | `func_80140AE4` | regorder | every ugen temp after the first value taken from a call is one higher in the original (`t7` where IDO gives `t6`); same code otherwise; `(u32)D_800B3CFC == 1U` is needed for the unshared constant (T-6060) |
| EVENT/8010FE10 | `func_801146B4` | regorder | same one-higher temp numbering after `func_8002328C`: `andi t1,v0,0x7f` where IDO gives `t0` (T-6060) |
| TT/8013C6D0 | `func_8013C764` | regorder | `&= 0xFFFF` copies of both arguments: original `move a1,t7; move a0,t6`, IDO `move a0,t6; move a1,t7` (T-6060) |
| 80041000 | `func_8004111C` | regorder | RECT on the stack set x,y,w,h and passed to `func_8009C7F8`: original `sh h` before `sh w` (li t6=w, li t7=h), all 24 statement orders give w first; same in `func_80059308` (T-6060) |
| EVENT/800FD2F0 | `func_800FD588` | promo | `u32` `D_800B1AE4 % 60` tested twice after a call: original $v1, IDO $v0 (T-6030) |
| DATE/80154260 | `func_80154E48` | regorder | `u8` GameState field `unk_110A` read after the table copy loop and compared (0xF..0x11): original `lbu $t1`, IDO `$v0`; table copy and local-table call otherwise match (T-6030) |
| DATE/80154260 | `func_801551D0` | regorder | `u32` counter `D_800E7384` incremented in place then compared with 0x10 (returns 0 early): original loads it into $v1, IDO $v0 (T-6030) |
| SHUGAKU/80133D60 | `func_80134024` | regorder | `u32` counter `D_800E7384` post-incremented and tested `== 0x3C` after a call: original keeps the compare as `xori $v0; sltiu $v0,$v0,1; beqz $v0` in place, IDO writes the `sltiu` to $t6 (or folds it to a plain `bnez`) (T-6030) |
| RPG_BAT/801368B0 | `func_80136A9C` | promo | inner `switch (D_8015EBAC)` (cases 0, 1; `s32` and `u32` tried) after the outer `switch (D_8015EDB0)` case branch: original selector in $v1, IDO $v0 (T-6030) |
| EVENT/800F9680 | `func_800FA194` | promo | `u8` global `D_800F19AB` loaded once after a call into $a1 and kept across the loop (`sltiu at,a1` each pass), base in $a2, counter $a0; IDO loads into $a0, copies to $a3, counter $v1 (T-6010) |
| EVENT/800F9680 | `func_800FA0D0` | promo | same shape as `func_800FA194` (`D_800F19AB` kept in $a1 across two loops after a call, counter $a0); IDO $a0/$a3/$v1, frame also 8 bytes smaller (T-6010) |
| SHOUGATU/80137FF0 | `func_80138158` | promo | `if (D_800E7384++ == 1)` on the `lw` global (GameState `unk_1104`): original `lw v1; xori v0,v1,1; sltiu v0,v0,1; addiu v1,v1,1; sw; beqz v0`, IDO `xori; bnez` after the store (T-6010) |
| DATE/80149310 | `func_80149AC0` | regorder | `type = f() & 0x7F` then `<2`, `== 2`, `== 3` chain: original `move v1,v0; andi t6,v1,0x7f; ...; or v1,t6,zero` (switch-temp shape on a call result), IDO `andi v1,v0,0x7f` directly, one temp register fewer (T-6010) |
| BUNKASAI/8013B190 | `func_8013B190` | regorder | `lw` global `D_80122EB8` (s32, shared) switched (cases 2, 3) after two stores: original loads it first into $v1 (before the stores), IDO $v0 after them; `u32` override and a `u32` local copy do not change it (T-6010) |
| GYOZI/80142D00 | `func_80142D40` | regorder | `u8` global `D_800F647A` read as table index before and again after the indirect call: original loads the second read into $v1, IDO uses $t4 (frame/table layout matched with the T-3330 idiom; rest identical) (T-6010) |
| EVENT/800F9680 | `func_800FA6A0` | promo | local function table (47 entries, T-3330 idiom matches frame and copy): `u8` global `D_800B1AF6` read as index before and again after the indirect call; the second read is `lbu v1` in the original, `$t4` in IDO (same as GYOZI `func_80142D40`) (T-6010) |
| BUNKA_SD/80134260 | `func_80134314` | promo | state machine on `u8 D_800E738D` (cases 0, 1, else) returning `D += 1` / call results: original keeps the selector and every reload in $v0 (`return D += 1` as `andi t8; move v0,t8`, falls off the end after `func_80044E8C()`); IDO uses $v1 for the loads and $v0 only for the result (also with the old scalar name and with a local selector copy) (T-6010) |
| RPG_BAT/8013E4D0 | `func_8013F350` | regorder | `s32` local copy of global `D_8015ED90` returned at the end (assigned 0 / `D_8015E744` in branches, no call): original keeps it in $v0 from the first `lw`, IDO uses $v1 and ends with `move v0,v1` (array element and scalar declaration of the global both give $v1) (T-6010) |
| DATE/80132000 | `func_801378BC` | promo | `u8` field read twice (`unk_F5E` as index, then `- 1` stored back): original keeps it in a temp ($t6) and the record byte in $t8; IDO promotes the field to $v0 (struct field is not CVT-converted) (T-6050) |
| DATE/80132000 | `func_8013FA6C` | regorder | one `li v0,128` shared by seven byte stores to different symbols in the original, IDO loads it per store (T-3001 family) (T-6050) |
| GYOZI/801418B0 | `func_8014210C` | regorder | sixteen `sh` constants of four 4-halfword arrays: original shares one register per constant value (-0x40, 0x40), IDO loads each (T-3001 family) (T-6050) |
| DATE/80156600 | `func_8015745C` | regorder | same code, but as1 delays the `sb zero` of the third `GameState` byte past the loop setup and does not hoist the later `lbu D_80120697` above the store to `D_80120653`; the three bytes are separate symbols in the original, fields of `D_800E6280` here (permuter 300) (T-6050) |
| DATE/80132000 | `func_8013DCC0` | regorder | nine `sh` constants stored through one register per value (3, 4, 2) in the original, IDO loads each (T-3001 family) (T-6050) |
| DATE/80132000 | `func_8013D2C8` | promo | `u8` `D_800CA2FC` loaded after a branch, kept in $v0 for three compares (`bne v0,v1` with the constant 1 in $v1); IDO emits the compare as `bne v1,v0` (only diff, plain C everywhere else) (T-6050) |
| GYOZI/8013B920 | `func_8013BB0C` | regorder | `D_800F62CF = 3; func_8008F618(3);` shares one `li a0,3` in the original (T-3001 family); the three `lw/sw` copies of separate symbols also schedule differently (T-6050) |
| DATE/80132000 | `func_801386C8` | promo | same as `func_801378BC`: `u8` `unk_F5E` used as record index and decremented; original keeps it in a temp ($t6), IDO promotes the field to $v0 (T-6050) |
| DATE/80132000 | `func_8013BDB4` | promo | `u8` `D_800E71DF` loaded after a branch and compared with 2, 3, 8, 5 in $v0 (variable first, `beq v0,at`); IDO shares `li v1,2` with the earlier `s32` test and swaps the compare (T-6050) |
| NAME_ENT/80132000 | `func_80134110` | promo | range tests on two `s16` globals, implicit-int return: original keeps `D_8011ECF6` in $v1 with `move v0,v1` in the first delay slot and loads `D_8011ECFA` into $v0; IDO uses $v0 then $v1 (local, if/else and return forms, permuter 110) (T-6050) |
| ETC/80146400 | `func_80146AC0` | regorder | twelve zero stores into `D_801230D0[4..15]`; the original builds the pointer `&D_801230D0[2]` with `li v1,2; sll; addu` (a run-time index 2) and stores through it, IDO folds the address into `lui at; sw` per store (T-6050) |
| DATE/80132000 | `func_8013D23C` | promo | `u8` `D_800CA2FC` compared with 0 and 1 after a branch: original `bne v0,v1` (variable first), IDO `bne v1,v0` (same as `func_8013D2C8`) (T-6050) |
| KANGEI/80134120 | `func_80135438` | promo | two `|=` through `D_800E6280.unk_1BC[unk_F5F]`; original hoists `lui` of the index byte and uses other temps, IDO reloads with a different `lui` placement (T-6050) |
| DATE/80132000 | `func_80140D38` | promo | `u8` `D_8015B6A0` read after a branch, `D = (D + 1) & 0xFF; if (D >= 4) D = 0;` kept in $v0 and passed as the fifth argument from the same register; IDO uses $v1/$t9 (T-6050) |
| EVENT/8010A210 | `func_8010A790` | promo | EVENT-private `u8` `D_800B0E72` clamped to 1 and incremented (`lui v0; lbu v0,0(v0)` then the value kept in $v0); IDO emits `lui v1; lbu v0,0(v1)` (direct form), `lbu v1` (local copy form) (T-6050) |
| DATE/80132000 | `func_801395B4` | regorder | plain C compare chain on `D_800E71DF` (field `unk_F5F`): the only diff is where as1 places `lui v0` for the field load (before `li t7,9` in the original, after `lui at` here); separate symbols in the original (T-6050) |
| ETC/80140F50 | `func_801428EC` | promo | `u8` `unk_03A` multiplied by 0x10101 inside the sixth argument: original reads it into $v0 and reuses $v0 for the shift/add chain, IDO uses temps ($t7), also with a local copy (T-6050) |
| TT/80133970 | `func_80133AD0` | regorder | `(a & 0x1F) | ((a & 0x100) >> 3)` of a masked argument then a byte test and store through the cached base: original temps t6,t0,t1,t9, IDO t8,t1,t2,t0 (permuter 30) (T-6050) |
| NAME_ENT/80132000 | `func_801341A8` | regorder | old `D_8014CCB4` loaded before the first test, new value stored in the branch delay slot; original keeps the constant -1 in $v1 and the two coordinates in $a0/$a1, IDO $a1 and $v1/$a0 (permuter 85) (T-6050) |
| NAME_ENT/80132000 | `func_80133FC4` | promo | loop `for (i = 1; i < 5; i++)` over `D_8011ECD0` records (stride 0x44) with `D_8014CCC8 == i`: original keeps `i` in $v0, the global in $v1 and one strength-reduced pointer; IDO unrolls with folded constants and reloads the global (T-6050) |
| TT/80132130 | `func_80132B88` | regorder | two clearing loops over `D_80158AA8` (stride 0x14) and `D_80158AA4` (stride 0x64, unrolled by 4) and a call with 0x80: original loads `li a0,128` once at the top and uses $a0 as loop bound and call argument, IDO loads it twice and swaps $v0/$v1 (permuter 460) (T-6050) |
| TT/80132130 | `func_80132C24` | regorder | same loops as `func_80132B88` with the constant 8 kept in $a0 (arguments passed through to `func_80132810`) (T-6050, not tried separately) |
| DATE/80132000 | `func_8013E6A4` | promo | `D++ == 1` in a void function: original loads the old value into $a0 and the xor result in $v1, IDO $v1/$v0 (after `func_80085A60`; permuter 30) (T-6050) |
| OMIMAI/801331B0 | `func_801337EC` | regorder | `unk_69C[unk_71C].unk_01` byte (separate symbols in the original) loaded through `lui t8; addu; lbu a0`, IDO folds into `lui a0` and reorders the following loads (T-6050) |
| GYOZI/80134C10 | `func_80134D9C` | regorder | `girl[idx]` (stride 0x38) three times plus `func_8008FB00(0x38)`: original multiplies with `li a0,56; multu` (the constant doubles as the call argument), IDO uses `sll/subu/sll` (same family as DATE2 `func_8013775C`) (T-6050) |
| DATE/80132000 | `func_8013F544` | regorder | plain C, only diff: the constant 0x38 is held in $a0 and shared by the three `multu` and the final call `func_80084D3C(0x38)` in the original; IDO keeps it in $a1 and loads $a0 again (T-6050) |
| SHUGAKU/801378B0 | `func_801385A8` | regorder | `s16` global decremented by 3 twice with a clamp test before: original reloads the global after the calls and truncates with `sll/sra`, IDO keeps temps in other registers and spills one (permuter 850) (T-6050) |
| BUNKA_SD/80132000 | `func_8013261C` | regorder | two `x == 0x14` tests on different globals and a counter: original keeps the constant 0x14 in $v0 (compare `bne v0,t0`) and uses ever-growing temps, IDO puts it in $a1 and reuses $v0 for the counter (T-6050) |
| NAME_ENT/80132000 | `func_80133D94` | regorder | range tests on two `s16` globals then a `-1` default in a leaf: original keeps the local in $a0 and the `s16` in $v0; IDO uses $v0 for the local and $a0 for the `s16` (permuter 580) (T-6050) |
| GYOZI/8013B920 | `func_8013BC40` | regorder | `rec[idx].flags |= 4` through the `idx * 0x38` shift sequence: the original skips $t9 (loaded byte in $t0, `ori t1`, constant $t2), IDO uses $t9/$t0/$t1 (permuter 40 only with a `long long` temp) (T-6050) |
| GYOZI/8013B920 | `func_8013BCB4` | regorder | same as `func_8013BC40` (T-6050) |
| GYOZI/8013C3B0 | `func_8013CAA4` | regorder | same as `func_8013BC40` (T-6050) |
| SHOUGATU/801397D0 | `func_80139E54` | regorder | same as GYOZI `func_8013BC40` (T-6050) |
| DATE/80132000 | `func_80139DD0` | regorder | switch on `D_800E71DF` (cases 4, 9, 5/6, default) compiled as a compare chain: original uses `li at,k; beq v1,at` and `bne v1,v0` against the loaded `D_800CA22C`, IDO keeps constants in $a0 and swaps the operands (T-6050) |
| DATE/80132000 | `func_8013E3C4` | promo | `D_800E71DF == 0xA` and other `u8` tests after branches: original compares variable-first (`bne v0,v1`) and loads `li at,1` per use, IDO emits `bne v1,v0` and shares `li v0,1`; the `s32` local sits at sp+0x28 in the original, sp+0x2C here (T-6050) |
| BUNKA_SD/80132000 | `func_80132708` | regorder | five-way jump table with `idx * 2` subtracted in each case: original computes `sll a0,v1,1` once before loading the selector, IDO sinks it into the cases and hoists `li t1,25` (T-6050) |
| RPG_BAT/801447E0 | `func_80144B7C` | promo | `s32` selector `D_8015EDC4 & 0xFFFF0000` in a sparse switch whose default returns the masked value: original keeps the global in $v0 with the masked copy moved to $v0, one shared `b epilogue` exit; IDO uses $v1 and duplicates the exits (also with a `u32` cast and a local copy) (T-6050) |
| ETC/80146400 | `func_80146400` | regorder | table pick with a three-way switch on `unk_03E` (0x5F, 0x60, 0x61 + default 6), printf of `*p`, then `rand() & 7` test against `*p`: original keeps only the pointer in a stack slot (frame 0x40) and loads `*p` into $a0 after the call; IDO spills more locals (frame 0x38) (permuter 700) (T-6050) |
| 80049FF0 | `func_80049FF0` | regorder | seven-argument packet builder: the original reads the `u8` stack argument with `lbu` at each use (frame 0x30), IDO keeps it in $a3 and spills it (frame 0x38); a K&R definition does not change it (T-6050) |
| RPG_BAT/80135AE0 | `func_80135D90` | promo | `s32` counter `D_8015EE0C` stored, passed through a call and compared again: original reloads it into $v0, IDO into $t7 (permuter 200) (T-6050) |
| DATE/80132000 | `func_8013A344` | regorder | if/else chain on `D_80122E9C` (0, 1, 2, 3, 3) calling `func_80083440` with 1, 2, 3, 0, 1, 3: original loads `li at,k` per compare, IDO shares `li v1,2` (T-3001 family) (T-6050) |
| RPG_BAT/8013F4F0 | `func_8013F5C8` | regorder | switch with a string copy and two constants `0x05000029`/`0x05000000`: the original shares one `lui a1,0x500` (loaded in a delay slot) between the two cases, IDO loads it again in the third delay slot; everything else matches with an implicit-int return (T-6050) |
| RPG_BAT/8013F4F0 | `func_8013F694` | regorder | same as `func_8013F5C8` (T-6050) |
| RPG_BAT/8013F4F0 | `func_8013F760` | regorder | same as `func_8013F5C8` (T-6050) |
| TACO/801447D0 | `func_8014488C` | regorder | `D_800E62B6 = 8; D_800E62B8 = 0x10; D_800E62B7 = 8;` and the `-0x5A`/`0x5A` halfword stores: original shares one `li` per equal constant, IDO loads each (T-3001 family) (T-6050) |
| TT/80133970 | `func_80133B70` | regorder | `for (i = 0; i < 0x80; i++) p[i * 0x68] = 0;` unrolled by 4 with the same instructions as the original; register assignment differs: original pointer $v1, counter $v0, bound $a0; IDO $a0/$v1/$v0 (declaration order, `u32` counter, permuter 65) (T-6050) |
| EVENT/8010A210 | `func_8010A6E4` | regorder | two 3-bit fields (bits 9-11 of a word, bits 1-3 of the byte at `D_800B0860 + 0x3ED`): original builds the base with `lui; addiu v1,v1,lo(D_800B0860)` and `lbu 0x3ED(v1)`, IDO folds the offset into the symbol (`D_800B0C4D`) (T-6050) |
| GEKO/801323E0 | `func_80132A4C` | regorder | local copy of an 11-halfword table, `D_80144C50 = (u8)arg0` in three branches: original computes `andi v0,a0,0xff` into a register before each `sb` and keeps the table at sp+0x14 (frame 0x30); IDO stores the argument directly (T-6050) |
| KANGEI/80134120 | `func_80135004` | promo | `D_800E7384 == 1 && (F5F == 1 || F5F == 3)` after a call: original keeps the constant 1 in $v0 (`bne v0,t7`) and compares `beq v0,v1`; IDO swaps the operands and the RECT local sits 4 bytes higher (T-6050) |
| RPG_BAT/80148F70 | `func_801495DC` | regorder | stack slots match with `sp30, var_v1, sp2C, temp_v0, var_v1_2` declared in that order, but the three stores `D_8015EBB0 = v` (after each test) are merged by IDO; the original keeps all three (T-6070) |
| GEKO/80140A00 | `func_80140EBC` | regorder | `if (GameState.unk_1104.u++ == 1)` as a value: original `xori v0; sltiu v0,v0,1` in the same $v0 and `beqz v0`, IDO `sltiu t7,v0,1`; `!(x++ ^ 1)` gets the shape but not the register (T-6070) |
| DATE/8014CC40 | `func_8014E010` | regorder | `s16` sum `D_8015E310 + 1` kept in a register: original sign-extends in place ($v0, `sll t9,v0; sra v0,t9`), IDO puts the extended value in a new register (T-6070) |
| DATE/8014CC40 | `func_8014DB98` | regorder | same `s16` wrap-around counter as `func_8014E010`: only $v0/$v1 and the place of the `lui` differ with `s32 v = (s16)(D + 1)` (T-6070) |
| EVENT/80107FC0 | `func_80108A24` | regorder | two `s16` sums (`D_800EB044 += 0x80`, `D_800EB042 += 0x80`) with a compare on the second: the extension goes to a new register instead of $v0 (T-6070) |
| OPTION/80132000 | `func_80132AB8` | regorder | `u8` GameState field (`D_800E62BA`) read as a scalar in the original (`lui v0; lbu v0,0(v0)`), as a member by IDO (`lui a1; lbu a1,58(a1)`): the extra base register changes every temp (T-6070) |
| OPTION/80132000 | `func_801387E4` | regorder | same GameState `u8` field `D_800E62BA`: `lbu a0,58(a0)` instead of `lbu v0,0(v0)`, and the new value is the return value (`move v0,t6`) (T-6070) |
| OPTION/8013A550 | `func_8013C780` | regorder | GameState `u8` selector `D_800E7389` switched and read again after the switch (`$v1` against `$a0`, one more spill) (T-6070) |
| TACO/80136B40 | `func_80136C60` | regorder | GameState `u8` counter `D_800E738D` read twice and incremented: `li v1,3; multu v0,v1` in the original, `a1`/`t6` in IDO (T-6070) |
| ENDING/80133C10 | `func_80134B18` | regorder | GameState `u8` triple `D_800E62B6..B8`: the first read is `lbu v1,0(v1)` in the original, `lbu v0,54(v0)` in IDO, then every temp shifts (T-6070) |
| DATE/8014CC40 | `func_8014E38C` | regorder | GameState word `unk_1104` incremented and compared (`lw v1`), IDO `lw v0,4356(v0)` (T-6070) |
| 80085E30 | `func_80086640` | promo | swap of two s16 globals (D_80120666, D_801206EE): original loads them into $v1 and $v0 and stores crosswise, IDO uses $t6 and $v0 (T-6040) |
| BUNKA_SD/80135160 | `func_80135440` | promo | `s32` field unk_1104 (D_800E7384) tested ==0 then +1, reloaded: original keeps it in $v0, IDO $v1 (T-6040) |
| 8007C030 | `normal_date_bg_fadein` | reverse | u8 global +4 clamped at 0x80: original adds in place in $v0 and keeps the masked copy in $t7 (move v0,t7), IDO promotes the global to $v1 (T-6040) |
| 8007C030 | `k_disp_inc2` | promo | `s32` counter D_80122CFC decremented, tested `< 1` (slti) and cleared: original keeps it in $v1, IDO $v0 and fuses the branch (bgtz) (T-6040) |
| BUNKAKEN/8013BFE0 | `func_8013BFE0` | promo | `s32` selector D_80122EB8 (cases 0, 1): original $v1, IDO $v0 (u32 declaration does not change it) (T-6040) |
| EN_NICHI/80132000 | `func_80132ADC` | promo | `s32` counter D_80139B24 incremented, wrapped at 0x36, tested twice: original $v0 with $t6/$t7 constants, IDO $v1 (T-6040) |
| GYOZI/8013AE80 | `func_8013B644` | reverse | read-modify-write of one byte of a GyoziWork girl record (stride 56): original takes $t0/$t1/$t2 after the index chain ($t9 never used), IDO $t9/$t0/$t1 (T-6040) |
| GYOZI/8013AE80 | `func_8013B6CC` | reverse | same shape as func_8013B644 (girl record byte ored with 4): original $t0/$t1/$t2, IDO $t9/$t0/$t1 (T-6040) |
| GYOZI/8013AE80 | `func_8013B740` | reverse | same shape as func_8013B644 (girl record byte ored with 4): original $t0/$t1/$t2, IDO $t9/$t0/$t1 (T-6040) |
| 80079B10 | `func_8007B5EC` | reverse | `u16` global D_80125CC0 tested, incremented and used as index: original reloads it after the check and keeps the `andi` copy of the argument in $t6 then $a0, IDO promotes the global to $v1 (T-6040) |
| 8005A0B0 | `func_8005B040` | reverse | table lookup `D_800B5970[D_800E62C2 & 7]` (8-byte rows): original takes $t0/$t1/$t2 for the index chain ($t9 skipped), IDO $t9/$t0/$t1 (T-6040) |
| 8005A0B0 | `func_8005B2BC` | reverse | table lookup `D_800B37D4[D_8011F414]` at the end of a long `&&` chain: original $t8 base and $t7 index, IDO $t7 and $t6 (T-6040) |
| 8005A0B0 | `func_8005B1A8` | reverse | table lookup `D_800B3B9C[idx][flag]` (16-byte rows) after a long `&&` chain: original frame 0x38 with $t8/$t0/$t2, IDO frame 0x30 and one register less (T-6040) |
| NAME_ENT/80139740 | `func_8013D6C0` | regorder | switch on `u8` `D_800E738D` (cases 0 to 2, compares inside the case bodies): original copies the selector with `or v0,v1,zero` in the delay slot of the first `beqz`, IDO does not, also with a `u8` local selector; same shape in OLH `func_80135DD4`, RPG_BAT `func_8013F4F0` and `func_80142000` (T-8050) |
| KANGEI/801355F0 | `func_80135678` | promo | table dispatch, then `D_800E71DF` compared twice and `D_800E7380++ % 300`: original keeps the byte global in $a0 and the counter in $v1, IDO uses $t8/$v0 (frame and table match with the scalar declared first) (T-8050) |
| GYOZI/8013E5B0 | `func_8013E724` | regorder | byte array store of `(x & 1) != 0`: original `sltiu t3,t2,1; xori t3,t3,1` in place, IDO emits `sltu` or a fresh temp for the `xori`, so every later temp shifts by one (T-8050) |
| TT/80146BE0 | `func_80147074` | regorder | 8-trip loop around a two-case switch: the constants 8 and 0x20 are kept in $s3/$s4 in the original, $s4/$s3 here (permuter: nothing better in 150 s) (T-8050) |
| NAME_ENT/80139740 | `func_80140D6C` | regorder | byte search loop up to the end symbol `D_800E7D1F`: the original loads the end address into $a0 for the loop and again into $t9 for the test after it, IDO reuses $a0 (permuter best 325) (T-8050) |
| NAME_ENT/80144AD0 | `func_801454CC` | regorder | 8-record loop: the last `sb` (record byte +0x1987) sits in the `bne` delay slot with its offset adjusted to +0x1943, IDO keeps it before the branch (permuter best 125) (T-8050) |
| KANGEI/80137B90 | `func_80138D40` | regorder | two `&= 0x7F` read-modify-writes in a row: the original loads the second byte after the first store, IDO hoists the load (permuter best 225) (T-8050) |
| RPG_BAT/801321D0 | `func_80132B34` | regorder | five-case table switch that stores constants: the original builds `0x84 + 0x30` and `0x6C - 0xB0` from the registers that hold 0x84 and 0x6C, IDO folds them into separate `li` (T-8050) |
| RPG_BAT | `func_8013F350` | regorder | flag/state variable loaded from D_8015ED90 and returned: original keeps it in $v0 throughout (return register), IDO $v1 plus a final `move v0,v1` (T-8010) |
| TT | `func_8013923C` | regorder | pointer copy `or v0,t6` plus u32 and u16 temporaries: original v0/v1/a0 for pointer/clamp/counter, IDO v1/a1/v0 (T-8010) |
| 80079B10 | `func_8007B2B4` | regorder | switch selector `arg0 & 0xFF` of a u16 argument: original $v0 (the flag word load reuses $v0 after the `ori`), IDO $v1 (T-8010) |
| 80079B10 | `func_8007A354` | regorder | jump-table switch on `arg0 & 0xFF`: code identical but temporaries start at $t8 in the original, $t6 in IDO (T-8010) |
| 80079B10 | `func_8007B144` | regorder | three-case switch with Work80125D10 flag RMWs: temporaries start at $t7 instead of $t6, D_80125D14 load not hoisted (T-8010) |
| 80079B10 | `func_8007BCFC` | regorder | flag word kept in $v0 and the 0x18/0 variable in $v1 around a stack byte array, IDO uses $t regs and $a1 (T-8010) |
| 80079B10 | `func_80079B34` | regorder | dispatcher loop calling f(r & 0xFFFF): original keeps the argument in $a0 across the compare chain, IDO $a1 plus `move a0,a1` (T-8010) |
| 80079B10 | `func_8007B734` | regorder | RMW loads of D_80125D10/D_80125D14 hoisted above the zero stores in the original, in source order in IDO (T-8010) |
| 80079B10 | `func_8007A868` | regorder | eight loads hoisted over twelve stores (D_80125D10 area): original v0/v1/a0 loaded first (T-8010) |
| TAIIKU | `func_8013CF88` | regorder | two clamp variables in $v0 (in-place add of D_80149974) and $v1; IDO $a0/$v1 and shifted constants a1/a2/a3 (T-8010) |
| TAIIKU | `func_8013C694` | regorder | position clamp: original $a0 = scaled D_80149984, $v1 = D_801206BA variable, IDO swaps them (T-8010) |
| EVENT | `func_80108A24` | regorder | s16 pair += 0x80 with compare of the truncated value: original keeps the s16 variable in $v0 (in-place add, sll/sra into $v0), IDO t-regs (T-8010) |
| ETC | `func_8014BF2C` | regorder | switch in an s32 function; call result compared in $v0 with a copy in $a1 for the argument, IDO compares the copy (T-8010) |
| TT | `func_80141A1C` | regorder | packet link (P_TAG bit-field insert) builds exactly; pointer to the double-buffer record lands in $t0 in IDO, $t1 in the original (T-8010) |
| TT | `func_8013D2E0` | regorder | packet link (P_TAG bit-field insert) builds exactly; locals v0/v1/a1 in the original (a0 unused), IDO v0/v1/a0 shifted (T-8010) |
| GEKO | `func_80140040` | regorder | two 10-trip loops over D_8011ECD0 records: hoisted constants and base one register higher in IDO (base a1 vs a0) (T-8010) |
| TACO | `func_8013546C` | promo | flags word D_800E7208 kept in $v1 and re-read after the call; s8 cursor loaded into $a0 in IDO, $v0 in the original, frame 8 bytes larger (T-8010) |
| VALEN | `func_80133670` | regorder | GameState member D_800E71DF read as a scalar: original `lui v0; lbu a0,(v0)`, IDO `lui a0; lbu a0,(a0)` (T-8010) |
| 80042540 | `func_80042960` | regorder | jump-table switch on GameState unk_1108 (u8): selector in $v1 with `or v0,v1,zero` in the original, $v0 in IDO (member of GameState, cvt pass does not apply) (T-8010) |
| BUNKASAI | `func_80146030` | regorder | nested switches; selector D_80122EB8 loaded early into $v1, D_800E64F0 is a separate symbol in the original (T-8010) |
| EVENT/800FD2F0 | `func_800FD588` | promo | `D_800B1AE4 % 60` evaluated twice around a call: original loads the global into $v1 (divu, mfhi $v0), IDO $v0; plain, local-copy and s32-override forms all give $v0 (T-8060) |
| SHUGAKU/80132000 | `func_80132BCC` | promo | u8 global tested and passed as first argument to three callees: original keeps it in $a0, IDO loads it into $a1 and moves (T-8060) |
| EN_NICHI/80132000 | `func_80132ADC` | promo | `D_80139B24 + 1` wrapped at 0x36: original loads and increments in $v0, IDO loads $v1 and adds into $v0; branch layout also differs (T-8060) |
| SHUGAKU/80132000 | `func_801322F0` | promo | u8 game-state byte saved to a local before a call and re-read after it: original keeps it in $v1 both times (and fills the final `sb` delay slot), IDO $v0 (T-8060) |
| GEKO/80134D10 | `func_801374A8` | promo | record index times 0x38 (`multu`) with the constant also the later call argument: original keeps `li a0,56` in $a0 before the first multiply, IDO uses $a1 and reloads $a0 (T-8060) |
| SHOUGATU/80134120 | `func_80134730` | promo | same shape as GEKO `func_801374A8`: original `multu $a1,$a0` with $a0=0x38 shared with the call argument, IDO shifts and subtracts (T-8060) |
| TT/8013A040 | `func_8013A040` | promo | constant 1 stored to two byte fields: original keeps `li a1,1` in $a1 across the function, IDO re-loads it in a temporary (T-8060) |
| SHOUGATU/8013A950 | `func_8013AB5C` | promo | `D_800E6280.unk_F5F = 4; func_800847B8(4);`: original loads the constant into $a0 once and stores it from $a0 in the call delay slot, IDO uses a temporary for the store and a second `li a0` (T-8060) |
| GYOZI/8013D470 | `func_8013D764` | promo | `func_8008F618(D_800F62CF = 4)` with later reads of D_800F62CF: original loads 4 into $a0 once (store in the call delay slot), IDO loads a temporary for the store and $a0 again; the same line matches in GYOZI `func_80138A10` with 0xC (T-8060) |
| 800420D0 | `func_800422C8` | promo | return value of func_80098370 kept in a local used in both arms: original copies it to $a3 (`move a3,v0`), IDO picks $v1; rest of the arithmetic identical (T-8060) |
| DATE2/80132DE0 | `func_80136794` | reverse | D_800E71DF read at entry and written later: original keeps `lui a1` of the symbol for both (and loads D_800E69DD through `lui a1` too), the GameState field form uses a fresh base each time and shifts the t6/t7 order (T-8060) |
| SHUGAKU/801378B0 | `func_801385A8` | promo | s16 global D_80120676 tested, reloaded after three calls, decremented twice and returned: original keeps it in $v0 throughout, IDO $v1 (plain C `if (D < X) {...} D -= 3; return D;` is otherwise identical) (T-8060) |
| GEKO/80139910 | `func_8013A2EC` | promo | local function-pointer table (0xDC bytes) called with 0x80, then `D_800E738A` re-read (u8, index of the same table) and compared twice: original keeps the re-read in $v1, IDO $t4; frame, table copy and compares match (T-8060) |
| BUNKA_SD/80132000 | `func_80132000` | promo | else-if chain on the u8 state `D_800E738D` with `D = (D + 1) & 0xFF` in each arm: original reloads it after each call and keeps `andi t8,v0,0xff; move v0,t8; sb t8` (value left in $v0, two epilogue labels), IDO drops the mask and the move (T-8060) |
| RPG_BAT/801421E0 | `func_801426B0` | promo | counter global D_8015EDB4 kept in $a3 across the calls (`addiu a3,a3,1` after the first reload), IDO has no register for it; same for sibling `func_80142DF4` (T-8060) |
| RPG_BAT/801421E0 | `func_80142544` | promo | counter global D_8015EDB4 read once into a local and used for compares, a division and the +1 store: original keeps it in $v1 and the quotient in $v0 (`mflo v0` straight into the four `sh`), IDO promotes the counter to $a3 and the quotient to $t6 (T-8060) |
| TT/8013BD10 | `func_8013BF9C` | promo | pointer parameter used after two calls: original copies it to $a2 at entry (`move a2,a0`, spilled from $a2) and reads it through $a2; IDO keeps $a0 and reloads into $t6; struct copy through $at otherwise identical (T-8060) |
| TAIIKU/801452D0 | `func_801485C4` | promo | loop `if (D_8014A3D8[i] == 1) n = i;` then `D_8011ECD0` word update: original keeps the record base in $v1 (`lui; addiu v1; lw/sw 0x1BC4(v1)`) and the branch bodies in source order, IDO addresses with `lui t0`/`lui at` and hoists the else-branch constants (T-8060) |
| RPG_BAT | `func_80144640` | promo | switch on `D_8015EDC4 & 0xFFFF0000` (8 cases, compare chain): original selector in $v0, IDO $v1 with s32/u32 local (T-8070) |
| ETC | `func_801428EC` | promo | u8 GameState field * 0x10101 + const as call arg: original loads it into $v0, IDO $t7 (permuter best 80) (T-8070) |
| DATE | `func_801551D0` | promo | `if (++D < 0x10) return 0;` on GameState word: original keeps the incremented value in $v1, IDO $v0 (permuter best 35) (T-8070) |
| DATE | `func_8013E6A4` | promo | `if (D_800E7384++ == 1)` after two calls: original loads the old value into $a0 (xori v1,a0,1; addiu a0,a0,1), IDO $v1/$v0 (T-8070) |
| DATE | `func_8013E3C4` | promo | `D_800E71DF == 0xA && ...` twice: original selector in $v1 and 0xA in $v0 (`bne v0,v1`), IDO swaps the two (local u8/u32/s32 copies tried) (T-8070) |
| GEKO | `func_8013E56C` | promo | `if (D_800E7384++ == 1)` (xori; sltiu v0,v0,1; addiu v1 after): original old value in $v1 and flag reuses $v0, IDO $v0 / $t6 (T-8070) |
| GYOZI | `func_80137188` | promo | `v = D & 7; if (v) v = (u8)(v-1);` kept in $v0 with the global load in $t6; IDO loads straight into $v0 and keeps v in $v1 (T-8070) |
| DATE | `func_8013A344` | promo | if/else chain on s32 D_80122E9C (0,1,2,3): original `bne v1,v0` (const 3 in v1 hoisted), IDO `bne v0,v1`; local copies/swapped operands/switch tried (T-8070) |
| SHOUGATU | `func_80137DE4` | promo | `D_80144E0C == 3 || == 0xB`, `== 5 || == 0xD` (s16): original selector in $v1, IDO $v0 (local s16/s32/u16, switch tried) (T-8070) |
| KANGEI | `func_80137484` | promo | `D_80120657 += 8` before a 19-trip loop, compared again after it: original reloads the u8 global after the loop (lbu t8), IDO keeps it in $a2 across the loop (T-8070) |
| SHOUGATU | `func_80138E60` | reverse | `Rec38[F5F].unk_0C.b[0] or-assign 4` (stride 0x38 index chain t6/t7/t8): original loads the byte into $t0 (skips $t9), IDO $t9 (T-8070) |
| SHOUGATU | `func_80138ED4` | reverse | same shape as `func_80138E60`: byte load in $t0 expected, $t9 built (T-8070) |
| KANGEI | `func_80136BF4` | promo | `t = D_80139DF4[(u8)D_80139DF0]; if (t == 2) .. else if (t == 1) .. else switch`: original compares `bne v1,v0` / `bne a0,v0` (constants first, kept in registers), IDO `bne v0,v1` (u8/s32 local, swapped operands tried) (T-8070) |
| 8007C030 | `normal_date_bg_fadein` | promo | `D = min(D + 4, 0x80)` on a u8 global then `if (D >= 0x7D)`: original keeps the clamped value in $v0 (copy from $t7), IDO uses $v1 (u8/s32/u32 locals, permuter best 325) (T-8070) |
| 8007C030 | `normal_date_bg_fadeout` | promo | same shape as `normal_date_bg_fadein` (`D - 4`, clamp to 0): $v0 expected, $v1 built (T-8070) |
| GYOZI | `func_80140780` | promo | local function table call then `D_8012B8D6 + 1` s16 increment returned through the call's $v0: original keeps the increment in $v0 (sll/sra in place), IDO $v1 / extra frame slot with a result variable (T-8070) |
| RPG_BAT | `func_80150CD8` | promo | 3-case switch on s32 D_8015EC74 plus `D_801213B0 == 1` (s16) sharing the constant 1: original selector $v1 / const $v0, IDO swaps them (u32 selector gives $v1 but then the constant is not shared; cast tried) (T-8070) |
| TT | `func_8014BB30` | reverse | `(v + 0x100000) * (arg0[0x50] - 2)` after two func_800A0070 branches: original temps t0 (v+C), t1 (byte), t2 (byte-2); IDO t2/t0/t1 (operand order, temp variables, separate statements tried) (T-8070) |
| DATE | `func_80139DD0` | promo | 4-way `switch` on `unk_F5F` with `s16` D_800CA22C compared against 2/3/4 in each case: body compiles (only operand order differs), original `bne v1,v0` with the constant 3 in $v1, IDO `bne v0,v1` (local copy of D_800CA22C per case made it worse) (T-8070) |
| ETC | `func_80141468` | promo | implicit-int 4-case switch; case 3 `D_800E62BA++ < 0x80` with the new value passed to a call: everything matches except the u8 global is loaded and incremented in place in $a0 (`lbu a0,0(a0)`; sltiu v0; addiu a0,a0,1), IDO uses $v0/$v1 plus an `a0` copy (T-8070) |
| SHUGAKU/80133D60 | `func_80134024` | reverse | post-increment compare `if (D_800E6280.unk_1104.w++ == 0x3C)`: IDO bnez, original materialises xori/sltiu then beqz (T-8040) |
| 8004F870 | `func_8005352C` | reverse | `(D_800E6374 >> 12)` range check 6..10: original keeps u16 load in v0 and shift in v1, IDO shift in t6 (T-8040) |
| EVENT/8010A210 | `func_8010A790` | promo | `u8 v = D_800B0E72` clamp then `D += v; D_800B0E72 = v + 1`: original `lui v0; lbu v0,0(v0)`, IDO lui v1 (T-8040) |
| GYOZI/8013B920 | `func_8013BC40` | reverse | girl record byte ored with 4 (`D_800F53A0.girl[D_800F62CF].unk_10[0] |= 4`): original $t0/$t1/$t2, IDO $t9/$t0/$t1; same family as func_8013B740 (T-8040, func_8013BCB4 identical) |
| SHOUGATU/801397D0 | `func_80139E54` | reverse | record byte ored with 4 (`D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.b[0] |= 4`): original $t0/$t1/$t2 (skips $t9), IDO $t9/$t0/$t1 (T-8040) |
| GEKO/801338F0 | `func_801339AC` | promo | `(D_800E6374 >> 12) == 6/7` chain on a u16 global: original lhu in v1, shift in v0, constants 6/7 in a0; IDO v0, t9, v1 (T-8040) |
| ENDING/8013B6A0 | `func_8013B9F0` | promo | init sequence stores one constant 0x70 to three separate s16 globals from $v0 (chain assignment of 3 array symbols gives the shared register but $t3, and the rest of the temps shift by one; T-8040) |
| BUNKASAI/8015AF90 | `func_8015B828` | promo | `D_800E6280.unk_1BC[i].unk_10.w` bit-field index into a local 16-word table: original loads through the separate symbol into $t8, IDO uses $a0 (T-8040; structure and frame match) |
| ETC/80146400 | `func_80146400` | reverse | two switches on u8 game-state bytes (D_800E62BE selector): original selector in $v1 (same register as the value), IDO $v0 (T-8040) |
| SHUGAKU/80135430 | `func_80135630` | promo | `func_800847B8(D_800E6280.unk_F5F = 2)`: original stores $a0 after the call from the argument register (li a0,2; jal; sb a0), IDO two separate li; rest of the function matches (frame needs one scalar above the RECT local) (T-8040) |
| SHUGAKU/80135430 | `func_80135480` | promo | local fn-pointer table then `D_800E738A` re-read after the call and range-tested: original keeps it in $v1 and the tbl entry in $v0, IDO $a1 and $v1 (frame matches, T-8040) |
| RPG_BAT/8014F790 | `func_8014F790` | promo | switch on `*D_8015ED8C` (s32 [0] of a pointer-less array): original hoists a dead `move v0,v1` of the selector into the first branch delay slot (default path returns the selector); IDO has no such copy (everything else matches with func_8014B738(.., 1, 200), T-8040) |
| TAIIKU/80133C80 | `func_80136DA4` | promo | `D_8014929C += D_80149218; clamp` on globals (promoted): original D_8014929C in $v1, D_80149218 $a0, D_801491E8 $a1; IDO $v0, $v1, $a0 (T-8040) |
| BUNKAKEN/80140F60 | `func_80140F60` | reverse | switch on s32 global D_80122EB8: original selector in $v1 (and temps shifted by one), IDO $v0; nested switch + sll/bltz flag test matched structurally (T-8040) |
| GEKO/801338F0 | `func_80133AA0` | reverse | four `if (D_800E7384++ == 1)` blocks (post-increment compare, known gap; T-8040) |
| ETC/80146400 | `func_801464D4` | promo | pointer walk over the GameState 8-byte records with ~8 loop-hoisted constants (a3=8, t0=2, t1=0x3B, t2/t3=0xB, t4/t5=0xF) and arg0 left in $a0; IDO moves arg0 and numbers constants differently (T-8040) |
| 800737A0 | `func_80074DE0` | promo | call result r tested twice then passed as arg: original keeps r in $v0 for the tests and copies to $a1 in the delay slot, IDO allocates r to $a1 (only 4 words differ, T-8040) |
| 80047550 | `func_80048EB8` | reverse | clamp of `arg0` into n (0xA0 cap) then loop calling func_80048F64: original has a dead `b` after the last arm (3-way if chain) that IDO drops; frame needs two extra words above the three saved s16 copies (T-8040) |
| DATE | `func_80145DF0` | reverse | andi result after a call kept in $t9 ($t8 skipped), IDO $t8; `(u32)(f() & 0x7F) >= 2` (T-8030) |
| OPTION | `func_80139F8C` | promo | `D & 0x60` compare chain on an s32 game-state field, original loads it into $v1, IDO $v0; tail copy `or a0,t1,zero` then `ori t2,a0`, IDO `ori a0` (T-8030) |
| OPTION | `func_80139878` | reverse | `D_800E62BA + 4` kept in $v1 (addiu) with the `andi` result in $v0, IDO loads into $v0 and puts the `andi` copy in $v1 (T-8030) |
| OPTION | `func_80139E3C` | promo | local u16 running value (D_8013D3B8 copy) kept in $a3 across the function, IDO uses $v0 (and $v1 for the D_800E7200 load, original $v0) (T-8030) |
| GEKO | `func_8013CC20` | reverse | `D_800E6280.unk_F5F = 7` stored from the call-argument register (`li a0,7; lui at; jal; sb a0` in the delay slot), IDO loads a second constant into $t6 (T-8030) |
| GYOZI | `func_8013AC38` | reverse | `D_800F5ACD = 8; D_800F62CF = 8; func_8008F618(8)`: original stores both bytes from $a0 (the argument constant), IDO reloads $t8/$t9; also a different load order of the final three global copies (T-8030) |
| SHOUGATU | `func_8013A2F8` | reverse | `unk_F5F = 5; func_800847B8(5)` stored from $a0 in the jal delay slot (same shape as GEKO `func_8013CC20`) and the three-global copy loads grouped differently (T-8030) |
| GYOZI | `func_80144A74` | reverse | `idx * 0x38` factor kept in $a0 (`li a0,56; multu`) and shared with the call argument `func_8008FB00(0x38)`, IDO uses $a1 and a second $a0 (T-8030) |
| GYOZI | `func_8013506C` | reverse | `if (D_800F6474++ == 0)` in an implicit-int function: original old value in $a0 and compare result in $v1, IDO $v1 and $v0 (T-8030) |
| GYOZI | `func_80135518` | promo | `D_8012E66C` loaded once into $a0 (first call argument) and copied `or v0,a0,zero` for a later store, compare `bne t6,a0`; IDO re-reads the global per use and orders the compare the other way (T-8030) |
| SHUGAKU | `func_80139054` | promo | `switch (D_800CA2CC)` with the byte loaded into $a1 (reloaded into $a1 for the last call), IDO loads it into $v1 ($v0 with an s32 local copy) (T-8030) |
| BUNKAKEN | `func_801428D0` | promo | nested switch on `lw` global D_80122EB8 (store of 3 to D_80122EBC first): original selector in $v1, IDO $v0 also with `u32` declaration/cast/local copy (T-8030) |
| TACO | `func_8014E1A0` | reverse | 21 constant stores through `D_8015EDB4[i]` in one block: original loads `li v1,0x800` (in a delay slot) before `li v0,0x40`, IDO the reverse; same for the later `li v0,0x80` (T-8030) |
| TACO | `func_8014F9B0` | reverse | `f(0xB, D_8015FEAC) != D_8015EDB4[20].unk2` then a ladder on the byte: original compares `beq v0,v1` and skips $t7 in the temp sequence ($t6 then $t8), IDO $t6, $t7 (T-8030) |
| TACO | `func_80150DE8` | reverse | 6-trip loop over two tables (8-byte and 2-byte records, pointers at D_80160138/D_80160216 with displacement -8/-2) plus a call with absolute symbols: original i in $s0, pointers $s1/$s2 and different prologue order, IDO swaps the saved registers (T-8030) |
| SHUGAKU | `func_80138DD8` | reverse | `if (a == 1 && b == 3) { if (c == 1) ...}` chain: the last `bne` has the loaded byte first and the shared constant register second (`bne t8,v0`), IDO the reverse; `1 == c`, nested/flat forms and the permuter do not change it (T-8030) |
| TACO | `func_8013D2A0` | reverse | ring-buffer write (`D % 16` index kept in $t3, record at $v1, counter reload for the compare): original loads the counter into $v0 and the record into $v1, IDO swaps them and adds a `move v1,t3` before the return (T-8030) |
| TACO | `func_8013D348` | reverse | pointer loop over a 16-record ring with `p->x++ >= 5` boolean compares in an implicit-int function: original has an extra `or v0,zero,zero` before the loop (a dead value kept live to the return), no C form found (T-8030) |
| GYOZI | `func_80142A8C` | reverse | polygon fill with three byte shifts of two stack arguments: original loads arg6 into $t1 and arg7 into $a0 and shifts into $t2/$t3/$t7/$t9, IDO uses $v0/$v1/$a0/$a1 and loads in the other order (T-8030) |
| ETC | `func_801436E4` | reverse | `v = arg0 << 15` spilled to sp+0x2C (top slot) with three calls; any unused local adds 8 bytes of frame (T-8030) |
| 80062CD0 | `icon_disp_switch` | reverse | pointer loop (`p = D_8011ECD0; ... p += 0x44`, bound `(u8)g->unk_F6D` through a `GameState *g` local) is right except `lui a0` sits before the guard `beqz` in IDO and in its delay slot in the original, for both arms; permuter finds nothing (T-8030) |
| OPTION/80132000 | `func_80137864` | reverse | 2-trip pointer loop with hoisted mask: original end pointer $v1 and mask $a0, IDO end $a0 and mask $v1 (T-8020) |
| DATE/8014CC40 | `func_8014DCCC` | regorder | function-pointer table call plus two address compares and `unk_110A` reads: original table pointer $a0 and index $v1 (plain global), IDO $v1/$a0 (GameState member base shifts the temps) (T-8020) |
| BUNKA_SD/801388D0 | `func_801389DC` | regorder | `return D += 1` on the `u8` GameState field `unk_110D` plus a switch on `unk_03E`: shape matches, original $v0 for both fields, IDO $v1/$v0 (member base shifts the temps) (T-8020) |
| EVENT/800F9680 | `func_800F9730` | regorder | 12-parameter helper (8 coordinates, 2 colours as stack arguments): frame and code shape match with one pad local, original keeps the colours in $a0/$t2 and the record pointer in $a1, IDO $v0/$a3 and $a2 (T-8020) |
| OPTION/80132000 | `func_80134804` | other | 3-trip record loop (`p == D_80120650 + sel * 0x44`, sel a signed byte GameState field): everything matches except the product, the original `sll 4; addu; sll 2`, IDO `li 68; multu` for every spelling (`*0x44`, `*17*4`, `s32[17]` rows, `Rec44` array) (T-8020) |
| ETC/8013FF80 | `func_80140BC4` | other | `return 2 / 1 / 0` over 13 consecutive GameState bytes (`>= 2`, then `!= 0`): the original zeroes $v0 before the first branch and ends the failing compares at one `jr; or v0,zero,zero`, IDO puts `move v0,zero` in every delay slot (T-8020) |
| TACO/80146D60 | `func_80147444` | regorder | RECT local filled from a word parameter and two `s16` parameters, then `func_8009C884(&r, arg0)`: with two pad locals the frame matches, temps are $t1/$t0 in IDO and $t6/$t7 in the original (T-8020) |
| OPTION/80132000 | `func_80136434` | other | four read-modify-writes of record bytes at `D_8011ECD0 + 1*0x44` (index kept in `$a0` as a non-constant, loads 0-3 then stores 3-0): IDO folds the index into absolute symbol addresses or builds a rolled loop; the sibling `func_801371FC` matched with the s16 pair as `D_80120912[]` and the flags as bytes of that array (T-8020) |
| SHUGAKU/80139D00 | `func_80139E8C` | regorder | scene init with a RECT and `v = D_800E71DF; if (v >= 0xE) v = 0xD`: frame, calls and RECT constants match with `u8 v` declared before the RECT; the GameState `unk_F5F` read is `lui v0; lbu a0,3935(v0)` against the original `lui a0; lbu a0,0(a0)` (T-8020) |
| TT/80142D40 | `func_801436BC` | regorder | straight-line zeroing of two TT records (pointer saved across `func_80143580()`) plus a 2-trip loop: code matches, the three hoisted constants (record address, 2, 0x60) are $a1/$a2/$a3 in IDO and $t1/$t0/$a3 in the original; unused parameters do not move them (T-8020) |
| DATE/8014CC40 | `func_8014E38C` | promo | `++GameState.unk_1104.u < 0x10` with an early `return 0`: original keeps the incremented word in $v1, IDO $v0; an `s32` return and a local copy do not change it (T-8020) |
| EVENT/800F9680 | `func_800FA194` | regorder | `u8` global read after a call and kept across a 15-record loop (index in $a0, pointer $a2, constant $a3, mask $v1 in the original; $v1/$a0/$a1/$a2 in IDO); `func_800FA0D0` is the same shape (T-8020) |
| BUNKA_SD/80134540 | `func_801345BC` | other | two 2-trip record loops plus two scalar stores before them: loops and offsets match with `D_8011ED58[i * 0x110 + k]` (`u8` override for `D_80120656`), but the `sb` to `D_8012069A` is scheduled after the pointer setup in the original and before it in IDO (T-8020) |
| RPG_BAT/801447E0 | `func_80145180` | other | three constants stored to three fields of `RpgStat` and a fourth store of 0x14 later: original `li a1,0x14` sits in the first delay slot, IDO keeps it with the stores (T-8020) |
| RPG_BAT/801447E0 | `func_801453B4` | other | same family: all code matches, the `addiu v1,-5` of the second return branch sits after the `lui at` in the original and before it in IDO (T-8020) |
| MASTER/80139000 | `func_8013B20C` | regorder | 3-case state switch (selector $v1 matches) ending in a `GsRec66C` byte `|= 0x40` indexed by `unk_0F4.h >> 12`: everything matches except the last temp pair, original $t6/$t7 (an unused $t5 in between), IDO $t5/$t6 (T-8020) |
| EVENT/800F9680 | `func_800FAA04` | regorder | 15-record init loop with a `func_800789A0() % 320` call: six constants live in saved registers (0x24, 0x140, -0x78, 0x1000, 8, 0xC1000000 in s2-s8/fp), IDO assigns the same set in a different order and keeps the record pointer in $v0 (T-8020) |
| GYOZI/801372B0 | `func_80137D70` | regorder | `(u8)func_8005E0E0(..) & 0x7F` and two `D_800F5AAE`/`D_800F647A` read-modify-writes: C matches except IDO hoists the `lbu` of `D_800F647A` above the `sb` of `D_800F5AAE`, the original does not (T-8080) |
| GYOZI/801372B0 | `func_801381DC` | regorder | same `(u8)` cast family: the original leaves a `nop` in the `jal` delay slot, IDO fills it with the `sb` to `D_800F5AAE` (T-8080) |
| GYOZI/801372B0 | `func_80137F48` | regorder | six word copies between globals: original uses $t6/$t7/$t8/$t5/$t6/$t7 in an interleaved order, IDO $t9/$t0/... after the sequence of stores (T-8080) |
| TEL/801397D0 | `func_80139E3C` | regorder | `(w << 3) >> 28` selector loaded from a game-state word: original keeps it in $v0 for the whole function (lw v0; sll t6,v0; srl v0), IDO moves the shifted value to $t7 (T-8080) |
| EVENT/8010FE10 | `func_80115714` | regorder | six halfword stores of 1/0 then 3/2: the original shares one `li` register across stores of different scalars, IDO loads per store (T-8080) |
| EVENT/8010FE10 | `func_8011C470` | regorder | `old = D_800B1AF6` kept at sp+0x28, return value in $v0 and `(u8)(v+5)` in $t0: IDO puts `old` in $t6 and the return value in $v1 (T-8080) |
| BUNKASAI/801354B0 | `func_801354B0` | regorder | u32 selector in $v1 matches (override) but the original loads the selector before the byte read-modify-write, IDO after (T-8080) |
| ENDING/80137340 | `func_80137F64` | reverse | counter `D_8013C360` tested for zero, then reloaded (`lw t9`) for `== 1` in the original; IDO keeps one register (same shape in `func_80137E40`, `func_80138C70`, `func_80138A9C`, `func_80138DC8`) (T-8080) |
| 8005A0B0 | `func_8005B0F4` | reverse | same lookup as `func_8005B040` (table `D_800B59B0`) (T-8080) |
| 8005A0B0 | `join_club_select` | regorder | C matches but the original leaves a `nop` after `lw t9,D_800E7208` where IDO moves the next `lui v0` into it, and uses $t5/$t6/$t7 where IDO uses $t2/$t3/$t5 (T-8080) |
| NAME_ENT/80132000 | `func_80133E54` | regorder | five-trip unrolled loop (`for (i = 0; i < 5; i++)` over `D_8014CCD0[i]`): shape right, original keeps x in $v0 and y in $v1, IDO $v1 and $a1 (T-8080) |
| NAME_ENT/80132000 | `func_80133D94` | regorder | `a` kept in $a0 (parameter-like), constant -1 hoisted into $t9: IDO uses $v0/$v1 in the other order for the two `lh` (T-8080) |
| NAME_ENT/80132000 | `func_80132198` | regorder | state machine matches except one `lui $at` (the final `D_800E738D = 1` store) is hoisted above the `li v0,1` (T-8080) |
| EVENT/8010FE10 | `func_801142E8` | regorder | `idx * 6` per access: the original recomputes it with `li v1,6; multu` six times, IDO computes it once with shifts (same for `func_80114784`, `func_80114948`) (T-8080) |
| 8005A0B0 | `func_8005BCEC` | regorder | menu arrays: everything matches (layout, constants) except the order of two store pairs (`sh t9;sh t8` against `sh t8;sh t9`) (T-8080) |
| 8005A0B0 | `func_8005E150` | regorder | same loop as `func_8005E7F0`, but with the `get_k_speed()` switch IDO allocates the loop pointers one register later (T-8080) |
| 800451D0 | `func_80045318` | regorder | ring-buffer push: the original keeps `48` in $v0 and copies `arg2` to $a3; IDO uses $a3 for the constant (T-8080) |
| BUNKAKEN/80152680 | `func_80152F4C` | regorder | local 16-word table (frame fixed with `s32 pad[2]` before it): original loads the column word into $t1 and the row pointer into $t8, IDO $t2 and $v1 (T-8080) |
| TACO/80138890 | `func_80138890` | reverse | `D_800E6280.unk_1100` read twice after the switch: the original CSEs the load into $t8, IDO reloads it into a second register (T-8080) |
| 8007C030 | `normal_date_three_select_init` | regorder | chain-assignment menu arrays match (T-9020 idiom) except the `0x10` chain: original `$v0`, IDO `$t3` (TAIIKU `func_80141964`, same body without the trailing stores, matches) (T-9020) |
| 8007C030 | `normal_date_girl_suddenin` | regorder | `D_801217D0[0].unk_14 = .unk_15 = .unk_16 = 0x80` chain: original `li v1; move v0,v1`, IDO one register (T-9020) |
| DATE/80132000 | `func_8013FA6C` | regorder | seven-target chain (`... = D_8011ECD0[0x843] = 0x80`) right except `li v0` scheduled after the first `lui at` (original before); permuter best 70 (T-9020) |
| DATE/80132000 | `func_8013BEB8` | regorder | three chains of the `D_800CA21C` rows: the second `3` chain is in `$v0` in the original, `$t4` in IDO (T-9020) |
| GEKO/8013C980 | `func_8013CC20` | regorder | `func_800847B8(*(u8 *)&D_800E6280.unk_F5F = 7)` shares `$a0` as the original does, but IDO puts `lui at` before `li a0` (original: `li a0; lui at; jal; sb a0`, all 37 such sites) (T-9020) |
| SHOUGATU/8013F190 | `func_80140AD8` | regorder | C right with `unk_0C.f.b1`; IDO hoists `lui v0; addiu v0` of the GameState base into the branch slots, the original hoists `li t2,1` instead (T-9020) |
| ENDING/80137340 | `func_80137E40` | regorder | `unk_1BC[1].unk_0C.f.b14 >= 1` gives the original's `sltiu; xori`; selector in `$a0` and flag word in `$v1` where the original has `$v1` and `$a0` (T-9020) |
| VALEN/80132760 | `func_80133670` | regorder | everything but the first argument load: original `lui v0; lbu a0,%lo(D_800E71DF)(v0)`, IDO `lui a0; lbu a0` (member, `*(u8 *)&` view and byte index all the same) (T-9020) |
| BUNKASAI/80146030 | `func_80146030` | promo | selector `D_80122EB8` in `$v1` and loaded before the stores: plain read gives the order but `$v0`, `*(u32 *)&` view gives `$v1` but loads after the stores (T-9020) |
| TACO/80134930 | `func_8013546C` | regorder | s8 counter `D_8015EDE8`: IDO sign-extends the reloaded value into a second register (`sll; sra; move`), the original uses it in `$v0` directly (T-9020) |
| 800737A0 | `get_weekly_bg_sector` | regorder | C right (`unk_69C[unk_71C].unk_01`, two tables); index in `$t6` and byte in `$v1` where the original has index in `$v1` and byte in `$a0` (T-9160) |
| 800674B0 | `func_80067DD4` | promo | `if (!(D & 1)) D |= 3` on a u8 global: original keeps the loaded byte in `$v0` and narrows the or-result into `$v0` (`andi v0,t7,0xff; sb v0`), IDO stores `$t7` directly; `(u8)` cast and a u8 local do not give it (T-9160) |
| 800674B0 | `func_80067DFC` | promo | same as `func_80067DD4` on `D_80120652` (T-9160) |
| DATE/8014CC40 | `func_8014DB98` | regorder | `s16` ring counter `v = D_8015E310 + 1; if (v >= 4) v = 0`: original narrows once in place (`sll t2,v0; sra v0,t2`) and keeps v0, IDO narrows twice or moves to `$v1` (T-9160) |
| EN_NICHI/80132000 | `func_80132ADC` | regorder | owned `D_80139B24` (island done): original loads and increments in `$v0` (`lw v0; addiu v0,v0,1`) and puts the `== 0x1B` store out of line (`beq`, then `jr`); IDO loads into `$v1` and lays the blocks out the other way, whatever the if/else or early-return form (T-9160) |
| EN_NICHI/80132000 | `func_80132A74` | regorder | squared Manhattan-style distance with u32 `ax*=ax; ay*=ay`: C right except `ay = dy` copy in `t0` / `dy` in `t1` vs `v1`, and the `move` in the `bgez` delay slot (T-9160) |
| SHUGAKU/80138A60 | `func_80138DD8` | regorder | C right; third compare `D_800CA2CC == 1` is `bne t8,v0` in the original (constant 1 kept in `$v0` from the first test) and `bne v0,t8` in IDO, whatever the operand order written (T-9160) |
| KANGEI/801355F0 | `func_80135C78` | regorder | nested-if or early-return both give the same code: original puts `lw ra` in the delay slot of the second `bne` (separate exit block for the 0xA/3 test), IDO shares the epilogue (T-9160) |
| TAIIKU/80142240 | `func_80143D24` | regorder | `D_8014A13C+0x16` s16 clamp: original reloads the field after each store through the hoisted base `$v1` (`sh t4; lh v0,22(v1)`), IDO forwards the stored constant; a struct aggregate for the stride-0x24 table is not known (T-9160) |
| DATE/8014CC40 | `func_8014E38C` | regorder | `if (++D_800E7384 < 0x10) return 0; f(); g(); h();` (s32, no final return): original keeps the incremented counter in `$v1`, IDO in `$v0`; return-0 and temp-variable forms do not move it (T-9160) |
| OPTION/801389A0 | `func_80139878` | regorder | `D_800E62BA` (`unk_03A`) `v = (u8)(x + 4)` stored three times: original `lbu v1; addiu v1,v1,4; andi v0,v1,0xff`, IDO `lbu v0; addiu v0; andi v1` (T-9160) |
| OPTION/801389A0 | `func_80139F8C` | regorder | `D_800E7208 & 0x60` compare chain: original loads the GameState word into `$v1` and `D_8013D3A8` into `$t1` (`lbu t1`, then `ori a0; andi a0` order), IDO `$v0` and `$v0` (T-9160) |
| GYOZI/80142D00 | `func_80142D40` | regorder | local FnTbl27 copy, `tbl.f[idx](0x80)` then compare of `tbl.f[D_800F647A]` with two main functions: structure and frame right (T-3330 idiom); original reads D_800F647A into `$v1` and keeps the function addresses in `t5/t8/t7`, IDO uses `t4/t8/t7/t6` (T-9160) |
| TACO/80156A80 | `func_80158F48` | regorder | C right (`(arg0 + D_8015EDB4)->` forms, `func_8013DB80(TcPos, 0x80, 0x80)`): only the reload order after the second `jal` differs (`lui t3; lw t3; lw v1,44(sp)` in the original, `lw v1` before `lw t3` in IDO) (T-9160) |
| KANGEI/80135F10 | `func_80137484` | reverse | `D_80120657 += 8` before a `for (i = 0x61; i < 0x74; i++)` record loop (`*0x44` multu), then `D_80120657 >= 0x80`: original reloads the byte after the loop, IDO keeps it in `$a2` across the loop whether the loop body uses an array index or a pointer (T-9160) |
| TT/80144D70 | `func_80145370` | regorder | two record loops (0x40 x 0x78, 0x80 x 0x68, second unrolled by IDO): C matches except the constants `0x70`/`0x40`/`0x80` are in `$a3/$a2` in the original and `$a1/$a0` in IDO (phantom argument registers; unused parameters do not move them) (T-9160) |
| SHOUGATU/80135B90 | `func_80136340` | regorder | `if (D_800E62BE == 0x62) .. else if (B20 == 0 && F5F == 9 && B1C == 9 && t < 2)` then `if (B20)`: original does not promote `D_80143B20` (reloads at the end through a `lui t7` hoisted into the first branch slot, constant 9 kept in `$v0`), IDO keeps it in `$v0` and uses `$a0` for 9 (T-9160) |
| SHOUGATU/80135B90 | `func_80136078` | regorder | local FnTbl50 copy and `*p == D_8007E390 || == D_8007E5A0` test (as `func_80134120`): original loads the index into `$a1` both times and keeps the table pointer in `$a2`, table at sp+0x34 (frame 0x100); IDO uses `$t2`/`$a0`, table at sp+0x2C or 0x30 whatever locals are declared (T-9160) |
