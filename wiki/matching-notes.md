---
type: concept
updated: 2026-10-18
sources: ["tools/cc.py", "tools/funcdiff.py", "include/game.h", "configure.py"]
sources: ["tools/viewsweep.py", "tools/cc.py", "tools/funcdiff.py", "include/game.h", "configure.py"]
---

# Matching notes

[[tickets/T-20810-viewsweep]], tool `tools/viewsweep.py` (the type pass as a tool, [[decompile-workflow]] "Step 1 for a near miss"), worktree `tr-viewsweep`. Not a wave 3 package: a tool and its first use, results in `drafts/T-20810/`.

- **Why.** One lever closed 19 of the 102 wave 3 blocker notes and 5 of the 7 T-20600 matches (3,772 B): the file's type of the function itself (`s32` definition without a `return` value, implicit int, `$v0` live at the exit) or of a callee (`s32 f();`, `void f();`, `*_OVERRIDE_` guards). Each use was found by hand with `--proto`, one callee per run, never the function's own type, never a callee that the same file defines, and a probe that died on a concurrent `build.ninja` rewrite counted as "no gain". `viewsweep` sweeps own type x every callee view on the full unit in memory (singles, best own x callees, pairs of the five best, extension), retries a failed compile once serially and reports it `UNTESTED`, and prints the recipe at 0 rows.
- **It finds the hand-found cases again.** With the lever taken out of a scratch copy of the file (`--src`, nothing in `src/` touched): RPG_BAT `func_80149C5C` (1,296 B; own `s32` plus `func_8013F0F4` as `s32 f();`; the unlevered file does not even compile, a call stands ahead of a `void` definition) in 6 compiles, 4 s; TACO `func_8014B8B4` (972 B; own `s32`) 4 compiles; TAIIKU `func_80145370` (720 B; `func_80085E30` as `s32 f();`, 22 rows before) 5 compiles; RPG_BAT `func_8013B5B0` (864 B; `func_800452C4` as `s32 f();`, 36 rows before) 41 compiles, the whole lattice, because the original never reads that result and the asm has no hint; RPG_BAT `func_8013E54C` (624 B; own `s32`, 32 rows before) 2 compiles. All five end in the literal-byte proof.
- **First use on the tree** (committed tool, `-j 2`, 16 minutes of machine time in all). The 101 guarded drafts: 996 compiles, 14 minutes, 1 match, 7 improved, 93 no gain, 0 UNTESTED, no target hit the 150-compile budget (median 6 compiles, largest 69). The 29 archived draft bodies that could be swept (24 wave 3 restores plus `func_80134FE4`, `func_8005938C`, `func_80142A8C`, `func_8006C988`, `func_801357AC`; `func_80140040` and `func_80139E3C` are guarded in the tree and were covered above): 157 compiles, 2 minutes, 16 no gain, 3 improved, 10 failed (the archived body does not compile in the current tree: undefined globals such as `D_8015EC64`, a missing `TcTex4` type, syntax; the package that wrote it added context that the restore removed). Expected 2 to 5 matches; got 1. The hand sweeps had covered most of what the lattice offers, which is the useful negative result: 93 of 101 guarded drafts are not a type-view problem.
- **The match: NAME_ENT `func_80134B94` (804 B, 10 rows before).** The file's local `void func_800AE090();` (an override of the three-parameter `main_api.h` prototype) becomes `s32 func_800AE090();`, the guard around the already-`s32` draft goes. Why it works: the callee's unused `$v0` result keeps the third name's first-byte address in `$v1`, as the original has it. File-wide, so every other caller in `80132000.c` compiles with the `s32` view too; the overlay sha1 stays OK. Verified with the change applied: `ninja` (NAME_ENT.bin OK, everything up to date), `funcdiff.py --resolve` MATCH (literal bytes), `check_headers`, `migrate_globals --check`, `sync_protos --check-branch`. Kept as `drafts/T-20810/patches/NAME_ENT_func_80134B94.patch` and not applied: the ticket's rules forbid editing `src/`, and `80132000.c` may be busy. The literal-byte proof on the sweep's own object (every guarded draft of the file compiled as C) did not pass; the fallback in `prove`, a unit where only this function is C, did, and that is the unit the real change produces.
- **Leads (improved, not matched; edits are the best variant).** DATE `func_8013B488` 75 to 49 (`s32 func_80083440();`), NAME_ENT `func_8013BB24` 28 to 17, EVENT `func_80118868` 16 to 8, SHUGAKU `func_801352B0` 37 to 27 (`s32 func_801343B0();`), main `menu_check_1` 27 to 20, NAME_ENT `func_80140B20` 15 to 12, main `func_8007B144` 21 to 20 (the edit is the function's own `s32` view unless one is shown); restores: RPG_BAT `func_80154760` 292 to 213 (`s32 func_8013E97C();`), main `func_8004B358` (the draft's header disagrees with its definition, 49 rows after repair), OMIMAI `func_8013388C` (594 rows after `s32 func_801340E4();`). Start from the variant in `drafts/T-20810/viewsweep-results.jsonl`, then the register or order rows.
- **What the sweep cannot do.** It changes return types and parameter lists only: an argument, a cast, a local, a statement order are other levers ([[register-cheatsheet]] rows, `order_search.py`, `autotweak.py`). A callee the asm calls but the body does not name is not probed. An archived body that needs declarations it does not carry fails at the baseline; `declarations.h` next to a `body.c` is read, nothing else.
- **For the next matcher:** run `viewsweep` first; its table says whether the type lever is spent. The register cheat sheet's first row says so too.

## T-25040: MAIN flag dispatcher scalar-view closure

`func_8006D138` now matches all872 literal bytes in guarded/default whole-TU private links; complete MAIN SHA1 and header/global/prototype checks pass. The empty unsigned entry switch preserves the normal copy boundary and initial flag snapshot. A late-zero identity in the empty year98 check removes one unprofitable constant range: measured year/base interference falls below the leaf threshold22, restoring the target register order. This corrects the earlier assumption that the caller-save count12 was the constraint threshold.

Overlapping flag word/byte views cause the final absolute-load residue. Explicit initial snapshot plus a late-zero first b7 address leaves only the first b8 byte load absolute. A late-zero b8 address alone moves static promotion to another live read. Keeping an exact-width discarded byte view, `if ((u32)gs->unk_F64[1] < 0) {}`, before the real late-zero b8 read holds the static alias without emitting its load. All comparisons are false and offsets zero; FAKE comments cover these controls and existing empty tests. No compiler/build/header change. [[tickets/T-25040-main-flag-dispatch]] and `drafts/T-25040/README.md` preserve phase evidence, the earlier checkpoint and controls; root's final standards/spec review has no findings, full27 integration pending.

## T-25045: ETC grade tail matched by a preceding call identity

[[tickets/T-25045-etc-ring-slots]] closes func_80146D4C, 264 instructions / 1056 literal bytes (corrects the old 1076-byte estimate). A byte-identical logging-only ugen replay proves only one extra allocation is needed before the last index shift; the later missing t5 is already absent from the free list. Index masks/casts break address CSE, and self-assignment supplies the allocation but adds stores. Transfer the new T-25047 alternating identity to the preceding total printf argument instead: reuse the pad as zero, `zero = g->unk_041 * 0; printf(format, (score + zero) | zero);`. Its one intermediate advances the ring; as1 deletes the identities and the zero dies before the call. Either identity alone, literal zero, or wrapping the total sum gives baseline cost12. Both unsigned-negative and multiply-zero producers match; multiply retained and marked FAKE. Default linked1056-byte proof, ETC checksum and declaration guards pass; root source review has no findings. See drafts/T-25045/README.md for controls. In Review for root full27 integration.

### T-25050 MAIN ring input literal match (2026-10-22)

`func_80075C84` in `src/main/80075320.c` matches all 796 default-C linked bytes. The inherited two-symbol ring omitted position rereads. An explicitly FAKE unsigned-impossible tail store preserves uopt's alias facts, then disappears with its branch; this restores both switch-arm and outer-loop rereads while keeping canonical array indexing. A marked raw input-word empty use fixes word/mask register allocation. Real increments `k++, j++` restore scheduling. A real local owning `rand()`'s result fixes the sole spill offset without padding; captured ucode showed the target needed one additional local extent word.

MAIN hash and declaration guards pass; parent standards/spec review has no findings. [[tickets/T-25050-main-ring-input]] remains In Review for root full27. Source-only baseline/replay and negative evidence are in `drafts/T-25050/`. Final controls: current0, raw-use removal180, tail-store removal148, inline random4, reversed increments4 diagnostic lines. Integer key-store addresses, useful during diagnosis, proved redundant and were removed. No shared header or compiler change.

## T-25054: ETC results scene matched with separate bases and array noalias

[[tickets/T-25054-etc-results-scene]] closes func_8014790C (282 instructions,1128 bytes). The archived T-24909 body measures cost66 in the current full TU. A late-zero OR around i+97 retains the target multiplication, but uopt shares the walked and indexed bases. A one-iteration initializer with an always-false unsigned comparison prevents that sharing; use an unrelated byte so the switch live range stays unchanged. Same-line counter initialization/do fixes the address schedule. The last call needs both coordinate arrays indexed to provide noalias metadata against the stack; indexing the value array too and placing the record update before the counter restores all induction setup/update instructions. Canonical DisplayActor44 fields preserve the match. Marked FAKEs include the inherited frame buffer and non-void fallthrough. Default literal proof, ETC checksum and declaration guards pass; root integration is pending. Details and controls are in drafts/T-25054/README.md.

## T-25055 TEL selector ownership and range profit

TEL func_80136724 reaches6,580-byte default-C literal linked MATCH. [[tickets/T-25055-tel-pass-mechanisms]] and drafts/T-25055 preserve the full-TU causal controls. The pointer donor's apparent missing temporary at word85 was a missing reserved caller register: replace its case0x40 ent local with direct D_801405E4 stores and reads, restoring the selector range and taking aligned cost970->127. Sharing case6 maximum and final total restores the dependent zero and length, producing1,645 words with41 changed ordinal positions. An always-false unsigned read of the girl field before the rank lookup changes range profit/allocation and address sharing (41->1). hp*1 in the second case0x40 compare restores operand order (1->0). Both erased constructs are FAKE-annotated, as are the recovered padding slots and inferred text extent. No compiler/header/flag changes. See the ticket for complete gate and ABI evidence.

## T-25059: ETC ranking generator matched through counter and scalar lifetimes

[[tickets/T-25059-etc-ranking-generator]] closes func_801480D0, 702 instructions / 2,808 bytes. The corrected T-24049 full-TU baseline reproduces 709 instructions and aligned cost 859. Target register continuity identifies the generation counter as the inner counter in both tie loops and the selection loop. Correcting those identities and separating first-noise storage restores the 208-byte frame and four saved registers. Product operand order, case-9-before-case-1 layout and observed scalar stack homes recover the generation phase. Existing `a`, `offset` and `j` then supply the second random product, curve/top-score shift and final player correction carriers. An explicit lower-bound delta prevents distribution; direct player-rank accesses preserve the reload after the other thirteen ranks change.

No late-zero runtime identity remains. Scalar order and one unused word are marked FAKE; removing the word regresses the match to aligned cost 42. Current default C matches every linked byte, ETC checksum and declaration guards pass (zero new / 15 known prototype disagreements), and root source review has no findings. Full 27-binary integration remains with root. The file has no remaining guards. Details, negative controls and private evidence paths are in drafts/T-25059/README.md.

## T-25057: NAME_ENT record selector remainder and expression order matched

`func_801398EC` matches 123 words / 492 literal default-build bytes. The ordinary indexed thirteen-record loop recovers the peeled first record and six paired trips. Initialize the index before the selector, retain an erased exact-width unsigned record test, and use a one-element u32 array for the remainder. Its dead initial frame assignment orders the memory range before frame/index; the computed remainder overwrites it before every semantic read. Uopt removes array storage and the initial load entirely, recovering a0 record / a1 remainder / a2 frame / a3 index.

The last 17 differing words came from evaluation order: captured optimized ucode evaluated tile-multiply before row-divide/multiply. `row - (0U - tile * 16) + 128` retains row-first evaluation and is equal modulo 2^32; index 0..12 bounds the signed row product. Four constraints are FAKE-marked and have removal controls; the redundant unsigned tail test was removed. Full build, fresh 27 hashes and declaration checks pass; root's direct standards/spec review has no findings. [[tickets/T-25057-name-ent-pass-investigation]] is In Review for integration. drafts/T-25057/README.md preserves traces, rejected controls and exact metrics.

## T-25102: TAIIKU func_80139AB0

Matched (812 bytes, default C; all 27 sha1 OK). The 20x4 grid loop is no longer a blocker. Write `i << 2` and `i << 3` instead of `i * 4` and `i * 8` (uopt does not strength-reduce shifts, so the outer loop recomputes them from `i` as the original does), and put the invariant first, `-160 - (i << 3) + ((i << 2) + 80) * j`: that body is long enough to be unrolled by 2 with `multu w, j`, and the invariant is hoisted into one register per array. `(j - 2) * w` is unrolled completely. One `i` serves all four loops (a fresh counter unrolls the small loops). A third declared scalar puts the spill of `i` at sp+0x50 (FAKE `pad`). The last 5 rows were a `lui $at` that as1 did not share: as1 reuses one `lui $at` across offsets only for a symbol defined in the compiled C and only inside one 16-byte-aligned chunk of the section, so `D_801499B0` (an `INCLUDE_RODATA` piece, an extern to the C) got two. Defining it in C (`TaiikuPair D_801499B0[1]`) matched. Details and negatives: `drafts/T-25102/TAIIKU/func_80139AB0/NOTES.md`.

## T-25102: TAIIKU func_8013E894

Matched (972 bytes, default C; all 27 sha1 OK), from the T-24521 body at 224 rows. Three source facts closed it. (1) The row offset is computed from the counter: `xp = x + (i << 1)`. `i * 2` is strength-reduced by uopt, and the old `row` copy made ugen emit an extra `mul`-turned-`move` (`cc -S` shows `mul $4,$2,2`, as1 folds the factor into the next shift). (2) The middle and draw loops read `xp[0]`/`xp[1]` directly (uopt forwards the stored values); only `yp[0]` of the middle loop is a local, and the draw loop passes `(s16)yp[0]`. (3) The rotations call `func_80086204`, the implicit-int name of Rot_2D already used by `func_8013C4E4`: the int result keeps the 4-loop x pointer out of `$v0` (44 rows to 0), so the camera x and the ball x get it. Two operand orders and a real `ball_x` local for the angle home finish it. Details and negatives: `drafts/T-25102/TAIIKU/func_8013E894/NOTES.md`.

## T-25102: TAIIKU func_8013DDB8

Matched (1092 bytes, default C; all 27 sha1 OK), from scratch (the older drafts were raw m2c). The case 2/5 grid loop is the `func_80139AB0` loop; the case 0/3/6 loop is the same 4-trip inner loop with `-160 + j * w`, which uopt unrolls completely (the `li s1,3; multu` is the unrolled `j = 3`). `(h << 4) * 3`, not `h * 16 * 3` (folded to `* 48`). A pointer local for the half reads puts `&D_801499C0` in `s2` after each call (FAKE: the word stays direct). The y store is `h - 0x100U` (FAKE): a signed subtraction lets uopt test `h >= 0x100` as `bltz` on it. The last 16 rows were order only, and the cause was the symbols: as1 keeps a load below a store only when both use one symbol. 0x80122EC4 is offset 0xF4 of the 0x100-byte block at `D_80122DD0` (new `TaiikuWork` view in TAIIKU.h), and 0x801207EB..0x80120812 are fields of actor record 102 of `D_8011ECD0`; with those views the step load stays above the camera stores and the `lbu` below the `sh`. Then `[102].unk_1A = x; [102].unk_1C = [102].unk_1A;` promotes `unk_1A` and sinks its store as the original does. Details and negatives: `drafts/T-25102/TAIIKU/func_8013DDB8/NOTES.md`.

## T-25102: TAIIKU func_8013F3A4

Matched (1444 bytes, default C, no FAKE; all 27 sha1 OK), from scratch (the older drafts were raw m2c). The long-standing blocker, IDO holding `&D_8011ECD0` (and 0x44) in a register across the whole switch, came from the source form, not the compiler. (1) No record pointer local: index the pool in every statement, `((DisplayActor44 *)D_8011ECD0)[arg0 + 104].unk_26`. A per-case `p = &pool[arg0]` makes uopt hoist the base and the record size to the switch head and into both flag arms (`li a2,68; multu`, 479-524 rows). The range is connected through the flag arms that precede the calls. (2) `if (arg0 == 0)` for the player test. uopt rewrites it into `bne v0,<base>` with a fresh `lda` that the allocator does not count. A written pointer compare counts as a use and brings the hoist back (482 rows). Matched TAIIKU `func_80147A6C` shows the same rewrite. (3) `phase` declared before `state` gives the original's `0x28(sp)` save slot for `state` (4 rows). (4) The x store before the y stores in each arm sets ugen's numbering (258 rows). Notes and negatives: `drafts/T-25102/TAIIKU/func_8013F3A4/NOTES.md`.

## T-25102: TAIIKU func_8013E1FC

Matched (1688 bytes, default C, no FAKE; all 27 sha1 OK), from the `func_8013E894` body of this package (T-24521's
best was 548 rows). The function is two 64-trip loops: straight strips, then the E894 rotated strips without the
camera angle. Seven source facts closed it. (1) Rotated phase: `Rot_2D(angle * 16)` and `Rot_2D((angle + 1) * 16)`;
uopt strength-reduces both into the two spilled +16 values and tests `!= 0x410`. (2) Straight phase: `angle++` at
the loop end; uopt then spills `angle + 1` before the first call, carries the test load into the next head, and both
phases' counters take compiler temporary slots (0x78, 0x60) below the 11 declared scalars. (3) `x[1] = x[0] = -320;`
chains. (4) `y[2] = y[0]; y[3] = y[1];` before the midpoint sums (ugen temporary order). (5) Plain `x[i << 1]`
indexing instead of `xp`/`yp` row pointers: pointer locals read in the straight draw loop cost the `.noalias` mark
between the x and y walks of the rotated translate loop. (6) Both outer loops as `for (angle = 0; ...)`, so the
counter init shares the line of the hoisted invariants. (7) The implicit-int `func_80086204` call, as in E894.
Measurements and negatives: `drafts/T-25102/TAIIKU/func_8013E1FC/NOTES.md`.

## T-25102: TAIIKU func_8013EC60

Matched (1788 bytes, default C, no FAKE; all 27 sha1 OK), from the `func_8013E1FC` and `func_8013E894` bodies of this
package (the older drafts were 489 to 784 rows). Straight phase: corners `{320, 320, 240, 240}` and
`{a << 3, (a + 1) << 3}` twice, each turned by the camera angle through `func_80086204` with `input_x = x[i]`, then
the E1FC tail over all four corners; between the phases `D_8011ECD0` record 102 `unk_26 = x[3] + 8`; rotated phase:
the E894 body with inputs `(0, -320)`, `(0, -240)`. Two source facts closed it. (1) Rotated phase as
`for (angle = 0; angle < 64; angle++)` with `angle * 16` and `angle * 16 + 16` (301 to 40 rows): the do-while
`angle += 16` keeps `angle` in `$s6` for the whole function, the strength-reduced form spills it at 0x78 like the
target. (2) `y[2] = y[0]; y[3] = y[1];` as copies (40 to 0 rows): the chained `y[2] = y[0] = angle << 3` stores
reserve one more spill slot (counter at 0x74) and leave the counter colour one register off. Notes and negatives:
`drafts/T-25102/TAIIKU/func_8013EC60/NOTES.md`.

## T-25106: TAIIKU func_80137C44

