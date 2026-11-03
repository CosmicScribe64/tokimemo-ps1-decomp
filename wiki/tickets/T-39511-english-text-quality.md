---
id: T-39511
title: Make the English read naturally on screen
status: In Progress
assignee: claude
created: 2026-11-01
updated: 2026-11-02
links: ["[[tickets/T-39500-english-translation-patch]]", "[[tickets/T-39506-english-script-runtime]]"]
---

## Goal

The owner found much of the on-screen English rough: lines broken where the Japanese broke them ("(What'll I do / this / week?)" over three lines) and layouts that made sense only in Japanese. The English must read as if it were written for these boxes.

## Acceptance criteria

- [ ] Dialogue, monologue, narration and telephone text is reflowed to each window's pixel width, ignoring the Japanese line breaks, with balanced lines, no orphaned words, and the fewest lines that fit (done in [[tickets/T-39506-english-script-runtime]]).
- [ ] Automated layout checks over the whole script and every menu, label and table: orphans, avoidable extra lines, overflow, clipping, labels too wide for their slot, inconsistent capitalisation and punctuation across menus, leftover Japanese. Every flag is fixed or explained.
- [ ] A reading review of the full emulator run (every dialogue page and screen) and of the off-route text, listing stiff or too-literal English, tone problems and menu wording, with proposed rewrites. The owner approves rewrites before they replace approved translations.
- [ ] Before/after screenshot sheet for the owner.

## Notes

Part of [[tickets/T-39500-english-translation-patch]]. Runs after T-39506 lands, alongside [[tickets/T-39510-no-japanese-left-check]].

## Comments

2026-11-02: assembled a local gallery of 235 captures on 31 contact sheets, with build provenance and controlled-entry scope labelled. Reviewed all 33 fresh festival/Kagami captures, the 29 Nijino trip captures, 14 sports captures and nine ending captures; partial typing and scene transitions are not complete-page proof. The main-route section carries the 134 previously inspected captures. All 16 encoded background stills were also inspected; follow-up lettering work is tracked in T-39533. Gallery: `build/visual-review-pack/index.html`.

2026-11-02: verified SHUGAKU table 4/group 9/line 0 (refusal choice 1) and group 38/line 1 with Kagami's loaded assets. Six native captures show opening rows and complete scrolled endings. All 30 memory samples match the current disc header and table pointers; all 25 post-selection samples match the requested group. The first probe interrupted unfinished dialogue and retained stale text; it is rejected. Waiting for the previous page before selection resolves the fixture problem. Receipt: `translations/en/reviews/mira-trip-long-pages-2026-11-02.json`. Of 67 flagged long-page records, 23 now have runtime visual proof and 44 remain pending. This is not whole-route signoff.

2026-11-02 follow-up: T-39519 album-choice spacing repaired and fresh GPU-verified. Metadata-only receipt `translations/en/reviews/visual-quality-2026-11-02.json` records all 145 main-route and 15 event/detail captures actually inspected, with image hashes and remaining findings. Before/after at `build/visual-quality-review/album-prompt-before-after.png`. Direct-row phone closing marks confirmed as a real gap and tracked in T-39520; whole-game visual-quality signoff remains open.

2026-11-02: owner requests explicit visual quality review across gameplay and artwork. Reviewed every one of 145 current main-route captures at native resolution; most dialogue, menus, phone/date and save/load layouts fit cleanly. Found bunched new-album Yes/No labels, now T-39519. Roster-animation pink flecks also appear in the original. Some phone pages apparently lack closing quotes; verify timing/continuation rather than assuming a defect. Reinspected maximum-name exam columns, festival signs, Rei ending scroll and ambush battle; text fits, ending viewport clipping is intentional scrolling. Older tournament capture has superseded Japanese art and is not current-release evidence; inspect the accepted placard capture instead. Whole-game visual coverage is not yet complete.

2026-11-01 (Codex): owner flagged the opening classroom reply: missing closing quote, crowded quote/first letter, and a split parenthetical thought. The original prologue table record separates the thought after the spoken sentence. `english_sndisp` lacked closing marks (the prior fix covered talk scripts only). Added closing marks with thought handling, a separate trailing-aside paragraph in the wrapper, and one extra pixel of quote advance. Fresh emulator verification pending; wording unchanged.

2026-11-01 (Codex): fresh capture `build/emu/route-en-quotes/037_prologue_09.png` visually verifies all three reported issues fixed. Full 145-capture review reaches save/load and identifies dynamic date-confirmation overflow in capture 115; runtime wrapping added, awaiting a new route. Name-entry art fixes are in a separate newer build awaiting GPU verification. No wording changes made.

