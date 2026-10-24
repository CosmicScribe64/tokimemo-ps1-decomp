---
id: T-28005
title: Cross-check text and image-text coverage against the fan patch
status: Done
assignee: reference-crosscheck
created: 2026-10-24
updated: 2026-10-24
links: ["[[ref-english-patch]]", "[[assets-text]]", "[[assets-media]]", "[[tickets/T-28001-text-script-export]]"]
---

## Goal

Use gothamor/tokimeki-memorial-english-patch as an independent metadata/layout reference for original text coverage, runtime patch cases and image lettering. Verify useful observations against original inputs; facts only, no copied code/prose/translations committed. Existing reference ingestion covered internals notes rather than this whole pipeline audit.

## Acceptance criteria

- [x] Compare script/source file+offset inventories and control/substitution/root layouts with ours.
- [x] Identify runtime-write/indirect-table cases and lettering targets relevant to current gaps, with source links and confidence.
- [x] Report concrete missing/already-covered/unproven items to owning text/media lanes; author completeness claims are not proof.
- [x] Independent reference findings reviewed and relevant wiki/log updated before Done.

## Result and review

Reference pin and findings are recorded in [[ref-english-patch]]. Independent review repeated
the per-file offset, file-set and load-base comparisons and found no blocker. Corrected the
commit/tree terminology; full-tree numeric inventories remain author-inspected leads rather
than independently verified completeness metrics. Evidence is ignored
`.claude/orchestrator/state/T-28005/crosscheck.md` and `review.md`.

Both asset owners received the handoff. The 633 local-only offsets, static New Year sites and
image-region leads remain follow-up work in T-28001/T-28004. This bounded reference audit is
complete; it does not close comprehensive text or image-lettering coverage. No third-party
code, translated text, images or copyrighted assets were copied into tracked files.
