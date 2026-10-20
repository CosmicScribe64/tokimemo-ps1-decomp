---
type: archive
---

# Matching notes, part 4

Sections moved verbatim from [[matching-notes]] by `tools/wiki.py rotate-notes`, oldest first. Find a function in [[notes/functions]] and a section in [[notes/sections]].

## regdiff and two register-order matches (T-10400)
`tools/regdiff.py` ([[decompile-workflow]], [[uopt-register-allocation]] "regdiff") explained and fixed two register-order T-0018 rows:
- EVENT `func_80119518` (`bne` operands of a `u8` selector swapped): `u8 sel; sel = D_8012531C;` and compare `sel`. A `u32` copy matches too, `s32` does not.
- DATE `func_80136F2C` (hoisted array bases in t0/t1 swapped, 28 hand variants and a permuter run had failed): drop the temporary of `v = D_800CA22C[i]; D_800CA224[i] = v;` and write `D_800CA224[i] = D_800CA22C[i];` with `D_800CA234[i] = D_800CA22C[i] - 1;` later. Hoisted constants and addresses are numbered by first use in the loop, the left side of an assignment first, and equal priorities colour the lower bit first.
EVENT `func_80114B0C` (draft now 5 differing lines, still guarded) and TT `func_801475C8` (0x40 against 6, not matched) show the same tie of hoisted constants. A swapped `bne` that no register change explains is operand order: try a local copy of the narrow global first.

## Wave 12, list 1 (T-10310)

Result: 2 matches (BUNKA_SD `func_80132708`, 220 bytes; SHUGAKU `func_80139740`, 304 bytes), 8 guarded near misses, 14 more drafts kept in scratch. Most of the 163-function list (checked with grep) had already failed in earlier waves; the fresh functions are 300-1000 bytes.

New patterns:
- **Dead test hoists a shared product.** BUNKA_SD `func_80132708`: `q * 2` is used by five switch arms (`0x19 - q*2`, ...). IDO re-computes it per arm; the original computes `sll a0,v1,1` once before the jump table. `t = q * 2;` followed by `if (t < 0) {}` (FAKE, T-10310) makes IDO compute it once. Also needed: explicit `case 4:` sharing the default (five table entries), `s32` return without `return`, and `v = v + 1; D = v;` as two statements (`addiu v0; andi t8; move v0,t8`).
- **Permuter finds narrow temporaries.** SHUGAKU `func_80139740`: `s32 t = func_80051A68(..)` gives temporaries one register low (the original skips `$t7`). `u8 t` (or `u16`) fixes every later register number; found by `permute.py` (best score 50, then confirmed by hand). Plus `s32 pad[2]` (FAKE) for the 8-byte larger frame. Lesson: run the permuter on drafts whose instruction count equals the target even when 30+ lines differ; it refuses drafts with a different shape ("invalid base").
- **Statement order changes load order.** GEKO `func_80140040`: `*p74 += *p7C; *p70 += *p78;` vs the reverse changed the whole temporary numbering (24 -> 8 differing lines). Permute independent read-modify-write statements by hand.
- **Frame slots in the T-3330 table idiom.** Locals declared before `tbl` sit above it (EVENT `func_801022C0`: `s16 i; u8 c; s32 idx; EvFnTbl53 tbl;` gives the frame 0x108 and the table at sp+0x2C; `s32 idx` declared first moved the table down by 4). A repeated `arg0 * 0x88` is spilled by IDO as its own CSE slot: do not declare `off` (TACO `func_801557E8`, an explicit `s32 off` added a hole).
- **K&R struct-copy tables need no `idx` local** when the frame has no slot above the table (SHUGAKU `func_80135480`: table 0x128 bytes at sp+0x2C, frame 0x158).
- `data_island.py` for `main/8007C030` and `TAIIKU/80138950`: define `D_800CA2A4` as `u8 D_800CA2A4[4]` (IDO aligns every scalar definition to 4, so three separate `u8` definitions shift the next variable); TAIIKU `D_80149984[2]` is an `s16` pair defined in the file.

Near misses (all in `#ifdef NON_MATCHING` with the INCLUDE_ASM fallback unless noted): GEKO `func_80140040` (7 lines: the original re-loads `D_801470AC` unconditionally after the join, IDO keeps the hoisted register on the no-call path), RPG_BAT `func_8014E300` (4 lines: the original does not move `lw D_8015EC2C` above the `sw D_8015EC50`), TACO `func_801546FC` (9 lines: the original has `move s0,a0` and loads through `s0`; IDO copy-propagates `a0`), ETC `func_80146400` (20 lines: `a0`/`v1` for the reloaded value, string `lui` placement; switch statements for both selector chains and `s32 pad[3]` FAKE for the frame), TACO `func_801357B0` (29 lines: saved-register order and a dead `move v0,zero`), main `junban_init` (34 lines, `$v0`/`$v1` swap), `normal_date_girl_suddenout` (18 lines, `lui a2` for 0x80000000 and store order), TACO `func_80154820` (44 lines, no guard: brightness clamp; `v2` form without the `command` copy).
Scratch drafts without a guard (reasons): SHUGAKU `func_80135480` (45, `$v0`/`$v1` swap of function pointer and index), SHUGAKU `func_801398E8` (58, selector `$v1`), EVENT `func_801022C0` (38, `$a3`/`$a0` for `c`/`i`), EVENT `func_801035BC` (58, loop-invariant `D_800EAFA7` hoisted), TACO `func_801557E8` (30, `addu` operand order), TAIIKU `func_8013D114` (address-in-register `lui/addiu v1` rematerialised after calls), TT `func_80147FD0` (36, zero kept in `$a1`/`$v1` for a chain of byte/halfword stores), TT `func_80149B20` (79), ENDING `func_80139B48` (152, `$a0` for the shifted colour), SHOUGATU `func_801359FC` (61, 35-byte local string table).
Tooling observations: `permute.py -j 2` crashed on `tools/queue.py` shadowing the stdlib `queue` until the coordinator's fix (1c7fa03); `ninja progress` fails with "neither INCLUDE_ASM nor C body" for BUNKASAI `func_80154018` and four neighbours (merge state, not touched here); `sync_protos.py --add-global` rewrites `main_api.h` without the `MAIN_API_OVERRIDE_` guards until `--write` is run again.

## Wave 13, list 2 (T-10520)

Matched (9): GEKO `func_80139D54` and SHOUGATU `func_80138D2C`, `func_80139514` (the three twins `dupes.py` skipped for declaration clashes, copied by hand with GameState fields and the `*(u8 *)&D_800E6280.unk_F5F = k` argument idiom), SHOUGATU `func_80137D0C` (`s32` function without `return`, `D_800E6280.unk_1104.w++ == 1`), TACO `func_8013474C` (twin of main `initCoordinate`), OMIMAI `func_801331B0` (37 pointer stores, plain m2c), TAIIKU `func_8013A63C`, `func_8013ACBC`, `func_8013C4E4`.

