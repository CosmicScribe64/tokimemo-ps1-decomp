---
id: T-24517
title: Reconstruct MASTER animation flow and pointer lifetimes
status: In Progress
assignee: Codex
created: 2026-10-20
updated: 2026-10-20
links: ["[[tickets/T-24514-master]]", "[[families/master-family-results]]"]
---

## Goal

Deep region-by-region reconstruction of MASTER func_801363F4, preserving target-specific flow and recovering clone/point-loop lifetimes.

## Acceptance criteria

- Any claimed match requires literal default C, linked bytes, all27 checksums, interface guards and standards/spec self-review.
- Otherwise isolate a concrete source or backend reproducer; no compiler-impossibility inference from a few variants.

## Notes

Base c768eda884, isolated /private/tmp/tokimemo-t24517, branch codex/t24517-master-flow. MASTER-only ownership. Existing T24514 candidate has original64-byte frame and first213 instructions exact; copy-end temporary, clone pointer comparison and spill homes remain. T24092 already fixes the historical unaligned-copy F/B direction issue; this lane does not repeat that fix.

## Comments

- Read index first, current status/playbook/standards/local guidance, function catalog, T24514 source/handoff and MASTER family history. Root explicitly requests deeper structural reconstruction, not tiny type grids. No agents, main_api edits, inline assembly, compiler patches or unrelated ownership.

- Structural evidence: target reuses clone spill sp34 as later speed spill sp34; point pointer uses sp30. All-indexed source restores original clone sltu/bnez compare but initially has +4 spill homes and653 instructions. A real RNG result local restores call-before-point-y-store order, all original spill homes,64-byte frame and652 instructions; aligned score330 (T24514 baseline342). Original first213 instructions still exact. Generic work-pointer reuse438 is merely a negative source control, not a backend conclusion.

## Structural checkpoint

`drafts/T-24517/func_801363F4.c` preserves the all-indexed/value source before further register work. `drafts/T-24517/checkpoint.py` recompiles the full translation unit privately, preserves relocation annotations, and reports652 positions,213 exact diagnostic prefix rows,167 register-only substitutions, and0 other rows. Score330 is the aligned full-function diagnostic cost. Section-owned strings/tables remain funcdiff-normalized, so this is not literal linked-byte equality or a C match.

The original flow is recovered independently of the register question: clone sltu/bnez comparison, call/store order, frame64, and clone/speed spill34 with point spill30. The first copy-end register t2/t3 does not alone establish a compiler defect; investigate uopt assigned ranges versus ugen temporary choice separately. Production remains unchanged. Ticket stays In Progress for the deeper reconstruction requested by root.

- Opcode-aware checkpoint correction from root review: REGIMM rt selector is now retained; COP formats are asserted absent, and syscall/break words retain their code bits. Existing-object recheck confirms neither REGIMM nor COP occurs here and preserves167 register-only rows/0 other rows. No compile repeated for this check.
- Exact-candidate tracer fidelity: stock and the existing read-only traced uopt produce identical `.text`, `.rel.text`, `.rodata`, `.rel.rodata` sections. Traced target equals checkpoint diagnostic keys. All final uopt assigned ranges use only v0/v1/a0/a1 (55 uncoloured,9 coloured); no t0..t9 colour is assigned. First copy-end t3 has no trace range. Remaining t-register differences therefore need ugen/assembler temporary attribution rather than uopt allocator-priority grids; this does not by itself prove a compiler defect.

## Root consultation packet

`drafts/T-24517/README.md` records exact commands, narrow candidate, synthetic aligned68-byte copy reproducer, stock/traced fidelity and enabled/omitted-shim fidelity. Omitting only the ugen shim in a private diagnostic process changes none of the complete text/data/relocation sections and leaves330 score/652 rows; production tools unchanged. T24092's prior6337-function audit was read, not repeated.

