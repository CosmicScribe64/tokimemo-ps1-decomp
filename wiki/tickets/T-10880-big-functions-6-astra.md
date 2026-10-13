---
id: T-10880
title: Big functions 6 (astra)
status: Done
assignee: Codex
created: 2026-10-13
updated: 2026-10-13
links: ["[[decompile-workflow]]", "[[register-cheatsheet]]"]
---

## Goal

Attempt the assigned 36 functions (59,144 bytes) and match as many as possible in NAME_ENT/80139740,
main/800674B0, main/8006CB30, and DATE/801461B0. Work largest first,
recover shared types and source idioms, and retain useful guarded near misses.

## Acceptance criteria

- [x] At least four hours of matching and focused experiments.
- [x] Matches verified with literal-byte funcdiff and all 27 unit checksums.
- [x] Register misses examined with regdiff; reusable findings in headers/workflow.
- [x] Branch/header/global checks and inline standards review complete.

## Notes

Started 2026-10-13. Shared wave11 brief and astra20 list read. Only owned C
files and permitted declarations/wiki are edited. No Git mutations or commits.

## Comments

- First verified matches: NAME_ENT func_8013ACA4, 724 bytes; func_80140044,
  1,004 bytes. Literal-byte funcdiff MATCH for both; full ninja 27/27 OK.
- Added ScheduleFlags/ScheduleClubFlags, NameMetric4/NameMetricWork and
  NameRecordF00 access views. No new global aliases or copyrighted data.
- Further literal matches: main func_8006F218 (780), func_8006C334 (972),
  NAME_ENT func_801425C0 (704): five functions / 4,184 bytes total.
- C334 initially used a marked oversized day home. Resolved during review:
  ordinary offset and magnitude locals recover the exact frame; no FAKE remains in C334.
- Added ScheduleGirlFlags and shared DisplayRecord44 views. Rejected
  permutation candidates that changed semantics; bounded safer searches
  continue from faithful seeds.
- Sixth literal match: NAME_ENT func_8013BF74 (1,332 bytes), six / 5,516
  total. Natural coordinate locals recover its frame; no new FAKE needed.
  Refined shared display +0x0C/+0x10/+0x34 fields to actual pointers.
- Seventh literal match: NAME_ENT func_8013EBEC (656 bytes), seven / 6,172
  total. State and call-result locals recover the buffer frame naturally.
- Seven-match full build: 27/27 OK. Global check OK; branch prototype check
  OK with zero new disagreements. The old C334 void-result baseline is
  now stale; the orchestrator can remove it from config/proto_known.txt.
- Shared-layout rebuild passes 27/27. Added ScheduleEntryDate/Flags from the
  eight-entry sorter and recorded its December/January ordering.
- Retained guarded EB64 (eight total +/- diff lines) and DF88 (30 lines).
  EB64 has two explicitly marked FAKE allocation devices; neither guard
  replaces assembly in normal builds. Follow-up searches remain active.
- DF88 improved to 22 lines with a plain while loop. EEC4 now has a guarded
  eight-line sibling draft; three near misses retained in total.
- Generated seven profile-table declarations from assembly widths/strides,
  including 17-byte text rows, 3x36-byte lines, and the word sound-ID table.
- Recovered a corrupted local Ninja deps cache after overlapping funcdiff
  builds; a fresh serialized rebuild passes 27/27. Subsequent build-backed
  checks are serialized. Corrupt cache preserved in scratch for diagnosis.
- Inline-review finding resolved: the DF88 update helper removed its guard;
  the unit checksum caught it. Restored the explicit NON_MATCHING/ASM
  wrapper. Both main helper guards remained intact; final build rechecked.

## Review and handoff

Inline review against CODING_STANDARDS.md: seven literal matches rechecked;
27/27 current unit checksums pass. The three guards verify at 8/8/22 lines,
with matching ASM defaults. C89 declarations and fixed-width types checked;
no SDK types redefined. IDO size/offset assertions pass for the new views.
Main API edits use the generator; overlay declarations are append-only,
and the promoted-coordinate override is required by the byte match.
The two event-helper allocation devices are explicitly marked FAKE.
The DF88 guard finding and C334 padding finding are resolved. No unresolved
code-review findings. Final search audit is complete; the session exceeded four hours.

Four T-0018 rows added. Other large drafts remain in scratch: schedule/event
builders, album/profile renderer, sorter, and DATE movie/menu code still
have structural or wider allocation differences. Unsafe permutation
candidates were rejected rather than promoted to source.

## Result

| Unit | Function | Verified linked bytes |
| --- | --- | ---: |
| NAME_ENT | func_8013ACA4 | 724 |
| NAME_ENT | func_80140044 | 1,004 |
| main | func_8006F218 | 780 |
| main | func_8006C334 | 972 |
| NAME_ENT | func_801425C0 | 704 |
| NAME_ENT | func_8013BF74 | 1,332 |
| NAME_ENT | func_8013EBEC | 656 |
| Total | 7 matches | 6,172 |

Guarded handoff: EB64 and EEC4 each have eight total +/- diff lines;
NAME_ENT DF88 has 22. Their remaining differences are register choices.
The final 1,800-second EB64 search and 480-second EEC4 search did not
improve these drafts. Four T-0018 rows document these and the larger CB44
register/frame miss.

`ninja progress`: `grand total   5238/6958  1018532/2279368  44.7%`.

Shared declarations: include/game.h has album/save, display, schedule and
call-pair views; include/main_api.h has generated helper signatures and
seven profile-table declarations; include/ovl/NAME_ENT.h appends owned
local declarations. Reusable source idioms are in [[decompile-workflow]].

Orchestrator follow-up: remove the stale `void-result main:func_8006C334`
entry from config/proto_known.txt when merging. Keep build-backed funcdiff
runs sequential when a shared header is dirty. No Git mutations were made;
all intended edits remain uncommitted in a20.

Final review: Done after more than four hours of work, approximately
11:22–15:24 UTC on 2026-10-13. All acceptance criteria are satisfied.
