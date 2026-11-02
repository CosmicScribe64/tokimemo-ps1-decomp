---
id: T-39516
title: Translate the omitted festival gate plates
status: Done
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39508-english-art-on-disc]]", "[[tickets/T-39510-no-japanese-left-check]]"]
---

## Goal

Replace the Japanese school-name plates beside the translated standing banners in both festival gate backgrounds, preserving the surrounding artwork.

## Acceptance criteria

- [x] Inspect decoded D_01_47 and D_01_48 and register replacements for every omitted gate plate.
- [x] Generated replacement crops contain only newly drawn artwork; raw sources remain unchanged.
- [x] Rebuilt disc readback and actual festival GPU captures show English banners and plates.
- [x] Relevant artwork tests and code review pass; inventory and wiki describe the complete coverage.

## Evidence

`build/emu/sports-arena-proof/shot_500.png` on target 32fb03da865a visibly shows Japanese on the right gate pillar, beside the English sports banner. Catalog rows 133 and 134 mention both the standing banners and the gate pillars, but the existing `translation_bs_signs.py` specifications replace only the standing banners. Neither gate plate is an owner-approved exception. The culture-festival counterpart needs decoded-pixel confirmation.

## Progress

Decoded both source backgrounds and confirmed their gate plates share the same geometry. Added `tools/patch/art/festival.py`: exact-source SHA256 guards, a synthetic plate face fitted from clean light samples, newly drawn five-pixel English capitals, and RGBA crops with unchanged pixels transparent. The crop covers only the inner 46x20 plate face; surrounding stone stays untouched. Both generated crops enter the standard still encoder beside the existing standing-banner crop. Thirty artwork tests pass, including old-ink erasure, border preservation, geometry and source rejection. Decoded previews read PRIVATE / KIRAMEKI / HIGH SCHOOL. Full `build/festival-plates-proof` is running; encoded-disc/GPU verification remains pending.

Full build/readback passes: `build/festival-plates-proof`, target prefix cfa3e93df1f4, 248 verified artwork items. The frozen main linked outputs are byte-identical to `exam-arena-proof`; actual-disc comparison finds only the two intended festival backgrounds changed. Inventory regeneration/check passes. Both sports and culture festival GPU probes are running on the rebuilt disc.

Both GPU probes completed 1,000 frames with zero original-font calls/unknown/overflow. Visually reviewed `build/emu/festival-plates-sports/shot_800.png` and `build/emu/festival-plates-culture/shot_800.png`: complete English banners, PRIVATE / KIRAMEKI / HIGH SCHOOL plates, preserved borders and readable event dialogue. The probes log their scheduler entry injections; they are controlled scene coverage, not normal career traversal.

## Review

2026-11-02 Codex: reviewed against CODING_STANDARDS.md. No game C, SDK declarations, existing names or raw sources changed. Source hashes reject mismatched inputs; dimensions and lettering bounds are explicit; face reconstruction uses a fitted plane, not copied extracted pixels. Only changed RGBA pixels are emitted, all generated media stays ignored, and the standard encoder/readback checks are reused. Tests cover source rejection, geometry, complete old-ink erasure and unchanged borders; actual-disc comparison limits changes to the two intended files. GPU evidence verifies the composed result. Inventory retention prose is corrected without falsely recording human approval. No unresolved findings.
