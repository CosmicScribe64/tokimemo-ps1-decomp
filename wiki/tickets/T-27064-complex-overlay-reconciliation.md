---
id: T-27064
title: Reconcile EVENT TT RPG and TACO naming
status: In Progress
assignee: parallel-chat
created: 2026-10-24
updated: 2026-10-24
links: ["[[phase2]]", "[[tickets/T-27000-phase-2]]"]
---

## Goal

Prepare body-backed, independently reviewed executable symbol packets and complete port specs for o-event, o-tt, o-rpgbat, o-taco. Root alone applies sources and owns shared tracking/builds/commits. Existing draft rows are leads, not trusted evidence.

## Acceptance criteria

- [ ] Resolve current ledger precedence, source ownership and collisions; inventory every in-scope definition or justified hardware exception.
- [ ] Supported symbol-only tables plan cleanly in Docker and pass independent Standards/Spec review.
- [ ] Complete essential local/parameter/constants site inventory, source-backed transitions and persistent effects in specs.
- [ ] Root application, matching proof and final review before Done.

## Comments

2026-10-24: User authorized parallel chats, Luna for bounded work and Sol6.1 for harder analysis/review. No direct Godot work. No source apply/build/commit in this lane.

2026-10-24 reconciliation checkpoint: bounded Luna passes cover EVENT, RPG_BAT and TACO; the lane owner covers TT. Existing drafts contain unsupported `new:` type rows and offset pseudo-fields that cannot be passed to `rename.py`; these belong in explicit unresolved inventories. TT's old spec incorrectly presents the contributor prefix/folder gate as settled; TT remains a candidate and its current folder is retained. EVENT and main BSS at the same address have different ownership keys; numeric overlap alone does not make EVENT's own tables main-owned.

2026-10-24 validation resource checkpoint: concurrent Docker plans across chats are consuming substantial resources. The RPG helper reports exit 137 from its plan and no completed validation output. This lane is stopping its own obsolete duplicate plans and will freeze tables before a single final plan for each unit. No source application or matching proof is claimed. Root owns the shared log/board updates.
