---
id: T-39500
title: Build and publish the English translation patch
status: In Progress
assignee: claude
created: 2026-11-01
updated: 2026-11-01
links: ["[[tickets/T-39500-english-translation-patch]]"]
---

## Goal

Ship an English translation patch for the original disc (PlayStation the Best, SLPM-86053): the decomp's English build, our English text and art, a disc rebuild, an xdelta patch and a browser patcher on GitHub Pages. Text and art are laid out without the original's width limits, because the code is ours to change.

## Acceptance criteria

- [ ] Playable slice: title, menus, name entry (English letter page by default), the opening and the first school day in English, booting in the emulator, patched by a local copy of the patcher page.
- [ ] All dialogue and system text in English.
- [ ] All registered English art in place.
- [ ] Scripted emulator runs with screenshots across the main screens; patcher tested in a headless browser against the command-line build.
- [ ] No Japanese is left on screen anywhere in the game, apart from the owner-approved exceptions: the title logo (TITLE frame 0 and the TOKIMEKI.BS splash), the magazine masthead (mainrom-0118), tiny authentic signs in stills (catalog-135, 138, 140, 148, 150), the illegible roster board P_01_02, the off-frame ride sign (catalog-142), and the kana and kanji keyboard pages of name entry (kept so players can still type Japanese names; the English letter page is the default and every tab label is English). Checked by [[tickets/T-39510-no-japanese-left-check]].
- [ ] Owner review and sign-off, then GitHub Pages, README link and repository description.

## Notes

Parts: [[tickets/T-39501-disc-rebuild-and-xdelta]], [[tickets/T-39502-english-text-runtime]], [[tickets/T-39503-text-container-writers]], [[tickets/T-39504-emulator-screen-route]]. Out of scope for the first release: voice, the opening movie credits, `SPEAKTST`. The normal build must still match 27/27; English changes sit behind a build flag.

## Comments

2026-11-01 (Codex): resumed after the Claude access error. Integrated recovered script and art branches (5074abaf2 plus the following art merge). Combined build, visual verification, remaining name-entry/text/art work and final audits are still pending. Original script worktree notes are preserved.
