---
id: T-25501
title: Stubborn function SHOUGATU func_8013EA98 (src/ovl/SHOUGATU/8013E330.c)
status: Done
assignee: wave-12 workflow
created: 2026-10-23
updated: 2026-10-23
links: `[tickets/T-25500-stubborn-functions`, [[playbook]]]
---

## Goal

Match SHOUGATU func_8013EA98 (src/ovl/SHOUGATU/8013E330.c) after earlier passes failed (T-25046, T-25143, T-25305), using independent lenses, a synthesis pass and adversarial review ([[tickets/T-25500-stubborn-functions]]).

## Acceptance criteria

- [ ] Literal default-C match, 27/27, or a verdict naming the blocker and the change it needs.
- [ ] Review against CODING_STANDARDS.md (7 and 7a) before Done.

## Comments
- 2026-10-23 (synthesis, w12-shougatu-final): not matched; `INCLUDE_ASM` kept. Best body `drafts/T-25501/SHOUGATU/func_8013EA98/func_8013EA98.c`, 120 rows (`funcdiff --resolve --version c`), 230 words, frame 0x58. The whole selection loop now matches, registers included, from the cleanroom W1a loop form (13 blocks give the original's constant order) and the compiler lens's split mechanism (b1 `* 0` hole, alias forced store, free `* 0` references, a post-loop hole and a `>= 0` block for the selector's priority window). Left: the post-loop `lui v1`. It is a selector piece coloured v1 in the original; the alias form Z1 reproduces it but then loses the loop split, and under IDO 5.3 costs both can't hold together. Also left: the operand order of the FE4 == 9 compare. No tool or config change. Notes: `drafts/T-25501/SHOUGATU/func_8013EA98/NOTES.md`; wiki/matching-notes.md section "T-25501".
- 2026-10-23 19:30 (synthesis, w12-shougatu-final): **matched.** Plain C in `src/ovl/SHOUGATU/8013E330.c`; `funcdiff --resolve --version default` MATCH; clean build 27/27. The FAKEs, all marked T-25501, are the loop `* 0` references, the forced choice in the counter, an alias-hole read, the D_800E71DF alias store and reads, and the byte-view argument to func_800847B8. Mechanism and the measured FAKE removals: `drafts/T-25501/SHOUGATU/func_8013EA98/NOTES.md`. Adds a D_800E71DF keep line with a byte-match proof (`proof=func_8013EA98:130:cb9e034c`, keepproof PROVEN). Needs review against CODING_STANDARDS 7 before Done.
- 2026-10-23, review (orchestrator): the technical reviewer approved (isolated clean build 27/27, `funcdiff --resolve --version default` MATCH 920 bytes, no INCLUDE_ASM, header/global/prototype checks, `keepproof` PROVEN for the D_800E71DF keep line, three other SHOUGATU functions unchanged). The standards reviewer found the match correct and every FAKE necessary, and blocked only on marking. Fixed: the loop comment now covers the three `*(u8 *)&` selector views (236/227/224 rows without them) and the u32 counter with its `(s16)` truncation (314 rows with `status++`); the post-loop comment covers the byte-view term in the forced store (152 rows); a comment covers the `[i++]` loop (8 rows with the header increment); NOTES.md now names the config keep line. Full FAKE list for this match, all T-25501 except the last: `* 0` free references in the loop, the three selector byte views, the u32/(s16) counter, the `[i++]` loop form, the forced choice held in the counter, the alias-hole record read in the D_80146054 compare, the forced store and two reads through the D_800E71DF alias (keep line), the byte-view call argument to func_800847B8, and the `(u8)` narrowing of func_80051A68 (T-25038, as in func_8013E6E4). This is a heavily FAKE-marked match; a later readability pass may replace some of them once the original's selector code is understood. Findings resolved; Done.
