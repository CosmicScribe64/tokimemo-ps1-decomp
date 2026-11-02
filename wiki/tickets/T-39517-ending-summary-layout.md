---
id: T-39517
title: Fit the scrolling English ending summaries
status: Done
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39511-english-text-quality]]", "[[tickets/T-39510-no-japanese-left-check]]"]
---

## Goal

Keep the complete English career/relationship ending summary inside the screen while preserving readable scrolling and all text.

## Acceptance criteria

- [x] Prove the summary window geometry and wrap every applicable summary record, including expanded names and career labels.
- [x] Preserve all wording and correct scroll progression without text-entry/glyph overflow.
- [x] Actual ending GPU captures show complete lines and final text; original build remains matching.
- [x] Regression checks, final-disc audit and code review pass.

## Evidence

The 5,000-frame controlled Yoshio ending reaches graduation, career summary and song with zero original-font calls. Nevertheless `build/emu/ending-arena-proof/shot_2500.png` visibly clips several summary lines at the right edge. `ending_graduation_result_dialogue_select` renders them at x=-0x48 (screen x=88); `english_talk_render` expands rows but does not dynamically wrap them. The summary's source-record/window mapping must be inspected; do not accept clean font tracing as layout proof.

The affected records are in `CDROM/SERIFU/EPILOG.BIN` (profile `serifu-epilog`), especially table-11 and table-12. Their command-7 metadata currently selects generic narration x=-0x80, 252-pixel width; the actual summary calls use x=-0x48. Generic M substitution reserves only 56 pixels, but ending career labels such as "The second-tier company" are longer. A correct repair must use the summary's actual width and context-specific substitution bounds or runtime wrapping, not merely move the clipped text left.

## Implementation

EPILOG records now use a dedicated 196-pixel summary window (x=-72 through +124) instead of generic 252-pixel narration. Runtime `english_talk_render` also wraps each fully expanded non-choice row at its actual text origin, accounting for career labels and full player names without a guessed reservation. It preserves explicit paragraph breaks, suffixes, atomic Shift-JIS/name-pair characters and every non-space character; long tokens split only on character boundaries. Choice rows retain one selectable row. New rows update `talk_next_row_y` so the existing ending scroll follows the complete text.

Measured all 17 generated career labels: the maximum is 126 pixels (The second-tier company), confirming that the old 56-pixel placeholder reservation is insufficient. Production-function C89 tests exercise expanded labels, exact reconstructed wording, y progression, long tokens, atomic characters, unchanged choices and explicit blank rows. English patch/window regressions pass. Full build and GPU/resource-limit checks pending.

Capacity audit found a second failure in this same path: EPILOG table-3 line-0 is 544 encoded bytes (528 glyphs, 17 rows), longer than the English parser's 512-byte block buffer and larger than the entire 384-glyph atlas even before preceding summary paragraphs. Combined summary upper bounds exceed the atlas for several candidates. The first wrapping implementation is therefore not accepted as a complete repair. A controlled Rei summary probe is added; the final implementation must retain and display the entire text with bounded live renderer resources.

The first GPU replays confirm the width fix but also the capacity failure: normal summary peaks at 370 glyphs; the long-career variant reaches 415 and the controlled Rei summary 670, visibly corrupting text. All font traces stay zero, so this is an atlas overflow rather than untranslated text. These runs are rejected as complete acceptance.

Implemented scene-owned streaming: capture all expanded/wrapped summary rows into a bounded overlay buffer, clear the capture hook immediately afterward, and load only rows within the existing -80..80 scrolling viewport. Reload glyphs only when the visible row range changes; otherwise move existing entries. The parser now holds 1,024 bytes, retaining the complete 544-byte record. Production C89 queue tests reach every row of a >1,024-byte summary while keeping the live atlas bounded, check unchanged stored text, avoid needless reloads and reject buffer/row overflow. Full rebuild and GPU checks of this streaming version are pending.

Pre-acceptance review rejected the first streaming image's queue placement: legacy overlay bss lies past the padded image and overlaps the background buffer. Nonzero initializers keep the queue in loaded `.data`; verified object references place rows at offset 0, text at 1024, and six counters at 5120..5140. The rebuilt map places this data at 0x8013cfe0, safely within the overlay. Capture now wraps the scene callback instead of inserting preprocessor branches into the signed original function, preserving the rename ledger's whole-function token proof. Default hash/rename/header checks and new GPU probes are pending.

Final streaming image `build/ending-stream-final-proof` (target prefix cd882b623807) passes actual-disc readback for 105 containers/31,471 strings/31,334 pointers with zero original-font bytes. All 31,031 encoded talk records fit the 1,024-byte parser (maximum 544 bytes). Default ENDING checksum, 14,449-row rename audit and headers pass. Main binary and ELF exactly match the completed 33,660-frame/145-capture main route through save/load.

Three 5,000-frame controlled ending probes pass with zero queue/atlas overflow: normal 15 rows/385 bytes/234 peak live glyphs; long-career 17/432/240; Rei 23/726/337. Actual Rei capture at frame 3,000 shows the complete closing sentence, with clear text and no right-edge clipping. Captures at `build/emu/ending-stream-final-{normal,career,rei}`. Twelve other candidate queues with long career/player names are now being captured for complete-window capacity checks before acceptance.


## Final review

2026-11-02: In Progress -> In Review -> Done after standards review. All 27 original hashes match; headers and 14,449-row rename audit pass. C89 production tests pass (39 tests, one opt-in skip); original function tokens remain unchanged, English changes are guarded, SDK types and owning headers are used, capture ends synchronously, queue storage is initialized data within the overlay, and byte/row/atlas bounds are explicit. No generated disc/assets are staged. Review findings (out-of-slot bss, signed-function preprocessing, overflow-fallback indexing) are resolved.

Thirteen long-career candidate queues (IDs 0..12), plus normal/Yoshio and full Rei scroll probes, have complete stored-row checks: each row fits x<=124, every integer scroll position fits the 384-glyph atlas, maximum 361. Largest captured queue is 773 bytes. Three full 5,000-frame probes show zero original-font calls and zero queue/atlas overflow; Rei's final sentence is visibly complete. These are controlled ending fixtures, not complete input-only playthroughs of every ending branch. Reports: `build/ending-stream-all-queue-capacity.json`, `build/ending-stream-final-gpu-summary.json`, `build/ending-stream-original-hashes.log`.

Browser package `build/patcher-ending-stream-review` passes all nine acceptance checks, actual download equals CLI target `cd882b6238075dd9d36fd7af7fe6b546e04ecfa2`, and the completion screen was visually reviewed. Local port 8769 now serves this package. Whole-game art/text coverage and owner release signoff remain under their parent tickets.
