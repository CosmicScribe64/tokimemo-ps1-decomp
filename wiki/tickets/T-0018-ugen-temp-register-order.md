---
id: T-0018
title: ugen temporary register order differs from the original
status: Backlog
assignee:
created: 2026-10-09
updated: 2026-10-10
links: ["[[matching-notes]]", "[[tickets/T-0016-frame-layout-emulation-pass]]"]
---

## Goal

In `LoadSquare`, `StoreSquare` and `MoveSquare` the original loads the u16 parameter homes (`lhu`) into `t8,t9` and `t7,t8,t9`, where IDO 5.3 (with or without the frame pass) uses `t0,t1` and `t8,t9,t0`. Find what produces the original's register order (source form, option, or compiler difference).

## Acceptance criteria

- [ ] Cause identified or documented as a compiler difference; the three functions match or stay `NON_MATCHING` with a written reason.

## Notes

Not a frame issue: stock IDO without the pass gives the same registers. Also open: `func_8004111C` (original stores h and w before x and y), `func_80044700` (original frame has 8 more bytes of locals). `strSync` moved to T-0017 (one delay slot left).

Scope extended 2026-10-09: the T-0750 compare-chain `switch` on a `u8` global (`$v1` in the original, `$v0` from IDO; about 60 overlay functions, plus `func_8004435C`, `func_800443F0`, `func_800443A0`).

## Comments

2026-10-09 (branch t0018-regs), verdict: compiler difference, not a C form and not an option. The original uopt register-promotes a scalar global that is read in several basic blocks of straight-line code: every load of it, including reloads after calls, goes into one register (189 of 195 functions that re-load such a global). IDO 5.3/7.1 only do that inside loops; in straight-line code they use a CSE temporary (`$v0`) and fresh ugen temporaries for later loads. No uopt/ugen/as1 option, `-O` level, per-pass 5.3/7.1 mix or C variant changes it (list in [[matching-notes]], section "Register promotion of globals"). A binasm pass would have to redo uopt's register choice, so it is not a uniform pass under CODING_STANDARDS 7a. `LoadSquare`/`StoreSquare`/`MoveSquare` are unchanged by all of the above. Next experiments: read uopt's promotion priority (ido-decomp) for a single weight that explains the 189 cases; older uopt via [[tickets/T-0100-older-mips-compiler-emulation]]. Moved back to Backlog.

2026-10-09 (T-1321): the global-promotion part is solved for unsigned globals by `tools/cvt_pass.py` (IDO's `CVT J<-L` on unsigned loads and its copy propagation of switch temporaries were the cause; see [[matching-notes]]). `LoadSquare`/`StoreSquare`/`MoveSquare` and the `regorder` rows are not covered (now matched or open in [[tickets/T-3002-remaining-promotion-shapes]]).

2026-10-10 (T-5010): the `$v0`/`$v1` selector split is explained by an entry rule (compiler: a global first touched after a call or branch is not promoted) and a source property (unit-private data is not promoted); `tools/cvt_pass.py` is now in the build with the entry and compare rules. Remaining: R/V promotions of globals first read after a call or branch (no C shape found), `LoadSquare`/`StoreSquare`/`MoveSquare`, the `regorder` rows. See [[tickets/T-5010-t0018-register-order-second-attempt]].
