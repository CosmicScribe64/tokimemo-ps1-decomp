---
id: T-12600
title: Increase verified byte coverage through shared function families
status: In Progress
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[broad-coverage-results]]", "[[matching-breakthroughs]]"]
---

## Goal
Prioritize verified byte gains toward the user's 80% game-code coverage target. Current main has 1,345,524 / 2,279,368 bytes; reaching at least 80% requires 477,972 additional aligned bytes. Combine largest-function work with shared type and calling-convention fixes that can apply across families.

## Acceptance criteria
- [ ] Test reconstructed source forms on BUNKAKEN flag/addressing near misses using full translation-unit context.
- [ ] Verify every accepted match with linked literal bytes and binary checksums.
- [ ] Record successful and unsuccessful experiments, and publish verified gains incrementally.

## Notes
Prior 190-function broad sweep yielded only 5,036 bytes, so repeating default generation is low yield. First root-owned family: BUNKAKEN func_80141654, func_801422C4 and func_80142A40. Other workers own main/8007C030.c, BUNKA_SD cx9 objects and DATE cx10 objects. Root edits take place in the isolated integration worktree; original checkout remains untouched.

## Comments

## Remaining-byte concentration (97ec697f)
Fresh source/assembly inventory reproduces877612unmatched bytes across1088functions. Largest10total53440bytes;25=103788;50=165336;100=257896;200=387848;400=570084. Even the largest200are insufficient for the421740-byte80%gap. Prioritize source/type fixes transferable across medium/large families, rather than unbounded single-function scheduling searches. Largest remaining scopes: main153212/180functions, TACO94780/126, RPG_BAT84580/83, TAIIKU66732/84, EVENT43796/48, DATE43364/58, NAME_ENT42408/55, ETC38716/49. Current matching scopes stay disjoint; TT remains separately owned and is not reassigned. Reproducible script/data: build/scratch/goal-inventory.py and goal-inventory.json.

## Retained draft census (a46afeb8 production source)
User query: whether the remaining game bytes all have nonmatching drafts. Source scan using srcscan.include_asm_entries and defined_functions identifies125unmatched functions/70500bytes with retained C definitions, versus947functions/799448bytes with only INCLUDE_ASM in tracked source. Total1072functions/869948bytes. Thus only8.1%of remaining bytes have retained source drafts; other functions may have scratch/generated drafts, but no complete cross-worktree scratch census has been established. Absence from tracked source does not mean never attempted. Largest retained drafts include dialogue_speak_part8672bytes and DATE80147958 4044bytes; many large unsuccessful reconstructions intentionally remain scratch-only. Mechanical m2c output is not proof of compiling, faithful or near-matching C.

## KANGEI draft integration (2026-10-15)
Integrated b9bb9045: three KANGEI functions add 1,328 verified bytes. Normal C switch selectors replace only INCLUDE_ASM entries; existing C is preserved. Docker build, affected KANGEI SHA1, header/global guards, branch prototype check and three explicit default linked literal proofs pass (build/scratch/goal-kangei-drafts.log). Current total: 5,930 / 6,958 functions and 1,442,920 / 2,279,368 bytes (63.3%); 380,576 bytes remain to the 80% target. Also integrated 1308ed4a, nine compiling DATE repair archives, with zero coverage credit.

## RPG and ETC draft follow-up (2026-10-15)
RPG_BAT 002eb61d adds 1,048 literal bytes with one shared byte-global declaration. Integrated full Docker build passes all 27 binary checksums; both explicit default function proofs pass. ETC 2c98de67 uses a consistent GameState pointer to resolve the final register difference in func_801447B0 (648 bytes), preserving prior C bodies. Draft archive checkpoint b59ad160 retains completed units through RENSYU. Seven drafting/repair tests pass from tools/; the earlier tests/ discovery ran zero tests and is not counted as validation. Evidence: build/scratch/goal-rpg-drafts.log and build/scratch/goal-etc-447b0.log.

ETC integrated checksum and explicit default literal proof pass. Verified total is now 5,933 / 6,958 functions, 1,444,616 / 2,279,368 bytes (63.4%). Remaining to the 80% threshold: 378,880 bytes.

## SHUGAKU and repair archive checkpoint (2026-10-15)
Integrated SHUGAKU 1addf889: affected checksum, header/global guards, branch prototype check and two explicit default linked proofs pass. Coverage increases 1,052 bytes; literal comparison spans total 1,056 bytes (624 + 432), including padding outside the coverage count. Current total 5,935 / 6,958 functions and 1,445,668 / 2,279,368 bytes (63.4%); remaining to 80% is 377,828 bytes. Archive checkpoints 01dccf6b and ce44a7e0 retain completed units through SHUGAKU and fifteen compiling type repairs, including corrected EVENT callback-table access. All fifteen body/declaration/context hashes pass audit; no repair receives matching credit. Eight exhaustive draft and repair tests pass. Evidence: build/scratch/goal-shugaku-drafts.log.

## Motion-structure and branch reconstruction (2026-10-15)
RPG_BAT 3d581b25 uses the existing RpgMotion20 aggregate for func_80148B08. Root Docker build passes all 27 binary checksums, headers/globals/prototypes and explicit default linked proof (1,128 literal bytes, 1,120 coverage bytes). Header additions are two shared u8 globals and the natural void forward declaration. KANGEI d55faf88 replaces a generated shared-store goto with normal if/else branches, adding 432 coverage bytes without header changes. Evidence: build/scratch/goal-rpg-motion.log and build/scratch/goal-kangei-35ce0.log. The new Sol 6.1 medium chat owns EVENT func_801103A0 (6,848 bytes), ticket T15200; T15100 owns nine unaligned-copy repairs and T15300 investigates fresh BUNKASAI near matches.

KANGEI integrated checksum and explicit default literal proof pass. Combined verified total is 5,937 / 6,958 functions and 1,447,220 / 2,279,368 bytes (63.5%). Remaining to 80%: 376,276 bytes.

## BUNKASAI encoding and bitfield pair (2026-10-15)
Root T15400 restores an ordinary Japanese literal in func_801431D4 (440 bytes); T15300 uses existing CharFlags bitfields in func_801438E4 (852 bytes). Combined default linked proofs, BUNKASAI SHA1 and header/global/prototype guards pass in build/scratch/goal-bunkasai-pair.log. Existing matching C and initial archives are preserved; no header changes or FAKE constructs. Total rises to 5,939 / 6,958 functions and 1,448,512 / 2,279,368 bytes (63.5%); 374,984 bytes remain to 80%.

## Flag continuation and second encoding recovery (2026-10-15)
BUNKASAI func_8013FEB4 adds 820 bytes using existing record-ten bit13; BUNKAKEN func_80145844 adds 440 bytes with the normal Japanese label. Both explicit default linked proofs, affected checksums and guards pass (build/scratch/goal-bunkasai-feb4.log and goal-bunkaken-label.log). Current total: 5,941 / 6,958 functions, 1,449,772 / 2,279,368 bytes (63.6%). Remaining to 80%: 373,724 bytes. Completed archive cb5aad13 and nine unaligned-copy repairs ffac0f04 are integrated; all nine repair hashes pass and none gets coverage credit. Fixed the repair-catalog join failure and passed all eleven draft/repair/audit tests. Initial census complete: 1,066 outcomes, including 129 failures requiring repair; no complete usable-draft claim.