[[tickets/T-25106-w8-TAIIKU-80133C80]] matches TAIIKU `func_80137C44` (153 words, 612 bytes, default-build C, `TAIIKU.bin: OK`) from the T-24008 body (189 rows). The decisive changes were ownership and order: the four mode/selection/frame/slot globals read directly in the loop (uopt promotes them and loads them in the preheader; snapshot locals took the lowest bits, an `actor` pointer local made every store a kill), the table term first in each sum (`D_801492B8[k] + (x >> 3) * 6`, which uopt turns into the original's product-first `addu`), the selected index straight from record 46 (`(&DISP(46))[D_801491D8]`, no CVT, so it shares the runner field's 0x44 constant), and a position snapshot before the second loop. Two FAKEs: a byte-offset record address in the second loop (with `DISP(i + k)` in both loops the loops share one address expression and the second loop ends on the runner pointer), and `*(s32 *)&D_80122E94 == 1` for the constant-first `bne`. Steps, rows and negatives: `drafts/T-25106/TAIIKU/func_80137C44/NOTES.md`.

## T-25106: TAIIKU func_80137784

No match; guarded at 4 rows (2 instructions), was INCLUDE_ASM with a 391-row best draft (T-24521). Levers, in order: a signed runner record index (`DISP(46 + (s32)RUN(arg0).unk_02)`) so it shares `li t5,68` with the two arg0 products while the D_801491D8 record stays unsigned (`(&DISP(46))[D_801491D8]`, strength-reduced); the runner and its `unk_02` as CSE expressions, not locals (spill slot 0x30, reload after `k_reset`); one declared local only (`i`), which fixes the frame at 56, with sx, ex, value and the slot pointers as expressions; `-(q) * 16` for `negu t8`; the block-3 sum operand order and the `unk_28` copy before the byte stores. The structural blocker: with `p == D_8014984C` / `p == D_8014981C` uopt keeps the two slot base addresses in registers (priority 0.5, one free compare unit each) ahead of the 0x44 stride (0.4), which shifts every colour after it. Taking the compare constant from another symbol (FAKE `(Fix32 *)&D_8014985C - 4`) removes it; left is the compare operand order (`bne a0,t8` wanted, constant first built). Details, negatives and the next hypothesis: `drafts/T-25106/TAIIKU/func_80137784/NOTES.md`.

## T-25106: TAIIKU func_80135F3C

[[tickets/T-25106-w8-TAIIKU-80133C80]] matches TAIIKU `func_80135F3C` (419 words, 1,676 bytes, default-build C). A fresh body replaced the T-24521 draft (543 rows), which was missing a store; the fresh body started at 510 diff lines. Four changes closed it. The inner duplicate loop counts `j = 1; j <= i + 1` (the original's `blez (i + 1)` guard and `slt` exit). Records are indexed from 46, so every product is `multu` with one 0x44 constant. The `>> 3` loop and the flag lookups are two loops, so the first stays rolled. The repeat tests read the records directly with no value local: uopt PRE then hoists the first read past the guard (12 lines left). One unused `pad` scalar (FAKE) restores the 12 bytes above the copied tables. Steps, causes and negatives: `drafts/T-25106/TAIIKU/func_80135F3C/NOTES.md`.

## T-25105: main func_80054E6C

`func_80054E6C` matches 340 literal default-C bytes (86 words). The unk_CC8 copy is a 16-trip byte loop that IDO unrolls by four, loading copy 0 last. The old guard wrote it as four trips of four hand-written stores and stored the club value through the reused counter, which kept the loop rolled. With the loop written plainly and the club stored directly, every other old FAKE edit could go. The `slot << 4` nickname split became a header type: `unk_CC8` is `u8 [16][16]` plus `unk_DC8[0x100]`, the same split as SaveImageView's unk_CD8/unk_DD8. The flat `slot * 16 + i` index is CSE'd with `nickname[slot]` across the memcpy calls; the row index is not. The NAME_ENT reads now use `[selected_slot][i]` and still build to the same bytes. The slot byte view became the work-pointer idiom of the NAME_ENT code: with `Work800E7D10 *w = &D_800E7D10; GameState *g = &D_800E6280;` and every access through them, the slot load is an indirect load. That keeps the raw byte in s0 and the slot and slot * 8 bases saved across the calls. A plain global load gives 97 rows. One FAKE is left: `s32 pad[14]`, 56 bytes of the 0x80 frame that the code never touches (the two pointer locals use the other 8). Measurements and negatives: `drafts/T-25105/main/func_80054E6C/NOTES.md`.

## T-25105: main func_80054FC0

`func_80054FC0` (the save-image serializer) matches 796 literal default-C bytes. Start: the T-24501 draft at 113 lines. Two causes closed it. (1) The two small record copies took their transfer register one ugen slot late. `tools/ugen_pass.py` puts its B/F sentinel before every `Umov`, small aligned copies included, and the sentinel kind sets the skip: two entries after B, one after F. The original is F; the pass model gives B because the last state op is the direct `selected_slot` store. A scratch run of the pass forced to F left only the epilogue. In C, an empty `if ((s32)selected_slot > 0) {}` after the indexed `slot_status[sel]` store gives the pass its signed positive-bound jump: F, no code (FAKE). Pointer, volatile and late-zero-index spellings of the store either stay `str` or put the base in a register. (2) The epilogue: as1 moves `lw ra` above the final store only when the store address names a global. `((SaveBank *)&D_80123120[arg0 * 0xF00 + 0x200])->checksum` gives `lw ra; sw; jr; addiu sp`; a store through a pointer local stays before `lw ra` with a `nop` in the delay slot. `func_8005557C` has the same epilogue. Notes and negatives: `drafts/T-25105/main/func_80054FC0/NOTES.md`.

## T-25105: main func_8005557C

`func_8005557C` (the game-save writer) matches 1212 literal default-C bytes (303 words). Before this pass it had only m2c drafts (523 rows). The new `SaveGameView` in `include/game.h` gives the bank layout, with `GsBlockC4` (word-aligned 0x7C copy) and `GsBlock56C` (byte-aligned 0x100 copy) for the two block copies. The first typed body was at 425 rows; four causes closed it. (1) Unrolling: IDO unrolled the 256-trip record loop. With one counter shared by all loops, only the last loop in the function is unrolled (truncation tests), and in the original that is only the byte sum. So the byte sum shares the copy loops' counter. (2) That counter is `u32`. With `s32`, the byte-sum bound 0xF00 becomes the same constant as `arg0 * 0xF00`, so uopt holds it in a register and the base turns into `li; multu`. (3) The original computes the bank base twice: a2 for the straight-line stores and a3 again before the first loop. The stores are written through `&D_80123120[arg0 * 0xF00 + 0x200]` and the loops through `((SaveGameView *)&D_80123120[0x200])[arg0]` (FAKE). (4) `GameState *g` is the fourth declared scalar and puts the spills at 0x2C/0x28. Six GameState bytes went from `s8` to `u8` in `main_api.h` (`unk_03C`, `unk_F6C`, `unk_F6D`, `unk_F6E`, `unk_F70`, `unk_F73`): every direct load of them in the game is `lbu`, and 27/27 hashes still pass. Notes, negatives and a follow-up on now-redundant `u8` views: `drafts/T-25105/main/func_8005557C/NOTES.md`.

## T-25105: main func_80055AFC

No match: `func_80055AFC` stays INCLUDE_ASM. It is the game-save loader, the mirror of `func_8005557C`. The best body is 113 rows at the original size (349 words), and it is kept in `drafts/T-25105/main/func_80055AFC/` (`best.c`, plus `best-plain.c` at 114 rows without the FAKE).

What closed:
- The bank spellings are the same as in the writer: spelling A for the straight-line code, the bank-array element for the loops, and one `u32` counter.
- Stores go through `GameState *g`. With direct field stores, uopt reuses the head's test loads, which the original reloads.

What is left:
- The head is a copy-propagation gap, the one in the T-11570 selector-copy family. The original keeps `arg0 = unk_1117 & 1` as a copy, so its bank base (`t5`) is one live range for the whole function. IDO 5.3 propagates the copy into the base and splits it. A volatile read through the splat label (`extern volatile u8 D_800E7397`) gives the original's head and one range. That label is a FAKE: it would need a keep line.
- The original never uses `t3` or `t4`. The draft has the counter's defs-only range in `t3`, so the original holds one more range at colour 11. That range is not found yet. Together with the two missing frame words (0x48 against 0x40), it accounts for about 50 rows.
- The colour order of the unrolled 256-record loop (about 40 rows). The original colours the end constant first.

Measured negatives and the next hypothesis: `drafts/T-25105/main/func_80055AFC/NOTES.md`.

## T-25107: MASTER func_801363F4

Matched (2,608 bytes, default build, MASTER sha1 OK), starting from the T-24517 body at 167 register-only rows. Both 0x44-byte record copies were in the wrong block-copy state. In our pipeline the `tools/ugen_pass.py` sentinel decides how many scratch registers ugen burns before a copy's end pointer (two in state B, one in state F). All stores before the first copy are direct global stores, so the pass chose B; the original is F for both copies. A marked FAKE never-true arm before the first copy, `if ((u32)D_8013C578 < 0) { do { *(u8 *)0 = 0; } while ((s32)D_8013C578 < 1); }`, gives the pass an `istr` followed by a signed `les` jump. That makes the procedure sticky F. ugen drops the arm, and as1 deletes the dead byte load. The last 5 rows closed when the point loop stored x before y, as record 1 does. Same open symptom: MASTER `func_801344E0` and ENDING `func_80134270`. Negatives and details: `drafts/T-25107/MASTER/func_801363F4/NOTES.md`.

## T-25107: MASTER func_801344E0

Matched (2,912 bytes, default build), starting from the T-24519 body at 171 rows. The first divergence was the first record copy's end pointer in case 4, one ring slot high: the block-copy state miss that closed `func_801363F4` in the same file. The same two changes closed it: the marked FAKE never-true arm before the first copy (state F for both copies) and x before y in the point loop. The arm alone leaves 5 rows. ENDING `func_80134270` has the same recorded symptom. Notes: `drafts/T-25107/MASTER/func_801344E0/NOTES.md`.

## T-25103: main func_8006C988

Matched (69 words, 276 bytes; default build compiles the C, 27/27 sha1). From the T-24928 guard (5 rows): an `s32 cur` with `n++;` written before `cur = val;` at the end of the loop body keeps the copy as one plain `move` in the join block, where the base advance sits too (the then-arm's `b` steals the advance into its delay slot, which made it look per arm). The first loop written as `for (i = 0; i < 0x100; i++)` instead of `i = 0; do { i++; ... } while (i < 0x100)` puts the induction init before `i = 0` and closed the last two rows. The T-24928 dead `first`/`last` tests are still needed (FAKE). Notes and negatives: `drafts/T-25103/main/func_8006C988/NOTES.md`.

## T-25103: main func_8006A044

Matched (162 words, 648 bytes; the default build compiles the C; 27/27 sha1). It was down as an allocator artifact (T-25012, T-25013 levers 1 and 5, T-25014: about 87 rows). The cause was the source shape. Every flag update in the original is a bit-field store: `lui rA; lbu rB,(rA); ori; sb`, and on the promoted byte `move t,v0; ori; andi v0`. A plain `|=` gives `lui t; lbu t,(t)` with no copy. Single-bit stores through a new `FlagsF64` view of `D_800E6280.unk_F64` (`include/main_api.h`) took it from 174 rows to 86. A dead read of `unk_F64[1]` at the top (FAKE) numbers the byte before the month, which fixes the `$v0`/`$v1` swap. An `s32` (implicit int) return keeps `$v0` live at the exit, so the last store of each arm goes through the promoted byte. That gives MATCH. Controls: with `void` 40 rows, without the dead read 86. Notes: `drafts/T-25103/main/func_8006A044/NOTES.md`.

## T-25103: main func_80067870

Not matched; back to `INCLUDE_ASM`. Best body 75 `funcdiff --resolve` rows (the T-17002 draft measures 122 today), saved with the evidence in `drafts/T-25103/main/func_80067870/` (body.c, NOTES.md). Real fixes: `(-dx * 2) / 3` (the original negates, `* -2` spends `at`), `D_80125C55--` (`cvt_pass` widens no `dec`, so the byte stays a ugen temporary), `x1 = dx + D_80125C50` and the 0x8-case bodies on one line (autotweak); one FAKE `(u32)` on the `D_8011F46A` store gives cvt(D_80125C52) a fourth use so it takes `$a0`. What is left is two allocator effects. (1) `D_8011F426 = y1 - dy` is copy-propagated to `y1 - (15 - ret)`. The T-17002 empty test splits that sub's two uses across blocks, and uopt colours it (`$a1`, prio 1.5) although the emitted code keeps it in `t8`. That phantom pushes `unk_F80` to `$a3`, so `D_800B5C08` cannot be one `$a3` range and the ugen ring runs one register late from word 14 to 97. Three empty tests (D=3) put `unk_F80`/`D_800B5C08` right but the sub then takes `$t0` from x1; an always-false `dy` redefinition removes the range but drops the `move` (235 words). (2) `D_80125C54`'s global range (`$a3`) and cvt(D_80125C54) (`$a2`) interfere, the T-0018 promo gap: about 12 words, with no source lever found. Next hypothesis: in NOTES.md.

## T-25103: main func_8006B0C8

Not matched; back to `INCLUDE_ASM`. Best body 130 `funcdiff --resolve` rows (a natural draft: 479), same length (318 words), words 0-70 equal; body and evidence in `drafts/T-25103/main/func_8006B0C8/` (body.c, alt-split-counter.c, NOTES.md). Real fixes: the compaction counter is the clear loop's counter (`for (j = n; ...)` right after keeps the first loop rolled with the `sltu` walk compare), the key is `day + month * 100` stored as `if ((keys[j] = e) < min)` or a separate store (the reread spelling unrolls by 2), and `for (n = 0, j = 0; ...)` for the entry order. FAKE: `s32 pad[10]` (the original has ten more words below `keys`), a `do { } while (0)` around the min update (a block boundary, so uopt does not forward the `keys[j]` store into a `$v0` CSE register and ugen forwards it in a ring temporary, as in the original), and a `goto swap; swap:` label before the swap. The label is the main finding: IDO keeps `&D_800E6280` in a register (`rlda`) from the entry to the swap, the original rebuilds it with `lui/addiu` at every use, even inside the call-free compaction loop; no matched C function in the tree builds a full address inside an innermost call-free loop. Left: uopt's order of strength-reduction temporaries (the original inits and increments the `keys` walk before the entry walk, and in the month loops the end before the start), and the ring offset that follows. With a separate counter for the pre-sort loops and the compaction test written as `if (!active) continue;` (`alt-split-counter.c`, 254 rows) the inner order and temporaries become the original's, the base stays unallocated without the swap label, and every word has the original's opcode except two schedule spots; what is left there is uopt colouring (first-loop walk/end, the counter, min and the entry walk) and the month-loop preheader order. Next lead: regdiff that shape with a traced uopt.

## T-25103: main func_80067F04

Matched (613 words, 2452 bytes; the default build compiles the C). It had never been worked by hand, only raw m2c drafts (T-14500, T-17000). A new body from the asm measured 489 rows. Separate mark blocks for the 0x6F arm and the else arm of the day-schedule loops, plus two more locals for the 0x80 frame, gave the original's 613-word shape at 519 rows. The first divergence was the init loop keeping the constants 8 and 0x62 in `$s7`/`$fp`. The cause sat much later: sections B, C and D each test the month twice (`month == unk_03F`, then `month == unk_03F + 1` after a mark). With `D_800E6280.unk_03F` in both tests, uopt shares `cvt(unk_03F)` across the mark's calls, and the extra ranges change the colouring of the whole function. The first test of each pair reads `gs->unk_03F` through the `GameState *gs` pointer, which the original also keeps in its sp+0x7C slot for the loop bounds. That makes it a separate expression with the same absolute `lui/lbu`: 519 rows to 16. Each mark (three record stores and `n++`) is one source line through a `CAL_MARK` macro, `unk_03F + 1 == month + 12` gives the operand order, and `left = 0; n = 0;` before the date extraction gives the delay slot. One FAKE is left: `s32 pad` puts `kind` in the sixth local slot. Notes and negatives: `drafts/T-25103/main/func_80067F04/NOTES.md`.

## T-25104: main mod_trans_vram

Not matched; `INCLUDE_ASM` kept. Best body 47 rows (was 226, T-24501), 187 against 188 words, in `drafts/T-25104/main/mod_trans_vram/mod_trans_vram.c`. What closed: the `mod_set_gpu` loop shape (u8 rows/cols, cols reloaded in the inner loop, rows inside `if (cols != 0)`); LoadSquare arguments with no explicit mask (the u16 prototype gives the hoisted and the call-site `andi`); one `size` variable whose else-arm `sw; li a1,16` is uopt's own split, made profitable by writing the column offset as `col * size` (strength-reduced) instead of an `xoff` accumulator; a declaration order that gives frame 0x1A0 and every home with no pad; `ypos` and `xh = x / 2` locals that put the hoisted s5..fp temporaries in target order; `uv = *data; uv &= 0xFF0000;` so a uopt colour takes `t0` and the ugen pool matches through word 122. Left: the loop-head tile word is in `v1` (target `t0`) with uv/bank colours swapped. A uopt-output dump (cvt_pass shim, decoded with its parser) shows the target needs a raw-word local that is coloured after `bank` and cannot take v0; no tested source gives that. Negatives and the next hypothesis: `drafts/T-25104/main/mod_trans_vram/NOTES.md`.

## T-25104: main mod_set

Matched: `mod_set` (195 words, 780 bytes) is default-build C, 27/27 SHA1. A fresh body on the `mod_set_gpu` loop shape was 195 words at once; the remaining causes were: `switch (w)` for the tile-size arms (block order default, 8, 16), x/y positions as expressions (`x + col * w`, `row * w + y` with `s32 row`), `s32 pri` passed to the u16 GsSortSprite prototype (`func_80098B80`, declared in `src/main/8004C3F0.c`; `func_80043510` keeps its implicit view), FAKE stack pads for the 0xE8 frame, FAKE one-line switch cases (as1 stores mode first in cases 3-5), and the last 10 rows: each tile field masked and shifted in one statement (`clut = (*data & 0xFF00) >> 8`). The split mask-then-shift form gives clut 2 uses + 2 defs (uopt priority 133), above the x position (103) and col (78); the one-statement form has priority 67 and the same instructions, because ugen still computes the mask in the variable's register. The rebuilt uopt trace (`regdiff -v`) showed this directly. Same symptom in the `mod_set_gpu` guard (clut outranks the x counter); try the one-statement form there first. Details and negatives: `drafts/T-25104/main/mod_set/NOTES.md`.

## T-25104: main mod_set_gpu

Not matched; the guard is at 4 rows (T-25009: 17). Registers, order and every stack slot but one match. Closed, one cause each: the tile fields masked and shifted in one statement (the `mod_set` lesson; with it the T-25009 FAKE `if (last) {}` had to go, 17 -> 8), the latch as `col++; data++;` (the original increments col, the x counter, then data; 8 -> 6), and `ypos = row * 8 + y + 0x168` inside the inner loop, so the hoisted ypos follows the x counter (6 -> 4). Measured rule for the spill temporaries: they sit below the last declared home. The spilled copy-propagated `p` expression comes first, then the inner preheader items in expression (bit) order. The x counter IV in `$s4` also takes a slot, which nothing reads or writes. Left: `p` at 0x68 instead of 0x6C. The original frame needs one more word between the declared homes and the temporaries. A control with a FAKE extra local after `p` plus `if (&p) {}` (p spilled at its own home) gets every slot right, but the latch load of `p` then goes to a ugen temporary instead of `$a3` (7 rows). Strongest lead: with `u8 rows, cols` and one pad above `row`, `s->x0 = (s16)(x + 0xA0) + col * 8` gives the original frame and slot map exactly, because `x + 0xA0` becomes a hoisted item of its own ahead of the IV. The cast adds `sll/sra` and splits the IV, so the original must keep `x + 0xA0` as an un-reassociated subtree with no cvt. Negatives (idx reuse, zcopy, a one-element array, x/ypos spellings, permuter 900 s) and the next hypothesis: `drafts/T-25104/main/mod_set_gpu/NOTES.md`.

## T-25104: main srn_tpage_show

Matched: `srn_tpage_show` (806 words, 3224 bytes) is default-build C, 27/27 SHA1. Start: the T-24046 body, 592 rows (812 against 806 words). Three causes, in order: one line per switch case (`divisor = 4; pageWidth = 0x40;`, FAKE layout: the target stores the width first); no `(s8)` casts on the V coordinates (in the middle page the cast built a real `sll/sra` sign extension and its own common subexpression with a spill slot: 812 -> 807 words, 57 rows); each quad's colour, UV and XY stores as one comma statement in the libgpu setRGB0/setUV4/setXY4 field order (FAKE layout, likely the original's macros), which fixed all store order and the v0/v1 tie of the column U values at once. Lesson: for a POLY_FT4 builder try the macro shape before register work. Notes: `drafts/T-25104/main/srn_tpage_show/NOTES.md`.

## T-25201: TAIIKU func_80137784 (closer)

No match; the guard stays at 4 rows (the two compare operand orders). Dumped uopt's input and output ucode for the compare: uopt swaps an `==` whose first operand is not a plain variable, then moves a constant addend of the second operand onto the first (uopt for an LDA sum, ugen when `(s32)` casts hide it). So an alias written with an offset (`(Fix32 *)&D_8014985C - 4`) always ends up first, and putting it on the left only adds `addiu t8,a0,16`. Pointer first needs the constant on the left with a zero offset. With `D_8014984C` itself that order is right, but the base constant (one reload-free unit, priority 0.5) outranks 0x44 (0.4) again. A positive control with undefined zero-offset alias symbols (`D_8014984C_alias == &D_8014984C[arg0]`) matches every instruction; the 8 rows left are its unresolved relocations. Next step, which needs approval: two overlay-only alias symbols in `config/symbols_TAIIKU.txt`. Details and negatives: `drafts/T-25106/TAIIKU/func_80137784/NOTES.md`, draft `alias-8rows.c`.

## T-25153: NAME_ENT func_8013AF78

Not matched. The T-24132 guard (99 rows) is gone and the source is plain `INCLUDE_ASM` again. The best
body is in `drafts/T-25153/NAME_ENT/func_8013AF78/body.c` (57 funcdiff rows, two FAKEs); NOTES.md there has
the full record.

- Closed: the `sltu` loop comes from reusing the loop counter after the loop, as in RPG_BAT
  `func_8014E994`. The masked values are int-typed (`x = D & 0x7F` gives the original
  `lui X; lbu X; andi t,X; or X,t,zero`; a `u8` local adds a CVT and loads into a temporary). The 0x30
  frame is `s32 a, b; s32 sel[3]; s32 c, d;` with a value local as the counter (IDO frame is
  round8(locals + 4), and every declared scalar takes a slot).
- Closed with FAKEs: `c` splits into a CSE and a local because its mask stays available in the `sel[0]`
  arm. An aliasing selector store, `((s32 *)(u32)sel)[...]`, kills that. An empty `if (arg0) {}` keeps
  `arg0` in `$a0`, because bits follow the first reference.
- Left: `d` and the loop end bound need `$v0`. In IDO 5.3 a range used in the zero arm, whose
  `return 0` defines the return register, never gets `$v0`. `return d` costs a final move.
  The next hypothesis (zero-arm stores through the AND CSE, so `d` has no zero-arm unit) is in NOTES.md.

## T-25101: main sndi

`sndi` (`src/main/8004F870.c`, 263 words / 1052 bytes) matches as default C. [[tickets/T-25101-w8-main-8004F870]]; evidence and negatives in `drafts/T-25101/main/sndi/NOTES.md`. The old blocker (T-24501, T-25027) was a strength-reduced cursor, held back with FAKE integer-address casts. The target's scans have two bases: the cursor uses the buffer address and the loop test uses a preheader copy of it. Plain C gives both with a scan pointer in the loop init: `for (i = 0, s = buf; s[i] != 0; i++) { p = s + i; if (*p == 'P' && (i & 1) == 0) { ...; break; } }`. A cursor written `&buf[i]` or `buf[i]` is strength-reduced. `p = buf + i` with the test `buf[i]` is not, but the copy is missing. `s = buf` before `i = 0` schedules the copy before the entry `beqz`. The `i = 0` in the found arm and on the exhausted exit is the next loop's init, moved there by uopt, so no explicit resets are needed. One scalar (`s`) declared before the arrays sets the frame. Only the inferred buffer sizes remain FAKE.

## T-25101: main func_80052F68

No match. Best body: 69 rows (compared NON_MATCHING C), up from the 604-row archived m2c drafts. It is over the guard limit, so the tree keeps `INCLUDE_ASM`; the body and notes are in `drafts/T-25101/main/func_80052F68/`. What closed: the loop-1 unroll (its own counter, because a counter assigned again later keeps a loop rolled), the frame (0x80, val at sp+0x48), the loop tests (an outer counter used only in loop 2 gives `bne`; the inner counter shared with loop 3 gives `slti`), and the loop-1 induction registers. Those last need table bases at 0x800B4348, 0x800B4354 and 0x800B4360, which have no splat label and would go in `config/symbol_addrs_main.txt`, plus a FAKE store through a copy of the array address. What is left: the constant 10 gets v1, and the loop-2 counters get a0 and v1, where the target has 10 in a0 and the counters in v0 and v1. Its `li a0,10` in loop 1's preheader is as1's global filler acting on that allocation; the ugen listing has the `li` after the clamp. The next step needs the uopt allocation trace.

## T-25101: main func_8005215C

Matched as default C, 1260 bytes (316 words), from INCLUDE_ASM. [[tickets/T-25101-w8-main-8004F870]]; evidence and negatives are in `drafts/T-25101/main/func_8005215C/NOTES.md`. The work started from the matched sibling `xa_wait`, not from the T-25021/T-25022 checkpoints. Four steps:

1. Every mode read is `*(u8 *)&D_800E6280.unk_03C`, with no local. uopt keeps the byte as one promoted global that it reloads after each call. The `(u8)` cast on the s8 field adds conversion ranges that cache it, which was the T-25022 blocker. 183 rows to 49.
2. `pad[2]` gives the 64-byte frame, as in xa_wait.
3. The plain argument in `k_disp_switch(F61, 0)` gives the F61 CSE range an a0 preference, so the range takes a2. `+ 0` (FAKE) gives v1 for F61 and a0 for F62.
4. Both call-pair tables are word-indexed with `[D_80125BFC * 2]`, as in `func_80052060`. The shift is then one shared ugen temporary. Int arithmetic and pointer views make it a uopt range in v0; struct indexing in separate statements emits two shifts.

## T-25101: main get_h_tokimeki

Matched as default C, 1600 bytes (400 words), from INCLUDE_ASM. [[tickets/T-25101-w8-main-8004F870]]; steps, evidence and negatives are in `drafts/T-25101/main/get_h_tokimeki/NOTES.md`. The archived m2c drafts were unusable, so the body is a typed reconstruction from the asm: a seven-step threshold ladder per girl kind (six cases), one `s8` bonus per selected parameter.

1. The seven levels `D_800B4198[k]` are read into `u32` locals after the flag loop. This gives their t0..t5/s0 colours (they are numbered before the loop body) and, with `u32` slots, the 136-byte frame. `u8` locals pack and leave the frame 24 bytes short.
2. The flag array `s32 use[8]` is declared after the eleven scalar slots and before `bonus`, which puts `&use[1]` at sp+0x40.
3. The last 230 rows were the bonus row and the parameter value swapped (s2/s1). Unconstrained ranges are coloured in bit order, and copy propagation re-enters every expression whose operands changed after the main pass. The row pointer is propagated (late bit), the parameter load was not. Reading the parameters through `GameState *gs = &D_800E6280` makes the value a re-entered expression too, numbered after the row: 0 rows.

Tool gap: `funcdiff --version c` reported "linked function bytes differ" with 0 rows, because the guarded compile does not apply the `.data_800B40F0` split, so the D_800B41xx references link elsewhere. Check the default build in that case.

## T-25101: main get_near_menu

Matched as default C, 1784 bytes (446 words), from INCLUDE_ASM, with no FAKE. [[tickets/T-25101-w8-main-8004F870]]; the steps and the control are in `drafts/T-25101/main/get_near_menu/NOTES.md`. The archived m2c drafts were not usable, so the body is a new reconstruction: an offset table `diff[16]`, a direction filter, and a nearest-entry scan with tie breaks that call `abs`.

1. With the tie block as an if/else-if chain on `mode`, the parameter was not allocated (regdiff "-ve save"). It stayed in its home slot, every saved register moved by one, and the result was 352 rows. The same arms as `switch (mode)` put `mode` in fp and the constant 1 in a2, as in the target, which left 9 rows. A control with only the switch reverted confirms that the switch is the lever.
2. The 9 rows were the `diff` base (sp+0x94 against 0x88). Declaring the array after the four scalars `g, n, best_d, best` matched.

## T-25101: main func_8004FC10

Matched as default C, 1812 bytes (453 words), from INCLUDE_ASM, with no FAKE. [[tickets/T-25101-w8-main-8004F870]]; steps and controls are in `drafts/T-25101/main/func_8004FC10/NOTES.md`. No earlier draft was in the tree, so the body was rebuilt from the asm with the sibling views (`MenuSelection5`, `GsRecF90 (*)[2]`, a `GameState *g` local). It is the menu cursor step: four modes on `unk_1092 & 3`, d-pad wrap with a skip over disabled entries, `get_near_menu` in mode 3, then the pointer clamps.

1. The first typed draft had 140 funcdiff rows. Three changes together left only the frame: `(s32)` on the u8 count in the signed wrap tests (-cckr promotes u8 to unsigned, so the tests became `sltu`), `unk_00 + unk_04` in the position sum (load order), and case 1 as `if (F80 & 0xF) { } else { switch }`. The target jumps from the empty arm to the wrap tests, while `if (!(...))` inverts the branch. That change alone is 52 aligned rows.
2. The frame was 8 bytes too large. One `s32 i` for the scan loops and the `get_near_menu` result, with no separate local, put the `g` home at 0x4C and the a2 spill at 0x30: MATCH.

## T-25201: main mod_set_gpu (closer)

Matched (default-build C, 804 bytes, 27/27). The last 4 rows were p's spill slot. Three changes: the loop bounds read from the record in `for` conditions with no `rows`/`cols` locals (uopt keeps both counts in v1/v0 as partial-redundancy items and reloads them at the inner latch and the inner exit, which is the original's odd `rows = p->unk_0D` placement); a FAKE dead store `idx = 0;` right after `p = ...`, so uopt does not copy-propagate p and every use reads p's own home (the latch reload stays in `$a3`); and an `s32 xpos` local before `ypos` for the home word that puts p at sp+0x6C. Rules read from the uopt decomp (uoptcopy.c, uoptreg2.c `spilltemps`, uopttemp.c): copy propagation is refused into a loop-latch branch condition, when the assignment is not available at the block, when its operands change in the block, or when it contains a load; temporaries are given in bit order and shared between items whose regions do not overlap. Details: `drafts/T-25104/main/mod_set_gpu/NOTES.md`.