2026-11-01 (Codex): `build/emu/route-en-final-layout` completed 33,649 frames and 146 captures through save/load. Capture 037 verifies the closing quote, quote/letter spacing and complete second-row parenthetical; capture 115 verifies the dynamically assembled date confirmation including its closing quote. Capture 024 verifies all birthday digits alongside the English panels. The original screenshot's defects are resolved; whole-script and off-route review remain open.

2026-11-02: actual-disc audit isolates three unresolved tab-corrupted festival records (AS_S6W g0078/g0079 l0000, HI_S6 g0176 l0010). Original raw prose decodes coherently when the stray tab is treated as spacing; renderer extraction instead paired it with a Shift-JIS lead byte and protected the following fragments as opaque tokens. Implement explicit per-record English repairs guarded by both original-byte and existing-target hashes. Keep raw source and prior translation rows intact, document each exception, and require a fresh clean disc scan before acceptance.

2026-11-02: all four opaque-token dialogue cases are repaired with source/previous-target hashes: three malformed tabs and one trailing apostrophe. The rebuilt disc passes exact payload/original-font auditing for every script/profile string. Actual festival/baseball wording captures are still required; these corrections are not yet a full text-quality completion claim.

2026-11-02: visually verified both AS_S6W anomaly repairs with actual BUNKAKEN assets, dialogue tables and renderer on `build/anomaly-repair-proof`. Captures `build/emu/festival-repair-g0078/shot_600.png` and `build/emu/festival-repair-g0079/shot_600.png` show complete three-line sentences, intact quotation marks and no text-window overflow. Each controlled 600-frame replay reports zero original-font calls/unknown/overflow. HI_S6 and KOSIEN repair captures remain pending; group selection was injected and logged, not reached by normal career play.

2026-11-02: HI_S6 g0176 l0010 now also has a clean GPU proof: `build/emu/festival-yuina-clear-g0176/shot_600.png`, two complete lines with intact quotes and no overlap; 600 frames, zero original-font uploads/unknown/overflow. The first forced-jump capture retained the prior line and is rejected as layout evidence. The corrected probe triggers the game's normal line-clear path before selecting the record. All three festival anomaly repairs now have controlled visual proof; KOSIEN's repaired baseball line still needs its capture.

2026-11-02: KOSIEN group 10 line 0 is visually verified in `build/emu/baseball-repair-g0010/shot_600.png`: three complete lines, one closing quote, no overflow. The 1,800-frame MASTER bootstrap and 600-frame controlled exact-line replay both report zero original-font calls/unknown/overflow. All four source-anomaly repairs now have GPU proof. Stadium plaques remain the explicitly approved catalog-138 exception.

2026-11-02: Saturday exam-ranking GPU capture exposes a full-name/score collision despite clean original-font tracing. New T-39515 covers preserving full names and all visible scores within both columns. Source rendering and persistent-text constraints are documented there; no layout fix has landed yet.

2026-11-02: T-39515 is Done. Maximum-width full player names fit both exam columns alongside ranks/scores; sparse visibility remains intact. The full-table stress exposed and fixed an English primitive-arena stride overrun. Three 2,000-frame fixtures on `build/exam-arena-proof` pass visual review and zero original-font tracing; reaction dialogue fits. Original screenshot repair remains verified; broader text review is still open.

2026-11-02: the full-name phone route exposed awkward short middle rows in introductory captures 032 and 041. Runtime wrapping split a prewrapped row while retaining the next row boundary. Local evidence is in `build/visual-quality-review/phone-route/shot_032.png` and `shot_041.png`. The original classroom report is corrected in capture 039, where the spoken text closes before the parenthetical line without overlap.

`english_phone_wrap` reserves 116 pixels for both player and girl name codes, while the build used the general talk-renderer widths, including 88 pixels for the player-form code. Telephone and prologue windows now use the same reservations as the runtime. Intentional breaks are unchanged. The wrapper and phone test suites run 39 tests, with 38 passing and the opt-in full-disc test skipped.

The corrected `build/phone-width-proof` passes the actual-disc script audit (105 containers, 31,471 strings and 31,334 pointers) and executable audit (all 2,940 slots accounted for, no unexplained referenced-byte candidates). Fresh voice-enabled captures 032 and 041 show both problem lines in two readable rows. Inspected `build/visual-quality-review/phone-width/shot_032.png` and `shot_041.png`; wording is unchanged. Standards review found no unresolved issue in the width change. The test proves that general talk-name metrics and intentional breaks remain unchanged. Broader text review and completion of the voice-enabled route remain open.

