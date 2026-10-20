---
type: playbook
updated: 2026-10-20
---
# Matching playbook

Read this first. It holds the recipes that closed matches; the long pages are reference. The loop: m2c draft -> fix types and control flow (`funcdiff` shows structure rows) -> `viewsweep` -> `autotweak` / `order_search` / `permute` -> `regdiff` for register rows -> `funcdiff --resolve` until `MATCH`, then the full `ninja` sha1. All tools run in Docker:

```
tools/docker.sh python3 tools/funcdiff.py --resolve UNIT:func_XXXXXXXX
tools/docker.sh python3 tools/viewsweep.py UNIT:func_XXXXXXXX            # callee/return-type views, run first
TOKIMEMO_SCRATCH=<scratch> tools/docker.sh python3 tools/regdiff.py UNIT:func_XXXXXXXX   # read FIRST DIVERGENCE only
tools/docker.sh python3 tools/autotweak.py UNIT:func_XXXXXXXX --time 300
tools/docker.sh python3 tools/order_search.py func_XXXXXXXX --unit UNIT -j 8
tools/docker.sh python3 tools/permute.py func_XXXXXXXX --time 120
```

Guard rules: readable C that does not match stays under `#ifdef NON_MATCHING` only when it is under about 30 rows, with a ticket id and what is left in the comment; every FAKE comment names the ticket and the reason; no inline asm. If a variant does not compile, `funcdiff` prints no diff lines: check for `MATCH` or `DIFF`, do not count zero rows. References: [[register-cheatsheet]] (symptom table), [[decompile-workflow]] (tools in full), [[notes/functions]] (past notes per function), [[build-step-gaps]] (what the compiler cannot reproduce), [[cloud-matching]] (single-function scratch setup).

Model of the compiler: uopt colours registers (v0, v1, a0-a3, t0-t5, s0-s7 in that order, by priority, then first mention). ugen hands out `t6..t9, t0..t5` as a ring for expression temporaries. as1 deletes redundant ops (`and 0xFF`, `sll/sra` pairs, copies) but those ops still spent ring slots, then schedules delay slots per source line.

## 1. Every register one off, instructions equal

**Type of the function and of callees.** Symptom: an `or v0,...` or `li v0` the draft lacks, wrong delay-slot filler, a promoted counter in `$v1` instead of `$v0`. Fix: run `viewsweep`; typical result is `s32 f()` with no `return` (v0 live at exit) or `s32 callee();` as a view in the header guard. It closed 19 of 102 wave-3 blockers; example `s32 func_8013E810();` took RPG_BAT `func_80138324` from 158 to 4 rows (T-24303). Caveat: re-check the callers (full-unit sha1), and check `funcdiff --proto` first to try a prototype without editing.

**Return type changes the delay slot.** Symptom: mode-address `lui` or loads in branch slots where the original has `nop`. Fix: make the shared declaration and definition `s32`, K&R. Caveat: the working explanation is return-register liveness, not an allocator rule; the void control differs.

## 2. A temporary one register too low or too high (ugen ring)

**Ops that as1 deletes still cost a slot.** Symptom: every `t` register one lower than the original from one block on, no instruction missing. Fix: add a no-op that ugen numbers: `idx = sp47 & 0xFF;` on a `u8` out-parameter (main `func_80054884`, 17 rows to 0, T-24214); `((f() == 1) != 0) && ...` on a call test (ETC `func_80140AE4`, 16 rows, T-24207); a third `|=` on a promoted narrow global (MASTER `func_80132444`, T-24208: each narrow `|=` spends two slots, so the count of ORs before the copies sets the numbering). Mark FAKE. Caveat: the right count is found by trying 1, 2, 3; the permuter finds these disc-free.

**Parameter width.** `t7` vs `t8` throughout: try `u16`/`s16`/`s32` for the parameter, or a K&R `u8` parameter for a selector (main `func_8007B144`, T-17012 family). Also split `v = x >> 3; v += (x >> 3) * 80 / 48;` when an s16 narrowing must take the temporary (T-19008).

**Colour bytes.** `p->r = (c >> 16) & 0xFF;` keeps the shift in a ugen temporary (the `and` is dropped before `sb`); plain shifts or `u8` locals become uopt ranges (T-16220). Put the three stores of one vertex on one source line (T-17203, DATE `func_8014EF1C`, 143 rows to 0).

