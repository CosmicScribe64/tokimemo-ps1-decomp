---
id: T-39547
title: Preserve TwinBee sprite texture attributes
status: In Progress
assignee: codex
created: 2026-11-03
updated: 2026-11-03
links: ["[[tickets/T-39511-english-text-quality]]"]
---

## Goal

Keep the original sprite attribute bytes that the text extractor decoded as a Japanese surname.

## Acceptance criteria

- [x] Exclude the attribute data from translation and verify original bytes and pointer binding on the rebuilt disc.
- [x] Inspect the affected minigame sprites and audit related sprite attribute tables.
- [ ] Pass release checks, review the change against coding standards and publish.

## Evidence

At TT.EXN offset 0x27B00, bytes 92 D2 00 00 decode as a surname. The fourth field of `tt_sprite_set_6`, at offset 0x27B10, points to these bytes. `TtSpriteSet.attr` is declared at offset 0x0C in `include/ovl/tt.h`. `tt_part_sprite_draw` in `src/ovl/TT/sprite_animation_and_followers.c` reads these bytes to select texture coordinates and texture pages. They are data, not a displayed label. The current English growth census proposes a 23-pixel translation for this record; screen-width review cannot validate this consumer.

## Correction and regression

The actual-disc comparison found three changed arrays, sprite sets 0, 6 and 8. All twelve arrays are now excluded from translation through the existing non-text ranges. A disc-backed regression follows the original and English sprite-set tables independently and compares each bound attribute array. It rejects the pre-fix build at exactly those three sets. Rebuilt verification follows below.

## Rebuilt verification

Target `39454a5b82bcc53f05daba81040e6622a85310e1` passes all twelve attribute comparisons and 29 focused tests. Complete disc/source audits pass: 2,802 exact executable payloads across 2,936 slots, 291 derived records, 286 pointers, 31,471 script strings and 1,964 source literals. All 27 default hashes and 15 browser checks pass. The language audit now recognizes the two restored Shift-JIS-looking attribute arrays by exact hash and symbol; its other classifications are unchanged.

Inspected all four native captures in `build/emu/twinbee-attributes`. The title and first-stage player sprite render cleanly; a second Start press pauses gameplay. These captures cover the player, not the other two corrected sprite sets. The title retains the original Japanese TwinBee logo, which needs reconciliation with the artwork inventory before full minigame sign-off. Metadata receipt: `translations/en/reviews/twinbee-sprite-attributes-2026-11-03.json`. Publication remains pending.

## Visual and standards review

Inspected all four first-boss captures and the native preview of sets 0, 6 and 8. Boss, projectile, player, HUD, game-over and ranking graphics are intact. Set 8 has no identified gameplay caller; its controlled preview verifies rendering only. Standards review found no unresolved issue. Original data is preserved through narrow exclusions, language exceptions require exact hashes, and the regression follows independent original pointers. Publication is pending. The original title-logo preference is a separate artwork question.
