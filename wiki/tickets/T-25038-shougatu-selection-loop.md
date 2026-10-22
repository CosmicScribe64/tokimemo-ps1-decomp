---
id: T-25038
title: Match SHOUGATU func_8013E6E4 selection loop
status: In Review
assignee: Astra implementation, Codex coordinator
created: 2026-10-22
updated: 2026-10-22
links: ["[[tickets/T-24523-seasonal-source-hypotheses]]", "[[tickets/T-25033-shougatu-canonical-call]]"]
---

## Goal

Match `SHOUGATU:func_8013E6E4` in `src/ovl/SHOUGATU/8013E330.c`, continuing the owned file after the verified E50C integration. Sustain implementation through a literal match or a concrete external blocker requiring human action.

## Acceptance criteria

- [x] Default C and independently linked function bytes match.
- [x] Affected overlay checksum and declaration guards pass.
- [ ] Standards review resolves all findings; root completes combined integration gate before Done.

## Notes

The isolated tree fast-forwarded cleanly from d091557e4 to root integration 77eae26cf. Prior work and private build evidence are preserved. E50C and GYOZI E0C are Done after root's all27 checks and 860-byte literal proof; do not repeat their matching work.

Selected E6E4 from the two authorized candidates: T-24523 preserves a corrected full-file body with resolved edit cost187 and exact local-table offsets. EA98's corrected typed port remained284 with copy/frame differences. E6E4 is948 bytes versus EA98's920 bytes, but has the closer, durable corrected body.

Read the exact negative ledger in `drafts/T-24523/SHOUGATU/func_8013E6E4/README.md` and the T-24523 ticket before experiments. Historical negatives: scalar-before-table layout214, sequential retry conditions187, integer debug-call view210 on the214 form, explicit stride/flag/status/dead-count reuse did not close187. The old ledger listed saved constant56, selector caching and downstream temporaries; the fresh baseline below supersedes its saved-constant claim.

Same Astra worker owns implementation and this file exclusively. No shared header/tool changes without root coordination. All compiler/analysis tooling runs in Docker; source archives contain no game instruction dumps. Root owns main integration.

## Comments

2026-10-22: Ticket and board created before experiments. Coordinator selected the durable closer body; Astra continues causal reconstruction without a time or probe-count stop rule.

2026-10-22: Fresh full-TU baseline with the canonical `check_para_limit()` and inherited E50C integer-847 view is187 printed/aligned lines,236/237 words. The old saved56 concern is absent: frame0x68, table homes0x50/0x40, count-loop entry and retry prefix through the debug call already match. Integer debug return fixes the first flag allocation (`v0` to `v1`), leaving177. A byte view at only the club6 selector restores that reload but leaves an unwanted `a0` selector range live to the status call. Adding a byte-view status argument removes that range and yields0/0,237/237 words. Private evidence: `build/t25038/`.

Declaration audit: existing `src/main/8007C030.c` and `src/ovl/DATE/8014B1F0.c` use `extern s32 D_80122D48`; the EVENT header's `FnTbl55` at the same address is an overlapping overlay view. Target stores literal3. Coordinator explicitly approved retaining the existing T-11590 local-extern exception, adding the overlay-owned `func_8013EE30(void)` forward declaration to SHOUGATU.h, and guarding the canonical debug declaration in main_api.h. Only this TU opts into the integer return; its canonical printf-like arguments remain unchanged.

## Verified closure

Default C has no assembly fallback. Both the private full-TU candidate and fresh production full-TU compile independently link to all 948 original bytes. A separate link of the actual Ninja-built object proves E6E4's 948 bytes and preserved E50C's 472 bytes. Source SHA256: `f78b646e687d2bc32c6d5329305ffbe490fc6435258d3f9af5698e247cb74954`.

Docker checks passed: `ninja build/ovl/SHOUGATU.ok` (whole-overlay SHA1), `tools/check_headers.py include src`, `tools/migrate_globals.py --check`, `tools/sync_protos.py --check-branch` (0 new / 15 known). Private proof JSONs are `build/t25038/{private-proof,production-proof,built-proof}/proof.json`.

Removal controls justify every retained FAKE: no club6 byte view costs174; no byte status argument costs177; no u8 result cast costs126 aligned; no random multiplication by one costs2; void debug return costs10. The byte views address the same unsigned byte and add no observable effects; multiplication by one and result narrowing preserve values. The ignored debug result changes allocation only. Canonical no-argument `check_para_limit()` is used.

Coordinator reviewed the source/header diff against CODING_STANDARDS and task scope with no findings. Ticket/card are In Review pending root's combined integration gate. No raw binary/assembly evidence is tracked; reusable findings and negative controls are in `drafts/T-25038/README.md`.
