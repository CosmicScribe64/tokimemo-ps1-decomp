---
id: T-39525
title: Check English boundaries before retaining source line breaks
status: In Progress
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39511-english-text-quality]]"]
---

## Goal

Remove mid-sentence English breaks that survived because the translation and Japanese source had equal row counts. Keep deliberate sentence and paragraph boundaries.

## Acceptance criteria

- [x] Validate English boundaries even when source and target row counts match.
- [x] Test retained pauses, moved boundaries, missing boundaries and page separation.
- [ ] Compare whole-script layout results and inspect affected text in game.
- [ ] Complete regression and standards review.

## Evidence

The static orphan diagnostic exposes breaks inside complete English sentences in DATE_KO, DATE_NI and EVENT_FU. `reflow_page` accepts the corresponding English row index without checking punctuation when row counts match. All 510 flagged records have retained source breaks; this alone cannot distinguish intended pauses from layout defects. Full diagnostics remain in ignored `build/layout-orphan-review.json`.

## Comparison

The full-script comparison changes 889 records. Orphan flags fall from 511 to 454; pages needing scrolling fall from 77 to 67; rows fall from 51,269 to 50,778. Four records lose a quote previously leaked by the layout-only closing-mark pass. All other non-whitespace bytes and control codes match. No changed record is unwrappable. Metadata receipt: `translations/en/reviews/layout-boundaries-2026-11-02.json`. The 795 unmapped source pauses are now reported rather than preserved at an invalid English boundary; broader flag classification remains in T-39511.

Inspected every newly introduced orphan and scrolling-page flag. Eight short final rows are complete interjections or farewells. Three repeated FU_S6O pages gain a row because the short first source sentence now keeps its pause at the correct English boundary; the following full-name substitution makes the remainder three rows. These remain pending in-game scrolling inspection. Title abbreviations are excluded from sentence-end matching. The affected suite passes 43 tests with one opt-in skip. The combined disc is in `build/english-boundary-proof`.

The combined candidate built in 172.8 seconds. Its target is `ca7ff1e93b03c8f7c0be9bebd61a02d6187cc439`, xdelta 6,566,887 bytes, 252 placed art checks passing. Frozen link inputs are under `build/english-boundary-proof`. A fresh voice-enabled route is running in `build/emu/route-en-english-boundary`; the script and executable disc audits and browser package are being checked.

Both actual-disc text audits pass on `ca7ff1e93b03`, covering 31,471 strings and 31,334 pointers in 105 containers, all 2,940 executable slots accounted for, no pending slots. Browser checks also pass, including exact downloaded-disc equality. Evidence is in `build/english-boundary-*-audit.json`, `build/english-boundary-browser.log` and `build/patcher-test/result.json`. The fresh route remains active.

Controlled off-route verification remains open. The first bootstrap from a prologue state did not load a scene and was rejected. A separate 300-frame settling run from the current candidate's first Sunday state produced a verified command-menu state. The retry loads a scene, but its dialogue-table identity has not yet been confirmed. Do not treat it as FU_S6O evidence. The scripts and captures are under `build/probe_festival_boundary*.py` and `build/emu/festival-boundary-ready-bootstrap`.
