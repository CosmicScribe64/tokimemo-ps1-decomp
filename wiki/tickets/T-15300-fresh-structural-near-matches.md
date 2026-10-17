---
id: T-15300
title: Resolve fresh structural near matches
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-17
links: ["[[matching-notes]]", "[[tickets/T-14501-remaining-draft-proof]]"]
---

## Goal

Resolve fresh non-guarded near matches from completed T-14500 units through bounded C structure/type changes. Start with BUNKASAI func_801438E4 (852 inventory bytes, score 14). Incoming zero promotion remains priority.

## Acceptance criteria

- [x] Only default literal matches and affected binary SHA1 passes are promoted.
- [x] Existing C preserved; ordinary C or any FAKE marked.
- [x] Header/global/prototype guards pass.

## Notes

Avoid main, EVENT, RPG_BAT, and separately reserved repair functions. Historical wiki/catalog contains no earlier attempt for BUNKASAI func_801438E4.

## Comments

2026-10-15: Claimed at root's request. Inspect original control flow before bounded experiments; do not change the frozen census context.

2026-10-15: BUNKASAI func_801438E4 matched: 852 inventory coverage bytes / 852 literal linked bytes. All 14 diff rows came from the final byte flag ORs. Replaced those operations with the existing character-record CharFlags.f.b12 and .b13 assignments; explicit byte read-modify-write syntax had not helped. This uses the established aggregate and bit-field model (T-9020/T-12300); no FAKE or headers changed. Original C preserved: only the target INCLUDE_ASM line removed. Docker default literal proof, BUNKASAI SHA1, headers/globals and branch prototype guard all pass (zero new / 15 known). Evidence: /private/tmp/t15300-bunkasai-{seed,rmw,bitfield}-diff.log and /private/tmp/t15300-bunkasai-final-proof.log. Continuing fresh TACO candidates under this ticket; incoming zeros retain priority.

2026-10-15: BUNKAKEN func_80145844 matched: 440 inventory coverage / 440 literal linked bytes. The generated mixed label used a C hex escape for the Shift-JIS byte before a backslash; IDO's K&R lexer does not handle that spelling as intended. Restored the ordinary Unicode label, following root's BUNKASAI encoding finding; the existing build converts it correctly. Default literal proof, BUNKAKEN SHA1 and header/global/prototype guards pass. No headers, FAKE, or existing C changed. Evidence: /private/tmp/t15300-bunkaken-unicode-proof.log. Generator owns the broader encoding post-pass and excludes this reserved repair.

2026-10-15: TACO func_80133738 remains local and guarded. Correcting its saved selector and using existing TcPair8 lookup entries plus TcActor[8].unk78 fixes m2c's genuine table-stride error (the seed scaled by 32 instead of 8). Remaining differences are the selector home slot (+0x2C instead of +0x28) and four lookup temporaries; paused for incoming exact/encoding candidates. Evidence: /private/tmp/t15300-taco-{state,structure}-diff.log.

2026-10-15: Final handoff at the user's request: no further experiments. The unfinished TACO func_80133738 probe is archived at drafts/T-15300/TACO/func_80133738/ with body, exact guarded translation-unit context, quoted header snapshots and SHA256 manifest, three diagnostic logs, transformation scripts, and findings. It remains nonmatching (eight instruction positions); no untested local-declaration reorder is claimed. Production src/ovl/TACO/801335A0.c is restored byte-for-byte to its committed INCLUDE_ASM fallback. No assembly files, original binaries, or build artifacts were added. Verified T-15300 production gains are BUNKASAI 852 plus BUNKAKEN 440 = 1,292 inventory coverage bytes; TACO adds zero. Status In Review for root's final handoff check.

2026-10-17: Review closed with no findings. BUNKASAI func_801438E4 (852 bytes) and BUNKAKEN func_80145844 (440 bytes) are on the integration tree as plain C, 1,292 bytes in total. The archived TACO func_80133738 probe was finished afterwards in T-16561 (matched, commit dc76bc447). Moved to Done.
