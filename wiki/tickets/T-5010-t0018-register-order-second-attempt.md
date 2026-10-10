---
id: T-5010
title: T-0018 register order, second attempt
status: Done
assignee: opus-agent (r3-regs)
created: 2026-10-10
updated: 2026-10-10
links: ["[[tickets/T-0018-ugen-temp-register-order]]", "[[tickets/T-1321-register-promotion-build-step]]", "[[tickets/T-3000-rematch-rv-functions-with-cvt-pass]]", "[[tickets/T-3330-local-fptab-frame-layout]]", "[[matching-notes]]", "[[toolchain]]", "[[original-compiler]]", "[[data/t0018-cases]]"]
---

## Goal

Find what separates the original's `$v0` switches from its `$v1` switches (the reason `tools/cvt_pass.py` stayed out of the build, T-1321), first among source properties (lesson of T-3330), then among compiler rules. Same for the cross-block promotion (R/V) cases. Retune the queue detector with the result.

## Acceptance criteria

- [x] Every observable property compared with scripts over all compare chains of the original (839 chains in 6958 functions) and the 3649 matched C functions: local order and count, local copy, type and signedness, case count, code and calls before the switch, position in the object, file, sharing of the variable.
- [x] Separating rule found and stated: two properties, one compiler rule (entry) and one source property (unit-private data). Details in [[matching-notes]], "Selector register rule (T-5010)".
- [x] Proof on 20+ blocked functions: 44 functions matched (6008 bytes), all needing the pass.
- [x] Pass uniform over the full matched set: with the two new rules it changes 17 of 3649 matched functions, 16 on unit-private data plus ETC `func_80145960`; those 17 are rewritten (16 `FAKE` local copies, one array declaration). The pass is in the build (`tools/cc.py` `SHIMS`).
- [x] Detector retuned (`tools/entry_rule.py`, small hook in `tools/queue.py`): R/V/U1 blocked set 558 -> 264 functions.
- [x] Clean build `rm -rf asm build; configure.py; ninja`: 27 of 27 sha1 OK, headers OK, progress 3438 -> 3482 of 6958.

## Notes

- Verdict: the `$v0`/`$v1` split is not one C idiom. It has two causes.
  1. Compiler rule (entry): the original promotes an unsigned byte or halfword global only when the procedure touches it before its first call, branch or label. Main-exe unsigned globals: compare chains at the entry are `$v1` in 260 of 283; after a call `$v0` in 135 of 155 (`$v1` 9); after a branch `$v0` in 35 of 41. The C (`f(); switch (D)`) is the same in both, so this goes into the pass, not the C.
  2. Source property (unit-private data): a variable that only one overlay or one C file uses stays in `$v0` even at the entry (28 of 30), while shared game state is `$v1` (258 of 270). Unit-private data was most likely defined in the original source file itself (overlay data, or a tentative definition that the linker put in the main executable's bss, like SHUGAKU's `D_800CA2C*` block or EVENT's `D_800B1746`), which the build cannot express because splat owns the data. Where it matters the C switches on a local copy, marked `FAKE`.
- A third, smaller rule: the original removes the CVT only where the widened value is compared or switched on. An assigned, passed or computed value keeps it (GEKO `func_80141D74`, `func_8013BC74`, `func_8013BCBC`).
- Tested and rejected as separators: case count and density, local count and order, a local copy in the original (does not explain the entry statistics), plain `char` (same ucode as `u8`), `static`/defined/initialized globals, struct members and arrays (IDO and the pass treat them like scalars; constant-index arrays go through `ILOD` and already give `$v1` in stock IDO), the first call's target (the 14 call-first `$v0` chains call 9 different functions first, so they are not one copied template).
- R/V (cross-block promotion): at the entry the pass covers it with natural C. For a global first read after a call or branch, no C shape changes IDO's allocation (temporary, `register` local, local copy written back, `do {} while (0)`, an extra block): all give fresh `$t6/$t7` reloads. These stay blocked (R 237, V 86 in the queue).

## Comments

- 2026-10-10: work in worktree r3-regs. Touched files owned by others, kept minimal: `tools/cc.py` (pass in `SHIMS`, `None` removes a shim), `tools/queue.py` (import plus three conditions and the R/V/U docstring), `tools/test_queue.py` (three tests follow the entry rule). Header additions: prototypes in `include/ovl/ETC.h`, `EN_NICHI.h`, `TACO.h`; `D_800E7D14` in `include/main_api.h`; `D_801491E0[4]` replaces `D_801491E0..E3` in `include/ovl/TAIIKU.h`.
- 2026-10-10: inline review against CODING_STANDARDS (7, 7a, 8a, 9, 11, 13). Pass meets 7a: uniform (no per-function control), documented (docstring, [[toolchain]], [[matching-notes]]), evidence-backed (tables above), fails loudly (unchanged `PassError` path), tested (`tools/test_cvt_pass.py` 38 tests incl. real-IDO snippets; `tools/test_entry_rule.py`; `tools/test_queue.py`), replaceable (C stays ordinary). The 16 local copies are marked `FAKE` with the reason; ETC `func_80145960` is marked as an unexplained exception. Ported bodies from the T-1321 scratch C: prototypes moved to the overlay headers (8a), no m2c leftovers, the `(u32)` casts of ETC `func_801460C4` commented. `check_headers` OK. No open findings. Done.
