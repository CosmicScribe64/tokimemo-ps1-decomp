---
id: T-25022
title: MAIN src/main/8004F870.c reload-boundary closure
status: Done
assignee: Codex
created: 2026-10-22
updated: 2026-10-26
links: ["[[tickets/T-25021-main-5215c-reconstruction]]", "`plan-remaining`"]
---

## Goal

Continue only func_8005215C in src/main/8004F870.c from883760224 and the T-25021 draft. Restore mode reload after k_disp_inc, preserve wide arithmetic, and inspect other read boundaries before allocation changes.

## Acceptance criteria

- [ ] Whole-TU resolved C proof; assembly fallback excluded.
- [ ] Independent review and root integration checks pass before promotion.

## Notes

Baseline target315/candidate313 words, printed138 and aligned138 (insert/delete1, substitution2),64-byte frame and three exact spill homes. Work is isolated on codex/t25022-reload; root owns headers, integration and progress totals. No extra workers.

## Investigation checkpoint

Reproduced315/313words,138aligned/138printed baseline in Docker. Initial mode reload repairs one missing word:314words,127/127. A second target reload boundary was found across final input-handler calls. Corrected source preserved as drafts/T-25022/main/func_8005215C/body.c even though it is worse:310words,179/179. Partial lower-score source is explicitly diagnostic, not the semantic baseline. Both variants retain64-byte frame and three original homes.

Bounded source-role, byte-view and read-placement tests recorded in the draft README. No broad sweeps or new workers. Traced compiler output equals normal compiler output for the semantic checkpoint (310words, zero resolved differences). Earliest structural difference: mode comparison introduces a v1-to-v0 copy; final eager reloads sink below guards. Private raw evidence in build/T-25022. Direct mode mutation by inspected callees is not established; actual known writes and remaining transitive audio uncertainty are recorded rather than guessed.

Prepared at most three bounded consultation questions in the draft README. Still unmatched; ticket remains In Progress. No production source/shared-header/build-tool edits and no full-build coverage claim.

## Bounded consultation follow-up

One read-only Astra consultation, Sol idle, identified distinct raw signed/unsigned-byte/signed-word conversion webs and the signed diagnostic's target-like final reload topology. Sol tested its two justified probes: explicit signed-word zero-extended zero tests315/309words,198aligned/198printed; word-mask representation315/317words,262/262, with incorrect lb+andi remaining. Neither recovers eager unsigned read placement. Both source controls and consultation findings preserved in the draft packet. No justified third probe; no generic tuning followed. Semantic body.c remains310words,179/179; lower-score partial source remains diagnostic only. Ticket still In Progress, no match claimed.

2026-10-26: Closed during kanban cleanup after Phases 1, 2, and 3 completed (27/27 binaries byte-matching, Phase 2 source audit complete, and Phase 3 Godot port and asset packs verified).
