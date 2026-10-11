---
id: T-9740
title: "Wave 9: list 4"
status: Done
assignee: wave-9-agent-4
created: 2026-10-11
updated: 2026-10-11
links: ["[[matching-notes]]", "[[decompile-workflow]]", "[[register-allocation-wave7]]"]
---

## Goal

Match the assigned wave-9 list of 135 functions (94,672 bytes) in 35 owned C files.

## Acceptance criteria

- [x] Ordered m2c/build/resolved-diff first pass followed by ranked deep attempts.
- [x] At least four hours of work, or every function deeply attempted, per shared brief.
- [x] Retain only verified matches; record unresolved register shapes and useful findings.
- [x] Prototype and GameState checks pass; final incremental ninja passes 27/27 units.
- [x] Inline standards/spec review recorded; no commits, pushes, or merges.

## Notes

Brief: scratchpad/wave9-brief.md. List: scratchpad/wave9-list-4.txt.
Session began 2026-10-11 14:59 UTC. Scratch belongs to scratchpad/w9-4/.

## Comments

User explicitly requires at least four hours; the brief's alternative does
not shorten this session. Work continues through at least 18:59 UTC.

17:22 UTC: RPG_BAT func_80150120 (960 bytes) passes the real overlay SHA1.
Packed shade and three-word transition views, a compound counter increment,
the missing third call argument 0x5A, and a verified unprototyped coordinate
caller view resolve the long-standing draft differences. Types/prototype
appended to RPG_BAT.h; real diffs/full header checks in progress. Total
5 functions / 2,520 bytes.

2026-10-11: Read wiki index, shared brief, local instructions, standards,
workflow, wave-2 overlay notes, register-allocation inventory, and T-9600/T-9610
changes. Proceeding with ordered drafts and a ranked deep pass.

Initial assembly reveals two concrete first-pass repair targets: main
load_palette/load_csr_ab return success/failure despite old void declarations;
EVENT func_8010B8A4 needs record-field declarations and a 32-bit cast for the
packet tag at D_800EC190 (m2c dereferences its byte-array view).

15:10 UTC: Initial configure/ninja completed, all 27 linked-unit SHA1 checks
passed, headers OK, globals OK. No C retained yet.

Baseline progress: grand total 4870/6958 functions, 776660/2279368 bytes (34.1%).
Initial owned-file dupes/neardupes scans both applied zero functions.

15:50 UTC: First retained match: main func_80041168, 376 bytes. Genuine
RECT plus separate recursive-loop counters, masked index in the loop
condition. Main executable SHA1 passes and resolved funcdiff reports MATCH.
No new declarations or FAKE needed. Standalone probe padding was the only
remaining scratch diff; the whole-file proof removed it.

16:00 UTC: Ordered first pass finished 135/135, with every rejected C draft
restored. Fifteen compile/diff cases; 120 declaration/type/syntax failures.
No untouched m2c draft matched immediately. Deep pass continues: the wipe
routine func_80057710 is down from 74 to 16 scheduling differences and has
a 300-second permutation running. SHOUGATU permutation best score 350,
no zero; the exact frame/table form retains an eight-line scheduling gap.

16:00 UTC: RPG_BAT func_8014FDBC matched (868 bytes). Restore the third
func_8014B738 call argument 0x3C that m2c omitted; all branch offsets and
the jump table then agree. Overlay SHA1 and resolved diff pass. Running
total: 2 functions / 1,244 bytes.

16:10 UTC: EVENT func_8010B8A4 matched (204 bytes) with recovered 0x44/0x24
record views, a byte snapshot and a signed clear mask. No scalar aliases or
FAKE needed. Real SHA1/diff pass. Types/prototype moved to EVENT.h for final
header verification. Historical T-9170/T-9320 gap closed. Total 3 / 1,448.

16:24 UTC: NAME_ENT func_801478E0 matched (112 bytes): plain 128-word
GameState palette fill, no invented members/pointer arithmetic. Real
NAME_ENT SHA1 and diff pass. Total 4 / 1,560. Full rebuild after EVENT's
header movement passed 27/27 units with headers/globals OK and retained
EVENT/RPG_BAT diffs rechecked. Scoped dupes scan started after the four gains.

2026-10-11: main func_8004164C (500 bytes) matches with native indexed sprite
records and a GameState pointer. Real main SHA1 and resolved diff pass; type
moved to include/game.h. Six retained functions / 3,020 bytes.

2026-10-11: BUNKASAI func_8013B190 (204 bytes) and SHOUGATU func_8013E330
(224 bytes) both pass real SHA1/resolved diff with genuine aggregate views.
Total 8 functions / 3,448 bytes. New header types/prototypes and BUNKASAI
formatting await regression. TAIIKU own data needs an island/YAML change;
dry-run only because YAML edits are outside this batch scope.