2026-11-02: the speaker/perspective route completed with 132 captures, all inspected. Its runtime date-confirmation orphan is tracked in T-39524. Static diagnostics contain 510 flagged records (511 pages); every record has at least one retained source break, but that does not prove each break is intentional. The 302 unique payloads include short complete replies, empty trailing rows and avoidable mid-sentence breaks. In particular, `reflow_page` maps breaks by index whenever source and English row counts match, without checking whether the English boundary ends a sentence. Inspect and repair that mapping before treating the warnings as explained. Diagnostic details remain in ignored `build/layout-orphan-review.json`.

2026-11-02: after T-39525, inspected every unique final-row pair behind the remaining 454 orphan flags (453 records, 251 unique payloads). These are short replies, interjections, asides or hesitations after sentence/ellipsis pauses. Keep those pauses; they are not accidental mid-sentence wraps. `translations/en/reviews/layout-orphans-2026-11-02.json` records each payload hash and disposition for candidate `ca7ff1e93b03`. This closes the remaining orphan-warning classification for that candidate only. The 67 long-page warnings, menu/label checks, off-route visual proof and prose-review requirements remain open.

2026-11-02: read all 49 unique payloads behind 67 long-page warnings. Every affected window supports scrolling; no row is unwrappable and no hard row limit is exceeded. The maximum page lengths are four rows in 54 records, five in 11, seven in one and 17 in the ending summary. The latter uses the separate ending-scroll window. `translations/en/reviews/layout-long-pages-2026-11-02.json` records payload hashes and static dispositions; runtime verification is explicitly pending.

2026-11-02: the boundary candidate's full 134-capture route was inspected, and the three newly scrolling FU_S6O passages now have controlled runtime proof (groups 119, 125, 131, line 4). Their final rows and closing quotes fit after scrolling. Updated only those three visual statuses in `layout-long-pages-2026-11-02.json`; the other 64 records still need runtime evidence. These injected line selections verify layout, not natural story context or wording.

2026-11-02: current-build HI_S6 groups 83/90 (lines 3/7) pass controlled scrolling inspection at frames 150 and 600. Both finish with complete text and closing quotes, with no clipping. Loaded headers match the built HI_S6 container; original-font, unknown and overflow counts are zero. Receipt: `translations/en/reviews/festival-long-pages-2026-11-02.json`. The long-page queue now has 62 records awaiting runtime evidence. Also created and inspected `build/visual-quality-review/dialogue-before-after.png`, comparing the earlier awkward opening wraps/sign with the current patch.

2026-11-02: corrected ending mode `0x63` reaches the English summary on the current disc. Captures at frames 1000, 2000 and 3000 show its beginning, middle and final paragraph fitting the scrolling area. The following staff-roll capture at frame 5000 exposes untranslated Japanese credit sprites, now tracked in T-39529. The earlier mode-`0x72` run is rejected as ending evidence. Do not treat clean font tracing as proof that the ending artwork is translated.

2026-11-02: the owner approved leaving the original ending credits unchanged. T-39529 is closed as an approved retention decision, not a completed translation. Gameplay dialogue, menus and the remaining long-page checks continue unchanged.

2026-11-02: resolved the longest pending record, EPILOG table 3/group 0/line 0. The ending selector's girl-12 case selects table 3, entry 0. Captured RAM at frames 1500 and 3000 contains all 17 epilogue rows, matching the approved translation after joining row boundaries, within a 23-row/726-byte composed summary with zero overflow. Inspected native captures at frames 1000, 1500, 2000, 2500, 3000 and 3500: the passage scrolls through to its complete final sentence. Receipt: `translations/en/reviews/ending-long-page-2026-11-02.json`. This is controlled scene-entry evidence, not a natural ending playthrough. The long-page queue now has 61 records awaiting runtime verification.

2026-11-02: verified KG_S6W group 30/line 4 in the current-disc Mira drama scene. Stage 0x66's return path loads her portrait, the auditorium and the script before the controlled line selection. Native captures at frames 180 and 600 show the four-row passage scrolling to its final line and closing quote without clipping. The loaded 128-byte header matches KG_S6W read from the disc; font tracing reports zero original-font, unknown or overflow calls. Receipt: `translations/en/reviews/mira-festival-long-page-2026-11-02.json`. Sixty long-page records still await runtime verification.

