---
id: T-39506
title: Make dialogue, choices, names and name entry work in English
status: In Progress
assignee: claude
created: 2026-11-01
updated: 2026-11-01
links: ["[[tickets/T-39500-english-translation-patch]]"]
---

## Goal

Under `TOKIMEMO_ENGLISH`, the script and name code handles the English encoding from [[english-text-runtime]]: the dialogue parser, choices, speaker labels, name substitution and honorifics, and name entry with an English letter page (A-Z, a-z) shown by default and single-byte names of up to 8 letters in the existing buffers.

## Acceptance criteria

- [ ] `talk_render`, `talk_speak_part`, `make_three_select`, `sndisp`/`sndi`, `gnsx`, `get_g_name`/`get_p_name` and the punctuation check handle English control bytes and single-byte text.
- [ ] Name entry opens on the English letter page; kana pages still work; names are stored single-byte and drawn in the Latin font everywhere they appear.
- [ ] A hand-encoded English dialogue page with a name, a choice and a page break shows correctly in the emulator.
- [ ] The default build still matches 27/27.

## Notes

Part of [[tickets/T-39500-english-translation-patch]].

## Comments