## 3. Value in a register vs reloaded; promoted ranges

**Narrow global promoted by a byte view.** Symptom: the original has `andi v0,..; or a0,v0,zero; jal; sb v0` where the draft forwards `sb v1`. Fix: store through a byte view and re-read the plain field, `((u8 *)&D_800E6280)[0x1118] = slot; f(D_800E6280.unk_1118)`; a second view re-store in a later arm may be needed (ETC `func_80144DF0`, 47 to 0 rows, T-24209). The reverse also works: plain stores, view re-reads (T-24206). Caveat: view store plus view re-read puts the value in `$a0` with no `move`.

**Original reloads, draft CSEs.** Fix: read the second use through an address view, `*(u8 *)((u8 *)&D_800E6280 + 0x3E)`, or through a `GameState *gs` pointer (uopt treats a pointer store as a possible alias). In main `func_8006D7EC` the original mixes `gs->` and absolute accesses; the `gs->` form for record stores recomputes `sll; addu` per statement as the original does (T-23003).

**Delete a redundant local.** If regdiff lists a shift or sum as its own range next to your local, remove the local and repeat the expression (TAIIKU `func_8013B200`, 79 to 43 rows, T-19008). If a global is written back at every loop latch, drop the local array base and index the global (main `func_8006612C`, T-17011).

**Global as the variable.** A local held in the wrong register and stored to a global in each branch: assign the global itself and read it back; uopt promotes it and colours as the original (T-10860 family).

**Split a range with an empty test.** `if (x) {}` after an assignment splits or reorders live ranges and adds no code (GEKO `func_801358CC`, found by autotweak, T-24302). Unassigned locals grow the frame by 8. Always FAKE.

## 4. Operand order of compares and sums

`bne` operands reversed, registers right: read the selector through a u32/s32 local (GYOZI `func_8014206C`, T-20005 note), or the aggregate (T-16930), or wrap the left operand `w * 1 != x->width` (`| 0`, `^ 0`, `<< 0` also work; `+ 0` does not) (main `strNext`, T-17014). For sums, spelling decides: `(idx + D_X)->f` gives index first, `(D_X + idx)->f` base first (TACO `func_801557E8`, T-16564); in RPG_BAT `func_80138324` the last 4 rows were the global first in two sums (T-24303).

## 5. Store order, delay slots and as1 line grouping

as1 schedules per source line. Put initial assignments on one line (`n = 0; b = 0; p = A; r = B;`) to keep ugen's order (main `func_80055A38`, T-17007); the loop init on the `do` line, the body on the `for` line (T-11650); two constant stores on one line in the constant numbering order (T-16554). Write `c++` before the record stores if the original loads early (T-23003). One statement per line is the default; the exceptions are the FAKE layout recipes above, marked in the source. `order_search` permutes lines but never joins them.

Hoists you cannot stop: as1 moves the head of a single-predecessor block into the predecessor's slot. A reference from the jump table (`case 10:` before `default:`) stopped it in VALEN `func_80134030` but changed the table size (T-24211). Where the unwanted pick comes from the global scheduler (VALEN `func_80132D0C`, T-24212; KANGEI `func_8013852C`, T-24202) the rows stay; see section 12.

## 6. Constants: folded vs kept in a register

One `li k` stored to several places: chain through elements, `a[0] = b[0] = k;` or `(&G)[0] = (&G)[1] = (&G)[2] = k;` (shares the `li`; the hidden chain temporary can add 4 bytes to the frame, a declared local can absorb it). `--(&G)[2]` as the chain source keeps the raw `addiu` and gives `andi` for the byte stores (TACO `func_8014F820`, T-24213). A constant stored to a global and passed as an argument: store plain fields, then pass them re-read through a view (FAKE). A constant argument scheduled too late: reuse a word local, `render_x = (render_x & 0) - 40` (T-24021). If uopt folds `idx = 0; idx++` into `li 1` and the original has `addiu`, only a live backedge keeps the `addiu` (T-24204, unresolved, 1 row).

## 7. Stack frame size and offsets

