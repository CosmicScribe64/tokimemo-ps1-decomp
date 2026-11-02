---
id: T-39525
title: Check English boundaries before retaining source line breaks
status: Done
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
- [x] Compare whole-script layout results and inspect affected text in game.
- [x] Complete regression and standards review.

## Evidence

The static orphan diagnostic exposes breaks inside complete English sentences in DATE_KO, DATE_NI and EVENT_FU. `reflow_page` accepts the corresponding English row index without checking punctuation when row counts match. All 510 flagged records have retained source breaks; this alone cannot distinguish intended pauses from layout defects. Full diagnostics remain in ignored `build/layout-orphan-review.json`.

## Comparison

The full-script comparison changes 889 records. Orphan flags fall from 511 to 454; pages needing scrolling fall from 77 to 67; rows fall from 51,269 to 50,778. Four records lose a quote previously leaked by the layout-only closing-mark pass. All other non-whitespace bytes and control codes match. No changed record is unwrappable. Metadata receipt: `translations/en/reviews/layout-boundaries-2026-11-02.json`. The 795 unmapped source pauses are now reported rather than preserved at an invalid English boundary; broader flag classification remains in T-39511.

Inspected every newly introduced orphan and scrolling-page flag. Eight short final rows are complete interjections or farewells. Three repeated FU_S6O pages gain a row because the short first source sentence now keeps its pause at the correct English boundary; the following full-name substitution makes the remainder three rows. Their completed scrolling checks are recorded below. Title abbreviations are excluded from sentence-end matching. The affected suite passes 43 tests with one opt-in skip. The combined disc is in `build/english-boundary-proof`.

The combined candidate built in 172.8 seconds. Its target is `ca7ff1e93b03c8f7c0be9bebd61a02d6187cc439`, xdelta 6,566,887 bytes, 252 placed art checks passing. Frozen link inputs are under `build/english-boundary-proof`. The completed voice-enabled route is in `build/emu/route-en-english-boundary`; the actual-disc audits and browser checks pass.

Both actual-disc text audits pass on `ca7ff1e93b03`, covering 31,471 strings and 31,334 pointers in 105 containers, all 2,940 executable slots accounted for, no pending slots. Browser checks also pass, including exact downloaded-disc equality. Evidence is in `build/english-boundary-*-audit.json`, `build/english-boundary-browser.log` and `build/patcher-test/result.json`. The route completed and its captures were inspected below.

The controlled off-route investigation initially loaded the wrong overlay; the corrected run is documented below. The first bootstrap from a prologue state did not load a scene and was rejected. A separate 300-frame settling run from the current candidate's first Sunday state produced a verified command-menu state. The retry loads a scene, but its dialogue-table identity has not yet been confirmed. Do not treat it as FU_S6O evidence. The scripts and captures are under `build/probe_festival_boundary*.py` and `build/emu/festival-boundary-ready-bootstrap`.

## Completed route review

The candidate route finished all 33,572 frames. All 134 captures were inspected in 15 contact sheets, with native-image rechecks of the command screen and date dialogue. No new clipping or overlap was observed. Long names fit dialogue and save/load details; the plaque, classroom closing quote and phone farewell remain correct. The font trace reports zero original-font calls, unknown calls and overflow calls. Receipt: `translations/en/reviews/boundary-route-2026-11-02.json`. This proves the opening-to-first-date route, not the remaining off-route pages.

The earlier festival bootstrap selected `0x70` (BUNKASAI), but the target function belongs to `0x71` (BUNKAKEN), confirmed by `include/game_enums.h` and `config/overlays/BUNKAKEN.yaml`. A corrected controlled run loads Shiori in the literature-club scene at stage `0x21`, step 7. Captures and state are in `build/emu/festival-boundary-tour-bootstrap`. Exact affected-line scrolling checks are now using this state; the previous bootstrap remains rejected as target-table evidence.

## Festival scrolling and code review

All three newly scrolling FU_S6O passages (groups 119, 125 and 131, line 4) were rendered from the corrected controlled state. Each run's first 128 container bytes match the built disc's FU_S6O file. Native captures show the passage beginning and its final three rows after scrolling, including the closing quote. No original-font, unknown or overflow calls were recorded. Receipt: `translations/en/reviews/boundary-festival-2026-11-02.json`. These are controlled line-selection checks, not claims of naturally reaching every festival branch.

Reviewed the changes against `CODING_STANDARDS.md`: source boundaries are validated before reuse; unmatched boundaries remain reported; page controls and translated wording are preserved; only layout-added quotes are removed. The synthetic tests cover sentence boundaries, abbreviations, asides, separate reply pages and trimmed whitespace. Existing complete-disc audits and the completed route cover the built candidate. No unresolved code findings. The current browser patcher is being tested with this candidate before publication.

## Publication

The current browser patcher passed all 15 checks with this candidate, including real archive input and exact downloaded-disc equality. Pages commit `81097d06555afdb858cb99a2e93318640168429c` is live; workflow `41526884160` succeeded. All 36 served files match the tested package. The published target is `ca7ff1e93b03c8f7c0be9bebd61a02d6187cc439`. Broader quality review remains in T-39511.
