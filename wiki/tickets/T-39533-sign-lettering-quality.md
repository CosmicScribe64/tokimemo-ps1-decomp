---
id: T-39533
title: Improve shoe-rental and class-roster lettering
status: Ready
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39508-english-art-on-disc]]"]
---

## Goal

Improve the cramped lettering found during the bulk background review without changing the translated wording or surrounding art.

## Acceptance criteria

- [ ] Give SHOE RENTAL in I_28_02 clearer letter separation at native resolution.
- [ ] Review the heavy, condensed roster names in P_01_03 and P_01_03B against the original sign style and revise the lettering.
- [ ] Inspect encoded results and actual in-game captures, including the roster animation.
- [ ] Pass existing art size, decode and disc-readback checks.
- [ ] Review changes against CODING_STANDARDS.md.

## Evidence

Decoded the published c3706ff4c86b disc at each registered image size. Reviewed all 16 patched background stills on four contact sheets, then opened these three images at native resolution. SHOE RENTAL has crowded letter boundaries; the roster names are readable but disproportionately heavy. These are visual-quality findings, not proof of missing text or a codec failure.

Local evidence: `build/visual-review-pack/signs/I_28_02.png`, `P_01_03.png` and `P_01_03B.png` in the same directory. Game-derived images remain ignored. The computer-club sign was also checked at native size and does not need a change from this review.
