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

2026-10-18, Sony/SN cc1psx (after Done; user-approved downloads of the mkst/esa `psyq-binaries` assets, run in throwaway wine and dosemu2 containers in the scratchpad, nothing committed):
- PsyQ 3.3, 3.5, 3.6 (DOS, cc1psx = GNU C 2.6.0 SN, ASPSX 2.21/2.34) and 4.0, 4.1, 4.3, 4.5, 4.6 (Win32) all compiled and assembled the ratan2 draft and the synthetic cases. None gives the original: 4.0 is identical to our gcc 2.7.2-psx + maspsx path (96 instructions); 3.x give 96 to 98 with 41 differing words; 4.1 to 4.5 give 95 and 4.6 gives 71.
- 3.3 to 4.5 print the `div rd,rs,rt` macro (ASPSX puts the move after the checks); 4.6 prints `div $0; mflo` with only the zero check. No version prints the `lui rd; addu rd,rd,rs; l* rd,%lo(rd)` load or its own hazard nops. Flag variations on 4.0 (`-O1`, `-O3`, `-mno-gas`, scheduling, `-mips2` and others) change nothing.
- No match, so TAIIKU `func_801488F0` was not re-checked and nothing needs wiring. Details: [[gcc-objects]], "Sony/SN cc1psx (PsyQ 3.3 to 4.6) on the same input (T-11890)".

2026-10-18, issue #63 follow-up (old ASPSX and library signatures; measurement only, nothing committed but wiki text):
- DOS ASPSX 1.05, 1.07, 2.05, 2.08, 2.21, 2.30, 2.34 (the archive has nothing between 2.34 and 2.56) under dosemu2, on the gcc 2.7.2-psx and PsyQ 4.0 `.s` of ratan2 and on the synthetic cases: none gives TT's 97 instructions (96 or 98) and none shows any of the three habits. Pre-2.30 versions also lack the `mflo`/`mult` nops and the `lui; addu` load form the SDK has, so they are out on the SDK's own pattern as well.
- lab313ru signatures (12 libraries x 16 releases) against the 722 main-exe SDK functions: no single release. libgte/libc are 3.3-era, libgpu/libcd/libetc best at 3.70/4.00, libsnd/libspu match badly everywhere. TT ratan2 and libgpu `func_8009E108` differ from 3.70/4.00 only at the habit spots; most other compiled functions also differ in instruction order (a compiler difference). So the game's SDK C code was built with a toolchain none of the released PsyQ sets reproduces, from roughly 3.5 to 4.0 sources. Details: [[gcc-objects]], "Old ASPSX and the PsyQ library signatures".

## Comments

2026-10-18, closed as Done without a match (outcome: evidence and tooling only). `func_8014D260` stays `INCLUDE_ASM`; the open lead is a compiler that emits the expanded divide (see Notes). Review against CODING_STANDARDS.md: the only code is `tools/aspsx_survey.py`, a read-only measurement script (module docstring, prints counts only, no game bytes, runs in Docker); no source, config or build change, no FAKE. No findings. PR from diman8, claim #56.

2026-10-18, proposal on issue #63: keep `func_8014D260` as `INCLUDE_ASM` permanently with the reason in [[gcc-objects]] (PsyQ 3.70/4.00 `RATAN.OBJ` from an unreleased toolchain). Wiki-only PR from diman8 with the cc1psx, old-ASPSX and signature findings.

2026-10-18, root integration of PR #64: preserve the contributor measurements as reported evidence. The permanent assembly exception is not accepted, and an unreleased toolchain remains an inference. This Done ticket records an evidence investigation only, not a C match or exemption; func_8014D260 remains unresolved in the existing completion scope. Documentation review found no code/config changes; no new match claimed.
