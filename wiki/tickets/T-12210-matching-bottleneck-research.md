---
id: T-12210
title: Identify and test high-leverage matching breakthroughs
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[acceleration-audit]]", "[[uopt-register-allocation]]"]
---

## Goal

Increase verified decompilation progress through research that unlocks groups of remaining functions. Measure the remaining bytes and repeated blockers, then test the best feasible hypothesis. Tool microbenchmarks alone do not satisfy this ticket.

## Acceptance criteria

- [x] Measure remaining-function and byte concentration, draft coverage, and reuse opportunities.
- [x] Distinguish observed compiler gaps from source/type/layout failures and stale classifications.
- [x] Run at least one concrete experiment against the leading opportunity; record verified gains or a useful negative result.
- [x] Provide evidence-ranked research priorities and a next experiment for each major unresolved uncertainty.
- [x] Review any changes against CODING_STANDARDS.md before Done.

## Notes

Reuse isolated /private/tmp/tokimemo-acceleration. Preserve cx1-cx10, w1, bm and TT ownership; main read-only. One CPU per research container, no heavy compiler search by default.

## Comments

2026-10-15 experiment: register-canonical and register-erased whole-function shapes each found only eight matched/unmatched targets (5,808 queue bytes), all already in the duplicate report. This bounded relaxation found no additional family; it does not test instruction reordering or partial/basic-block reuse.

2026-10-15 matching result: copied BUNKASAI func_801456E0 -> func_8014FFE4 and func_80145C28 -> func_8015052C through dupes.plan_copy, then mapped constants using instruction correspondence. Automatic near-copy rejected character ID 7 because the same literal appears in `case 7:`; retained state labels while changing assigned/passed character IDs to 6. Changed corresponding script/text/sector constants. Both passed literal linked funcdiff: 592 + 920 = 1,512 bytes; BUNKASAI full SHA1 passed before and after. No FAKE idioms, new declarations, flags, or source in a cx-owned file. Proof: build/acceleration/twin-proof.txt. Remaining siblings and role-aware substitution are a testable follow-up, not assumed gains.

2026-10-15 third match: BUNKAKEN func_801386C0 from func_801475CC, 840 linked bytes. Character-ID 6 -> 4, preserve `case 6:`. BUNKAKEN SHA1 and literal funcdiff pass. Total 3 matches / 2,352 linked bytes. Headers, globals, and prototype branch check pass (0 new / 15 known).

2026-10-15 synthesis: [[matching-breakthroughs]] records the baseline, different size conventions, experiment limits, and five research priorities. The default under-600-byte bulk queue covers only 21.8% of remaining bytes; 600–1,999-byte functions hold 59.2%. R/V/U flags also appear in 457 matched controls, so they do not prove compiler impossibility. Family-copy experiment succeeded; register-relaxed whole-function retrieval found no new leads. All investigation criteria complete; moved to In Review pending standards and requirements checks.

2026-10-15 review complete: independent standards and requirements reviews of 47bdf34e...9f34527f both passed with no actionable findings. Reviewers inspected sibling source, literal proof and checksum logs, scope, byte conventions, and uncertainty boundaries; no builds were independently rerun. All criteria satisfied; moved to Done. Work remains on the isolated branch without merge or push. Larger research proposals are not marked achieved by this ticket's completion.