- **Shared `lui $at` groups of TAIIKU/80138950 are now defined in the C file** (the object has a data island): `D_80149988` (`TaiikuSel`: s32 + u16 + u16, so `D_8014998C`/`D_8014998E` are `unk_04`/`unk_06`), `D_80149970` (u16 pair), `D_801499A0` (`TaiikuA0`: u16, u16, s16, s16, s32, s32 for A0..AC), `D_80149968`, `D_80149974` (s32 pairs), `D_80149980` (s16 pair), `D_80149990` (s32 x3). Members stored in one block then share the `lui`; `func_8013ACBC` (22 stores) and `func_8013A63C` match with plain member updates. Loads of a member do not share (a separate `lui` per load), as in the original.
- **TAIIKU/80133C80 slot data** `D_80149208..D_80149297` is one object of 0x90 bytes (`TaiikuSlots`, four 0x24-byte slots; `f0C/f0E/f14/f1C`, `f30...` are per-slot counters). With the per-group definitions I tried first, IDO hoisted the next `lhu` above the previous `sw` (different symbols never alias); as one object the loads stay behind the stores as in the original. `func_801368B8` is then exact except for one `lui`/`sw` swap (the original schedules the next load's `lui t4` before the `sw`), kept guarded. `D_80149298` is `TaiHalves` (s32 whose high half is `D_8014929A`).
- **Frame by dummy locals:** `func_8013C4E4` needs 7 local words; two unused `s32` locals before the four output words give the frame (FAKE comment in the source). Without them the frame is 8 bytes short and the output words sit 4 bytes too low.
- **K&R narrow parameter definitions** (`func_80045318(arg0) u8 arg0;`) give the `andi t6,a0,0xff; move a0,t6` prologue; `check_headers.py` rejects a K&R definition when the header has a prototype (TACO `func_801347F4`), so such near misses keep an ANSI header-compatible definition.
- **Promoted globals written back late:** `D_80125CC0` (`func_8007B64C`), `D_8014929C` (`func_80136DA4`), `D_800E62B6`/`D_80120657` (`func_80134B18`): write the C as in-place updates of the global (`D_x += 8; if (D_x == 0 ...) D_x = 0xFF;`), IDO keeps it in one register and stores it before the first call. A local copy changes the register order.
- **`static` data definition** (to make a global unaliasable) emits `.data`-relative relocations and breaks the link; do not use it.

Near misses (guarded `#ifdef NON_MATCHING`, ticket T-10520): TAIIKU `func_8013AE14` (2 lines: ugen temp of `D+0x48` is `$t9`, original `$t1`; the original consumes two more temps), `func_801368B8` (2 lines, swap), `func_80138800` (6 lines, ugen temporaries), `func_80136DA4` (14 lines: the original never uses `$v0`), `func_80137228` (all registers: `D_8014929A` is hoisted out of the loop in the original, never in mine, array or pointer form, with or without a definition); TT `func_8014C5F8` (12: constant 0 of two `sb` kept in `$a0`), `func_80145370` (10: hoisted constants in `$a1/$a0`, original `$a3/$a2`), main `func_80058F9C` (16), `func_80045318` (16), ENDING `func_80134B18` (9: `D_80120657` in `$a1` as one range across the calls), VALEN `func_80133670` (2: base register of the argument load `$a0` against `$v0`), TACO `func_801347F4` (7: `move s2,s3; addiu s2,s2,8` against one `addiu`), `func_80146D60` (9: saved-register order of arg0, base and loop end). `regdiff.py` reported "ugen temporary" for the VALEN and TT rows (no allocator decision to change).

Unmatched without a guard: `InitMouse` (two stores through one `lui $at` to `D_800E36C0`/`D_800E36C4`; the object 80059710 has no data range), `func_8007A868` (the original loads `D_80125D10`/`D_80125D14` into `$v1/$a0` at the top and ANDs twice without folding), `func_8007B64C` (promoted `D_80125CC0` in `$v0`, `D_80125CC2` loaded early; IDO sinks the load or takes `$v1`), `func_80144198` (GEKO, `arg0` is kept in `$a2` in the original, IDO leaves it in memory), `load_tpage_buf_lock` (parameters reloaded from the home slots in the second branch), TT `func_8013BE9C` (priority of `base` against `arg0`: raise `base` above 2), KANGEI `func_80136970`, SHOUGATU `func_8013D820`, main `func_800590CC` (0x140/0xF0 set before the branch in the original), `func_80079F00`, GYOZI `func_80137188` (kept guarded by wave 12).

Tooling observations: `funcdiff.py --resolve` printed a bare `sw ... 28(at)` vs `0(at)` pair for TAIIKU `func_801368B8` while the object differs by a `lui`/`sw` swap that only the sha1 line and `objdump` show; and once reported MATCH (TAIIKU `func_80137228`, pointer-form draft) for an object whose function was 12 bytes longer, which `ninja` then rejected. Decide by the sha1 line, not by funcdiff alone. `dupes.py -v` skips twins whose target declares a global with another type; the three here were done by hand.

## Wave 13, list 1 (T-10510, w13-1)

Result: 11 matches, 2,380 bytes: GEKO `func_801397A8`, `func_80141DDC`, `func_80142018`; SHOUGATU `func_80139CCC`; DATE2 `func_80135DA4`, `func_80136A90`; EVENT `func_80100AB8`; NAME_ENT `func_80145EA4`; TT `func_80133C1C`; main `yuukou_down`, `select_girl_main`. 15 guarded near misses (plus `normal_date_girl_suddenout`, a wave-12 guard re-annotated) and about 12 more drafts in the agent scratch. Rows for the failures are in [[data/t0018-cases]].

Idioms that worked:
- **Matched twins with other global names need the same field expression, not the m2c names.** GEKO `func_80141DDC` is the EVENT `func_8010B678` shape: `(&D_8011F4EE)[0x22] += 0x40;` (the 0x44-apart pair first, through the lower symbol), `D_8011F4EE += 0x40;`, then `D_801217D4`/`(&D_801217D4)[0x12]` for the Rec24 pair. Writing `D_8011F532` or `D_801217D0[1].unk_04` changed the load placement. `func_80142018` needs `(&D_8011F4CB)[0x44] &= mask;` first for the same reason. DATE2 `func_80135DA4` is `Rec24` `D_801217D0[4..7]` as in DATE `func_80148924`; EVENT `func_80100AB8` is `D_800B0A04[7].unk_02/unk_06` (m2c prints `D_800B0B72`/`D_800B0B76`).
- **Narrow parameter types change the hidden ugen temporaries.** TT `func_80133C1C(o, kind, idx)`: `s16 kind` and `s32 kind` start the temporaries at `$t8`, `u16 kind` at `$t7` as the original does; regdiff's line "ugen temporaries in order of definition: draft t8 ..., original t7 ..." is the hint (40 s once the three statements were right). The same hint fixed DATE2 `func_80136A90`: `(u8)func_80051A68(..) & 0x7F` instead of `func_80051A68(..) & 0x7F` removes one `andi` temporary.
- **A 15-line near miss can be a layout problem.** NAME_ENT `func_80145EA4`: the loop-prologue instructions came out in another order until the last `if` of the peeled first word shared its source line with the `for` (marked FAKE). The permuter found it in 20 s: run it on 2-line diffs even when the cause looks like register allocation.
- **Write the natural index loop first.** main `yuukou_down` compiled to the original's peel-plus-pairs structure from `for (i = 0; i < 0xB; i++)` with `D_800E6280.unk_1BC[i]`; the unreachable `p == &D_800E6280` compares of the original are the `i == 0` test of the second branch. Only the operand order of one `bne` was left: a constant local (`two = 2; ... two == q`) gives it (FAKE, same family as `s16 one` in ENDING). The field was `unk_06` (offset 0x1C2), not `unk_0A` that m2c suggested.
- **Hand the record stride to the compiler.** main `select_girl_main`: `*((u8 *)&D_800E6280 + i * 16 + 0xF9E)` gives the original's `sll t0,v0,4` index (the struct form `unk_F90[i * 2 + 1].unk_06` multiplies by 2 and then 8). The found-branch ends with `return 0` (`or v0,zero,zero` in the `b` delay slot), so the function is `s32`; `sync_protos --fix` moved the type in `main_api.h`.
- **Switch with an 18/19 shared case**: `case 18: case 19:` with `return;` in every case but the last gives the original's per-case `jr ra`; `D_80155B42`/`D_80155B3C` are `&D_80155B30[3]`/`[2]` (TtCollision6 stride 6).

Left unsolved, with the closest numbers: DATE2/TT/ETC register shapes in the table (4 to 30 lines). Two shapes to try next:
- **Constant materialised once in an argument register** (DATE `func_80137D1C`, main `func_800422C8`): the original keeps `0x14` (or `id`) in `$a1`/`$a3` for both a store and a call argument; no spelling tried (`f(0, D = 0x14)`, locals, u32 constants, order swaps) reproduces it.
- **Pointer parameter advanced before a loop** (TACO `func_80154230`, also `func_801546FC`): the original copies `a0` to `$s0` at entry (`move s0,a0` first) and loads the header words through `$s0`; IDO folds the copy. The permuter refuses the base because the instruction count differs.

Not compiler output (stay `INCLUDE_ASM`): DATE `func_8015A980` and TACO `func_8015D0D0` are the `RotMatrixY`-style rotation helpers already named in [[gcc-objects]] (`multu` for signed products, `$t7`-first registers).
Tooling notes: `permute.py` rejects a base whose instruction count differs from the target (TACO `func_80154230`, DATE `func_80137D1C` e3), which hides the cases where the missing instruction is the whole problem; `tools/regdiff.py` cannot test a draft that defines file-scope data (the function is cut out).

## T-10610 (wave 14, list 1)

Matches: EVENT `func_80103BF8`, `func_80104B00`, `func_80104D48`; SHOUGATU `func_80135B10`, `func_8013507C`; ETC `func_80140BC4`. Guarded near misses: TAIIKU `func_801339C4` (4 lines), TT `func_80147FD0`, EN_NICHI `func_801330D0`, EVENT `func_8010B614`, main `func_8007A868`.
- **Separate-symbol names in the asm are often one array.** When the original's loads of neighbouring globals stay in source order but IDO hoists a later load above earlier stores, write the symbols as offsets of one array: SHOUGATU `func_8013507C` (`D_80120650[3]`, `[0x46]`, `[0x47]`, `[0x8A]`, `[0x8B]` for D_80120653/696/697/6DA/6DB; the stride is 0x44 records) and EVENT `func_80104D48` (`D_800B0860[6..8]` for D_800B0866..868) matched at once after every plain-scalar form failed with a swapped load. EN_NICHI `func_801330D0`: `(&D_8011ECF4)[1]` for D_8011ECF8 fixed all eight switch arms (the cursor words are the 16.16 positions whose high halves are D_8011ECF6/FA). The reloc names in the splat asm are no evidence of separate symbols; `funcdiff --resolve` compares the resolved bytes.
- **`()` prototype for a live argument register.** TAIIKU `func_801339C4` reads `$a0` although its overlay prototype said `(void)` and the caller passes nothing: the K&R definition `void f(arg0) s32 arg0;` plus `void f();` in the header gives the original's body (135 of 135 instructions, the selector load lands in `$v0` instead of `$v1`).
- **Return value set before the test.** ETC `func_80140BC4` (returns 0/1/2): `r = 0;` as the first statement, `return 0;` inside the inner block and `return r;` at the end reproduces the original's `or v0,zero,zero` before the first branch and its `jr` with an empty delay slot; a bare `return 0;` at the end fills the delay slots with `move v0,zero`.
- **Twin overlays can use other symbols.** EVENT's `func_80103BF8`/`func_80104B00` are DATE's `func_80156570`/`func_80157184` with `D_800B0A04[5].unk_0C.b14`, `D_80094714`, `func_80011DFC` and `func_8002328C(D_800B1746)`; `dupes.py` skips them (the twin's field path is not valid in the other overlay), so copy by hand and map the symbols from the neighbouring matched functions.
- **Struct view of u8-times-0xF00.** `(s32)D_800E8BEF * 0xF00` still compiles to `li; multu`; `((Blk *)base)[D_800E8BEF].b[k]` with a 0xF00-byte `Blk` gives the original's `sll 4; subu; sll 8` (NAME_ENT `func_801398EC`, draft only; IDO then unrolls the 6-trip loop 4x or 8x where the original is rolled).
- **Dead ends (see the rows in [[data/t0018-cases]])**: the move-then-add prologue shared by main `initModelingData_init` and TACO `func_801347F4` (`or s2,s3,zero; addiu s2,s2,8`), the checksum loop `func_8004636C` (permuter refuses the base: 28 against 30 instructions), GEKO `func_80144198` (frame 0x40 with the argument in `$a2`), the EN_NICHI/NAME_ENT range checks with the early `or v0,v1,zero`.

## Wave 14, list 2 (T-10620)
8 matches (1,784 bytes) from a list whose easy members earlier waves had already tried: six from matched twins, one plain counted loop, one found with the permuter.
- **Twins in other overlays need the twin's spelling.** `dupes.py --apply` skipped or rejected six twins because the target overlay declares the globals differently. Copying by hand and keeping the twin's idiom matched them: `(&D_x)[0] = (&D_y)[0] = k` chains for scalars (EVENT `func_8011B5A8`, from DATE `func_8013FA6C`), `(&D_80124394)[0] = (&D_80124394)[1] = (&D_80124394)[2] = 3` for a three-element table written through its first symbol (EVENT `func_80119C9C`), a generated table body for OMIMAI `func_80132AD0` (39 stores, three `s32[]` symbols). A callee whose twin returned `s32` (GYOZI `func_8013FD9C`: `func_8008D610`) needs the `MAIN_API_OVERRIDE_` view in the .c file; the final call differs between twins (`func_8004DE1C`, not `func_8004284C`), so read the asm, not just the twin.
- **Two 5-5-5 colours per word are two bit-field structs** (main `func_8004B590`): read as `u32 r0:5 g0:5 b0:5 x0:1 r1:5 ...`, written as `u16` fields. The writes then come out as `sb`, `lhu/sh`, `sb` per pixel, exactly the original's partial stores. `dst->r0 = t = lum / 100; dst->g0 = t; dst->b0 = t;` matched only after the permuter dropped a redundant `& 0x1F` (the bit-field store already masks). Lesson: when the structure is right and only ugen temporaries differ, run `permute.py` on the plain draft before logging a row. `make_color_bar` in the same file (8 rows x 32 columns of such pixels, three 16-trip channel loops and two 128-trip alpha loops) reproduces the instruction count and every field store with a `u16 r:5 g:5 b:5 a:1` struct; what differs is the allocation order (the original colours the bit-field temporaries first, then the induction values i/j/p), see its row in [[data/t0018-cases]]. `make_color_bar16` (1,540 bytes, different arithmetic) was not drafted beyond m2c.
- **`for (i = 2; i != 0xA0; i += 2)`** is the loop of TACO `func_80143AF4` (the `L` flag was a plain counted loop).
- **Actor lists need no aggregate:** TACO `func_8015C0A0` has the right body with `u8`/`TcActor` fields; what is left is one copy (`move v0,v1`) because IDO keeps the pointer local and the `&D_8015EDB4[arg1]` expression as two ranges.
- **Frame offsets of two local copies** (TAIIKU `func_80137090`: `Tw3` + `s16[3]` at sp+0x2C/0x24): the original frame is 0x40 with both copies 8 bytes lower than IDO places them; `s32 pad`, `s16 pad`, `s8 pad` and hoisting `D_80120A72` into a local all grow the frame instead.
- **Data hoisting depends on who owns the symbol.** TAIIKU `func_80137228` and `func_80137090` hoist the file's own `D_8014929A` / the extern `D_80120A72` into a register, but IDO reloads them when they are fields of a union (`halves.high`) or plain scalars. Splitting the union in C breaks `func_8013ACBC` (shared `lui at`) and the unit sha1. TAIIKU `func_8013CF88` is within 4 lines of the original when `D_80149974/78` and `D_80149968/6C` are separate scalars, but the overlay yaml has no `symbols_TAIIKU.txt`, so the second name cannot be declared next to the C-defined struct (draft in the T-10620 scratch directory, rows in [[data/t0018-cases]]).
- **Constant folding that the original does not do** (RPG_BAT `func_80132B34`): the original stores `x + 0x30` and `y - 0xB0` computed from registers that hold 0x84 and 0x6C; uopt folds them in every C form tried (locals, chain, store read-back).
- **Do-while word-clear loops** (RPG_BAT `func_8014EF8C`, `func_8014E994`, `func_8014F278`): start pointer in `v1`, end in `v0`, and a 24-record byte loop kept rolled in straight store order. IDO gives start `v0`/end `v1` (the pointer is a named local with a lower bit than the hoisted end), unrolls the record loop or rotates its stores. Not a register-allocation row alone: both loop kinds need a different source shape.
- **Open tooling points:** `funcdiff.py` printed `MATCH (relocations resolved)` once for TT `func_80149B20` while the machine was loaded (a first run that took more than ten minutes); the identical source later gave 62 differing lines and a failing TT unit sha1. Treat any `MATCH` as unconfirmed until `ninja` ends with the unit `OK`. `permute.py` refuses bases whose instruction count differs by one (seen four times here), which hides the near misses with a single missing instruction. TT `func_8014D260` (gcc `ratan2`) still needs the ASPSX division layout ([[gcc-objects]]).

## Wave 15, list 1 (T-10710, w15-1)

4 matches (1,068 bytes): main `menu_check_2`, GYOZI `func_8013E0A4`, TACO `func_8015CCC8`, ETC `func_8013E094`. About 17 guarded near misses (list in [[tickets/T-10710-wave-15-list-1]]); the rest are rows in [[data/t0018-cases]].

Idioms that worked:
- **Record view for the +0xF90 menu records.** `((GsRecF90 (*)[2])g->unk_F90)[i][arg0].unk_06` with `MenuSelection5` for `count`/`current` (the idiom of `func_80050324`) matches `menu_check_2` once `sel = -1;` is written before `g->unk_1095[arg0] = -1;` (first use fixes the bit order, so `sel` gets `$v1` and the pointer expression `$a1`). The same views give `menu_check_1` with identical structure and loop; there the K&R product `(d - u8field) * d < 0` must be `(s32)(...) < 0` (an unsigned product is folded away), but d/cond/cnt registers still differ.
- **Offset-first element address.** `i * 0x88 + (u8 *)D_8015EDB4` (written out at every use, no macro) gives `addu a1,a0,t5`; `D_8015EDB4[i]` and `i[D_8015EDB4]` give `addu a1,t5,a0` (TACO `func_8015CCC8`). This answers the regdiff hint "write the operands of the sum the other way round" when the sum is an array element address.
- **Twin of a matched function in another overlay.** GYOZI `func_8013E0A4` is EVENT `func_8010F4B8` with the overlay's own callee names (`dupes.py` cannot see it because the callee names differ); the queue flag `apply:` names the twin.
- **Two unused `s16` slots above a stack local** (ETC `func_8013E094`): a local `s16 base` that is only set in the first block of a function lands at sp+0x16, the original has it at sp+0x12; one unused `s32 pad;` declared first (marked FAKE) moves it by exactly 4 without growing the frame. The same trick failed for a `Lim3`/`MATRIX` local (TAIIKU `func_80147120`, TACO `func_8015B1B8`), where it grows the frame by 8.
- **Locals for pointer globals hoist their loads.** TT `func_8013D150`: `p = D_80158A64; q = D_80158A6C; r = D_80158A70; t = D_80158A74;` at the top gives the original's four hoisted loads (99 of 99 instructions); written through the globals each access reloads (126 instructions).
- **Pointer parameter types change the first instructions.** `initModelingData_init(s32 *arg0)` keeps `s3 = a0 + 4` as one instruction where `s32 arg0` computes it in `a0` first; the header declaration changes with it (`sync_protos.py --write`, and the alias `func_80058F0C` by hand).
- **Array-indexed stores let loop-invariant global loads hoist** (RPG_BAT `func_80153A60`): `D_8011ECD0[i * 0x44 + 0x1107] = D_8015EC74` with an `&D_8011ECD0[i * 0x44] != D_80120650` do-while bound gives the original's 41 instructions; a `u8 *p` loop reloads inside the loop.

Dead ends (rows in [[data/t0018-cases]]): the RotMatrix helpers (`func_8015D0D0`, DATE `func_8015A640`, `func_8015A980`) are hand-written libgte assembly (`addu` moves, `multu` + `nop nop`), see [[gcc-objects]]; every `lui at` pair that the original shares between D_8015EDC4/EDC8 or D_8015EDF0..EDFC needs the words as one C-defined struct, but the other RPG_BAT files read them as scalars; `InitMouse` needs a data range that `config/objects/main.txt` does not have for object 80059710; hoisting differences for scalars versus GameState fields (SHOUGATU `func_801347E0`, main `func_8006BA40`) change the base register of every field load.

Tooling notes: `funcdiff.py` builds without `NON_MATCHING`, so a guarded draft reports `MATCH` (the `INCLUDE_ASM` fallback); use `regdiff.py` (it compiles the guarded variant and prints `MATCH` for identical instructions) or remove the guard before trusting `funcdiff`. `put2.py`-style replacement of a guarded definition keeps the guard; a stale `#else`/`#endif` pair from an earlier replacement broke one build in this wave (caught by `ninja`).

## Wave 15, list 2 (T-10720)

Ticket [[tickets/T-10720-wave-15-list-2]]. Recipe that matched TAIIKU `func_80137090` (slot loop over `D_80149208`, stride 0x24):
- Reach the slot fields as members of the one defined object, indexed (`D_80149208.pad24[i * 0x24 + 6]`, `*(s32 *)&D_80149208.pad24[i * 0x24 + 0x18]`), not through a `u8 *p = (u8 *)&D_80149208 + ...` pointer. With the pointer IDO assumes the stores alias the s16 global `D_80120A72` and does not hoist its load; with member indexing it hoists the load into a1 as the original does.
- Stack layout: each *used* scalar local takes a 4-byte slot (unused ones are removed). Locals are placed top-down in declaration order, so `i`, `lim`, `th` then a throw-away `s16 y` (used once) reproduced the original's `sp+0x2C`/`sp+0x24` copies and the 0x40 frame. A `u8 f` copy of a loaded flag adds a register copy (`move a2,a0`); leave it out.
- Load order: `th.v[i] <= (y = ...)` puts the table load before the member load (`y >= th.v[i]` loaded y first).
- dupes.py with `--lenient --apply --check --files <list>` copied 4 matches (GEKO `func_8013E69C`, `func_8013E760`, SHOUGATU `func_801366A4`, `func_80141AE8`); the plain run skipped them for declaration differences that the build accepts.
Dead ends logged in [[data/t0018-cases]]: TT phantom a0/a1 constants (`func_8014A814`, `func_80147D08`, `func_80145370`), GameState field vs separate scalar promotion (`func_80136948`, `func_80137F48`), the `move s2,s3; addiu s2,s2,8` copy (TACO `func_801347F4`, TAIIKU `func_801406A8`). Tooling note: `tools/sync_protos.py --write` does not add a prototype for a newly C-defined main function that is called before its definition in the same file (main `func_8007BBE8` needed `void func_8007BBE8();` added by hand to main_api.h).

## Wave 16, list 1 (T-10810, w16-1)

4 matches (1,064 bytes): TAIIKU `func_80147210`, `func_80147120`; EN_NICHI `func_80134F2C`, `func_80135044`. Guarded near misses: EN_NICHI `func_80136A2C` (6 lines), main `func_80055A38` (10), TACO `func_8015B1B8` (8). Rows for the rest are in [[data/t0018-cases]].

Idioms that worked:
- **`for` with a comma-separated init decides the colour order.** TAIIKU `func_80147210`: `for (i = 0, v = D_8014A400, a = D_8011ECD0, d = D_8014A3DE; i < 3; i++, v += 0x1C, a += 0x44)` gives the original's allocation (i=v0, v=v1, a=a0, d=a1) and the original's init schedule (as1 schedules by source line). The same body as a do-while with the inits on separate lines gave the right instructions in the wrong registers for every statement order (`regdiff`: "unconstrained ranges are coloured in bit order", bit order is the order of first mention). An `(i + 1) * 8` operand is enough: do not add a separate `j += 8` counter, IDO strength-reduces it into the later-coloured temporary the original has.
- **A loop exit test written through the index: `slti` instead of `bne`.** `for (sh = 0; sh < 8; sh++) w += *b++ << (sh * 4)` keeps the original's `slti at,a2,32` (the `sh += 4` form turns the test into `bne` against a hoisted constant). Backward `goto` loops (`L: ...; if (i < n) goto L;`) are never unrolled, which is how main `func_8004F4F0` keeps its three nibble loops rolled (108 of 109 instructions; not matched, see the row).
- **Accesses through a struct array on the symbol, not through a local pointer, let loop-invariant loads hoist and keep the frame small.** TAIIKU `func_80147120`: `((Tk1C *)D_8014A400)[i].unk_14` for every access hoists `D_80120896` before the loop without a named local (a local `y` or a pointer `v` each add an 8-byte frame slot because every named scalar gets one). Only `i` and `lim` remain: frame 0x30, `lim` at sp+0x20.
- **Declaration order moves stack locals: later-declared sits lower.** TACO `func_8015B1B8`: declaring `i, j` before `matrix` puts the MATRIX at sp+0x20 (frame 0x48 unchanged). Main `func_8004F4F0` needs two scalars above the bit buffer and four below (`row, sh, bits[0x90], src, ...`).
- **The record pointer stays an expression when the original spills it.** EN_NICHI `func_80134F2C` / `func_80135044` use the file's `EN_OBJ(arg0)` macro at every access; a local `EnObj *e` gives frame 0x38 (the local keeps a home slot next to the spill slot at sp+0x28) where the original has 0x30. With the expression the pointer sits in `$v1` and is saved and reloaded around `func_800AE0D0` exactly as in the original. Statement order decides the ugen temporaries: `e->unk_28 = e->unk_08 * 2 + 4;` before `e->unk_2C = 0x80;` gives `lw t9` before `li t2` as in the original.
- **Dead initial assignments set the colour order.** Main `func_80055A38`: writing `n = 0; b = 0; q = ...; p = ...; r = ...;` at the top (the loop re-assigns them) changes which local is coloured first; it cut the register diff from 26 to 10 fields. Not plain C that survives review as a final form, so the function stays guarded.
- **`j` starts at 4 in unrolled-by-4 byte loops.** `for (j = 4; j < 0xF00; j++)` gives `li t0,4` and the original's `bne t0,t2`; with `j = 0` the counter starts at `move t0,zero` (the loop processes 0xEFC bytes, the last four bytes of the bank hold the checksum).

Dead ends: see the T-10810 rows. The RPG_BAT trio needs a header change: the build already generates interior data symbols for C-defined data (`build/ovl/RPG_BAT_data_syms.ld`), so only `include/ovl/RPG_BAT.h` (`extern s32 D_8015EDF0`/`D_8015EDF4`) blocks a struct definition. TT `func_8014D260` stays blocked on the ASPSX division expansion ([[gcc-objects]]).

Tooling notes: `regdiff.py --src` takes a path under the mounted scratch directory (`/scratch/<name>/draft.c` inside Docker); a scratch `rv.sh` that replaces the function's `INCLUDE_ASM` or its whole `#ifdef NON_MATCHING` block and runs regdiff made 5 to 10 s experiments possible. `permute.py` refuses a base whose instruction multiset differs from the target (it stopped on `li t0,0` against `li t0,4`).

## Wave 16, list 2 (T-10820)

Ticket [[tickets/T-10820-wave-16-list-2]]. Matched: VALEN `func_80132274`, SHOUGATU `func_80136078`, GEKO `func_8013A2EC`, RPG_BAT `func_801528DC`. Recipes that worked:
- **Switch dispatch shell** (VALEN `func_80132274`): same shape as GYOZI `func_80138784` (call, `switch (D_800E6280.unk_1109)` over four handlers plus `func_80046500`, then seven calls). dupes.py skipped it; a hand copy with the GameState field name matched at once.
- **Function-pointer table copy** (SHOUGATU `func_80136078`, GEKO `func_8013A2EC`): a local `FnTblN tbl` assigned from the overlay table, then `p = &tbl.f[idx]` compare/replace and `tbl.f[idx](0x80)`. Frame check: the original's table sits at `sp+0x34` / `sp+0x30`; a `void (**p)()` local and the double-test FAKE of `func_80134120` (T-4090: `if (!x && !x) {}`) give exactly that. For GEKO the decomp-permuter found the FAKE in the second `if` (permuter score 20 was a real match: scores of 20-25 can be 0 against the real build, always rebuild a candidate with `funcdiff` before dismissing it).
- **Unsigned view of a signed GameState field** (RPG_BAT `func_801528DC`): `unk_110C` is `s8` in `GameState` but the original reads it with `lbu`/`sltiu`. `u8 *p = (u8 *)&D_800E6280.unk_110C;` and `*p` everywhere reproduced the promoted value in $v1 (a `(u8)` cast on each use did not). The loop compare also needed the operand order the original has: `while (D_80120650 != &D_8011ECD0[i * 0x44])` (the other order gives `bne a2,v0` instead of `bne v0,a2`).
- **RPG_BAT `func_80153A60`** went from 41 register lines to 4 (kept `#ifdef NON_MATCHING`): same pointer view (`s32 *p = &D_8015EC74`), the swapped loop compare, and a FAKE dead local `dead = (*p >= 0x61)` with `else dead = 0` that holds $v0 so the pointer takes $v1 as in the original. Left: the original uses `slti at` and leaves $v0 unused.
- **Permuter reach** (`tools/permute.py`): `invalid base ... instruction shape differs` means the guarded draft differs from the target by instructions, not registers (TAIIKU `func_8013AE14`, `func_80137228`, `func_8013C780`, TT `func_80147FD0`, ETC `func_80143F24`, TACO `func_801347F4`, ...); fix the shape first. A candidate with a dead-compare local (`int dead; ... (dead = (cmp))`) is the permuter's standard way of reserving a register; TT `func_80145370` went 20 -> 4 lines this way (constants then sit in a1/a2 instead of a0/a1; a3 for 0x70 still missing).
- **dupes.py**: `--files` takes a comma-separated list. A space-separated list (zsh word splitting) silently scans nothing.

Dead ends and shapes, rows in [[data/t0018-cases]]:
- The four-function copy family (SHOUGATU `func_80136948`, GEKO `func_80137580`, DATE `func_80149F48`, SHUGAKU `func_80133038`): the original keeps the first load (an `s16`) in $v0 and the later loads in t5/t2/t1 (uopt colours). The permuter found a 22-line variant for GEKO (`u8 new_var` copy of the selector plus an empty test of `D_80144FA4`, kept under NON_MATCHING); the other three need the same trick.
- Base register of an argument load (SHOUGATU `func_801347E0`, VALEN `func_80133670`, DATE2 `func_80136794`, EVENT `func_8010B614`): the original emits `lui rA; lbu a0,0(rA)` with the base in a different register, the GameState view emits `lui a0; lbu a0,3935(a0)`.
- Frames: TACO `func_8013E424` (signature `(SVECTOR v, s32 c)`, frame 0x60 with 0x30 bytes of other locals) and GEKO `func_80144198` (no `sw a0` home store in the original; a 24-byte local table copy always gets one) keep their frames in INCLUDE_ASM.
- The three DATE/TACO 416-byte rotation helpers (`func_8015A7E0`, `func_8015CF30`, `func_8015D270`) are the hand-written libgte routine ([[gcc-objects]]), not attempted.
- Loops: ETC `func_80143758`, TT `func_8014A814` (phantom a0/a1 constants), TACO `func_80147B24` (index and pointer forms both 91 of 91 instructions, 92 differing lines of register choice), TAIIKU `func_801406A8`/TACO `func_801347F4` (`move s2,s3; addiu s2,s2,8`, now with six C variants for the shared base, none changes it).

## T-10730 big-function batch (a15)

Three integrated matches add 6,044 text bytes: `read_bustup`,
`func_8006F524`, and `func_8006FB4C`. The complete Docker build verified
27/27 units after integration. Current retained near misses are
`func_8007EF48` (three scheduling pairs) and `func_80082764` (two commutative
addition operand orders), both below thirty funcdiff lines.

All 21 assigned functions received an initial draft, regdiff and type/control
flow iterations. The broad drafts remain in `scratchpad/a15/`; the exact
seed names and per-function evidence are in `scratchpad/a15/HANDOFF.md`.
See [[decompile-workflow]] for shared dialogue, choice-row, motion-record,
bitfield, frame and selector idioms. No T-0018 rows are added: the retained
gaps are scheduling or operand ordering, and the broad drafts still have
source-shape differences rather than an isolated allocator failure.

DATE `func_8013272C` now passes its complete overlay SHA1 in an isolated
scratch build (`b5fb83738364a317a590735578f5878077fcda3d`). DateFlag17 fixed the
last temporary gap. A DateSelectionView over byte slots at D_800CA2F8/+4
then fixed one comparison operand swap that only appeared in the complete
source file. The unsigned-load pass tracks rewritten global locations across
procedures; this is a concrete reason to verify complete objects/units.
Its inherited void prototype conflicts with the recovered signed result, and
the append-only exception remains pending. `scratchpad/a15/date-match-pending.diff`
contains the exact body and one-line prototype correction. Source stays
INCLUDE_ASM, so this fourth verified scratch match is excluded from the
three integrated matches and progress total above.

Full-file NON_MATCHING verification at the final review gives eight resolved
funcdiff lines for func_8007EF48 and six for func_80082764. The latter includes
one load-scheduling pair in addition to the standalone addition operand-order
pairs. Both compile with all existing guarded drafts enabled and satisfy the
retention threshold.

## Big functions 3 (T-10830, a16)

Ticket [[tickets/T-10830-big-functions-3-astra]]. Accepted: all 29 assigned
BUNKASAI functions, 36,716 official progress bytes. The four owned BUNKASAI
objects pass real resolved funcdiff and the complete 27-unit checksum build.
Progress is `5215/6958  1003464/2279368  44.0%`. No new T-0018 rows: the
remaining cases still have structural, access-width, declaration, or data
ownership differences rather than an isolated compiler register-order proof.

Reusable findings are in [[decompile-workflow]] under T-10830. Appended
DateMovieWork and DateMovieHeader8 to DATE.h, EventMovieState to EVENT.h,
and four later-defined scene-helper declarations to BUNKASAI.h. The matching
BUNKASAI idioms use stored-member call arguments, the existing return-flag
bit fields in target store order, and Rec38 bit-field accesses. No new FAKEs
were needed in the 29 accepted bodies. The 80143770 object keeps a justified
K&R call override for func_80083440, whose callee narrows the argument.

Eight compile-checked research seeds are retained in scratchpad/a16/handoff/.
Its README distinguishes real diffs from normalized rankings, valid corrected
seeds from numerically closer but unfaithful experiments, and the inherited
13-line bounce guards from broad drafts. Do not paste the smaller movie
handlers without reconciling their inherited void prototypes; observed zero
returns were hidden by that declaration view. The bounce ownership probes
need data-island/YAML integration outside this batch and used dummy data
initializers only to investigate code generation. Keep those probes out of
source. EVENT 8011C750's omitted empty state-10 case was recovered from its
jump table; all six large movie draft outer case lists were audited.

The exploratory permuter's assignment-splitting pass can change
`phase = phase + 1` to a transition to 2. That candidate is rejected even if
its search score improves. The pass is disabled in subsequent a16 scratch
searches; project tools are unchanged. All bounded movie searches completed
without another acceptable source candidate. The final checks and inline
standards review pass; the ticket is Done after more than four hours of work.

## Wave 17, astra easiest-first (T-10850, a17)

Five literal-byte matches (936 bytes): EVENT `func_80104DB4` (176), GYOZI `func_80137E84` (196), KANGEI `func_80136970` (180), OPTION `func_80139878` (148), SHOUGATU `func_8013DDD0` (236). Final totals and verification are recorded in [[tickets/T-10850-wave-17-astra]]. All 69 assigned functions have a compiling draft and resolved diff in `scratchpad/a17/attempts/`.

- GYOZI's copied text table is a 0xCC3-byte aggregate. Declaring the scalar girl index before the aggregate reproduces the frame and copy sequence without padding tricks.
- SHOUGATU copies a 23-entry callback table. A typed 0x44 counter record (byte +7, signed halves +8/+0x16/+0x18) allows the original load ordering; the implicit-int/no-return form preserves its epilogue.
- KANGEI's final operand swaps need a shared unsigned constant and a separate byte local for the post-call selector. Merely reversing the source comparisons did not work. The constant steering is marked FAKE.
- EVENT's six record-flag updates match with the loop header and its preceding store on one source line. That scheduling dependency is marked FAKE. Removing m2c's spurious arguments to `func_80011DFC` is also necessary.
- OPTION's byte update needs an empty conditional between the raw sum and its truncated uses. It separates their live ranges and is marked FAKE. Removing the test, using an empty scope, chaining the stores, or capturing the assignment loses the match; the permuter's float cast and unused constant were unnecessary and removed. The full OPTION checksum passed.
- TAIIKU's four contiguous 0x1C slot records at `D_8014A454` are all zero-initialized owned data. A 0x70-byte C definition preserves the overlay checksum and removes the repeated `$at` loads from its tick updater. The remaining loop-bound/register scheduling miss is kept separately from this confirmed data-layout result.
- For register diagnostics, use a scratch translation unit that disables unrelated NON_MATCHING guards; this makes the traced function correspond to the default-build candidate being compared. The new function-level harness also checks that the installed source stayed unchanged during verification.

Known unchanged blockers: `InitMouse` and `menu_bar_color` need owned data outside the available object data ranges; the three DATE/TACO rotation helpers are handwritten SDK assembly; TT `func_8014D260` retains the documented ASPSX division-expansion gap. No object configuration or tool files were changed.

Final near-miss retention: main byte checksum and GYOZI counter each differ by four listing lines; TAIIKU tick updater also differs by four. ETC selector improved from 24 to 14 lines; simplifying its marked zero-valued comparison loses that improvement. The extra owned GYOZI sprite initializer improved from 101 to 19 lines with a documented 0x44 record view. The menu helper keeps its compatible s8 return because s32 breaks its matched caller. Thirty-four case rows record these and other remaining patterns in [[data/t0018-cases]].

## Wave 17, list 1 (T-10910)

Ticket [[tickets/T-10910-wave-17-list-1]]. Matched (4 functions, 1,476 bytes): RPG_BAT `func_80153A60` (168), TT `func_8014A814` (228), TT `func_80147D08` (524), TAIIKU `func_801339C4` (556). All four came from long `tools/permute.py` runs on guarded near-miss drafts, then a manual strip of the permuter noise. The recipes are FAKE-tagged in the sources.

What worked:
- **Long permuter runs (900 s, `-j 3..4`) on guarded near misses with a base score of 25-45** found the answer where 240 s runs and a day of manual variants had not: RPG_BAT `func_80153A60` (base 45 -> 10, verified MATCH), TAIIKU `func_801339C4` (30 -> 10, MATCH), TT `func_8014A814` (110 -> 0, verified by the tool). A candidate with score 10 is a real match when the only leftovers are the permuter's stripped-unit effects; always rebuild it and run `funcdiff --resolve`. Bases above ~300 (loads in another order, frames) did not converge in the same time. `permute.py` cannot see data the C file defines itself, so its base score for such functions is meaningless (TAIIKU `func_801368B8` showed 6760 for a 2-row diff).
- **Phantom a0/a1 constants (TT `func_8014A814`, `func_80147D08`)**: a dead pair inside the loop, `new_var = D_80158A80 && D_80158A80;` early and `if (new_var && D_80158A80) {}` a few stores later, reserves two registers without emitting code, so the hoisted constants start at a2 as in the original. Both statements are needed, neither alone nor a single `if (G) {}` works. In `func_80147D08` the second loop needs its own pointer (`q`), otherwise the pointer shares $v1 with the first loop's `p`. The loop counter must stay shared between the two loops, or loop 1 turns from `slti at,v0,3` into `bne v0,t4`.
- **Dead copy + empty branch (RPG_BAT `func_80153A60`)**: `(dead = *p) >= 0x61` keeps the compare as `slti at`, and an `if (0) {}` right before `i++` moves the table pointer to $v1.
- **Selector through a pointer (TAIIKU `func_801339C4`)**: `sel = *(ptr = &D_800E6280.unk_F80) & 0xF000; switch (sel)` puts the selector load into $v1 (the "switch selector $v1 vs $v0" T-0018 shape).

Levers found but not finished:
- **Own data fixes the copy-chain family's schedule.** DATE `func_80149F48`, GEKO `func_80137580`, SHOUGATU `func_80136948` (and SHUGAKU `func_80133038`) copy globals the object itself owns. After `tools/data_island.py <UNIT>/<addr>` and C definitions for the twelve (six) variables (their initial words are the pointers 0x801B9DA0/0x801B9EA8/0x801BA2C8 and zeros), the instruction order, the shared `lui at` and the interleaving all match (43 of 43 instructions, only registers differ); with the extern declarations every variant tried was out of order. With `new_var = F5F; D = new_var; D = D * 3 + k;` on the destination the first copy also gets $v0. What remains is one puzzle: the original loads the first of the three "constant pointer" globals into the uopt colour t5 and the other two into ugen t6/t7, and keeps the product in a uopt range (sll t9; subu t1); every statement order (630 orders), local, dead-pair and type variant tried (3,000 compiles) keeps the first of the three as a ugen temp.
- **Frame homes for promoted locals** (BUNKAKEN `func_80145550`, GEKO `func_80144198`, TAIIKU `func_80139FE4`): the original's local tables sit 4-16 bytes lower than ours. In exp files a pad declared before the scalar (`u8 q[4]`) shifts them exactly, but in the full function two `+= 2` statements raise the frame by 8 again; GEKO `func_80144198` gets the frame right with `s32 pad[2]` before the scalars and the table last, but keeps the `sw a0,64(sp)` home store the original does not have.
- ETC `func_80143F24`: a 280-point base reached 40 after 900 s (chain assignment `((u8 *)base)[i*0x24+0x14] = ... [0x15] = ... [0x16] = D_800E6280.unk_03A` through a `s32 *base` local): the value now lands in $a1 as in the original, 24 lines left (pointer $v0 vs $v1, the `lui v0` of D_80150138 not hoisted); kept as the older guarded draft.
- Near misses kept guarded (INCLUDE_ASM fallback): main `func_800764AC` (24 lines), DATE `func_80149F48`, GEKO `func_80137580`, SHOUGATU `func_80136948` (14-15 lines each). Everything else on the list got its m2c draft, a build and a funcdiff; the three libgte rotation helpers (DATE `func_8015A7E0`, TACO `func_8015CF30`, `func_8015D270`) were re-read and are hand-written assembly, not compiled C.
- Tooling: `permute.py` scores for functions that need the object's own data defined in C are unreliable (TAIIKU `func_801368B8` base 6760 for a 2-row diff); `funcdiff` row counts are in rows, `--resolve` lines in instructions. With the machine at load 40 a 3,000-compile exp grid takes hours: keep grids under ~600 compiles.

## T-11020: big functions (Sonnet) 25

Ticket [[tickets/T-11020-big-functions-sonnet-25]]. Lists: DATE/80132000, EVENT/8010FE10, `src/main/8007C030.c`, `src/main/8006CB30.c` (47 functions). 11 matched (7,520 bytes): EVENT `func_80111F04`, `func_80112460`, `func_80112930`, `func_8011220C`, `func_80113A40`, `func_80113FD4`; DATE `func_8013426C`, `func_801344C8`, `func_8013518C`, `func_80135C14`, `func_801349A0`. Near misses kept under `NON_MATCHING`: DATE `func_8013AF1C`, `func_8013B208`, `func_8013B488`, `func_8013D5FC`, `func_80140374` (5 instructions), main `func_80072D70`.

What worked (the scene-setup family of EVENT and DATE, each a girl-index table walk):
- **Girl index as a struct member.** EVENT: `g->unk_EE6` of `EventStateView *g = (EventStateView *)&D_800B0860;` instead of `D_800B1746` (same address); DATE: `g->unk_F5F` of `GameState *g = &D_800E6280;`. The `* 0x34`, `* 9`, `* 0xC` strides then come out as `multu`/shifts exactly like the original.
- **Record byte through a pointer built from the base.** EVENT `u8 *r = D_800B0860 + g->unk_EE6 * 0x34;` with `r[0x1BC]`/`r[0x1BD]` (one `addu v0`, offsets in the load); `g->unk_1A4[..]` folds the 0x1A4 into the base and does not match. When the pointer is used by one block only, write the second read as a fresh `D_800B0860[g->unk_EE6 * 0x34 + 0x1BE]` (EVENT `func_80112460`).
- **Stop uopt promoting a global: read it through an address.** `((DateSelectionView *)&D_800CA2F8)->option` for D_800CA2FC (DATE `func_801349A0`) and `*((u8 *)&D_80125318 + 4)` for D_8012531C (EVENT `func_80112930`) leave the value in a ugen temporary (`t8`) that stays valid over the table stores, as in the original; the plain global was promoted to `a0`. Same trick for `*(u8 *)&D_8015B68C` (a `lbu` each time instead of `lb; andi`). Order inside such a function matters: `D_80122D08 = 0xA;` before `temp = sel * 3;` moves a temporary.
- **Duplicated stores.** `D_800EECE4 = 0;` after an inner if/else must be written in both arms when the original has `sw zero` in two places (EVENT `func_80111F04`).
- **Mixed record/target blocks.** EVENT `func_80113FD4` is `func_80113CA0` with other constants: copy a matched twin and change only the constants.
- **A main-exe global matched as another type.** `D_800CA158` is read as `s16` in DATE: `#define MAIN_API_OVERRIDE_D_800CA158` at the top of DATE.h, `extern s16 D_800CA158;`, and the guard around the main_api.h line (what `sync_protos --fix` writes; that run also edits unrelated files, revert those).
- **Chain assignment order.** `a[0] = a[1] = a[2] = 4;` stores in the order the original has for DATE `func_8013D5FC` (22C/22E/230 rows share one `li`); as1 does not keep source order for the chained stores, try all six permutations (script in the scratchpad, ~15 s each).
- **A store after an inner if appears twice** (once in the branch delay slot, once in the fall-through): a single `g->unk_110D = n;` after `if (n++ == 0) {...}` is right, not two stores.

Locals and frames (new, measured):
- IDO reserves a stack slot for every declared local, also for ones kept in registers or dead. The first declared local is the highest, so an original that has the escaped `s16` at `sp+0x32` with 4 bytes above it has one more local declared BEFORE it (EVENT `func_80118010`: `u8 temp_v0; u8 pad[3]; s16 sp32;`, marked as frame padding in the near miss notes, not kept). A unused `s16`/`s32` before the escaped one moves it down by its size; an 8-byte array `s16 x[4]` rounds the frame up instead.
- A variable that lives across a call but not the original's value (`a0 = g->unk_F5F` reloaded after a call) must not be a local: DATE `func_8013D5FC` frame went from 0x30 to 0x28 by reading `g->unk_F5F` again.

Open (kept in `NON_MATCHING`):
- DATE `func_8013D5FC`: 39 register-only differences; the constant 8 is hoisted twice (a3 for the compares, a2 for the call argument) where the original has one `a2`, and `n` takes `v0` where the original has `v1`.
- DATE `func_80140374` (5 differing instructions) and EVENT `func_8011BE00`'s twin: the hoisted constant 2 (s6) is compared as `(var, s6)` and the fourth call argument is `andi a3,s6,0xff` in the original; every spelling of the constant (`2 & 0xFF`, `(u8)2`, a local) gives `(s6, var)` and `li a3,2`/`move a3,s6`. A permuter run (240 s) found nothing.
- DATE `func_8013B488`, `func_8013AF1C`: register order of promoted globals (`unk_75C` and `D_80122D08` in v1/a0 instead of v0/v1; the three u8 fields 0x3E/0x3F/0x40 swapped).
- DATE `func_8013B208`: the original frame has the two escaped `s16` at 0x28/0x2A with 4 bytes above them; no declaration order found.
- Not attempted past a draft: EVENT `func_80118010` (twin of `func_8013B488`), `func_80119110` (the original keeps `idx * 0x34` as three `sll/addu` sequences while ours hoists the 0x34 into a register once more than two products exist), `func_8011C164` and DATE `func_801407D0` (record pointer splits into `v0`/`s0`), DATE `func_8013C190`/EVENT `func_80118B4C` (the base of `D_800CA22C` is a pointer kept in `s0` in the original; a `p = D_800CA22C` local is folded away), main `func_8006D138` (the original loads `D_800E6280` into `a0` and the flag word into `v0` before the switch).
- DATE `func_80132000`: starts with a one-nop pad (known); the 40 stores compile.

## Big functions (Sonnet) 24 (T-11010)

Ticket [[tickets/T-11010-big-functions-sonnet-24]]. List: TAIIKU `80138950`, NAME_ENT `80139740` and `80132000`, main `800674B0` (60 functions, 65,920 bytes). Matched (7 functions): NAME_ENT `func_80139CB4`, `func_801387F4`, `func_80137FC4`, `func_80142028`, `func_8013D054`, `func_8013B77C`, `func_801372A0`. Guarded near misses kept (INCLUDE_ASM fallback): NAME_ENT `func_8013F208` (6 lines), `func_80135BB4` (13), `func_8014231C` (28), `func_80135168` (19), `func_801357F4` and `func_80135F74` (23), `func_8013BB24` (88 lines but only instruction order), `func_801377E8` (40, hi-constant register).

What worked:
- **The list's functions are mostly big only because of repeated text-UI calls.** The seven matches were direct m2c translations once the existing views were used: `NameMetricWork` / `NameRecordF00` (`include/game.h`) for the album records at `D_800E7D10` / `D_80123120`, `D_800E6280.unk_xxx` for the old `D_800E7xxx` scalars (`D_800E738D` is `unk_110D`, `D_800E7398` is `unk_1118`, `D_800E7397` is `unk_1117`, `D_800E7208` is `unk_F88`), and `tools/docker.sh python3 tools/m2c.py --unit NAME_ENT`.
- **`unk_110D++` instead of `+= 1`, with an `s32` return and no `return`** (NAME_ENT `func_80142028`): the original keeps `(x + 1) & 0xFF` in `$v0` (`andi; move v0`). `+= 1` gives a plain `addiu; sb`. Without the `s32` return the `++` form is not enough; the caller ignores the value.
- **`u8 state; (state = D_800E6280.unk_1115) == 0xF0 ... state == 0xD0`** (NAME_ENT `func_8013D054`): the shared post-call selector copy, as in the register cheat sheet; `printf` with `state` takes the same register.
- **A signed field read as unsigned** (`unk_F6C` is `s8`, the original does `lbu`): `*(u8 *)&D_800E6280.unk_F6C == 2` (NAME_ENT `func_8013B77C`).
- **Old-style definition for a function whose header says `func()`** (NAME_ENT `func_801372A0`, `func_80134B94` attempt): `void f(arg0) s32 arg0; { ... }` matches the K&R header prototype and the `(a + 1) % 10` / `/ 10` code; keep three `s32` locals for the three spilled date values.
- **Statements that rewrite the same globals as a record array** (`func_8014231C`): three scalar-looking bytes `D_80121247`, `D_8012128B`, `D_801212CF` are `D_8011ECD0[9591]`, `[9659]`, `[9727]`. Writing them as elements of the record array stops as1 from hoisting the loads over the stores (same-symbol store/load dependency).

Levers found but not finished:
- **Global promotion competing with the selector** (NAME_ENT `func_801357F4`, `func_80135BB4`, `func_80135F74`): the original reloads `D_8014CC70` into `$v0` (unpromoted, same register as the `unk_F88` value, which is dead in that branch). Our uopt promotes the global because it has 3 uses and 2 defs over 4 units, and gives it `$v1`; regdiff shows the tie. In `func_80135BB4` a local `cur` for the first reference gets the register right everywhere except this one value (13 lines). A 300 s permuter run improved the base from 335 to 160 without a zero.
- **Constant in the same register as the dead selector** (`func_801357F4`): the original materialises the stored constant 2 into `$v0` (the branch compare's register) with a `nop` delay slot; ours uses an ugen temporary `$t7`. Everything after it shifts by one temporary. No source form found.
- **Frame 8 bytes larger with a jump-table switch** (`func_8013F208`, buffer `sp30[0x108]` FAKE): the same buffer without the switch sits at `sp+0x30` like the original, with the switch at `sp+0x38`.
- **Hi constants and the loaded value in one register** (`func_8013BB24`, `func_801377E8`): the original loads with `lui a1; lbu a1,lo(a1)` (macro form). A loaded value that is also stored to a global and then passed to `printf` must be written as `D_x = *(u8 *)&field; printf(fmt, *(u8 *)&field);` (two reads, the second is forwarded). Storing then passing the stored global promotes the destination and takes `$v0`. In `func_8013BB24` the registers match; the 16 swaps left are the order of `lui a0,string` / `lui at,store`.
- **Loop shapes**: the original's `i < 13` byte-count loops are pointer loops (`p[0x14]`, bound `&sym`), not index loops (index loops unroll). A do-while with `p` incremented last gives `bne v0,v1` bounds; `!= end` on unrelated symbols makes IDO emit a trip-count `divu` (use an index loop with a known count for stride 0x110).
- **Not attempted in depth**: the 15 largest functions of the list (more than 1 KB each: TAIIKU `func_8013EC60`, `func_8013E1FC`, `func_8013F3A4`, ..., NAME_ENT `func_80139F30`, `func_80140F90`, `func_80141820`, main `func_8006A2CC`, `func_800696DC`, `func_80067F04`). `func_80068BE4` (main) has the right control flow and local array order but its frame sits 8 bytes lower than ours (locals declared first-to-last land top-to-bottom: `saved`, `sp10C`, `spCC`, `sp8C`, `half`, `phase`); the original has an 8-byte hole above `saved` that no plain local reproduces. TAIIKU `func_8013C978` fails on the known switch-selector register (`state` in `$a1` against `$v1`).

## T-11030: big functions (Sonnet) 26

Ticket [[tickets/T-11030-big-functions-sonnet-26]]. Same four files as T-11020 (DATE/80132000, EVENT/8010FE10, `src/main/8007C030.c`, `src/main/8006CB30.c`). Matched: main `func_80072338` (660 bytes) and DATE `func_8013CDF8` (892 bytes). Near misses under `NON_MATCHING`: main `date_sasoi_judge` (10 insns, v0/v1 tie of the 03F and 040 ranges), DATE `func_8013C7D0` (2 insns, order of two `lui`).

What worked:
- **Promoted global that must not be promoted.** main `func_80072338`: reading `unk_03F` as `*(u8 *)&D_800E6280.unk_03F` everywhere stops uopt from promoting it to v1 and leaves the cvt temporary in v0 (the m2c `var_v0` is just that). The `D_800E6376` byte update is `*(s8 *)&unk_0F6 = (u8)unk_0F6.h & 0xFFFE;` (a plain `*(u8 *)&x &= ...` makes the address `addiu v0,v0,0` shared between load and store; the narrowed halfword read does not). `func_8007259C` needed a one-line forward declaration in main_api.h (defined later in the same file).
- **`g->unk_1104.u++ >= 0x400U`** gives the `sltiu; xori` compare of the old value; a plain global with no local (`g->unk_110D == 0 ... g->unk_110D++`) keeps the promoted v0 of m2c `var_v0` (a `u8 v` local gives t-temporaries instead). A `|= 0x10` on a flag byte is `unk_0C.f.b4 = 1;` (bit-field store, one ugen temp fewer); the `& 0x10` test is `.f.b4`. `unk_54C[0].w & 0x20000000` is `((ScheduleEntryFlags *)&g->unk_54C[0])->active` (one ugen temp fewer than the mask).
- **Chained flag views.** `ScheduleClubFlags.unk_25 == 3` tests the 2-bit field at bits 25-26 of a `GsRec66C` word and `= 3` stores it as `ori 6` on byte +3 (DATE `func_8013C7D0`); a new `DateFlags0F4` view (bits 19 and 20 of the word at +0xF4) is in DATE.h.

Open:
- DATE `func_8013C7D0`: the original reads `D_800E69A8` as its own symbol; as1 orders the two hoisted `lui` by symbol, so the draft has `lui D_800E6280` before `lui D_800CA2F8`.
- Frame puzzle (EVENT `func_80117D80`, DATE `func_8013B208`): the original has a pair of `s16` at sp+0x28/0x2A in a 0x30 frame. `s16 a, b` alone puts them at 0x2C/0x2E; a 4-byte local above them grows the frame to 0x38 only when the function also has the long `D_800B1746 == k` chain (the draft without the chain gives exactly 0x2A/0x28 in a 0x30 frame; `int pad; short a; short b` in a scratch file). So the extra 8 bytes come from the chain itself (hoisted constants?), not from the locals.
- EVENT `func_80119110`: with three or more `idx * 0x34` products IDO hoists `li 52` and uses `multu` whatever the C spelling (struct-member index, `(s32)` cast, `D_800B0A04[idx]`, reads through `*(u8 *)&D_800B1746`); the original has eight shift sequences. Measured: one or two products give `sll/subu/sll/addu/sll`, three give `multu`.
- DATE `func_801407D0` and EVENT `func_8011C164` (twins of `func_80140374`): with `p[98]` indexing (`p = (View *)D_8011ECD0 + i`) the pointer is in s0 with offsets 6700/6667/..., and the instruction set equals the original; only the load order inside the loop body differs (statement order has no effect, as1 reschedules; 12 permutations gave the same bytes). EVENT needs a signed-halfword view of the word at +0x28 (`lh`/`slti` and `li -120`).
- DATE `func_8013C190`: the original keeps `&D_800CA22C[0]` in s0 across the calls; a `p` local, pointer arithmetic or byte arithmetic on the symbol are all folded back to `lui/lh %lo` (frame 0x50 instead of 0x60).
- main `func_8006D138`: the original loads `D_800E6280` into a0 and the +0xF64 flag word into v0 once before the switch (speculative hoist), with the other fields as separate symbols; the draft with `F64Flags` bit tests has the right shape but no hoist.
- DATE `func_8013A94C`: frame 0x78 against 0x70; the original has 16 unreferenced bytes below the flag byte, the draft 28.

## TT overlay (T-11000)

Fork contribution, two parallel lists over disjoint TT files (issue CosmicScribe64/tokimemo-ps1-decomp#2). 12 functions
matched, 13 guarded near misses, 16 rows in [[data/t0018-cases]]. Ticket [[tickets/T-11000-tt-overlay]].

- **Narrow K&R parameter called in a loop.** Callers of the K&R `func_80133C1C(o, kind, idx)` emitted `li a1,CONST`
  where the original has `andi a1,sN,0xFFFF` (the hoisted case constant, narrowed). Making the definition itself ANSI
  (`void func_80133C1C(TtActor68 *o, u16 kind, s32 idx)`) fixes all three loop callers (`func_80133FB0`, `func_801342CC`,
  `func_80134630`) and leaves the callee's own bytes unchanged. A separate ANSI prototype beside a K&R definition is
  rejected by `check_headers.py`. `(u16)` casts or a `u16` local give `move`, not `andi`.
- **Single-case switch vs `if`.** `if (kind == 0xA8)` hoists 0xA8 into a saved register; `switch (kind) { case 0xA8: ...
  break; }` compares through `$at` and keeps the register free (`func_80134178`).
- **Unsigned store shares the switch constant.** Storing the case constant into a `u16` field (`*(u16 *)(c + 2) = 0xA0;`)
  shares the register of the `u16` selector's compare (`li a0,160; beq v0,a0 ... sh a0,2(v1)`); an `s16` store gets its own
  `li` (`func_801365C4`).
- **Stack layout by declaration order.** IDO gives locals homes from the top of the frame down in declaration order, and
  every declared local reserves a word even when it lives in a register; spill slots of promoted variables land below.
  Reorder declarations, drop or add locals (pads marked `FAKE`) and read the `sw`/`lw` offsets in `funcdiff` to place a
  slot: `func_80137878` (`c, b, pad[8], pt, p, i, n`), `func_80141A1C` (leading `s32 pad` gives the 72-byte frame),
  `func_8014CDBC` (`m, buffer, q`), `func_801365C4` (`c, v, t, s`).
- **`n` loop-bound local.** `for (i = 0, n = 12; i != n; i++, p += 0x54)`, with `n` assigned in the for-init, makes IDO
  hoist the constant 12 into an s-register instead of `&D_xxxx` (`func_80137878`). Assigning `n` before the loop spills it;
  a plain `i != 12` does not hoist it.
- **Stores are scheduled per source line.** Writing several packet stores on one line reproduces the original order
  (`func_80148244`: `x, y, u, v, clut, tag`; `func_80141A1C`: `len` and `code`). Keep a comment saying why.
- **Field expression vs named cursor.** Using `g->cmd[...]` and `g->cmd += 6` directly (new `TtCmdStream` over
  `D_80158A70`) makes the `*(g + 4)` load an s0 expression temp reloaded after each call, as in the original; a named
  `u8 *cmd` local moves to $a0/$v0 and changes the whole allocation (`func_8014C2F8`).
- **Callee return types.** m2c's "unset register $v0" after a call means the callee returns a value: `func_8014D260` (gcc
  SDK atan2) returns `s32`; with the fixed prototype `func_8013B928` matched and `func_801349B0` came within 7 lines.
- Smaller forms: `x[0x52] = 0; v = x[0x52];` gives `andi v0,zero,0xff` where `v = 0` gives `move`;
  `*(u8 **)(buf + 4) += 0x30;` after reading the field keeps the value in $v0; `D_80158A70` is `s32 *`, so byte offsets
  need `(u8 *)` first; pool loops `o = func(o, o + 0x3400)` reuse the previous result as the next base.
- Tooling: every `include/ovl/TT.h` edit rebuilds all TT objects; iterate with `funcdiff` and run the full `ninja` once
  at the end. The permuter helped only with pure scheduling (`func_80148244`); for frame and hoisting problems fix the
  layout by hand first.

## T-11040: big functions (Sonnet) 27

Ticket [[tickets/T-11040-big-functions-sonnet-27]]. Lists: `src/ovl/TAIIKU/80138950.c`, `src/main/800674B0.c`, `src/ovl/NAME_ENT/80139740.c`, `src/ovl/NAME_ENT/80132000.c` (53 functions). Matched: TAIIKU `func_8013AE14` (232 bytes, a leftover near miss). New guarded near misses: main `func_8006B648` (10 lines), TAIIKU `func_80139FE4` (register order only), NAME_ENT `func_8014231C` (28 lines down to 4). The rest of the list stayed `INCLUDE_ASM` (see the t0018 rows and the notes below).

What worked or was learned:
- **A stored value forwards into the next expression.** TAIIKU `func_8013AE14`: `D_80120668 = D_80149988.unk_00;` followed later by `D_80120668 = D_80149988.unk_00 + 0x48;` gave the ugen temp `$t9`; `D_80120668 += 0x48;` (uopt forwards the stored `$v0`, so the add is `addiu t1,v0,72`) matched. Try the compound assignment when only the temp number of a `value + k` differs.
- **Fixed-stride record loops are IDO-unrolled index loops.** NAME_ENT `func_8014231C`: a "2 x 4 fields" body with `i * 0x110` and four constant offsets (9591, 9659, 9727, 9795) always compiled the loads in ascending order, whatever the source order (all 24 permutations). The original is eight records of stride 0x44 (flag byte +3): `for (i = 0; i < 8; i++) D_8011ED9C[i * 0x44 + 9591] &= 0x7F;` makes IDO unroll by four and gives the original order (9659, 9727, 9795, 9591 loads; reversed stores) and the `bne v0,v1` end test. Look at the offset spacing first: constants 0x44 apart are one record array, not four fields.
- **Local layout (confirmed again, frame_pass).** Arrays sit lowest in the locals area: scalars declared before them go ABOVE them (main `func_8006B648`: `s32 first; s32 i;` before four `s16[12]` arrays moved the arrays down by 8 and gave the original slots), unused scalars are dropped, an unused pad array is placed below the arrays and only grows the frame. Declaration order among arrays does not matter.
- **Little-endian bit-fields.** The PSX build allocates bit-fields from the least significant bit: bit 1 of a word is `struct { u32 low : 1; u32 f : 1; u32 rest : 30; }` (`sll 30; srl 31`). Declaring the 30-bit field first reads bit 30.
- **Reload instead of promotion.** TAIIKU `func_80139FE4`: a global reloaded after a store in the original (`lh` after `sh` for `D_80120676`) needs the record view (`((DisplayActor44 *)D_8011ECD0)[96].unk_26`) in the C, the plain global is promoted to a register and a `move` appears. A `s16 pos;` local assigned in each branch gives the original's `sll/sra/move v1` per branch.
- **Not solved (do not spend time):** TAIIKU `func_8013A6FC` (`li a1,k` before `lui at`, already known), `func_8013CF88` (store scheduled into a branch delay slot, permuter 300 s found nothing), NAME_ENT `func_8013BB24` (`lui a0,string` / `lui at,store` order, permuter 300 s), `func_8013F208` (buffer at sp+0x38 instead of 0x30 whatever the pad), `func_8013DF88` (file index takes `$a2`, a 0.67 against 0.5 priority tie that the "selected slot" range wins), `func_80135BB4` (global promoted to `$v1`), main `func_80068938` (`D_800B3748 + idx*4` reload: uopt keeps the sum in a register or spills; `s32`, `u8 *` and `void **` views all failed) and `func_8006A044` (the original keeps `sb v0,F65` in every case; the draft drops the repeated store, 141 against 161 instructions).
- **Tooling note.** `include/ovl/EVENT.h` declares `D_8012069C` as an overlay-local `s16`; adding the main-exe declaration (needed to match main `func_80068EC0`) makes `check_headers` demand deleting the EVENT line, which this batch was not allowed to do. `func_80068EC0` (616 bytes) has an m2c draft (three switches on `(t >> 6) & 3`, loop `i = 0x62; i != 0xA0; i += 2` over `D_8011ECD0` records) that was written but never built for this reason.

## T-11050: big functions (Sonnet) 28

Ticket [[tickets/T-11050-big-functions-sonnet-28]]. Same four files as T-11030. No function matched; no source change kept. Findings for the next attempt:
- EVENT `func_80114C5C` (shop price/row table, 1756 bytes) reached 28 structural and about 230 register differences (423 of 432 instructions). What fixed the shape: read the shop index as `g->unk_EE6` with `g = (EventStateView *)&D_800B0860` (a plain `D_800B1746` gets CSE'd products, `g->` gives one `multu` per statement as in the twin `func_80112BEC`); store the six results through `((EventResultRow6 *)&D_8012439C)->value[k]` with `(s16) *(u8 *)((idx * 0x12) + D_80125230 + sel * 3)` and `(u8 *)(...) + (k)`; the flag test is `if (!(f & 1)) a0 = 0; else a0 = 1;` (the `bnez` + `li` in the delay slot + `b`/`or` shape); `a0` is `s16` (a `u8` local makes uopt forward the store to `D_8012531C` into the later `sel * 3`, the original reloads it). Open: the CSE'd `sel * 3` temporary (bit "mpy(cvt(D_8012531C),3)", priority 10.5) outranks the hoisted `lui 0x800B0000` constants (7.5) and takes v0, so D_800B1746 lands in v1 and the local in a1 instead of v0 and a0. Draft kept in the session scratchpad only.
- EVENT `func_8011B848` (particle step, 1408 bytes): the record view is `EventMotion44`, records 96..99 at `D_800E9620`; the first-block pointer is `(EventMotion44 *)(D_800E9620 + D_80125860 * 0x44)` with `[95]` fields, the loop body indexes `((EventMotion44 *)(D_800E9620 + i * 0x44))[96]` with `s16 i` (a shared pointer local gives a separate register range where the original has one expression temp in s0). Still about 120 register differences: the original keeps D_800B1AF6/D_800B1AF0 in a1/a2 promoted across the loop (reloaded after every call) and a local copy does not reproduce that. `x & 0x7FFF` is `% 0x8000` (the `D_7FFF` reloc).
- EVENT `func_80119110`: with raw `D_800B0860[...]` or local `u8 *` pointers the build is 122-139 instructions against 151; the original reloads `D_800B1AF0` and `D_800EECE4` after the `+= 1` store (an indirect store that kills them), ours merges them whatever the pointer form.
- EVENT `func_8011767C` needs local `s16[11]` initialisers copied from `D_80125330`/`D_80125348` (22-byte copies), which needs `data_island.py` for this object (it edits the splat yaml, so not done here).
- DATE `func_8013C7D0`: operand swap of the `D_800CA2F8` compare and a 240 s permuter run leave the two `lui` in the wrong order. DATE `func_80140374` (420 s) and main `normal_date_three_select_init` (200 s, five statement orders of the four chains): permuter finds nothing, no order of the chains moves `0x10` into v0.
- DATE `func_80132000`: the leading `nop` belongs to the padding before the function at `func_80132004` (called from `func_8013E9BC`); not portable.

## TT overlay, batch 2 (T-11500)

An Opus pass on the five closest T-11000 near misses (3 matched in about 32 minutes), then a Sonnet list over
untried TT functions (9 matched out of 17 attempted; 28 untouched). Ticket [[tickets/T-11500-tt-batch-2]].

- **A call ends a uopt basic block; its argument registers are forbidden block-wide.** `regsused` of a call block
  includes the argument registers the call moves values into, and `updateforbidden` (IDO `uoptreg2.c`) forbids them for
  every live range that touches the block. A value that should sit in a1 while the call takes two arguments must end
  its live range in an earlier block: `if (c) { x++; } else { return; }` instead of nesting the call in the `if`
  (`func_801349B0`); `goto` + label and `do {} while (0)` also split it, a label nothing jumps to does not. Probe: with
  a 1-argument call the value took v0, with 3 arguments a3. Read `R9SAVE`/`R9UNIT`/`R9COST` in the regdiff trace
  ([[uopt-register-allocation]]) to see which blocks a range touches.
- **Compound assignment keeps the call result first.** `angle = call(); angle += k;` gives `addu d,v0,t`; a copy
  `base = call(); angle = base + k` gives `addu d,t,v0` (`func_80134470`).
- **Delay-slot filling follows the order of independent stores.** Permute the statement order of packet stores; one
  line per store is fine. `func_80147150` and its twin `func_801336F8` need x, y, h, w (12 orders tried).
- **Stage-machine handlers.** `k = *(u16 *)(D_80158A8C + 8); c = D_80158A8C; switch (k) { case 0: ... break; case 0x40:
  ... break; } *(u16 *)(c + 0x364) += 1;` gives the `lw t6; lhu v0,8(t6); move s2,t6` head and the shared tail; an
  if/else chain loads `s2` at once. Inside the 0x40 arm read the table through a fresh `e = D_80158A8C` (the original
  reloads the global). Used by `func_8013B1C4`, `func_80135FA8`, `func_8013FBD0`.
- **Rolled slot loops.** `for (i = 0x10; i != 0x13; i++)` over `((TtSlot14 *)(c + 0x38))[i].f` with some stores through
  struct-array indexing stays rolled; an `n` bound in the for-init or a do/while with a constant bound is unrolled with a
  `li t0,3` remainder.
- **Twins.** `func_80141C6C`/`func_80141EBC` are copies of `func_80141A1C` with other packet constants; `func_80135FA8`
  and `func_8013FBD0` are twins of `func_8013B1C4`; check `tools/neardupes.py` before drafting from scratch.
- Smaller: a `u16` store of a switch case constant shares the compare register again (`func_8013B6C8`); BCD position
  bytes `(x / 10) * 16 + x % 10` with plain `while (f(...) == 0) {}` polls match the CD seek (`func_80142DEC`); an
  `s32` definition without a return statement keeps the temps of `func_80144A04` where `void` shifts them by two.
- Process: stage files by name; a `git add -A src` once swept a half-finished draft into a commit and broke the TT sha1.

## TT overlay, batch 3 (T-11510)

Opus pass on four T-11500 near misses (3 matched in about 41 minutes), then a Sonnet list of 14 untried functions
(1 matched, 7 guarded near misses). Ticket [[tickets/T-11510-tt-batch-3]]. Over the two Opus passes, 6 of 9 near
misses matched.

- **Join blocks decide what stays in registers.** A `break` or `continue` that sends a path straight to the shared tail
  keeps loaded values (shared expressions such as `+0x364`/`+0x368`) in registers; a separate join block reloads them
  (`func_8013B6C8`, marked `FAKE` when the `break` is redundant).
- **Saved-register priority and block counts.** uopt's priority is the saving divided by a compressed block count
  D = floor((N-2)/4)+2 for a range over N blocks. Removing one empty loop block (`if (p[0] == 0) continue;` instead of a
  nested `if`) took the counter of `func_80147644` from N=10 to 9, D from 4 to 3, priority 7.75 to 10.33, above the
  hoisted constants (10), and gave the original's s3..s7 order.
- **20-reference block split.** uopt ends a block after 20 variable references (`varreflimit`); reloads go at the start
  of the next block, so putting two stores on one line can move a reload ahead of a temp (`func_80147644`, `FAKE`).
- **Stores do not move above loads through another pointer** (possible alias): the source order of packet stores
  against the `p` loads decides what fills a load delay slot (x, y, v, u in `func_80147644`).
- **abs as an inline ternary.** `if (((v < 0) ? -v : v) >= 0x801)` gives `bgez; move; b; negu` with no extra local; an
  `a = v; if (v < 0) a = -v;` local outranks other values (`func_80136244`).
- **ugen temporaries.** t6..t9 are handed out in the order they were freed, so an extra global reload in a switch
  default changes the numbering (`p` instead of `D_80158A8C` in `func_80136244`). A constant used twice in one block is a
  ugen temp, not a uopt range, and takes the lowest free of v0, v1, a0... (`func_80144A04`, still open).
- **Share the loop variable.** A tail loop with its own counter got other s-registers; reusing the `i` of the earlier
  loops fixed `func_80139770`. To take the address of a function whose ANSI prototype would change K&R callers, use a
  file-local non-prototype `extern void f();` and a `void (*fn)()` pointer.
- **Keep an induction variable.** `x = col * 16 - 0x80` is strength-reduced and `col` disappears; `(col << 4) - 0x80`
  keeps it (`func_8013DE10`).
- **Operand order of sums sets temp numbering.** Trying operand orders of the u/v/clut sums took `func_8013DE10` from
  178 to 58 rows; script the 2^n combinations.
- Process: restrict scripted edits (`sed`) to one function's line range; a global substitution of `s32 i;` broke other
  functions. Do not edit a C file while the permuter verifies through it.

## T-11120: small functions (Sonnet via Antigravity) 36

NAME_ENT `src/ovl/NAME_ENT/80139740.c`. Six matches: `func_8013C934`, `func_8013C580`, `func_80142880`, `func_80140430`, `func_80140574`, `func_8013C9F0`.

- **Pointer read of a field splits a range** (`func_80142880`): `s8 *sel = &D_800E6280.unk_1094; switch (*sel)` while a later `D_800E6280.unk_1094 >= 0` stays a plain global read. Without it uopt joins both reads into one range in $v0 and the later read lands in $v0 instead of the ugen temporary $t8 (regdiff: "the original's $t8 is not a uopt colour"). The permuter found it (score 35 candidate).
- **Selector in $v1 after a call** (`func_8013C580`): `GameState *g; u8 sel; g = &D_800E6280; sel = g->unk_1115;` together (neither alone helped); found by the permuter.
- **Switch-temp copy** (`func_80140430`, `func_80140574`, `func_8013C9F0`): `s32` return type without a `return` gives `or v0,v1,zero`; the 8-byte table `D_800B394C`/`D_800B3A3C` is `s32[]` indexed `[i * 2]`, `[i * 2 + 1]` (declared with `sync_protos --add-global`).
- **Rolled constant-trip loop** (`func_8013C9F0`): the pointer form `for (p = D_800E7D11; p != &D_800E7D1F; p++)` is unrolled by four; the index form `for (i = 0; i < 14; i++) if (D_800E7D11[i + 4])` stays rolled like the original. The frame needed `s32 pad[3]` above the counter (FAKE).
- **Blocked:** the selector-copy prologue shared by eight functions (`D_800E7313` in $v1 plus a `move v0,v1` copy, constants in $a0/$a1); see the rows in [[data/t0018-cases]].
- Tool notes: the edit tool refuses a replacement whose target text appears twice in the file, even when the line range is unique (the three twin prologues here); include a unique neighbouring line. `permute.py --no-verify` lets the permuter run while the source is edited.

## TT overlay, batch 4 (T-11520)

Opus pass on the five closest near misses: no match, all closer (`func_8013E850` 14 -> 2 lines, `func_8013DE10`
29 -> 7). Sonnet over the 13 large untouched functions: 2 matched (`func_80138300`, `func_801430D4`), 1 guarded.
Ticket [[tickets/T-11520-tt-batch-4]].

- **Stop store forwarding through another symbol.** Writing a byte through a different symbol base that reaches the same
  address (`(&D_800E7D4E)[-1] = 0` for `((u8 *)&D_800E7D4C)[1]`) keeps uopt from forwarding the stored constant into a
  later read-modify-write, giving the original `lbu; or; sb` (still `$at` relocations; a raw address uses a t-register).
  `FAKE`.
- **PsyQ macro-style packet lines.** Packet init written one macro per line (`setPolyG4`: len and code on one line, all
  eight coordinates on one line, one line per colour) reproduces both the store order and the ugen constant registers;
  one store per line gives the right order but other registers, one single line lets as1 reorder freely
  (`func_8013DE10`). Same for RECT: `r.x = ...; r.y = ...; r.w = ...; r.h = ...;` on one line stores h, w, y, x
  (`func_801430D4`).
- **Same index expression joins live ranges.** `*(s16 *)(c + K + i * 2)` instead of `((s16 *)(c + K))[i]` makes
  `c + i*2` one uopt range shared with other loops of the same form (`func_801392A0`).
- **Local copy of a global.** `g = D_80158A60;` gets a uopt register loaded early; an inline global read gets a late ugen
  temp (`func_80134BBC`). Globals loaded once and reused after stores through a `u8 *` are reloaded unless copied to a
  local first.
- **Pointer reuse instead of a second pointer.** An m2c `tempN = base + 0xNNN` later used in place of the base means
  `s += 0xNNN` in the source; a second pointer local changes which values are callee-saved and the frame
  (`func_80138300`).
- **Operand order sets ugen temps.** `x + call() / 64` vs `call() / 64 + x` decides t7 vs t9 (`func_80138300`).
- **Locals and pads.** Homes go top-down in declaration order; a FAKE pad must come after the variable that should keep
  the higher offset (`RECT r; s32 pad[2];`, `func_801430D4`). Declaring a counter after a struct puts its spill home
  below the struct (`func_80140624`); an unused declared scalar still moves spill slots.
- **m2c's `default` can hide case labels.** Values handled after the jump table (0x81, 0xA0 in `func_801400B0`) must be
  real `case` labels.
- Process: verify that a scripted body insertion actually applied (`git diff --stat`) before trusting a `funcdiff`
  MATCH, which otherwise compares the untouched asm.

## TT overlay, order_search pass (T-11540)

First use of `tools/order_search.py` (T-11530) over the 33 guarded TT drafts. Ticket [[tickets/T-11540-tt-order-search-pass]].

- **Match: `func_8013DE10`.** Both u/v sums reversed and the flags lookup written `*(r[0] + flags)` instead of
  `flags[r[0]]`: the operand order of the address addition set the ugen temps. Hand search in T-11520 had tried only the
  outer sums.
- **Statement order is where the tool pays off.** Store blocks reordered by the tool improved five drafts: `func_8013C1FC`
  36 -> 4 diff lines (only the `addu` operand order of buf+idx*4 left), `func_80140920` 84 -> 50 (case 0x40 matches with
  the 0x212 update before 0x3EC), `func_801330D8` 45 -> 34 (the `a+0x28` store after `a+0x24`), `func_8013B4A0` 22 -> 20
  rows, `func_8013E498` 101 -> 99 rows.
- **Top-level `+` operand swaps rarely change anything**: IDO canonicalizes most `x + y` orders. They mattered only where
  the sum fed an address (`func_8013DE10`). `==` and `*` swaps tried here did not help. The `addu` operand order of
  buf+idx*4 (`func_8013C1FC`, formerly `func_8014CC04` too) did not move with any swap or array form.
- **Order-independent:** `func_8013E850` (condition shape), `func_80144A04` (ugen temp for a constant used twice),
  `func_80138E28`, `func_801392A0`, `func_8013D2E0`, `func_8013D3C0` (r/g allocation reversed), `func_80140624`,
  `func_80142570`, `func_801436BC`, `func_8013C8DC`: their remaining differences are register allocation, not order.
- Marking rules that worked: only commutative operators and independent stores; never across a call or a load a store may
  alias. The tool does not check dependencies; check `git diff` after each run.
- Cleanup: `func_8014CC04` had a stale one-instruction NON_MATCHING draft (T-9380) above a typed version that has matched
  since T-9940; the stale draft was removed.

## TT overlay, Opus pass with all solvers (T-11550)

One Opus agent on the eight closest guarded TT drafts with autotweak (order stage T-11200, regdiff hints T-11220),
order_search, regdiff and a quick scratch loop (`cc.py` + `funcdiff --built`, about 5 s per try): 5 matched
(`func_8013E850`, `func_8013C1FC`, `func_80140624`, `func_801330D8`, `func_80140920`), `func_80142350` 11 -> 2 lines,
two unchanged. Ticket [[tickets/T-11550-tt-opus-near-miss-pass]].

- **Delay-slot `lui $at` depends on the whole flow graph.** as1 can move a store's `lui $at` into the delay slot of an
  earlier branch of the same if-chain, but only with the right control flow: restart/loop/reset gotos blocked it, two
  nested `for (;;)` loops (break to the reset call, `goto tail` out) allowed it (`func_8013E850`). Found by deleting
  statements one at a time: the placement changed only when the restart block was emptied.
- **Typed indexing fixes `addu` operand order.** `&((TtDrawBuffer80B8 *)buf)->ordering[idx]` gives `addu buf, idx*4` where
  every raw-pointer form gave the reverse (`func_8013C1FC`, like `func_8014CC04`).
- **Pointer walks are often strength-reduced indexed loops.** A loop ending on `y != &next_table` was `for (i ...) x[i]`
  in the source; writing it back changed the other loops' allocation through the shared `i` (`func_80140624`).
- **Locals named for a reloaded expression change everything.** Named `t`, `x`, `vx`/`vy` copies changed `beq` operand
  order, ugen temp numbering and spills; where the original reloads from memory or merges two reads, write the
  expression inline (`func_801330D8`, `func_80140920`).
- A struct local's frame position can be fixed by declaration order alone (`pt` after `vy` in `func_80140920`).
- Tools: regdiff's hints led to the `func_801330D8` and `func_80140920` fixes; autotweak found the `func_80142350`
  improvement; order_search was not needed for these (their misses were not order-dependent).

## TT overlay, batch 6: bulk_m2c pipeline and Opus (T-11560)

`tools/bulk_m2c.py --unit TT --exclude ""` (the default `--exclude` is TT) drafted the 65 remaining TT functions up to
2,400 bytes: 1 match (`func_8013E1E8`), 62 drafts ranked by score. One Opus agent on the ten closest drafts plus
`func_80142350`: 3 matched (`func_80148154`, `func_80142C70`, `func_80144CF0`), 3 guarded, 5 not kept. Ticket
[[tickets/T-11560-tt-batch-6]].

- **Where `i = 0` goes decides delay slots and priorities.** For a loop under an `if`: `for (i = 0, p = X, q = Y; ...)`
  inside the `if` makes `move sN,zero` the first instruction of the fall-through block, which as1 puts in the `beqz` delay
  slot (`func_80148154`); a `for (i = 0; ...)` head instead of `i = 0` before the `if` gave `i` its s-register ahead of
  `base` (`func_80142C70`).
- **Offset before index.** `*(T *)(base + OFF + i * k)` gives the `addu base, idx` operand order (`func_80142C70`).
- **`slti` on a stepped value is an LFTR'd counter loop:** write `for (i = 0; i < 2; i++) { x = i * 5 + 5; }`;
  `n = 5; while (n < 15)` gives `bne` (`func_801437F0`).
- **Compare operand order:** a field load compared in an if-chain against constants held in registers gives
  `bne const, val`; a local gives `bne val, const` (`func_80135148`).
- **A later reload of the same field** becomes its own register range only through a separate local; `volatile` on a
  pointer local reproduces a reload uopt would CSE (FAKE; `func_80148764`).
- Two plain stores of the same constant share one register (`a[0x55] = 9; p[0x92] = 9;`); a chain assignment was worse
  (`func_80144CF0`).
- Tidy m2c leftovers in matches: `*(u8 *)((unsigned char *)D_80158A64 + (28))` is `D_80158A64[0x1C]` (`func_8013E1E8`).
- Tools: regdiff named the priority problems (`func_80142C70`, `func_80135148`); autotweak (5 runs) and the permuter (1)
  found nothing on these drafts; a quick ninja-one-object + `funcdiff --no-build` loop was the main workhorse.

## Wave 1 package w1-p04: EVENT 8010FE10.c (T-11600)

Twelve functions of `src/ovl/EVENT/8010FE10.c`, six guarded drafts and six m2c drafts. 10 matched (`func_80119B78`,
`func_801155D4`, `func_801198E8`, `func_8011C470`, `func_80117178`, `func_8011AD7C`, `func_801142E8`, `func_80114784`,
`func_80114948`, `func_80113520`; 3,676 bytes); two stay guarded. Ticket [[tickets/T-11600-w1-ovl-event-8010fe10]].

- **`s32` without a return again.** `or v0,tN,zero` after a byte update (`func_8011C470`, `func_80119B78`) and
  `move v0,zero` after `sw zero` (`func_801155D4`) are the K&R int function falling off its end. In `func_80119B78`
  the byte update is not at the entry, so the global is not promoted; a `u8` copy does it:
  `cur = D_800B1AF6; D_800B1AF6 = cur -= 5;`.
- **cvt_pass decides "unsigned only" over the whole file.** `_unsigned_narrow_globals` collects every load and store of
  a global in the translation unit; one signed access anywhere (`extern s8 D_80125320` with plain stores in
  `func_801155D4`) means its CVTs are never stripped, so a compare against a register-held constant always puts the
  constant first. `D_80125320` is now `u8` in `include/ovl/EVENT.h` (all 27 builds still match); with a dead load
  before the first call, branch or label (`sel = D_80125320;`, FAKE) the global goes first in `bne` like the original
  (`func_801198E8`, `func_8011C470`). The same dead load of `D_801252E4` makes uopt keep it in `$a1` across the call
  in `func_8011AD7C` (whose `func_8002328C` call has one argument: `$a1` only holds the promoted global).
- **`li 6; multu` per access is the EventStateView index.** The pool, skate, ski and bowling setups match in the
  shape of the already matched `func_80112930`: `g = (EventStateView *)&D_800B0860;` and `g->unk_EE6 * 6` in each
  table access gives one `multu` per access with the constant in `$v1`; with `D_800B1746 * 6` IDO shares one shifted
  product (the old T-0018 rows of `func_801142E8`, `func_80114784`, `func_80114948`).
- **Reload of the record index after a compare chain.** `*(s16 *)(D_800B0860 + ((EventStateView *)D_800B0860)->unk_EE6 * 0x34 + 0x1A6) += 1`
  reloads the index into a temporary like the original; `D_800B1746 * 0x34` keeps `D_800B1746` in `$v1` from the
  compare chain (`func_80117178`).
- **Not matched.** `func_80114B0C` (4 lines): the two hoisted row addresses tie at priority 2.5 and `D_8012439C`,
  used first by `v = *r`, is coloured first; the DATE twin's fix (no temporary, `a[i] = b[i]`) needs array views,
  which hoist the `D_800B0F2C` load out of the loop. A `u8` copy of `D_800B0F2C` fixed its `bne` order (autotweak).
  `func_80118868` (8 lines): the third chain's 3 is a uopt colour (`$v0`) in the original and a ugen temporary in
  every draft tried.
- Tooling: `funcdiff --resolve` on the guarded full unit reports "linked function address or size differs" when other
  guarded drafts change size; unguard and run ninja to confirm. Some candidate sources fail in cfe with a syntax error
  at an unrelated line (a truncated line in the `.sjis.c`); adding one space elsewhere in the file avoided it.

## Wave 1 package w1-p05: main 80042540.c and 80058D20.c (T-11610)

Ten functions, 7 matched: `func_80042808`, `func_8004284C`, `func_80042908`, `func_80042940`, `initModelingData_init`,
`func_80058F9C`, `func_80042540` (`src/main/80042540.c` is now all C). `func_800590CC` stays guarded (22 lines);
`func_800591D8` and `func_8005938C` are back to `INCLUDE_ASM`. Ticket [[tickets/T-11610-w1-main-80042540]].

- **`return 0` after a last store through `lui at` (the reset-helper family, T-4020 to T-10850).** The original puts
  `or v0,zero,zero` before the last `lui at` and the store in the `jr` delay slot. Every `return 0` form puts the move in
  the slot. The `normal_date_girl_in_init` trick (T-9540) fixes it: `s32` function with no `return`, and the last store
  reads a zero through its address: `z = 0; p = &z; D_800E6280.unk_110D = *p;`. Only the last store needs it. FAKE.
  Other functions with the same tail (`or v0,zero,zero; jr ra; s? ..($at)`) can use it.
- **PsyQ TMD walk.** `initModelingData_init` and `func_80058F9C` are the libgs sample's model setup (func_8009B3C0 and
  func_8009B430 look like GsMapModelingData and GsLinkObject4). Write the pointer as separate steps,
  `dop = arg0; dop++; map(dop); dop++; n = *dop; dop++;`: that keeps the original's `move s2,s3; addiu s2,s2,8` (and
  in `func_80058F9C` the `addiu a3,a0,4; lw v0,0(a3); addiu a3,a3,4` chain), which `arg0 + 1`/`t + 2` forms fold. The
  object loop of `initModelingData_init` is an indexed `for (i = 0; i < 16; i++)` over `D_80122640[i * 4]` and
  `&D_801227A0[i]`; the pointer do-while gave the wrong address-setup order. The TAIIKU twin `func_801406A8` should take
  the same C (`dupes.py`).
- **Repeated slot address instead of a local** (`func_80058F9C`): `(u8 *)D_80122640 + arg1 * 0x10` written three times
  shares the original's one spill slot (compiler temporaries share slots, named locals do not), and
  `&D_80122640[arg1 * 4]` costs one more ugen temporary than `arg1 * 0x10` (`t7` instead of `t6`).
- **Byte views of both selectors** matched `func_80042540` (`switch (*(u8 *)&D_800E6280.unk_1108)`, and the s8
  `unk_111A` read with `lbu`).
- **Constants shared between a store and a call argument (`func_800590CC`, `func_800591D8`, T-0017 family).** The
  original stores and passes the same `a0`/`a1` (`li a0,0x200; sh a0,D_800E6280; ...; jal func_80097F58`) and, in
  `func_800590CC`, loads the else-arm's 0x140/0xF0 into `a0`/`a1` before the branch. IDO propagates the constants
  (`li t9; sh t9` and a separate `li a0`). Tried without effect: a `u16` prototype for func_80097F58 (it looks like
  GsInitGraph), separate scalar globals instead of GameState fields (same code), assignments inside the call
  arguments, `w`/`h` locals, a local struct. Reading `w`/`h` through their addresses (FAKE) stops the propagation and
  gives the original's hoisted constants, but then `w`/`h` are spilled before each call (frame +8): 22 lines in
  `func_800590CC`, 54 in `func_800591D8`.
- **Struct copies and ugen temporaries (`func_8005938C`).** The draft (an 0x88-byte model record over two
  `D_8011ECD0` slots: GsDOBJ2-like at +4, coordinate at +0x14, rotation/scale/translation/velocity SVECTORs at
  +0x64..+0x7C; locals `SVECTOR rot; VECTOR scale; MATRIX m, sm, ls, lw;`) is exact apart from ugen temporaries. The
  frame (0x108, `lw` at sp+0x64) needs three 4-byte scalars declared before `rot` (`i`, `p` and one more, FAKE if
  unused). The rest: the original's three `MATRIX` copies each burn one ugen temporary before the data register
  (`t4` then `t5`), IDO burns two (`t4`, `t5`, then `t6`). Probes: IDO burns two after a store to a stack local or at
  the start of a function, one after a store through a pointer (`q[0] = x;` right before the copy); ETC
  `func_8013FA7C`'s original burns two after its stack stores, so the original compiler does both. No C form found
  that keeps the rot stores on the stack and burns one; a `temp` local for one rot load fixes copy 1 only (104 lines).

## NAME_ENT 80139740 wave 1 package w1-p01 (T-11570)

Twelve guarded NAME_ENT drafts, all previously tried; no match. Ticket [[tickets/T-11570-w1-ovl-name_ent-80139740]].

- **Selector-copy family is a copy-propagation gap, not a C form.** `func_8013D4CC`, `func_8013DAC4`, `func_8013E430`,
  `func_8013F0DC`, `func_8013F454`, `func_8013FB8C`, `func_8013CEC8` load the s8 selector `GameState.unk_1093` into
  `$v1`, compare it constant-first (`beq a1,v1`) and divide/index a copy (`move v0,v1`). The plain source
  `idx = D_800E6280.unk_1093; if (D_800E6280.unk_1093 != -1) { ... idx / 7 ... idx % 7 ... }` is right: with uopt's
  global copy propagation off for the procedure (`OPTN 405 0`, the `-zcopy:0` switch `tools/cvt_pass.py` already sets
  for unsigned-global switch temporaries; tested by a temporary local edit of the pass, not committed) the drafts come
  to 4 lines (`DAC4`, `F454`, `FB8C`: only the two compare operand orders), 8 (`F0DC`, `E430`: plus the frame), 10
  (`D4CC`) and 14 (`CEC8`). IDO's default propagation folds `idx` into the global and gives the global `$v0`. No C form
  tried keeps the copy with the right registers: `u32 idx` keeps it and gets every register right but divides with
  `divu`; `(s32)idx` lets uopt see through the conversion again; casts, `*p` views, aliasing stores and constant-address
  stores do not help. The constant-first compare also fits a widening `CVT` after narrow *signed* global loads (the
  original has `lh` globals constant-first 40:8, see "K&R promotion rules (T-7000)"): a crude experiment inserting
  `CVT J<-L` after them turned the compares constant-first. Follow-up for the toolchain owners: decide whether the
  original's narrow signed global loads carried a widening (and whether a local copy of a global is never propagated),
  then extend `tools/cvt_pass.py` and re-verify every matched function. Only 7 unmatched functions show the exact
  `lb v1; ... or v0,v1,zero` shape (the 6 above plus OPTION 80132000 `func_80134110`), none matched.
  **Update (T-16920): it is a C form.** Read through a local `GameState *g = &D_800E6280;` (`idx = g->unk_1093;
  if (g->unk_1093 != -1)`), all 7 match with the pass as built: the indirect load is not a plain variable, so uopt
  puts the constant first and keeps `idx`. See "GameState +0x1093 and the selector-copy family (T-16920)" below.
- **IDO frame rule with locals** (measured on `func_8013BE18`): locals are placed from the frame top in declaration
  order (first declared highest) and the frame keeps 8 spare bytes below them (`raw frame = end of saves + 8 +
  locals`, 8-aligned; no locals, no spare). A 5-argument call puts the saves at 0x18..0x1F, so the locals start at
  0x20 and an 8-byte RECT lands at raw 0x28 (sp+0x38 after the frame pass). The original has it at sp+0x34 in the same
  0x40 frame, which this rule cannot give: any extra local (s32/s16/u8 pad, 12-byte struct, `s16[6]`, block scope)
  grows the frame by 8. The draft's other difference (`li v1,0x80` before `lui at`) was the order of the two case-2
  stores (`unk_03A = 0x80` first); it is now frame-only (10 lines).
- **`D_800E6280.unk_3A` is not a GameState field.** It belongs to `Work80125D10`; under `-cckr` IDO accepts a member
  of any struct with warning 749 and uses its offset, so older drafts compiled by accident. The GameState byte at
  +0x3A is `unk_03A` (`u8`). For `func_80140B20` the u8 view `*(u8 *)&D_800E6280.unk_03A` (15 lines) beats the plain
  field (25: the global moves from `$a0` to `$v0`). The in-place form (`G -= 2; chain = G; if (G < 0x80) f(G);`)
  gives both stores of the global like the original; the original's extra `andi v0,t6,0xff` chain value stays out of
  reach (separate stores, locals, masks, zcopy off all fold it).
- **printf pairs (`func_8013BB24`)**: writing `D_8014D0B0 = speed; func_800AE0B0("kanji_speed...", speed);` on one
  source line fixed that pair's `addiu a0`/`sb` order (as1 schedules by line); the other seven pairs (`lui a0,string`
  before `lui at,store` in the original) do not move with one-line pairs, assignment-in-argument (goes through `$v0`)
  or comma forms. 28 lines.
- **`func_8013A9C8` draft dropped** (232 lines; still in commit 64965d57): the else branch needs `GameState.unk_1117`
  in a uopt register (`lbu a0`), `rec` in `$t0`, the walker in `$a2`; writing the 0xF00 multiply as shifts puts the
  global in a register and hoists `lui`/`i = 0` above the branch like the original, but ugen's multiply expansion
  (one reused temporary) shows the original wrote a plain multiply. `func_8013CEC8` (42 lines) dropped too; its
  natural `n` draft is in the same commit.
- Tools: regdiff (the traced uopt was in the scratch directory) named the priorities; autotweak found nothing on
  `func_8013DAC4`/`func_8013BB24`; a small local loop (variant file -> guarded block -> `funcdiff --resolve`) did most
  of the work.

## T-11620: wave 1 package w1-p06 (TT 80133970.c, 80142D40.c)

Four matches (2,052 bytes): `func_80135148`, `func_80134E20`, `func_80134D0C`, `func_80144854` (FAKE view). Ticket
[[tickets/T-11620-w1-ovl-tt-80133970]].

- **Unused trailing call arguments.** func_80141C6C/func_80141EBC take (ot, packet, q) even where the callee ignores q.
  Passing q puts it in a2 for its whole range and fixes every other register (`func_80134D0C`, `func_80134E20`). The
  callee's asm does not show the extra argument, so m2c drops it; check the other callers in `src/` first. In the same
  way `func_801441BC` calls `func_801437F0(h, g)` with D_80158A6C/D_80158A64 in a0/a1.
- **Loop bound in a local set in the for-init.** `for (i = 0, n = 0x80; i != n; i++)` makes 0x80 a variable that is
  numbered before the chain constants (0xAB, 0xAC, 0xAD), giving the original's s5/s6 order (`func_80135148`, the
  pattern `func_80134BBC` already used). `n = 0x80;` as a statement before the loop moves its `li` too early.