Every declared scalar owns a 4-byte slot, promoted or not; arrays sit lowest; scalars declared before an array go above it; spilled locals take slots top-down in declaration order. Fixes: declare the real scalars before tables (GEKO `func_80132B24`, only three scalars, T-24302); add `s32 pad[n]` or `u8 pad[4]` and mark FAKE (ETC `func_80144DF0` needed `pad[3]`, T-24209; TAIIKU `func_8014280C`, T-16554); remove expression-temp locals (`tbl`, `rec`) in big functions; `A[f()] = v` reserves a spare slot, so use `idx = f(); A[idx] = v;` (T-16554). Overlay and main frames are handled by `frame_pass`; write C for a normal frame. `order_search` with `/* ORDER */` around declarations finds the declaration order (T-16230). Turn a pad slot into the real local the source likely had when ring numbers also differ.

## 8. Loads re-read vs CSE'd; struct aggregates

Several close `D_` names in one function are usually one struct or array. Write accesses through one base (`D_X[i]`, `(&D_X)[k]`); consecutive scalar globals with a store, then the next load, are one struct declared at the lowest address (T-10610 family). The opposite case: in main 80075320 `u8 D_800B6990[8]` plus `u8 D_800B6998` shared `lui $at` correctly while a single `KeyRing` struct did not (`func_800755B4`, T-24303). `viewsweep` and `data_island.py` before guessing.

## 9. Jump tables and switch spelling

If the table starts at 0 or below the lowest case, add the empty labels the table has: `case 0: break;` (TACO `func_80143B58`, T-16562), a last empty `case 7: break;` (T-11640), `case 0xE: case 0x46: break;` (ETC `func_8013B1F0`, T-22026: without them IDO built a compare chain 24 bytes longer). Shared labels: write `case 8: case 9: default:` as the table has them. Rows that are all branch offsets and `t` registers shifted by a constant: compare each switch's setup (`addiu x,-lo; sltiu x,N`) first. A selector with an extra `cvt` or copy: switch on `*(u8 *)&D_X` or `((u8 *)&D_X)[0]` (T-10860).

## 10. Block copies

For copy mismatches, distinguish direction state from temporary-register choices. The historical direction mismatch is handled by the enabled T-24092 `ugen_pass.py`; see [[build-step-gaps]]. Source recipes still matter: copy the sibling's declaration order for record-drawing frames; write `Rec *p` with index arrays instead of pointer walks (main `menu_girl_taku_set`, matched once the m2c walks were replaced, T-24303). A remaining register mismatch alone does not prove that no C source can match.

## 11. Return value, implicit int and narrowing

Callee returns 16 bits: declare `u16 f();` and make the loop variable one `u16 r` that is also the argument (main T-17019 family). Narrow the second result with a `u8 f();` view and `s32 res` to get `jal; or a0,fp,zero; beqz v0; or a0,v0,zero` (main `func_8006D7EC`, T-23003; FAKE inside the guard). A call without a prototype in the original is an implicit int: do not declare it unless `funcdiff --proto` shows a gain (T-17018: `get_now_k`). Wrong signedness of a header field causes `lb` for `lbu` across the whole function; fix the field in `main_api.h` (T-17018, `xa_wait`).

## 12. Byte views and bit-fields

Flag byte `|=`: use a bit-field store `((Bits *)&x)->f = 1;` when one temporary is missing (T-11650). A 3-bit field decremented in a byte is `f -= 1` on a bit-field typedef, not shifts (T-23003, 561 to 482 rows). Repeated `|=` through a byte view spends ring slots (T-24302: one OR gives `t4`, two give `t8`, three `t7`). A u16 flag read after a `+=`: test through an `s16` view `((*(s16 *)&D) & 0xFF00)`, not a cast (main `func_8007A50C`, T-17012).

## Known compiler limits

Do not spend a session on these without a new idea; leave a guard with the ticket id and what remains. Details and the census in [[build-step-gaps]].
- Historical ugen block-copy direction mismatch (T-19001, T-22014): addressed by the enabled `tools/ugen_pass.py` in [[tickets/T-24092-ugen-block-copy-state-pass]]. Reproduce remaining failures with the current pipeline; 32-byte unrolled-copy register choices are a separate question.
- as1 global scheduler picks and default-block hoists (T-24202, T-24211, T-24212): same family, no source form found.
- uopt `idx = 0; idx++` fold (T-24204): proves the zero through every source form tried; only a live backedge keeps the `addiu`.
- uopt priority and colour order with no uniform rule: apply recipes above, do not search flags (exhausted in T-0014, T-0017, T-9800).
