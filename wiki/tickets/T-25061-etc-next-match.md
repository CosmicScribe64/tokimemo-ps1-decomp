---
id: T-25061
title: Match next ETC function
status: Done
assignee: Astra ETC worker
created: 2026-10-22
updated: 2026-10-26
links: ["[[tickets/T-25059-etc-ranking-generator]]", "[[tickets/T-24513-etc-matching]]", "[[tickets/T-24518-etc-flow]]", "[[playbook]]"]
---

## Goal

Qualify up to three remaining ETC candidates against current source and their latest specific ledgers, then claim one new source file before editing. Continue sustained causal matching until literal default-C equality or a concrete external blocker. Preserve the completed T-25059 pin and all private evidence. Root owns integration; no other source files, shared-header changes or new agents are authorized.

## Acceptance criteria

- Literal default-C equality, affected ETC checksum and declaration guards.
- Independent standards/spec review; artificial forms explicitly marked FAKE.
- No instruction patches, inline assembly, compiler exceptions or semantic shortcuts.
- Ticket and board remain In Review until root's independent integration gate.

## Activity

2026-10-22: Parent reserved this continuation after T-25059. The completed src/ovl/ETC/80146400.c has no remaining fallback guards. Created the ticket before new experiments and preserved pin dc0cedcbd8280cf82fd5c8a9aa6da52d3f9c3f04 on a new branch. Current root source still has six unmatched functions in four other ETC files; selection will compare at most three specific ledgers, without a generic sweep.

2026-10-22: Claimed func_80144224 in src/ovl/ETC/80143240.c before source edits; parent confirmed exclusive ownership. Compared its T-25002/T-24074 history with T-24518's initializer and T-24078's polygon renderer. This is the smallest concrete candidate: old T-25002 has 185/194 instructions, aligned cost 97, with frame-counter and index global promotion gaps. Current declarations retain the separate frame/selected-girl symbols and real fourteen-entry order table. The assembly body spans 780 bytes, followed by four alignment words; current resolved measurement includes those words and will be used consistently.

2026-10-22: Current full-TU baseline measures 199 target / 190 candidate words, 203 printed rows and aligned cost 175. Historical row counts are not the current baseline. Initial frame-counter load remains an unallocated temporary, while the order index occupies a3. Trace that priority/lifetime relationship and test targeted frame-read/view controls before changing arithmetic or table structure. Production remains assembly.

2026-10-26: Closed during kanban cleanup after Phases 1, 2, and 3 completed (27/27 binaries byte-matching, Phase 2 source audit complete, and Phase 3 Godot port and asset packs verified).
