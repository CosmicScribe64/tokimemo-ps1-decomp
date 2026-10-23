---
id: T-25201
title: Wave 9 closers for wave 8 near misses
status: In Progress
assignee: wave-9 conveyor
created: 2026-10-23
updated: 2026-10-23
links: [[[tickets/T-25200-wave-9-neighbor]], [[tickets/T-25100-wave-8-finish]], [[playbook]]]
---

## Goal

One fresh-eyes Opus pass on each guarded near miss (25 rows or fewer) that a merged wave 8 package left, one job per C file, starting from the wave 8 agent's NOTES.md in `drafts/T-251NN/`. The wave 8 conveyor skips these closers because of a function-name format mismatch in its script.

## Acceptance criteria

- [ ] Each job: match (literal default-C, 27/27) or an updated NON_MATCHING comment and NOTES.md with the next hypothesis.
- [ ] Review against CODING_STANDARDS.md before Done.

## Notes

Jobs are appended under Comments as they finish (file, functions, result).

## Comments

2026-10-23, TAIIKU `src/ovl/TAIIKU/80133C80.c`, `func_80137784`: no match, guard kept at 4 rows (NON_MATCHING comment now names T-25201). Measured why the compare operand order cannot be reached with the offset alias: uopt swaps `==` when its first operand is not a plain variable, and a constant addend on the second operand moves to the first (uopt or ugen). A control with undefined zero-offset alias symbols gets every instruction right (8 rows, all alias relocations). Next: `config/symbols_TAIIKU.txt` aliases at 0x8014984C and 0x8014981C, which need approval. Notes: `drafts/T-25106/TAIIKU/func_80137784/NOTES.md`, `alias-8rows.c`.

Review of w9c-TAIIKU-80133C80 against CODING_STANDARDS.md (2026-10-23): the diff is one comment in `src/ovl/TAIIKU/80133C80.c` plus notes under `drafts/T-25106/TAIIKU/func_80137784/` and `wiki/`. No header, build, tool or game-data change. Findings: (1) the NON_MATCHING comment was dense and repeated the ticket id; reworded so it states the uopt swap rule, the register-priority reason the real symbol on the left fails, and the next step, one ticket id. (2) `alias-8rows.c` is not buildable and says so in its header; it sits under `drafts/` where nothing is compiled. Both resolved. No function matched: `func_80137784` stays guarded (NON_MATCHING) at 4 rows (`bne` operand order at rows 119 and 263, checked with funcdiff --resolve --version c). Clean rebuild 27/27 (default build unchanged). Notes: `wiki/matching-notes.md` T-25201 section and `drafts/T-25106/TAIIKU/func_80137784/NOTES.md`.

2026-10-23, main `src/main/8004C3F0.c`, `mod_set_gpu`: matched, guard removed (default build MATCH, no INCLUDE_ASM, 27/27 SHA1, headers/globals/branch checks OK). The for-loop bounds read from the record, a FAKE dead `idx = 0;` after p (named T-25201, stops copy propagation of p) and an `xpos` local. Notes: `drafts/T-25104/main/mod_set_gpu/NOTES.md`, [[matching-notes]] "T-25201: main mod_set_gpu (closer)".

Review of w9c-main-8004C3F0 against CODING_STANDARDS.md (2026-10-23): the diff is `src/main/8004C3F0.c` (the only C file), notes under `drafts/T-25104/main/mod_set_gpu/` and `wiki/`. No header, build, tool or game-data change; the diff has no inline asm. Findings: none. `mod_set_gpu` is plain C with no guard and no INCLUDE_ASM (grep for the INCLUDE_ASM line is empty); the one new FAKE (`idx = 0;`, dead store that stops uopt copy-propagating p) names T-25201 and gives the reason, and the existing T-25006 FAKEs are unchanged; the file-scope `u16 search_tpage_multi();` view is the pre-existing one and is explained in the function comment. C89 holds (declarations at block top, `/* */` comments). Open follow-up from the closer: find a natural form that avoids copy propagation of p so the dead-store FAKE can go (idx reuse and `idx = p->unk_08; switch (idx)` both miss). Verified: clean rebuild 27/27 SHA1; `funcdiff mod_set_gpu --unit main --resolve --no-build --version default` prints MATCH (804 bytes); `mod_set` still MATCH (780 bytes). Matched: `mod_set_gpu` (804 bytes). Left with rows: none from this job; notes in [[matching-notes]] "T-25201: main mod_set_gpu (closer)".

2026-10-23, TACO `src/ovl/TACO/8013DB80.c`, `func_8013EAA4`: matched, guard removed (default build MATCH, no INCLUDE_ASM, 27/27 SHA1, headers/globals/branch checks OK). The 4 rows were the third grid's Y temporary at sp+0x64 instead of 0x58: uopt `spilltemps` hands out temporaries in bit order, and `o * 0x100` came before `j * size * 2`. A constant local `half = 0x80` used as `j * half * 2 - half` and `o * half * 2 - half` makes the propagated Y item late, so it gets 0x58. `pad_b` became `half`; `pad[8]` and `pad_a` stay FAKE. Notes: `drafts/T-25110/TACO/func_8013EAA4/NOTES.md`, probe `spillpatch.py`.
