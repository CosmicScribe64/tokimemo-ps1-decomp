---
id: T-39508
title: Put the English art on the disc
status: In Progress
assignee: claude
created: 2026-11-01
updated: 2026-11-01
links: ["[[tickets/T-39500-english-translation-patch]]", "[[tickets/T-39507-english-disc-build]]"]
---

## Goal

The English disc build writes every registered English image into its container on the disc: the indexed NBN/TIM sheets (MAINROM, TITLE, OPTION, FILESEL, NAME_ENT and the others), with their palettes and semi-transparency bits, and the MDEC background stills, re-encoded. Sprites the English art widened get the code and texture-page changes they need in the English build.

## Acceptance criteria

- [ ] Every artwork item with a registered English candidate is written by `tools/patch/build_english.py`; the report lists each item as placed, deferred (with the reason) or untranslated by decision.
- [ ] Indexed images round-trip byte-exact with the original pixels; English pixels use only colours (and STP values) the container's palette allows.
- [ ] Stills are re-encoded to fit their read size or moved, and decode back to the intended image within the codec's error.
- [ ] The emulator route on the English disc shows the English art at the title, file select, name entry, the magazine, the opening stills and the sports card.
- [ ] The default build still matches 27/27.

## Notes

Part of [[tickets/T-39500-english-translation-patch]]. Art sources: `translations/en/graphics/translation_inventory.json` and the checkpoints under `translations/en/graphics/artwork/checkpoints/`.

## Comments
