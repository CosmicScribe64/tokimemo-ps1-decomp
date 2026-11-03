---
id: T-39539
title: Check shortened speaker labels after repeated replies
status: Done
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39511-english-text-quality]]", "[[tickets/T-39537-dialogue-break-review]]"]
---

## Goal

Determine why speaker labels lose leading letters in some controlled reply replays, and fix any defect that also affects normal play.

## Acceptance criteria

- [x] Compare controller-driven page advancement with controlled clear/line selection.
- [x] Inspect name bytes, text entries and glyph positions when a label is shortened.
- [x] Repair a confirmed runtime defect or correct the fixture if normal play is unaffected.
- [x] Recheck full names and repeated dialogue before publication.
- [x] Review any changes against CODING_STANDARDS.md.

## Evidence

The camp wrap replay has a complete three-row body and keeps the numbered label together, but its speaker column appeared to show only a suffix. Similar apparent shortening occurred late in some controlled date batches. Fresh scene captures have full labels. Approval of the text correction receipts covers reply text and their stated renderer scope; it does not resolve this separate repeated-label observation. Current build is `build/yumi-reviewed-release`, track `7738fcbe89bee49102c68b025a5f5104ccf6b09f`.

First compare natural progression and the fixture's manual clear sequence before changing game code. Relevant code is `english_speaker_draw`, `english_k_reset`, `english_load_string` and `english_disp_string`.

## Result

No shortened name was reproduced. Pixel enlargements of the camp, Ayako and late date captures show the complete speaker names. The reduced previews were misread. The camp memory probe also has intact name bytes, all nine name glyphs with valid pen positions, and draw origins of 160 in both buffers. A controller-only comparison preserves the full name as well.

No game or fixture fix was needed. Reviewed the diagnostic evidence against CODING_STANDARDS.md; all probes stay ignored under `build/`, with no code changes or raw-source edits. Publication can proceed after the remaining release checks. This closes the observation, not the broader game review.
