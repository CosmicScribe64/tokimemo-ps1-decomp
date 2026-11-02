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
