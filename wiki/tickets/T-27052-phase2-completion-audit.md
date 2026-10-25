---
id: T-27052
title: Phase 2 completion and header-split design
status: Done
assignee: parallel-chat
created: 2026-10-24
updated: 2026-10-25
links: ["[[phase2]]", "[[tickets/T-27000-phase-2]]"]
---

## Goal

Audit actual remaining acceptance gates, proposal coverage and header/file split prerequisites; produce concrete port-first closeout work, no source changes.

## Acceptance criteria

- [x] Source-backed outputs with exact coverage and open work; no false completion claims.
- [x] Reconciled names respect applied ledgers and ADR ownership, Docker dry runs where relevant.
- [x] Independent review of prepared executable batches before application (no executable batch produced; both audit/design artifacts independently reviewed).
- [x] No conflicting source/shared-file edits; primary chat applies/commits and maintains shared wiki.
## Comments

2026-10-24: User explicitly authorized fan-out across multiple chats. Shared-checkout worker edits are limited to assigned tables/specs and this ticket; root serializes applications, builds, indexes, board and log.

2026-10-24 audit checkpoint: authored `wiki/spec/phase2-completion-audit.md`; header ownership plan is
being prepared at `wiki/spec/header-split-plan.md`. All 28 base unit tables/specs exist, but the audit
finds 11,013 proposal rows, only five applied ledgers (780 current rows), extensive in-scope source
residue and explicit partial rule coverage. Ordinary table planning cannot directly consume 2,007
const, 331 comment or 20 `function` rows. The current enum still calls 0x11 prologue while NAME_ENT
evidence identifies 0x11 attract opening / 0x15 voiced prologue; a prefix-only batch does not fix that
semantic conflict. O.BIN comments, essential literals/roles, runtime confirmation, file/header moves
and final layer-aware acceptance remain open. Independent Sol 6.1 review is underway. No source,
symbol, ledger, tool, build or shared-wiki mutation was made by this lane; root owns registration and
log/status/board updates.

2026-10-24: Parallel chat 01a16f17-1d7e-75b0-9108-f8600a581705 dispatched. No source application or shared wiki mutation permitted; primary chat integrates reviewed outputs.

2026-10-24 handoff: `wiki/spec/phase2-completion-audit.md` and `wiki/spec/header-split-plan.md` are
complete and independently reviewed by Sol 6.1 with no unresolved findings. Docker rechecks reproduced
11,013 base proposal rows, all 28 unit pairs, all 34 main objects/26 overlays, 780 applied ledger rows,
and the header inventory (73 shared typedefs; main_api 835 function/1,230 object declarations; 437 C
closures). Read-only `check_headers.py include src` passed. The review corrected TaiikuBig from a
sports-only owner to neutral shared records plus main/view declarations, and refreshed the audit's
mode-proposal paragraph after root's semantic correction. No apply/build/clean/commit or new checksum
claim. The design identifies exact tools that fail open or hardcode umbrellas, dependency-ordered
splits and seven closeout gates; phase 2 remains incomplete. Root must register both pages, append
the shared log and synchronize ticket/board status (this worker leaves In Progress to avoid a
kanban mismatch). The audit/design task is ready for root's final status transition; actual naming,
header migration, runtime/TT decision gates and final phase-2 review remain implementation work.

2026-10-24 user-priority extension: the human explicitly prioritized logical C filenames and a logical
header breakdown. Updated the two assigned plans with a reviewable first migration:
`src/main/8006CB30.c` -> `src/main/scheduler.c` (text `[0x8006CB30,0x800737A0)`), and
`src/main/800737A0.c` -> `src/main/school_day.c` (`[0x800737A0,0x80075320)`). The audit now gives
all 34 existing main-file logical-path candidates with exact text intervals/evidence; the header plan
gives exact code/rodata/data VRAM and ROM spans, 63 existing declarations assigned to
`main/schedule.h`, `main/day_event.h` and `main/school_day.h`, complete 75-definition accounting,
consumer/include edges, preserved six GYOZI compatibility `()` entries and 34 intentional absent
shared prototypes. First deliver the two names, then bounded owner-aware tooling and the three header
leaves; no full 73-type split prerequisite. Umbrella deletion remains a separate guarded end gate.

Read-only Docker `rename.py plan` of ignored `audit27052-main-file-pilot.tsv` passed: two rows, six
affected paths, two YAML names per row, no asm strings/config keys, one KANGEI comment reference.
Independent Sol 6.1 re-review reproduced the intervals/plan and exact header declaration set; no
unresolved findings. T-27043's corrected GameMode names are active root integration, not an unresolved
naming decision. This extension also made no source/header/tool/ledger edits or builds. Root still
owns application, shared registration/log and synchronized ticket/board transition.

## Final integration evidence (2026-10-25)

Reviewed source digest: `687b5ce789d59f154b527264d23227b741a08bc619e4a1f8c67d617257f4c4f3`. Final clean matching proof: 27/27 binaries, 6,952 functions, 2,276,920 bytes. Independent original Astra review and separate fixer Astra review passed this exact source digest. See `.claude/orchestrator/state/T-27000/final-tracking/criterion-evidence.md`.