- **One PsyQ macro per line for TILE packets.** `setXY0`, `setRGB0` and `setWH` each on one line; the order of the
  lines sets the schedule (tile 1 xy/rgb/wh, tiles 2 and 3 rgb/xy/wh) and `((TtOtTag *)t)->len = 3; t[7] = 0x61;` on
  one line gives the original constant order (`func_80134E20`).
- **x and y stores before the flag bytes** (`func_80134D0C`, found by `permute.py` in 180 s): the order of the
  sprite stores moved a `li` of 0x3D94 and a `move` that looked like a uopt range.
- **A test that re-reads a global after a local copy.** The original loads p straight into s2 and reloads D_80158A74
  for the next test; IDO merges the first test's read with p. Only an array view of the D_80158A60 pointer block on
  that one read keeps them apart (`func_80144854`, FAKE). In `func_80143978` and `func_801441BC` the plain global read
  already stays separate.
- **Frames without saved registers.** Every declared local takes a 4-byte slot, top-down in declaration order, whether
  or not it is spilled; spill offsets and the frame size follow from the declaration list (`func_801441BC`: frame and
  slots matched once the locals were declared i, g, h, r, p, q, v plus a pad).
- **`found = a[0]`** after `a[0] = 1` gives the original's `andi t4,t1,0xff`; `found = 1` gives `li` (`func_80143A44`).
- **Slot searches** are pointer walks with a separate counter: `r = D_80158A80 + 6 * 0x54; for (i = 6; i < 8; i++)
  { ...; r += 0x54; }`; `r = base + i * 0x54` inside the loop strength-reduces differently (`func_80143A44`).
