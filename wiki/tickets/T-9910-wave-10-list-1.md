---
id: T-9910
title: "Wave 10: list 1"
status: Done
assignee: wave-10 batch agent 1
created: 2026-10-11
updated: 2026-10-17
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal

Match the assigned list-1 functions using the wave-10 brief. Work only in the 47 owned source files and leave edits uncommitted.

## Acceptance criteria

- [ ] Draft/build/diff first pass and ranked deep iteration.
- [ ] At least four hours of work, or a deep attempt on every function.
- [ ] Retained matches pass real funcdiff and all 27 binary checks.
- [ ] Header/global checks and inline CODING_STANDARDS review pass.

## Notes

List: scratchpad/wave10-list-1.txt (132 functions, 56,324 bytes).
Evidence: scratchpad/w10-1/. Started 2026-10-11 20:19 UTC.

## Comments

- Read the wiki index, brief, standards, workflow, register clusters, allocator trace implications and relevant overlay notes. The brief requests inline review without reviewer subagents.
- Initial worktree required configure and baseline generation before m2c could run. No source changes yet.

- Collected 202 earlier C drafts for 54 assigned functions from T-9800's frozen manifest into scratchpad/w10-1/historical/. These are candidates, not verified matches. Baseline generation is running; scratch return-type probes can run against the generated main assembly independently.

- Scratch zero-return probes: s32 definitions with explicit return 0 produce the missing value operation, but the jr/store scheduling remains different; void definitions reject return 0. Rejected probes remain in scratch only. Migration of old GameState names uses inferred access-width declarations to select union members before removing the temporary declarations.

- Fresh baseline Docker build: 27/27 SHA1 checks OK, headers OK. Baseline report saved in scratchpad/w10-1/baseline-progress.txt. No baseline source changes.

- Baseline grand total: 4948/6958 functions, 807520/2279368 bytes (35.4%). Scoped dupes and neardupes applied zero candidates. Ordered first pass is running; zero-return helpers also have five-minute scratch permuter searches running independently.

- Ordered first pass reached item 10; per-function build/diff evidence and rejected drafts in scratchpad/w10-1/firstpass.jsonl and firstpass/.

- Ordered first pass reached item 20; per-function build/diff evidence and rejected drafts in scratchpad/w10-1/firstpass.jsonl and firstpass/.

- Historical scratch probes: 202 candidates evaluated, 98 compiled against current headers. Best prior main func_80074DE0 draft matched instruction span with a void definition, incompatible with the real u8 prototype/caller. A 35-form return-type/selector probe found the ordinary local GameState pointer idiom matches the scratch span for u8 as well; queued that body for the ordered runner's real build. Not counted as a match until real verification.

- Ordered first pass reached item 30; per-function build/diff evidence and rejected drafts in scratchpad/w10-1/firstpass.jsonl and firstpass/.

- Ordered first pass reached item 40; per-function build/diff evidence and rejected drafts in scratchpad/w10-1/firstpass.jsonl and firstpass/.

- Ordered first pass reached item 50; per-function build/diff evidence and rejected drafts in scratchpad/w10-1/firstpass.jsonl and firstpass/.

- Ordered first pass reached item 60; per-function build/diff evidence and rejected drafts in scratchpad/w10-1/firstpass.jsonl and firstpass/.

- Type repair probes now compile packet candidates with the shared psx_types.h/libgte.h and replace invalid byte-array unk-offset syntax only where the original instruction gives an unambiguous access width. Retained all evidence in scratchpad/w10-1/type-repair*. No packet match claimed; the ENDING G4 routine still differs in argument/pointer allocation after type repair.
- Five-minute scratch permuter for NAME_ENT func_80132198 improved its weighted score from 85 to 25; no zero candidate yet. Search scores are not instruction-diff counts. Main zero-return line-layout variants are also being measured separately.

- Ordered first pass reached item 70; per-function build/diff evidence and rejected drafts in scratchpad/w10-1/firstpass.jsonl and firstpass/.

- NAME_ENT func_80132198: resolved scratch probe of the permuter's score-25 candidate is zero instruction rows. Removed redundant all-ones masks; replaced its dummy goto/label with one empty statement. Six simplification probes show no-empty-statement = two rows, empty statement/dead-if/do-while = zero. Queued the simplest empty-statement body with a FAKE scheduling comment for real verification.

