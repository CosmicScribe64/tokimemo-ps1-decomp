---
id: T-12801
title: EVENT control flow matching
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[decompile-workflow]]"]
---

## Goal

Recover byte-identical C in the assigned files; prioritize largest functions using full-TU evidence.

## Acceptance criteria

- Resolved full-TU byte proof and affected binary SHA1 for each win.
- Review against CODING_STANDARDS.md; final 27 checks.

## Notes

Isolated branch codex/t12800-large-event-tel, origin/main 95c3fc9f. No changes to original checkout or other workers. Parent T-12800; scope main/8006CB30.c, EVENT/8010FE10.c, TEL/80132000.c. At most two concurrent compilers.

## Comments

2026-10-15: Read T-11600 and T-11030 history. The 6,848-byte dispatcher func_801103A0 had not been attempted by T-11030. Generated m2c draft in untracked build/t12800-evidence/event/t12801. Original explicitly writes v0=0 at handled-event returns, while existing declaration and m2c output are void; testing s32 with return 0 and fallthrough call. Indexed bytes at D_800B0A34/D_800B0A36 map to Rec34 offsets 0x30/0x32; D_800B0A5E maps to record 1 offset 0x26. Draft uses existing pad array fields pending score, and removes phantom arguments m2c inferred on unprototyped calls. Configured shared worktree successfully in Docker. No matching claim yet.

2026-10-15 bounded full-TU experiments (Docker, existing arm64 image):

- `func_801103A0`: initial reconstruction with handled-event `return 0`, phantom arguments removed, and Rec34 accesses recovered compiled successfully. `funcdiff --unit EVENT --resolve` reported approximately 2,400 differing output lines; generated code was only 20 bytes shorter but the register assignment diverged throughout. First differences: mask generation order and stack byte slots 0x2F/0x2E versus expected 0x2B/0x29; selector/constant chains used a3/a2 rather than expected a2/a0. This is not a near match and no new externs were retained.
- Removing m2c's synthetic `var_at` comma assignments from the three-way month test (ordinary OR followed by `sp2B < 2`) worsened the result to 2,795 differing instruction lines. The generated short-circuit temporary influences allocation; simple Boolean cleanup does not solve this case.
- `func_80114B0C`: a discarded pre-loop read of D_80124394, testing the historical entry-read mechanism against the equal-priority pointer-base tie, left exactly the same four differing instruction rows (t1/t2 bases exchanged). Reverted; do not repeat this candidate.
- All owned production C and EVENT.h restored to baseline. Scratch drafts and exact reports remain locally under `build/t12800-evidence/event/t12801/` (`start.c`, `large-cleaned-tu.c`, `diff1.txt`, `diff2.txt`, `tie1.txt`). No byte gains claimed, no code or header changes handed off. Parent handles ticket/kanban/log review status and final integrated checks.

Third-chain follow-up for func_80118868: explicitly initialized n=3 before the D_800B095E branch, used it in D_80094730 += n and the final halfword chain, to test a value shared across branches becoming a uopt colour. s32 n broke the chain into separately materialized constants and added 8 bytes; s16 n preserved chain shape but returned to the same historical 8 differing rows (four store-order rows, li t6 versus v0, and three register uses). Both rejected and production source restored. Exact reports chain1.txt/chain2.txt and final s16 candidate are retained in local scratch.

Final checkpoint: 27/27 binary SHA1 hashes pass; prototype branch check has zero new disagreements; headers and global migration checks pass. Standards and separate spec review found no outstanding issues. Investigation batch complete; only date_sasoi_judge gained matching bytes (644). Evidence is local and gitignored under build/t12800-evidence/. No claim of global 80% completion.
