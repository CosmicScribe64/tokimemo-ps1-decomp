---
id: T-39533
title: Improve shoe-rental and class-roster lettering
status: Done
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39508-english-art-on-disc]]"]
---

## Goal

Improve the cramped lettering found during the bulk background review without changing the translated wording or surrounding art.

## Acceptance criteria

- [x] Give SHOE RENTAL in I_28_02 clearer letter separation at native resolution.
- [x] Review the heavy, condensed roster names in P_01_03 and P_01_03B against the original sign style and revise the lettering.
- [x] Inspect encoded results and actual in-game captures, including the roster animation.
- [x] Pass existing art size, decode and disc-readback checks.
- [x] Review changes against CODING_STANDARDS.md.

## Evidence

Decoded the published c3706ff4c86b disc at each registered image size. Reviewed all 16 patched background stills on four contact sheets, then opened these three images at native resolution. SHOE RENTAL has crowded letter boundaries; the roster names are readable but disproportionately heavy. These are visual-quality findings, not proof of missing text or a codec failure.

Local evidence: `build/visual-review-pack/signs/I_28_02.png`, `P_01_03.png` and `P_01_03B.png` in the same directory. Game-derived images remain ignored. The computer-club sign was also checked at native size and does not need a change from this review.

## Progress

Added build-time regeneration from the existing lettering plans, with original-source hashes and exact registered-crop checks. Kept the approved rotated roster layout, original ink sampling and identical Fujisaki position in both images. Reduced roster stroke weight and kept its edge margins; gave SHOE RENTAL thinner strokes and more spacing. The existing lettering suites ran 63 tests: 61 passed, two optional checks skipped. The rental sign's encoded preview is visually clearer and fits its original sectors; pixels outside edited macroblocks remain identical. Roster encoding and in-game replay are still pending. No candidate has been published.

Completed all six contact sheets from `build/emu/sign-quality-roster` (50 timed captures). The roster lettering remains legible during the fade, with the highlighted name aligned between both variants. The following classroom dialogue shows the repaired closing quote and separate parenthetical without overlap. Timed partial reveals are not counted as complete dialogue checks.

The candidate winter probe completed 6,000 frames and all 23 captures were inspected. It reached the ordinary ski-date arrival, choices and departure, but not the rental counter. It therefore adds scene coverage without proving the revised rental sign in-game. The rental sign still needs the correct scene entry; decoded/disc-verified art alone is not a substitute.

Correcting the fixture's `ovl_entry` to the linked `date_scene_mode_handler` reached Nozomi's special winter scene (`build/emu/sign-quality-scene-winter`, 23 captures reviewed). The rental-counter background appears at frames 3,000–3,250, but foreground art covers its lettering. This still does not prove the sign's readability in gameplay. The earlier fixture changed the mode byte while leaving the ordinary date handler installed; that fixture issue is now understood.

Started `build/emu/sign-quality-bowling-winter` on the published candidate, using the bowling venue instead of the earlier ski setup. Captures every 20 frames from frame 2,000 through 5,000 are intended to catch the counter before or between foreground transitions. Sign readability remains pending until these images are inspected.

Inspected all 151 dense bowling-scene captures across seven overview sheets and opened frame 3160 at native size. The original foreground composition covers the rental lettering throughout the counter shot; there is no unobscured interval in this replay. The rest of the scene renders without new clipping or damaged artwork. Native encoded inspection verifies the revised lettering itself. All three images read from the published a839441add3e disc decode pixel-for-pixel to the reviewed previews (`build/sign-quality-current-disc-proof.json`). The roster fade was separately inspected in all 50 captures.

Standards review checked the three-source hash guard, exact registered-crop replacement, unchanged surrounding pixels, encoded size limits and ignored generated assets. The existing suite passes 61 tests with two optional skips. No code findings remain. The ticket criteria are complete; the receipt explicitly limits the rental-sign gameplay claim to its occluded composition. Broader route and dialogue review remains under T-39511.
