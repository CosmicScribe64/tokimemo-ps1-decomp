---
id: T-39214
title: English WP-I: presenter English binding (date, phone, subgame, ending)
status: In Progress
assignee: Claude (phase 4 root, Lane A)
created: 2026-10-30
updated: 2026-10-30
links: ["[[english-names]]", "[[phase4-design-2026-10-29]]"]
---

## Goal

Bind English text and images on the date, phone, subgame and ending presenters with a strict byte-identity fallback.

## Acceptance criteria

- [ ] Hooks: bind_english / clear_english seams with tests (done in wave 1).
- [ ] Text rows: source-location-verified binding rows for DATE/DATE2/GEKO, TEL.EXN, RPG_BAT/TACO/TAIIKU and ENDING.
- [ ] Image keys: WP-B page/BS keys for EXAMINE, BATTLE VS_*, TAIIKU, LASTPHOT and D_/I_ signage, or design-approved gaps.

## Notes

Kept In Progress per `.asset-cache/phase4-patches/english-names.v2.notes.md`. Root hook 6 now calls bind_english(layer, [], images) / clear_english() from overlay_dispatcher.apply_language and playable_boot; with no binding rows yet, only image replacements bind.

## Comments
