---
id: T-39410
title: Fix the MAINROM English art after the owner review
status: In Review
assignee: worker agent
created: 2026-11-01
updated: 2026-11-01
links: ["[[tickets/T-39400-finish-missing-english-art]]", "[[tickets/T-39009-mainrom-indexed-drafts]]", "[[tickets/T-39004-english-asset-completion]]"]
---

## Goal

Redraw the MAINROM items the 2026-11-01 owner and reviewer notes sent back (Save labels, magazine banners, issue banners, genre tags, stat labels, game covers), fix the stale weekday bindings, and move the text dictionary to the approved stat names.

## Acceptance criteria

- [x] Save labels 57-59: "Save" in the Japanese rows on the lettering layer; the card layer (frame border and icon) is untouched; all six palette variants.
- [x] Banners 126-133: one bold 17-row face at one size, one wording rule (完成 Opens, 特集 Special, 紹介 Guide), one file per variant.
- [x] Issue banners 122-125: "Issue" restores 月号; digits even (the 1 is 3 wide), layout balanced.
- [x] Genre tags 134-145: 1 px margin on both sides, centred (owner decision 2026-11-01); `TAG_ALIGN` is the one switch.
- [x] Weekdays 82-88: `weekday-resolution.json` shows every variant resolves to the approved two-letter set; the twelve stale three-letter drafts are listed for the integrator to supersede.
- [x] Stat labels 61: Health, Hum., Science, Art, Sports, Trivia, Looks, Guts, Stress, left-aligned, clear of the arrow marker.
- [x] Covers 151-154: English titles (SUIKODEN, PARODIUS, PRO BASEBALL, TWINBEE PUZZLE) in each logo's colours; the rest of the cover art is unchanged.
- [x] Text dictionary: Health, Humanities, Sports in the three stat records and their three executable-offset twins.
- [x] Drafts reproducible with `tools/translation_mainrom_fixes.py`, tests in `tools/test_translation_mainrom_fixes.py`, checkpoint `T39410-mainrom-fixes-v1` with ledger and `REGISTER.md`; no shared registration file edited.
- [ ] Code review against `CODING_STANDARDS.md`.

## Notes

- Checkpoint: `translations/en/graphics/artwork/checkpoints/T39410-mainrom-fixes-v1` (192 PNGs, 32 frames x 6 variants). The integrator step is in its `REGISTER.md`.
- The SLPM tables the translation tree builds from are gitignored (`SLPM_*`), so the text change was made in `translations/en/returned-v3.jsonl` directly. The inventory pins that file's hash, so `test_translation_inventory` fails until `translation_inventory.json` is rebuilt (REGISTER.md).
- All new lettering is port-made and unreviewed; nothing is approved or integrated. The covers are thin 1 px capitals because eight letters must fit 40 px, flagged for owner review.

## Comments