## T-25152: TACO func_80138B2C

[[tickets/T-25152-w8-TACO-80138890]]: not matched; guard from 30 rows to 3 (`src/ovl/TACO/80138890.c`). Cause of the old register miss, from the uopt source (`cupcosts`): a parameter range with a live unit in the entry block cannot take `$v0`/`$v1`. The stage and the total both live in the unused K&R parameter `arg0`, which puts them in `$a0` and the time bonus in `$a1`. Two marked FAKE address views (`*(u8 *)&D_8015EDCC`, `*(s32 *)&D_8015EDF0`) stop copy propagation (`has_ilod`), so arg0 stays a variable. With the total in arg0 instead of the promoted `D_8015F490[4]`, the total is stored in place before the time bonus; a promoted global is stored at the branch, after all plain stores of the block. Computing the total before the four bonus stores fixes the as1 order. Left: 9999999 in the clamp arm is built in `$t2`, not `$a0`, because uopt copies the constant into the store. Negatives and next hypothesis: `drafts/T-25152/TACO/func_80138B2C/NOTES.md`.

## T-25152: TACO func_80138B2C (closer)

[[tickets/T-25152-w8-TACO-80138890]]: matched (default C, 476 bytes). The last 3 rows were the clamp constant in `$t2`. uopt's input pass replaces a load of a variable with the constant stored to it earlier in the same block (`uoptinput.c`), so `arg0 = 0x98967F; D_8015F490[4] = arg0;` stores the constant and ugen builds it in a ring temporary. For an expression used once, the load stays a read of the variable when the expression has an indirect load (`has_ilod`) or an operand killed in between (`treekilled`). `arg0 = 0x98967F + *(u8 *)&D_8015EDCC * 0;` (FAKE) keeps the def in `$a0`, and ugen folds the zero term. Reading `arg0` in the zero term also puts the arm in `$a0`, but it makes the total live into the arm, and the total then gets its own range in `$v1` (25 rows). Variants: `drafts/T-25152/TACO/func_80138B2C/NOTES.md`.

## T-25151: GYOZI func_801412A4

