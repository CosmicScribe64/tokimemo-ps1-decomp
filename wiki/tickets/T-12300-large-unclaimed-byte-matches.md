---
id: T-12300
title: Match substantial unclaimed functions
status: Done
assignee: Codex large-byte-matches
created: 2026-10-15
updated: 2026-10-17
links: ["[[decompile-workflow]]", "[[register-cheatsheet]]"]
---

## Goal

Prioritize verified matched bytes in substantial functions outside all existing object claims.

## Acceptance criteria

- [x] Establish exclusive object ownership before source edits.
- [x] Verify retained matches with literal linked bytes and unit SHA1.
- [x] Full 27-binary and prototype checks at handoff; ticket In Review and clean commits.

## Notes

Baseline 7a65b3d8. Main and all other worktrees are read-only. Exclude cx1–cx10 headers, w1/bm/TT, existing worktree changes and acceleration T-12230 manifest. Ownership census in build/large-bytes; four objects claimed below. Docker limited to one or two jobs.

## Comments

2026-10-15: Read index, standards, local instructions, workflow/register notes and external matching research. Acceleration has expanded to a 101-function broad sweep; its full source objects must be excluded, including unattempted siblings.

Owned source objects for this pass: `src/ovl/RPG_BAT/80145CC0.c`, `src/ovl/RPG_BAT/8014ADD0.c`, `src/ovl/RPG_BAT/8014A660.c`, `src/ovl/BUNKAKEN/8013BFE0.c`. Excluded RPG_BAT/80137280 despite census eligibility because T-9960 explicitly retains its claim. Also exclude BUNKASAI/801354B0 from acceleration's earlier cohort. Main remains untouched. Only canonical overlay declarations changed; shared main API untouched.

Baseline passed all 27 SHA1 checks. First default-m2c pass attempted eight targets / 10,676 list bytes; no matches. Exact differing-row scores: BUNKAKEN 8013C634 124, 8013CA6C 119, 8013C0AC 130; RPG_BAT 8014BB80 214, 8014B294 501, 8014ADD0 284, 8014A660 103, 80145CC0 195. Drafts outside checkout at /private/tmp/tokimemo-large-results. Scores include unnormalized old global names, so normalize aggregates before interpreting them as structural blockers.

User requested subagents. Delegated BUNKAKEN/8013BFE0 and RPG_BAT/80145CC0 to one compiler worker each; third worker reasons about RPG_BAT/8014ADD0 and 8014A660 while waiting for a compiler slot. Parent serializes headers, validation, commits and wiki. Maximum two compiler jobs across workers.

Verified BUNKAKEN gain: **3,604 literal linked bytes / 3 functions** (8013C0AC 1,416; 8013C634 1,080; 8013CA6C 1,108). Consistent initialized GameState pointer access plus existing CharFlags bit2 reads/stores solved this family. No FAKE needed. Full-file funcdiff literal proof and BUNKAKEN SHA1 passed. Handoff build/large-bytes/bunkaken/handoff.md. Two canonical overlay declarations added; no shared main API changes. RPG workers continue.

RPG_BAT func_80145CC0: **0 gained bytes**, guarded reconstruction of a 2,232-byte target retained because the entire control-flow/scheduling structure is recovered. 557/557 compared instructions; 58 register-only positions (116 +/- diff rows), D_8015EDE0 in v0 instead of v1. Removing m2c temporary/self-store noise and writing `(-D_8015EDE0) << 3` restores missing instructions. Traced uopt shows timer priority 0.6667 and no competing v1 range. Tested void/s32 fallthrough, bare return, local timer pointer, empty tests, postfix updates and boolean compare locals; none resolves the range. No speculative dummy statements retained, no FAKE added. Default RPG_BAT SHA1, headers/globals and whitespace checks pass. Evidence build/large-bytes/rpg-large/{handoff.txt,final.diff,regdiff.txt,checks.txt}.

