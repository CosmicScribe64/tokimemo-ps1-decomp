---
id: T-24512
title: Match remaining ENDING functions
status: In Review
assignee: Codex
created: 2026-10-20
updated: 2026-10-20
links: ["[[families/ending-family-results]]"]
---

## Goal

Match remaining ENDING functions in isolated branch codex/t24512-ending.

## Acceptance criteria

- Literal default C with resolved linked-byte verification for any claimed match.
- Full 27 checksums, interface guards, and code review before Done.

## Notes

Baseline 23e1a20fb; source ownership src/ovl/ENDING and include/ovl/ENDING.h. Existing large drafts show scheduler and block-copy limits; start with bounded causal probes rather than repeating broad searches.

## Comments

- 2026-10-20: Read index, matching playbook, standards, function catalog and prior ENDING history. Created isolated /private/tmp/tokimemo-t24512; original disc mounted read-only in Docker.

- Initial Docker ENDING baseline checksum OK. T-23002/T-24009 A2E8 and T-24047 35F4C scheduling histories already cover the obvious hypotheses, so their guarded production source remains unchanged.
- 330E4: existing T-20930 draft rescored at aligned104. Moving four scalar declarations before arrays and removing redundant `n`, with a first-declared four-word frame pad, improves to84 and restores B0/table30/points58/rank78. Halfword spill remains56 versus54. Flag-first expression86; word-level86; narrow extracted-value locals u8/u16/s16 give127/126/131. No match; production restored. Pads are experimental FAKE devices, not recovered source.
- 387B0: repaired stale byte indexing to matched DisplayActor44 sibling views; signed flag148, unsigned177. No match, production unchanged. Next investigate 34270 using one current typed redraft, as current uniform ugen pass postdates its earlier archive.

## Self-review

Applied `.claude/skills/code-review/SKILL.md` standards/spec axes locally, per coordinator instruction that the explicit three-worker cap and self-review requirement override the skill's extra reviewer agents. No new agents used.

Standards: production source/header diff empty. Archived C is C89 and uses existing game types. Experimental frame pad is marked FAKE with reason and ticket. No tools/compiler changes, generated assembly, binary outputs or game data staged. Sources over30 diagnostic rows remain outside production. `git diff --check` passes.

Spec: bounded function-by-function causal hypotheses, current typed draft control, and exact negative evidence preserved. No new C match claimed; original guarded blockers remain unchanged. Full build/interface results pending below.

## Verification and handoff

- Docker full build completed; explicit manifest checks pass for the main executable and all26 overlay bins (27/27). First manual main check used the wrong extension and failed to open; corrected `.bin` path passes. No mismatch occurred.
- `check_headers.py`: headers OK; `migrate_globals.py --check`: globals OK; `sync_protos.py --check-branch`:0 new disagreements,15 known; branch check OK.
- Archived Python scripts pass Docker py_compile. `git diff --check` passes. Final src/include diff is empty.
- Moving research checkpoint to In Review for coordinator handoff. Matching objective remains unresolved; zero new C bytes. Avoid repeating the measured negative controls. No linked C MATCH claim is made, because no candidate reached equality.

- Archive follow-up self-review: generated34270 context.c merely copied the existing translation-unit siblings; none of the reproduction scripts reads it. Removed that unnecessary full context and empty declarations.h. Retained the compiling nonmatching body and score/provenance JSON only; no original ASM dump is present. Ticket stays In Review pending root acceptance.

- Retained-body Docker compile review after context removal:330E4 candidate84;34270 retained body504. The redraft command's reported typed890 is its initial scorer result; the retained finish body is a different postprocessed candidate. Both compile, neither matches. Stale context_hash fields removed in result/catalog and replaced with exact baseline/source-path references.
