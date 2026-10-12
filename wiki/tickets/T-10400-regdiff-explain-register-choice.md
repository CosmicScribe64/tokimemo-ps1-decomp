---
id: T-10400
title: regdiff - explain why a draft gets different registers
status: Done
assignee: r13-regdiff
created: 2026-10-12
updated: 2026-10-12
links: ["[[uopt-register-allocation]]", "[[register-allocation-wave7]]", "[[decompile-workflow]]"]
---

## Goal

Matching agents fail on "the original puts this value in another register" because they cannot see why uopt chose a register. Build `tools/regdiff.py <func>`: compile the draft with the instrumented uopt (T-9800), align it with the original asm, map the first register difference to the live range responsible, print its priority against the competing range and a hint from the documented rules. Also fix `tools/sync_protos.py --add-global` dropping `MAIN_API_OVERRIDE_` guards and `--write` reordering unrelated lines.

## Acceptance criteria

- [ ] `tools/regdiff.py` works in an edit loop (runtime stated) with the normal build's flags, cvt and frame passes.
- [ ] Validated on 6 or more real cases and 2 matched controls (no divergence reported).
- [ ] At least 2 near misses fixed with the tool's hints, or an honest report why not.
- [ ] Unit tests for parsing and alignment; sync_protos tests for the guard and minimal-diff bugs.
- [ ] Usage documented in [[decompile-workflow]]; pointers in the register pages.
- [x] Clean build 27/27, headers and globals OK; tests pass; inline review.

## Notes

No compiler binary and no reconstruction source in git. The trace uopt stays in external scratch (T-9800 recipe). Normal build's compiler unchanged.

## Comments

2026-10-12: Created.

2026-10-12: Built `tools/regdiff.py` (+ `tools/test_regdiff.py`, 41 tests). Validated on 10 real drafts (EVENT `func_80119518`, `func_80114B0C`, DATE `func_80136F2C`, TT `func_801475C8`, `func_8014A814`, GYOZI `func_8013E638`, ETC `func_80143F24`, DATE `func_801511B4`, main `MouseState` draft, plus the survey of 163 T-9800 drafts with no failure) and 3 matched controls (EVENT `func_8010AC40`, GYOZI `func_8013AE80`, main `MouseState`: MATCH, no divergence). Fixed with the tool's hints: EVENT `func_80119518` (`u8 sel` copy) and DATE `func_80136F2C` (no temporary); both through ninja with EVENT and DATE SHA1 OK. `tools/sync_protos.py --add-global` fixed with a test. Runtime: traced compile 1.4 to 2 s, whole call about 3 s in a running container, 6 to 12 s from the shell at load average 20 to 55.

2026-10-12: Clean `rm -rf asm build`, Docker configure and ninja: 27/27 SHA1 OK, headers OK, globals OK (1 min 52 s). Progress 5,101/6,958 functions, 911,512/2,279,368 bytes (+2 functions: EVENT `func_80119518`, DATE `func_80136F2C`). Tests: test_regdiff 41, test_sync_protos 70, test_funcdiff 49, test_permute 50, test_check_headers 16, all OK.

2026-10-12: Inline review against CODING_STANDARDS.md. Sections 1/7: the two new C bodies are plain C (a local copy of a selector, a dropped temporary), C89, `/* */` comments with the ticket id; the guarded EVENT `func_80114B0C` draft keeps its `NON_MATCHING` guard and ticket, the default build matches. Section 10: no compiler binary, reconstruction source or game data added; the traced uopt stays in scratch and the tool reads it by path. Section 11: Python 3, Docker only, docstring, paths as arguments, exit status 0/1/2. Section 8a: no header change. Wiki: usage in [[decompile-workflow]], rules and validation in [[uopt-register-allocation]], pointers in [[register-allocation-wave7]], [[matching-notes]], [[data/t0018-cases]]. Findings: none unresolved. Open limits are listed in [[uopt-register-allocation]] (the instruction-to-range link is a strong hint, not proof; TT `func_801475C8`, GYOZI `func_8013E638`, ETC `func_80143F24` explained but not matched).
