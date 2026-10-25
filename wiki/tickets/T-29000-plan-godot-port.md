---
id: T-29000
title: Plan the Godot port from the readable decompilation
status: In Progress
assignee: orchestrator
created: 2026-10-25
updated: 2026-10-25
links: ["[[phase2]]", "[[asset-export]]", "[[spec/index]]"]
---

## Goal

Mark completed Phases 1 and 2 accurately in the roadmap and have an Astra agent produce an actionable Phase 3 Godot port plan, grounded in the finished source and existing asset pipeline.

## Acceptance criteria

- [x] ROADMAP.md records Phases 1 and 2 complete with evidence, distinguishing ongoing asset work.
- [ ] Astra authors a source-backed port plan with architecture, original-game import, fidelity boundaries, staged playable milestones, dependencies, validation and unresolved decisions.
- [ ] Asset gaps are assigned to the milestones that actually need them; unrelated extraction/context work does not prevent beginning a proven slice.
- [ ] Japanese-first behavior, later unrestricted translation layout, original-data ownership and a preserved matching decompilation remain explicit.
- [ ] A first implementation slice has concrete files, acceptance checks and agent ownership; independent review findings are resolved before Done.
- [ ] Wiki index/status/board/log and roadmap agree; no unrelated source, pending asset work or game data is changed.

## Notes

User request on 2026-10-25 authorizes Godot port planning now, superseding the earlier prohibition on direct Godot work for this planning scope. Existing engine-independent asset work continues separately. Root serializes tracked writes, staging and commits; Astra prepares its plan in ignored coordination storage for root integration. The choice of implementation language, Godot release and runtime architecture must be justified against actual source and official engine documentation, rather than guessed.

## Comments

2026-10-25: created before planning. Phase 1 completed 2026-10-24; Phase 2 source and final bookkeeping were pushed at 182af9d13/d7fb33f96 after the requested Astra review/fix/verification and 27/27 matching proof. Pending asset tooling is outside phase acceptance.

2026-10-25: independent roadmap review PASS at ignored T-29000/roadmap-review.json. Phase 1 remains complete as of 2026-10-24, Phase 2 is explicitly complete as of 2026-10-25, and Phase 3 is planning. Astra is preparing the plan; the asset lanes continue separately. Prose audit found only established-title and game-title punctuation leads.
