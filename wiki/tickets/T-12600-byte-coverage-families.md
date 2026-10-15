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
