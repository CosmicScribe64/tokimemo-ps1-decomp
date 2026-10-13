---
id: T-10850
title: Wave 17 (astra, easiest first)
status: Done
assignee: Codex a17
created: 2026-10-13
updated: 2026-10-13
links: ["[[register-cheatsheet]]", "[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal

Match the 69 assigned functions in the explicit a17 ownership list, easiest first.

## Acceptance criteria

- [x] Every assigned function receives a draft, build and diff.
- [x] Deep attempts for near misses, with regdiff for register misses.
- [x] Four-hour working session, isolated to assigned source files.
- [x] Final build, prototype/global checks and inline standards review.

## Notes

Shared wave11 brief applies; no commits or tooling edits. Scratch: `scratchpad/a17/`.

## Comments

- Started 2026-10-13. Read register cheatsheet, wiki index, local instructions and standards. The malformed `file --files` and broad queue invocation are excluded from ownership.
- Baseline: fresh configure/ninja passed 27/27 SHA1 checks and header/global guards. Initial generation required disc preparation and splat; no raw data modified manually.
- First pass: 69/69 drafted and build/diff attempted. Errors remain for aggregate repair; no first-pass matches. Four inherited reset-helper return types corrected with generated declarations; a full 27/27 build proves caller compatibility. A 26-candidate bounded permutation pass is queued, each with a 300-second budget.
- Type repairs: current drafts use existing GameState/Rec24 views where available; copied aggregates recovered for GYOZI (0xCC3), SHOUGATU (23 callbacks), EN_NICHI (64 words). RPG_BAT owned scalar data definitions preserve its full checksum before function conversion. Further compilation/diff evidence is under `scratchpad/a17/attempts/`.
- Buildable coverage complete: all 69 functions have at least one successful compile and resolved diff. Four matches currently total 788 bytes (EVENT, GYOZI, KANGEI, SHOUGATU). TAIIKU's owned slot-array definition passes the overlay checksum and reduces its updater to a branch-form or two-load scheduling miss, depending on loop form. Additional narrow-local, declaration-order and expression-order trials are recorded per function.
- Fifth match: OPTION `func_80139878`, 148 bytes, brings the total to 936 bytes. Reviewed source retains a marked FAKE empty conditional to separate byte live ranges; simpler forms were tested and did not match. OPTION's complete checksum passed. Continuing the four-hour deep pass on remaining close drafts.

- Final inventory: 5 matches / 936 bytes; all 69 assigned functions compiled and diffed, plus two owned-file follow-ups. Scoped lenient dupes applied 0. Added 34 T-0018 case rows. Final 27/27 checksum build passed. Inline standards review is underway.

- Inline CODING_STANDARDS.md review complete: five literal-byte matches confirmed by function diffs and all 27 final binary SHA1 checks. New/updated nonmatches are ticketed and guarded; C89/fixed-width types, documented aggregate offsets, existing SDK declarations and placeholder names are preserved. All matching tricks have reasoned FAKE comments; unused matching-only scaffolding was tested for removal. Corrected menu indentation and TAIIKU blank-line debris, then reverified menu diff/regdiff and both affected binary checksums. No unresolved findings.
- Validation: headers OK; globals OK; sync_protos --check-branch reports 0 new disagreements (17 known); git diff --check clean. Scope audit: 36 tracked files plus this new ticket, only owned source/permitted declarations/wiki; no staged files, commits, raw/config/tool edits or host installs. 69/69 resolved build/diff coverage; 68 register-trace families (the remaining GYOZI table-copy function matched directly). All 34 new case identities resolve as open.
- Final progress: `grand total   5195/6958   968748/2279368  42.5%`. Added 5 matches / 936 bytes and 34 T-0018 rows. Working session lasted at least four hours (started 07:18:56 UTC; final review completed after 11:18:56 UTC).
- Remaining blockers: handwritten SDK rotation assembly, known ASPSX division expansion, and unavailable owned data ranges for shared-at helpers. No new repository-tool bug confirmed. An early scratch-harness concurrent-source false positive was invalidated, rerun under per-file locks, and excluded from all totals. Evidence remains in `scratchpad/a17/`. Changes intentionally uncommitted.
