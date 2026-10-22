---
id: T-25045
title: Match ETC ring-temporary tail
status: Done
assignee: Astra ETC worker
created: 2026-10-22
updated: 2026-10-22
links: ["[[tickets/T-25039-main-club-carrier]]", "[[playbook]]"]
---

## Goal

Match ETC func_80146D4C in src/ovl/ETC/80146400.c, exclusive source ownership. Root reassigned this scope after confirming the previous MAIN file is complete; no further checks of stale MAIN archives are needed. No shared-header changes without coordination. Parameter_up_down belongs to another worker.

## Acceptance criteria

- Literal default-C match and ETC checksum, declaration guards and independent review.
- No inline assembly, patched instructions, per-function compiler exceptions or semantic shortcuts.
- Root owns integration and full 27-binary gate; continue until verified match or concrete external blocker.

## Notes

Historical tail is 12 rows, with ugen temporary ring slots implicated. Read T-25007 and later evidence before measurements and phase-informed experiments. Initial selection inspected only the source's remaining guards and three stale archive ledgers; there were no production changes or new match claims. Private evidence from earlier assignments stays in /private/tmp/tokimemo-t25035.

## 2026-10-22 baseline and controls

Actual target is 264 instructions / 1056 bytes (old 1076-byte ticket figure is stale). Complete-TU baseline is 264/264, cost 12, six changed positions: index shift t2 versus t3, counter load t3 versus t4, increment result t4 versus t6. The early control-flow and grade chain match.

Late-zero terms applied jointly to the two final index uses retain or worsen the temporary mismatch and may change the address register; counter self-zero expressions promote the counter and add code. Late-zero terms at the preceding total-print argument or from the index in the increment remain baseline. Full-width byte/counter bitfield views also remain baseline. No production edits. Capturing unmodified ugen input/output next to determine which actual operations consume ring slots.

Unmodified ugen capture confirms the skips are absent before as1: final index multiply uses t2, counter load t3, result t4. Unsigned widening/empty late-zero branches retain this baseline; signed casts break the shared address CSE. A promoted byte self-assignment produces the target index t3 and counter t4/t6, but adds stores and a t2-to-a3 reload copy. This is phase evidence only; testing a local byte capture to avoid the unwanted global writes. All private probes remain under build/t25045.

### Allocation trace refinement

A logging-only recompiled ugen replay produces a `.G` byte-for-byte identical to the standard capture. It proves that only **one** additional allocation is needed before the final shift. The self-assignment's `and t2,a3,255` spends that slot; t5 is already absent from the subsequent free-list order, so load t4/add t6 follow automatically. Earlier descriptions of two independent missing temporaries were inference, now corrected. Narrow late-negative expressions give the exact first lbu/shift but lose address CSE; explicit shared-address/index controls investigate this boundary. Local dead masks/late-zero conditions fold away and preserve baseline. No production code changes.

## Match and verification

Transferred verified T-25047 alternating identities to the preceding total printf argument, where the late-zero carrier dies before the call. Final source reuses the old pad as `s32 zero`, assigns `g->unk_041 * 0`, and passes `(score + zero) | zero`. One intermediate consumes the missing ugen slot, as1 removes the operations, and the final address CSE stays unchanged. Either identity alone, literal zero or identities on the total sum retains cost12. Unsigned-negative and multiply-zero producers both match; multiply retained. FAKE annotated; no compiler/header changes.

Default funcdiff confirms1056 literal linked bytes with no assembly fallback. ETC checksum, headers, globals and prototype branch check pass (0 new / 15 known). Root independent standards/spec review found no issues: defined byte multiply, score identities, reused frame word, complete FAKE annotation and unchanged behavior. Full27 integration is root-owned. See drafts/T-25045/README.md for measured controls and private evidence paths.

Root acceptance2026-10-22: final standards/spec review no findings; independent default-C literal1056bytes, fresh27/27 binary checksums, headers/globals and prototypes0new/15known pass. Moved Done. Worker continues T-25054 func8014790C in the same source file.
