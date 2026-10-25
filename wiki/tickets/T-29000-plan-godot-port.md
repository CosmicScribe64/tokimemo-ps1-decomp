---
id: T-29000
title: Plan the Godot port from the readable decompilation
status: Done
assignee: orchestrator
created: 2026-10-25
updated: 2026-10-25
links: ["[[phase3]]", "[[phase2]]", "[[asset-export]]", "[[spec/index]]"]
---

## Goal

Mark completed Phases 1 and 2 accurately in the roadmap and have an Astra agent produce an actionable Phase 3 Godot port plan, grounded in the finished source and existing asset pipeline.

## Acceptance criteria

- [x] ROADMAP.md records Phases 1 and 2 complete with evidence, distinguishing ongoing asset work.
- [x] Astra authors a source-backed port plan with architecture, original-game import, fidelity boundaries, staged playable milestones, dependencies, validation and unresolved decisions.
- [x] Asset gaps are assigned to the milestones that actually need them; unrelated extraction/context work does not prevent beginning a proven slice.
- [x] Japanese-first behavior, later unrestricted translation layout, original-data ownership and a preserved matching decompilation remain explicit.
- [x] A first implementation slice has concrete files, acceptance checks and agent ownership; independent review findings are resolved before Done.
- [x] Wiki index/status/board/log and roadmap agree; no unrelated source, pending asset work or game data is changed.

## Notes

User request on 2026-10-25 authorizes Godot port planning now, superseding the earlier prohibition on direct Godot work for this planning scope. Existing engine-independent asset work continues separately. Root serializes tracked writes, staging and commits; Astra prepares its plan in ignored coordination storage for root integration. The choice of implementation language, Godot release and runtime architecture must be justified against actual source and official engine documentation, rather than guessed.

## Comments

2026-10-25: created before planning. Phase 1 completed 2026-10-24; Phase 2 source and final bookkeeping were pushed at 182af9d13/d7fb33f96 after the requested Astra review/fix/verification and 27/27 matching proof. Pending asset tooling is outside phase acceptance.

2026-10-25: independent roadmap review PASS at ignored T-29000/roadmap-review.json. Phase 1 remains complete as of 2026-10-24, Phase 2 is explicitly complete as of 2026-10-25, and Phase 3 is planning. Astra is preparing the plan; the asset lanes continue separately. Prose audit found only established-title and game-title punctuation leads.

2026-10-25: integrated the frozen Astra plan (SHA256 99fb1c4a763307f7b9c109cad9d7b92427a1765a28f3289be9043fd506473c12) into [[phase3]] with six implementation tickets. Independent Standards/actionability PASS and spec/architecture PASS; the one minor applied-name correction is resolved as boot_game_data_init. Final integrated publication review is pending; this ticket is In Review, not a claim of implemented Phase 3.

2026-10-25: final integrated Standards/actionability and source/spec reviews PASS, bound to canonical wiki plan SHA256 ed57b399ecca034a920f6ee6531810c60ed894dff95dffa6bb9de9f8f8802325. The applied initializer correction is verified and no findings remain. T-29000 is Done as a planning deliverable; Phase 3 implementation remains planned, with three Ready and three dependent Backlog tickets. No engine build/runtime or decompilation-source change is claimed.
