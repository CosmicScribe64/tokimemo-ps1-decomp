# T-25118: main strCallback (161 words, 640 bytes)

## Start state

`src/main/800563F0.c` (w2-good) held the T-17014 body under `#ifdef NON_MATCHING` with no comment.
`funcdiff strCallback --unit main --resolve --version c`: 75 rows, 160/160 words. From the RECT copy
on, every ugen temporary is one ring slot high: the original's second copy word is `$t1` and the
hoisted `lui` of the else arm `$t2`; the build has `$t2` and `$t3`. Read before starting: T-25001 and
T-25010 (source spelling negatives), T-25029 (`drafts/T-25029/README.md`: the tiny-copy evaluator in
ugen's `f_eval_mov` reads a stale word at its frame slot +232; 0x10000 there gives two dummy scratch
allocations, a small value gives one), T-24067 (the same stale word in ETC `func_80141ABC`, where
the original has two dummies), T-24092 (`tools/ugen_pass.py`, which injects a B or F copy-state
gadget before every `Umov`).

## What was measured

1. **The C body is complete.** A private copy of the toolchain (`build/T-25118/diag/tools/`,
   gitignored, never used by the build) with `DIAG_STATE=F` forces the F gadget before every `Umov`.
   The unchanged body then compiles to the target: official `funcdiff strCallback --unit main
   --resolve --no-build --built build/T-25118/diag/out/sF.o` prints `MATCH (linked function bytes
   equal target (640 bytes))`. Forced B, no gadget at all, and the normal shim all give the same
   75 rows. So all 75 rows come from the copy-state choice before the RECT copy, and none from the C.
2. **Why the shim picks B.** Post-uopt ucode before the copy (records 285-302):
   `cup` (func_80088210), `ldc 0; str D_80125C5C`, `lod D_800B5900; ldc 1; and; str R(v0)`,
   `lab`, `rlda` (rect preloaded in `$a0`), `loc`, `unal`, the source address, `mov 8`. The last
   state op the shim tracks is the direct `str`, so it injects B (`Uldc 0; Ustr`), whose
   `f_loadstore` leaves 0x10000 at slot +232: two dummies.
3. **No source route to F under the current shim.** F needs an `Uistr`/`Uistv` as the last state
   op. `*&D_80125C5C = 0` and `(&D_80125C5C)[0] = 0` fold back to `str` (75 rows, same object).
   `*(volatile s32 *)&D_80125C5C = 0` keeps an `istr` and does select F (one dummy), but ugen then
   computes the address into a register (`lui t8; addiu t8; sw zero,0(t8)`): 162 words, about 143
   rows (private word compare).
   The target's store is the direct `lui at; sw zero,%lo(D_80125C5C)(at)`, so no indirect store
   can produce it. No loop exists for the `uist_rill_loop` route.
4. **Copy-state census of matched code** (`build/T-25118/diag/census*.py`, private): inverting the
   gadget only for copies of 32 bytes or less changes 141 matched functions. Every one of them
   matches with the shim's current choice, so the shim's B/F rule is right for all of them.
   strCallback is the only unmatched guarded copy function that improves under inversion (75 to 0).
   The other guarded copy functions (EN_NICHI 80136DEC, GEKO 80136C20, 80133024, 80133C50,
   80132B24, ENDING 80135F4C, 8013A2E8, TACO 8013EAA4) get worse under inversion; their remaining
   rows are not this mechanism.
5. **Candidate shim rules (private, not applied).**
   - RULE1: a copy of 32 bytes or less whose last state op is a run of direct `str` that came right
     after a `cup` takes F. It fixes strCallback (0 rows), leaves the other guarded copy functions
     unchanged, but changes 6 matched functions: ETC `func_8013FA7C`, `func_801433E4`, GYOZI
     `func_8013A5F4`, TACO `func_8013B2A0`, TAIIKU `func_8013D2B4`, `func_8013FEF0`.
   - RULE2 (fixed form, `ugen_pass-candidate.diff` here): the same, but a `cup` after the stores
     cancels it (fixes TACO `func_8013B2A0` and GYOZI `func_8013A5F4`). With a 32-byte limit
     it still changes the four aligned 32-byte MATRIX copies (`cup`, three `lod; str`, copy) in
     ETC and TAIIKU, which the original copies in state B.
   - RULE2 with a 16-byte limit (the form in `ugen_pass-candidate.diff`): over all 437 C files
     (default build, normal vs RULE2) no matched function changes, no compile fails; strCallback
     goes from 75 rows to 0 (160/160 words); the eight other guarded copy functions above keep
     their rows. This is the candidate lever.

## Result

No match. Guard removed: the body is 75 rows, over the 30-row guard limit, so
`src/main/800563F0.c` is back to plain `INCLUDE_ASM` and the body is `strCallback.c` here.
Rows now: 75 (normal pipeline), 0 with the F state or with the RULE2 candidate shim.

Reproduce: `diag/README.md` here (scripts and scratch-shim patches; the scratch trees under
`build/T-25118/` do not survive a clean rebuild).

## First divergence left

Row 25 (`lui t2` vs `lui t3`, else-arm address hoisted into the copy) and row 27 (second copy word
`lwl t1` vs `lwl t2`): ugen's tiny-copy path spends two dummy scratch registers instead of one,
because the B gadget the shim injects before this `Umov` leaves 0x10000 in `f_eval_mov`'s stale
frame word. The original compiler had a small value there. That is the copy-state model, not
the source.

## Negatives added (do not repeat)

- `*&D_80125C5C = 0;`, `(&D_80125C5C)[0] = 0;`: fold to `str`, 75 rows.
- `*(volatile s32 *)&D_80125C5C = 0;`: F state, but extra `addiu` and register address, about 143 rows.
- Any further C spelling of the copy, the flag test or the reload: the body is already exact under
  F, so a source change can only add a FAKE that moves the ring (for example a ring-wrap identity
  chain before the copy, which would still leave the source pointer register 6 rows off).

## Next hypothesis

This needs a tooling ticket, not a source pass. Refine `tools/ugen_pass.py` so that this copy gets
the F gadget and no matched function changes. Candidate: RULE2 with a 16-byte limit
(`ugen_pass-candidate.diff`), which changes no matched function in the private census. Check it with `tools/lever_lab.py` (re-insert the
guard from `strCallback.c` first so lever_lab re-scores it), then the 27/27 gate. The
distinguishing features of this copy against the matched B-state copies are: 8-byte unaligned
(16-bit alignment) source, destination preloaded in a register (`rlda`), and a constant-zero direct
store after a call, followed by a register reload and a join label before the copy. Only one
unmatched data point supports the rule, so name it as a model of the original's stale word, not
as a proven mechanism.