Synthetic direct byte clear gives source t7/end t0 (+3 ring); source-equivalent integer-address byte clear gives source t8/end t0 (+2 ring). Full target wants source t0/end t2, while current source yields t0/end t3. The integer-address source changes preceding machine address setup, so it is not a solution. First unresolved step is zero-based row213 at target80136748. Root question: preserve the current preceding address setup/copy bases while obtaining source+2, or identify a concrete ugen input/state distinction. Relative ring choice alone proves neither a compiler defect nor source impossibility.

Archive self-review: only compiling narrow C, synthetic C and replay/diagnostic Python plus concise prose are retained; no raw binary, ucode, original ASM or full context. Replays use current source/header context. FAKE marker explains the synthetic address view. Python scripts pass Docker py_compile; full-target and synthetic C were compiled in Docker; diagnostic corrected-mask check used existing objects. src/include/tools/config diff is empty. Prose review found only a weak passive construction, which was tightened. Ticket remains In Progress while root considers this specific boundary.

## Later-region lane

Root integrated the consultation packet as036407cd5 and authorized independent later-region mapping while Astra handles the first copy. Preserve the original packet tree unchanged. New isolated /private/tmp/tokimemo-t24517-later, branch codex/t24517-master-later, base036407cd5. Read current index/status/playbook/standards and append future log entries after rotation; do not restore old log entries. Begin with read-only ring-delta and call-boundary mapping from existing packet objects; no first-copy compiler probe.

- Read-only later363F4 map: cases6..10 have119 differing rows and152 differing register fields. All follow one bijection t1->t3,t2->t4,t3->t5,t4->t6,t5->t7,t6->t8,t7->t2,t8->t1, unchanged across observed calls. No independent later-region issue is established; arbitrary global register remapping is not a fix. Root Astra's isolated integer-copy-operand hypothesis was negative (651 positions/cost403, bases/prefix disturbed), without a compiler-impossibility claim.
- Root authorizes independent MASTER326B4 structural work under this continuing MASTER flow ticket. Read its function catalog and T22015 history before probes. Start with an original-region map and exact load/branch differences; do not repeat old callee/type grids. Original packet tree/source remain unchanged; later tree base036407cd5.

## Independent326B4 structural checkpoint

Fresh MASTER baseline checksum passes on036407cd5. T22015 clean body538/832 candidate positions versus816 target; unsupported fadeout argument removal492/830 and established wide girl-call view490/830 correct two stale-source issues without shared header edits. A packed u16 bitfield mode view is identical to492/830 and not retained.

`drafts/T-24517/later/func_801326B4.c` reconstructs the raw case4 selector lifetime and reloads at observed call boundaries, including unconditional reload after case3's CD-ready branch. This gives190/816,430 exact diagnostic prefix positions,83 register-only rows and19 actual opcode/immediate/placement differences under corrected masks. Remaining first difference is inner-switch extraction t8 versus v1 at430; candidate CSE moves shifted selector materialization into reload-path branch slots and before the join's zero store. Later packed-word loads retain their original lw/sll20/srl29 shape. See the narrow README region map/replay. This is diagnostic, not linked proof or a C match. No production files change; ticket stays In Progress. Standards/spec self-review found no archive scope findings.

## Closest owned sibling35FB8

Matched33F80 twin baseline58/271. Indexed zero-initialization recovers original pointer comparison and defers saved s2 base until shuffle row109, scoring48. Using the existing pointer for the shuffle avoids an unused local. Equivalent threshold reassociation shifted_stat-(0x140-club_score) restores original first-sum and downstream t7/t8 order:12/271,232 exact diagnostic prefix positions, six register-only rows232,233,235,236,240,243 in final fade v1(target)/v0(candidate). Target/candidate lengths271; no literal match claimed. Narrow source/replay35 preserved before final fade check; inherited scalar probability FAKE views remain marked. Two326B4 separation controls533/807 discard correct reloads and are rejected. No production edits.
