---
id: T-25502
title: Stubborn function TAIIKU func_80137784 (src/ovl/TAIIKU/80133C80.c)
status: In Progress
assignee: wave-12 workflow
created: 2026-10-23
updated: 2026-10-23
links: [[[tickets/T-25500-stubborn-functions]], [[playbook]]]
---

## Goal

Match TAIIKU func_80137784 (src/ovl/TAIIKU/80133C80.c) after earlier passes failed (T-24521, T-25106, T-25201, T-25309), using independent lenses, a synthesis pass and adversarial review ([[tickets/T-25500-stubborn-functions]]).

## Acceptance criteria

- [ ] Literal default-C match, 27/27, or a verdict naming the blocker and the change it needs.
- [ ] Review against CODING_STANDARDS.md (7 and 7a) before Done.

## Comments

- 2026-10-23, plain-C lens (worktree w12-taiiku-plainc): MATCH in plain C, no new symbols, no config change. The two pointer compares are uopt's test replacement of `arg0 == 0` (arg0 dead after each test, slot address CSE in a0/a1). Clean rebuild 27/27, check_headers, migrate_globals --check and sync_protos --check-branch pass. Notes and measured negatives: drafts/T-25502/TAIIKU/func_80137784/plainc/NOTES.md. The T-25309 alias patch is not needed. Review against CODING_STANDARDS.md still due.
- 2026-10-23, synthesis (worktree w12-taiiku-final): took the plain-C lens change (the symbols lens found the same one independently). `funcdiff --resolve --version default` MATCH (1216 bytes), clean build 27/27, check_headers, migrate_globals --check and sync_protos --check-branch OK. Correction to the plain-C comment above: arg0 does not have to be dead. It stays live in s0 after the block-3 test. The rewrite happens because the slot address is already in a register at the test. The toolchain lens's uopt rule is not adopted (not needed, and it changes BUNKAKEN func_8014D65C). The T-25309 alias patch and the second-names plan are dropped. The three T-25106 FAKEs (RUN, the `(s32)` record index, the D_801491D8 record) stay. Notes: drafts/T-25502/TAIIKU/func_80137784/NOTES.md (lens notes as lens-*.md). Next: review against CODING_STANDARDS 7 and 7a.
