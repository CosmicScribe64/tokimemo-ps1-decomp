---
id: T-25056
title: Resolve ENDING A2E8 scheduler residue
status: Done
assignee: Codex
created: 2026-10-22
updated: 2026-10-26
links: ["[[families/ending-family-results]]", "[[as1-scheduler-rule]]"]
---

## Goal

Qualify and match func_8013A2E8 in src/ovl/ENDING/801396E0.c with persistent causal compiler investigation.

## Acceptance criteria

- Literal default-C linked MATCH before promotion.
- All 27 checksums and declaration guards pass.
- Direct standards/spec review; no unresolved findings before Done.

## Notes

Exclusive source ownership: src/ovl/ENDING/801396E0.c. Isolated worktree /private/tmp/tokimemo-t24512, branch codex/t25056 from main633d95356; old codex/t24512-ending remains at71896dd66. No shared header or build-tool edits without coordinator approval. Docker tokimemo-decomp-arm64 only, at most two compiler jobs.

## Comments

- 2026-10-22: Read wiki index, current status/playbook, standards, local agent rules and function histories. Starting from the guarded source and previous pointer-lifetime/scheduler investigations; no new match claimed.
- Qualification: Docker T-24009 replay still gives baseline aligned cost12 and pointer-lifetime10. Neither is a literal match. Existing guarded source retained. Investigating current full-TU assembler trace before source controls.
- New print-only as1 evidence: first flag destination bb10 never reaches weight4 arms bb11/58. It spends the scanner's candidate-count budget on equal-weight later joins through bb319/320. In reconstructed as1 func_42aa04, L42acdc sets sp100 to10 when region+4==0,100 otherwise; L42ad20 increments sp104 for dominated candidate blocks, including those whose instructions all fail. The count cutoff is distinct from the weight cutoff. Next controls stage string/flag preloads using late-eliminated unsigned-false tests; no per-function build rule.
- Exact gate correction: func_42aa0c (not42aa04), labels42acdc/42ad20. The comparison is old_count >10, so11 dominated candidate blocks are examined; bb320 is candidate12 and stops the search. Natural-loop regions get100. Six late-false string/flag preload controls retain12 except pad-pointer unsigned-false10. Wrapping the flag chain in single-iteration late-false loops regresses508/564/1093; no candidate promoted. Preparing scratch budget diagnostic and a mask-lifetime control while compiler slot is yielded.
- Scratch budget counterfactual: default diagnostic equals stock target; raising the general threshold to100 on unchanged C/binasm yields literal linked MATCH4648bytes. This is not production C progress. Exact patch/replay live in drafts/T-25056; proprietary compiler source/binaries and captures stay ignored. Candidate11 is reached at old_count15 and arm58 at16; natural-loop region+4 is the header-block pointer. Pointer+second-mask controls both660. Source-only region/lifetime work continues.
- Narrow first-arm backedges: mulzero/unsigned-false668, immediate-andzero12. Trace proves early backedge removal before as1: unchanged threshold10/list,1164 candidate words, first mask t9->v1. Late-zero carrier682; carrier+unreachable identity710; flag-identity733. Identity carrier preserves a natural-loop region with threshold100 but leaves real branches/two zero moves/identity add,1170words. Flag load is hoisted outside loop. This rejects the simple zero-runtime region hypothesis; details archived.

### Checkpoint review (2026-10-22)

Applied local code-review skill's standards/spec axes directly, per coordinator instruction to avoid new agents. Standards: production src/include/tools/config diff empty; only ticket/wiki and source diagnostic scripts archived. Compiler reconstruction/binaries, binasm, target data, generated full C and traces remain ignored. Exact private compiler patch is reproduced by the hash-pinned script; diagnostic match is explicitly not production progress. Spec: source-only controls and causal phase evidence preserved, task remains In Progress; no candidate promoted and no completion claimed. Root's review finding about ambiguous stock-output wording in README resolved.

- Follow-up: conditional equivalent first-call pointer controls score692/691. Cached original IDO5.2 as1 has the same100/10 selection and dynamically reproduces all1164 stock5.3 candidate instructions, zero differences, on the identical captured input. Original binary hash and exact replay command archived. This rejects a simple5.2-to5.3 historical change. Raised-budget diagnostic preserves all15 sibling instruction bodies; trailing padding differs after A2E8 shrinks8bytes, while final sibling B640 independently MATCHes96bytes. No production change or new C match.
- Research decision prepared: no current source mechanism explains address-half separation without allocation or runtime changes. The archive now gives a staged original-assembler/candidate-trace/whole-corpus investigation for coordinator review, with no request to adopt the scratch threshold. Documentation now calls bit0x40 a control-transfer barrier rather than implying that the ENDING path contains an indirect jump. Direct standards/spec review of this follow-up preserves all scope and promotion restrictions; task remains In Progress.

2026-10-26: Closed during kanban cleanup after Phases 1, 2, and 3 completed (27/27 binaries byte-matching, Phase 2 source audit complete, and Phase 3 Godot port and asset packs verified).
