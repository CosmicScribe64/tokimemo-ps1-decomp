---
id: T-39010
title: Produce battle Miss compact lettering drafts
status: In Progress
assignee: Claude (artwork worker)
created: 2026-10-29
updated: 2026-10-29
links: ["[[tickets/T-39004-english-asset-completion]]"]
---

## Goal

Produce English drafts for the ten 16x8 battle Miss regions (frames 64/65 part 0) from original compact capitals, without scaling or invented glyphs.

## Acceptance criteria

- [x] Draft candidates produced by a Docker-only, deterministic tool with source/output hashes, edit regions and glyph provenance; pixels outside edit regions equal the exact source.
- [x] Docker tests (cache-free synthetic plus original-data rebuild) and mutation tests pass; translation_inventory check passes.
- [x] Root defaults recorded as revisable decisions; agent review kept separate from human approval (none claimed).
- [ ] Code review against CODING_STANDARDS.md with all findings resolved.

## Notes

Design: judged plan from design workflow wf_7f549dff-def (2026-10-29).

### 2026-10-29: drafts built and published (agent)

- Tool: `tools/translation_battle_drafts.py` extended (no parallel pipeline) with `CompactLatin`, `compose_miss`, `build_miss`, `publish_miss`; CLI `--mainrom ... [--publish ...]`. The existing `build()` is unchanged: a rebuild of the 30 command drafts is byte-identical to `.asset-cache/translation-assets/battle-drafts-v1/manifest.json`.
- Source verified in Docker: MAINROM.NBN sha f425e1d1..., sheet 0x16000 (low nibble first; equals `original-legacy-v1/mainrom.source-index-sheet-16000.png` at index*17) sha 313084e9.... Frame 118 part 19 draws u0 v0 112x32 of it (the logo line). Pinned masks with a clean 1-px zero ring: M x17 y21 3x5, I x37 y21 1x5, S x78 y21 3x5 (x68 is large art, not S). The second M at x25 overlaps other art and is not used.
- No non-H M: an exhaustive search of all ten MAINROM sheets for isolated 4/5-wide M shapes found none; the judge-reported STADIUM M at x97 y74 is a decorative banner, not a clean glyph. So only variant A is produced; the 3-wide M may read as HISS (needs human readability check).
- Regions: frames 64/65 part 0, page0 u40 v72 / u40 v80, 16x8, index sha 5e799a76... / ada7cff2..., identical in all five containers. Inks 0x10/0x12 and 0xEE/0xEF (STP range, runtime risk). HP reference frame 8 part 0 (u96 v56) pinned, 2 hand-placed shadow pixels beyond the diagonal.
- Composition: MISS, 1-px tracking, ink bottom on source row 6, pure (+1,+1) shadow with the region shadow index. Patched container copies re-parse with identical tables and palette; 146 changed bytes per container, all inside the two rectangles; decoded crops equal the drafts. Patched VS_SIKA 5410a21c... and VS_UFO 220b2663... reproduce the design experiment.
- PARK.BIN, UTYU.BIN, VS_PARK.BIN, VS_UTYU.BIN hold the same two index blocks (file 0x2428/0x2828, verified row by row): scope note only.
- Outputs: `.asset-cache/translation-assets/battle-miss-drafts-v1/` (originals, drafts, `compare/*.x12.png` magnified comparisons, `review.agent.json`, manifest). Published drafts only (no originals) to `translations/en/graphics/artwork/checkpoints/T39004-battle-miss-v1/` (ledger sha 3a406e11...). All style unreviewed, human_approved false; agent review is separate and is not approval.
- Decisions for the integrator: `translations/en/graphics/decisions/battle-miss.json` (defaults D1-D8 recorded as revisable; catalog line 122 correction; `candidate_key`/LEDGERS integration note: until added, the ten items stay missing).
- Tests (Docker, tokimemo-recovery-splat): `python3 -m unittest test_translation_battle_miss test_translation_battle_drafts` 15 OK; `python3 translation_inventory.py check` exit 0. Full `test_translation_*` discovery: 82 run, 2 errors in `test_translation_tree` (missing `translations/en/system/dictionaries/slpm_86_053.md`), unrelated to this ticket.
- Not done: CODING_STANDARDS review; human decisions listed in the decisions file.

### 2026-10-29: review findings fixed (agent)

- Finding 1 (medium, accepted): seven fail-closed guards had no test that failed when the guard was removed. Fixes: per-container lettering moved into `letter_container()` so tests can call it; the publish refusals are split so each has its own message (`sidecar changed`, `claims approval` (now `is not False`), `built output changed`, `is not a derivative`). New tests in `tools/test_translation_battle_miss.py` cover a stray byte outside the Miss rectangles, MAINROM altered outside the sheet, an altered source region, a moved frame 65, a review claiming approval, an original PNG presented as the output, and an existing destination. Mutation check in an isolated copy (repo mounted read-only): each of the 9 guards, removed in turn, makes a test fail.
- Finding 2 (low, accepted, no lettering change): the draft reads as HISS. The M is authentic: it is the first letter of the logo word MEMORIAL. `review.agent.json` now says so and notes the lower contrast of frame 65. The readability decision is now a blocking item in `battle-miss.json` (`blocking_human_decisions`). Rebuilt into `.asset-cache/translation-assets/battle-miss-drafts-v1-r2/` (the first build is left untouched) and republished the checkpoint. The 10 PNGs are byte-identical; the manifest is now 119afdab..., the review ae7b36e0... and the ledger 2ef2651e....
- Tests: 22 OK (miss + command drafts); inventory check exit 0, still 78 missing; full discovery 121 run, the same 2 unrelated `test_translation_tree` errors.

## Comments
