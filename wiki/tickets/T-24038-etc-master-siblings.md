---
id: T-24038
title: Recover ETC renderer and MASTER sibling handlers
status: In Progress
assignee: Codex
created: 2026-10-17
updated: 2026-10-17
links: ["[[etc-family-results]]", "[[master-family-results]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal
- ETC src/ovl/ETC/8013F980.c: func_8013FC64, 796 literal bytes; matched MAIN func_8004AE54 sibling (0.866).
- MASTER src/ovl/MASTER/80139000.c: func_8013B448, 956 literal bytes; matched MASTER func_80138088 sibling (0.85).

## Acceptance criteria
- Read relevant family notes, prior drafts and failed probes; bounded novel pass near 25 minutes.
- Reuse built /private/tmp/tokimemo-complete-etc on codex/complete-etc-master-siblings from 5ef7c0dd5, preserving all prior verified/archive ancestry.
- One GPT-6.1 Sol medium compiler total, Docker only, ninja -j1. Only two assigned C files; notify exact necessary header types before production edits.
- Explicitly remove target guard and INCLUDE_ASM before normal-default literal linked proof; affected checksum and header/global/prototype guards required.
- Preserve existing matched bodies and useful source-only drafts; send pinned wins promptly. Root owns independent review, all-27 checksums, integration and push.

## Prior evidence
T-17013 matched MAIN func_8004AE54 in src/main/8004AD60.c using SPRT_8, paired font-table bytes, do/while inside the sign-range guard, explicit texture-coordinate locals tpx then tpy for the spill, and division before digit-index increment. Its apparently redundant signed-range tests correspond to target branches and are documented. T-18600 MASTER func_8013B448 typed draft reached 264 rows from 329; fresh m2c still failed, and shifted operand registers were noted. Prefer matched sibling/control-flow/type evidence over re-running those broad redrafts.

## Comments
T-24031 source-only archive is frozen at 5ef7c0dd5237fc006d1f94ac3957d5605e75fc13 with zero new C bytes; T-24030 BUNKASAI two matches remain preserved in ancestry. No other files claimed, no main API edits without prior notification, no new checkout/reset or host install.