Matched: `func_801412A4` (172 words, 684 bytes) is default-build C. Started from the matched sibling
`func_80140F90` instead of the T-25012 seed (96 rows). Four causes, in order: the random word read
through the member `D_800F53A0.unk_10C8`, which may alias the `unk_F2F` store, so the load stays in the
retry loop (the donor's `* 1` and the plain `D_800F6468` symbol both hoist it, 276 rows); `D_801486E4`
read once into the reused s16 counter (only s16 gives `beq a2,v0`); an `(s32)` view of that u32 word,
which puts the load first in the sum (FAKE: `801418B0.c` needs the field unsigned); and the count-loop
body on the `for` line (FAKE layout, as1 orders by line). Details and negatives:
`drafts/T-25151/GYOZI/func_801412A4/NOTES.md`.

## T-25108: main func_80065964

[[tickets/T-25108-w8-main-80062CD0]], branch w8-main-80062CD0. Matched in plain C (424 B), from about 68 rows (T-25010 draft).

- **Eight variables, not four.** The original's eight coordinate registers (four copies, one pair per corner test) are two local arrays `s32 cx[4], cy[4]` tested in an inner `for (j = 0; j < 4; j++)` loop. IDO unrolls the 4-trip inner loop and scalarises the arrays (eight ranges), so the 15-trip outer loop is no longer innermost and stays rolled (`li at,15; bne`), with no `goto`. The register pressure pushes the other record's address and ty into s0/s1 and gives the 0x68 frame.
- **The dead stores are CSE temporaries.** sp+0x20..0x28 sit below the declared locals' 15 words: they hold the shared subexpressions (`&D_80125C10[idx]`, x0, y0, x0 + 19, y0 + 15). They only appear when the arrays are filled from repeated expressions (`cx[1] = p[0] + dx + 19;`), one source line per corner; `cx[1] = cx[0] + 19` or declared `x`, `y` locals do not give them.
- **Last rows.** The fourth bound test reads `cy[1] > 120` (t1), the third `cy[0] < -120` (a3); the other record is read inline (`((s16 *)&D_80125C10[i * 4])[0]`), as a `q` pointer it takes a2 instead of s0. One unused local (FAKE) is the 15th local word; without it the spill slots move by 4.
- Measurements and negatives: `drafts/T-25108/main/func_80065964/NOTES.md`.

## T-25108: main func_80065B0C

No match: guarded at 9 funcdiff lines (2 instructions out of 266 words) in `src/main/80062CD0.c`. Before this pass the function had only m2c census drafts (313 rows). The selection-cursor step of the 15-icon menu.

What closed:
- **The selection index through `GameState *g`.** The original keeps `unk_1093` in `$s3`, re-read after every call, and recomputes the `D_80125C10` address for the y store after the x store. Read directly, the index stays in temporaries and the address is shared (169 rows). Through `g->unk_1093` it is an ILOD: the x store kills it for CSE, the extra use gives the range `$s3` (125 rows). Every other GameState read goes through `g` too; at the `n` test this also stops the `unk_F6C` read from being PRE'd into `$v0`.
- **An unsigned switch selector.** `switch ((u32)g->unk_F80 & 0x5000)` keeps the `case 0x1000` constant apart from the loop's halfword 0x1000 stores; signed, one register holds both and the loop counter and the constant swap (125 to 25 rows).
- **One multiply in the flag index.** `unk_F90[i * 2].unk_06` is two multiplies in ucode and ugen spends a temporary on the folded one, so the loop's else arm is one ring slot late; `((GsRecF90 (*)[2])g->unk_F90)[i][0].unk_06` (the 8004F870 idiom) closes it.
- `D_8011ECF6`/`D_8011ECFA` as `((DisplayActor44 *)D_8011ECD0)[0].unk_26`/`unk_2A` keep the y load behind the x store in as1. New externs `s8 D_800B5A9C`, `u8 D_800B5AA0`.

What is left: the else arm sets the shared constant 3 (`$s5`) before `jal menu_check` in the original and after the index reload in the draft (uopt moves the join's reload to the end of the else arm). From the IDO uopt sources, the original's `$s5` range covers the menu_check block, which needs a reference to the constant there or in the entry block; none of about 30 variants found one, and autotweak and the permuter found nothing. Analysis, negatives and the next probe: `drafts/T-25108/main/func_80065B0C/NOTES.md`.

## T-25201: main func_80065B0C (closer)

Matched (default-build C, 1064 bytes, 27/27) from the T-25108 guard at 9 funcdiff lines. The last miss was where the shared constant 3 (`$s5`) is loaded in the else arm: before `jal menu_check` in the original, after the index reload in the draft. The R9SAVE units of the traced uopt show the constant's range (divisor in block 1, three loop stores) with no unit in the else blocks 25/26/58; uopt's `regdataflow` makes a block part of a range only where the item has appeared on some path from the entry, so the join's reload went to the end of the else arm. The T-25108 store probe `rec[64].unk_01 = 3` made no unit because a store to a fixed address is a `str` to a variable, and its value is optype 0 (small constants are not register candidates). An indirect store (`istr`, value optype 3) is one. The fix is a never-true arm before menu_check, `if ((u32)a < 0) { rec[a + 64].unk_01 = 3; }` (FAKE): uopt keeps it, the else blocks join the `$s5` range with the reload at the start of block 25, and ugen removes the arm. `*(u8 *)a = 3` and `[a]` give the same bytes; the arm after menu_check gives 6 rows. Notes: `drafts/T-25108/main/func_80065B0C/NOTES.md`.

## T-25108: main func_8006509C

[[tickets/T-25108-w8-main-80062CD0]], branch w8-main-80062CD0. Matched in plain C (1076 B, no FAKE), from score 99 (T-24301 draft, T-24501 measured 95).

- **The case 2 copy reuses the count variable.** After `n >= 2` the copy runs `for (n = 0; n < 15; n++)`, so its strength-reduced pointers are new uopt bits. On `i` they share the bits of the case 0/1 copies: one range spans several cases and takes one colour, so the dst/src registers of cases 0/1 swapped and the scan count took the end constant's register (56 to 0). The scan has its own counter `j`.
- **The `i = 0` between the first two loops sits at the join** (a `for` loop): uopt PRE moves it onto the loop-1 exit edge as in the target (99 to 93). The first record half stores `unk_14` right after `unk_34` (93 to 89).
- **Every GameState access goes through `g`.** The u8 views on F6C/F6D and the `(u8)` switch cast of the old draft are no longer needed once the reads use `g`. All loops are `for (i = 0; i < g->unk_F6D; i++)`.
- New K&R declaration `void func_800654D0();` in `include/main_api.h` (an `(s32 n)` prototype breaks the OPTION.bin checksum). Steps and negatives: `drafts/T-25108/main/func_8006509C/NOTES.md`.

## T-25109: EN_NICHI func_80136DEC

No match. A guard holds the best body: 348/348 words, frame 0xF8, all registers and table offsets right. 4 aligned rows are left (two delay-slot fills; `funcdiff --resolve` counts 258 positional rows because each fill shifts one word). The start was the T-24516 body at 191 aligned rows (347 words, frame 0xF0). Five source facts closed the rest. (1) The case 3 / inner case 4 clears are one `for (i = 0; i < 5; i++)` loop on the case 1 counter. The shared, longer range for `i` lets the kinds cursor take `s0` first, and it gives frame 0xF8. (2) Two scalars are declared above the stack tables, and there is no record-pointer local. (3) The selector is not a direct `switch (D_800E6280.unk_110A)`: with cfe's switch temporary, IDO neither folds the peeled iteration of the 21-trip flag loops (`sll tN,zero,6`) nor reloads the case 2 alpha byte. `GameState *gs = &D_800E6280; switch (gs->unk_110A)` fixes both and puts the selector in `v1`. (4) Case 0 uses plain `((s32 *)D_801391E4)[i]` loops. (5) The init loop runs in ugen evaluation order (`unk_01` first, `unk_14` before `unk_05`); as1 reorders the stores. Left: as1's global filler fills D1 and D2 of the dispatch from the case heads (`move s1,zero`, `lui t4`), where the original fills them from the next compare. The traced as1 shows RPO numbering with the case head first, so a different CFG or region is needed. Details, negatives and the next hypothesis: `drafts/T-25109/EN_NICHI/func_80136DEC/NOTES.md`.

## T-25109: EN_NICHI func_8013735C

Matched (2,300 bytes, default build), starting from the T-24516 body at 453 aligned cost (552 of 575 words). Semantic fixes came first: the sibling fade loop in case 3, the case-1 fill loop `for (; n < 5; n++)` (it stays rolled, and loop 1 unrolls), and the case-4 `if (all < 0x10) {} else if (C4C[0] == 0xFF && C4C[1] == 0xFF) {}`. That gave 276. Three findings close it:
(1) uopt removes an empty or dead-store arm whole. A never-true `do {} while ((u32)n < 0);` survives the branch cleanup and ugen folds it, so the arm keeps the original's two `bne` (FAKE).
(2) Every `mpy x, 68` of a function shares one hoisted-constant range (`constinreg` optype 5). An index expression `rec[x + 98]` therefore multiplies with case 1's `li a3,68` register. A plain index on a record-98 base, `((DisplayActor44 *)D_8011ECD0 + 98)[x]`, is an `ixa` with stride 68, which ugen expands to `sll 4; addu; sll 2` as in the original (276 to 76).
(3) The final `t1`/`t2` swap was the priority order of the hoisted 0x8011ECD0 (7.75) and 0xFF (8.5). A never-true two-term loop between case 1's loops raises 0xFF's block denominator from 6 to 7 and emits no code (FAKE).
A dead `unk_F5F` test (FAKE) fixed the ugen ring offset. Details, negatives and controls: `drafts/T-25109/EN_NICHI/func_8013735C/NOTES.md`.

## T-25110: TACO func_8013DEE4

Matched (1344 bytes, default C) from the T-24502 69-row body in about 25 minutes. Three causal steps: (1) the target's rot and offset homes (0x60/0x54) are uopt spill homes of strength-reduced `o * step` and `o * 4`, not declared locals: an index loop with `&pt[j * 2 + o]` restores them and the latch (69 to 21 rows; `&pt[o + j * 2]` reduces the full address and gives 93); (2) `mode = kind` after the first switch with the if tests on `mode` and the jump-table switches on `kind` puts the tests on kind's home and the switches on the spill copy, and lets uopt keep 5 and 4 in s3/s1 (21 to 3); (3) the colour value is a `case 5 / case 6 / default` switch; its cfe temporary takes one slot, so the old `pad[2]` became one `s32 pad` (FAKE, T-25110). The TACO.h declaration is now `s32` (explicit -1 exit). Notes and negatives: `drafts/T-25110/TACO/func_8013DEE4/NOTES.md`.

## T-25110: TACO func_8013EAA4

Not matched; guarded at 4 rows (from the T-24502 96-row `coordinate.c`; default build stays INCLUDE_ASM). Four causal steps: (1) the frame is the declared slots top-down (0x110..0x70: MATRIX 0xE8, v 0xC0, pt 0xB0, pos 0xA8, depth/flag 0x94/0x90, kind/phase 0x88/0x84, height 0x74; the eleven other scalars fill the single slots, the 32-byte gap is a FAKE `pad[8]`), then uopt temporaries from 0x6C down; (2) the third grid as a plain index loop (`o * 0x100`; uopt replaces the test by Y != 0x200 itself); (3) the first grid as an index loop (`o * 0x80`), which adds the loop-1 induction temporary and fixes the init order; (4) `row = D_8015F568[kind];` as its own statement after the frame if/else fixes the CLUT registers. The four tables are declared in `ovl/TACO.h`. Left: the third grid's Y temporary shares the particle pointer's spill home (0x64) instead of 0x58. uopt's `spilltemps` (n64decomp/ido decompilation) hands out temporaries in bit order, first fit, blocked only by lower-bit items sharing a graph node; keeping the pointer live past the grid moves Y to 0x58 but adds two temporaries (frame 0x118). Negatives, probe scripts and the next hypothesis: `drafts/T-25110/TACO/func_8013EAA4/NOTES.md`.

## T-25150: KANGEI func_80134724

Matched (default C, 688 bytes, 27/27), after the T-24201/T-24902/T-25042 history of the as1 default-arm hoist. The cause was the file's data, not the function: the original object 80134120 owns `.data` 80139B30-80139C80, and its scene table `D_80139B30` names func_80134724. as1 (`func_41dc10` in the recomp source) gives every block that ends in an indirect jump other than `jr ra` an edge to every address-taken label of the file, and a function named by a `.word` in the file's data is one. With the table in C the switch dispatch has a back edge to the entry, the loop makes the dispatch lighter (weight 2) than the default arm, and the range-check slot takes the dispatch `sll`; the default arm then needs no FAKE cast. Done with `tools/data_island.py KANGEI/80134120` and the table defined in C; two main functions got link names in `config/symbol_addrs_types.txt`. Lead for other functions: an unmatched switch whose object's original `.data` names functions. Details: `drafts/T-25150/KANGEI/func_80134724/NOTES.md`.

## T-25149: NAME_ENT func_80138C1C

[[tickets/T-25149-w8-NAME_ENT-80132000]]: matched (default C, 740 bytes; NAME_ENT sha1 OK). The T-24515 body (pointer walks, volatile space reads) was at 104 rows. What closed it: index loops over the three name arrays (`D_8014CC74[i]`, `i += 2`) with the copies stored through `GameState *gs = &D_800E6280`. The strength-reduced pointer lets as1 hoist the store-block `lbu` above `sb` (a declared `u8 *p` gets no `.noalias` mark), and the `gs` stores make uopt keep the three second-byte loads separate. The full-width space bytes are read as `D_800B3644[0]`/`[1]` in the tests, not through locals: uopt promotes them (two `lui`/`lbu` pairs), and the comparison keeps the loaded byte first (`beq t6,t0`). ugen puts a register variable before a loaded expression, so local copies of any type give `beq t0,t6`. Then `for (i = 0, n = 0; ...)` for the `move a1,zero` position, `u8 buf[0x20]` for the 96-byte frame (FAKE), and `(x + 1) & 3` on the two-bit `unk_09` fields for one more ugen temporary in each date (FAKE, 62 rows to 0). void would match this function, but the caller `func_80138F60` needs the s32 declaration (FAKE). Details and negatives: `drafts/T-25149/NAME_ENT/func_80138C1C/NOTES.md`.

## T-25148: ENDING func_801387B0

Matched: `func_801387B0` (188 words, 748 bytes) is default-build C. It started from the T-25002 draft (156 diff lines).
Four causes, in order:
- **Boolean sharing.** uopt shares the largest common tree, so one spelling of `(flag & 1) == 1` gives one saved boolean. The original shares only the AND (`andi v1,a0,1`) and recomputes `xor; sltiu` per test. Four spellings (plain, `^ 0`, `* 1`, `<< 0`) stop the EQU sharing; `| 0`, `1U`, `1 ==` and `(s32)` fold into the plain form (FAKE).
- **Where `flag` is read.** `flag = D_800E6280.unk_53C[6];` before the if chain makes the first test use the shared AND; uopt sinks the load into the else block.
- **The 0x1C arm.** `if (D_8013C360 == 0) { } else if (D_8013C360 == 1)` brings back the original's reload of `D_8013C360` (same form as the matched func_80137F64).
- **The `D_8013CAA0 == 1` order.** Reading it into a local gives load-first in uopt's raw output, but `tools/cvt_pass.py` `_constant_first` swaps it back. Its `rewritten` set covers the whole unit and includes `D_8013CAA0` through func_80137BB4's FAKE entry loads. Reading the byte as `((u8 *)&D_8013CAA4)[-4]` (another location, FAKE) avoids the swap.

Possible tool follow-up: a per-procedure `rewritten` set in `_constant_first`, gated by lever_lab. Details, negatives and ucode traces: `drafts/T-25148/ENDING/func_801387B0/NOTES.md`.

## T-25111: TACO func_8013C6D4

Matched: `func_8013C6D4` (347 words, 1388 bytes) is default-build C. Start: the T-24502 body, frame 0x50 against 0x88. Closed by, in order: a pointer `for` for the first loop (`sltu` exit and the original setup order; index loops give `bne`) and an index loop for the second (`bne`, dead `i = 0` kept in `v0` by the `s32` no-return FAKE as in func_8013C1F4); homes from declaration order (`u32 i` between the flags takes 0x50, the register locals above them six words, a FAKE `pad[6]` the rest); one flag clear per mode case and the later clears after their calls; the bonus carried in `D_8015EDD4` itself with `case 7: default:`, which gives `li v0,1` and one store at the join (the T-24502 global form failed only for lack of the default). Notes and negatives: `drafts/T-25111/TACO/func_8013C6D4/NOTES.md`.

## T-25111: TACO func_8013BAC4

Matched: `func_8013BAC4` (460 words, 1840 bytes) is default-build C, 27/27 SHA1. Start: the T-24502 body, cost 330 (456 against 460 words, frame 0x4A8 against 0x4A0). Closed by, in order:
- the second distance compared directly, with a separate variable per loop;
- the score index base first, as in func_8013CE78;
- `continue` before the second score add, which stops ugen reusing the stored D_8015EDF0 value across the fall-through (322 to 90);
- loop 2 as an indexed `for`, so uopt's strength-reduced homes land at 0x5C/0x60 and the dead `i = 0` store stays;
- 11 scalars with i third and damage tenth (score 5);
- the s32 return with no `return`, for the loop-2 branch slot;
- `switch (*(u8 *)&D_800E6280.unk_1109)`, which keeps global copy propagation on. The tools/cvt_pass.py zcopy-off rule otherwise leaves the y distance as a range with `move a0,v0`, and as1 keeps that move in an s32 function.

`s32 func_8013D2A0(s16, s16, s16)` and `TcDamage9 D_8015F548` are now in TACO.h. The two FAKEs (s32 return, byte-view selector) name T-25111. Measured negatives and the toy evidence are in `drafts/T-25111/TACO/func_8013BAC4/NOTES.md`.

## T-25113: ETC func_8013F054

[[tickets/T-25113-w8-ETC-8013BEF0]]: matched (default C, 716 bytes, no FAKE), from INCLUDE_ASM. The best earlier body had 71 rows (T-24076). Three causes, each closed by one change:

1. A pointer do/while lets uopt CSE the base and the bound. An index do/while lets LFTR build the bound in `$a2` while the base stays a ugen temporary: 71 to 31 rows.
2. The entry load with its `move`: `i = D_80150064;` at the top. The promoted global and `i` tie on priority, and the lower bit (first appearance) wins.
3. The compare operand order in the loop: no `v` local, and the record read as an expression after `i--`.

A `GameState *g` pointer replaces the FAKE scalar views and keeps the two final `unk_F5F` reloads. Details: `drafts/T-25113/ETC/func_8013F054/NOTES.md`.

## T-25113: ETC func_8013D570

[[tickets/T-25113-w8-ETC-8013BEF0]]: matched (default C, 2352 bytes), from INCLUDE_ASM. The T-24078 drafts (aligned cost 604 to 618, about 670 to 850 rows) walked a `POLY_FT4 **` cursor with explicit page and UV counters, and the constant 160 then took a saved register from the page. Three steps:

1. Write it like the matched sibling func_8013D32C: `for (i = 0; ...)`, `poly[i]`, page `0x140 + i * 0x80`, UVs from `i`, coordinates as full expressions. The original preheader sets s0, s3, s4, s5, s7 and fp in register order, which marks them as strength-reduction and range-entry setup. Cost 604 to 20.
2. A height local `h = 0xA0 - arg0;` adds the missing scalar slot, which moves the UV induction spills to 0x50/0x54: 22 to 4 rows.
3. One source line per vertex (FAKE, layout only). uopt emits `160 - arg0` and `160 - Q11` under the x2 line (see `cc -S`), so as1 orders case 0 correctly only when x2, h and y2 share a line: 4 to 0.

Details and negatives: `drafts/T-25113/ETC/func_8013D570/NOTES.md`.

## T-25112: TACO func_80139FB8

Matched as default C, 1480 bytes (370 words), from INCLUDE_ASM, with no FAKE. [[tickets/T-25112-w8-TACO-80139170]]; steps, measurements and negatives are in `drafts/T-25112/TACO/func_80139FB8/NOTES.md`. The start was the T-24502 body (FAKE zero subtree), 179 rows in the full unit.

1. The mirrored half had v0/v1 swapped (input and angle result). Writing the angle result into the scale variable, which is coloured first, gave 169 rows; a separate local or `d`/`angle` cost 10 rows on the final body.
2. Without the FAKE, one sine variable for the front, +i and back entries (the -i sine stays separate) has priority 4.8, between the radius and the constants, and takes s5: 163 rows.
3. The last 163 rows were the constant colours: in the m2c `do { } while (i != 20)` form, 3 (3.5) beat 40 (3.444), and 0x100000 and 96 (2.5) beat the table address (2.333), so 0x100000 took fp and the table was addressed with `lui/%lo`. `for (i = 1; i < 20; i++)` (also with `!=`, also with an explicit step) changes the loop's block layout and matched. A do/while with `<` stays at 163. Check the `for` form when the s registers of hoisted constants are permuted in a constant-trip loop.

## T-25112: TACO func_80139424

Matched as default C, 1676 bytes (419 words), from INCLUDE_ASM. [[tickets/T-25112-w8-TACO-80139170]]; steps, measurements and negatives in `drafts/T-25112/TACO/func_80139424/NOTES.md`. The start was the T-24502 body, 282/265 rows in the full unit.

- **K&R `u8 arg0;` definition** keeps the glyph in s1 from entry (the header's `void func_80139424();` stays).
- **The frame homes list the locals**: the caller-save slots hold, top-down, x0, y0, x1, y1, x2, y2, x3, y3, w; x1 and x2 are memory-only copies. Declaring them in that order (two FAKE pad words below) gives the 0x68 frame.
- **One full expression per vertex, offset grouped first**: `D_800E6280.unk_014[i] + (arg4 - dx)`. uopt shares the centre load but finds no common sum, and the later re-association emits `centre + arg4` twice, as the original does. `(centre + arg4) - dx`, a centre local, explicit copies or chains all CSE or propagate (300+ rows).
- **setUVWH on one line** for the `@ * #` glyphs: the 0xD3/0xDF constants are coloured by first mention, which only the macro's field order gets right together with the store order. The letter index is assigned in its test, `(letter = arg0 - 'A') < 26`.
- **FAKE `x3 = 0;` at the top**: x0 and x3 tie at priority 1 and are coloured by uopt bit; the dead store gives x3 the earlier bit without changing the evaluation order. Real source unknown.

## T-25201: EN_NICHI func_80136DEC (closer)

Matched; the guard is removed. The T-25109 body was right. The last 4 rows (the delay slots of the
D1/D2 dispatch compares) came from data: the object's `.data` table `D_80139C5C` names this function,
and it was still `INCLUDE_RODATA`. Defined in C in `src/ovl/EN_NICHI/80136B10.c` (prototypes in
`include/ovl/EN_NICHI.h`), it makes as1 add an edge from the case-3 inner jump-table `jr` back to the
entry (rule 8 in [[as1-scheduler-rule]]). The natural loop holds the compare chain and the case-3 head
but not the case-1 and case-2 heads, so D1 and D2 take the next compare's `li s0,2` and `li at,3`. This is the second
case of the [[playbook]] recipe "A function named by the file's own data" (after KANGEI
`func_80134724`). The symptom here is different: a compare-chain switch, not a range-check slot. Check
it whenever the original fills a slot from the next compare and the case head's instruction would
pass the path test. Details: `drafts/T-25109/EN_NICHI/func_80136DEC/NOTES.md`.

## T-25201: TACO func_8013EAA4 (closer)

Matched (default C, 1964 bytes, 27/27) from the T-25110 4-row body. Cause of the last rows: uopt's `spilltemps` gives temporaries in bit order (first fit, skipping those of lower bits that share a node). The third grid's `o * 0x100` (bit 196) came before `j * size * 2` (bit 201) and took the particle pointer's sp+0x64, since their regions do not meet. Fix: `half = 0x80;` after the height statement, and `j * half * 2 - half` / `o * half * 2 - half` in the second grid X and third grid Y. Copy propagation then makes the Y item late (bit 215): `j * size * 2` takes sp+0x64 (it stays in s1) and Y gets sp+0x58. `half` takes the old `pad_b` slot. Found with a scratch uopt that prints each `spilltemps` decision (`drafts/T-25110/TACO/func_8013EAA4/spillpatch.py` on top of `tools/uopt_patch.py trace`). Details and controls: `drafts/T-25110/TACO/func_8013EAA4/NOTES.md`.

## T-25147: main func_8005938C

Matched (764 bytes, default C), [[tickets/T-25147-w8-main-80058D20]], wave 9 neighbor conveyor (T-25200). Start: the T-17017 closer draft at 62 rows, all ugen temporaries; T-25007 had called the remaining MATRIX-copy cost unexplained. It is the `tools/ugen_pass.py` block-copy state: a dump of the ucode ugen reads showed all three `Umov` in state B (direct `rot` and `m.t[]` stores before them; uopt turns `p->vz` with `p = &rot` into direct stores), the original is F (one skipped ring slot before each copy's data register). The T-25107 FAKE never-true arm before the loop, `if ((u32)D_800B0896 < 0) { do { *(u8 *)0 = 0; } while ((s32)D_800B0896 < 1); }`, makes the procedure sticky F and closes all 62 rows at once. Notes: `drafts/T-25147/main/func_8005938C/NOTES.md`.

## T-25115: main _sprite_set_light_effect1

Matched (1124 bytes, default C) from the T-24501 body (233 rows) in about 25 minutes. Three causal steps: (1) the black edge is `x * 0 / 5` per colour and per vertex, so r, g and b each have a second use; uopt then reloads them into t1-t3 after the calls, which takes t0-t3 out of ugen's ring for the whole function (233 to 7 rows; the old shared `temp_lo` with an (s8) cast gave one range and a sign extension); `w` declared first owns the unused top slot 0x3C. (2) `(s32)` on the u32 frame counter in `(D + arg8)` restores the load order (FAKE; swapping the operands alone does nothing). (3) One comma statement per vertex in setRGB1/setRGB2 field order makes as1 store b, g, r while r keeps v1 (FAKE layout, 4 to 0). Notes and negatives: `drafts/T-25115/main/_sprite_set_light_effect1/NOTES.md`.

## T-25115: main func_80049A40

Matched (1456 bytes, default C) from the m2c census draft (354 rows in the first hand draft) in about 60 minutes. POLY_G4 box with a 7-way shading switch. Steps: (1) the definition reads full words, so `main_api.h` declares it with `s32` parameters (no caller unit changed). (2) `p` was in `s0`, the original keeps it in `a2` with stack saves around the three calls: the first use of a saved register costs `0.25 * blocks`, and the draft had 21 uopt blocks against the 24 needed. uopt ends a block at a statement boundary after 20 counted variable loads (`varreflimit`). The blocks come from `s32 r, g, b` computed once before the switch (uopt copies them into every arm) and black vertices written `r * 0` (as in `_sprite_set_light_effect1`): cases 6, 5, 4 pass the limit. `gs = &D_800E6280;` before the calls ends a block before `y2`, where the original reloads arg3. (3) Case 3 uses `(u8)r * 8` (FAKE) for the ugen temporary the original spends before each shift. Steps, costs and negatives: `drafts/T-25115/main/func_80049A40/NOTES.md`.

## T-25114: RPG_BAT func_80154DD8

[[tickets/T-25114-w8-RPG_BAT-80154430]], branch w8-RPG_BAT-80154430. Not matched: guarded at 34 funcdiff rows, all aligned, down from T-24910's 102. Three source facts closed most of it:

- **Counters are the global itself.** `D_8015EC9C++; if (D_8015EC68 < D_8015EC9C)` in all three cases. uopt promotes the global `D_8015EC9C` into `$a0` for the call-argument and counter pieces. The `temp = G + 1` locals give a CSE of `G + 1` in `$v0`.
- **Quotients and sums are their own webs** (`$v0`/`$v1`), negated into named `nx`/`ny` (`$a1`/`$a2`, stored at 0x48/0x44). A plain single-use `x = q; nx = -x;` is folded (`mflo a1; negu a1,a1`). An address view on one operand, `*(s32 *)&D_8015EC58.unk_04` (FAKE), keeps the web. Volatile `D_8015EC64` or a pointer local does the same at a frame cost. An indirect read comes first in a commutative op, so the case 3 view goes on the counter (`addu v1,t7,t8`).
- `D_8015EC5C/64/68` as `D_8015EC58.unk_04/0C/10`: same bytes.

Left: frame 0x60 against 0x58 (a web kept by an unmovable read costs frame words; an empty `if (x) {}` keeps it for free but splits blocks), the case 0 `D_8015EBAC` load (first in the original), and the case 3 counter store (before the test loads in the original). Fast scorer `drafts/T-25114/score.py`; details, negatives and the next hypothesis in `drafts/T-25114/RPG_BAT/func_80154DD8/NOTES.md`.

## T-25114: RPG_BAT func_8015528C

[[tickets/T-25114-w8-RPG_BAT-80154430]], branch w8-RPG_BAT-80154430. Matched as default-build C (1500 B, 27/27), from T-24522's 4-line draft. The last two words were commutative `addu` instructions with their operands exchanged. cfe sets the order: `&p[i]`, `p + i` and `i + p` are index first, and a subscript read or store (`IXA`), `p += i` or an integer sum in source order are base first. uopt keeps the order.

- **Progress through subscripts.** `(&D_8015EBAC)[arg6]` everywhere, with no pointer local. The base-first `IXA` address is CSE'd into `$fp`.
- **One phase range with two orders.** The target writes `$s7` base first in the correction arm and index first after the join. Integer address sums, `(s32)&D + arg7 * 4` in the arm and `arg7 * 4 + (s32)&D` at the join, are one uopt expression with two defs, and each def emits its own order. Base-first pointer forms in the arm (`p += i`, subscripts) split it into an arm-only range in `$v0`: 230 lines.

FAKE: the array view of the scalar-declared D_8015EBAC and the integer sums (T-25114); T-24522's `* 1` remainder is still needed. Controls: `drafts/T-25114/RPG_BAT/func_8015528C/NOTES.md`.

## T-25117: VALEN func_80133088

Matched: `func_80133088` (277 words, 1108 bytes) is default-build C. Start: the T-24523 body, cost 26 in this tree (14 before the T-24903 `volatile` on `D_80134544`). Closed by, in order:
- a non-volatile view of `D_80134544` in the 自宅前 test, so as1 hoists its `lui` above the affinity test (26 to 14);
- in the second 校門前 block and the 教室 block, the strcpy call first and the id after it (14 to 0). uopt moves the constant above the call and spills it, which gives the original's `lui a1; li a2; sw a2; addiu a1`. With the id written first, `li a2` stays first in every spelling tried;
- the GYOZI twin `func_801393C8` shape: `D_80134548` itself is the selector, with `switch (D_80134548)`. One dead entry store of it is needed for v0/v1 (37 without). One `GameState *state` is used for every field access (40 with direct accesses). This replaced the T-24523 gotos, union and `(u8)` cast.

Two FAKEs name T-25117: the entry store and the byte view. Notes and negatives: `drafts/T-25117/VALEN/func_80133088/NOTES.md`.

## T-25117: VALEN func_80132834

Matched: `func_80132834` (310 words, 1240 bytes) is default-build C with no FAKE. Start: the T-24523 typed body, cost 336 (342 rows). Closed by, in order:
- a `GameState *state` pointer for the ranking loop (336 to 120). uopt lets later stores change reads through it, so the first two score assignments reload both fields, as in the original;
- sum operand order: comparisons `unk_06 + unk_02`, assignments `unk_02 + unk_06` (to 108);
- declaration order for the frame (to 74). Locals take slots top-down in declaration order at natural size (s16 is 2 bytes, arrays 4-aligned), with about 12 bytes of temporaries below them. Six orders fit this model;
- the random word read as `state->unk_10F8`, so uopt keeps `r` ahead of the rotation loop's guard and does not copy it into the preheader (to 23);
- `count += 1` before `D_8013453C += 1` (to 0).

Negatives and a hint for the GYOZI sibling `func_80138C9C` (same `r` symptom) are in `drafts/T-25117/VALEN/func_80132834/NOTES.md`.

## T-25116: main func_80047560

Matched (352 bytes, default C) from the T-24303 guard (11 differing words) in about 35 minutes. The quotient is narrowed twice: once for the `unk_014[0]` store and once for the shared offset (uopt colours it `v1`; ugen computes it into `t0` and as1 sinks the `move v1,t0` into the last delay slot). The original's ring order (`t0` for the shared narrowing, `t1..t3` for the first store) means ugen emits the shared narrowing first, yet the first store still comes first. uopt forwards a local to its first use, so the guard's narrowing came after the first chain whatever line it was assigned on. A dead store `q = 0;` after the first store redefines the operand, so uopt keeps `off = q;` on its own line: the ring matches, and as1 (`-xbbdbg 8` trace) breaks the aftercycles tie between the two chains by source line, which gives the original's interleaving. A late-folded `+ ((u32)off < 0)` in the first store fixed the ring but left that tie on one line (2 words). A same-type `*(u8 *)&D_801255DC` view in the divisor replaces T-24303's `s8` view, which T-25058 flagged as semantically wrong (the plain read gives 32 words). With that, `s16 off = q`, no `x`/`y` locals, and a `for` loop with `s32 i` all match. Both FAKEs are marked. Controls and negatives: `drafts/T-25116/main/func_80047560/NOTES.md`.

## T-25116: main func_800476C0

Not matched. The function stays `INCLUDE_ASM`. About 95 minutes of work took the best body from 1,191 diff lines (the T-23006 draft, 816 words against 783) to 649 (780 words against 783). The body is `drafts/T-25116/main/func_800476C0/body.c`. Everything is in `drafts/T-25116/main/func_800476C0/NOTES.md`: steps with row counts, negatives, and a 17-second single-function scorer `score.py`.

What closed:

- The record is the global array `((DisplayFixActor44 *)D_8011ECD0)[i]`, not a pointer. uopt keeps field CSEs of a named global across stores to an address-taken local, and kills them for a pointer.
- `tpx`/`tpy` are computed after the tpage switch.
- Rotate is the word `DisplayFixActor44.unk_20`. The draft needs `s32 unk_20` in place of the halfwords `unk_20`/`unk_22` in `include/game.h`; no other user reads them. The edit is not committed (the header stays as the committed C needs it); apply it before running `score.py` (see the notes).
- There is a `parts = anim->part` base and a separate count variable for the `j` loop.
- Both `unk_05` switches have `case 0` labels.
- One index variable serves both animation arms.
- The mirror arms store the scale last.
- In the tail, `seq` is assigned at the join, and a FAKE empty `else {}` gives IDO's PRE a real block. PRE does not split critical edges.
- The record loop is a `for` with `continue` after the `printf`.

What is left:

- The constant 0x1000 sits in fp instead of s4. Traced priority is 600/41 = 14.63 against 520/27 = 19.26 for ox. The halfword scale stores are not uses of the constant range.
- gs gets a split piece in s0 with priority (20 - 10)/5. Assigning gs after the calls removes the piece but breaks the prologue.

The traced uopt was rebuilt in scratch from `wiki/uopt-register-allocation.md` in about 7 minutes. `rd.sh` runs regdiff with it.

## T-25146: ETC func_80144224

Matched, 796 bytes, the default build compiles the C and the ETC sha1 is OK. The step that closed it
reads the frame counter through a pointer local, `GsWord *fc = &D_800E6280.unk_1104;`, with every read
as `fc->u`. With that the counter is one register range (`$a3`, uopt reloads it after each call group,
701 in `$t0`); `D_80150148` is promoted only in the remainder-1 arm (`$v1`, with the original's
write-backs); the phase local `x` stays a real variable in `$v0`, so the original's dead
`or v0,v1,zero` appears. Direct reads (field, `D_800E7384` scalar or the sibling's repeated `% 701U`)
and cast forms of the pointer all let `D_80150148` take `$a3` for the whole function. The arm and the
final print use the sibling's `D_80150150[D_80150148 - 1]`. Two marked FAKEs are the same as the
matched sibling `func_80143A88`'s `sp30`/`sp28`: a word view in the quotient test keeps quot in its
slot 0x30, and word stores keep `v = x * 3` in 0x28 with no extra spill slot (frame 0x38). Levers and
negatives: `drafts/T-25146/ETC/func_80144224/NOTES.md`.

## T-25145: ENDING func_801330E4

Matched (default C, 796 bytes) from the T-24924 92-row body, in about an hour. Three causes. (1) `level` is saved at 0x54: declare the register-allocated `s16 i` after `points`, so `level` sits below it; frame and `chance` stay. (2) The original loads the f9 flag word into `a0`, shifts it into `v0` and copies the `srl` temporary back to `v0` after the sum. That is ugen's destination forwarding ([[tickets/T-25034-main-quadrant-extent]]) with the word coloured on one read, which IDO 5.3 does not do ([[original-compiler]] rule 3); a FAKE empty test on the word gives it a second read and `a0`. (3) The in-place `sltiu/xori` and every ugen temporary of the block and the tail came from `(x > 0) * 2` (not `(x == 0 ^ 1) * 2`) and three statements, `level = f9; level += A; level += B;`. Step 3 was found with a FIFO model of ugen's free list applied to the `cc -S` listing: the target's registers after the clamp need the first sum freed before B's byte. Candidate [[playbook]] recipe: when every ugen temporary of a sum and later blocks is off and one-statement operand orders do not fix it, try the sum as separate `+=` statements. Details and negatives: `drafts/T-25145/ENDING/func_801330E4/NOTES.md`.

## T-25119: main func_80062948

[[tickets/T-25119-w8-main-80061710]] matches `func_80062948` (736 bytes, default C) from plain `INCLUDE_ASM`. What closed it: only `i`, `j` declared above the SDK aggregates (two scalar slots above `rot`, which gives the 0xD0 frame with every original home); `D * (j + 2) * -2` so as1 expands the multiply by -2 with `$at` and spends one ring temporary; the position update before the matrix copy; the copy through the record (`*(MATRIX *)D_801227A0[i].unk4 = m`, as in DATE), which gives the copy walker its own induction variable after the flag walker; and a local copy of the position reloaded after `rsin`. The last one is a front-end rule: cfe orders the operands of a commutative operation heavier first, and an indexed load is heavier than `v / 32`, so the draft emitted `-x / 2` first and the FIFO free list permuted t0..t2. Two FAKE spellings restore the original order where no natural form did: `(D | 0) * (j + 2)` and `(v & 1) - (0 - X) + 2`. The negatives and the scratch method are in `drafts/T-25119/main/func_80062948/NOTES.md`.

## T-25119: main func_80061A3C

[[tickets/T-25119-w8-main-80061710]] matches `func_80061A3C` (1216 bytes, default C) from plain `INCLUDE_ASM`; earlier work had only m2c drafts. A hand-written body (switch on `unk_110D`, one shared counter `i` for every loop, `tbl[8]`, actor 96 through `DisplayActor44`) started at 60 rows. What closed it:

- **Copy loop**: a separate first store and a 12-trip loop `(&D_801230D0)[i + 1] = D_800B5A40[i]` for i = 1..12 (the work-area spelling of func_80062948). The pointer offsets give it away: the source pointer starts on its element, the destination 4 bytes before. A 13-trip loop puts the remainder through the pointers; a separate counter makes uopt unroll the table loop as well.
- **Frame**: `s32 pad[2]` below `tbl` (FAKE) for the 0x88 frame with `tbl` at sp+0x60.
- **v0 after an int call** (34 rows): the copy pointers took v1/a0/v0 for the original v0/v1/a0. A void `func_8006190C` (experiment) gave the original colours, so the return register of the int call is reserved in the rest of the block. `if (func_8006190C()) { }` (FAKE) ends the block; uopt removes the empty test.
- **One shared constant reorders argument setup** (4 rows): `memcpy(p, 0x801F0000, ...)` in case 2 made as1 emit case 1's `lui a1,0x801f` before `li a0,1`; a minimal repro showed it. `(void *)0x801F0000` for the memcpy source fixed it.

Steps, removal controls and negatives: `drafts/T-25119/main/func_80061A3C/NOTES.md`.

## T-25144: RPG_BAT func_8014BFE4

Matched (892 bytes, default C), [[tickets/T-25144-w8-RPG_BAT-8014ADD0]], wave 9 neighbor conveyor (T-25200). Start: the T-17110 hand draft at 128 rows. The function was read through ugen's `cc -S` listing of the function alone (shims of `tools/cc.py`), which shows every real load, the ring order and where uopt places an expression. Levers in the order they cut rows: an empty first arm `if (arg3 < 10) {} else if ...` (its label clears ugen's register cache, so arg1 and arg3 are reloaded in each test and as1 leaves the original's dead `move`s); a record-index variable `j` whose dead init is the original's `move v0,t1`; `slot * 12` in a one-word array `t[1]` (FAKE: it stays in memory at sp+0x50, is reloaded after the loop and is not CSE'd across the calls); five scalars with div fourth so div's home is sp+0x58 (128 -> 35); `div = 1` before the switch, which lowers div's priority and gives the `li a1,1` delay slot; the call index as `k = j + 0x40` at the top of the loop body (strength-reduced into `s2`, so `k + 8` is computed before the first call into `s5`, which pushes div out to `a1` with its spill at 0x58); `j++, i++` in the for increment for the order of the IV increments (35 -> 3); the pre-loop store, `j = t[0]` and the `for` header on one source line (FAKE, as1 line grouping: the record-pointer init moves up behind `lw s4`, 3 -> 0). The idx+8 range came from the strength-reduced index, not from a variable: a plain `k = idx + 8` is always forward-substituted to its use, and an empty test that keeps it early wrecks the allocation. Negatives and the traced-uopt rebuild recipe in `drafts/T-25144/RPG_BAT/func_8014BFE4/NOTES.md`.

## T-25118: main strCallback

[[tickets/T-25118-w8-main-800563F0]]: not matched; the guard (75 rows) is removed and the body is in `drafts/T-25118/main/strCallback/`. The body itself is complete: with the copy-state gadget forced to F before the RECT copy (a private copy of `tools/ugen_pass.py` under `build/`), the unchanged C gives `MATCH` (640 bytes) in `funcdiff --built`. All 75 rows come from the tiny-copy dummy count that T-25029 traced: the shim injects B here (last state op is the direct `D_80125C5C = 0` store after the `func_80088210` call), B leaves 0x10000 in `f_eval_mov`'s stale frame word, and ugen spends two dummy scratch registers instead of the original's one.

- No source route under the current shim: F needs an `Uistr` as the last state op. `*&` and `(&x)[0]` stores fold back to `str`; a `volatile` pointer store keeps `istr` and selects F, but costs a register address and an `addiu` (162 words).
- Census (private, `build/T-25118/diag/`): inverting the state of copies of 32 bytes or less changes 141 matched functions, so the current rule is right for all matched code; strCallback is the only guarded copy function that improves under inversion.
- Candidate rule for a tooling ticket (`drafts/T-25118/main/strCallback/ugen_pass-candidate.diff`): F for a copy of at most 16 bytes whose last state ops are direct stores right after a call. In a private census of all 437 C files it changes no matched function and takes strCallback from 75 rows to 0; the eight other guarded copy functions keep their rows. With a 32-byte limit it changes the four aligned MATRIX copies in ETC/TAIIKU, which the original copies in state B. It rests on one unmatched data point, so confirm it with `lever_lab` (re-insert the guard first) and the 27/27 gate.

Details, negatives and the census commands: `drafts/T-25118/main/strCallback/NOTES.md`.

## T-25118: main func_80056BA8

[[tickets/T-25118-w8-main-800563F0]]: matched (default C, 1296 bytes), from INCLUDE_ASM. Start: the T-17014 draft, 219 rows with the types in headers (DISPENV/DRAWENV now in `psx_types.h`, CdlFILE in a new `libcd.h`). What closed it:

1. Semantic holes: short prototypes for the two volume calls (file overrides), the sample's `id = rectid ? 0 : 1` ternary (keeps `id` in v0), the GsOT draw argument as `(u8 *)&D_800E8C70 + D_8011ECA0 * 0x14` (the `* 5` form shifts every later ring temporary), `start` assigned before the limit: 219 to 94.
2. Allocation by block count: a new saved register costs `0.25 * blocks`. At 75 blocks `&draw` (savings 21) still beats 20.95 and the arg0 piece shares its s3. A `for` vol loop (76 blocks) drops `&draw`; one FAKE empty test in the loop (78 blocks) raises the `&disp` range's denominator so the arg0 pieces are coloured first and fail: 94 to 50 (offsets only). The R9 SAVE/COST lines of regdiff's trace log show savings and costs directly.
3. Frame: `limit` is the hoisted expression `arg1 - 3` (uopt spill home sp+0x48), plus FAKE pads for six unknown words of locals: 50 to 0.

Details, the cost arithmetic and negatives: `drafts/T-25118/main/func_80056BA8/NOTES.md`.

## T-25123: main func_800455C4

Matched (2,768 bytes, default C) from the T-24701 guard (13 differing words) in about 75 minutes. Three source facts closed it:

- **D_801255B5 is a byte of the D_801255B4 object.** In the two `head += 1; D_801255B4 = 2; status |= 1; D_801255B5 += 1;` arms the original issues `lui at; li 2` for the D_801255B4 store before the status load; the draft hoisted the status load first. Writing the last statement as `((u8 *)&D_801255B4)[1] += 1` (FAKE, aggregate unknown) puts the B5 load and the B4 store on one symbol, so as1 orders them and the B4 store chain gains priority. Found with a 4-second toy of the arm in `build/toy` (the whole unit takes 30-50 s under load); line groupings, statement orders, a separate `cd_read_status` symbol, volatile B4 and empty reads all left the order or made it worse.
- **VSync (`func_800AD950`) as an implicit int.** With the `void` prototype the wait-loop counter takes `$v0`; with `s32` it takes `$v1` as in the original. The file now defines `MAIN_API_OVERRIDE_func_800AD950` and calls it undeclared, as `include/ovl/TT.h` does; `func_80046478` in the same file still matches.
- **The counter spill at sp+0x2C is a uopt induction temporary.** A declared counter spills to its own home (0x4C); declaring it after the buffers or in a block scope grows the frame by 8. `var_v1 * 1 != 0x200` in the loop test makes uopt strength-reduce the test into a temporary, which lands in the slot below the locals with the frame unchanged (FAKE). `+ 1`, `- 1`, `<< 0`, `| 0`, pointer and `(u32)` spellings are folded before the induction pass and keep the home.

Measured negatives: s16/u16 counters (extra `andi`/size change), `sp48` or `sp44` reused as the counter (size change), post-increment test (cfe temporary, frame +8), `if (D_801255B4) {}` and similar empty reads in the arm (29-130 rows). Full checksum 27/27 after a clean rebuild; headers, globals and branch protos pass.

## T-25143: SHOUGATU func_8013EA98

No match. The best body is still the T-25046 36-cost source (42 rows, 230/230 words, frame 0x58).
That is over the guard limit, so the source stays `INCLUDE_ASM` and the body is in
`drafts/T-25143/SHOUGATU/func_8013EA98/func_8013EA98.c`. All remaining rows are the
girl-selection loop. A unit-level allocator trace now explains them. The trace is the
`uopt_patch.py trace` binary's `R9SAVE`/`R9UNIT` lines, scripts in the draft folder. Findings:
a block with a computed-address `ilod` (`unk_1BC[F5F]`) is an alias hole; the blocks after a hole
get reload units; the split starts at the forced post-loop store and grows along successors only.
The original needs three things: a latch block shared by all five rejections (as in
`while ((...) && (i++, 1))`, which reproduces the original's final-test layout), the `>= 9` block
outside the selector range (a hole there reproduces that block exactly), and a latch reload that
stays at the block start. With all three as diagnostics, the loop piece reaches priority exactly 0.
An extra reload on the empty fall-through block after the `>= 9` test blocks colouring. Natural
source for the hole and the latch-reload placement is the open question. Negatives, the trace
recipe and the next hypothesis: `drafts/T-25143/SHOUGATU/func_8013EA98/NOTES.md`.

## T-25121: ETC func_80132C3C

[[tickets/T-25121-w8-ETC-80132880]], branch w8-ETC-80132880. Not matched: guarded `NON_MATCHING` body, 15 rows (from T-24012's 105-row draft). Left: four 4-use coordinate constants coloured in another order (draft t0..t3 = 0xA0 u1/u3, 0x128, 0x20, 0xA8; original 0x20 first). Findings that apply elsewhere:

- **uopt splits a basic block after the statement at which it holds 16 indirect stores.** `istr` are counted before dead-store elimination; `str` to locals do not count; the count restarts at calls. Constants first used in a block are loaded at its head, so a split moves a constant's range start (here past a call result in v0). Seven dead stores or one empty `if (p) {}` move the split.
- **Constant-propagated and dead scalars keep frame slots** (three folded coordinate locals grew the frame by 8): a frame larger than the live locals can be folded locals, not arrays.
- **Constant type follows the store, unless uopt folds it.** A literal or a propagated local stored to a `u8` field is an `L` constant, to an `s16` field a `J` constant, and the two never share a register; a `u16`/`s8` view, or a sum uopt folds (`0x18 + i`, i = 0), gives a `J` constant that does.
- **One line per PsyQ macro expansion** (comma expression, x0 first) reproduces as1's in-line store order and the hoisted-constant numbering at once; one statement per store in the target order gets the stores right and the constant order wrong.
- A `volatile` view of a counter global gives one load for a run of byte stores (no copy-propagated second `lw`) and adds in the loaded register.

FAKE (T-25121): `volatile D_8014DB80` in `ovl/ETC.h`, `pad[13]`, `s8` views on u0/u2, the empty block-split test, no `return` plus a dead test. Measurements, negatives and the next hypothesis: `drafts/T-25121/ETC/func_80132C3C/NOTES.md`.

## T-25121: ETC func_801328A4

[[tickets/T-25121-w8-ETC-80132880]]: func_801328A4 (920 bytes) is a literal match in the default build. The T-24012 pointer and byte-offset draft (cost 217) was rewritten in index form over a real `Rgb555Split buf[3][274]`. What closed it: `row + 1` is a uopt temporary spilled below the declared homes (sp+0x6C), not a `next_row` local. The pixel tail keeps both stores (`*dst = (cr & 0x1F) | (cg & 0x3E0) | (cb & 0x7C00); *dst |= 0x8000;`), and the combined channels are locals. A sweep of the 24 operand orders found `up + down + left + right`, which uopt reassociates into the original's load order. Six scalars declared above the buffer and a FAKE `s32 pad[3]` give the 13-word slot count. Rgb555Split channels are now `u32` (srl/multu/divu without casts). Notes and negatives: `drafts/T-25121/ETC/func_801328A4/NOTES.md`.

## T-25141: TACO func_8013FBF0

Matched (1040 bytes, default C), [[tickets/T-25141-w8-TACO-8013F250]], wave 9 neighbor conveyor (T-25200). Start: the T-24502 typed SPRT draft at 84 rows. Four causes, each measured alone. (1) The colour word is `(r << 16) + (g << 8) + b` on u8 channels after `r *= value`: the `andi` in the target is the u8 truncation of the product, and the `& 0xFF` that m2c printed added redundant `and`s that cost ugen ring slots and sent the products to temporaries (`mflo t4` for `mflo s0`); `cc -S` shows it (80 to 51 rows). Candidate [[playbook]] recipe: when a u8 variable's product sits in its own register in the target and the draft has `mflo` into a temporary, drop the explicit masks at the use. (2) The second sprite's y0 has the shift written before the array read; cfe swaps `+` operands by weight, so that order makes ugen read the array first (51 to 4). (3) UV stores of each sprite on one line, as in `func_8013F874` (4 to 0). (4) `s32 pad[6]` for the 0x60 frame (FAKE). Negatives: s32 channels, separate product locals, a `u32 dy` local, lighter shift spellings. Details: `drafts/T-25141/TACO/func_8013FBF0/NOTES.md`.

## T-25142: BUNKA_SD func_80134D94

Not matched; aligned cost 102 -> 12 (243/243 words), but 45 positional rows, so the source is plain
`INCLUDE_ASM` again (the old T-24137 guard is gone) and the body is in
`drafts/T-25142/BUNKA_SD/func_80134D94/func_80134D94.c`. It needs `s32 func_80134D94(void);` in
include/ovl/BUNKA_SD.h (caller and BUNKA_SD sha1 checked with it). What closed rows: the fade colour
is `(0x68E - frame) * 0x10201` done as two in-place `v += v << 8` (the old draft had `* 0x10101`, a
wrong value); an implicit-int function whose last block returns its quotient (`return w;`, the
original's `move v0,v1`); that block written like func_80134850 (u8 views of `unk_110C`, promoted to
`a0`); the switch arms read `t = q * 1` with an empty `if (t < 0)` (a range uopt keeps, `move v0,a0`
at the switch head, like `q * 2` in func_80132708); and the frame counter read through
`GsWord *fc = &D_800E6280.unk_1104;` (an `ilod` is not copy propagated, so the colour stays one
register, as in T-25146). Left: in the first block the original loads the byte counter straight into
`a1` (`lui a1`) while the draft keeps a load range in `v0` (`lui v0`), and the selector needs the
T-25053 empty test to get `a2`, which loads it one slot early; the last block copies w to `v0` after
the store instead of before. Negatives and the next hypothesis: `drafts/T-25142/BUNKA_SD/func_80134D94/NOTES.md`.

## T-25140: main make_color_bar

Matched (1,072 bytes, default C, no FAKE) from the T-24501 walked-pointer draft (186 rows, cost 330) in about 20 minutes. The first divergence was the row pointer: as a declared variable it is range bit 0 and takes `$v0`. The target colours it after the masked gradient values (`$v0`, `$a1`), at `$t0`, so it has to be a uopt induction temporary. Writing every store from the pair index, `p[k + 16 * row].r1 = k * 2` and `.r0 = k * 2 - 1` in `for (k = 0; k < 16; k++)`, lets uopt strength-reduce both values and the address, and use `k * 2 - 1 < 31` as the loop test. That closes all three loops at once. Negatives: indexed `p[k]` with separate `i`/`j` counters (247 rows), and a `Pix2Out *` parameter with no `p = arg0` copy (260 rows). A toy file with only the typedef and the function, compiled with `tools/cc.py`, reproduced the full-unit score in 8 s. Details: `drafts/T-25140/main/make_color_bar/NOTES.md`.

## T-25124: main func_80046AC8

[[tickets/T-25124-w8-main-80046500]] matches `func_80046AC8` (2696 bytes, default C). It is the quadrant renderer from [[tickets/T-25034-main-quadrant-extent]], whose `uv-standard.c` body was the start, at 78 rows. The code from the quad colour block to the texture page already matched. What was left were stack homes and ugen temporaries. What closed it:

- **Spill homes come from `spilltemps` order** (T-25110 rule, traced with a spill-printing uopt built from `spillpatch.py`). Texture u/v go inline in the `func_8009ECB0` call, with no `texture_row` local. `order * 4` is inline. `alpha` is a variable tested as `alpha & 0xF`. The v2/v3 endpoint reads a top `row` local, so copy propagation numbers it after texture v. Nine declared scalars keep frame 0xA8. The last two rows (tile blue at 0x74, not 0x70) closed with a FAKE `if (order) {}` after the tile loop: it keeps the order item in the tile region, which pushes red, green and blue one temporary up.
- **Temporaries that as1 deletes still set ugen numbering** ([[playbook]] section 2). `u8 alpha` gives `sra; and 255; and 15`; as1 deletes the `and 255` and renames the shift to its register. A `(u16)` conversion on red (same expression in both loops, with `red` assigned again in the tile loop), the green and the blue, and `(u8)` on the tile flag supply the other missing or extra slots. Convert the shifted or masked value, not the parameter: `(u16)arg6` loads with `lhu`.
- **The quad loop as `for`** puts `move s6,zero` in place.

Notes, negatives and scripts: `drafts/T-25124/main/func_80046AC8/NOTES.md`.

## T-25120: GYOZI func_8013907C

Matched (844 bytes, default C), [[tickets/T-25120-w8-GYOZI-80138B50]], in about 70 minutes, starting from the T-24707 23-row guard. Three causes. (1) The girl index is read through `GyoziWork *w = &D_800F53A0;`, as in the matched VALEN sibling func_80132D0C. The table value then stays in `$a0` for the store and the call, and the later reads form one promoted `unk_F2F` range over the 3rd load and the switch. (2) That range took `$a0` because its bb5 use is the func_8005EBE0 argument; the traced uopt's R9COST line showed `$a0` at cost 1 against `$v1`. Declaring `u32 func_8005EBE0(s32 arg0);` in main_api.h (before, an implicit int) removes the preference, and the range takes `$v1` as in the original. (3) The dispatch `sll` needs `extern volatile u8 D_80147240;` in GYOZI.h, the as1 lever of [[as1-scheduler-rule]]. It works here although the byte is read twice, because the two reads sit in separate conditional blocks. func_801393C8 needs a non-volatile view at one read to keep its `lui` order. Both are FAKE. Details: `drafts/T-25120/GYOZI/func_8013907C/NOTES.md`.

## T-25120: GYOZI func_80138C9C

Matched (992 bytes, default C), [[tickets/T-25120-w8-GYOZI-80138B50]], in about 80 minutes, starting from the T-24707 75-row guard. Three causes. (1) uopt's global copy propagation put the rotation offset `r = D_800F6468 % count` into the rotation loop and hoisted it after the loop guard; the original computes it before the guard. Propagation stops when an operand is altered in the loop, so the gather, print and rotation stages read the list and the count through one record view `#define CL (*(CandList *)D_80147224)` (count at +0x18, FAKE): the values store then aliases the count. The reorder loop keeps the separate symbols, because there the original keeps the address of the last entry across a store (75 to 12 rows). (2) `s32 func_800BCDF0();` as a per-file override (as in 80140C90.c) moves the offset from `$v0` to `$v1` (4 rows). (3) `bne s4,t6` against the original `bne t6,s4` in `D_80147240 == 1`: tools/cvt_pass.py makes any narrow unsigned global compared with a register constant come out constant first, either through uopt's input swap of a widened `cvt(D) == k` or through `_constant_first` after the widening is stripped. A kept widening on the right of a non-variable left operand gets past both: clear the byte at the entry through the view (`CL.special = 0;`, so it is not an entry location and the `CVT` stays) and compare `r * 1 == D_80147240` with `r = 1;` before the loop (both FAKE). Details and negatives: `drafts/T-25120/GYOZI/func_80138C9C/NOTES.md`.

## T-25126: ENDING func_80135F4C

Matched (default C, 3220 bytes, 27/27), after the T-24047 history (cost 35: the default arm's two `lui` hoisted into the switch head, plus branch offsets). The cause was the T-25150 one: object 80134D20 owns `.data` 8013C5F0-8013CAA0, and its function table `D_8013CA3C` (read by func_80135DD0) names func_80135F4C. With `tools/data_island.py ENDING/80134D20` and the table defined in C, as1 gives the dispatch `jr` an edge back to the entry and keeps the original's `nop` and dispatch `sll`; the function body did not change. Both T-24047 FAKEs (menu-byte address roundtrip, u16 width view) are still needed. Volatile views or declarations of the default-arm cells do not work (base-register addressing, or the hoist moves to the call-argument `lui`s). Lead: func_80134270 (80133C10.c, table D_8013C5BC) and func_8013A2E8 (801396E0.c, table D_8013CB74) are also named by their own object's data and have indirect jumps. Details: `drafts/T-25126/ENDING/func_80135F4C/NOTES.md`.

## T-25201: ETC func_80132C3C (closer)

[[tickets/T-25201-wave-9-closers]], branch w9c-ETC-80132880. Matched (884 bytes, default build, guard removed) from T-25121's 15-row body. The four 4-use coordinate constants had equal priority 1.5 (savings 3 over D = 2) and were coloured in bit order; the original colours 0x20 first. A never-true arm after the XY3 line, `if ((u32)p0 < 0) { p1->code = 0x20; }` (FAKE), adds one use of 0x20 after its first block, so no new reload: savings 4, priority 2, colour t0; ugen drops the arm. Placement rules measured: before the constant's first use the arm needs its own load and a second one on the fall-through path (no gain); a store through p3 raises p3's priority and swaps the saved registers; the arm adds a reaching block to every range it sits in (0x13 and 0xB3 went from D = 2 to 3 and kept v0/a3). Notes: `drafts/T-25121/ETC/func_80132C3C/NOTES.md`.

## T-25122: main func_8004A1A0

[[tickets/T-25122-w8-main-80049FF0]]. No match. The file keeps `INCLUDE_ASM`. Measured with whole-unit `funcdiff --resolve --version c`: T-25010 draft 89 rows, clean draft now 64 rows (157/157 words, no FAKE). Bodies, negatives and scratch scripts (binasm and ucode dumps) are in `drafts/T-25122/main/func_8004A1A0/` (NOTES.md).

- Closed: both loops now match register for register. (1) `tp + col + row * 32` in loop 1 gives `tp + col` the lower bit, which wins the 103.3 priority tie in loop 2 (tp counter `$s3`, index pointer `$s4`, increment order). (2) `if (n <= 0x40) continue; printf(...)` adds one uopt block to `w`'s range (N 21 -> 22, D 6 -> 7, priority 40.33 -> 34.57), so `row * 0x78` takes `$s5` and `w` `$s6`. (3) Header order `unk_00, x, y, w, h` follows the original's ugen temporary ring (x `t2`, y `t3`; as1 sank the y store into the last delay slot).
- Left: header colouring. The original has map `$a0`, 0x80 `$a2`, 0x78 `$t0`, `&D_80122120` `$t1` and 0x1000 `$v1`, the same register as row. Under per-block interference that means the original header is two uopt blocks: the 0x1000 stores before the split, the map def and `row = 0` after it. A late-folded `do {} while ((u32)slot < 0);` before `bg->unk_20 = 0` reproduces every uopt colour and the whole ugen ring. But uopt then emits no `unal` for the map `ixa`, so ugen writes no `.noalias a0,sp`/`a0,v0`, and as1 keeps two slot reloads (159 words) and the header order. Next step: find the split construct after which uopt still emits `unal` (read uopt's emission rule first).

## T-25122: main func_8004A414

Matched (1,024 bytes, default C, [[tickets/T-25122-w8-main-80049FF0]]). It started from the T-24501 `for_loops` body at 142 diff lines, with frame, homes and instruction count already right. What was left were the saved-register colours (y/shade/x/w/h/base), the constant registers (0x60 in `a0`) and the clut argument association. The constants and coordinates are exactly the PsyQ `setXYWH` and `setUVWH(poly, 0xC0, 0x50, 0x10, 0x10)` macros, plus `setRGB0..3`. One comma statement per macro, in macro field order, gave 142 -> 16 lines and fixed every colour and the store order (FAKE: hand expansion, as in other files). The clut needs both `rand()` calls nested and no `random_x` local, and uopt reorders the operands, so the operand order matters: `g->unk_014[i] + (s16)x + (rand() & 8)` and `(s16)y + g->unk_018[i] + (rand() & 8)` (16 -> 0). Lesson: when a POLY packet's constants spell an SDK macro, try the macro expansion before working on priorities. Details and negatives: `drafts/T-25122/main/func_8004A414/NOTES.md`.

## T-25127: ENDING func_8013A2E8

[[tickets/T-25127-w8-ENDING-801396E0]]: func_8013A2E8 (4648 bytes) is a literal match in the default build; 801396E0.c has no INCLUDE_ASM left. The T-23002 guard body (6 rows: the first flag test's load slot kept a `nop` instead of the first string's `lui a1`) matched unchanged once the object's scene table D_8013CB74, which names func_8013A2E8, was defined in C (`tools/data_island.py ENDING/801396E0`). This is the T-25150 mechanism: the function's jump-table `jr` gets an edge back to its entry, so the first flag block lies in a loop region and as1 searches it with the larger candidate budget that T-25056 had shown would match (its scratch threshold 100). Lead: ENDING func_80134270 and func_80135F4C also have a `jr` and are named in their own objects' `.data`. Details: `drafts/T-25127/ENDING/func_8013A2E8/NOTES.md`.

## T-25125: main func_8007132C

Not matched; `INCLUDE_ASM` stays (best body 738 rows, no guard). Start: the T-25052 body at 950 rows. Two semantic
holes closed: `u32 t` for the `unk_0F4.h >> 12` tests (the original uses `sltiu`) and a redundant `b3 >= 7` test in
the rand condition. A `f.b16 = 1` bit-field store fixes the ugen slot at word 161; with the index read through byte
views the first 192 words match exactly. The blocker is the address constant 0x800E6280: uopt colours one split
piece of it (`$a3`), the original never does. Measured from the traced uopt: a constant piece is allocated when
`sum(uses * freq) - sum(freq of reload units)` is positive (loop-first reloads with an in-range predecessor are
free); the draft piece sits at +1 to +5. The original's opcode walk uses a GameState-typed record pointer
`p = base + idx * 4` (fields at +0x69C, index forwarded with `idx++`), which adds uses; taking the schedule reads off
the constant (absolute-address FAKE) leaves it unallocated and gives the walk's structure (825 rows). Left: the walk
colours (op, 0x25, p). Bodies, measurements, negatives and the next hypothesis (the schedule reached through
another aggregate or symbol than the GameState base): `drafts/T-25125/main/func_8007132C/NOTES.md`.

## T-25138: RPG_BAT func_80144C84

Matched (1,276 bytes, default C) from the T-24522 28-row draft in about 25 minutes. Both remaining gaps came from
one ownership fact: the flag words `D_8015EBCC`, `D_8015EBE0` and the counters `D_8015EBFC` are one object.
With direct members of one view (`extern RpgBattleView D_8015EBCC`, a file override of the header's `s32[3]`),
the indexed read `flag[D_8015EBAC]` aliases `EBE0` and `unk_08`, so uopt ends their ranges at the third
threshold and reloads them after the test (as the target does), and as1 keeps the `EBE0` load before the
counter writebacks at the join (one symbol). A pointer view of an incomplete array gave the reloads but
swapped the counter colours; negative offsets from `D_8015EBFC` did not alias `EBE0`. Then `EBE0 -= 0x14`
before the `unk_08` clamp, and the real locals `sel`/`total` for the 0x38 frame. The T-24522 `* 1` wrappers
and `RpgStat *stat` are gone; the comma return stays (FAKE). Details: `drafts/T-25138/RPG_BAT/func_80144C84/NOTES.md`.

## T-25136: TACO func_80145650

Matched (1,908 bytes, default build, 27/27 sha1 OK). The starting point was the T-24502 body at 200 rows. The first divergence was the transfer register of the second matrix copy. Forcing the block-copy state per copy in a scratch `ugen` shim showed the cause: copies 2-4 are in state F in the original and B in the draft, because only stack stores to the input come before them. Forcing BFFF dropped the draft to 92 rows. Writing the four tail sums field first (`arg0[2]->x0 + rsin(...) * len / 3 / 4096`) gave the original's call-first evaluation order. `s32 x, y` restored the 0x88 frame. In place of the forced state, one FAKE: an empty `if (x + y > 0) {}` after the vertex-0 stores. It sets `ugen_pass.py`'s signed-jump flag after an `istr`, so all later copies are F. Testing only x or only y reorders the saved registers. Negatives: pointer stores to the input (uopt makes them direct), the copy before the input stores, a vertex loop (not unrolled). Details: `drafts/T-25136/TACO/func_80145650/NOTES.md`.

## T-25139: KANGEI func_801327DC

Matched in literal default C, 1,096 bytes. Started from the T-24516 body
(`drafts/T-24516/kangei-327dc.c`, 270 rows). What closed it, in order:
(1) two empty truth tests of `D_800E6280.unk_0F4.h` (FAKE). The global is read only through
`>> 12`, so cvt_pass keeps the CVT of every read and uopt saw no plain use. Two plain uses make it
one range coloured `$v1`, the original's `lui v1; lhu v1` at every read. One test is not enough,
any two are (175, 180 to 197, then 0 rows).
(2) the object's `.data` scene table `D_801399B0`, which names this function, defined in C
(`tools/data_island.py KANGEI/80132090`, T-25150 rule 8). Without it, as1 hoists the default arm's
`D_800CA19C` address into the first switch head (29 rows in the control).
(3) bit-field flag tests `unk_0C.f.b2` / `.f.b19`.
(4) `D_800CA148` assigned and incremented directly (`+= 1`, `+= 2`) instead of m2c's locals, so
uopt promotes it into `$v1` (150 to 17 rows).
(5) an `else if (club == 7) ... else` chain, and `(u8)` on the `get_g_zyotai_s(6)` result for one
ring slot (FAKE).
Details and negatives: `drafts/T-25139/KANGEI/func_801327DC/NOTES.md`.

## T-25130: MASTER func_801326B4

[[tickets/T-25130-w8-MASTER-80132140]] matches `func_801326B4` (3264 bytes, default C, about 55 minutes) from the T-24517 body (`drafts/T-24517/later/func_801326B4.c`), 113 rows here and 102 once `func_800847B8` is called as an implicit int. The first divergence was the case 4 switch: uopt copy-propagated the raw `sel` local into the global D_800E6374 and made `shr(D.h, 12)` one PRE range in `$v1` (computed at the switch, inserted after each reload in the call arms), where the original recomputes both shifts in ugen temporaries. What closed it:

- **Different expressions for the two shifts.** The switch is `(u16) D_800E6280.unk_0F4.h >> 0xC` (`shr(cvt(D.h))`) and the join test reads a u32 copy, `mode = D_800E6280.unk_0F4.h; if ((mode >> 0xC) != 3)` (`shr(D.h)` after copy propagation). Nothing is shared, so nothing is PRE'd.
- **Uses in the call arms.** Six always-false tests, `if ((u32) D_800E6280.unk_0F4.h < 0) {}` after each arm call (a FAKE macro), give each arm one use with its reload; the global becomes profitable, gets `$a0` and is reloaded after each call as in the original. ugen folds the tests. Removing any one costs 229 register rows.
- **`s32 func_800847B8();`** under the existing override pattern: a `void` view moves the case 0/1 ranges (39 rows).

Negatives and the full measurement list: `drafts/T-25130/MASTER/func_801326B4/NOTES.md`.

## T-25137: TACO func_8013B460

[[tickets/T-25137-w8-TACO-8013B170]], branch w9-TACO-8013B170 (wave 9 neighbor conveyor). Matched (1328 bytes, default C, guard removed) from the T-24200 guard (cost 4: the `p->y3` store before the UV stores instead of between `lui` and `lw` of the AddPrim argument). Stock `as1 -R -xbbdbg 8` plus the recompiled as1 show the per-block pick order: smallest start time, larger aftercycles, larger latency, smaller hazard address (+48), smaller **source line** (+12), then the front of the ready list (released nodes go to the front, which is why one UV statement comes out v3 first). The y3 store, the UV stores and the `lui` tie on everything but line, so the original's y3 statement had a later line than the AddPrim call. IDO 5.3 gives a statement the line of its first token and gives code moved to a `goto` site the jump's line, so no plain or `goto` layout reaches it. A `#line` pair per quad (the y3 statement on AddPrim's line + 1, numbering restored after it) gives MATCH (FAKE). `funcdiff --version c` and `preprocess_draft` run `gcc -E -P` and drop the markers; the default build keeps them. Negatives and the trace recipe: drafts/T-25137/TACO/func_8013B460/NOTES.md.

## T-11900: GEKO 80134D10.c and OPTION func_801326BC (outside contributor, claim #65)

[[tickets/T-11900-geko-option-closers]]. Four matches: GEKO `func_80136C20`, `func_801367EC`, `func_80135DC0` and OPTION `func_801326BC`.

- **A data table that names the file's functions closes as1 weight misses.** GEKO `D_80145094` (73 scene handlers, indexed by `unk_110A` in `func_801351F8`) defined in C instead of `INCLUDE_RODATA`. as1 then links the jump-table dispatches back to the named function entries ([[as1-scheduler-rule]], T-25150), the switch head gets lighter, and the selector `lui` moves into the `D_80145088` `bnez` slot. This closed `func_80136C20` (4 rows, C unchanged) and `func_801367EC` (the T-24922 44-row draft, unchanged). Any unit whose `.data` names its own functions and still has an unmatched jump-table switch may close the same way. The table names two main functions with no link name from GEKO asm (`func_80083338`, `func_80083474`); they went into `config/symbol_addrs_types.txt`.
- **`func_80135DC0` (2,260 B).** autotweak's GameState-pointer edit took the draft from 297 to 84 rows. Reading the `appointment_find_today` arguments through `D_800E6280.unk_*` instead of `gs` (FAKE) splits the uopt range that otherwise keeps `unk_03E`/`unk_03F` in `$a0`/`$a1` for the whole function (24 rows). strcpy's real return type in this file (`void *func_800AE0F0()`, a `MAIN_API_OVERRIDE_`, as in RPG_BAT) closed case 0. `(gs->unk_71E & 0x10) > 0` gives `sltiu; xori`; FAKE pads fix the frame.
- **OPTION `func_801326BC`.** The palette stores go through a `GameState *g` pointer (uopt treats the pointer store as a possible alias), so `% 4` is recomputed in every iteration and statement as in the original. Flat reads `((s32 *)0x801CBEF0)[k % 4 * 128 + i]` spend one ugen temporary per access. A FAKE empty `else { return; }` stores the promoted `D_8013D308` back on both arms after the compare.
- **Not closed:** GEKO `func_80133024` (2 rows; the T-24926 hypothesis (b) gives 37-41 lines), `func_80132B24` (about 28 rows; return type, bitfield date reads and autotweak gave nothing), `func_80142C9C` (about 40 rows; the `ids` table base kept in a register across the second loop clashes with the post-loop `&ids.v[sel]`; checkpoint in `drafts/T-11900/GEKO/func_80142C9C/body.c`).

## T-25129: GYOZI func_801368B8

Matched (2256 bytes, default C), [[tickets/T-25129-w8-GYOZI-80136220]], in about 65 minutes, from plain INCLUDE_ASM (T-24707 draft: 765 rows). Five causes, in order of rows. (1) One symbol for the display records: every access through `D_80129F40` (words of actor 0x60 through `DisplayFixActor44`, the rest through `DisplayActor44`). The overlapping word/half accesses keep all of that symbol's accesses indirect, which gives the unpromoted actor-0x61/0x62 increments and the folded `unk_2E` reloads; a second symbol (`D_8012B8C0`) for some of them lets uopt promote those (765 -> 222). (2) No `return 0` in a block that uses the record base: case 4 is `if (stop) { ...; D_800F6474++; } else { move } return 0;`. A `return 0` in the stop arm puts the return value in the same block as a base use, and uopt then gives the base `$v1` in every case (222 -> 34). (3) A FAKE dead entry read `s32 pad = D_800F647D + D_80146178;` (also the 0x7C frame slot): tools/cvt_pass.py then drops the CVTs of both u8 globals, so each stays in one register. (4) Case 6 increments before it sets the flag. (5) The name table is `u8 D_8014617C[][5]` indexed `D_8014617C[D_80146178]`: an index load has no CVT, so no copy register, and as1 schedules the global's `lui` above the store as the original does. Notes: drafts/T-25129/GYOZI/func_801368B8/NOTES.md. SHOUGATU func_801350F8 has the same shape.

## T-25128: ETC func_8013FFAC

Not matched, [[tickets/T-25128-w8-ETC-8013FF80]]. The source stays `INCLUDE_ASM`; the best body is `drafts/T-25128/ETC/func_8013FFAC/body.c`: 197 resolved diff rows with an `s32` view of the function, 206 with `void`, from 346 rows for the T-24518 body. What closed: (1) the second address table is the matched `func_8013CCE4` recipe (explicit peel plus a `u32` 2..6 loop, T-24706). It only works with uopt's copy propagation off. cvt_pass.py turns it off for a compare-chain switch on an unsigned global at the entry, and a 15-case jump table has no switch temporary, so a dead two-case `switch (unk_F5F)` at the entry (FAKE) provides one. Then the peel constants stay ugen temporaries shared with the loop body, as in the original. (2) A separate `s32` counter keeps the first loop's `slt`, and a dead `i = 0;` keeps that loop rolled. (3) `((s32 *)D_8014DB90)[unk_F5F * 2]` (the `func_8013CF2C` spelling) restores the missing ugen slot, which lines up every later temporary. (4) `s8` store views in case 8 move `unk_03A` to `$v1`. (5) The `s32` return fixes the case 13 delay slot. Left: the dead switch's entry load of `unk_F5F`, the selector in `$a0` (original `$a1`, held off by the 0x01000000 constant in `$a0`), the case 8 `andi v0,v1,0xff` shade value and the case 9 old/new value split, and a shift/pointer priority tie in the second loop. Negatives and the next hypothesis: `drafts/T-25128/ETC/func_8013FFAC/NOTES.md`.

## T-25134: main palette_load_vram

**Matched (2,024 bytes, literal default C).** Three causal steps from the T-24501 draft (554 exact rows, 32 rows without register names). (1) Index the records as `base->unk_1328[index]`, the way the matched sibling `csr_load_vram` does, and drop the stepped `GameState *g`: uopt then makes the pointer itself (`$t1`, copied from the `$t6` that also stores the memory-resident `base`), giving 22 rows. (2) Compute `y` before `first`: the ugen order t8, t9 (y), then t7 (the shift), giving 18 rows, all stack offsets. (3) Two more stack words below `index` give the 0x78 frame. These are a second loop counter `k` and `u16 height = 1`, both marked FAKE. `GsRec1328.unk_00` is now `u32 *`. Keep the empty `case 1: default: break;`: without `default` the function is 6 words shorter. No saved registers is expected here. For a function with this many blocks, uopt's first-use cost for a callee-saved register is higher than saving around the one call. Details: `drafts/T-25134/main/palette_load_vram/NOTES.md`.

## T-25131: NAME_ENT func_80143580

[[tickets/T-25131-w8-NAME_ENT-80142A80]]. No match; the file keeps `INCLUDE_ASM`. Best body in `drafts/T-25131/NAME_ENT/func_80143580/` (body.c, declarations.h, replay.py, NOTES.md): 1076 of 1077 words, 471 rows (T-20008 archive: 1078 words, 835 rows), Levenshtein score 465 against 811, frame 0x90 and stack homes as in the target.

- Closed (structure): (1) the selector is the promoted global, not a local copy. `switch (step)` on `step = D_800E6280.unk_110D` and `D_800E6280.unk_110D++` in every case; uopt's saves of the promoted byte are the target's `sb t0,D_800E738D` in case 5, so the T-20008 FAKE stores and reloads are gone. (2) Case 5 is `if (spr0.unk_04-- < -0x140)` with the promoted s16 field passed to `func_8004B19C`, no old_x local. (3) Fade steps are chained assignments, `S.unk_10 = S.unk_11 = S.unk_12++` inside the test, and the case-6 reset is `S.unk_10 = S.unk_11 = S.unk_12 = 0` (gives the target's `move t2,zero` stores). (4) No `depth` local in the record loop, so 0x01000000 gets the target's `ra`. (5) Case 7 uses its own u8 local. (6) One counter `i`, declared second after `tiles`, for the case-6 and case-11 loops: frame 0x90, spill at sp+0x64, counter `t0`. (7) Case 9 `c10 = c11 = c12 = 0x80` (autotweak chain kind) shares one `li 0x80` as the target does. (8) Case 16 `girl = (girl & 0xF) + 0x10` without `& 0xFF` gives the target's four-word arm. (9) Record-loop set-up on the `do` line (FAKE layout).
- Left: the selector takes `a2` where the target has `t0` (about 70 words); the lowest-free rule cannot give `t0` because `a2`/`a3` are only live in the case-6 loop, where the selector is dead. Also the spr0 high part, 0x80000000, D4/F8 and their compare colours, the case-6 store order (target increments mid-body and stores at -0x24), the fill-loop constant ranges and the case-11 `slti` test. NOTES.md has the negatives (argument views, prototypes, loop spellings, a diagnostic zcopy-off compile). Lead: an empty test of the not-yet-assigned `sprite` right after the case-6 `load_palette` call (not valid C to keep) turns the selector into `t0` everywhere, 471 -> 445 rows; the next pass should find the real variable whose range reaches case 6 from the entry.

## T-25133: TEL func_8013D88C

[[tickets/T-25133-w8-TEL-8013CCD0]]. Matched (2980 bytes, default C, guard removed); TEL has no `INCLUDE_ASM` left in `8013CCD0.c`. Start: the T-25060 pointer-walk body, 751 against 745 words, cost 786, frame 240 against 224. The frame decided the rewrite: only 8 bytes of declared scalars sit above the four 24-byte arrays and the y/step/flag homes lie below them, so the walks are uopt induction variables. With `s16 x[11], y[11], avail[11], step[11]` indexed by `i` and the call coordinates written as the same expressions as the stores, the frame and length matched; the rest was allocation. Levers in order: row-10 stores on one line (FAKE, as1 order); the month bypass as `if (a || b || c || d) { } else {` (compare order 12, 2, 0, 14 and the 0x74 constant below the loop-three `i * 2` range); the first month read of each ladder through `GameState *g` so uopt does not promote the month into v0 (the F64 PRE temporary takes it); spot flags, page-one and ladder flag bits through `g`, the other flag bits through `D_800E6280` (page one keeps the base in v0 for all five cases). Negatives and the cost of each step: `drafts/T-25133/TEL/func_8013D88C/NOTES.md`.

## T-25303: main strCallback (second pass)

Matched (640 bytes, default C), [[tickets/T-25303-w10-main-800563F0]], from the T-25118 body (75 rows). The first pass had shown the body exact under the F copy-state gadget and concluded that a `tools/ugen_pass.py` rule change was needed, because no spelling of the flag store `D_80125C5C = 0` (the last state op before the RECT copy) gives F. The shim's sticky arm does not depend on that store: the never-true arm `if ((u32)D_800B0896 < 0) { do { *(u8 *)0 = 0; } while ((s32)D_800B0896 < 1); }` at the top of the function (T-25107, T-25147 idiom, marked FAKE) leaves an `istr` and a signed positive-bound loop test in the ucode, ugen drops the arm, and the RECT copy is compiled in state F: 75 rows to MATCH with no other change. Negative: a literal-address flag store `*(s32 *)0x80125C5C = 0` stays an `istr` but ugen bases it on `t8`, not `$at` (one extra word). Notes: `drafts/T-25303/main/strCallback/NOTES.md`.

## T-25132: SHOUGATU func_801350F8

Matched (2308 bytes, default C, guard removed, 27/27), [[tickets/T-25132-w8-SHOUGATU-80134890]], in about 90 minutes from the T-24029 body (656 aligned edits / 672 funcdiff rows). What closed it, in order: (1) the real control flow and returns: cases 1/7/9 `break`, the button tail after the switch, `return 0` in the cases, `return 1` on the two button exits, fall-off at the end; case 4 as `if {..} else {..} return 0;` because a `return 0` in the same uopt block as the last `&D_8011ECD0` base use forces the whole base range off `$v0` (IDO's jump-to-jump retargeting still gives each path its own `b end; move v0,zero`). (2) One declared scalar for the frame slot above the strings. (3) Every record-96..99 global (`D_80120652`..`D_80120734`) written as `((DisplayActor44 *)D_8011ECD0)[n].unk_xx`: with separate symbols as1 reorders the loads around the stores. (4) The timer byte through `GameState *g` (FAKE, one register instead of promoted byte plus widened copy). (5) `sp38.s[(s32)D_80143DD8]`: a u8 index multiplies the raw byte, so uopt keeps the widened selector in a second register and every prologue temporary shifts (487 to 2 rows). (6) The last swapped pair (`lui` of the selector after the first flag store): an empty `if (D_80143DD8 != 0) {}` at the head of case 8 (FAKE) makes ugen load it there. Details and negatives: `drafts/T-25132/SHOUGATU/func_801350F8/NOTES.md`. GYOZI `func_801368B8` is the same scene; try these levers first there.

## T-25304: main mod_trans_vram (second pass)

Matched (752 bytes, default C), [[tickets/T-25304-w10-main-8004C3F0]], from the T-25104 body (47 rows). The range in `t0` that the first pass looked for is not the tile word but the flags byte. Written as the siblings do (`uv = (*data & 0xFF0000) >> 16; bank = (*data & 0xFF000000) >> 24;`), plus `flags = *data & 0xFF;` in a `u8 flags` local passed to `get_fnt`, uopt colours the `*data` CSE `v0`, uv `a2` (precoloured: it is passed itself), bank `v1` and flags `t0` (it lives past the `a0..a3` argument writes). `t0` then leaves ugen's pool for the whole function and the tile word lands in `t0`, as in the original. Declaring `flags` next to the other u8 locals keeps the frame (declared last it moves the p spill slot to 0x70: 10 rows). Same bytes with `s32 col/row/xh` and no casts on the divisions by the u8 ratio (unsigned under -cckr). The for-loop shape of `mod_set_gpu` does not fit here (188 rows); the xh and ypos locals are needed (143/175 rows inlined). Notes and negatives: `drafts/T-25304/main/mod_trans_vram/NOTES.md`.

## T-25302: main func_8004A1A0 (second pass)

[[tickets/T-25302-w10-main-80049FF0]]. Matched (628 bytes, default C), from the T-25122 boundary draft (all uopt colours right, 159/157 words) in about 50 minutes. Two causes, both read from the uopt reconstruction (`base_in_reg` in uoptemit, `findbaseaddr` in uoptkill). (1) uopt writes `unal` (ugen's `.noalias`) for a store only when the store's base address, fixed when the ucode is read, is an `lda`. A load of a pointer local is replaced by its copy only inside the block that defines it, so the first pass's `map = &D_80122120[slot]` above the split left every map store with an unknown base and no `.noalias` (two slot reloads, the header order). Defining it after the split: 4 rows. (2) The split gives the `bg` address CSE a stack temporary, so the cell and index spills sat one word low. The original has four locals (row third): the map stores indexed (`D_80122120[slot].cellw`) with locals `bg, cellh, row, col` match. On top, natural GsBG/GsMAP field order, `(u32)&D_80122120[slot]` and a plain `if (n > 0x40) printf` still match. One FAKE: the empty `if (slot) {}` before `bg->unk_20 = 0` (the split; `do {} while (0)` forms move the 0x1000 load). Notes and negatives: `drafts/T-25302/main/func_8004A1A0/NOTES.md`.

## T-25301: NAME_ENT func_8013AF78 (second pass)

Not matched; [[tickets/T-25301-w10-NAME_ENT-80139740]]. The source now has a `NON_MATCHING` guard at 22 funcdiff rows (11 words, from the T-25153 drafts at 57 and 61). Every register, temporary, the loop and both arms match; only the as1 order of the mask block after the loop differs. With global copy propagation off (`OPTN 405 0`, a scratch variant of `tools/cvt_pass.py`) the guard body matches every instruction. Closed, one cause each: a label between the zero arm's stores and `return 0` (FAKE) puts the return in its own block, so `d` may take `$v0` (uopt forbids `$v0` to a range used in the block that sets the return value); `d` as the loop counter gives the loop's `$v1`/`$v0`; copies `d = D_80120C2B; c = ...; b = ...; a = ...;` in their own block (an empty test ends it, FAKE) give the colour order d, c, b, a while the masks `b &= 0x7F; ...` keep the ugen order b, c, a, d; every `|= 0x80` arm written through its local (`a = (u8)(a | 0x80); ... D_80120BE7 = a;`) gives the original's extra ugen slots and its hoisting; the T-25153 `(u32)` alias cast is no longer needed. Left: IDO 5.3 propagates each copy into its mask, so loads and masks are emitted together and as1 cannot reach the original's load order. This is the copy-preservation family of T-11570/T-16900, here for narrow unsigned globals; a narrow zcopy rule would need approval and a full regression. An 18-row alternative (masks in d, c, b, a order, wrong temporaries), the scratch harness, negatives and the next hypothesis: `drafts/T-25301/NAME_ENT/func_8013AF78/NOTES.md`.

## T-25301: NAME_ENT func_8013AF78 (third pass)

Matched (default C, 296 bytes), [[tickets/T-25301-w10-NAME_ENT-80139740]], from the second-pass 22-row guard; only the declaration changed. `s32 d;` became `register s32 d = D_80120C2B;` (FAKE). cfe declares a `register` local with a `VREG` record. A direct load of an unsigned global stored into a `VREG` before the first branch is what tools/cvt_pass.py reads as a switch temporary, so it turns global copy propagation off for the procedure (`OPTN 405 0`). With propagation off, the guard body was already known to match every instruction. The initializer is dead (the loop sets `d`), so it emits nothing. The route there: a dead two-case `switch` on an unrelated unsigned global fixed the mask block (16 rows), but its temporary took a frame home and the frame grew from 0x30 to 0x38. Narrow types and an undersized `sel` did not free a word. A `register` local adds a `VREG` without a new home. Negatives (`*(u8 *)&D_X` copies, self-stores, the line groupings with propagation off) and the logging shim: `drafts/T-25301/NAME_ENT/func_8013AF78/NOTES.md`. Lever in [[playbook]].

## T-25305: SHOUGATU func_8013EA98 (second pass)

No match, [[tickets/T-25305-w10-SHOUGATU-8013E330]]. The best body is still the T-25046 one (42 rows), which is over the guard limit, so the source stays `INCLUDE_ASM`. Results:

- The natural spelling is closer than the byte-view base in three places. With plain `D_800E6280.unk_F5F` compares, `tools/cvt_pass.py` keeps the CVT, because F5F is first touched after a branch. That reproduces the original's `>= 9` block (forwarded `andi t9,t8,0xff`), the count block with no reload, and the exit (`lui tX; lbu a0,F5F(tX)`). The selector then becomes a CSE temporary with priority 13.3 and takes v0 (the original has t4), and there is no latch: 227 words, 203 rows.
- IDO 5.3 threads an empty latch before register allocation. The latch survives only when it holds code at CFG time. `while (D_800E6280.unk_F5F)` gives the original's final-test layout and a latch that reads F5F, but the latch ends in `bnez` instead of `b`.
- `docreatebb` is off by default, so a reload at a join stays at the block start when a non-member predecessor ends in a conditional jump. The T-25143 concern about the latch reload moving does not apply.
- The empty edge block between the `>= 9` test and the `== 5` label is always in F5F's range. It makes the loop part of the range worth exactly 0. The original's allocation appears only when the range is split starting at the `== 5` block. A FAKE-built diagnostic (hole, latch, alias forced store) reproduces the loop allocation, but the split piece then runs into the post-loop code.

Negatives, trace helpers (`pri.sh`) and the next hypothesis: `drafts/T-25305/SHOUGATU/func_8013EA98/NOTES.md`.

## T-25308: RPG_BAT func_80154DD8 (second pass)

[[tickets/T-25308-w10-RPG_BAT-80154430]], branch w10-RPG_BAT-80154430. Matched as default-build C (1204 B), from the T-25114 guard at 34 funcdiff rows. Two source facts closed it:

- **Only x and y are declared.** With the web-keeping views, a declared `nx = -x` is spilled to a new uopt home below the declared locals, so the `nx`/`ny` slots cost two unused words (frame 0x60). Writing `-x, -y` in both calls lets uopt keep the negations in temporaries across the first call: frame 0x58, homes 0x48/0x44, as in the original.
- **One symbol for the counter, the selector and the test reads.** as1 does not move a load above a store through the same symbol. `(&D_8015EBAC)[60] = 1;` (the counter is element 60 of the int array at `D_8015EBAC`) keeps the case 0 selector load in front of the stores; `(&D_8015EC9C)[-16]` for `D_8015EC5C` keeps the case 3 test loads behind the counter store. Both FAKE until the array is declared in the header.

Controls, negatives and the aggregate follow-up: `drafts/T-25308/RPG_BAT/func_80154DD8/NOTES.md`.

## T-25307: main func_80052F68 (second pass)

[[tickets/T-25307-w10-main-8004F870]]. Matched (1200 bytes, default C) from the T-25101 body (69 rows) in about 70 minutes. The cause was counter ownership, read from the traced uopt with the interference count added to its output (a leaf range is constrained at 22 interferences here). The original uses two counters: `m` for loop 1, the loop-2 inner loop, the loop-3 outer loop and the copy loop, and `i` for the loop-2 outer and loop-3 inner loops; the stress clamp uses `sum`, and both mask arms use one value `v`. With `i` in loop 3, `i` becomes constrained with a priority above the constant 10, so the order is m v0, i v1, 10 a0. With `m` in loops 1 and 3, `val[m]` is one expression, so the strength-reduced val pointer is one range over loops 1, 3 and 4 that interferes with `i` and 10 and takes a2, while it keeps the lower bit that orders its increment first (as1 keeps ugen's increment order). A separate loop-1 counter shared with loop 3 fixes loop 1 but lets uopt turn the loop-2 inner test into `bne` (EQ_INEQ); the `slti` stays only while `m` is also the loop-3 outer counter. The `pv` FAKE of T-25101 is gone; three unused scalars remain as FAKE frame padding. The table bases `D_800B4348`, `D_800B4354` and `D_800B4360` went into `config/symbol_addrs_main.txt`, with their data pieces included after `D_800B41B0`. Notes: `drafts/T-25307/main/func_80052F68/NOTES.md`.

## T-25306: BUNKA_SD func_80134D94 (second pass)

Matched (972 bytes, default C), [[tickets/T-25306-w10-BUNKA_SD-80134540]], from the T-25142 body (45 rows) in about 80 minutes. A dump of ugen's binasm (as1 shim that saves the `.G` file) showed that the first-block `lui v0` belongs to a load-expression range: as1 renames a load's destination when it folds a copy, never the `lui`, so the original loads the counter straight into `a1` and some other range fills `v1` in that block. Three levers closed it. The selector's empty read moved from the first block to the last one, where it costs no load but keeps the selector live beside `w` (`v1`): it still gets `a2`, and its first-block load stays at the switch (45 -> 6). The counter is read through `GameState *g` (not copy propagated, so `lbu a1` directly) and copied once, `t = (u32)b * 1; if (t < 0) {} q = (u32)t / 14;`: the copy is a uopt range in `v1` that as1 folds into the `divu` (6 -> 2). The last block uses a local counter n loaded before the `0x40` store, an extra `if (w) {}` use, and a `#line` pair that gives `return w;` an earlier line than the store, because as1 breaks the store/`move v0,v1` tie by line (2 -> 0; with the promoted-global counter the store-back takes the return's line and `#line` cannot help). Notes, negatives and the binasm dump scripts: `drafts/T-25306/BUNKA_SD/func_80134D94/NOTES.md`.

## T-25311: ENDING func_80134270 (second pass)

Matched (1848 bytes, default C), [[tickets/T-25311-w10-ENDING-80133C10]], from the uncommitted body of the stopped T-25135 agent (5 rows after renaming the absorbed global `D_800E71DF` to `D_800E6280.unk_F5F`) in about 40 minutes. The 5 rows were the GameState `unk_037`/`unk_036` clears at the switch join: the original has `or v0,zero,zero; sb v0; sb v0`, then `sb zero` to `unk_038` in the `jal` slot. The element chain `(&D_800E6280.unk_036)[0] = [1] = [2] = 0` (the T-19030 idiom, FAKE) gives the store order and the shared hidden temporary. A binasm dump showed that as1, not uopt, folds the chain: ugen emits `move v1,zero; andi v0,v1,0xff` and as1 propagates the zero into all three stores. With the function `s32` and no `return` (FAKE, T-11590), `$v0` is live at the exit and as1 keeps it, although a `jal` comes before the exit. Negatives: zcopy off, the object's function table `D_8013C5BC` defined in C (T-25150 back edge), plain chains, `*= 0`, s8 views. Notes: `drafts/T-25311/ENDING/func_80134270/NOTES.md`.

## T-11910: GEKO func_80133C50 (outside contributor, claim #65)

[[tickets/T-11910-geko-remainder]]. Matched (836 bytes) from the T-25002 guard (13 rows).

- **Measure the order, not the ring.** A scratch ugen shim that dumps uopt's output ucode (plus the `cc -S` ugen listing) showed the temporaries were already in the right ring positions. The miss was order: uopt sank `v = h >> 12` to its first use in `v == g`, so the record load (`mul`/`addu`/`srl`) was emitted first.
- **Why the empty test was one instruction long.** `if (v) {}` (T-25002) fixes the order but leaves an `fjp`/`lab` pair in the ucode. The label clears ugen's register cache, so `unk_F5F` is reloaded for the `multu`.
- **Fix.** Read the club through a volatile view, `D_800E6280.unk_0F4.vh >> 12` (FAKE member `volatile u16 vh` in the `GsHalf` union, `include/main_api.h`). uopt keeps a volatile read in place and no label is emitted. It is used in the zero-girl and 1/2/3 branches. A volatile pointer cast in the C file instead builds `lhu v0,244(a1)`, not the original absolute `lui`/`lhu`.
- **`func_80142C9C` (not matched, 119 lines).** A FAKE `if (ids.v) {}` in loop 2 makes the loop match. The ids address is a single uopt candidate covering every use after the `rand` call, so the post-loop `&ids.v[sel]` and `&ids.v[13]` reuse `t4` where the original recomputes `addiu t6,sp,52`. Pointer locals, `s32`/`u32` base locals, offset forms and the `&flags[sel]` difference trick fold back or spill. Details in the draft header.

## T-25309: TAIIKU func_80137784 (second pass)

No match committed; the guard stays at 4 rows (the two pointer compares, constant first instead of pointer first), comment now T-25309. New evidence from the traced uopt (regdiff -v): with the natural compares (`D_8014981C == &D_8014981C[arg0]`, 226 rows) the counter base still takes a free register (a3) at cost 0 when D_80122E94 is ranked ahead of it. So the original's compare constant cannot be the same uopt candidate as the slot base, and no block-count or tie lever helps (the do/while loop removes one block but leaves 0x44 at 0.4). The only form that gives both the operand order and the unallocated base is a second zero-offset name for 0x8014981C and 0x8014984C. Tested end to end and reverted: splat accepts two names at one address with `allow_duplicated:True` on both entries and keeps the D_ names for labels when the aliases come first; with `drafts/T-25309/TAIIKU/func_80137784/alias-match.patch` the default build gives MATCH (1216 bytes) and a clean rebuild gives 27/27. Applying it is a policy decision (second names, like [[keep-lines]]) left for the user. Details and negatives: `drafts/T-25309/TAIIKU/func_80137784/NOTES.md`.

## T-25309: TAIIKU func_80137784 (third pass)

No match; the guard stays at 4 rows with its code unchanged. New negatives: a dead second parameter as the slot pointer keeps frame 56 and gets a0, but IDO stores the incoming a1 and the alias offset still moves to the pointer side (230 rows; with `pos == D_8014984C` the base takes t5, 250). Pointer casts on the natural compare change nothing (240), integer casts make it worse (388). `volatile` slot arrays keep the base in t5 and grow the frame to 64 (292). A late-folded zero index (`[arg0 * 0]`) stays the same lda candidate. cfe 5.3 ignores `#pragma weak` for an extern. No other linker symbol sits at either slot address. That confirms the second pass: only a second zero-offset name (the tested `alias-match.patch`, waiting for approval) gives the original's separate compare constant. A reduced-unit compile (the two includes, the macros and the body) screens a variant in about 13 s under load, against minutes for funcdiff. Details: `drafts/T-25309/TAIIKU/func_80137784/NOTES.md`.

## T-25310: main func_80055AFC (second pass)

Matched (1396 bytes, default C) from the T-25105 113-row draft in about 50 minutes. Three causal steps:
- **Frame.** The spill homes (0x30, 0x2C) sit below five words of declared locals; the draft declared two. `u8 *p` plus a FAKE `s32 pad[2]` give frame 0x48 and both home offsets.
- **Slot read through `g`.** `g = &D_800E6280;` before the `func_80055A38(0)` call, then `arg0 = g->unk_1117 & 1;`. The indirect load keeps `arg0` a copy, so the bank base is one range and the T-25105 volatile view is gone (102 rows left).
- **A dead call result.** The original reads bzero's `v0` after the call: something must hold v0 in the 256-loop preheader and not in the loop body. Then the unrolled loop's start values skip v0, the end pointer takes v0, and the counter and bank-offset temporaries take t3/t4, so the bank bases land in t5 and ra (every other row). `void *bzero(void *p, s32 n);` in `main_api.h` (other callers discard it, no code change) and `p = bzero(g->unk_75E, 0x800);` with p never read (FAKE: its real use is unknown). A function-pointer cast instead gives `jalr t9`.

Notes, negatives and the unroll rule check: `drafts/T-25310/main/func_80055AFC/NOTES.md`.

## T-11910: GEKO func_80132B24 and the func_80133024 ucode finding (outside contributor, claim #65)

[[tickets/T-11910-geko-remainder]]. `func_80132B24` matched (480 bytes) from the T-24302/T-24709 guard (57 funcdiff lines).

- **A local returned with `return x` never gets `$v0` here.** Small tests in this pipeline always give `move v0,vX`. Where the target exits with the value already in `$v0` and no move, write the function as `s32` with no return statement and keep the value live to the exit with an empty test (`if (girl) {}`, FAKE). girl then takes `$v0`, day `$v1` and the constant 12 `t0`, as in the original (57 to 18 lines).
- **cfe picks the operand order of a commutative `+`.** In `bits + days[]` it emits the table load first whatever the source order or casts. A side effect in one operand makes cfe emit that operand first: `(date = date) << 23 >> 27` (FAKE). A new dead local also works but costs 8 bytes of frame.
- **`func_80133024` (guard kept, 2 rows).** uopt-input/output ucode dumps: uopt inserts `cvtl 8` when it copies the day value into the `u8 day` local, giving a field range (`$a1`) and a local range (`$a2`). ugen emits `lbu a1; and a2,a1,0xff`, which as1 folds into `lbu a2,64(a1)`, so the `lui` base is always the field range's register. Reading the field directly gives the target's `lui a2; lbu a2`, but then the compare's `day + i` and the arms' `day + i` become one sum and month falls to `a1`. `i + day`, casts on `i` or `day`, `(i = i)` and `(day = field)` in the compare all give the same ucode. No C form found that splits the sums with a single day range.
## T-25403: GEKO func_80142C9C (wave 11 pass 1)

Not matched, [[tickets/T-25403-w11-GEKO-80142760]]; the source stays `INCLUDE_ASM` (best body 159 rows, over the guard limit). The best body is still the T-11900 checkpoint. The first divergence is the one T-11900 named: the original keeps `&ids` in `$t4` for the second loop only and rebuilds it after the loop. A traced uopt with two more print points (live blocks per range, and each successor that `split()` tests) explains why no spelling reaches that. uopt keys an LDA constant by symbol, so `&ids` is one range with units at the copy, in loop 1, in loop 2 and at both post-loop references. Loop 1 forces splits whose reloads cost 21. Once the loop 2 piece is profitable, it is seeded in the loop 2 body and grows along successors through both loop exits into the post-loop blocks: the heuristic passes, and no exit node is deadout, a call or an `ijp`. With `continue`, the post-loop comes first in graph order, but that piece (2 uses, 1 reload) takes `$v1`. Copies of `&ids` are always propagated, and an alias through another table keeps its offset in the load. Global copy propagation off (the T-25301 `register` FAKE) keeps loop 1 exact (166 rows) and is the lead for the next pass. Measurements, negatives and the trace patch: `drafts/T-25403/GEKO/func_80142C9C/NOTES.md`.

## T-25403: GEKO func_80142C9C (wave 11 pass 2)

Matched (default C, 3,656 bytes), [[tickets/T-25403-w11-GEKO-80142760]], from the pass-1 analysis of the `&ids` constant's split. Four changes, one cause each: the first loop reads `ids` through the `names` table (`((u8 *)&names)[i - 0x40]`, FAKE), so the `&ids` remainder no longer pays 21 in reloads after the loop-1 piece and its priority is positive without an extra use; `if (flags[i] == 0) continue;` in the second loop puts the post-loop block ahead of the loop body in block order; the post-loop compare goes through a second variable (`p = &ids.v[13]; if (q == p)`, FAKE), because with `q == &ids.v[13]` cfe emits the address first and uoptinput moves the `+13` to `q`, which made both post-loop references one constant; with one post-loop use left, that piece is one use minus one reload and stays uncoloured, and the piece seeded in the loop stops at the loop exits and gets `$t4`. Last, `((u8) get_g_zyotai_s(...) & 0x7F) < 2` in switch case 6 (the KANGEI spelling) fixes the ugen temporary after the call. One of the two `if (&flags[i]) {}` FAKEs is no longer needed. The packed-appointment compare needs `D_800E71DF` (2 rows with the GameState field): keep line with a keepproof proof. The variable route (copy propagation off, a pointer local) gives every instruction but not the colour order (178 rows). Lever in [[playbook]]; measurements and negatives in `drafts/T-25403/GEKO/func_80142C9C/NOTES.md`.