2026-10-11: KANGEI func_80137484 (192 bytes) passes real SHA1/diff with
all byte accesses through one sprite array. Nine functions / 3,640 bytes.
Eight-match full header rebuild also passed all 27 checks after BUNKASAI
formatting/type migration. ETC zero-instruction probe awaits real proof.

2026-10-11: ETC func_801464D4 (292 bytes) passes real SHA1/diff with a
GameState pointer and direct record fields. Removed both inferred year
snapshot and redundant mode copy; no FAKE. Total 10 functions / 3,932 bytes.

2026-10-11: Added 14 bounded T-0018 rows and documented five historical
resolutions. Ten-match full rebuild passes 27/27 SHA1 and headers/globals.
The four-hour floor remains 18:59 UTC; deep work continues.

18:29 UTC: KANGEI func_80137544 (244 bytes) passes real SHA1/resolved diff.
Sprite-array reload plus shared signed mask resolve the historical row;
integer definition/no explicit return preserves the original register form.
No FAKE. Total 11 functions / 4,176 bytes. Header/comment regression follows.

18:33 UTC: SHOUGATU func_801326F8 (364 bytes) passes real SHA1/resolved
diff with canonical GameState fields and the indexed b1 >= 1 loop. No FAKE.
Scratch-only trailing nop is padding. Declaration/prototype moved to header;
loop counter renamed, all RGB-byte offsets documented, integer definitions
commented. Total 12 / 4,540 bytes. Final regression follows.

2026-10-11 inline standards/spec review against CODING_STANDARDS.md and the
shared brief, with the local code-review skill applied inline as required by
the brief. Checked all 12 retained bodies and changed headers. Review fixes:
RGB byte offsets documented; integer/no-return definitions explained;
redundant EVENT prefix type replaced with existing complete Rec24. EVENT
SHA1/resolved diff still pass. No unresolved source/spec findings. Final
prototype/global checks and all-unit regression remain before Done. Sixteen
new T-0018 rows; no extra gain from late table/counter probes.

## Retained matches

| Unit/file | Function | Bytes |
| --- | --- | ---: |
| main/80041000 | func_80041168 | 376 |
| main/80041000 | func_8004164C | 500 |
| BUNKASAI/8013B190 | func_8013B190 | 204 |
| ETC/80146400 | func_801464D4 | 292 |
| EVENT/8010B360 | func_8010B8A4 | 204 |
| KANGEI/80135F10 | func_80137484 | 192 |
| KANGEI/80135F10 | func_80137544 | 244 |
| NAME_ENT/80144AD0 | func_801478E0 | 112 |
| RPG_BAT/8014F790 | func_8014FDBC | 868 |
| RPG_BAT/8014F790 | func_80150120 | 960 |
| SHOUGATU/80132550 | func_801326F8 | 364 |
| SHOUGATU/8013E330 | func_8013E330 | 224 |
| Total | 12 functions | 4,540 |

2026-10-11 final verification: incremental ninja (no -k) passes 27/27 SHA1
checks after the review change. Fresh resolved diffs pass all 12 retained
functions. Prototype branch check: zero new disagreements, 17 known; globals
check OK. Shared headers, log, T-0018 inventory and own matching-notes additions
are append-only against HEAD. Nine changed source files are all assigned.
Ticket/card/index and new wiki links pass scoped lint. Final exact and near
owned-file scans see 4,882 live C matches and apply zero additional functions.

Actual ninja progress grand line:
`grand total   4882/6958   781192/2279368  34.3%`

16 new unresolved T-0018 rows. Seven older functions' gaps resolved without
altering historical entries. Top other blocker: TAIIKU/80133C80 has ownership
but no active data island; the dry run needs a YAML change prohibited here.

Tool observations: permuter setup on full-header contexts can run outside
--time for many minutes; minimal contexts finish normally. Exact/near scans
use stale generated assembly folder classification until splitting again;
a scratch-only live-C wrapper supplies the 12 current seeds. No repository
tools/configs modified. No commits, staging, merges, pushes or host installs.

User's four-hour requirement remains pending until at least 18:59 UTC;
additional bounded work continues before final Done.

18:57 UTC: Final controller and bitmap probes rejected (best 72/139 lines),
with no retained source/header changes. m2c loses func_8004F6B0's loop-back
button reload; recorded as a semantic tool observation. Direct headers
check passes separately, and build/headers.ok/build/globals.ok are current.
All source review findings resolved; final elapsed-work floor is next.

19:00 UTC: Four-hour requirement met (started 14:59 UTC, worked through
at least 19:00 UTC). Inline review and all required checks pass; no unresolved
findings. Ticket/card/index moved to Done. Retained 12 functions / 4,540 bytes;
16 new T-0018 rows. All changes remain uncommitted for the orchestrator.
