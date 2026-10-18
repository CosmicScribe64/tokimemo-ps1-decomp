---
id: T-11890
title: Match TT func_8014D260 (gcc ratan2) through an ASPSX pass on the gcc path
status: Done
assignee: diman8 (claim #56)
created: 2026-10-18
updated: 2026-10-18
links: [[gcc-objects]], [[tickets/T-9200-gcc-built-sdk-objects-in-overlays]], [[toolchain]], tools/cc.py, config/toolchains.txt
---

## Goal

Match TT `func_8014D260` (object `TT/8014D260`, PsyQ gcc `ratan2`), the last TT function. Model the three ASPSX behaviours that maspsx lacks ([[gcc-objects]]): the `div`/`mflo` order, the `mflo` hazard nops, and `lh rd,sym(rs)` expanded through rd. Scope: the gcc/maspsx path for SDK objects only.

## Acceptance criteria

- [x] Each modelled behaviour measured over the ASPSX-built SDK asm of the main exe; rule and exceptions written down (CODING_STANDARDS 7a)
- [ ] ~~Pass is uniform, fails loudly, has synthetic unit tests, and is documented in a module docstring, [[toolchain]] and [[matching-notes]]~~ (dropped: no ASPSX behaviour to model)
- [ ] ~~`TT/8014D260` listed in `config/toolchains.txt`; `func_8014D260` matches~~ (not reached: the habits come from the compiler, not ASPSX)
- [x] Every other function unchanged: clean rebuild 27/27, `TAIIKU/801488F0` still matches
- [x] Code review against CODING_STANDARDS.md

## Notes

2026-10-18, evidence pass (stopped before any rule was built, per the claim's condition):
- Baseline: clean rebuild (asm/ and build/ removed) of `gcc-d260` is 27/27 SHA1 OK.
- New `tools/aspsx_survey.py` measures the three habits over every `asm/lib*.s`; counts and reading in [[gcc-objects]], "ASPSX habits in the SDK asm (T-11890)".
- Habit 2 (`mflo`/`mfhi` nops): uniform, 290 of 290 sites (pad before mult/div, a label, or a gcc `.set noreorder` branch; an ASPSX-filled branch + its nop close the window). Also: no nop between a late move and a reader of rd (16 of 16), where maspsx adds one.
- Habit 3 (`l* rd,sym(rs)` through rd): uniform, 103 of 103 loads with rd != rs; rd == rs (53) and stores (99) use `$at`.
- Habit 1 (move right after `div $zero` when the checks do not read rd): not uniform. 63 of 67 sites fit, but the 4 others (libspu `func_80098D00`, `rem` with rd distinct, late) contradict it, the SDK has no signed-quotient site with rd distinct (the case TT needs), and maspsx's real-ASPSX fixture `aspsx/fixtures/div.yaml` (`div $2,$4,$6`) puts the `mflo` after the checks in every ASPSX version 1.07 to 2.86. In 44 of 67 sites ASPSX has scheduled other instructions into the expansion, so the position looks like an ASPSX scheduler effect. No pass written.
- The C is settled (draft in [[gcc-objects]]): gcc 2.7.2-psx (2.6.3-psx gives the same `cc1` output) + maspsx (aspsx >= 2.50) + the three habits applied by hand in a scratch copy = all 97 instructions identical to the original except relocation fields. Needs `extern s16 D_80159B20[];` in `include/ovl/TT.h`.
- `src/ovl/TT/8014D260.c`, `include/ovl/TT.h` and `config/toolchains.txt` are unchanged.
- Way forward (done below): run the real ASPSX (maspsx `aspsx/README.md`: binaries in a maspsx release archive, dosemu2/wine) on the gcc output of this function and on small cases (`div $4,$2,$5` followed by a dependent / independent instruction / a label / a noreorder branch) to see when the move goes early. That needs a download and new image tools, so it waits for a maintainer decision.

2026-10-18, real ASPSX (user-approved download of the maspsx author's `aspsx-binaries.tar.gz`, run under wine in a throwaway container in the scratchpad; nothing committed):
- Versions 2.56, 2.67, 2.77, 2.79, 2.81, 2.86 behave identically and as maspsx models them. 36 synthetic divide cases (`div`/`divu`/`rem`; rd distinct, = rs, = rt; followed by a reader, an independent instruction, a label, a noreorder branch): the move is always after the checks, with a nop before a reader of rd. `mflo` before a label or a branch gets no nop (only before `mult`). `lh $3,sym($2)` and every other symbol+register load goes through `$at`. The gcc 2.7.2-psx output of the draft assembles to 96 instructions, equal to the maspsx path except relocation fields (original: 97). No accepted ASPSX option changes this (`-0` drops the checks).
- Conclusion: none of the three habits is ASPSX behaviour on gcc's macro output; the SDK and this function were most likely produced by a compiler that emits the expanded divide, the split `%hi`/`%lo` loads and its own hazard nops, which none of our old-gcc builds (2.6.3 to 2.95.2) does. An ASPSX pass would model something ASPSX does not do, so none is built and `TT/8014D260` stays INCLUDE_ASM. Details: [[gcc-objects]], "The real ASPSX on synthetic input (T-11890)".
- Open lead, not tried: a gcc build that prints the divide expanded with checks and splits symbol addresses (a Sony/SN `cc1psx`, or an old-gcc option set), checked against the SDK habits above.

## Comments

2026-10-18, closed as Done without a match (outcome: evidence and tooling only). `func_8014D260` stays `INCLUDE_ASM`; the open lead is a compiler that emits the expanded divide (see Notes). Review against CODING_STANDARDS.md: the only code is `tools/aspsx_survey.py`, a read-only measurement script (module docstring, prints counts only, no game bytes, runs in Docker); no source, config or build change, no FAKE. No findings. PR from diman8, claim #56.
