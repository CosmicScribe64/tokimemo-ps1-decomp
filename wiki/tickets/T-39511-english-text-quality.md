---
id: T-39511
title: Make the English read naturally on screen
status: In Progress
assignee: claude
created: 2026-11-01
updated: 2026-11-01
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

2026-11-01 (Codex): owner flagged the opening classroom reply: missing closing quote, crowded quote/first letter, and a split parenthetical thought. The original prologue table record separates the thought after the spoken sentence. `english_sndisp` lacked closing marks (the prior fix covered talk scripts only). Added closing marks with thought handling, a separate trailing-aside paragraph in the wrapper, and one extra pixel of quote advance. Fresh emulator verification pending; wording unchanged.

2026-11-01 (Codex): fresh capture `build/emu/route-en-quotes/037_prologue_09.png` visually verifies all three reported issues fixed. Full 145-capture review reaches save/load and identifies dynamic date-confirmation overflow in capture 115; runtime wrapping added, awaiting a new route. Name-entry art fixes are in a separate newer build awaiting GPU verification. No wording changes made.

2026-11-01 (Codex): `build/emu/route-en-final-layout` completed 33,649 frames and 146 captures through save/load. Capture 037 verifies the closing quote, quote/letter spacing and complete second-row parenthetical; capture 115 verifies the dynamically assembled date confirmation including its closing quote. Capture 024 verifies all birthday digits alongside the English panels. The original screenshot's defects are resolved; whole-script and off-route review remain open.