2026-11-02: NI_S6S group 1/line 0 has two reply variants. The initial test displayed the short acceptance reply; the longer refusal requires talk_choice=1. Inspected the refusal at frames 180 and 600, then advanced with Circle at frame 700. Frames 800/1000 show a cleared text box with no stale letters. The loaded header matches the disc. Recorded dialogue-only evidence in `translations/en/reviews/saki-festival-long-page-2026-11-02.json`; festival artwork remains rejected under T-39530. The long-page queue now has 59 pending records.

2026-11-02: inspected 39 captures across two 6,000-frame school-trip runs on published target b8b5be606a64650203c4b6c452fb8f61cdadef83. Controller input advances the scene after entry; the continuation has no additional setup writes. Departure, hotel, temple and corridor captures show no clipped or overlapping text. Completed dialogue closes correctly; destination labels and visible backgrounds are legible. The invitation at sightseeing frame 2700 splits "day / after tomorrow" and needs a source/width check before layout changes. Receipt: `translations/en/reviews/school-trip-visual-2026-11-02.json`. This scene survey does not certify additional flagged long-page records; 59 remain pending.

2026-11-02: verified SHUGAKU table 10/group 37/line 0 in a controlled line replay. The loaded text/command/voice pointers identify table 10, and the 128-byte container header matches the published disc. Captures at frames 150/300/450/600 show the four-row passage scrolling through its final sentence and closing quote. This checks dialogue rendering in the hotel fixture, not the Hokkaido background. Added the proof to `school-trip-visual-2026-11-02.json`; 58 flagged long-page records remain pending.

2026-11-02: verified group 37/line 0 separately in SHUGAKU tables 0 through 9. Inspected each passage near its start and after scrolling through its final sentence and closing quote. All 100 runtime samples match the selected table pointers/group and the published disc header. The fixture uses the hotel scene with controlled table/line selections; it does not prove natural destination traversal or artwork. Receipt: `translations/en/reviews/trip-table-variants-2026-11-02.json`. The queue now has 48 pending long-page records.

2026-11-02: also inspected SHUGAKU table 11/group 34/line 0 and table 12/group 14/line 0 through their closing quotes. All 20 memory samples confirm the selected pointers/group and disc header. Both payloads are unchanged between the saved-state disc and the current c3706ff4c86b build. Added the scoped proof to `trip-table-variants-2026-11-02.json`; 46 long-page records remain pending.

2026-11-02: inspected both pending TEL passages in `build/emu/phone-long-pages`. The answering-machine message and relationship-report follow-up scroll through their final sentences and closing parentheses without overlap. All 49 samples from frame 720 onward match both relevant function bodies read from the published TEL.EXN. This uses controlled stage/substep selection, not natural call triggering. Receipt: `translations/en/reviews/phone-long-pages-2026-11-02.json`. The queue now has 25 verified records and 42 pending. Yukari's mapped ending probe reaches her close-up and opening dialogue; a controller-only continuation is running for the three pending table-9 passages.

The ending-specific viewport correction revealed 20 additional three-row ending records that scroll in its two-row area. Updated the long-page queue for candidate `c0dafc36074dd66d6e6debb5b7d14d0f44e6649a`: 87 records total, 25 previously verified and 62 pending. Verified payload hashes remain unchanged; only five already-pending ending payloads changed. New ending replays are not counted until their complete passages have been inspected.

The local review gallery now contains 439 captures across 54 contact sheets, including all 196 reviewed frames from the wider-column ending candidate and the Japanese face comparison. The ending section is explicitly marked superseded and not approved; the latest layout refinements still need runtime verification. The gallery's pending count is updated to 62 of 87 flagged records.

On the current ending candidate, two additional long passages are visually checked with complete-text samples and capture hashes (`ending-long-pages-2026-11-02.json`). The targeted queue is 27 of 87 inspected, with 60 pending. The local gallery now contains 454 captures across 56 sheets, with the current ending checks separated from the superseded candidate. Gallery size is evidence volume, not a count of approved passages.

The current ending continuation clears four more flagged records, bringing the queue to 31 inspected and 56 pending. All six flagged Yukari ending passages now have completed-text and screenshot evidence. The gallery contains 516 captures across 63 sheets. T-39534 is complete; other endings, routes and the remaining sign checks still require inspection before whole-game sign-off.

Started `build/emu/ending-scoped-nozomi` against the published a839441add3e build. The fixture selects Nozomi before the ending scene loads; table 10 is bound by the game's own heroine dispatch. Captures and text-state samples run every 30 frames through 24,000 frames, with no controller input after frame 4,200. No additional records are approved until the completed passages are inspected.

