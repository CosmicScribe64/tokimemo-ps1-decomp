---
id: T-39521
title: Translate the three-legged-race speech bubbles
status: Done
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39508-english-art-on-disc]]", "[[tickets/T-39510-no-japanese-left-check]]"]
---

## Goal

Translate all four speech-bubble sprites used beside the animated characters during the three-legged-race instructions.

## Acceptance criteria

- [x] Identify the source image, lettering and runtime consumer.
- [x] Translate the bubbles while preserving their animation, palette and surrounding artwork.
- [x] Register the repair and verify the pixels read back from the rebuilt disc.
- [x] Inspect both instruction variants in the emulator.
- [x] Complete code review against CODING_STANDARDS.md.

## Evidence

Controlled sports captures on `build/phone-width-proof` show an untranslated speech bubble at the right edge of `build/emu/sports-card-pair-0/shot_1500.png` and `sports-card-pair-1/shot_1500.png`. Both instruction panels are English, but the bubble is not an approved retained-Japanese exception. All five sports-card fixtures complete 1,800 frames with zero original-font calls, confirming that glyph tracing does not cover this sprite lettering.

`build/probe_sports_cards.py` loads the compatible full-name holiday state, requests the sports overlay through the loader, then selects the source-defined instruction phase. For the two partner variants it sets `sports_schoolyard_result_selector` to 0 or 1 and starts stage 3. These are controlled scene fixtures, not normal-career traversal evidence. The instruction resources are loaded from NINSETU.

Source inspection identifies four speech bubbles in the first sprite page of `CDROM/TAIIKU/NINSETU.BIN`, at file offset 0x2000. They convey "Let's go!", "Ready!", "Right" and "Left". `D_8014A1FC` supplies three 8-bit texture pages at 0x80191000, 0x80195000 and 0x80199000, corresponding to file offsets 0x2000, 0x6000 and 0xA000. `sports_schoolyard_primary_actor_setup` binds these pages to the animation/frame tables in TAOPD at offsets 0x6674 and 0x66BC. The current card writer covers separate pages starting at 0xE000, so these bubbles were outside its views. Inspected the source pages in `build/sports-flag/source-pages.png`; the two other pages contain character animation frames.

The writer now registers four bubble views and replaces only their lettering, using the original palette and animation records. The initial preview used a pale antialiasing shade; the corrected preview uses the original saturated blue and red for legibility. Inspected `build/sports-flag/english-page.png`. All 27 sports/art tests pass, including source-hash rejection, unchanged palette and file size, changes confined to the bubble rectangles, and rejection of a damaged neighboring sprite page.

## Review

The rebuilt disc (`build/sports-callout-proof/disc`, SHA-1 prefix `28107c11bdbc`) verifies all 252 placed images, including the four bubble views; 28 approved exceptions remain. The patch is 6,566,654 bytes. Both 1,800-frame partner fixtures complete with zero original-font calls, unknown glyphs or overflows. Inspected each variant at frames 1200 and 1500: the direction labels fit, and the character and card artwork remain intact. A 2,400-frame cycle also shows the readiness label clearly. The start-call bubble was reviewed in the decoded sprite-page preview and disc readback; it did not appear in these instruction-animation captures.

Standards self-review checked the source hash guard, bounded image regions, original-palette reuse, unchanged animation data and exclusion of generated assets from Git. The 27 art tests pass, as do both actual-disc text audits. No game C or default-build inputs changed. No unresolved code finding remains. The staged documentation, comments and receipt assessments were reviewed with deslop and Humanizer. Removed a stale build-status sentence; no remaining prose finding or game-text edit.
