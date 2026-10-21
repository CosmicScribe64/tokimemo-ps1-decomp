---
id: T-24800
title: "Test per-file compiler options against the as1 order near misses"
status: Done
assignee: as1-flags agent
created: 2026-10-20
updated: 2026-10-20
links: ["[[as1-scheduling]]", "[[tickets/T-20620-as1-scheduling]]", "[[tickets/T-20700-ido41-as1-test]]", "[[build-step-gaps]]"]
---

## Goal

Find out whether the original makefile compiled some objects with other options, which would explain the eight
guarded near misses that differ only in as1 scheduling: KANGEI `func_8013852C`, VALEN `func_80132D0C` and
`func_80134030`, GYOZI `func_80139E4C`, GEKO `func_80136C20`, ENDING `func_8013A2E8`, TACO `func_8013B460` and
`func_801511C0`.

## Acceptance criteria

- [x] Harness in `tools/` (CC0, docstring) that compiles one file under flag variants and counts its matching functions.
- [x] as1-level and driver-level variants and combinations run on the 8 files.
- [x] [[as1-scheduling]] with usage, the variant table and a verdict.
- [x] Any working per-file flag implemented through `config/toolchains.txt` and `tools/cc.py`, with the function matched.
- [x] Clean rebuild 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch`, tool tests.

## Notes

Worktree `tokimemo-wt/as1-flags`, branch `as1-flags` (from `w2-good`).

## Comments

2026-10-20 (agent): verdict, details in [[as1-scheduling]].
- Per-file options do not explain the family. Only as1's `-noxbb` (`-Wb,-noxbb`) fixes any of the eight.
- VALEN `func_80134030` is matched with `VALEN/80133C70 ido 5.3 -Wb,-noxbb`. Every function of the file matches,
  VALEN.bin sha1 is OK, and funcdiff `--resolve` reports MATCH for `func_80134030` and `func_801342E8`. This is an
  option change for one file, so it is marked FAKE (CODING_STANDARDS 7a). `-noxbb` changes no other function of
  the file, so the file gives no positive evidence for the option.
- GYOZI `func_80139E4C` matches with `-noxbb`, but `func_801399C0` of the same file breaks (the original hoists
  across blocks there). So the original scheduled per function, not per file.
- Census: `-noxbb` changes 2,450 of 6,814 unguarded C functions, in 384 of 435 IDO files.
- Tool changes: `tools/as1_flag_sweep.py` (new); per-object IDO options in `configure.py` and `tools/cc.py`
  (`toolchain_of`, `toolchain_of_source`); `funcdiff_verify`, `order_search` and `permute` compile with the
  object's toolchain; tests in `tools/test_cc.py` and `tools/test_configure_toolchains.py`.

- 2026-10-20 orchestrator: the VALEN/80133C70 `-Wb,-noxbb` line and the func_80134030 hunk were dropped before merge: an option change with no positive evidence is a fakematch (CODING_STANDARDS 7a), and T-24900 matches the function in C with a volatile declaration. The per-object option support and the harness stay.
- 2026-10-20 review (orchestrator, CODING_STANDARDS.md): harness and per-object IDO option support with tests (test_cc, test_configure_toolchains, test_order_search, test_frame_pass OK; test_funcdiff_verify and test_permute fail the same way on main before this change); the VALEN -noxbb option change was dropped as a fakematch. Merged with a clean 27/27 build and header/global/prototype checks. Done.