- **Blocked:** `func_801441BC` (the original keeps the `break` after an inner `if` as a separate `b; nop` block, IDO
  threads it), `func_80143A44` (the pad & 2 part), and the phantom-register drafts (`func_80134768`, `func_80134874`,
  `func_801436BC`, `func_80144A04`, `func_801437F0`). Drafts of the two big functions are in
  [[data/t11620-func_801441BC-draft.patch]] and [[data/t11620-func_80143A44-draft.patch]].
- Tools: a side-by-side `objdump` of `build/permute/<func>/base.c` (compiled with its `compile.sh`) against
  `target.o`, with registers and branch targets normalized, showed the structural differences of the big functions in
  a few lines. autotweak took `func_80143A44` from 320 to 106 lines (its edits include a FAKE empty test).

## TT `8013C6D0.c`, wave 1 package w1-p07 (T-11630)

Eight functions of `src/ovl/TT/8013C6D0.c`; 6 matched (`func_8013C8DC`, `func_8013CC24`, `func_8013D508`,
`func_8013D73C`, `func_8013D150`, `func_8013D3C0`), `func_8013D2E0` guarded (registers only), `func_8013D988` not kept
(66 rows; draft in [[tickets/T-11630-w1-ovl-tt-8013c6d0]]).

- **Draw pool records are `TtPoolDraw14`, filled by indexed loops.** `func_8013C8DC` and `func_8013CC24` write
  `D_80158AA8` records; with `TtPoolDraw14 *p` and `for (i = 0; i < 3; i++, p++) { ... p->text = D_80155D50[i]; }`
  IDO strength-reduces `i` into the table pointer and compares it with `&D_80155D50[3]` (the `lui/addiu D_80155D56`
  pair). A four-iteration loop is unrolled completely (`func_8013CC24`; the dead `addiu v0,v0,0x50` after it is the
  pointer step). The byte-pointer drafts of T-11000 could not get this.
- **A loop counter that unrolling removes still has a frame slot.** `func_8013CC24` needs `u8 *q; s32 i; TtPoolDraw14 *p;`
  in that order (q at 0x34, hole at 0x30, p at 0x2C); no FAKE pad.
- **Real locals instead of FAKE pads.** `func_8013D508` (sibling of `func_8013DE10`) got its frame from a used `tm`
  (tile map pointer) and `st` (`TtCmdStream *`) local; each declared local takes a slot even when it lives in a
  register. (In `func_8013DE10` itself a used `tm` changes the code, so its pads stay.)
- **Read a just-stored field back through the pointer instead of keeping a local.** `func_8013D150`: without the
  `target` local (`if (q[1] < 0x8000) q[1] = 0x8000;` and so on) uopt keeps q[1] in v0, the two `0x30000` stores stay
  separate constants and the ugen temporaries line up. `cur = (diff < 0) ? -diff : diff;` gives the original
  `bgez; move; b; negu` abs.
- **Comma expressions for packet macros.** `((TtOtTag *)v)->len = 3, v->code = 0x61;` and `v->w = 0x100, v->h = 0xE8;`
  (the libgpu `setTile`/`setWH` shape, one source line) give the original store order where separate statements do
  not; the same for `setXY4`/`setUV4`-style lines in `func_8013D988`. This replaces the T-10850 same-line FAKE.
- **uopt colours unconstrained ranges in first-reference order.** `func_8013D3C0` wants g in a1 and r in a2: a dead
  `g = 0;` before the red channel numbers g first (FAKE, T-11630). Declaration order does not change it.
- **Struct field vs. pointer arithmetic changes operand order.** In `func_8013D988`, `(j - 8) * (0x8800 - c->unk_34)`
  through a struct field evaluates `j - 8` first as in the original; `*(s16 *)(c + 0x34)` (one tree level deeper) puts
  the load first, whatever the source order. Also there: `x += (s16)(rsin(...) / zoom)` on both arrays, s16 coordinate
  locals computed `x0, y0, x1, y1, ...` before a one-line store, `len = 9, code = 0x2D` order for the 9/45 constants.
- Still open: `func_8013D2E0` (original leaves a0 unused; an unused parameter only adds a home store) and
  `func_8013D988` (the original gives `hi` the register that IDO gives the constant 3). Rows in [[data/t0018-cases]].
- Tools: a local objdump side-by-side of the guarded draft was faster than funcdiff for structural work; autotweak
  (2 runs on `func_8013D988`) found nothing; regdiff named the bit-order cause in `func_8013D3C0`.

## Wave 1 package w1-p03: main 8007C030 (T-11590)

Ticket [[tickets/T-11590-w1-main-8007c030]]. One Opus agent on `src/main/8007C030.c`: 15 matches. From the work
list: `normal_date_girl_suddenout`, `normal_date_three_select_init`, `normal_date_bggirl_fadein`,
`normal_date_bggirl_fadeout`, `normal_date_girl_in_main`, `normal_date_girl_out_main`,
`normal_date_move_place_main`, `func_80085CD4`, `func_80083A10`, `func_80085234`; past it: `func_800850D4`,
`func_80085B3C`, `func_80083474`, `func_80084E90`, `func_80083808`. Two matched functions lost their FAKE
(`normal_date_girl_suddenin`, `normal_date_girl_in_init`). Left guarded: `junban_init` (34 lines),
`dialogue_speak_part` (16), `make_three_select` (6, frame only). `_sprite_set_box_shade_tarao` (188 lines) and
`dialogue_render` (5,588 bytes) stay `INCLUDE_ASM`.

- **`s32` without a return value keeps the last chain value in v0.** IDO compiles `a = b = c = k;` through one
  hidden temporary per function. In a `void` function uopt folds or copy-propagates it away; in an `int` function
  that falls off its end, v0 is live at the exit, so the temporary's last value stays in a register: the original's
  `li v0,0x10` for the last array chain (`normal_date_three_select_init`), `move v0,a0` / `andi v0,a0,0xff`
  for the two colour chains (`normal_date_bggirl_fadein`, `_fadeout`), `andi v0,a0,0xffff`
  (`func_80084E90`), even a `li v0,-129` that is never read (`func_80083808`) and the `move v0,zero` of a
  cleared promoted global (`func_80085B3C`). Symptom: an extra `move`/`andi`/`li` into v0 near the end of the
  function, or v0 holding a value that nothing reads. Try `s32` with no `return` first (and declare it so in
  `include/main_api.h`; the callers did not change).
- **The normal_date sprite bytes are two GsSPRITE-like `Rec24` records.** `D_801217E4..E6` and `D_80121808..0A`
  are r, g, b of `D_801217D0[0]` and `[1]`, `D_801217DC/DE` are their tpage and u. One chain per record in r, g, b
  order (`D_801217D0[0].unk_14 = D_801217D0[0].unk_15 = D_801217D0[0].unk_16 = 0x80;`) gives the original store
  order (outer targets right to left, the innermost scheduled last). The old scalar and byte-view chains needed
  FAKEs (`normal_date_girl_suddenin`, `normal_date_girl_in_init`); these are now plain C.
- **The flag bytes `D_8011F50F`, `D_8011F4CB`, `D_8011F513`, `D_80120653`, `D_80120697`, `D_8011ECD3` are bytes
  of the `D_8011ECD0` record table** (records 31, 30, 96, 97, 0; byte 3 = the display flag). Written through the
  table (`D_8011ECD0[0x83F] &= ~0x80;`) the second load stays after the first store, as in the original; the
  scalars let IDO hoist it.
- **A global updated in place is promoted by uopt and stored back once.** `if (D_800B5940 >= 8) D_800B5940 -= 8;`
  followed by chains that read `D_800B5940` gives the original's single register (`v1`), the `andi; move` after the
  subtraction and the late store (`normal_date_girl_out_main`, `girl_in_main`, `move_place_main`); a local copy
  gives two registers. The same holds for a flag byte (`func_80083A10`: `D_8011ECD0[3] &= 0x7F; ... |= 0x80;
  if (D_8011ECD0[3] & 0x80)`).
- **Return types from the asm:** a `move v0,zero` on one path before the epilogue is `return 0` there and no
  return elsewhere (the normal_date mains, `func_800850D4`, `func_80083474`).
- **K&R callee narrowing.** `func_800850D4`'s `sll/sra a3` at entry and the `lh` reload of its home slot after the
  call are an `s16` K&R parameter; `func_80085B3C`'s `move t7,a1; move a1,t7` is a `u8` K&R parameter (ANSI `u8`
  or K&R `s32`/`u32` are 140+ lines off). K&R definitions need `()` in `include/main_api.h`.
- **Types fixed from all users:** `D_800CA360` is `u8` (its only loads are `lbu`). `Rec24.unk_08` is probably
  unsigned like GsSPRITE `w` (`func_80084E90` shares one 0x80 between `unk_08`, `unk_0A` and `unk_0F`, so it needs
  a FAKE `u16` view); changing the member needs ETC `func_80142A4C`'s `s16` view of `unk_0A` removed (follow-up).
- **Tool notes.** `funcdiff --resolve` on a guarded draft compiles the whole NON_MATCHING unit, so an earlier
  guarded draft of another size moves every later function: it reports "linked function address or size differs"
  for a correct draft (and so did the trailing-nop pad of `func_80085CD4`, the last function of the object).
  Check such a candidate by removing its guard and running ninja; the unit sha1 decides.
- **Left open.** `junban_init`: the v0/v1 swap regdiff names (unconstrained ranges coloured in bit order, the
  global first); `return D_800CA2A4[0] = r;` puts the global in v1 but adds an `andi v0` (27 lines).
  `make_three_select`: every instruction matches, but the original's buffer is at sp+0x64 and IDO places the first
  local at sp+0x68 or higher. `dialogue_speak_part`: the two `li a0,-40` rotations (the original fills the `lbu a2`
  delay with the `li`). `_sprite_set_box_shade_tarao`: the original keeps the colour shifts in ugen temporaries
  (t6..t9) and the params in a1/t2, IDO gives the shifts uopt registers (POLY_G4 draft, 188 lines, not kept).
  `D_80122D48` is declared in the C file because EVENT `80103B60.c` declares that address as a function table;
  moving it to `main_api.h` needs a `MAIN_API_OVERRIDE_D_80122D48` in that EVENT file (follow-up).

## T-11580 wave 1 package w1-p02 (DATE/80132000)

Ticket [[tickets/T-11580-w1-ovl-date-80132000]]. 11 matched (5,016 bytes): from the list `func_80137D1C`,
`func_8013D8B0`, `func_801366A4`, `func_80136B70`; outside it, with the place-handler shape of `func_801366A4`,
`func_801347B8`, `func_801353F8`, `func_80135620`, `func_80135804`, `func_80136870`, `func_80136D3C`,
`func_801393DC`. Guarded drafts added: `func_801324DC` (1 instruction), `func_80137DF4` (4),
`func_80139664` (9); improved: `func_80139834` (15 -> 7), `func_8013D5FC`, `func_8013B488` (134 -> 75 funcdiff lines).

- **Place-handler table readers are one family.** Every DATE handler that stores two three-byte result rows
  `*(F5F * N + D_8015Bxxx + D_800CA2FC * 3 + k)` into `D_800CA22C[0..2]`/`D_800CA21C[0..2]` matches with the
  `func_80139944` idiom: `GameState *g = &D_800E6280;`, the table declared `u8 *` in `include/ovl/DATE.h`, and the
  reads written `*(g->unk_F5F * N + D_8015Bxxx + choice_offset + k)` (FAKE: the pointer view stops uopt CSEing the
  multiply, the original repeats `multu` per element). The bit test before it stays a plain field
  (`D_800E6280.unk_1BC[F5F].unk_14[k]`). Converting m2c output mechanically took about a minute per function; the
  T-0018 "hoist" row of `func_801366A4` (family) is solved by it. `func_80137154` (three shops, 18 reads) is the
  same shape but still 13 instructions short in its counter branches.
- **A store forwarded to a call argument through an element view.** `D = 0x14; f(0, 0x14)` with one `li a1,20`
  for both: `(&D_80122D08)[0] = 0x14; func_80085B3C(0, (&D_80122D08)[0]);` (FAKE). The same view makes
  `D_80122CDC = 1; return 1;` share `li v0,1` (`func_80137DF4`).
- **A local copy of a selector puts the variable first in `bne`.** `u8 sel = *(u8 *)&D_8015B68C;` in the else
  chain gave `bne v1,a0` (`func_8013D8B0`). The opposite also holds: a compare on the field itself puts a
  register constant first (`bne v1,v0` with `li v1,9`, `func_80139664`).
- **Byte view to stop a CSE.** Two reads of `D_800CA2FC` in one block were CSEd into `$v0`; reading the second one
  as `*(u8 *)&D_800CA2FC` lets ugen reuse its own load as the original does (FAKE, `func_801393DC`).
- **Leftover argument registers.** m2c reads constants that live in `$a0`..`$a2` at a call as arguments:
  `func_8007ED84(0x4529, 5)`, `func_80084D3C(0x38, &D_800E6280)` and the first `func_80048390()` take no extra
  argument; check the callee and the asm before keeping them. A constant shared by a multiply and a compare in an
  argument register (`multu a0,a1` with `li a1,6`) is a `u8` local (`six = 6`) when the call does not take it
  (`func_801324DC`).
- **Halfword views.** `D_8015BF18/1C/20` are halfwords (`sh` only); their declarations are now `s16`.
  `D_800CA15C` is read with `lh` in DATE: `*(s16 *)&D_800CA15C`.
- **Frame slot from declaration order.** An unused or register-held byte local declared before `s32 prev` puts
  `prev` at 0x28 instead of 0x2C (`func_80139664`, marked FAKE when unused).
- Blocked (rows in [[data/t0018-cases]]): `func_80139834` and `func_80139664` keep a post-call `unk_110A` in the
  wrong register or with a hoisted `lui`; `func_80137DF4` selector `$v1` vs `$v0`; `func_801324DC` one schedule
  slot; `func_8013E6A4` (old value of `D++` in `$a0`), `func_8013BEB8` (third chain value in `$v0`),
  `func_8013AF1C` (3E/40 range priority), `func_8013B208` (two `s16` locals 4 bytes lower; any extra local grows
  the frame by 8) were retried with new ideas and no gain.
- Tools: a side-by-side wrapper around `funcdiff.diff_lines` and a variant runner (one funcdiff per variant, about
  3 s) did most of the work; a full-unit draft whose instructions match but whose address differs (other drafts
  change the layout) is a candidate match, confirm it with the guard removed. autotweak (3 runs) found nothing.

## TT overlay, batch 7 (T-11800)

bulk_m2c rerun (no match, 52 drafts ranked), then two Opus passes. Pass 1 on the guarded drafts: 3 matched
(`func_80142350`, `func_80149BB0`, `func_80145370`), two closer. Pass 2 on the best pipeline drafts: 7 matched
(`func_8014CFEC`, `func_80149400`, `func_801491F0` and its twin `func_8014ACEC`, `func_8013AE3C`, `func_80138B90`,
`func_80132198`). Stayed off the wave-1 TT files. Ticket [[tickets/T-11800-tt-batch-7]].

- **Empty `if (x) {}` (FAKE) is a priority lever.** It adds a use and stretches the live range without code. Placement
  decides the effect: in a `default` case before the calls it lowers a range's priority (more blocks), so it is coloured
  later (`func_80142350`); next to the existing uses it raises it (`func_8013B4A0`). `if ((s32)p != BOUND) {}` on another
  variable gives a loop bound a saved register without raising the counter (`func_801491F0`/`func_8014ACEC`).
- **Reserving a register in a loop:** a local assigned before the loop and tested by an empty `if` inside it holds a
  register there (`func_80145370`, phantom a0/a1).
- **Odd rebases from record walks.** `D + 0x10CC` came from walking fixed-stride records (`o = base + first; ...;
  o += stride` repeated), which uopt folds (`func_80149BB0`). Typed record indexing `((TtAnim14 *)(p + 0x38))[i].f`
  folds unrolled copies into offsets where a pointer IV or `p + i * k` does not (`func_80138B90`).
- **Loads and reloads.** `*(s32 *)&G` stops uopt reusing an earlier load of a global pointer (FAKE; `u16 **` views and
  `volatile` did not work; `func_80149400`). A pointer walk avoids reloading a global base after aliasing stores.
- **Call arguments as register holders.** A pointer the original keeps in a1 across a call can be passed as an extra
  argument of an unprototyped callee (FAKE; `func_8013AE3C`).
- **Store pairs in the wrong order with the right registers:** put both on one line (setUV0 style; `func_8014CFEC`).
- **Constants vs locals in the preheader:** locals are scheduled before hoisted constants, so an `n = 8` local cannot
  reproduce an original where 8 comes last; an expression used twice (`i + 0x2B`) becomes a CSE temp coloured before the
  locals (`func_80132198`).
- **Fully unrolled short loops:** IDO unrolls a 4-trip flag loop completely and hoists its constants (`func_8013AE3C`).
- Tools: `regdiff -v` (allocation order, priorities, constrained/unconstrained) found most levers; order_search fixed
  `func_80148E7C`'s store order; autotweak's line join finds one-line store pairs. A body-splice + `funcdiff --resolve`
  loop ran at about 4 s per try.

## TT overlay, batch 8 (T-11810)

Opus pass A on eleven untried bulk_m2c drafts: 7 matched (`func_80149B20`, `func_801369F4`, `func_8013EC64`,
`func_80149EA0`, `func_8013F9E0`, `func_80135870`, `func_8013C41C`), `func_8013A0DC` guarded at 16 lines. Opus pass B on the
closest guarded drafts: no match, but the rules below. Ticket [[tickets/T-11810-tt-batch-8]].

