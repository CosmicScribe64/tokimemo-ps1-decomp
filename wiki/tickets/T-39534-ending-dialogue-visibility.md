---
id: T-39534
title: Verify and repair ending dialogue visibility
status: Done
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39511-english-text-quality]]"]
---

## Goal

Keep translated ending dialogue readable through every page, including heroine lines and long scrolling passages.

## Acceptance criteria

- [x] Identify why Yukari's heroine passages are absent from the timed captures while player replies appear.
- [x] Verify text reveal, clearing and audio-wait behavior before changing the renderer.
- [x] Inspect complete lines 8, 10 and 14 in ENDING table 9, including all scrolled rows.
- [x] Check both voice-enabled and voice-disabled playback.
- [x] Compare visible face-animation seams with the original game and resolve any patch regression.
- [x] Review any fix against CODING_STANDARDS.md and verify the default matching build remains unaffected.

## Investigation history

`build/emu/ending-yukari-dialogue` continues the mapped ending fixture with controller input only. Sampled table pointers select ENDING table 9. Timed captures show player replies, but no readable heroine text across the intervals containing the pending long passages. Line 8 has kind 0xF, line 10 kind 1 and line 14 kind 1. All three remain pending; this is not accepted ending coverage.

The source permits the clear/wait branch to advance when text is unfinished and no button is held, subject to audio state. That is a timing lead, not yet a proven cause. Color, voice gating and live text entries also need inspection. Small horizontal seams appear in the eye/mouth animation; original-game comparison remains necessary.

Source follow-up: `ending_audio_level_update` explicitly sets all text entries inactive for voiced line kinds when no button is held, while keeping kinds 0/6 visible. This explains the player/heroine difference as original behavior, not yet a patch regression. Started a replay holding Circle throughout to inspect the subtitles. Asked the owner whether English subtitles should appear automatically or preserve the hold-button behavior; no presentation change has been made yet. The timing and animation checks remain open.

The owner chose automatic English ending subtitles. Added English-only visibility and timing guards. Voiced text no longer requires a held button, and clear/wait transitions require the full text plus one second with its final rows visible. Original audio completion gates remain in place. The default-language branches are unchanged. Build and runtime checks are pending; no new build has been published.

The combined candidate built successfully at `build/sign-subtitle-release/disc` (track SHA-1 prefix `636b9f436977`, 6,563,038-byte patch). Build verification accepted all 252 translated art placements and 28 intentional retentions. A fresh ending overlay entry is running against this candidate; old ending save states are not reused across changed overlay addresses.

Candidate disc audits passed: 105 containers, 31,471 strings and 31,334 pointers; executable audit reports zero pending slots (2,827 exact slots plus accounted derived/retained entries). Receipts: `build/sign-subtitle-script-audit.json` and `build/sign-subtitle-executable-audit.json`. The rebuilt default ENDING binary passed its original SHA-1 check; the full default target is still running its remaining checks.

The fresh candidate replay finished 7,500 frames (`build/emu/ending-yukari-auto`). Inspected contact sheets 2–4, including heroine text at frames 5,000–5,750 and 6,500–7,500, outside the four-frame controller taps. Subtitles now appear automatically and scroll; the first heroine passage reaches its complete final rows. These early captures do not cover the three pending long passages. Started a continuation with no controller input and captures every 60 frames to check those passages. Small face-animation seams and missing closing quotation marks remain under review.

The full incremental default target finished successfully, including the matching ENDING binary, globals and all 14,449 rename-ledger rows (`build/sign-subtitle-default-check.log`). The candidate main executable is byte-identical to the earlier boundary-proof main, validating reuse of the pre-ending gameplay fixture. The input-free continuation shows the complete last rows of table 9 lines 8 and 10 at local frames 5,340 and 6,780. Quotation closure and the unusually narrow ending text column still prevent visual sign-off; long-page records remain pending until those presentation findings are resolved.

The input-free continuation completed 12,000 frames and produced 200 timed captures (`build/emu/ending-yukari-auto-long`, sheets in `build/live-sheets/ending-yukari-auto-long`). Inspected sheets 1, 3, 5, 7, 9–14 and 17–23. Table 9 line 14 reaches its final rows at frames 9,540–9,600, then the scene proceeds through the remaining dialogue and into the epilogue. All three target passages now have visible complete final rows, but this is timing evidence rather than final presentation approval. Remaining sheets, voice-mode comparison, quotes, column width and the original-game face-animation comparison are still open.

Added dedicated ending subtitle geometry to the wrapper. Player text starts at -0x58 and heroine text at -0x82, with two visible scrolling rows and no speaker-name reservation. Closing quotes stay in ENDING payloads for standalone speech and the final continuation; intermediate continuation records remain open. The English-only renderer brings opening punctuation eight pixels closer to its text. A Japanese-reference replay is running from an original-game fixture to compare the facial animation.

Focused layout and display-wrap tests: 45 run, 44 passed and one optional full-disc test skipped (`build/ending-layout-tests.log`). The first Japanese-reference probe remained in the original loader and never displayed the ending, so it is rejected as comparison evidence. Retrying from the fully initialized holiday checkpoint instead of the immediate post-prologue checkpoint.

The holiday-based Japanese replay successfully loaded ENDING and displayed Yukari's face. Compared the exact RGB rectangle `(90, 60, 220, 180)`, covering the eye/nose seams, against the previous English replay: 96 English captures match an original capture exactly, across two distinct animation states. Inspected the side-by-side sheet at `build/ending-face-reference/comparison.png`; machine-readable pairs and hashes are in `comparison.json` alongside it. The observed seams occur in the original game under this emulator and are not introduced by the English patch.