- Ordered first pass reached item 80; per-function build/diff evidence and rejected drafts in scratchpad/w10-1/firstpass.jsonl and firstpass/.

- Ordered first pass reached item 90; per-function build/diff evidence and rejected drafts in scratchpad/w10-1/firstpass.jsonl and firstpass/.

- Ordered first pass reached item 100; per-function build/diff evidence and rejected drafts in scratchpad/w10-1/firstpass.jsonl and firstpass/.

- First pass verified main func_80074DE0 (324 bytes): real funcdiff MATCH and full ninja success.

- Ordered first pass reached item 110; per-function build/diff evidence and rejected drafts in scratchpad/w10-1/firstpass.jsonl and firstpass/.

- First pass verified NAME_ENT func_80132198 (392 bytes): real funcdiff MATCH and full ninja success.

- Ordered first pass reached item 120; per-function build/diff evidence and rejected drafts in scratchpad/w10-1/firstpass.jsonl and firstpass/.

- Ranked deep probes now include explicit-return/assignment forms for four reset helpers; NAME_ENT coordinate-tail assignment forms (84 variants, four rows remain); typed ETC fade loop widths and chain stores; club-selection bit-field/control forms; main wipe loop forms; and main menu-selector state/pointer views. Evidence is in scratchpad/w10-1/ with per-trial resolved instruction scores. The checksum has typed K&R parameters and explicit tail locals; no further match yet.

- Ordered first pass reached item 130; per-function build/diff evidence and rejected drafts in scratchpad/w10-1/firstpass.jsonl and firstpass/.

- Deep bounded search main join_club_select: resolved scratch score 1 after 45 seconds; evidence scratchpad/w10-1/deep-queue/.

- Ordered first pass completed all 132 functions. New scratch-span matches: main func_80057710 with s32 fallthrough (484 bytes), main func_80076B48 with s32 fallthrough plus a local GameState pointer (308 bytes), and ETC func_80142AE8 with u8 fade temporaries and reverse byte chains (152 bytes). Club selection is one padding nop from a span match with s32 fallthrough and a nibble view. Real verification is serialized; scratch candidates are not yet counted.
- Thirty further bounded search jobs are queued from ranked, compiling C candidates. These run one search worker and do not mutate src; `--no-verify` prevents concurrent candidate staging during the real verification queue. Scratch target normalization only equates addresses/aliases already documented by config; real assembly is untouched.

- Deep pass verified main func_80057710 (484 bytes), s32 fallthrough definition and generated API declaration: real funcdiff MATCH plus full ninja success.

- Deep bounded search ENDING func_80139F3C: resolved scratch score 18 after 339 seconds; evidence scratchpad/w10-1/deep-queue/.

- Deep pass verified main func_80076B48 (308 bytes): real funcdiff MATCH and full ninja success.

- Deep bounded search KANGEI func_80135004: resolved scratch score 27 after 348 seconds; evidence scratchpad/w10-1/deep-queue/.

- Deep bounded search GYOZI func_8013E638: resolved scratch score 29 after 344 seconds; evidence scratchpad/w10-1/deep-queue/.

- Deep pass verified main join_club_select (252 bytes): real funcdiff MATCH and full ninja success.

- Deep pass verified ETC func_80142AE8 (152 bytes): real funcdiff MATCH and full ninja success.

- Deep bounded search EVENT func_8010B614: resolved scratch score 17 after 336 seconds; evidence scratchpad/w10-1/deep-queue/.

2026-10-17: Closing as superseded. The Codex run hit its usage limit after 6 matches were merged (main func_80074DE0, func_80057710, func_80076B48, join_club_select; NAME_ENT func_80132198; ETC func_80142AE8; 1,912 bytes), all still C on main and covered by the T-20900 27/27 rebuild. The rest of list 1 is covered by the T-14500 census (drafts/T-14500) and the later waves; ENDING func_80139F3C has matched since, KANGEI func_80135004 is still open. Review against CODING_STANDARDS found nothing: both FAKE comments give reason and T-9910, no NON_MATCHING, no inline asm.