- **Call-argument constants share a register only under two conditions** (scratch compiles with the build's IDO 5.3):
  (1) the same constant type: under `-cckr` a constant compared with, stored to or used as a case of a u8/u16 value is
  unsigned, so the argument must be `NU`, `(u16)N`, or go to an unsigned or unprototyped parameter (an `s32` argument
  never joins; in `s32` contexts a plain literal does); (2) another use in the call's own basic block. A use in an earlier
  block, a nested `if` or a switch dispatch never joins, so an original that reuses a dispatch constant in a later block
  (`func_8013A0DC`) is not reproducible. After joining, the temp is precoloured to aN only if aN is free over its whole
  range, else `move aN,temp`. When an argument register seems to reuse a constant, first check whether the callee takes
  arguments at all (`func_80142774` and `func_80084D3C` take none).
- **Store forwarding of a u8 field:** `p[k] = X; switch (p[k]) { ... p[k] = 0; ... } use(p[k]);` gives the original
  `andi vN,zero,0xff` / `andi vN,tX,0xff` copies (`func_8013C41C`).
- **Inline ABS without temporaries** keeps a large constant in one register used in both arms (`func_80149EA0`).
- **Stack layout:** in a function that keeps locals on the stack, every declared local gets a slot in declaration order
  (first declared = highest address), register-allocated ones too; a FAKE pad array then places the frame
  (`func_8013F040`).
- An empty `if (x) {}` as the last statement before a `break` can emit a real `beqz`; put it before an existing statement.
- A local assigned the same constant in two places is not propagated; it becomes a real high-priority range (not a useful
  lever).
- `cc -S` (ugen output before as1) explains delay-slot and preheader order: PRE insertions go at the end of their block,
  loop-invariant `li`/`la` after the init moves, and as1 then hoists the `lui` (`func_8013B4A0`).
- IDO 5.3 propagates every zero form tried (`0U`, `(u8)0`, `x * 0`, chains, zero locals) into `$zero`, so originals that
  chain zero stores through a register that later holds another variable stay open (`func_80147FD0`, `func_8014C5F8`).
- Earlier record-walk and twin idioms (T-11800) matched three functions here on the first try; typed record indexing
  (`TtAnim14`) cleared the T-10030 blocker of `func_8013F9E0`.

## TT overlay, batch 9 (T-11820)

One Opus pass on never-attempted TT functions and the drafts T-11810 did not reach: 3 matched (`func_8013F3B4`,
`func_801333D4`, `func_8014B274`), 3 guarded at 6 to 17 lines, 3 not kept, 2 not reached (`func_80142AF0`,
`func_80146E10`). Ticket [[tickets/T-11820-tt-batch-9]].

- **Constant-trip loops are peeled then unrolled.** IDO peels the remainder and unrolls the rest: 11 iterations become
  1 peeled + 5 x 2, 6 become 2 + 4. An index offset (`i + 1`) shows as constants in the peeled copies and `a0 + k` in the
  unrolled body; a leftover `sw 1, 0x28(sp)` is the peeled counter (`func_8013F3B4`, `func_80136830`).
- **Hand-written switch for m2c failures.** `func_801333D4` (m2c put case labels inside else-if arms) matched with the
  switch structure written from the asm: case 0x40 plus a 0x70..0x80 jump table, inside a `for` loop with `continue`.
  `g[0x25]++` followed by a reread matched where a `k = ++g[0x25]` local did not.
- **as1 rewrites what the C seems to say.** It copies a join instruction into both branch delay slots, so a ternary
  `+= (c) ? 0x60 : -0x60` looks like duplicated arms; and it moves loads above an `addiu base, N`, adjusting their
  offsets, so negative-offset C after `p += N` produces positive-offset loads (`func_8014B274`).
- A dead final `addiu` on a walked pointer means the source had `q += stride` blocks. A call result held in a local keeps
  the induction-variable increment after the call.
- A `u16` versus `s16` store view splits a hoisted constant into two webs (`func_80146000`).
- Adding a K&R or `void` prototype for a function that callers used implicitly can change their code: declaring
  `func_80133AD0` changed `func_80140920`, so that call stays implicit.
- Twins found by reading, not by `neardupes`: `func_8013F3B4` / `func_80135870`, `func_8014B274` / `func_8014B088`.

## T-12010 Codex cx1 main helpers

InitMouse now matches 16 linked bytes. The historical missing-range blocker was configuration, not an unknown C idiom: every reference to the two pointer slots belongs to 80059710, and the contiguous mouse state ends at 800E36EC with padding to 800E36F0. An explicit owned-data range and generated island let the two-word zero-initialized pointer array be defined in C, which shares the original lui $at. Other mouse data stays as generated INCLUDE_RODATA. Full configure/build passes 27/27, headers and globals OK. See [[tickets/T-12010-codex-cx1]].

The main entry loop func_80042134 matches 404 bytes using GameState *g = &D_800E6280 only for the final g->unk_110F loop condition. Keep earlier accesses direct: the original holds this base in s2 across the whole loop but still uses direct addresses for earlier selector reads. Its two later-defined void helpers need shared prototypes.

func_80049044 matches 112 bytes with a local u8 *p = &D_800E6280.unk_03A and loads/stores through *p. Direct singleton member reads put the raw brightness in t6/t7 before addition; the pointer view puts it directly in v1, matching both clamp branches. The permuter stripped score stayed nonzero (50), but the full translation unit is exact; verify promising nonzero candidates in context.

func_80059938 matches 232 linked bytes with ordinary indexed coordinate updates and a named pointer-table slot before reading the mouse packet: slot = &D_800E36C0[port]; p = *slot;. This yields the original v0 index and v1 packet pointer; direct indexing swaps their allocation. The old void prototype concealed a real incoming port parameter, now explicit in its matched caller.

The cx1 deep pass retains file_checksum_ok (4 listing lines), func_800422C8 (8), load_tpage_buf_lock (29) and func_800460FC (17) behind guards. func_800460FC uses a shared four-byte word/byte union, a bounded retry loop and a pointer to the ring read index; these recover the original restore and decrement path. Local homes and zero stores remain different; its retry-zero chain is marked FAKE. Enlarging the location buffer reaches 13 lines but lacks source evidence, so it was rejected. An unsigned comparison improves the loader score to 19 but changes negative-index behavior, so the signed 29-line draft is retained. Larger 47560/45414/455C4/476C0 drafts remain scratch-only (91/128/533/1036 rows). Fresh m2c's nested-switch reconstruction for 476C0 creates duplicate case labels; --no-switches is a useful compiling starting point. Full trial details and evidence are in [[tickets/T-12010-codex-cx1]].

## T-12020: cx2 TAIIKU deep pass

Three exact linked matches, 1,300 bytes, in `src/ovl/TAIIKU/80133C80.c`: `func_80135DF8` (324), `func_80136EC0` (380), `func_80135AD0` (596). Each passed the TAIIKU SHA1 before its immediate commit. No new FAKE constructs were needed. The copied-table assignment, signed switch selector and actor-field store ordering respectively resolved the matches; see [[tickets/T-12020-codex-cx2]].

All fifteen listed functions received a full-file compilation pass. Final guarded drafts: `func_801346FC` has eight +/- listing rows (four register positions); `func_801368B8` has four rows (one scheduling swap); `func_80136BB4` has four rows (one store-order swap). The animation update improved from 98 to four rows by copying eleven u16 limits, loading the frame byte into a used u8 local, and ordering the final frame-five assignment before the actor sprite. Its remaining f03 store/f0C clear reversal survived 130 additional increment and line-boundary variants. The counter swap survived 337 variants. Traced regdiff on the decrement case identifies competing priorities; signed/unsigned selector lifetimes and combined predecrement conditions did not resolve it. Existing FAKE in that inherited draft remains labeled T-10890.

Broad drafts were not retained: `func_80137228` (best 60 rows, high-half alias hoisting), `func_80137C44` (138, slot-loop allocation), `func_80137544` (103, selector/constant allocation), `func_80135480` (124, local-table frame and pointer reuse), `func_80135830` (135, frame/loop allocation), `func_80137EA8` (254, finish-line state/actor loops), `func_801365C8` (228, timer-loop allocation), `func_80133CFC` (74, loader table-copy placement), and `func_80138224` (232, three actor-initialization loops). Oversized inherited guards for 37228/37C44 were restored to plain INCLUDE_ASM as required. Drafts and detailed experiment logs remain ignored under build/scratch.

Useful type evidence for follow-up: D_801492A0 stores are s16; D_801492C4 is loaded/stored as a word although its canonical declaration currently says u8; D_801492DC's loader copies seven words although the current array declaration is six. The latter two declarations were not changed because the loader remains assembly. Actor scale at offset 0x1C must use a halfword view of pad_1C. Main function aliases identified in the loader: Fade_Out_Init, Fade_Out_Main (no arguments), load_palette and search_load_tpage_buf_lock.

## Codex cx3 matching pass (T-12030)

Ticket [[tickets/T-12030-codex-cx3]]. Added **5,200 verified bytes across twelve functions** in src/main/8005A0B0.c:

| Function | Literal bytes |
| --- | ---: |
| join_club_message | 544 |
| func_8005BE8C | 704 |
| func_8005D448 | 292 |
| func_800603C0 | 324 |
| system_settings_init | 360 |
| func_8005E150 | 368 |
| func_8005DEA0 | 376 |
| func_8005FDEC | 380 |
| func_8005F09C | 400 |
| func_8005BCEC | 416 |
| func_8005C5BC | 516 |
| func_8005CF6C | 520 |

All promotions were checked against literal linked bytes and the main executable SHA1, then committed. The save/load initializer family contributes 1,036 bytes using natural indexed arrays and the established FlagsWork24 aggregate. The byte range at +4 is used for slot state, so its former padding member is now `unk_04`; no new aggregate or scalar alias was introduced.

Two useful acceptance lessons: the earlier BE8C permutation scored 140 in the stripped harness but matched in production after exact rescoring; and normalized-zero guarded comparisons can still fail literal proof because other active drafts change layout. BCEC, the menu twins, and BE8C were promoted experimentally and accepted only after the default build and literal proof passed. Grouping each width/height store pair on one source line resolves BCEC's historical scheduling gap. A correct void prototype fixes F09C's extra return-address reload.

Marked fakematches remain in join_club_message (selector view), D448 (stack reservation), E150 (initialized empty counter test), F09C (scalar-byte clamp view), and BCEC (stack reservation/store grouping). The other new matches use ordinary typed C.

One near miss is retained: `pre_syogatu1`, 15 diff output rows, with typed buffers and a marked frame reservation. Its remaining blocker is cached buffer-address CSE. `func_8005A410` was restored after coordinate-loop/unroll/spill trials (best 62 diff rows); `func_8005CA94` was restored at 57 rows after typed-slot, result/view and bounded search experiments. Full evidence is in build/scratch/ and the ticket; new compiler-gap/resolution rows are in [[data/t0018-cases]].

Final Docker configure/build passes all 27 SHA1 targets, headers and globals. `sync_protos.py --check-branch` reports zero new disagreements (15 known). Fresh default-build literal comparisons pass all twelve functions. Logs: build/scratch/final-build.log, final-protos.log, final-matches.log and final-near-miss.log. Ticket is In Review, pending the required standards review.

## T-12040 Codex cx4 RPG battle helpers

func_80132EE0 matches256 bytes after replacing a saved m2c local with direct reads of D_8015EDB0.unk_04. The original reloads the counter after calls; the saved draft incorrectly preserved its entry value, adding a spill and changing behavior. Current aggregate and scalar views both score zero, but the aggregate agrees with neighboring helpers. Linked bytes and overlay checksum verified; see [[tickets/T-12040-codex-cx4]].

func_8014E300 matches692 bytes with a shared RpgMotion30 view of the EC28..EC57 coordinate/displacement work range. Both source and destination accesses must use the aggregate: changing only one side leaves the load/store scheduling gap. A local pointer and direct casts both match; the local pointer is retained. This resolves the old T-10310 scheduling case without a compiler change.

func_801321D0 matches376 linked bytes using direct global selector tests and a direct switch after the optional call. The m2c-saved selector local concealed the original post-call reload and introduced an unnecessary range-test temporary/goto. Later C helpers need explicit shared prototypes once promoted.

func_8013415C matches1044 linked bytes after both explicit increment-result temporaries become compound assignments and direct global tests. The 22-row phase/divisor register swap looked local to the negative multiplication block, but traced uopt showed competing webs from later increment copies. Changing only one of the two increments worsens the diff to66; changing both is exact. No artificial register forcing or dead code is needed.

The same two compound-increment changes also match the func_80134570 twin (1044 linked bytes), with independent linked-byte and checksum verification.

func_8015A1F0 matches1252 linked bytes through the same direct-reload/compound-counter pattern. Its case9 m2c var_at pseudo-variable should become a normal if(counter == 0) counter = 60 followed by the global arithmetic expression. Retaining a separate negation variable leaves eighteen rows; removing it while still retaining a counter local changes allocation elsewhere. Expressing the actual global update directly matches without FAKE code.

func_80132348 matches1404 linked bytes after two compound counter updates reduce147 rows to12, then a shared RpgMotion20 displacement/rectangle view preserves the final load/store schedule. Applying the view throughout the function is necessary; a view on only the close pair changes allocation elsewhere. This reuses both cx4 discoveries on the largest assigned movement state machine.

The cx4 pass ends with seven matches/6068 linked bytes. Existing func_8015A744 remains guarded at17 rows after one bounded record/global-view retry. The saved func_8015A8B4 draft is not safe to retain: its tail reads the next record while the original preserves the current coordinate; corrected forms leave50 rows, dominated by a shifted sequence after early a0 setup and missing copy. Its apparently better24-row unsigned variant changes signed comparisons/division and was rejected. func_80132B34 still constant-folds the original's derived coordinate expressions; func_801328C4 still differs in aggregate-copy address CSE, local layout and loop allocation after natural initializer/counter/loop variants. Both remain assembly-backed, scratch-only. See [[tickets/T-12040-codex-cx4]] for measured logs.

## T-12050: cx5 menu/text deep pass

One exact match: `menu_bar_show`, 208 bytes, plain C without FAKE. Linked relocation-aware comparison and main SHA1 pass. The prior T-10010 seed needed the canonical `MenuSelection5` and interleaved `GsRecF90[16][2]` views; read the packed global directly into mode/page/flag locals. Caching a packed color or base pointer changed scheduling. Seed provenance: retained T-10010 scratch `main-menu_bar_show/declaration-order-probes/best.c` beneath session `43264b98-bb59-4c1b-ad99-a2084d55fcf0`.

All 14 functions attempted; 13 remain unmatched. Retained guards: `menu_bar_color` 17 differing listing rows (shared lui/data ownership), `menu_check_1` 27 (base/selector registers, inherited FAKE duplicate pointer), `x_taku_menu_set` 24 (three array homes eight bytes too high). Added canonical `s16 D_800B40F0[4]` declaration for its row table. Array dimensions, local order, ANSI/K&R signatures, unused padding and aggregate variants did not resolve the stack layout; no padding retained. Removed the inherited `menu_set` guard, now 102 rows. Its argument/loop experiments reached 101 safely; a 73-row unmasked argument variant changes semantics and is rejected.

Larger candidates stay in ignored scratch: `func_8005215C` (1260 bytes) 335→219 rows after reusing disjoint m2c temporaries; an empty-test FAKE reaches 195 but remains too broad. Regdiff shows field-load lifetimes and frame/register differences, not an isolated allocator fix. `xa_wait` (580 bytes) reaches 49 safe rows; narrowing `F61+2` to a byte reaches 44 but breaks values 256/257 and is rejected. `func_80052F68` (1200 bytes) stays 327 rows across bounded loop variants: target peels two then unrolls four, draft rolls. `get_h_tokimeki` (1600 text bytes plus 24 rodata) improves 817→637 using indexed flag stores; target rolls the initial seven flags and hoists seven threshold bytes, while the draft unrolls and reloads thresholds inside cases. Explicit bound locals and case-local value/weight pointers did not close it. These need structural/type evidence before further mutation sweeps.

Other repaired drafts: `menu_girl_taku_set` 58 rows (array homes +12 and flag-load scheduling), `sndisp` 77 (buffer homes, saved registers, forwarded first byte), `sndi` 332, `get_near_menu` 1424, `func_8004FC10` 752. Corrected m2c pointer strides, byte buffers and canonical fields before scoring. Broad drafts are not production C. The external AS1 scheduling research was read as a future lead; no uncalibrated tracing result is claimed here. Full-TU scoring and saved diffs are under `build/scratch` in cx5; traced uopt was copied from the parent-provided r9-uopt trace variant and used for state-machine/threshold diagnostics.

## T-12070: TACO particle actor loop

`func_8015B59C` matches 1,940 linked bytes. Reconstructing by-value TcPos calls and using unsigned flag stores restored repeated particle emissions. Replacing m2c pointer/byte-offset induction temporaries with a natural indexed for loop and removing a switch temporary restored pointer allocation and the original frame. Direct typed fields change alias/reload allocation; the byte-offset views are explicitly FAKE-labeled. Production literal funcdiff and TACO SHA1 pass; [[tickets/T-12070-codex-cx7]].

`func_8015C718` adds 1,340 linked bytes in T-12070. Its final six-row switch gap disappeared with a direct u16 dereference, while `(u16)` applied to an s16 load retained v0 instead of t0 despite identical load width/sign. Removing saved m2c actor pointers restores reloads after random calls; swapping the commutative coordinate/random additions restores scheduling. Byte-offset actor views remain FAKE-labeled; default linked funcdiff and TACO SHA1 pass.

## Role-aware twin constants (T-12210)

BUNKASAI func_8014FFE4 and func_8015052C match from func_801456E0 and func_80145C28; BUNKAKEN func_801386C0 matches from func_801475CC. The near-copy tool rejected excess literal occurrences because character ID 7/6 also appears as `case 7:`/`case 6:`. Instruction correspondence maps only runtime character IDs (7 -> 6 or 6 -> 4); state labels remain unchanged. Script/sector constants map separately. All three pass literal linked verification (592 + 920 + 840 bytes) and both unit SHA1s; no FAKE changes. See [[matching-breakthroughs]] and [[tickets/T-12210-matching-bottleneck-research]].

## Substantial BUNKAKEN handlers (T-12300)

In BUNKAKEN/8013BFE0, func_8013C0AC, func_8013C634 and func_8013CA6C match after normalizing GameState aliases, using one initialized `GameState *g = &D_800E6280` for every state access, and expressing character flag bit2 tests/stores through the existing `CharFlags.f.b2`. Direct `.w & 4` and byte-OR forms retained register/temporary differences. The ambiguous old symbol D_800E6480 is character 1's flag byte; its use must resolve to the existing record. Correct u8 declaration of overlay flag D_80155F50 removes the final two instruction rows in C0AC. Three literal linked matches total 3,604 bytes; whole BUNKAKEN SHA1 passes. This is a source/aggregate idiom, no compiler change or fakematch.

### RPG_BAT counter reconstruction (T-12300)

`func_8014A660` (1,316 literal linked bytes) needs explicit nested switch cases for the final counter values rather than m2c's default-with-if shape. This yields exact control flow but 31 v0/v1 substitutions. Consistent scalar pointer views of the existing aggregate member (`*(s32 *)&D_8015EDB0.unk_04`) remove those substitutions; plain members, direct scalar/local copies and s32-without-return do not. The redundant cast is marked FAKE for register shaping. Production literal proof and RPG_BAT SHA1 pass. This does **not** transfer to the standalone D_8015EDE0 timer in guarded func_80145CC0: same-type/unsigned pointer, array and record views all retain 58 differing positions. Preserve this negative result rather than treating the cast as a general rule.

`func_8015C2CC` adds 972 linked bytes in T-12070. As with C718, a direct unsigned selector load is significant. Explicit case 0 preserves the original jump-table range 0–16; otherwise IDO narrows it to 11–16. The final two load ordering differences disappear when the case-13 call and following flag load share a source line. Byte views, case boundary and line grouping are FAKE-labeled; TACO SHA1 and default literal linked funcdiff pass.

T-12070 handoff: three matches total 4,252 linked bytes. Existing guards remeasured unchanged at B1B8 16 rows, C0A0 9, BF70 32. ACCC 411, BD30 128 and CE64 73 remain scratch-only; user priority shifted to the separate 8,672-byte dialogue target before further small-function passes. Clean configure/ninja passes all 27 checksums, headers/globals; branch prototype audit reports 0 new / 15 known disagreements.

## T-12090: cx9 BUNKA_SD byte-first pass

Six verified matches, **4172 text bytes**, plus124 bytes of jump tables. All ten targets attempted. Every match has exact production linked proof and passing BUNKA_SD SHA1, with immediate commits. Final package verification is recorded in [[tickets/T-12090-codex-cx9]].

| Function | Linked bytes | Source pattern |
|---|---:|---|
| func_80132428 |448| Canonical GameState fields and missing overlay forward declarations repair the old stripped score-zero dispatcher. |
| func_80132FF0 |532| Explicit case4 restores the five-entry jump table. FAKE:24-byte name-buffer capacity reproduces original stack homes; source capacity unknown. |
| func_80132940 |1144| Reused palette/setup index; explicit case4; u16 intermediate frame index before subtract2; typed DisplayActor44. |
| func_80138D48 |952| Unsigned record bytes and32-bit frame induction; image metadata assignment before index updates; for-clause increment, with i=2 then frame=15 initialized before the loop. |
| func_80133204 |828| Explicit fifth switch case, typed word-table pointer arithmetic, separate if/else assignments instead of m2c shared goto. Linked size includes8 trailing padding bytes. |
| func_801388D0 |268| Neighboring s32 no-return state-machine idiom, canonical byte +=1, and no spurious argument to func_80048390. |

Retained guards: func_8013987C14 differing listing rows (pointer setup and missing clear-zero live range); func_8013441826 rows (base/selector allocation and shared0x80 constant). Saved func_801344A8 remains57 rows and stays assembly. No new FAKE beyond the disclosed name buffer.

Largest remaining target func_80139290 is1516 text bytes plus88 table bytes. Repaired canonical records, natural independent case stores instead of two m2c shared gotos, and a timer local improve509→89 rows; repeating the delta expression instead of its local reaches87. Regdiff reports377 versus378 instructions: target copies the switch result v1→a1, then allocates the character byte in t0; the draft coalesces that copy and takes a1 for the character, shifting later mask values. Explicit copies, signed/unsigned views, assignment-in-switch, first-case splitting, bounded empty tests, and return liveness do not close it. Broad candidate stays scratch-only. These are measured failures, not proof that matching C is impossible.

Native AS1 diagnostics were calibrated on the952-byte near miss. A scratch copy of frame_pass applies the unchanged transform and injects `-R -xbbdbg 8` only at real as1 invocation; complete stock and traced objects are byte-identical. Captured native output has8958 lines and a reached loop DAG. Direct `-Wa` forwarding fails the production frame wrapper's numeric-argument parser, so do not add it to production flags. The remaining six rows were equal-priority scheduling nodes; natural for-loop increment placement closed them. Retained evidence in cx9 `build/scratch/as1-stock.o`, `as1-trace.o`, `as1-native.log`, `38d-clean.c` and `38d-final.c`. No production tool changes or diagnostic patches are part of the match.

## T-12900 small OPTION and main audio pass

OPTION func_80137E38's word-pair table needs ordinary word-array indexing:
`((s32 *)*e)[D_801230D0 * 2 + 1]` and the following word. Byte-offset
expressions compute the same addresses but number the load/shift temporaries
differently. This resolves the guarded 360-byte handler without a FAKE form.

Main func_8007A868 (188 bytes) needs consistent Work80125D10 member accesses
to preserve the early flag loads. Put the independent unk_08 assignment after
the four flag-mask updates; uopt retains the original merged stores and load
scheduling. No dummy local or source-line scheduling trick is needed.

Main func_80079F00 (264 bytes) calls func_8008BFB0 with signed-halfword second
and third arguments. Preserve other files' old-style declaration with a scoped
MAIN_API_OVERRIDE guard. Pass the aggregate halfword directly; explicit s32
casts generate different conversion chains. Assign unk_1C from unk_1E before
the flag-word update to retain the original high-immediate scheduling.

All three have full-TU funcdiff --resolve literal linked-byte proof; the
affected main/OPTION binary SHA1 checks pass. See
[[tickets/T-12901-small-option-main-first]] and [[tickets/T-12902-small-draft-triage]].

## Native AS1 diagnosis of dialogue_speak_part (T-12500)

The 8,672-byte guarded function still differs at eight instruction positions (16 minus/plus rows), comprising two rotations of `li a0,-40` past the signed-Y conversion and two outgoing string-pointer stores. [[tickets/T-12500-largest-dialogue]] uses native AS1 `-R -xbbdbg 8` diagnostics after the existing frame transformation, preserving the normal cvt/layout pipeline. Ordinary, capture-only and diagnostic full-TU objects are byte-identical: SHA256 `d7802f1a49e6ffc06219dcc8863d5c6f492a30715b0d75851d11751a5cba2566`. Thus all existing matched functions in the TU also serve as controls. No production compiler change was made.

The exercised native trace shows the constant and signed shift as ready nodes with zero downstream cycles. The shift is attached to the preceding increment statement and is selected first. Moving the increment into the call changes the input DAG but adds a narrow assignment copy, changing dependency priority. Fine line-boundary, coordinate type/scope, constant-expression and control-flow probes did not improve the inherited candidate. This is a measured plateau, not proof that the function cannot match.

Reproduction evidence is retained under `build/scratch` in the isolated `largest-dialogue` worktree: `baseline-score.py`, `baseline.diff`, `as1-capture.py`, `trace-compile.py`, `as1-{stock,trace,inline,ycomma,blocks}/`, each `dialogue-*.py` probe and its log, and combined-search JSONL ledgers. The diagnostic wrapper injects options only at AS1 spawn time because the frame shim does not parse the extra option value. Compiler inputs/outputs and transformed binasm remain ignored; no copyrighted binary data was committed. No source candidate has been promoted from this pass.

## DATE movie and scene records (T-12100, 2026-10-15)

The cx10 pass gains 5,628 verified bytes in 489D0, E3E8, A4DC and D298. Existing aggregate/data ownership resolves shared-LUI misses; exact details and proof logs are in [[tickets/T-12100-codex-cx10]]. E3E8's anonymous initialized data requires literal linked-byte proof rather than trusting two unresolved normalizer rows.

For large DATE movie handlers, natural `D_801217D0[i + k]` record loops recover register choices that m2c pointer temporaries obscure. Removing redundant inner returns and moving the shared animation tail after the main switch recovers original branch structure. The 4,044-byte 47958 guard now differs in four fade register rows; 462B0 reaches the original 950-instruction count but remains too distant to retain. Callback source-line grouping affects AS1 load order and is explicitly FAKE where required.

AD9C's motion stream uses 12-byte records and its angles use 8-byte SDK SVECTOR entries. Selecting the motion record before the MATRIX copy and ordering translation additions correctly reproduces temporary creation order. Using `s16[arg0 * 4]` for angles consumes an additional ugen temporary despite producing the same final shift; SVECTOR indexing removes it. The retained guard differs only at two saved-pointer stack slots (five rows). D298's three 68-byte record copies match with the existing DateParticleView44 and a genuine record-pointer local declared before the local scene; no padding is needed.

## DATE conditional table dispatch (T-14100, 2026-10-15)

Natural local table aggregates plus a genuine selector declared before the table match func_8015A288 and func_80154004 (340 new bytes). Range callbacks may mutate GameState.unk_110A: preserve sequential conditions and reload the selector between callbacks rather than combining disjoint ranges into one OR test. Both promotions have full-TU default literal-byte and DATE SHA1 proof; see [[tickets/T-14100-date-near-matches]]. Scoped fade-width probes did not improve the existing four-row 47958 guard and were restored.

For DATE func_801449D4, explicit pointers to dynamically indexed local table slots lose alias information and induce selector reloads after every callback-slot store. Direct `tbl.f[idx]` writes recover that information, but the recovered draft still differs in frame, source temporaries and scheduling; this is a useful hypothesis rather than a match. The 21-table func_80144E20 recovery is type-valid yet distant. Both larger drafts and temporary header context were restored; evidence is in [[tickets/T-14100-date-near-matches]].

## OPTION settings bitfield family (T-14400)

OPTION 8013A550.c settings handlers require actual bitfields and a consistent local GameState pointer. func_8013BFF8 improves canonical masks 60 -> bitfields 32 -> pointer 0 full-TU rows. Its schedule word has low five bits, bit 24, and bits 25-28. func_8013AF10's +0xF4 player word uses bits 8-11, 12-15 and 16-20; typed bitfields reduce its diff to 30 rows and the pointer removes the rest. Both pass default literal linked-byte proof (952 and 1196 bytes), with OPTION SHA1 OK. Row tables advance by one word; raw m2c drafts wrongly express a four-byte advance as a four-word advance. See [[tickets/T-14400-option-menu-families]].

The same family extends to func_8013B568 (2320 verified bytes): complete character bitfield view and independent case stores replace m2c's cross-case goto. Both global and local-pointer forms score zero; direct fields retained. Byte 3 is an eight-bit field even though the menu helper permits values up to 999, so assignment naturally truncates as the original does. The fourth sibling 8013A7EC still needs structural/induction recovery; do not count or reuse its distant score as a match.


OPTION audio row handlers func_80138C20, func_80138FC0 and func_80138DF8 add 1400 literal verified bytes. Each row is 16 bytes (63 XA/SE rows, 58 sequence rows). Reuse the actual signed loop index across clearing, numeric drawing and text drawing. Ordinary index increments rather than m2c's next-index copies restore saved-register roles; a real local pointer to the text-row array plus a conventional two-induction-variable for loop puts the row-table base before counter resets. Pointer declaration alone stays six rows away. The third handler matches the transferred source and an independently written conventional clearing loop. All three pass default full-TU literal proof and OPTION SHA1; [[tickets/T-14400-option-menu-families]].

### T-14900: shared motion aggregate preserves source store/load order

[[tickets/T-14900-rpg-near-matches]]: RPG_BAT func_80148B08 recovers the existing `RpgMotion20` view rooted at EC50, including displacement dx/dy and rect words at EC68/EC6C. Independent scalar declarations let IDO move the EC6C load before the EC50 store; one genuine shared aggregate view restores the original ordering. Both member and local-pointer forms compare at zero full-TU rows; pointer form passes default literal linked bytes (1,128 including eight padding bytes) and RPG_BAT SHA1. Reuses the established 80132348 layout rather than inventing padding or volatile barriers. Void forward declaration avoids a generated implicit-int repair while preserving exact bytes.
### T-15000: duplicated final stores instead of m2c's shared goto

KANGEI `func_80135CE0` (432 inventory coverage and linked proof bytes) matches as an ordinary `if`/`else if` with `D_80120657 = 0` at the end of each branch. The six-score m2c seed shared that store through a goto into the second branch, which removed the target's first branch store/branch sequence. Restoring the source-level branches fixes the entire difference without type tricks or FAKE. Default linked bytes, KANGEI SHA1 and header/global/prototype guards pass. See [[tickets/T-15000-kangei-near-match]].

### T-15300: BUNKASAI character flag stores

BUNKASAI `func_801438E4` (852 inventory coverage and default linked literal bytes) needs the existing character 7 flag fields `.unk_0C.f.b12 = 1` and `.f.b13 = 1`. Byte-OR stores, including explicit read-modify-write assignments, reuse the byte load's base register and shift seven temporary registers (14 diff rows). CharFlags already documents the original bit-field signature: load the byte into another register than its LUI. The canonical aggregate/bit-field view matches without FAKE. Default linked proof, BUNKASAI SHA1 and all guards pass; see [[tickets/T-15300-fresh-structural-near-matches]].

## T-15400: mixed Shift-JIS escape in generated Unicode strings
BUNKASAI func_801431D4 had a two-row resolved difference entirely in one label relocation. The generated mixed Unicode/escaped string emitted literal ASCII x83 in place of Shift-JIS byte 0x83. Writing the actual label as ordinary Unicode パソコン占い produces the original bytes through the existing conversion pipeline. Canonical GameState access alone did not solve it. Default literal proof and overlay checksum confirm the 440-byte function. Preserve initial drafts and inspect actual emitted bytes before changing control flow for string-only differences.


### T-15600: packed record load and old-counter branch timing

[[tickets/T-15600-fresh-near-matches]] accepts RENSYU func_801320E0 (688 literal linked bytes) and SHOUGATU func_80137144 (568), with both overlay SHA1 checks. RENSYU's word inference at D_800E68EE has wrong width/scaling; actual target lhu accesses bytes +2/+3 of the existing four-byte GameState.unk_66C record. SHOUGATU compares the old counter then increments, faithfully represented by `counter++ >= 0x401U`; the automatic postincrement rewrite had moved the comparison to the new value. These are genuine storage and control-flow corrections. RPG_BAT80155A70 remains a single delay-slot NOP gap, archived without artificial shaping.

## T-16210: RPG_BAT wave-2 package w2-p01 (5 of 5 matched)

[[tickets/T-16210-w2-rpg_bat-801454f0]] matched RPG_BAT `func_80155A70`, `func_8013AF1C`, `func_801454F0`,
`func_801568A4` and `func_8013AD50` (5,072 bytes) from the T-14500 census drafts; the four owned files
(`801454F0.c`, `80155A70.c`, `80156870.c`, `8013AD50.c`) hold no asm any more.

- **Callers saw `func_8013E7C0` and `func_8013E97C` as implicit `int`.** A counter `D += k; if (D >= n)` right after
  a call to one of them is in `$v1` in the original and in `$v0` with the header's `void` prototype (regdiff: "an
  interfering range holds `$v0`"). The call's `$v0` result only exists for an `int` callee, and it moves the counter's
  range off `$v0`. The definitions in `8013E4D0.c` stay `void`, so a file matched with the int view defines
  `RPG_BAT_OVERRIDE_<name>` before its includes and declares `s32 <name>();` itself; `RPG_BAT.h` wraps the two
  prototypes in `#ifndef RPG_BAT_OVERRIDE_<name>` (same shape as `MAIN_API_OVERRIDE_`). Matched `func_801568A4`
  (`func_8013E7C0`) and `func_801454F0` (`func_8013E97C`). The same symptom in other RPG_BAT drafts (a counter or
  stored value in `$v1` where the draft has `$v0`, after a call to a header-`void` function) is worth this test first:
  copy the C file and `include/ovl/RPG_BAT.h` to `build/scratch/`, change the prototype in the copy and run
  `regdiff.py --src` on the copy.
  Since T-16530 no copy is needed: `regdiff.py RPG_BAT:func_801454F0 --proto 's32 func_8013E97C();'` (and the same
  option on `funcdiff.py`) compiles the file with that declaration in place of the header's; on the pre-override
  file it turns the `$v0`/`$v1` divergence into MATCH ([[decompile-workflow]], "Trying another prototype").
- **`func_80155A70`: `s32` with no return.** The single `nop` in a `bgez` delay slot (T-15600 left it as a gap) is the
  K&R implicit-int shape: `v0` is live at the exit, so as1 does not move the next block's `lui v0` into the slot.
- **`func_8013AF1C`:** both scroll cases written as `D_8015EDB4 += 3; if (D_8015EDB4 >= 0x21)` keep the first load of
  case 1 in `$v0`; m2c's temporaries (or the compound form in one case only) put it in `$a1`.
- **`func_8013AD50`: one base symbol, three FAKE views.** The split `lui t8; lw t9` of `D_8015ED98` needs the
  absolute address (as `func_8013D030`, T-4010); the switch load of `D_8015EDAC` and the load of `D_8015EDA0` stay
  after the stores to `D_8015EDB4`/`D_8015ED98` only when both accesses name one symbol (`D_8015EDA8[3]`,
  `(&D_8015ED98)[2]`, T-4070). With `D_8015ED8C[1]` (T-4070) this is more evidence that the words from about
  `D_8015ED8C` to `D_8015EDB8` were one aggregate in the original, not the separate variables `8014E780.c` defines.
- Census drafts were close: after placing the body only types (`D_8015EC60..6C` are `D_8015EC58.unk_08..14`,
  `RpgRec18`), new `u8` main-exe bytes (`D_80121316`, `D_8012135A`, `D_80120E0A`) and the prototypes of two helpers
  defined below their caller (`func_8013B2F0`, `func_8013B534`) were missing.

## T-11670: main 8006CB30 schedule helpers (wave 1 w1-p11, wave 2 w2-r11)

[[tickets/T-11670-w1-main-8006cb30]]. Wave 1 matched `_schedule_init`, `geko_judge` and `schedule_girls_gakko`; wave 2 matched `func_8006EA40` (292 bytes) and moved three drafts closer. What worked:

- **`(start + i) % n` gives the original's strength-reduced index** (`func_80072D70`): the original keeps `start` on the stack, copies it once (`mfhi t8; sw t8,0x38(sp); move s1,t8`) and increments `s1` with the counter. A separate `idx = start; ... idx++` local makes the remainder a uopt range (`mfhi a0`) and shifts every later t-register by one. `for (i = 0; i != 11; i++)` instead of a do-while puts the `li v1,11` reload after the counter increment, as in the original.
- **Frame by declaration order** (`func_80072D70`): once any local lives in memory, every declared local takes a slot top-down in declaration order (the rule in this page). Declaring the four register locals first and the two spilled ones (`flag`, `start`) last reproduces the 0x50 frame and the 0x38/0x3C slots without a pad.
- **A value-returning call used as a statement blocks `v0` in its block** (`func_8006EA40`, matched): with `s32 func_80053418()` uopt kept the loop counter, record base and count out of `v0` (they start in the call's block), giving a1/v1/a0/v0 for a0/v0/v1/a1. A `void func_80053418();` override in `8006CB30.c` (`MAIN_API_OVERRIDE_func_80053418`) gives the original allocation; the other three callers in the file still match. The same trick moved `func_80072998` from 16 to 14 lines (K&R `s32 func_80068938()` frees `v0` for the constant 9). Check regdiff's allocation list for a range that skips `v0` next to a call whose result is unused.
- **Buffers of `func_80053418`** (`func_8006EA40`): `s32 count; s32 i; u8 rank[20]; s16 points[16];` reproduces the 0x68 frame and the 0x2C/0x4C buffer addresses, the same sizes as `special_event_select`.
- **One condition for all `return 0` paths** (`func_8006EA40`): the original's inner tests branch straight to the epilogue with `move v0,zero` in the delay slot. That shape comes from a single `&&` chain over the outer and inner tests, with one `return 1` and one `return 0`; nested `if`s or a separate inner `return 0` give `bnez/b` pairs with nop slots.
- **Promoted decrement through the `GameState` pointer** (`func_8006D6E0`): `if (gs->unk_71C != 0) gs->unk_71C--;` gives the original's `andi t3; move v0,t3; sb v0` and the reload in both predecessors; a `u8 idx` local gives an extra `andi a0,v0,0xff`. A forwarded `(u8)0` (`andi v0,zero,0xff`) after `p->unk_00 = 0` means the original re-reads the field, not a local `c`.

What blocked the rest (rows in [[data/t0018-cases]]):
- `func_8006D6E0` (5 lines): the original keeps the second loop's start index as a copy (`move v1,v0`) before the strength-reduced pointer; IDO folds it into the promoted `unk_71C`. Types, a second index variable, `while`, autotweak (150 s) and a 420 s permuter run did not keep the copy.
- `func_80072D70` (22 lines): the original loads `unk_118.unk_02` straight into `lim`'s `s1` and the selector byte into `v1`; IDO keeps the `0x800E0000` base in `v0` and a 1-use local (`M277-28` in regdiff, not visible in the code) takes `v1`, pushing the selector to `a0`.
- `func_8006EB64`/`func_8006EEC4` (8 lines each): regdiff shows the T-10880 `variant` local as a phantom range in `v1` (no instruction uses it) that pushes the +0x388 flag word to `a2`; without the local the word gets `v1` but `unk_110` and `unk_F68` shift (256 lines).
- `func_800726F0` (20 lines): the original promotes `unk_03D` into `v1` (load, both tests and the 1/2/3 stores: 2 uses, 6 defs). In plain C the K&R `cvt` of the byte takes the two tests as its own expression, so the global has 1 use and 6 defs and is not worth a register (regdiff: uncoloured, -ve save); `tools/cvt_pass.py` does not cover it because the first access follows calls (entry rule). A `u8 h` with `D = h = k` gets the promotion and the `v1` stores but puts `h`'s load in `v0`. Separate `extern` symbols for the date bytes compile the same as the struct fields.
- `func_80072998` (16 lines): the K&R declaration of `func_80068938` and month locals change nothing; month stays in `v0`.
- `func_8006D138` (left `INCLUDE_ASM`): the original loads `&D_800E6280` into `a0` and the +0xF64 word into `v0` once before the first switch; a local copy of the word is sunk into the cases by IDO and the base is rematerialised per block (282+ lines).
- `func_8007132C`, `func_8006D7EC` (4.3 and 4.7 KB): not attempted beyond reading the census drafts; both need a full aggregate rewrite of the T-14500 draft first.

## T-16230: DATE 80150010.c finished (wave 2, w2-p03)

Ticket [[tickets/T-16230-w2-date-80150010]]. All four functions of the work list matched in plain C, so `src/ovl/DATE/80150010.c` has no `INCLUDE_ASM` left: `func_801511B4` (332 bytes), `func_80150460` (340), `func_80150ED4` (400), `func_80150C78` (548), 1,620 bytes. Each had one or more rows in [[data/t0018-cases]] (T-4010, T-9110, T-9320, T-9580, T-9780); none was a compiler gap. What fixed them:
- **Name the real aggregate, then chain through its elements.** `D_80121FD4` is `D_801217D0[57]` (Rec24, stride 0x24; `unk_10A5[57]` is its GameState byte) and `D_801206D8` is display record 98 of `D_8011ECD0` (DisplayRecord44). In `func_801511B4` the four stores of `D_800B593C` (`lui v1; lbu v0`, the old "hoisted lui" row) match only as one chain whose targets are all elements: `D_801217D0[57].unk_14 = D_801217D0[57].unk_15 = D_801217D0[57].unk_16 = ((DisplayRecord44 *)D_8011ECD0)[98].unk_07 = D_800B593C;`. With separate statements the value and its address share `$v0`. With the scalar `D_801206DF` in the chain, none of the 24 target orders matched (best: two stores swapped). The store order of a chain is not simply right to left, so try the orders when the stores come out shuffled.
- **Copy elements of one array, not two splat symbols.** `func_80150460`'s 0x44-byte copy matches as `((DisplayRecord44 *)D_8011ECD0)[98] = ((DisplayRecord44 *)D_8011ECD0)[96];`. Copying `*(T *)D_801206D8 = *(T *)D_80120650` left the last `lw`/`sw` pair of the copy in the other order (T-9320, T-9780).
- **A `GameState *g` pointer fixes allocation ties.** In `func_80150ED4` (three-year date check, as in DATE 80132000.c) the swapped `$a0`/`$v1` of `unk_03F`/`unk_040` and the `% 180` value in `$v0` instead of `$v1` all go away with `g->`. In `func_80150C78` it moves the `unk_10F8` load from `$a0` to `$a1` and removes a one-register shift of every ugen temporary in the loop. With `g`, IDO still addresses the fields directly (`lui; lw %lo`). The pointer is a local, so it changes the frame: in `func_80150ED4` it takes the slot the T-3330 `idx` scalar would take above the 39-entry function table (`FnTbl39`, new).
- **Spilled locals and the frame.** `func_80150C78` uses all nine callee-saved registers, so uopt keeps one of the two equal colour arguments in memory (`sw fp,0x64(sp)`, reloaded in the loop). The variable needs a stack home, and only some locals get one (a `phase` local costs 8 bytes, while `grey` and an `angle` local cost nothing). The home offset and the frame size followed the declaration order. `order_search.py` with `/* ORDER */` around the declarations found the order (`col0, i, col1, g, grey, offset`). The call results are inlined as the arguments of `func_80049FF0`, as the frame size requires.

## Wave 2 package w2-p02: main 800420D0 and 8007C030 (T-16220)

Ticket [[tickets/T-16220-w2-main-8007c030]]. Three matches, 804 bytes: `func_800422C8` (268), `make_three_select`
(348, one FAKE) and `junban_init` (188). `_sprite_set_box_shade_tarao` is now a guarded draft at 30 funcdiff lines
(188 before). `dialogue_speak_part` stays at 16 lines.

- **Assign to the global, then read the global.** `func_800422C8` (8 lines, tried in T-8060, w7-4, T-9730, T-10020,
  T-10510, T-12010) and `junban_init` (34 lines, T-9150, T-10310, T-10620, T-11590) were T-0018 `regorder` rows.
  Every earlier draft kept the value in a local: `id = func_80098370(); ... D_8011ECA0 = id;` and
  `r = D_800E6280.unk_10F8 % 3; ... D_800CA2A4[0] = r;`. The original writes the global first and reads it back:
  `D_8011ECA0 = func_80098370();` and `D_800CA2A4[0] = D_800E6280.unk_10F8 % 3; if (D_800CA2A4[0] == 0) ...`.
  uopt promotes the global, sinks its store into each branch (before the call, or to each exit), and colours it the
  way the original does (the `a3` copy, the remainder in `v0`). Symptom: the draft stores a local to a global in
  every branch, and that local sits in the wrong register. Row added to [[register-cheatsheet]].
- **Frame of `make_three_select`.** The slots fill the locals area from the frame top in declaration order, so an
  unused `u8 pad[0x44]` between the three `s16` locals and `buf[3][0x40]` gives the original 0x170 frame with the
  buffer at sp+0x64. It is marked FAKE, since the real locals are unknown.
- **`_sprite_set_box_shade_tarao` (POLY_G4).** Findings that got the draft from 188 to 30 lines:
  - `(arg6 >> 16) & 0xFF` keeps each shift in a ugen temporary (`t6..t9`), as the original does. ugen drops the
    `and` for `sb`, so the `and` value is a uopt range that never shows up in the code but still takes a colour.
    Bare shifts, chain assignments and `u8` locals all become uopt registers instead.
  - The reload of a spilled pointer goes to the argument register of the call that uses it last (a1 for
    `AddPrim(ot, p)`), unless something live is in that register. The original has `p` in a2 because arg6 is in
    a1. That needs the `and(arg6)` range coloured fourth, so the draft writes `r2` before `b0`: uopt numbers
    ranges in ucode first-mention order.
  - The colour words are `u32` (`srl`), and `main_api.h` now says so. A `GameState *gs` local takes the place of
    the earlier FAKE pad word, which kept `p` at sp+0x28.
  - Still open: as1 loads arg6 before arg5 and puts the `r2` store first. The original loads arg5 first and
    stores `r2` after `g2`. A leading `p->b2` store fixes the load order but moves the stores. About 6,000
    single-function compiles over statement order, line grouping and mask/cast forms found nothing below 6 diff
    lines.
- **`dialogue_speak_part`.** New probes with no gain: an `s32` coordinate (with and without the pad word, it
  changes the frame and loses `s1`), narrowing inside the call, a separate `y` local, an `x` local (constant
  propagated), and the increment on the `if` line or joined by a comma. Still 16 lines.
- **Tooling.** A standalone compile of one function through `tools/cc.py` with the project headers takes under a
  second, which makes brute-force searches over statement order and line grouping practical (2 workers). The
  regdiff trace shows uopt colours (1=v0, 2=v1, 3=a0, ...). The final register can still differ when ugen
  retargets a reload, as with `p` above.

## T-11650: NAME_ENT 80132000 (wave 1 w1-p09, resumed in wave 2 as w2-r09)

[[tickets/T-11650-w1-ovl-name_ent-80132000]]. Wave 1 matched 8 functions (`func_80132A38`, `func_80132E80`, `func_80135168`, `func_801357F4`, `func_80135F74`, `func_80136688`, `func_801377E8`, `func_80138488`); they were merged onto the wave-2 base unchanged (NAME_ENT sha1 OK). Wave 2 matched 8 more, all rewritten by hand from the census drafts: `func_80139684`, `func_80135650`, `func_80137CA0`, `func_80139440`, `func_80132B78`, `func_80133510`, `func_801338DC`, `func_8013635C`.

What worked:
- **Pointer position is actor 0 of the record pool.** `D_8011ECF6`/`D_8011ECFA` are `((DisplayActor44 *)D_8011ECD0)[0].unk_26`/`.unk_2A`, and the scalar-looking `D_80120xxx` bytes and halfwords are fields of records 96..124. Written through the record pool, the original's load order comes back by itself (`func_80135650`: the cursor load stays above the six record stores; the T-10320 row on it was a separate-symbol artefact).
- **Implicit int (`s32`, no `return`) also fixes delay slots.** In `func_80135650` the only difference of the typed draft was as1 filling two delay slots with `lui v0` from the fall-through. With v0 live at the exit as1 may not do that and leaves the original's `nop` / fills from the target (`lw ra`). Look for a fall-through `lui v0` that the original did not hoist.
- **Scalar byte views on both bytes of a switch handler** (`func_80139684`): `switch (*(u8 *)&D_800E6280.unk_110D)` with `*(u8 *)&...unk_110D += 1` gives the `$v1` chain, and writing the brightness byte as `*(u8 *)&D_800E6280.unk_03A -= 4` / `= 0` (no local) lets the cvt pass promote it to `$a0` with the `andi; or` narrowing the original has. The u8-local form matched case 1 only while the selector was a member (copy propagation is off once the selector is a scalar, `cvt_pass.py`).
- **`setWH(sprite, 8, 0x10)`** keeps the original's order of the `w`/`h` stores in a SPRT drawer (`func_80137CA0`); separate statements put them in source order. `NAME_ENT.h` now includes `psx_types.h`; `D_8014D014` is the digit (u, v) table.
- **IDO unrolling matches when the loop is written plainly**: the nine `GsRec0FC` records (`(&D_800E6280.unk_0FC)[i]`, one peeled store plus two groups of four) and the 12-record actor initializer of `func_8013635C` (unrolled by four, copy order 1, 2, 3, 0). `CharFlags` bit-field stores (`.f.b1 = 1; .f.b3 = 1;`) give the original's temporary numbering where `b[0] |= 2` is one temporary short (`func_80139440`).
- **Promoted selection byte**: `func_80132B78` reads and writes `D_8014CC70` directly in every statement; IDO promotes it to `$a3` with the store before the call and the reload at the loop end, exactly like the original. The only gap was the frame (one more home), kept as a marked `FAKE` unused local.
- **strncpy without its length** (`func_80133510`): the original calls `func_800AE090` with two arguments at the last two levels of the help-text splitter, so the original had no prototype. The file now overrides it (`MAIN_API_OVERRIDE_func_800AE090`, `void func_800AE090();`), and the matched callers still match.
- **Frames of local buffers**: locals get homes in reverse declaration order (first declared highest). `func_80133510`: `s32 i; u8 *p; u8 *text;` above six `u8 line[0x24]` buffers reproduces the 0x130 frame; `text` must be the last scalar so it sits right above the first buffer.
- **A pointer local can move the spill slot** (`func_801338DC`, the same splitter with a `switch` head): with `p = text + 0x20` the spill of the line address lands 4 bytes too low; writing `text + 0x20` in each call (a compiler temporary) puts it where the original has it. The frame was then 8 bytes short: an unused two-word local marked `FAKE` restores 0x110.

Left open (guarded unless noted):
- `func_80132320` (14 lines): masked byte in a ugen temporary, T-0018 row.
- `func_80134B94` (10 lines): implicit int fixed the loop-init hoisting; the third name's end-pointer register remains (T-0018 row).
- `func_80138C1C` (INCLUDE_ASM, about 200 lines): frame solved (`u8 buf[0x18]` declared after five scalars, counter reused so the second clear loop stays rolled, third copy loop `p != D_8014CC84 + 10`, u16-style bit-field masks come out of the u32 `GsDateBits`-like view); blocked by the hoisted second space byte (T-0018 row). Draft kept in the session scratch only.
- `func_80138F60` (INCLUDE_ASM): the new-game type assignment needs `CharFlags.f20` writes, a signed `(day * 3 + month) % 11`, and a bits 9-10 field of the date word (`GsDateBits` has none yet; `NameDate` in 80139740.c calls it `week`). Store order of the `f20` byte against the three table stores is still wrong (about 460 lines).
- `func_80134110`: the actor-pool view does not change the known `$v0`/`$v1` gap.
- `func_80134248` (2256 bytes, guarded T-10890 draft about 455 lines): not attempted in wave 2.

## T-11690: main 800674B0 (wave 1 package w1-p13, resumed in wave 2 as w2-r13)

Ticket [[tickets/T-11690-w1-main-800674b0]]. Matched: `club_list_menu`, `func_80067C28` (wave 1), `func_8006A2CC` (3112 bytes, wave 2).

`func_8006A2CC` (profile card setup, switch on the month `unk_03F`, branches on the day `unk_03E`) went from 116 differing lines to a match in four steps:
- Each record triple `D_8011ECD0[n].unk_18 / unk_26 / unk_2A` after the loop sits on one source line, as if from a macro. as1 schedules by line, so this alone fixed the in-branch store order (the loop's own triple stays on three lines; joining it breaks the loop).
- Case 1 sets `kind` before the flag store, like the other cases; that fixed two ugen temporaries.
- Dead reads of both selectors at the top (`i = unk_03F; n = unk_03E;`, both overwritten later, no code emitted) make uopt promote `unk_03E` and give `unk_03F` `$t0`. Only these two globals, only before the first call, and both are needed: one alone, a neighbour field or a pointer view does not work. FAKE.
- With the dead `unk_03F` read the two `if (unk_03F != 8)` in cases 8-10 compile to `beq t0,a1`; the original has `beq a1,t0` (8 is a hoisted constant in `$a1`). Writing `8 != x`, `!(x == 8)` or `8U` changes nothing. A named `u8 eight = 8;` puts the constant first, as in NAME_ENT `func_80148C20`. The new local must take an existing pad slot (it replaced the frame pad above `i`), or every spill slot moves down 4 bytes. FAKE.

Lesson: when regdiff reports "not allocated ... global X (-ve save)" for a narrow selector the original reloads after every call, try a dead read of X at the very top of the function. Likely reason (not traced): the read comes before the first call or branch, so the entry rule of `tools/cvt_pass.py` applies to X and uopt promotes it. The pass docstring also explains the operand order: the original puts a constant held in a register first when it compares a narrow global.

Not matched:
- `func_8006BC28` (guarded, 17 lines, wave 1 draft): the original loads `D_800B6708` once into `$v0` before the `arg0` dispatch. Local copies (s32, s16, u16, s8), a returned copy, dead reads at the top and an assignment to the copy in the clamp body all leave the load in each arm. A u16 copy without casts did hoist the load, but its compares become unsigned (`sltiu`); any `(s32)` or `(s16)` cast sinks it again.
- `func_800676AC` (guarded, 11 lines): IDO moves arg0 to `$a1` at entry and hoists `li a0,27` into the bnez delay slot. Index local, if/else, goto, a named 27, s16/u16/u32 parameters and a dead read did not change it.
- `func_8006C988` (INCLUDE_ASM): best source shape found: records indexed by the count `n` (`D_800E6280.unk_75E[n]`, `n++` last) with a separate do-while counter `i`, `switch (t) { case 0: case 1: case 2: case 0xF: f = rec.unk_00; }` then `l = rec.unk_00` (gives the original PRE load in both paths), and an unused third parameter (gives `sw a2,8(sp)`). That is 70 lines: `found` is numbered after the PRE temporary and gets spilled. Dead copies `pf = found; pl = last;` at the top keep both parameters in `$a0/$a1` (46 lines); left are the PRE temporary and the record pointer swapped between `$t0` and `$t1`.
- `func_8006BA40` (INCLUDE_ASM, best 63 lines): arms written `x = D_8011ECD0[0].unk_26; x += speed;` reproduce the original in-place `lh v1; addu v1,v1,v0`, and `D_800B6708` as a plain global keeps it in `$v0`. Left: the original computes `lui t0,%hi(unk_2A)` for the first clamp right after the speed test (one row early), the -0x96 clamp goes through `x` in `$v1`, and the `== 0xAC` test reads a `move v1,v0` copy.
- `func_8006A044` (INCLUDE_ASM): two unused parameters give the original home stores `sw a0,0(sp); sw a1,4(sp)`. Writing every update on the global itself (`D_800E6280.unk_F64[1] |= 0x40;` and so on, no local) reproduces the original's extra store of the flag byte before each `== 0x61` test (a store-back of the promoted byte). A dead read `x = D_800E6280.unk_F64[1];` at the top then puts `unk_03F` in `$v1` and the byte in `$v0` like the original (148 lines). Left is the T-0018 `promo` shape of `func_80067DD4`: the original widens the byte into a temporary before each `ori` (`move t5,v0; ori t6,t5,k; andi v0,t6,0xff; sb v0`) and loads it first into `$t6`, IDO uses `$v0` directly.
- `func_8006B0C8` (INCLUDE_ASM): a natural draft (compact by `ScheduleEntryFlags.active`, `bzero` the rest, December fix, selection sort with `keys[8]`, swap through a `GsWord` temporary, undo the fix) compiles to the same blocks but 556 lines: the original frame is 32 bytes larger, the first loop is a pointer walk compared with `&unk_54C[8]`, and the inner sort loop is unrolled by 4 (IDO unrolls by 2).
- `func_80067870`: dead reads of the promoted globals at the top make it worse (329 lines against 304).

## T-11640 wave 2 (w2-r08): TAIIKU 80138950.c

[[tickets/T-11640-w1-ovl-taiiku-80138950]], resumed under [[tickets/T-16200-matching-wave-2]]. Merged the wave-1 branch (7 matches) and matched 10 more: `func_8013CD34`, `func_8013FC5C`, `func_801400D8`, `func_8013A314`, `func_8013C978`, `func_80139610`, `func_8013D9B4`, `func_8013F948`, `func_8013BEF8`, `func_8013C0E4` (6,876 bytes). Guarded drafts: `func_80138AA8` (22 register lines), `func_80138DF8` (30 lines).

What worked:
- **Stack-table frames.** IDO gives every declared local a slot from the top down; a local table that sits 4-12 bytes below the frame top needs that many scalars declared first (`func_8013FC5C`: loop counters before the table; `func_80138AA8`, `func_80139610`: FAKE unused scalars). A chain assignment `A[f()] = B[i] = v` adds compiler temporaries at the bottom of the frame; two statements (`A[f()] = v; B[i] = v;`) removed 8 bytes in `func_80138AA8`.
- **Constant types decide register sharing.** In `func_801400D8` the constants stored to the 0x01/0x06 bytes of the display records must not share registers with the unsigned switch constants of the later loops. With `u8` fields they merge; TAIIKU's own view `TaiikuActor44` (signed bytes at 0x01/0x06) matches. Changing `DisplayActor44` in `game.h` breaks BUNKA_SD, so the view stays TAIIKU-local.
- **Record views, not scalars, for record 104.** `func_8013F948` matches with `((DisplayActor44 *)D_8011ECD0)[104].unk_18` (promoted with the original's writebacks and reloads) and `D_801499CC[i - 1]` for the runner flags; earlier attempts with an `s16 D_80120888` scalar failed.
- **Peeled copies.** `D_80149DF4[0] = rgb.v[0]; for (i = 1; i < 5; i++) ...` reproduces the "first word, then four words through `s1 = 1`" copy of `func_80138AA8`.
- **Statement order fixes ugen temporaries.** `func_8013C978`: store `D_801206BA` before `D_801206BE` in case 4, and `D_801206BA = -0x90;` before `D_80120697 |= 1;` in case 3. A jump table with an empty last case needs the explicit `case 7: break;`.
- **Callees that ignore their arguments.** `func_8013D9B4` never reads `$a0/$a1`; the `$a1` in its caller `func_8013D788` holds a promoted global, not an argument. It is `void func_8013D9B4(void)` now.
- `func_8013D9B4` needs a FAKE `(u8)` narrowing before `+ 8` in the v coordinates (the original allocates one more ugen temporary there).
- **The D_801499A2 counter family: no volatile flag.** `func_8013BEF8` and `func_8013C0E4` match with u16 pointer views of both the counter (`((u16 *)&D_801499A0)[1]`, which IDO promotes without the extra CVT copy of the plain field) and the flag (`((u16 *)D_801499B0)[1]`). The FAKE `volatile D_801499B2` of `func_8013C33C` stops the `lui` of the next flag load from moving into the branch delay slot and costs a match wherever the flag is read after a block. Both views are marked FAKE. `func_8013B78C` gets to 50 lines this way (the original promotes the counter before `D_801206BA`); `func_8013BCEC` to 80 (the else-path writeback `sh $v0` is missing).

What blocked the rest (rows in [[data/t0018-cases]]): the rest of the D_801499A2 counter family (`func_8013BCEC` writes the promoted counter back on a path IDO does not; `func_8013B3B0`, `func_8013B94C`, `func_8013B608` reload it with hoisted `lui`s, while the view is promoted and a volatile alias loses the `lui`s; `func_8013B78C`, `func_8013B200` keep it in `$v1`/`$a1` where IDO colours other globals first); the `flags` prologue of `func_8013D2B4`/`func_8013D788` (uopt register in the original, ugen temporary in IDO, every later temporary one off; `func_8013D788` also does not promote D_80149960 in the original); the 20x4 grid loops of `func_80139AB0`/`func_8013DDB8` (inner loop unrolled by 2 with `multu` in the original, fully unrolled by IDO); `func_8013F3A4` (IDO hoists `&D_8011ECD0` into `$a3`). `func_801406A8` and `func_80139FE4` keep their wave-1 guarded drafts. Not attempted: `func_8013E894`, `func_8013E1FC`, `func_8013EC60`.

## T-16510: recurring m2c defects, rewritten and rescored

[[tickets/T-16510-m2c-defect-rewrites]]: `tools/draft_fixes.py` (also `tools/m2c.py --fix-defects`) writes the fixes of [[decompile-workflow]] "Recurring m2c defects" as alternatives; `rescore` compiled them in the full unit for the 50 T-14500 drafts it changes (results in `drafts/T-16510/README.md`). What the numbers say for a draft you are about to match:

- **Shift-JIS as escapes**: the text form always helped or tied (13 bodies; `BUNKAKEN func_80145844` and `BUNKASAI func_801431D4` reach 0, as T-15300 and T-15400 found by hand). Octal escapes and the text compile the same, since `cc.py` turns the text into octal; only the `\x` hex form changes the bytes. Do it first.
- **Old-value post-increment compare**: `D++` in the condition helped every time (11 bodies, `SHOUGATU func_80137144` 14 to 0). m2c.py's own pass missed the case where the asm adds in place (`addiu $v1, $v1, 1` after the compare); it now sees it.
- **Shared-store `goto`**: a coin flip (13 better, 13 worse of 26 bodies); try both. Direct stores won where each case stores its own constant or address (`main func_80079070` 450 to 35, `BUNKA_SD func_80139290` 509 to 133). The per-case initializer tables with `var = D_a; D_b = ...; block: D_c = var;` (the load moves behind another store) split 5 to 7: `GEKO func_801327AC` 223 to 2 and `SHUGAKU func_8013282C` 183 to 116 gain, `GEKO func_801355E8` and `VALEN func_80134030` lose. A loser keeps the early load and one shared store, as the TT block of `func_80132000` does; nothing in the C text tells which.
- **Near match from this pass**: `GEKO func_801327AC` is one row (`sb` for `sh`) after the `goto` rewrite; `*(s16 *)&D_80144C40 = 0;`, the line the initializer in the same file already has (T-9510), gives a full-unit score of 0 (`drafts/T-16510/follow-up/`). Not promoted: no linked-byte proof yet.
- **`s32` without a return**: a dead copy or constant in `$v0` at an exit is in 207 of 334 matched `s32` functions that return no value and in 32 of 5,383 matched `void` ones. Of the 71 T-14500 drafts that show it, a signature-only change to `s32` improved 3, tied 29, worsened 12 and 27 did not compile (mostly a redeclaration against the existing `void` prototype or call); change the header too before judging. Callers that read `$v0` after a `void` draft do not occur in the archive (0 of 1,066).

## Constant-first compares follow aggregate membership, not copies (T-16910)

Census for the T-16900 `cpw` rule (zcopy off plus constant first, only where a procedure copies a narrow signed global
into a local and compares it with a constant). Ticket [[tickets/T-16910-cpw-census]] has the method and the lists.
Scope: 6958 game functions of the split asm, a forward dataflow over registers; compares with the constant in `$at` are
ugen's and left out.

- Narrow signed globals (`lb`/`lh`) compared `==`/`!=` with a register constant: 72 compares in 23 functions. Coupled
  shape (a used `move` copy plus a constant-first compare of the same global): 7 functions, all in NAME_ENT
  `80139740.c` on GameState +0x1093 (the T-11570 family). Constant-first without a copy: 15 functions, 50 compares.
  Variable-first: 6 functions, 8 compares, none with a copy. 10 more functions copy a narrow signed global without
  comparing it with a register constant.
- The order follows aggregate membership: 64 of 64 constant-first compares are on members of GameState, the
  `D_8011ECD0` record table or an indexed array; 8 of 8 variable-first ones are on the two `s16` scalars ENDING
  `D_8013C35C` and SHOUGATU `D_80144E0C`. Other widths lean the same way: `lbu` GameState members 332:10, `lw` scalars
  12:82 (constant-first:variable-first).
- IDO already does this when the C reads through the aggregate. ENDING's table fields compare constant-first only
  through `*(s16 *)&D_8011ECD0[...]` (T-9020) and DATE's `D_800CA22C[]` is an array; the other 5 matched functions with
  this shape declare a scalar and carry a `FAKE` or `(u32)` trick (ENDING `func_8013855C`, NAME_ENT `func_80148C20`,
  BUNKA_SD `func_8013261C`, EVENT `func_80117004`, RPG_BAT `func_80150CD8`). Update (T-16930): four of the five now read
  through the aggregate and lost the trick; only RPG_BAT `func_80150CD8` keeps its `(u32)`.
- The NAME_ENT drafts with the selector read as an element (`(&D_800E6280.unk_1093)[0]`) get the constant-first
  compares from IDO; with copy propagation off for the file (scratch only) `func_8013DAC4`, `func_8013F454`,
  `func_8013FB8C` reach 0 lines, `func_8013D4CC` 6, `func_8013E430` 4, `func_8013F0DC` 8, `func_8013CEC8` 10.
- Verdict: no `cpw` step in `tools/cvt_pass.py`. Open: the type of GameState +0x1093/+0x1094 (element access for these
  compares against the three matched `switch` selectors that need members, [[game-state]]), and a separate census for
  the kept copy (the original keeps `idx = sel` in its own register here; IDO folds it).

## GameState +0x1093 and the selector-copy family (T-16920)

Ticket [[tickets/T-16920-gamestate-1093-type]]. The 7 NAME_ENT selector-copy functions (T-11570, the `cpw` targets of
T-16900/T-16910) match as C with `tools/cvt_pass.py` as built, 2,332 bytes.

- **Read the field through a local GameState pointer.** `GameState *g = &D_800E6280; idx = g->unk_1093; if
  (g->unk_1093 != -1) { ... idx / 7 ... }` gives both of the original's features that looked like compiler
  differences: the constant-first compare (`beq a1,v1`) and the kept copy (`move v0,v1`). `g->field` is an indirect
  load, not a plain variable: uopt puts the constant first and does not fold `idx` into the field. DAC4, F454 and
  FB8C match with no other change; D4CC and CEC8 also need the operand order `g->unk_1118 == idx + 1` /
  `D_800E7D36 == n + 1`; E430, CEC8 and F0DC need a `FAKE` `s32 pad[2]` for the original's 0x38 frame (8 bytes above
  the spill slot when the copy lives across a call; T-11120 frame rule). Register row in [[register-cheatsheet]].
- **+0x1093/+0x1094 stay members.** As `s8 unk_1093[2]`, 15 matched functions change: every `switch` on the element
  gets its selector in `$v1` (original `$v0`), in ETC with an extra `move v0,v1`. For IDO a constant-index element is
  an indirect load like `g->field`; the original's switches compile like the plain member.
- **`D_800E7D15` is a `u8` array** (`lbu`, indexed by the selector in all 7 functions); it replaces the drafts'
  `(u8)(&D_800E7D15)[idx]`.
- **Kept copies in the original** (T-16910 census): used `move` copies of a global load in 42 functions for aggregate
  members (12 matched) and 47 for scalars (27 matched). The matched copies come from ordinary C (4 through
  `GameState *g`, 3 read-modify-write, 5 direct). Copy propagation off for every procedure changes 737 of 5,967
  matched functions, so no zcopy rule: the kept copies are a C matter.

## Pointer reads for kept copies and constant-first tricks (T-16930)

Ticket [[tickets/T-16930-pointer-read-kept-copies]]: the T-16920 reading (a field read through a local pointer is an
indirect load, so uopt keeps a `move` copy of it and compares it constant-first) applied to the other census functions
and to the matched functions that carried a trick. Eight functions match as plain C (4,424 bytes), four tricks are gone.

- **Matches.** main `func_80042AC8` (268 bytes), main `func_80078C48` (920), GEKO `func_80134040` (352), RPG_BAT
  `func_801495DC` (276), ENDING `func_80137730` (888), OPTION `func_80137C1C` (540), OPTION `func_801383C4` (556),
  SHUGAKU `func_80137490` (624). Control with plain member reads (`D_800E6280.unk_x`): main `func_80042AC8` 2 rows
  (`bne v1,a1` where the original has `bne a1,v1`), main `func_80078C48` 21, ENDING `func_80137730` 46, OPTION
  `func_801383C4` 49, SHUGAKU `func_80137490` 94, OPTION `func_80137C1C` 109. So `GameState *g = &D_800E6280;` decides
  these, not a trick. GEKO `func_80134040` needs no pointer: it is the read-modify-write of `D_8011ECD3` that its
  matched siblings (`func_801351F8`, `func_8013882C`) use.
- **A global the original keeps in one register and stores from several branches is the variable itself.** uopt
  promotes it and writes it back in each branch: `ovl_load_lba` in main `func_80078C48` (assigned in every `case`, read
  after the `switch`; [[tickets/T-16220-w2-main-8007c030]]) and `D_800B5A60` in ENDING `func_80137730`. Do not add a local for it.
- **s32 functions that end without a return.** `func_80078C48` and `func_80137730` are `s32` and the last path has no
  `return` (K&R), which keeps `$v0` live at the exit (the "`s32` without a return" row above); their prototypes in
  `include/main_api.h` and `include/ovl/ENDING.h` changed from `void`.
- **A natural extra local closes an 8-byte frame.** RPG_BAT `func_801495DC` declares `rate` first
  (`rate = D_800E6280.unk_10C.unk_02; lv = (rate > 300) ? 300 : rate;`): the copy and the original's 0x38 frame come
  with it; without `rate` the frame is 0x30 and 14 rows differ. The three `FAKE` `s32 pad[2]` of T-16920 (NAME_ENT
  `func_8013CEC8`, `func_8013E430`, `func_8013F0DC`) may be replaceable the same way; not tried.
- **One base symbol for a record table.** In OPTION `func_80137C1C` and `func_801383C4` the three stores at
  +0x2A3F, +0x2A56 and +0x2A58 and the two loads at +0x26 and +0x2A all go through `D_8011ECD0`; as1 keeps the loads
  behind the stores only then (comment in the C). The per-record stores of the loops use `D_80120650`. Declaration
  order `e, sel, i, j` sets the OPTION spill slot.
- **The five constant-first tricks of T-16910.** Four are gone and each function stays byte-identical: EVENT
  `func_80117004` (`s16 *res = &D_8012439C`), BUNKA_SD `func_8013261C` (`s16 *state = &D_801206F0`), NAME_ENT
  `func_80148C20` (`NameRecord44 *rec` at record 96 of `D_8011ECD0`, +0x08 named `unk_08`), ENDING `func_8013855C` (the
  file's table view of `D_8011ECD0`, T-9020). The two scalar globals have no known aggregate, so their pointer is
  still a trick and carries a `FAKE` comment (T-16930); the other two read a real record table. RPG_BAT `func_80150CD8`
  keeps `(u32)`: the original shares the constant 1 in `$v0` with the `case 1` of the `u32` switch, so the compare has
  to be unsigned (through the table view the constant moves to `$at`; an `s32` switch moves the selector, 38 rows).
  ENDING `func_8013855C` keeps its dead-test `FAKE` (T-9410); reading `unk_10F8` through `g` does not replace it.
  The list of "5 matched functions with a FAKE or `(u32)`" in the T-16910 section above is therefore down to one.
- **Misses** (drafts in the ticket's scratch folder): main `func_8007B64C` (52 rows; the count sits in `$v1` instead of
  `$v0` with or without a pointer), TACO `func_801484E4` (65; plain local copied across calls, load in `$a1`, frame 0x30
  against 0x38), EVENT `func_800FEDC8` (44; two register swaps), DATE `func_8013FAD0` (~123), VALEN `func_80133088`
  (193), RPG_BAT `func_801447E0` (174; frame 0x48 against 0x40), main `func_80065B0C` (353). Not attempted: SHOUGATU
  `func_8013FB28`, TACO `func_8014616C`, `func_80146754`, OPTION `func_8013A7EC`, KANGEI `func_80132FF8`, NAME_ENT
  `func_80134248`, ENDING `func_8013A2E8`. They go to the next inventory.

## T-16554: TAIIKU package p4 (80132000.c, 80140810.c, 80142240.c)

[[tickets/T-16554-pt-taiiku-p4]], stage B of the types-first pilot ([[taiiku-family-results]]): started from the T-16550 redrafts with the new `TAIIKU.h` types. 4 of 7 functions matched (3,208 bytes): `func_80132970` (456 B, 9 minutes in), `func_80143DD0` (384 B), `func_80143F50` (956 B), `func_8014280C` (1,412 B). `func_80140A90` (2,020 B) is a guarded draft with 33 differing instructions, `func_8014357C` (664 B) a guarded draft with 20, `func_80142FB8` (992 B) stays `INCLUDE_ASM` (60 differing instructions). The siblings tool found no matched relative (ratio under 0.70) for any of the seven.

What worked:
- **Quick experiments outside funcdiff.** `funcdiff --resolve` takes 25-35 s on a loaded machine; a scratch file compiled with `/opt/ido/5.3/cc -c -EL -O2 -mips1 -G 0 -non_shared -Wo,-nokpicopt -cckr -Xcpluscomm -Iinclude` inside `tools/docker.sh sh` takes about 2 s and holds 5-10 variants as separate functions (read the registers out of `objdump`). That found most of the answers below.
- **Index loop, not pointer walk** (`func_80132970`): `for (i = 0; i < 3; i++)` over `D_80148EC0[i]` makes IDO build the three address constants (`D_80148EC0`, `+2`, `+3`) with separate `lui/addiu` in the original order; the pointer walk with literal addresses gave `ori` and a different `lui` order. The runner byte is reloaded at each use only when the record stores go through a `GameState *gs` (indirect stores kill the forwarded load).
- **A field the original keeps in a register is used directly, with no local** (`func_80143DD0`): the animation frame `D_8014A13C[0].unk_00` is promoted into `$v0` and written back by uopt at the end of each arm, so no self-assignment (m2c's `x->f = x->f`) and the clear `unk_04 = 0` stands in both arms of the `if/else` (the delay slot of the branch takes it). The last two position copies match only as `((DisplayActor44 *)D_8011ECD0)[100].unk_26 = [96].unk_26` (display records, as the neighbours), not as the scalar names `D_8012078A = D_8012067A`.
- **A local read per region raises a global's priority** (`func_80143F50`): `bits = D_800E6280.unk_F88;` at the top of each of the two regions gives the field `$v0` and the hoisted `& 5` expression `$v1`, as in the original; reading the field directly lets the expression (priority 1) outrank the field (0.67) and the colours swap. regdiff names exactly this ("bit 8 outranks bit 6").
- **`lb` for `lbu` with the right allocation** (`func_80143F50`, `func_8014280C`): a flag declared `s8` read as `D_8014A110 != 0` has the allocation of the original but loads with `lb`; `(u8) D_8014A110` (double conversion) and a `u8` declaration both lose the allocation and keep `lbu`. The scalar view `*(u8 *)&D_8014A110` gives both (FAKE, commented).
- **m2c field aliases**: m2c printed `D_8014A13C->unk0`, `D_8014A13C->unk_00` and `D_8014A13C->unk_02` for three different loads in `func_80143F50`; read the asm for the offset. Two table stores `A[f(x[i])] = v; x[i] = v` are two statements with `i + const` written twice (a chain assignment stores in the other order).
- **Loop starting at the struct base** (`func_8014280C` case 6): `D_800E6280.unk_1228[0x15 + i]` with `i` from 0 gives the original's pointer at the base and the displacement 0x1278; `for (i = 0x15; ...)` folds the offset into the pointer. A literal `&D_800E6280` in the function hoists the base into `$s0` and changes every table store.
- **Spill slots**: one FAKE `u8 pad[4]` reserved the top local word (`func_8014280C`; the spills of the tpage loops sit at 0x38 and 0x30).

What blocked the rest (rows in [[data/t0018-cases]]):
- `func_80140A90`: the case 22 block copy of display record 96 to 98 gets its end pointer one ugen temporary early (`$t3` for `$t4`) and every later `$t` register is one off. Harness result: after a strength-reduced loop the block copy hides one temporary fewer than after a call or at the start (end register = destination + 3 instead of + 4); any real store between the loop and the copy (even `D_8014A064 = 0`) restores + 4. No source form of the loop (index, pointer, do-while, `u8`/`s16` counter, record pointer in the loop, record view of the copy) changed it. The `$v0`/`$v1` half of the miss is solved: two callees whose result is ignored are `s32` in the original (`MAIN_API_OVERRIDE_func_80085368`, `func_800853FC`, blocks `$v0` like the `RPG_BAT` rows of the cheat sheet), and the selector `switch (*(s32 *)&D_80122CDC)` (FAKE) keeps uopt from promoting `D_80122CDC` into `$v0`.
- `func_8014357C` (one window of 19 instructions left; the record-1 stores `unk_14.halves.high = 8; unk_1C = 1;` match when written on one source line): the flags byte `D_800E6375 & 0xF` is a uopt variable in `$v1` in the original, a ugen temporary (`$t4`) here, and the s16 rating field is loaded straight into `$v0` where IDO promotes the field into `$a1` and copies. The same family as `func_8013D2B4`; none of `u8`/`s32` locals, a `GameState *`, a scalar view, absolute address, store aliasing helped.
- `func_80142FB8`: the original keeps the tpage-style loop pointer, the constants `1`/`4` in caller-saved registers with a spill around the call (`sw v0,0x3C(sp)`) and only `i` in `$s0`; IDO gives the pointer and both constants `$s1..$s3` (frame 64 against 72). Pointer, index, do-while, `u8` counter, explicit pointer variable: same. `func_8014280C` shows the effect can come out naturally (there both loop variables are `$a2`/`$a3` with spills), and it depends on the rest of the function: removing the nested `switch` of its case 3 gives `$s0..$s2` again.

## T-16552: TAIIKU 80138950.c, types-first package p2

[[tickets/T-16552-pt-taiiku-p2]], from the [[tickets/T-16550-taiiku-types-first]] redrafts, worktree `pt-taiiku-p2`. Three matches (`func_80139FE4`, 816 bytes; `func_80138DF8`, 1,336 bytes and `func_80138AA8`, 848 bytes in the closer pass) and no guarded draft left; the five functions of the D_801499A2 counter family and `func_80139AB0` stay `INCLUDE_ASM`. What the pilot learned:

- **A leaf function is coloured in bit order, and the bits follow the first mention that survives into the ucode.** `func_80139FE4` (matched) had cnt/pointer/flags/record in $a0/$a1/$a2/$a3 in the wrong order. `regdiff -v` prints the bit of every range; moving a mention earlier moves the bit. A real statement before the first loop works (`rec[96].unk_26 = 0` gave the record bit 6), but the same C that is dropped before numbering does not: `if (0)`, an unused local, an address without a load, `n = 0; if (n != 0) {...}`, and two adjacent assignments to one local (the first is dropped). Two dead assignments to different locals that are used later do survive: `m = ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26; cnt = D_800E6280.unk_F80;` at the top of the first loop (both overwritten before use, no code) matched the function. FAKE. Declaration order does not matter. In a non-leaf function (all ranges constrained) this does not help: priority decides, ties go to the lower bit.
- **`A[f()] = v` with the call in the index reserves a second, unused spill slot.** `func_80138DF8` had its stack tables 4 bytes high (30 differing lines); the original has the spill slot directly under the lowest local and no gap above it. Scan: the draft's spill sat at `bottom - 8` whenever a statement of the form `D_800E6280.unk_1228[search_load_tpage_buf_lock(...)] = v` existed in the function and at `bottom - 4` without it. Writing `idx = search_load_tpage_buf_lock(...); D_800E6280.unk_1228[idx] = v;` with a real local `idx` removes the phantom slot and keeps the three scalar words above the tables (`p`, `i`, `idx`): frame, table offsets and spill slot equal the original, 30 lines to 3. The same `idx` replaces the two FAKE pads of `func_80138AA8`. Probe: compile variants with `regdiff --src` and read the `addiu rd,sp,k` bases and the `sw/lw` pair offsets from the disassembly of `build/scratch/regdiff/<func>/draft.o`.
- **The type of a callee parameter decides how a constant with bit 15 set is built.** In `func_80138DF8` case 0 passed `0x801F8000` to `cd_read_sectors(u8, s32, s32)`: IDO emitted `lui s0,0x801f; ori a1,s0,0x8000` (the unsigned constant converted to s32 is split into a hoisted 0x801F0000 range in $s0), the original has `lui a1; ori a1,a1,0x8000`. With `u32 arg1` the argument is the plain constant, uopt pre-colours it to $a1 and the difference is gone (regdiff says so: "the draft does not pass the same value straight to a call"). `funcdiff --proto 'void cd_read_sectors(u8 arg0, u32 arg1, s32 arg2);'` finds it without editing; with the definition in `src/main/800451D0.c` changed to `u32 arg1` as well, the whole tree built with every unit sha1 OK (525 steps), so `u32` is probably the real type. The package did not own that file, so `80138950.c` carries a file-local `MAIN_API_OVERRIDE_cd_read_sectors` and `main_api.h` guards the prototype. Do not run `sync_protos.py --write` for the guard: it rewrote 369 lines of `main_api.h` and dropped other guards; the `#ifndef` was added by hand.
- **The grid loop of `func_80139AB0` is not a lost cause as a loop.** Written as `D[i * 4 + j] = k + (i * 4 + 0x50) * j` over four arrays, IDO unrolls the inner loop by 2 and keeps `multu`, as the original does (the old note says it unrolls completely). What is left is the outer loop: IDO strength-reduces the seven linear expressions of `i` (`-0xA0 - i * 8`, `i * 4 + 0x50`, ...) into induction registers `s0..s5, s7` and keeps `0x14`, `4` in registers, the original recomputes them from `i` (kept in $v0 and `sw`/`lw` at 0x50(sp)) and keeps the four array bases in `s5, s6, s7, fp`: 213 against 203 instructions, 99 differing lines, so the draft is not kept. Open: what makes uopt skip the outer strength reduction.
- **The D_801499A2 counter family is a promotion gap, not a statement-order gap.** `func_8013B3B0` written with the u16 views: IDO promotes the counter in $a1 (8 uses, 2 defs against 10 units), the original reloads it into a ugen temporary at every test (111 lines). `func_8013B608` (counter not promoted, as in the original; 33 register lines, 9 structure lines) differs only in where the `lui` of `D_80149986` is allocated: the original has it in the `bnez` delay slot with the next temporary after the condition (`andi t0`, `lui t1`), allocated before the `x * 80 / 48` chain; here it is allocated after the chain (`lui t0`, the chain takes `t1`), so every ugen temporary after it is one off. Tried without effect: a real `s16` local for the result (it fixed the uopt registers), operand and statement order of the two position stores, a pointer or a scalar for `D_80149986` read first, dead mentions of it, separate scalar globals instead of the struct fields, permuter 180 s, autotweak 120 s.
- **`func_8013B78C`**: draft 45 lines (priority 0.4 of the counter against 0.5 of `D_801206BA`; dead reads to add uses were dropped before allocation, so the numbers did not move).
- **Tools.** `tools/siblings.py --package` reads the list inside the container: a list in the scratch directory is treated as a comma string and fails with "no unmatched function"; keep it under the worktree (`build/`) and write entries as `UNIT/func` (the `UNIT:func<TAB>...` lines of an orchestrator list do not parse). All TAIIKU siblings are below ratio 0.70. `tools/permute.py` and `autotweak.py` found nothing on `func_80138AA8` (22 lines), `func_80138DF8` (3 lines) or `func_8013B608`.
- **Closer pass (Opus).** Both guarded near misses matched. `func_80138DF8`: `ugen -S` (IDO `cc -S` through the `tools/cc.py` environment) shows the pre-as1 order, which already had `move s0,zero` first. The difference was as1's: a `move` in the fall-through block is never scheduled first, but one before the branch fills the `beqz` delay slot. Writing `idx = cd_read_sync_status(); i = 0; if (idx & 1)` gives the slot. The loop then needs no entry test (`for` and `while` leave `sll t9,zero,2` as the pointer start), and its increments need the `for` line (a do-while puts them after the stores). `goto first; for (; i < 18; i++) { first: ... }` does both (FAKE). `#line` reproduces it too but `funcdiff` drops it (`gcc -E -P`). `func_80138AA8`: `idx` did not replace both pads (frame 8 bytes short). The register swap went away with `s32 func_80085E30();`: its unused int result keeps the loop pointer out of `$v0`. It is a file-local `MAIN_API_OVERRIDE_func_80085E30`, likely an implicit int call in the original. The extra frame word is a real local, `p = 0x8018F000;` in case 2.

## T-16553: TAIIKU 801452D0.c (game C), types-first package pt-taiiku-p3

[[tickets/T-16553-pt-taiiku-p3]], package 3 of [[tickets/T-16550-taiiku-types-first]]. Ten functions matched (6,324 bytes; the first nine, 5,244 bytes, in the main pass, `func_80145640` in the closer pass below): `func_8014666C`, `func_80145C28`, `func_80147318`, `func_80147928`, `func_80146A60`, `func_80147F58` from the list, plus `func_80148370`, `func_80146338` and `func_80147594` from the rest of the file. Three stay guarded (`func_8014607C` 20 lines, `func_80145370` 22, `func_80146D18` 31); `func_80147B14` is untouched. Matched siblings were no help (`tools/siblings.py` found none above 0.70); the redrafts only gave the call structure, every match was rebuilt from the asm with the typed records.

What worked:
- **Index loops over typed records, not m2c's pointer walks.** `DISP(n)` (`((DisplayActor44 *)D_8011ECD0)[n]`, one macro at the top of the file) and `D_8014A454[i + 1].f` loops compile to the original's pointer induction variables and negative or shifted offsets. m2c's byte offsets decode as record `n` plus field: 6620 is record 97 field 0x18, 6838 record 100 field 0x26 (not 0x16: check the low bits).
- **Which induction variable ends the loop is decided by statement order.** `func_80148370` and `func_80146338` had the right body but IDO tested the loop on the table pointer or on the byte value instead of on the x position. `tools/order_search.py` over the four or thirteen store lines of the body found the order in seconds (scores 20 to 2 to 0 and 52 to 14 to 0). Run it as soon as the structure is right.
- **Loop bound as the next table symbol.** The original ends loops over word tables with `sltu`/`bne` against the next symbol (`D_8014A4D4` after `D_8014A4C4`, `D_8014A4E4`, `D_8014A4F4`, `D_8011EF34`). `i < n` gives `bne` against a computed end; `&D_8014A4C4[i] < D_8014A4D4` or `!=` reproduces the original, and swapping the two operands swaps the `bne` registers (`func_8014666C`, `func_80147F58`, `func_80147594`). A bound that is another symbol of the same array needs an alias symbol: `func_80147594` uses `extern TaiikuSlot1C D_8014A4A8;` (a splat name inside `D_8014A454[4]`) as the end of its first slot loop, which gives the original's separate `lui`/`addiu` instead of `base + 84`.
- **Declared locals decide the stack table's place.** The table's distance from the frame top counts the scalars declared before it, and every declared scalar takes a slot once something is spilled: `func_80147F58` has one local and a 16-byte table (`i` before `tbl`), `func_80147594` three locals before its 12-byte table. Extra locals below the table fill existing slack until the area grows by 8 (`func_80145C28` needed `j` declared before the spilled `p`). Locals that the original does not have add 8 bytes; use direct expressions instead of temporaries (the `DISP` expressions of `func_80147F58`).
- **Two index variables give two pointer ranges.** In `func_80147594` the slot pointer of the first and the second loop must be different registers; one index shared by both loops makes one hoisted constant for both (22 uses) and every s-register shifts. Using `i` for the first loop and `k` for the second (and `r` for the random value) fixed it. A `k * 0x13 + 0x13` written with parentheses is hoisted as one term (`addiu s5,s5,0x13` before the loop).
- **Chain assignment through the global array, `a2`/`a3` around calls.** `func_80145640` (`D_8014A4F8[i] = unk_1228[slot] = i + 0x80000100;` after `slot = search_load_tpage_buf_lock(D_8014A4F8[i], 1);`, see the closer pass) puts the counter in `$a2` and the element pointer in `$a3` with `sw`/`lw` around the call, as the original does. With the value kept in a local (m2c) IDO uses s-registers instead.
- **Flag word in a local.** `func_80146A60` (listed under T-0018, `$v0`/`$v1` swapped) matches with `flags = D_800E6280.unk_F80;` before the mask tests. `func_80147928` updates the promoted slot word through the global itself, writes the bonus as a ternary (`b` over an empty else) and the random value as `u32`.
- **Struct fields of a one-word array.** `func_80145370` needs `s32 a[1]` for the first stack word (a scalar is promoted to `s0`), a local `j` before it and the destination tables as arrays (`D_8014A4F8[12]`, `D_8014A528[9]`, the second loop indexes `D_8014A528[i]` from 5).
- **Same-line statements.** `k = 0; do {` on one line moved the `move s4,zero` behind the hoisted table addresses (`func_80147594`, as1 schedules by source line).

What blocked the rest:
- `func_80145640` (matched later, see the closer pass): everything matched except the slot of the counter (`sw a2,0x3C(sp)`, the original `0x38`); an unused second declared local grows the frame to 0x48.
- `func_8014607C`: the tail loads `D_800E638E` into a separate global range (`$a1`, then a copy) because IDO propagates the copy into the shift; the original loads straight into the variable in `$v0` and keeps the mask in `$v1`. `s16`/`s32`/`u32` variables, pointer or `GameState *` reads, a repeated read and statement orders do not change it (20 lines).
- `func_80145370`: the two loops that store 8 have the constant in `$v1` and the pointer in `$v0`, the original the other way round. A pointer variable, loop forms, a named constant and a `/* ORDER */` search did not flip it (22 lines).
- `func_80146D18` (31 lines): the table-copy loop stores before the record pointer advance (offset 7110 against 7042), and the zero-search at the end keeps the index in `$v1` instead of `$a0`.

Closer pass (Opus, T-16553):
- **`func_80145640` matched: the call result in a declared local.** `slot = search_load_tpage_buf_lock(D_8014A4F8[i], 1); D_8014A4F8[i] = D_800E6280.unk_1228[slot] = i + 0x80000100;` with `s32 slot;` declared before `i`. With the call nested in the chain, the frame holds three spill temporaries under the one local and the counter's home is sp+0x3C. With `slot`, the counter's home moves to sp+0x38 and the frame stays 0x40. An unused second local grows the frame, but a local that replaces a nested call result does not: it takes the place of a temporary. Try this first when only the spill offset of a counter is off.
- **`func_8014607C` (still 20 lines): IDO propagates local copies of globals into uses past the `||`.** `var = D_800E638E` is propagated into `>> 1` (separate global range plus `move`), and `mask = D_800E6375 & 0xF` into both bit tests (the expression lands in a ugen temporary, not `$v1`). Proof: a then-arm that does not read `var` puts the `lh` straight into `$v0`; a volatile access between the definitions and the `if` stops the propagation but costs an address materialization. Statement order (720 orders), local types, labels, `goto`, `if`/`else` forms (uopt sinks the load into both arms), record-view stores and autotweak do not change it. Possibly a T-0018 compiler gap; the next idea is a kill between the definitions and the `if` that generates no code.

## T-16562: TACO types-first matching stage (package pt-taco-p2)

[[tickets/T-16562-taco-types-first-p2]], from the redrafts of [[tickets/T-16560-taco-types-first]] ([[taco-family-results]]). Matched in plain C: `func_801430D0`, `func_801347F4`, `func_80143B58`, `func_801342CC`, `func_80133870`, `func_801432F0`, `func_80154820` (3,120 bytes). The m2c drafts needed little: the type work was the font tables (`D_8015F5A0`/`D_8015F5B4` as `u8` UV-pair arrays, eight scalar `u8` for the special glyphs, all in `include/ovl/TACO.h`) and the SPRT layout that `SPRT *` already gives.

What worked:
- **A clone's C.** `func_801347F4` is a clone of TAIIKU `func_801406A8` and main `initModelingData_init` (siblings.py ratio 1.000). The sibling's C (`dop = arg0; dop++; ...; dop++; dop++;` and `for (i = 0; i < 0x40; i++)` over `Tc10`/`TcObj50`) matched at once; the prototype became `s32 *arg0` and the caller casts. The relocation names differ (`D_80127480+0x1400` for the loop end against `D_80128880`), the linked bytes are equal: funcdiff `--resolve` reports a diff there, ninja's sha1 is the judge.
- **A jump table that starts at 0.** `func_80143B58` is a switch on `D_8015EDBC` whose table has an entry for 0 that goes to the exit, not to the default (a different target). The m2c draft omitted it, so the table started at 0x10 (`addiu t7,t6,-16`). An explicit `case 0: break;` fixed the whole function (808 bytes).
- **Repeated loads of one word.** `func_801342CC`: the unrolled `lw/and/sw` x4 over one address is `for (i = 0; i < 16; i++) buf[0x80] &= 0x7FFF0000;` with `u32 *buf = (u32 *)0x80190200` (the same pointer is the last `LoadSquare` argument). It is called before its definition, so `s32` with no return.
- **`s32` without a return fixes a delay slot.** `func_80133870` differed only in the slot of the `beqz` before the last call: the original puts `move a1,zero` there, IDO put the fall-through's first instruction `lui v0`. With `v0` live at the exit (implicit int, no `return`) as1 may not move `lui v0`, so it takes the next free instruction. The register cheat sheet had the row; the 18-row near miss from T-16560 was this.
- **The digit and string drawers** share the SPRT setup of NAME_ENT `func_80137CA0`. `func_801430D0` matched on the first compile (K&R definition, `D_8015F5A0[n % 10 * 2]`, `n / 10` with the test `n < 10` before it).

- **Read the memory expression, do not copy it into a local.** `func_801432F0` (string drawer, 644 bytes) and `func_80154820` (record colour clamp, 320 bytes) stayed at 4 rows and 64 lines with a local `c`/`word`; writing `*s` (`*s >= 0x30 && *s < 0x3A`, `idx = *s - 0x30`, `*s == 0x2E`) and `*p` (`p += 1; r = *p >> 16; g = *p >> 8; b = *p & 0xFF; ... *p = (*p & 0xFF000000) + ...`) made uopt keep the load as a CSE range in `$v1`/`$v0` and the `*s - 0x30`, shift and mask values as ranges in `$a0`/`$v1`/`$a1`, which the original has. In `func_80154820` r/g/b are also first the raw value and then the clamped sum (`r = *p >> 16; ... r = (r & 0xFF) + arg1;`); a one-step `r = ((*p >> 16) & 0xFF) + arg1` stays 40 lines out (ugen temporaries instead of ranges), `u8` intermediates are worse. The funcdiff guarded compile reported "address or size differs" for the matching `func_80154820` (trailing padding of the last function in the file); regdiff said MATCH and the plain build is the judge. In `func_801432F0` the constant 0xC against 0x10 in `$fp` follows the order of the stores: `sprite->w = 0xC; sprite->h = 0x10;` on one line (FAKE, line grouping), and the typed font tables are one array per symbol (`D_8015F5B4`, not an offset into `D_8015F5A0`).

What blocked the rest (T-0018 rows in [[data/t0018-cases]]):
- **The entry copy** (`func_80154230`, `func_801546FC`, `func_8015443C`, 8-9 lines each): the original keeps `move s0,a0` and loads count/limit through `s0`; IDO forwards `a0` into the loads and folds the move into `addiu s0,a0,36`. regdiff shows `arg0` as one range in `$s0`, so the folding is after the allocation. New measurements: a `volatile` load of `limit` (cast, one load) keeps the move at the right place and the `limit` load on `s0`, but the first load stays on `a0` (1 row left); volatile on the first load or on all of them moves the `move` up to the 4th instruction and the loads in front of the `lui`s; a nested block, a `register` local copy, an integer parameter, ANSI against K&R, source line grouping, `-O1/-O3`, a struct pointer and `order_search` over the five entry statements (best 3 rows, with the add first, which changes the reads) did not help. The three functions share one header shape; a fix opens 1,232 bytes.
- **`func_801345D4`** (376 bytes, 12 lines, guarded): the true path matches with bit-field views of the settings word (`lw`/`lhu`/`lbu` views at `D_800E7D10 + 0x40`). In the else path the original keeps `&D_800E7D10` in `$v0` for the byte read-modify-write; regdiff lists that access as a plain global variable (`S74+64`, one use, one def) and IDO folds it into two `lui/%lo` addresses.
- **`func_80133E98`** (1,076 bytes, left INCLUDE_ASM): switch on `D_800E6280.unk_110D` that returns the new state (`var_v0`); the original loads the state into `t7`, adds into `v0`, masks into `t8` and moves it back (`addiu v0,t7,2; andi t8,v0,0xff; move v0,t8`); a chain `v = D = D + k`, `u8`/`s32`/`u32` for the variable and the return type give 126-226 lines. Scratch compiles show that a plain `D_800E6280.unk_110D += 1;` statement in an `s32` function without return already gives the original's `addiu v0,v0,1; andi t6,v0,0xff; move v0,t6; sb t6` shape; what is missing is the selector in `$v0` (IDO uses ugen `$t6` for a jump-table selector, `sll t6,t6,2`; a local `sel` that is returned at the end gives `$v0` and `sll t6,v0,2` but spills it across the calls). The next try: find the source that keeps the selector in `$v0` on the default path without a spill.

**Closer pass (Opus, 04:29-05:00 UTC):** all four guarded near misses matched in plain C (1,608 bytes; package total 11 functions, 4,728 bytes). Guards and the FAKE `s32 pad[2]` are gone.
- **The entry copy is as1, and a two-step add brings it back.** ugen's listing (`cc -S`, or the `.G` binasm from `cc -K`) has `move $16,$4; lw $17,4($16); lw $18,12($16); addu $16,$16,36`, the original's order. as1 then copies `$4` forward into the uses of `$16` until `$16` is redefined or a `.set volatile` record comes, and folds the move into the `addu` (that is why one `volatile` load stopped it half way). Two adds to the pointer bring the copy back: `arg0++; count = arg0[0]; limit = arg0[2]; ... arg0 += 8;` gives ugen `move $16,$4; addu $16,$16,4; lw 0($16); lw 8($16); addu $16,$16,32`, and as1 (by the observed output: it folds the copy into the first add, then moves that add's 4 into the load offsets and the second add) emits `move s0,a0; lw s1,4(s0); lw s2,12(s0); addiu s0,s0,36`, byte for byte the original. Same mechanism as `func_801347F4` (`dop++; call; dop++; dop++;` gives `move s2,s3; addiu s2,s2,8`). `func_80154230` and `func_801546FC` matched with that alone; `func_8015443C` also needs `arg0` itself as the walking pointer (a separate `p = arg0; p++` is folded by uopt into `p = arg0 + 4`, and `arg0` stays in `$a0`).
- **The frame of `func_8015443C` is two named constants.** `u32 command0 = 0x24000507, command1 = 0x34000609` as in the two sibling functions, compared as `command == command0 || command == command1`: uopt folds them into the compares (`lui at` each time, as in the original) but they keep their two stack words, which is the 88-byte frame. This replaced the FAKE pad.
- **`func_801345D4`: an overlapping word access keeps the address in `$v0`.** uopt turns a load or store through a constant address into a direct global access (`lui/%lo`) when nothing in the function touches that memory with another width. The flag test read `D_800E7D50` (another symbol to uopt), so the else path's `lbu/ori/sb` at `D_800E7D10 + 0x40` was the only access of its kind and was folded. Reading the test through the same base, `save->set.word & 1` with `save = (TcSave *)&D_800E7D10`, makes the byte overlap a word access: `lui v0; addiu v0,%lo(D_800E7D10); lbu t2,0x40(v0); ... sb t3,0x40(v0)` as in the original. Scratch check: `((TcSetF *)&D_800E7D50)->flag = 1` also kept `$v0` because the test read `D_800E7D50` as a word, only with the wrong base symbol. The byte-offset casts became one file-local view (`TcSave`, a per-width union at +0x40), same bytes.
- **Lead, not tried in its own file:** TACO `func_801471D0` (T-11300, 4 lines, `src/ovl/TACO/80146D60.c`) has the same `move s4,s3; lw s6,4(s4); addiu s4,s4,8` shape before its second loop. A scratch test with a new local `q = arg0 + 4; n = *q; q += 4;` changed the allocation of the whole function; the next try is a form that reuses an existing variable for the two steps.
- `func_80133E98` (INCLUDE_ASM) was out of the closer's scope and is unchanged.

## T-16551: TAIIKU 80133C80.c (types-first pilot package pt-taiiku-p1)

[[tickets/T-16551-pt-taiiku-p1]], pilot stage B, package list from [[tickets/T-16550-taiiku-types-first]]: five of eight functions matched (`func_80138224` 1,116 B, `func_80133CFC` 868 B, `func_801368B8` 312 B in the first pass, 2,296 B; `func_801346FC` and `func_80136BB4` in the closer pass, 4,480 B in all), two new guarded drafts (`func_80137544`, `func_80135480`), `func_80134060` left `INCLUDE_ASM`. `tools/siblings.py` found no matched relative above ratio 0.70 for any of the eight.

What worked:
- **Display-record initialisers from the m2c fold draft** (`func_80138224`). The T-16550 draft already had the right structure; the byte offsets were `((DisplayActor44 *)D_8011ECD0)[96 + i]` fields (`6529` = record 96 + 1, `6498` = the next record's `unk_26` before the increment). With field names and the sibling initialiser of `func_80135DF8` (`unk_38` first, `*(s16 *)rec.pad_1C`), loops 1 and 2 needed only the store order: of the five stores `0C, 10, 34, 14, 16` the order that compiles to the original is exactly that one (`0C, 34, 10, 16, 14` is the asm order, the source order differs; 12 permutations tried by script). In loop 3 `unk_01 = spA4.v[i]` has to be the second statement (same place as in loops 1 and 2); in the asm order the temporaries are one off.
- **Frame**: `func_80138224` needs two word slots above `spA4` (`s32 i; s32 j;` declared first, `j` unused, FAKE). One local declared first only rounds to the same frame; two reproduce 0xB8. Unused locals declared after the arrays take no slot, unused ones before them do.
- **A table the original defines in the same unit** (`func_80133CFC`). The copy of the stack table to `D_801492F8[0..1]` came out with two `lui at`; the original has one, shared by both stores, and loads both words first. Defining the table in the file (`s32 D_801492F8[10] = { 0x801BB000, ... };`, replacing the three `INCLUDE_RODATA` pieces `D_801492F8`, `D_801492FC`, `D_80149300`) made the stores share it and the function matched at once. The compiler sees a symbol defined in the unit as a section-relative base, an `extern` one as an independent relocation; it is the same effect as the `D_80149208` object of T-9010. Look for it first when a pair of stores to one table takes two `lui at`.
- Other `func_80133CFC` details: all six loops share one `i` (separate counters unroll the 11-iteration GameState loop and grow the frame); the 8-word copy needs its first two words written out (`D_801492F8[0] = sp44.v[0]; D_801492F8[1] = sp44.v[1];`, then `for (i = 2; i < 10; i++)`, IDO unrolls by four); `i` and `j` declared first for the two word slots above `sp9C`; `D_801492C4` is a word (`sw`), now `s32` in the header; the GameState loops are index loops over `unk_1228[i + 5]` (11) and `[i + 0x25]` (3).
- **A loop that IDO unrolls completely** (`func_801368B8`). The 17 increments are not 17 statements: `for (i = 0; i < 4; i++) { (&D_80149208.player)[i].unk_0C += 1; ... unk_1C += 1; }` plus the `unk_07` increment. IDO unrolls it with the first iteration peeled, and that peel is the scheduling boundary between `player` and `cpu[0]` that 337 straight-line variants of T-10520/T-12020 could not produce (`lui t4` before the `sw`). FAKE comment: the four runners are `player` + `cpu[3]`, not one array. Lesson: a long run of identical statements over the four runners is a loop ([[taiiku-family-results]], game A); try the loop before the variants.

What blocked the rest (rows in [[data/t0018-cases]]):
- `func_80137544` (guarded, 26 lines): decoded to a hit test over ten 0x20-wide boxes (`D_80120922` picks `sel` 0/0xA/0x14/0x80, the loop adds its index). Left: `i` and the hoisted `D_8012091E + 11` swap `$a2`/`$a3`. The `bne v0,t8` operand order needs a named `one` (FAKE), and a `(s8)` cast of `i` (FAKE) moves the ugen temporaries.
- `func_801346FC` (8 lines) and `func_80136BB4` (4 lines): unchanged by the types, the loop forms, the real locals and autotweak (339 and 297 edits).
- `func_80135480` (guarded, 22 lines, 11 instructions): the frame needs exactly three promoted word locals, two declared before `sp38` and one after (`s32 i; s32 t; TaiikuS11 sp38; s32 v;`; the layout was found by printing `sp38`'s offset for every above/below assignment, and every other split or extra local moved it); the runner loop matches as `D_801491E0[i] = 0;` first, then `unk_20.halves.high = i * 8` and the sixteen cleared fields through `(&D_80149208.player)[i]` (FAKE; any other order makes IDO test the byte pointer instead of `i * 8 < 0x20`); `v` must be an `s32` with `(s16)` on the shift. `D_801492A8` is defined in the unit (`u8 D_801492A8[4]`) for the shared `lui at` of its two byte stores. Left: the original keeps `t` in `$v1` and loads `v` straight into `$v0`; here `t` is a ugen temporary and `v` arrives through a promoted range of the global (`lh a1; move v0,a1`).
- `func_80134060` (1,348 B) was not started.

Tools: `siblings.py --package` takes `UNIT/func` entries (not `UNIT:func`) and reads the list from the repository (the Docker container sees only the worktree). A script that permutes statements must restore the file in a `finally`; `timeout` does not exist on the host, run long probes in the background with a log. About 80 funcdiff probes were scripted (`tools/funcdiff.py` is 10-15 s a run under load).

Closer pass (Opus, about 40 minutes): `func_801346FC` (1,688 B) and `func_80136BB4` (496 B) matched in plain C; the package is now 5 functions, 4,480 bytes. `func_80135480` and `func_80137544` stay guarded (not worked), `func_80134060` `INCLUDE_ASM`.
- **A wrong `s32` callee prototype blocks `$v0` in the call's block** (`func_801346FC`). `func_801365C8` sets no return value and nobody reads one; declared `void func_801365C8(void);` in `include/ovl/TAIIKU.h`, the `D_80120676` decrement in case 0 takes `$v0` as in the original. The FIRST DIVERGENCE hint of `regdiff` named an unrelated pair (switch temporary against selector); `regdiff -v` lists every range, and the one holding the differing value (bit 45, `S148+6566`) sat in `$v1` next to the `jal`. Test with `funcdiff --proto 'void f();'` first. In a guarded file `funcdiff` may then report "linked function bytes differ" with no differing word: the other guarded drafts compiled as C move the addresses, so remove the guard and check the unit sha1.
- **uopt writes the store-back of a promoted global at the end of the block, after the plain stores** (`func_80136BB4`). The `cc -S` listing (ugen's order, before as1) showed `sh $0, f0C` then `sb $2, f03` at the join, whatever the C order. The original stores f03 first because the cleared field is promoted too: a `u16 count = D_80149208.player.unk_0C;` copy for the three threshold tests loads the field without a widening, uopt promotes the field itself from the tests through the clear, and its store-back follows f03's. When a plain store comes before a store-back in the join block and the original has the reverse, promote the plain store's variable (local copy of its type for the reads before it). A FAKE empty truth test `if (field) {}` before the store does the same and is a quick probe.
- Listing recipe (scratch only): preprocess the file with `-DNON_MATCHING`, drop the `INCLUDE_ASM`/`INCLUDE_RODATA` lines, and run `/opt/ido/5.3/cc -S` with `tools/cc.py`'s `IDO_CFLAGS` (without `-c`) and `ido_frame_env`, so the uopt shim (`cvt_pass`) runs as in the build.

## T-16561: TACO types-first package p1 (pt-taco-p1)

[[tickets/T-16561-taco-types-first-p1]], matching stage of the pilot [[tickets/T-16560-taco-types-first]] on `src/ovl/TACO/801335A0.c`, `80137590.c`, `80159090.c`, `80146D60.c`. 8 of the 12 functions worked on (the 9 of the list plus `func_80146F74`, `func_80147674`, `func_80147B24` of the same files) matched (5,612 bytes: `func_80147488`, `func_80159878`, `func_8013788C`, `func_8013760C`, `func_80147C98`, `func_80146F74`, `func_80147674`, `func_80147928`), 2 stay guarded (`func_80133738` 8 lines, `func_801472D4` 20), `func_801471D0` keeps its T-11300 guard and `func_80147B24` is back to `INCLUDE_ASM` (63 lines). The new types in `include/ovl/TACO.h` carried the m2c drafts; the work was the shape of the locals.

What worked:
- **A pointer local costs a stack home, so leave it out.** `func_80147488`, `func_80146F74`, `func_80147C98` (clones of the matched `func_8015B3B0` family) match with the MATRIX declared first, one `TcPos *pos` at most, and the coordinate object written as `((TcTransformObj50 *)&D_80127480[arg0])->...` each time (uopt shares the address). With an `obj` local the spill slots sit 4 bytes low; in `func_80147C98` neither `pos` nor `obj` is a local (the frame is 0x68 only without them; the `sw v0,0x2C(sp)` is the shared address's own spill).
- **The parameter itself as the walking pointer.** `func_80147674`: `arg1 += arg2 * 6; ... arg1 += 3; ... *arg1; arg1++` gets the original's `$s0`; a separate `s16 *p` local stays in a t-register and is spilled across the calls.
- **An empty `default: break;` changes register allocation.** `func_80159878` keeps `arg0` in `$s0` (prio 1.33) without it and reloads it from its home with it; `func_8013760C` stops hoisting the `0x801AC000` constant into a third s register. The blocks add to the first-use cost of a callee-saved register (0.25 * blocks, [[uopt-register-allocation]]). Look at `regdiff -v`: a parameter or constant range that gets an s register where the original has none, with a priority close to the cost.
- **Accumulate in the promoted global, not in a local.** `func_8013788C`: `D_800E6280.unk_F80 |= 0x2000;` ... (every range test, the last one `if (t >= 0x1CD && t < 0x1E0) ... |= 0x9000`) gives the original `ori t,v1,k; move v1,t` chain and its stores; m2c's `var_v1` local plus `goto` did not.
- **`(u8 *)i + (u32)D_8015EDB4`** puts the byte offset first in the `addu` (`addu a1,s0,t0`); `D_8015EDB4 + i`, `&D_8015EDB4->unk0[i]` and `i + (s32)ptr` all put the pointer first. FAKE in `func_8013760C`. The same function needs `s32` without a return, `switch (*(u8 *)&D_800E6280.unk_110D)` and an `end` local for the loop bound.
- **`s16 *` index form with the record first for allocation order.** In `func_80159878` (1,768 bytes) the by-value `TcPos` copies, the selector `if (arg0 == k)` tests and a FAKE unused `TcPos` above the two positions (frame 0x60) were all the draft needed; the one-line `sp48.y = ..; sp50.x = ..;` pairs fix the order of the `sh` stores (the third assignment stays on its own line).
- **An empty `do { } while (0);` (or `if (0) { }`) can give a parameter its register.** `func_80147928` reloaded `arg2` into `$a0` (`lw a0; addiu a0,a0,4`) where the original has `lw a3; addiu a0,a3,4`, which means arg2 has an allocated range there. `tools/permute.py` (150 s, 2 jobs) found a no-op block after `base = arg2 + 4;` plus the declaration order `p, pos, slot, base`: the block changes the profit of the single-use parameter range. FAKE in the source. Run the permuter on every 2-4 row register miss, it found in 150 s what 40 minutes of hand variants did not.
- **One-line inner `for` body** (`for (j = 0; j != 3; j++) m.m[i][j] = *arg3++;`) puts `addiu a3` before `addiu a0` as the original, 24 to 20 lines in `func_801472D4`.

Tooling notes:
- `funcdiff` compares the whole translation unit with `-DNON_MATCHING`; when earlier functions of the file are guarded drafts, every later function reports "linked function address or size differs" or "bytes differ" even when `regdiff` says `MATCH: identical instructions` (`func_80147674`, `func_80146F74`). Confirm with `regdiff`, remove the guard, run `ninja` (SHA1 OK).
- `tools/siblings.py --package` and `funcdiff` need the list file inside the repo (the Docker mount), not in the orchestrator scratchpad.
- A guard-removal script must anchor on the `#else INCLUDE_ASM(func)` line and search backwards for its own `#ifdef`; a non-greedy regex from the first `#ifdef NON_MATCHING` of the file swallows everything between.

Left (rows in [[data/t0018-cases]]):
- `func_80133738` (8 lines): declaring `pair` before `var_v1` fixes the spill slot (0x28); the final table lookup uses `$t0/$t1` where the original has `$t1/$t2` (one more ugen temporary consumed after the `& 1` test of the else-if; address, multiply and `%`/`&` spellings do not move it).
- `func_801472D4` (20 lines): the block copy and the final reload are one ugen temporary low (`regdiff`: draft `t7 t7 t7 t7 t8`, original `t8 t8 t8 t8 t9`).
- Permuter runs (240 s, 2 jobs each) on `func_80133738`, `func_801472D4` and `func_801471D0` found no score-0 candidate. `func_80133738`: a `(u16)` or `(u8)` cast on the else-if test `(D_800E6280.unk_10F4 & 1)` fixes the lookup temporaries (8 to 6 lines) but moves the load temporary of that test (`lw t9,4340(t8)` against the original `lw t8,0(t8)`). Empty `do { } while (0);` / `if (0) { }` blocks at six places did not help there, nor in `func_801471D0`, `func_801472D4` or `func_80147B24`.
- `func_80147B24` (63 lines, `INCLUDE_ASM`): with the record access first in `sum1 += D_8015EDB4[i + 16].unk7A * ((TcPair2 *)arg1)[i].a;` uopt gives `arg1` `$a3`, the record address `$a1` and the arg1 pointer `$t0` like the original, and the unrolled body has the original's 92 instructions; what is left is the loop test: the draft compares the arg1 pointer with an end pointer (`sll; addu` before the loop) where the original keeps the counter (`addiu v0,v0,4 ... bne v0,a0`). Loop spellings (do/while, `!=`, `u32` counter, one-line body) and pointer-walking variants (not unrolled, 111+ lines) do not change it.

Closer pass (Opus, 2026-10-16, about 35 minutes):
- **Matched `func_80133738` (312 bytes) by indexing the pair table as flat words.** `p = &D_8015E238[idx * 2]; func_80046290(p[0], p[1], 8);` with `extern s32 D_8015E238[];` gives the original's `sll t1,v1,3; lui t2` (ugen allocates a temporary for `idx * 2` before the shifts fold into one `sll`); `&pairs[idx]` with the `TcPair8` type, `pair->a` and every multiply or shift spelling of the struct form are one ugen register low. The `s32 *p` local declared before `idx` keeps the spill slot at 0x28 (no FAKE needed). A test on the else-if (`(u16)` cast) only moves the missing temporary to the wrong place: look at the index expression of the table lookup first.
- **`func_801472D4` (20 lines): the struct copy's ugen temporaries depend on the last store before it.** Standalone probes (`build/scratch/ct/`): a `lw at / lw tN` struct copy allocates its destination and source address registers and then skips one ugen register before the copy temporary when the last store before it went through a pointer register (the loop stores of `m` here, or `p[3] = 0` through `$a0`), two when it went through `$sp` or a global (`lui at; sw`), after a call, or at function entry. The draft is the one-skip case; the original has two skips with no store between the loops and the copy. Copy, record and loop spellings, labels, `if (1)`, a `rec` test and no-op blocks did not change it; autotweak (120 s, 322 tries) found nothing.
- **`func_801471D0` (4 lines): the `move s4,s3` is not a copy that uopt keeps.** A separate walker local (`s32`, `u32`, `u32 *`, the parameter as walker, signed/unsigned conversions, the base variable reused for the loop-2 `&D_80127480[i + 1]` pointer) is always copy-propagated into `lw 4(base)` and its `+8` CSE'd with the loop-1 temporary. One `dop` variable incremented in place (the libgs sample-code shape `dop++; n = *dop; dop++;`) gives every instruction but the move, with the base in one register: the move looks like a uopt live-range split. autotweak (120 s, 323 tries) found nothing.

## T-16563: TACO package p3 (80151C30.c, 8015ABF0.c)

[[tickets/T-16563-taco-types-first-p3]], stage B of the types-first pilot ([[taco-family-results]]): started from the T-16560 redrafts and the new `TACO.h` types. 9 of 10 listed functions and the unlisted `func_8015BD30` matched (4,740 bytes) in plain C, three with a FAKE (a pad, a view base, an empty test). `func_8015B1B8` stays guarded (8 lines, T-0018). Nothing was solved by a draft as is: m2c's draft shape (explicit pointer variables, `goto`) was replaced by the loop the compiler turns into that shape.

What worked (in the order found):
- **No local for the actor pointer, index first** (`func_8015C0A0`, `func_8015BF70`, `func_8015CE64`, `func_801523D0`): write every use as `(arg1 + D_8015EDB4)->field`, not `a = &D_8015EDB4[arg1]; a->field`. uopt then keeps one range in `$v0` (the T-0018 rows of T-10620..T-10850 were the local and the CSE'd expression as two ranges), and ugen builds the index multiply before the base load, as the original does (`sll; lui; lw; addu; sll; addu`). `D_8015EDB4[arg1]` gives `lui` first. In `func_8015CE64` the same form changed which of `$a0`/`$a1` the global and the loop counter get (the global is mentioned later). `func_8015BD30` is the exception: there `D_8015EDB4[arg0]` (base first) matches `addu v0,t7,t6`.
- **A delay slot takes an instruction from the join block** (`func_8015BF70`): `n = 0;` written after the first `if` ended in that branch's delay slot, as in the original. Written before the `if` it was scheduled ahead of the pointer copy and the two moves swapped.
- **Table walk by incrementing a parameter** (`func_80152DB4`, `func_801523D0`): `func_80147674(arg0 + i * 3, D_801603F0[arg1], k, 0, 0); arg1++;` in `for (i = 0; i < 7; i++)`. Several variants failed first: with `D_801603F0[arg1 + i]` uopt builds a standalone `i * 4` and uses it for the exit test (`bne s1,28`), the original tests `3 * i` against 21; with an explicit pointer variable the colours were right but the stack-argument store went before the register arguments; `arg1++` inside the index (`D_801603F0[arg1++]`) moved the pointer increment. The 5-argument call with its `sw zero,16(sp)` in the `jal` delay slot is the signal that the pointer is an induction variable, not a local.
- **Pair tables read at `i - 1`, as `TcPair8`/`TcPair2` records** (`func_80151D1C`, `func_80152808`; `D_80160290[i - 1].a`, `D_801603A8[i - 1].a`, the same shape as `func_80148FE0`): uopt makes the pointer walks start at `D_80160290 + 8 * i0` with the displacement `-8`. **Or as words and bytes at `i * 2 - 2`** (`func_801521EC`, step 3): both tables then share one product `i * 2` (`sll v0,a0,1` once, `v0 * 4` and `v0 + base` for the two pointers), which two record indexes do not.
- **The pointer table of a third pointer walk needs its own base symbol** (`func_80152808`): `D_80160444[i - 0x1C]` gives init `D_80160444 + 0x74` with displacement `-0x70`; the original has init `D_80160448` and displacement 0, which is `D_801603D4[i]` (`D_801603D4 + 4 * 0x1D`). uopt keeps the symbol of the address and moves every constant into the displacement, so the symbol decides the pair. FAKE (declared `s16 *D_801603D4[]` in `TACO.h`, a view that overlays the `D_801603A8` bytes).
- **Else-if chains with the redundant lower bound** (`func_80152944`, `func_80152DB4`, `func_801523D0`): `else if (arg1 >= 8 && arg1 < 0x10)` keeps the `slti` the original repeats at each step. The first draft of `func_80152944` was nine hand-written do/while loops over a pointer; `for (i = 0; i < 4; i++) func_80147674(arg0 + i, D_80160444[i + 1], 1, 0, 0)` matched at once (index loop, the constant `+4` stays in the displacement).
- **FAKE slots** (`func_801523D0`): the original frame is 0x40 larger and `k` sits at `sp+0x4C`: `s32 pad[15]; s32 k; s32 pad2;` (scalars below an array do not move above it; this order puts `k` 4 bytes under the array).
- **Quick scratch compile**: `tools/cc.py x.c out.o ido 5.3` on a copy of one function plus the two includes takes 1 s against 25-35 s for `funcdiff --resolve`; diffing it against the `expected/` object (`objdump -dr`, relocated operands and branch targets masked) is enough to iterate on one function. Not kept as a tool (`build/` scratch).

- **`func_8015BD30` (576 bytes, not on the list, draft score 140; matched after about 35 minutes with a 1-second scratch compile)**: the m2c draft's `temp_v0_2` is a local pointer in its text but an expression range in the original. With a local the pointer splits into a copy (`move v1,v0`), with `D_8015EDB4[arg0]` the address of `unk14` is a second, commuted expression (`addu a0,s5,t6; addiu a0,a0,20` for `addiu a0,v0,20`); writing the actor as `((TcActor *)((u8 *)D_8015EDB4 + arg0 * 0x88))` in the loop shares one pointer (the neighbour style `arg0 * 0x88 + (u8 *)D_8015EDB4` leaves 2 rows). Locals: the frame was 16 bytes too large with the same saved registers; declaring one local before the spilled `val` fixed the frame and the slot (`val` declared first put it 4 bytes too high). `n = 0` after `phase = frame % arg1[2]` and before the key search puts the `or s4,zero,zero` right after the `div` as in the original; later it floats behind the `addiu`. `i` and `j * 4` swapped colours (priority 16.4 against 15); an empty `if (i) {}` in the found branch (found by `tools/autotweak.py`, 150 s, 30 rows to 8) splits `i` and fixed it; FAKE. A `u8`/`s16 *` descriptor (`first, last, period, length`, then `limit, value` pairs) is read with plain `arg1[k]` indexes.

What did not match (rows in [[data/t0018-cases]]):
- `func_8015B1B8` (8 lines): the struct-copy temporary is `t2` where the original has `t3`. New this time: the object address as `(arg0 + D_80127480)`, the copy before or after `unk_00 = 0`, a `MATRIX *` destination, and the 1-second scratch loop over about 10 more variants; none moves it. ugen's temporary rotation (`t6, t7, t8, t9, t3, t4, t5`, then `t6` again) is the same in both up to the copy, so the copy takes its temporary by a rule of its own.
- `func_8015ACCC` (1260 bytes, no compiling draft; m2c's body plus the `TcPos` stack fields at `sp+0x78..0x8E` is readable) was rewritten once by hand in about 20 minutes and left at 437 differing lines: the frame matches after dropping four locals (`base` reused with `base += 8`, the pair table pointer and two loop temporaries as expressions: every declared local costs a 4-byte slot, two more slots add 8), but the original keeps `a` in the stack home (reloads `lw v1,0x70(sp)`) and `base` in `$fp`, uses `slti` for the slot search where IDO emits `bne i,63`, and starts the free-slot counter in `$v1`; each fix moves the next register.

Closer pass (Opus, about 40 minutes): `func_8015B1B8` stays at 8 lines. What decides the struct-copy temporary:
- ugen takes temporaries from a first-in, first-out free list. Before the copy's data register (the one used with `$at`), it skips one free temporary when the last store it emitted went through a pointer, and two after a direct stack or global store or at the start of the function. Calls, loads, register assignments and outgoing-argument stores do not change this; a conditional global store does. In `cc -S` output the one-skip case has two `.set noat` lines before the copy and the two-skip case has one.
- The loop's stores to `matrix` go through pointers, so IDO skips `t9` and uses `t2`. The original's `t3` means two skips, or `t2` reserved by uopt (uopt registers are left out of ugen's list for the whole function). A stack store before the copy, or a uopt temporary in the inner loop, gives `t3`, but each also changes other code. About 60 variants, autotweak (120 s) and the permuter (420 s) found nothing better.
- TACO `func_801472D4` has the same shape and the same miss (`t8` against `t7`). Main `func_8005938C` (T-11610) shows the opposite case. This may be a difference in the original compiler, not in the C. T-0018 row.

## T-16564: TACO package p4 (80154960.c, 80137A60.c, 80138890.c)

[[tickets/T-16564-taco-types-first-p4]], stage B of the types-first pilot ([[taco-family-results]]): started from the T-16560 redrafts. 6 of 8 functions matched (5,852 bytes) in about 55 minutes of the 2.5 hour budget: `func_801563E4` (1,344 B), `func_80154960` (1,556), `func_80138D08` (1,040), `func_801557E8` (372), `func_80138964` (456), `func_801553AC` (1,084). Two stay guarded register near misses: `func_80138B2C` (30 differing instruction rows) and `func_80137D00` (8 rows, the T-9180 pointer row). Rows in [[data/t0018-cases]].

What worked:
- **The draft's m2c temporaries are the first thing to remove** (`func_801563E4`): four `temp_hi` locals each reserved a frame slot (frame 112 against 96). Writing `(func_800AE0D0() % 30) + 0x28` inline gave the frame; the spill under the TcPos locals (`sw v0,64(sp)` in the original) then needed one FAKE `u8 pad[4]` after the TcPos locals. The operand order of `a%2000 + (b%4000 + unk74) - 3000` fixes which call is hoisted first (IDO hoists calls left to right, then evaluates the tree).
- **Where an independent store sits in the source decides which side of the next `jal` it lands** (`func_801563E4`): `sp58.x = (rand() % 30) + 0x28;` before the three `sp48` stores put them in the div shadow after the call (the draft stored them first and the scheduler hoisted them before the call); the same for `sp58.x = ...; sp50.x = 0; sp58.y = ...` in the three later calls.
- **A packet drawn through a pointer is a `POLY_F4 *`** (`func_80154960`, as in 80153AA0.c): the byte-offset stores of m2c become `poly->r0 = ...; poly->x0 = ...`. The callees defined later in the file (`func_80156178` whose result the caller reads, `func_801563E4`, `func_80154F74`, `func_8015595C`, `func_801553AC`) need prototypes in `TACO.h` or the guarded compile stops at "Incompatible function return type"; `func_80156178` became `s32` with no `return` (v0 passes through).
- **Cases that are m2c fall-through chains are `switch` tables of 10 entries** (`func_80138D08`): the original has `sltiu 10` and `sltiu 11` jump tables whose last entries (8, 9 and 10) point at the `default` label. Write `case 8: case 9: default:` first and `case 0: case 1: break;` last; with cases 6/7, 4/5, 2/3 falling through, the tables and the code match. Only the third `switch` (it also stores to the selector) needed the FAKE `*(u8 *)&D_800E6280.unk_110D` view; with the plain field it is promoted into `$v1`.
- **idx + base against base + idx** (`func_801557E8`, a row of T-9180 that was recorded as a frame gap): with no locals at all except the shared value `t` (spilled across the two calls), `(arg0 + D_8015EDB4)->f` gives `addu idx,base` and `(D_8015EDB4 + arg0)->f` gives `addu base,idx`; the original mixes the two per statement (`unk78`, the `unk74` statement, `unk64`, the two `unk68` stores use base first). The `off` and `a` locals of m2c are what made the frame 0x38.
- **A promoted global pair needs one web** (`func_80138964`): writing the counter update directly on `D_8015F4A8` (no `cur` local) and `D_8015F4A4` made `cur` `$a0` as in the original; the target still got `$v1` and the reset path a `lw v0,40(sp)`. The whole difference was that the original's reset path (`D_8015F4A4 = arg0; D_8015F4A8 = 0;`) ends without a value: an `if/else` whose reset arm falls off the end of the `s32` function makes `v0` the promoted target register (`move v0,a0; sw v0,D_8015F4A4`), the same trick as the `return`-less s32 functions of the cheat sheet. autotweak's `emptytest` found the way (84 to 3 lines) before the real cause was clear: a bare `return;` in the reset arm.
- **Constants numbered by use, two stores per line** (`func_801553AC`): `sp50.y = (x > 0) ? 1 : -1;` numbers `-1` first as the original does (the if/else numbers `1` first); in the four repeated calls `sp40.unk6 = 0x14; sp50.z = -0x384;` on one line (unk6 first so its constant is numbered first, as1 then stores z first). One FAKE unused `TcPos sp48` between the two reproduces the frame (0x58).
- **Array for the five score words** (`D_8015F490[5]`, `type_recovery` proposal): D_8015F490..D_8015F4A0 are the total and the four bonus lines; `func_80138D08` still matches with `D_8015F490[k]`, `func_80138B2C` is unchanged by it. `D_8015F4A4`/`D_8015F4A8` (target and shown score of `func_80138964`) stay scalars.

What blocked the rest:
- `func_80138B2C` (30 rows): the original colours the u8 count `D_8015EDCC` `$a0`, the zero-initialised local `$a1` and the final sum `$a0`; IDO gives `$v0`, `$a0`, `$a1` (the same three webs, shifted). The structure matches since the final arm is a plain `else if ... else { var_v0 = 0; }` (the draft's `var_v0 = 0; if (c) ...` lacked the original's jump to the join). Tried without effect: chain assignment of the first store (fixed `$t8`), globals instead of locals (each one, and all three), a shared `total` variable for the count and the sum, K&R parameters used as the locals or left unused, `*(u8 *)&` and pointer views of the count (promotion off), empty tests to extend the count's range, `void`/`return` forms, order_search over the first and last store groups (60 to 56 rows), autotweak 300 s (60 to 54 rows with an `s8` view of D_8015F4A0, not kept), a 300 s permuter run. regdiff shows no original range that holds `$v0` while the count lives, so the cause is a colour preference, not a conflict.
- `func_80137D00` (8 rows): known from T-9180 and T-10020. Same single row as before: `lw v1,0(t9)` with the hi part in a separate temporary (a uopt register for the pointer) against IDO's `lw t9,0(t9)`. The regdiff trace says the pointer web is allocated `$v1` (priority 5, one block), yet the object uses `$t9`. New negative results: a four-iteration loop for the `0xFF00` stores (IDO unrolls it as 3+1 with an index register), the pointer as an integer, `register`, assigning it at the function top or before the preceding call (spill), the store after the call (spill), an `s32` instead of `void` function.

Closer pass (Opus, about 25 minutes of exploring, 06:05-06:30 UTC): `func_80137D00` still guarded at 8 rows, nothing new matched. What the pointer row is, from uopt's output ucode (`cc -K`, the `.O` file; on macOS give `-o` another name than `t.o`, the bind mount is case-insensitive and `t.O` gets overwritten) and a minimal test file:
- uopt colours `p` `$v1` as regdiff says, and emits `str R3 (lod D_801FAA84)`. It also copy-propagates `lod D_801FAA84` into the first store (`p[5] = 0x28`), the only use before an `istr` kills the load. ugen then sees one load with two consumers and keeps it in a temporary (`lw t9`); every later `lod R3` reads that temporary and `$v1` is never written. No uopt option (`-zcopy:0`, `-norlodrstropt`, `-noprecolor`, `-nogenvreg`, `-docopy`) and no ugen option (`-nocpalias`, `-nooffsetopt`) changes it. `-Wo,-varref` stops the copy propagation but gives `lui v1; lw v1,lo(v1)`, which is also wrong (the original has the address in `$t9`).
- ugen loads the shared value straight into the register variable (`lui t9; lw v1,lo(t9)`, the original's form) in two cases only: the variable is read after a branch target (TT `func_801367C0`), or it has at least three in-place `p += k` updates in the same stretch (TT `func_80149B20`; two updates are not enough). A label that no branch targets, as from `do { } while (0)`, does not count.
- The in-place form (`p[5] = 0x28; p += 11; *p = 0xFF00; p += 7; ...`) puts the pointer in `$v1` with the original's stores, but ugen writes back the last update before the switch join (`addiu v1,v1,64`, or `addiu v1,v1,0` when the walk returns to the base), one instruction too many. The last row (`addiu t9,t8,-1` against `t0`) also stays, because the address temporary `$t9` is freed only after `$t0` in both forms; the original frees it first.
- Also tried without effect: the store order `p[11]` before `p[5]`, a 14-byte record struct with the base at +8, `(p = D_801FAA84)[5] = ...`, a chained `=` for the four `0xFF00` stores, a `const` global without a local (reloaded after each store), labels before or after the stores, autotweak 120 s (16 to 16).