The wider-column candidate built successfully (`build/ending-layout-release/disc`, track SHA-1 prefix `c0dafc36074d`, patch 6,563,389 bytes). Script readback verified 31,471 strings and 31,334 pointers; executable readback has zero pending slots. Replays with explicit voice options 0 and 1 are running against this candidate.

Extended the original reference through later face animation. Inspection of the seven distinct matched crops found six facial states and one black transition; closed-eye poses are not among them. The observed open-eye seams match exactly, but the larger closed-eye seams still need a later Japanese capture. The comparison count must exclude the black transition.

Both 20,000-frame candidate replays reached the epilogue. Sampled voice bytes remained 0 and 1 respectively, and dialogue transitions occurred at the same sampled frames. Of 196 timed image pairs, 189 are byte-identical; seven differ and need direct comparison. Inspected all 22 contact sheets from voice mode 0. The wider columns and quotes render correctly in several complete speeches, but two presentation findings remain. There are very short final rows in some wrapped sentences, and a closing quote after a trailing parenthetical thought. Adjusted ending-only wrapping to balance the final row and close spoken text before the thought. Regular dialogue-box wrapping remains unchanged. The sparse 100-frame captures miss some final punctuation intervals, so the next replay needs denser captures before long-page approval.

The later original replay supplies the closed-eye pose. After excluding black transitions, 183 English face crops match the Japanese reference across seven facial states. Inspected all seven pairs, including closed eyes; `translations/en/reviews/ending-face-comparison-2026-11-02.json` records the hashes and scope. The facial-seam comparison is complete. The ending-layout refinement tests ran 47 cases: 46 passed, one optional full-disc case skipped.

Inspected the seven differing voice-mode frame pairs in `build/ending-voice-comparison/differences.png`. Every difference is confined to the mouth animation (x=154–168, y=138–152); subtitles and the rest of the frame are identical. All 196 sampled subtitle images therefore agree between the two settings, and the table/line transition samples agree as well. This verifies subtitle behavior for both options, not audio muting. The original ending renderer requests its voice clips independently of the ordinary dialogue voice option.

Code review found a scope correction before publication. ENDING tables 0, 3, 13 and 15 bind the regular `talk_text_table` renderer in the companion/secondary scenes. Only tables 1, 2, 4–12, 14 and 16 bind `ending_scene_dialogue_tables` for subtitles. The initial profile-wide geometry rule is too broad. Restrict it to those subtitle tables and add a regression check for all four boxed tables before accepting the candidate. The currently running balanced build predates this correction and must not be published.

Restricted subtitle geometry and payload punctuation to the 13 tables actually bound by the subtitle renderer. Added regression checks that tables 0, 3, 13 and 15 retain three-row message windows. The focused suites now run 48 cases: 47 passed, one optional full-disc case skipped (`build/ending-scoped-tests.log`). Starting the corrected build; the prior balanced candidate is superseded.

The table-specific balanced candidate built successfully at `build/ending-scoped-release/disc` (track SHA-1 prefix `a839441add3e`, patch 6,563,384 bytes). Both disc-text audits pass: 31,471 strings, 31,334 pointers and zero pending executable slots. The early replay now captures every 30 frames and samples the text reveal position as well as script pointers; it saves an ending-state fixture for subsequent scrolling checks. This candidate is not published.

The current candidate's early replay completed 7,500 frames. Inspected the complete rows of table 9 lines 1 and 3, including their closing quotes; text-state samples confirm reveal position equals total length at frames 5760 and 6840. No clipping, overlap or isolated short final row was visible. Recorded capture and payload hashes in `translations/en/reviews/ending-long-pages-2026-11-02.json`; the long-page queue now has 27 inspected records and 60 pending. A 12,500-frame continuation from this candidate's own ending state is running with 30-frame screenshots and text-state samples.

Reviewed the C and layout changes against `CODING_STANDARDS.md`. The C changes use C89, existing declarations and ticketed English-only guards; the original branches remain intact. No symbols were renamed, SDK declarations replaced, generated assembly edited or game data added. The default ENDING checksum and globals/rename checks pass in `build/ending-layout-default-check.log`; the later changes only affect Python layout. The renderer-table scope finding is resolved and covered by tests for all four ordinary dialogue-box tables. The focused suites pass 47 cases with one optional disc case skipped. Runtime acceptance of the remaining long passages is still open.

The 12,500-frame continuation completed. Inspected 62 selected captures across seven sheets, including every row and final punctuation of table 9 lines 8, 10, 12 and 14. All 416 script-header samples match the candidate disc. Text-state samples confirm completion before each approved final capture. Together with the early replay, six flagged passages are accepted in `ending-long-pages-2026-11-02.json`. The trailing thought in line 17 now follows the closed spoken sentence. Browser acceptance also passes for the same candidate, including ZIP drag-and-drop, cancellation, invalid inputs and exact downloaded disc/audio hashes. Code review and this ticket's runtime criteria are complete. The broader visual queue remains open with 31 inspected records and 56 pending.

Published the tested package to the Pages branch at `fa1b90a521d592da6a6e62567fcca2e04320d1f9`; deployment and served-file verification are pending. Source fixes and receipts are pushed to `patch/english` at `3d09cbcc3`, with draft PR #71 targeting main. Automatic approval review rejected a direct main update, so main was left unchanged.

Pages deployment 41554082219 succeeded. All 36 served package files match the tested candidate exactly, including the patch and release manifest (`build/ending-scoped-served-files.json`). The live patcher now serves review build `a839441add3e`; broader visual sign-off remains open.