Verified RPG_BAT func_8014A660: **1,316 literal linked bytes**, bringing total to **4,920 bytes / 4 functions**. Explicit nested final cases recover forward-branch structure; canonical `*(s32 *)&D_8015EDB0.unk_04` accesses then fix 31 register substitutions. Redundant same-type pointer views are transparently marked FAKE T-12300; plain members/direct scalar/local copies/s32-return versions did not match. Production literal proof and RPG_BAT SHA1 pass. Missing word counters declared from sw accesses; existing helper definitions provide prototypes. The same pointer/unsigned/array/record variants fail to transfer to func_80145CC0 (still 58 register-only positions); the clean guarded draft is retained without those casts.

2026-10-17: Reviewed at 28d5f509. The ticket's own two-axis review already fixed its one standards finding, and re-checking the merged code finds nothing more: the single FAKE in 8014A660.c names T-12300 with a reason, BUNKAKEN 8013BFE0.c is plain C, and the guarded func_80145CC0 was matched later by T-17101 (guard gone). Only trivial: redundant ticket-named #ifndef guards inside BUNKAKEN.h and RPG_BAT.h. func_8014BB80, func_8014ADD0 and func_8014B294 stay in the unmatched inventory. Moved to Done.

## Bounded pass outcomes

| Unit/function | Target bytes | Outcome |
|---|---:|---|
| BUNKAKEN func_8013C0AC | 1,416 | Literal linked MATCH |
| BUNKAKEN func_8013C634 | 1,080 | Literal linked MATCH |
| BUNKAKEN func_8013CA6C | 1,108 | Literal linked MATCH |
| RPG_BAT func_8014A660 | 1,316 | Literal linked MATCH; FAKE pointer-view annotation |
| RPG_BAT func_80145CC0 | 2,232 | Guarded; 58 register-only positions / 116 +/- rows |
| RPG_BAT func_8014BB80 | 1,124 | Scratch only; 214 -> 145 +/- rows after 24-byte menu aggregate/state view |
| RPG_BAT func_8014ADD0 | 1,212 | Scratch only; 284 -> 277 +/- rows; broader expression/aliasing differences |
| RPG_BAT func_8014B294 | 1,188 | Scratch only; initial exact score 501; not worth extending this pass |

No elapsed-throughput claim: this pass includes ownership audit, isolated baseline, source reasoning, failed attempts, and review. Four substantial matches add 4,920 bytes; the 2,232-byte near miss adds zero. Next substantial work in these owned files: the standalone-timer interference in 80145CC0, then the real menu-table type and remaining selector/reload structure in 8014BB80. Avoid repeating the rejected scalar-pointer variants on 80145CC0. Broader ADD0/B294 drafts remain outside production source.

## Final validation and review

Final source after review cleanup: fresh Docker ninja succeeds; independently rehashed **27/27 built binaries** against configured target SHA1 values. `check_headers.py` and `migrate_globals.py --check` pass. `sync_protos.py --check-branch`: **0 new disagreements, 15 known**, branch check OK. Literal default-build proofs: BUNKAKEN 1,416 + 1,080 + 1,108, RPG_BAT 1,316. Final progress **5,737/6,958 functions; 1,293,004/2,279,368 bytes (56.7%)**. Baseline was 5,733 functions / 1,288,084 bytes; delta 4 / 4,920.

Evidence: build/large-bytes/{setup.log,bunkaken-proof.txt,review-fix-build.txt,final-sha1.txt,final-guards.txt,rpg-match-proof.txt,progress-final.txt}; failed/near candidate scripts and drafts retained in ignored scratch and /private/tmp/tokimemo-large-results. Independent code-review skill: requirements axis no substantive findings; standards axis one unreachable break, removed and reverified with exact bytes and checksum. Standards follow-up confirms resolved; no other findings. Ticket remains In Review as requested. No merge/push/rebase/reset/stash; main unchanged.

Local branch `codex/large-byte-matches`. After final documentation commit, return the isolated checkout from temporary sandbox staging to its managed path `/Users/lila/.codex/worktrees/large-byte-matches/tokimemo-decomp` so the chat attachment remains usable. Source claims remain with this branch until integration/reassignment.
