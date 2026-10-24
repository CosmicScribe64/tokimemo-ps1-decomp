---
id: T-27068
title: Reuse proven constants at precise literal sites
status: In Progress
assignee: constant-tooling
created: 2026-10-24
updated: 2026-10-24
links: ["[[rename-tool]]", "[[tickets/T-27067-core-scheduler-essential-completion]]"]
---

## Goal

Allow reviewed literal replacement to reuse an existing matching enum value and select only proven expression sites. The scheduler baseball-club hunk currently exposes duplicate-member rejection and broad member-site ambiguity. Preserve proof/rollback/ledger behavior; do not replace girl7 or other unexplained values.

## Acceptance criteria

- [ ] Existing-name/value/type ownership validated, mismatches/collisions rejected.
- [ ] Exact reviewed semantic sites supported without blanket value replacement.
- [ ] Meaningful synthetic regression tests and independent Standards/Spec reviews resolved.
- [ ] Root integrates reviewed tooling and then applies proven sites with matching proof.
