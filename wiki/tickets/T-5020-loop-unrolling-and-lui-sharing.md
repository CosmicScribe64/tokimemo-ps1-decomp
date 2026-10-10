---
id: T-5020
title: Loop unrolling and lui sharing
status: Done
assignee: r3-consts
created: 2026-10-10
updated: 2026-10-10
links: ["[[matching-notes]]", "[[decompile-workflow]]", "[[toolchain]]", "[[data/shared-at-groups]]", "[[tickets/T-3001-shared-constant-registers]]"]
---

## Goal

Decide whether two recurring wave-2/3 blockers are source or toolchain differences, and fix them with a C idiom or a uniform pass (CODING_STANDARDS 7a):
- (b) the original shares one `lui $at` across stores to neighbouring globals (`k_reset`, `k_disp_start`, TAIIKU `func_8013A63C`, the RPG_BAT `D_8015EDB0` counters); IDO re-emits it;
- (c) IDO unrolls loops the original does not, or with another load rotation (main `func_800415B4`, ETC `func_80143240`, OPTION `func_80134210`/`801385F0`/`80137FA0`).

## Acceptance criteria

- [x] Verdict for each, with evidence over the whole corpus.
- [x] Idioms documented in [[matching-notes]] and [[decompile-workflow]] and proven on blocked functions; any flag or pass change verified on the full matched set (none adopted).

## Notes

Results in [[matching-notes]], "Shared constants, shared `lui $at` and loop unrolling (T-3001, T-5020)".
- (b) Toolchain: IDO 5.3's as1 reuses `$at` only for the same symbol and offset. The original shares across offsets in 171 groups (74 functions) and re-emits in 19026 places; per original object no address pair is both (320 shared pairs, 10669 re-emitted, 0 conflicts), so the original keyed the reuse on the symbol and the source named the shared groups through one symbol. Groups: [[data/shared-at-groups]] (handed to the type-recovery work). No pass: a symbol-keyed rule changes 135 matched functions that reach neighbours through one base symbol (there for as1's load hoisting); both as1 rules would have to be modelled together.
- (c) Mostly source. Flags tested by full rebuild: `-Wo,-unrolllimit,N` changes 8/8/6/4/0/0 matched functions at N = 60/80/100/150/200/250, `-Wo,-nomultibbunroll` changes 8; neither stops the blocked loops. Idioms: struct-array element fields for a big body (main `func_800415B4`, matched), `s32` counter against a `u32` bound (TACO `func_80147400`, matched), pointer loop to `start + N` (loop shape of `func_800673B8`, NAME_ENT `func_801478E0`, BUNKA_SD `func_8013987C`, but the end pointer differs). Toolchain part: the original has no run-time remainder unroll of a pointer loop anywhere (IDO's `subu; andi mask` prologue: 0 of 6958 functions), no IDO option turns only that off. OPTION `func_80132624` (unrolled by 2 at the budget the matched set needs) is open.
- Matched: main `func_800415B4` (152 bytes), TACO `func_80147400` (68 bytes). Fewer than the 10 per pattern the brief asked for: the other listed loop functions fail on an end pointer, a store elimination or register order once the loop shape is right.

## Comments

- 2026-10-10 review (inline, CODING_STANDARDS checklist): C89, fixed-width types, struct offsets documented with `unk_XX` names, comments `/* */`, no FAKE needed (both idioms are ordinary C: a struct view of the table, a signed counter), new extern `D_8015F290` in `include/ovl/TACO.h` (overlay symbol, 8a), `SprEnt` is local to `src/main/80041000.c` (one user; the type-recovery work may move it to `game.h`). Clean build 27/27 OK. No findings open.
