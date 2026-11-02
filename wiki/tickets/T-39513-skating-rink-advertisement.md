---
id: T-39513
title: Translate the omitted skating-rink advertisement
status: In Progress
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39508-english-art-on-disc]]", "[[tickets/T-39510-no-japanese-left-check]]"]
---

## Goal

Translate the visible advertisement in CDROM/BS/D_19_03.BS, absent from the existing artwork inventory and not an approved Japanese exception.

## Acceptance criteria

- [ ] Record the source dimensions, text regions, translation and source identity.
- [ ] Reproducible English lettering preserves unrelated artwork and passes the existing still size/decode gates.
- [ ] Build report includes this item and final-disc readback verifies it.
- [ ] Controlled skating-date GPU replay verifies all visible lettering without overlap.
- [ ] Code reviewed against CODING_STANDARDS.md.

## Evidence

`build/emu/skating-english/shot_2000.png` shows a large Japanese billboard above the portrait. The scene calls `date_skating_rink_enter_spot` in `src/ovl/date/place_scenes.c`, reading LBA 0x4574 (17780), original size 17,952 bytes. Neither `image_text_catalog.jsonl` nor the current build's artwork list contains D_19_03. Zero font-upload events in this fixture confirms that the runtime glyph trace cannot detect this artwork gap.

The preceding D_19_02 entrance is already translated; `shot_1000.png` shows its English arrow sign, while the portrait hides part of the shop board. The fixture uses logged scene/date RAM writes; it is not a normal career playthrough.

## Progress

- Source SHA-256: 79a65609c52a2a38d674bb549da0f60a17383f54c5fa0734f659b4de5419c151; quantizer 5, 200 macroblocks, 320x160 decoded pixels.
- `build/skating-ad/draft.py` produces a preview/crop using code-made text and local background-color fitting; no source file changed. English draft lettering is readable in `draft.png`. The tiny suffix at the right edge of the bus band is ambiguous and cropped; draft treatment requires owner review before acceptance.
- `build/skating-ad/encode.py` is running the production encoder and unchanged verification gates. Integration, final readback, GPU replay and code review are still pending.
