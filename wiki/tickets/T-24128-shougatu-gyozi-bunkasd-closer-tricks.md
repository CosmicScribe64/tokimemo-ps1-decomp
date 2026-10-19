---
id: T-24128
title: Apply SHUGAKU girl-picker recipe and uopt CSE priority tricks to SHOUGATU, GYOZI, and BUNKA_SD
status: In Progress
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24122
  - T-24125
---

## Summary

Apply the newly discovered `T-24122` girl-picker loop recipe (`if (1) {}` + `s == best_s && s == best_s &&` + `(best_i & 0) + best_i`) and `T-24119` / `T-24125` `uopt` CSE / `ugen` temp-ring / `bittab` seeding tricks across `SHOUGATU`, `GYOZI`, and `BUNKA_SD`:
- `SHOUGATU:func_8013288C` (`src/ovl/SHOUGATU/80132550.c`, girl-picker loop: `$s2/$s3` swap between `best_s` and `s` and redundant `s == best_s` re-check!)
- `GYOZI:func_801347D4` (`src/ovl/GYOZI/80134520.c`, girl-picker loop: `$s0/$s1/$s2/$s3` allocation order for `i/best_i/best_s/s`)
- `BUNKA_SD:func_80139290` (`src/ovl/BUNKA_SD/801388D0.c`, 379/379 insns, `0x3B` RLDC live-range coloring `$a1` vs `$t1`) and `BUNKA_SD:func_80134D94` (`src/ovl/BUNKA_SD/80134540.c`, `bittab` seeding for `i` vs `p1..p4`)
- `SHOUGATU:func_801347E0` (`2` rows), `SHOUGATU:func_8013D820` (`score=13`), `SHOUGATU:func_80139334` (`score=17`), `SHOUGATU:func_80137664`, `SHOUGATU:func_8013D92C`, `SHOUGATU:func_8013FB28`, `GYOZI:func_801421B4`
