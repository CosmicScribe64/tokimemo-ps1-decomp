---
id: T-24030
title: Recover BUNKASAI sibling package
status: In Progress
assignee: Codex
created: 2026-10-17
updated: 2026-10-17
links: ["[[bunkasai-family-results]]", "[[bunka-family-results]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal
Match func_80142700 in src/ovl/BUNKASAI/80142700.c (1,176 literal bytes) and func_80151364 in src/ovl/BUNKASAI/801511E0.c (844 bytes). Reuse preserved built worktree /private/tmp/tokimemo-complete-etc, branch codex/complete-bunkasai-siblings, fast-forwarded without resets from a00f55090 to main a6fa29a65.

## Acceptance criteria
- Consult current family notes, sibling bodies and prior failed probes before new work.
- One compiler total, Docker only and ninja -j1; no per-function patches or inline assembly.
- Preserve existing matching C. Report necessary shared-header edits first.
- A C win must remove target INCLUDE_ASM and guard, then pass normal default linked funcdiff, affected checksum and header/global/prototype guards.
- Send pinned wins promptly; root handles independent checks, all 27 checksums and pushing. Preserve useful source-only drafts and concrete blockers.

## Notes
Fresh main sibling census build/complete-siblings.json: func_80142700 is 0.886 similar to matched BUNKAKEN func_80144F70; func_80151364 is 0.822 similar to matched BUNKASAI func_801537BC. Prior T-22012 leaves func_80151364 at two swapped lui instructions in case 5; do not repeat exhausted casts, local-width, declaration-order, line-group and broad permuter probes without new evidence. Recent T-24016 matches confirm direct D_800CA148 stores and GameState pointer access.

## Comments
Exclusive production ownership limited to the two assigned C files/targets. Completed BUNKASAI 801354B0.c / 80154018.c, TT, DATE2 and NAME_ENT are excluded. Other active root workers retain their assigned DATE, EVENT, GYOZI, GEKO, SHUGAKU and MAIN files. Worktrees and prior DATE/ETC history remain preserved.
