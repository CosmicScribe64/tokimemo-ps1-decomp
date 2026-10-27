# Roadmap

The goal is a playable, readable, English-translated version of *Tokimeki Memorial, Forever with You* that does not
depend on the original code. It comes in five phases, in order. Later phases depend on earlier ones, and none has a date.

## 1. Byte-matching decompilation (complete)

Write C for every function so that the rebuilt executable and overlays are identical to the originals. All 6,952 game functions are C, and every binary rebuilds byte for byte.

## 2. Readable decompilation (complete)

All 437 game C files have logical names, shared declarations are organized into topic
headers, and essential game state, locals, constants, and behavior are documented.

## 3. Godot port (in progress: readiness repairs)

Independent review found that several live scenes skip interaction, original audio is replaced by generated tones, and ending/subgame flows remain incomplete. Existing unit tests and static image comparisons do not establish a fully playable port. See [the readiness review](wiki/port-readiness-review.md) for findings and the actual-route verification gate. Phase 4/5 expansion is held while these baseline issues are repaired.

## 4. Modernize the port (in progress)

Widescreen support, modern desktop settings, and native achievements are planned in [wiki/phase4.md](wiki/phase4.md) (`T-30000`) and ready for implementation while retaining an original `4:3` (`320x240`) option:

- [Widescreen presentation](wiki/tickets/T-30001-modern-widescreen.md) (`P4.1`, Ready) covers `16:9` (`426x240`), `16:10`, and `21:9` aspect-preserved scene framing (`320x88`, `320x160`, and `320x240` safe-frame apertures), integer/linear scaling filters, and responsive HUD/dialogue layouts.
- [Modern settings](wiki/tickets/T-30002-modern-settings.md) (`P4.2`, Ready) covers graphics/display (with 15s/60s revert confirmation and crash-recovery sentinel), input remapping with dynamic button glyphs, 5-bus audio and accessibility controls, and independent text/translation-pack and voice-pack selectors.
- [Achievements](wiki/tickets/T-30004-modern-achievements.md) (`P4.3`, Ready) covers 40+ deterministic, spoiler-aware route, confession, club/parameter, and minigame achievements with persistent unlock tracking and toast/gallery UI.
- [Player customization](wiki/tickets/T-30003-player-customization.md) (`P4.4`, Backlog stretch goal) covers playing as a girl, with gendered references updated throughout dialogue, events, and presentation and explicit art/voice fallback policies.

## 5. English translation (complete, not implemented)

AI translation (with Gemini 4 Argon) of game text into English is complete. Ideas for translated images are tracked but not yet created.

The first translation will come from an AI, and the game and the repository will say so. Human translations are
wanted, and they will replace the AI text where they exist.