The 56 pending records contain 51 distinct payload/window pairs. These are still individual pending records; identical bytes alone do not grant runtime approval. Source tracing found an unvisited Kagami arrival event selected by `date_scene_choose` when scene flag 51 is clear. Started `build/emu/mira-park-no-special` with that event marked visited to inspect the ordinary park date. This is a controlled fixture change, not a patch change or proof of natural event ordering.

## Continuation repair review

Inspected 214 new ending captures across 24 sheets: Nozomi's complete dialogue and Rei through line 38, including the longest continued speech. Added five flagged-record approvals (36 of 87, 51 pending). The local gallery now contains 999 captures across 108 sheets, including historical defect evidence; these totals are not all unique scenes or current-build approvals. Receipts specify each build. Rei's tail and fresh park-variant checks continue. A follow-up build containing two further line-break corrections terminated with exit code 137 during concurrent replays and will be retried after they finish.

## Camp long-page follow-up

Inspected 20 captures across three contact sheets from `build/emu/camp-long-followup` on the published track. GASSHUKU table-4 group 5 line 6 and group 6 line 1 scroll cleanly from their opening rows through closing punctuation. Table pointers, post-dispatch line counters and fully revealed text were checked. The controlled fixture retains Kagami's name binding in the second passage; this is layout evidence, not approval of natural scene staging or its heroine binding. The receipt states that limit explicitly: `translations/en/reviews/camp-long-followup-2026-11-02.json`. The long-page queue now has 44 pending layout checks; scene-specific name binding still needs a natural-route check.

## Published-build gallery update

Added nine groups from the published build's hashed review receipts: Yumi's date replies, festival invitation/walk/visit, New Year and event dialogue, the Ayako and camp wrap fixes, and the camp scrolling follow-up. Each copied capture matches its receipt hash. The gallery now contains 1,209 captures on 139 sheets, including 60 newly added captures on 13 sheets. Historical runs remain labeled with their own build and scope; the total does not mean 1,209 unique current-build approvals. The local gallery is `build/visual-review-pack/index.html`.

## Camp scene binding and Yumi long pages

The camp initializer binds Nozomi's correct name and portrait. Controller-only progression from that initialized scene reaches table-4 group 5 line 6; its full scrolling passage is clean. A later controlled selection verifies group 6 line 1 with the full Kiyokawa-san substitution. The earlier Kagami-name limitation is resolved for these two records. An attempted selection during unfinished speech produced stale glyphs in the fixture; those frames are excluded, and the first passage uses the controller-driven run instead. See `translations/en/reviews/camp-nozomi-scenes-2026-11-02.json`.

Two pending EVENT_SA passages also pass actual-renderer scrolling checks, including their closing punctuation and intentional thought breaks. Their receipt, `translations/en/reviews/yumi-event-long-pages-2026-11-02.json`, retains the limitation that the controlled fixture uses an existing date background. Active table pointers, line counters and full reveal were checked for all four passages. The long-page queue now has 42 pending layout checks. No game bytes changed.

## Seasonal long-page batch

The remaining flagged New Year passage and three White Day passages pass scrolling checks. Receipts are `translations/en/reviews/new-year-long-page-2026-11-02.json` and `translations/en/reviews/white-day-long-pages-2026-11-02.json`; they record actual tables, full reveal, screenshot hashes and the limits of controlled scene selection. The White Day receipt also verifies the disc-loaded script header. One further White Day passage splits a short exclamation awkwardly and remains pending under T-39537. Its exact-record wrap correction is prepared but needs rebuilt-disc visual verification. The long-page queue has 38 pending records.

## White Day wrap retest

The flagged White Day exclamation now stays on one row in the rebuilt disc. Inspected the full passage after reloading its script through the CD loader; the closing quote and scrolling rows are complete. `translations/en/reviews/white-day-wrap-verified-2026-11-02.json` records the screenshots and full-disc comparison. Only WHITE.BIN changes from the published disc, and the corpus proof preserves unrelated dialogue. The queue now has 37 pending layout checks. The new build is local; the public patch remains unchanged.

## White Day build published

Published `fd2f74c0a3bb0cc853ed7849ec6bffdf9dc84c5f` after all 15 browser checks passed. Pages deployment 41581825760 succeeded, and all 36 served files match the tested package. `translations/en/reviews/published-white-wrap-2026-11-02.json` records the release hashes and verification. The visual review queue remains at 37 pending records.
