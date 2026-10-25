---
id: T-27074
title: Prepare logical overlay directory and header paths
status: In Progress
assignee: path-packet
created: 2026-10-24
updated: 2026-10-24
links: ["[[phase2]]", "[[spec/file-naming-overlays]]", "[[spec/header-split-plan]]"]
---

## Goal

Finish the accepted English module-path portion of phase 2 after all 437 C basenames
became logical. Prepare evidence-backed overlay directory/header rename requests and
their configuration/reference map, preserving original disc identities and boundaries.

## Acceptance criteria

- [ ] Every current overlay directory/header has a reviewed target or an explicit applicable exception.
- [ ] Targets follow the ADR/glossary and avoid claiming unproved unused status or game branding.
- [ ] Root applies supported requests only through rename.py after naming/header providers stabilize.
- [ ] Independent Standards/Spec review, path-mapped views and final matching proof pass before Done.

## Notes

Preparation occurs only in ignored scratch and this ticket. Root owns source applications,
shared wiki, Git and builds. TT's English folder remains subject to the existing contributor
agreement condition; its already adopted technical prefix and logical C basenames remain valid.

## Path packet preparation

Pinned to committed tree `3794d7e733a5ed0b8e0469bfef7c1ec6a855baa4`, the ignored packet in
`.claude/orchestrator/state/T-27074/` covers all 26 overlay yaml/header pairs and 403 already-logical
C basenames. `table.tsv` has 56 supported `rename.py` rows (27 `dir`, 29 `file include`), with the
same accepted ADR folder stem used for each corresponding header. Its SHA-256 is
`c79088a3997888d5c592ab4bbd089025d139565deba231d5ead398d54080c98f`. All 26 pinned yamls retain
their disc-name `name`, `basename`, target, asm and build identities.

The packet routes case-only DATE/date and ENDING/ending directory/header moves through neutral
two-step names because the path tool rejects case-insensitive collisions. TT's directory remains an
explicit contributor-agreement exception; its proposed `tt.h` header uses only the accepted technical
TT namespace. Confirm-status directory/header notes preserve `HYPOTHESIS:` in the candidate ledger
notes; `rename.py` does not propagate a hypothesis marker for `dir` or `file include` rows. EVENT and
GYOZI `*_unused` labels are hypotheses, not claims: no loader found does not prove unused status.
ETC's folder migration remains separate, and header declaration ownership/topic extraction remains
with T-27061.

Preparation ran in the `tokimemo-decomp-arm64` Docker image with 2 CPUs against pinned Git objects.
No rename plan, apply, build, or live C/header/config/tool/Git mutation was performed. The packet is
candidate metadata, not an application approval; independent path review and root final-state planning
remain open.

## Header owner profile path rewrite prerequisite

Follow-up tooling review found that `rename.py`'s header-path config scan omits the approved optional
`config/header_owners.json` profile introduced by T-27061. The ignored proposal
`.claude/orchestrator/state/T-27074/header-owner-rename-fix.patch` adds a path-field-only rewrite and
a two-step neutral-stage/case-only fixture. The proposal parses and validates the whole JSON, rejects
duplicate keys, and changes only top-level owner paths and required-type paths while retaining unknown
metadata and field order; an existing empty profile fails closed, while an absent profile is skipped.
Four targeted fixtures and the full rename test suite passed (130 tests);
`git apply --check` passed without applying the patch. No live tooling files were changed. Root
review/integration is still required before these path rows can safely update an installed owner
profile.

## Current-source integration review

The current preparation is based on committed `302996710`, after the full topic-header stack.
The original Astra reviewer approves the exact five-tool patch `f246ca735a46b9d251360823c19176eaab470d4ab13e4c52030a84af05726811`:
controlled registry/provider path rewrites, explicit provider-root selection, EVENT unit identity
after its directory move, directory-move diagnostic mapping and case-only transaction safety.
The two reviewed filesystem findings are corrected; independent zero-fuzz patch application
reproduces all five reviewed files.

The isolated 56-row application exposed a further local guard problem: macOS case-insensitive
path lookup reports the old DATE/ENDING/TT spelling as present after the case-only move.
The live checkout has not received the path packet. A Linux-only pass cannot replace the
required local guard; exact path-spelling checks and regression coverage must resolve this
before root applies the operation. The revised exact-spelling fix is independently approved in
patch `33a688df3fe19b55afa8007b788270eff6c7b9415646b84a305eb033ce6394ec`.
Root applied its five tool files and ran the full six-module Docker suite: **337 tests pass**.
Actual source paths remain unchanged pending the repaired macOS-bind guard and migration proof.
Evidence remains ignored under `T-27074/` and the original reviewer's
`T-27055/path-tool-review-in-progress.md`.
