---
id: T-39500
title: Build and publish the English translation patch
status: In Progress
assignee: claude
created: 2026-11-01
updated: 2026-11-02
links: ["[[tickets/T-39500-english-translation-patch]]"]
---

## Goal

Ship an English translation patch for the original disc (PlayStation the Best, SLPM-86053): the decomp's English build, our English text and art, a disc rebuild, an xdelta patch and a browser patcher on GitHub Pages. Text and art are laid out without the original's width limits, because the code is ours to change.

## Acceptance criteria

- [x] Playable slice: title, menus, name entry (English letter page by default), the opening and the first school day in English, booting in the emulator, patched by a local copy of the patcher page.
- [ ] All dialogue and system text in English.
- [ ] All registered English art in place.
- [x] Scripted emulator runs with screenshots across the main screens; patcher tested in a headless browser against the command-line build.
- [ ] No Japanese is left on screen anywhere in the game, apart from the owner-approved exceptions: the title logo (TITLE frame 0 and the TOKIMEKI.BS splash), the magazine masthead (mainrom-0118), tiny authentic signs in stills (catalog-135, 138, 140, 148, 150), the illegible roster board P_01_02, the off-frame ride sign (catalog-142), the original ending staff credits (T-39529), and the kana and kanji keyboard pages of name entry (kept so players can still type Japanese names; the English letter page is the default and every tab label is English). Checked by [[tickets/T-39510-no-japanese-left-check]].
- [ ] Owner review and sign-off, then GitHub Pages, README link and repository description.

## Notes

Parts: [[tickets/T-39501-disc-rebuild-and-xdelta]], [[tickets/T-39502-english-text-runtime]], [[tickets/T-39503-text-container-writers]], [[tickets/T-39504-emulator-screen-route]]. Out of scope for the first release: voice, the opening movie credits, `SPEAKTST`. The normal build must still match 27/27; English changes sit behind a build flag.

## Comments

2026-11-02: browser-built playable slice is proven by `build/emu/route-en-browser-slice`: fresh boot through first school day, 24,063 frames, 555 s, 82 screenshots, zero original-font uploads/unknown/overflow. It uses the actual browser-download track and CUE from the passing nine-check Chromium run, target SHA-1 e00ffdf0b44b94ad97168b425c1ce564dad04b9f. Main route separately reaches save/load in 33,649 frames with 146 screenshots. Remaining blockers include long presets, whole-game text/art coverage (new omitted rink billboard T-39513), broader event checks and owner sign-off. No publication yet.

2026-11-01 (Codex): resumed after the Claude access error. Integrated recovered script and art branches (5074abaf2 plus the following art merge). Combined build, visual verification, remaining name-entry/text/art work and final audits are still pending. Original script worktree notes are preserved.

2026-11-02 publication update: the owner authorized publishing the review build. GitHub Pages, the README link and repository description are live. T-39527 adds tested local drag/drop support for raw disc images, CHD, ZIP and 7z. Full longer presets, the speaker gap/color, perspective plaque and phone farewell fixes are also complete. The boundary candidate has passed all 134 route captures and three controlled festival scrolling checks. Whole-game text/art coverage and final quality sign-off remain open; review publication does not close those requirements.

2026-11-02 verification closeout. The published target remains `fbb1eb25a60fbcc51ab308c15c9d7e19777a48e4`. All 87 flagged long dialogue records have runtime layout evidence; all 798 distinct generated phrase-boundary pairs and 39 remaining early-break cases have reviewed dispositions. The artwork pass inspected 254 shipped sprite targets and 16 decoded stills. The local dialogue gallery contains 1,482 captures on 174 sheets, including historical comparisons and repeated frames. These counts do not imply an end-to-end playthrough of every route. The GitHub description and homepage both link the patcher. PR 71 now describes the current build and verification scope. Final parent-ticket reconciliation remains open; the patch itself is already published.
