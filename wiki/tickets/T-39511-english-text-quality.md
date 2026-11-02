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

2026-11-02: actual-disc audit isolates three unresolved tab-corrupted festival records (AS_S6W g0078/g0079 l0000, HI_S6 g0176 l0010). Original raw prose decodes coherently when the stray tab is treated as spacing; renderer extraction instead paired it with a Shift-JIS lead byte and protected the following fragments as opaque tokens. Implement explicit per-record English repairs guarded by both original-byte and existing-target hashes. Keep raw source and prior translation rows intact, document each exception, and require a fresh clean disc scan before acceptance.

2026-11-02: all four opaque-token dialogue cases are repaired with source/previous-target hashes: three malformed tabs and one trailing apostrophe. The rebuilt disc passes exact payload/original-font auditing for every script/profile string. Actual festival/baseball wording captures are still required; these corrections are not yet a full text-quality completion claim.

2026-11-02: visually verified both AS_S6W anomaly repairs with actual BUNKAKEN assets, dialogue tables and renderer on `build/anomaly-repair-proof`. Captures `build/emu/festival-repair-g0078/shot_600.png` and `build/emu/festival-repair-g0079/shot_600.png` show complete three-line sentences, intact quotation marks and no text-window overflow. Each controlled 600-frame replay reports zero original-font calls/unknown/overflow. HI_S6 and KOSIEN repair captures remain pending; group selection was injected and logged, not reached by normal career play.

2026-11-02: HI_S6 g0176 l0010 now also has a clean GPU proof: `build/emu/festival-yuina-clear-g0176/shot_600.png`, two complete lines with intact quotes and no overlap; 600 frames, zero original-font uploads/unknown/overflow. The first forced-jump capture retained the prior line and is rejected as layout evidence. The corrected probe triggers the game's normal line-clear path before selecting the record. All three festival anomaly repairs now have controlled visual proof; KOSIEN's repaired baseball line still needs its capture.

2026-11-02: KOSIEN group 10 line 0 is visually verified in `build/emu/baseball-repair-g0010/shot_600.png`: three complete lines, one closing quote, no overflow. The 1,800-frame MASTER bootstrap and 600-frame controlled exact-line replay both report zero original-font calls/unknown/overflow. All four source-anomaly repairs now have GPU proof. Stadium plaques remain the explicitly approved catalog-138 exception.
