---
id: T-24011
title: Match assigned DATE dispatch family
status: In Review
assignee: Codex DATE worker
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-24010-date-etc-coordination]]", "[[date-family-results]]"]
---

## Goal
Match func_80144E20 and func_801449D4 in src/ovl/DATE/80141EE0.c, and func_8015A640 in src/ovl/DATE/8015A640.c.

## Acceptance criteria
Follow T-24010 scope and verification gates; preserve matching siblings, useful drafts and concrete negative evidence. Review before Done.

## Comments
Assigned isolated worker; no other C files claimed.

2026-10-17: Docker source-only probes found exact-frame near misses for both dispatchers; details in [[date-family-results]]. No production changes or verified bytes yet. The small body improved to 11 isolated resolved +/- rows; the large to 72. Return-view probes unchanged. T-17208 already establishes that func_8015A640 is hand-written libgte assembly, so no repeated compiler sweep was attempted.

2026-10-17 checkpoint: retained both improved source-only dispatcher drafts and safe diagnostic/stage harnesses under `drafts/T-24011/`. Full-unit resolved diagnostics: small 48 positional +/- rows, large 72; isolated small 11 uses edit alignment and terminal padding. Astra follow-up unchanged; observed small constant hoisting happens after the cc-S stage during as1 optimization, with no proof of an assembler-version mismatch. Production src/include unchanged. Docker validation: DATE checksum OK; headers OK; globals OK; branch check OK (0 new, 15 known); default resolved linked fallback checks MATCH 1100/3200/416 bytes. Zero new C bytes. Required two-axis code review remains with the coordinator; status In Review.

2026-10-17 review correction: Standards review found P2 in the diagnostic harnesses: they ignored funcdiff process status and could print zero rows after an objdump failure or traceback. Both now share explicit status/verdict validation before reporting: exit 1 requires a named resolved DIFF with instruction rows; exit 0 requires a named MATCH; other statuses, incomplete verdicts, and exception diagnostics fail without a score. Docker failure-contract tests pass (5 tests, 12 cases); real full-unit harness remains 48/72 and stage harness passes twice consecutively at 11. Rerunning stage also exposed IDO rejecting an existing listing supplied via -o; the unnecessary -o flag was removed while retaining ignored working storage. Source bodies and nonmatching status are unchanged. P2 addressed; ticket remains In Review pending coordinator re-review.

Final independent review: Spec PASS (0 findings) for archive 4c5123f11...2f7bd397b. Standards initially reported P2 diagnostic subprocess-failure handling; fix 5c14a3c40 adds strict status/verdict validation and failure-path tests. Re-review of 2f7bd397b...5c14a3c40: Standards PASS, P2 resolved with 0 new findings; Spec PASS, 0 findings. Docker tests: five tests / twelve cases pass; full-unit diagnostics remain 48/72; stage reproducer runs twice at eleven isolated edit rows. No production source/header/C-body changes. Bounded pass and review complete; all three targets remain pending assembly, zero new verified C bytes.
