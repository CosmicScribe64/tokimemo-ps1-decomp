---
id: T-27058
title: Extract scheduler, day-event and school-day topic headers
status: In Progress
assignee: integration
created: 2026-10-24
updated: 2026-10-24
links: ["[[spec/header-split-plan]]", "[[tickets/T-27054-topic-header-tooling]]", "[[tickets/T-27044-main-file-name-pilot]]"]
---

## Goal

Move the reviewed63existing declarations into include/main/schedule.h, day_event.h and school_day.h. Keep main_api as a forwarding umbrella during transition; preserve all C code, widths, intentional prototype omissions and legacy GYOZI views.

## Acceptance criteria

- [ ] Exact reviewed declaration extraction with preserved evidence comments, no duplicate canonical declarations.
- [ ] All437snapshot payloads compare0benign/0check/0risky, header/rename/aggregate guards pass.
- [ ] Clean Docker27/27hashes and6952functions, independent code review resolved before Done.
- [ ] Living header/file documentation and tracker updated; umbrella deletion remains a later full-tree gate.

## Comments

2026-10-24: Primary integration active after reviewed four-file header-tool patch was applied byte-for-byte and integrated23header+95prototype tests passed.

2026-10-24 live extraction: include/main/schedule.h32declarations, day_event.h5, school_day.h26; total63existing declarations moved from main_api into forwarding leaves. Transactional guard passed:all437Csourcehashes unchanged, all34intentional prototype omissions preserved, header checker0problems, all437views0benign/0check/0risky. Metadata/evidence .claude/orchestrator/state/T-27058/result.json; strictCLIcheckandclean27/27buildare running. Real topicheaders nowexist, broaderumbrella deletionstillopen.

2026-10-24 matching proof: strictCLIcompare0benign/0check/0risky; cleanDockerbuildexited0 with27/27SHA1and6952/6952functions(2276920/2276920bytes). All437Csourcefiles remain byte-unchanged in this extraction. The3topicheadersarelive. Independentfinalreview remains pending; removal of the forwarding umbrella and wider shared-type/header migration remain separate work.

2026-10-24 final-review fixes: moved orphan T-25049 and T-14501evidence comments beside their declarations in school_day.h/schedule.h; added the plan's explicit topic includes to scheduler.c and school_day.c while retaining game.h state closure. No bodies/types change: both Cfiles differ only by5include lines; other435remainbyte-identical. Rechecked all437viewpayloads0/0/0 andheaderguard0problems. Original437unchanged claim describes the initial declaration-only extraction; finalsourceincludes are the deliberate reviewed follow-through. Post-fix matching rebuild is recorded separately.
