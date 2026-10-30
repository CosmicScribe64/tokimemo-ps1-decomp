---
id: T-39012
title: Produce BS background sign drafts
status: In Progress
assignee: Claude (artwork worker)
created: 2026-10-29
updated: 2026-10-29
links: ["[[tickets/T-39004-english-asset-completion]]"]
---

## Goal

Census the BS still backgrounds, build a macroblock splice tool, and produce English drafts for the backgrounds where it is feasible, plus LASTPHOT ALL_C.

## Acceptance criteria

- [ ] Draft candidates produced by a Docker-only, deterministic tool with source/output hashes, edit regions and glyph provenance; pixels outside edit regions equal the exact source.
- [ ] Docker tests (cache-free synthetic plus original-data rebuild) and mutation tests pass; translation_inventory check passes.
- [ ] Root defaults recorded as revisable decisions; agent review kept separate from human approval (none claimed).
- [ ] Code review against CODING_STANDARDS.md with all findings resolved.

## Notes

Design: judged plan from design workflow wf_7f549dff-def (2026-10-29).

### 2026-10-29 worker notes (bs-backgrounds)

- Tools: `tools/assets/mdec_splice.py` (per-macroblock BS v1/v2 span parser, VLC writer, anchored RD encoder scored in exact RGB555 through `image_mdec_model`, float-IDCT cross-check, rebuild that keeps original trailing bytes and refuses to overwrite non-zero ones, hard fail on v3) and `tools/translation_bs_drafts.py` (`census`, `build --output <.asset-cache dir> [--publish <new dir>]`). Tests: `tools/test_translation_bs_drafts.py`, 12 tests (10 synthetic cache-free incl. v3 refusal, trailing-byte refusal, tamper, clip refusal; 2 original-data incl. determinism, publication match and a crop mutation that must not publish).
- Census: all 22 decoded files are BS v1; the no-edit splice reproduces every file byte-exact. Catalog bindings are rotated: brass-band banner is in D_29_09, ride sign in D_11_11, museum monument in D_15_03, entrance/MAIKO in D_19_02, library aisle sign in I_17_01, science-club plaque in D_01_69, computer-club banner in D_01_70. I_11_15, D_01_67 and D_11_12 have no legible text. D_29_08 holds an uncatalogued congratulation banner (the design proposal had D_11_12/D_29_08 swapped). The ticket booth sign of catalog-145 was not found (I_17_01, D_11_10, D_11_12 checked). All bindings with hashes: `translations/en/graphics/decisions/bs-backgrounds.json` (for the integrator; raw catalog untouched; needs human confirmation).
- Geometry: only P_01_02, P_01_03, P_01_03B and C_01_28 have caller proof; all other rows stay missing (geometry unresolved). Census decodes of the rest use the macroblock-count shape for inspection only.
- Drafts (unreviewed, human_approved false): C_01_28 chalkboard "FINAL / EXAMS" (6 macroblocks re-encoded, outside bit-identical, ring max 1, float cross-check 1, 19298 bytes within the 10-sector read) and ALL_C.NBN banner "ALL CLEAR CONGRATULATIONS!" through the caller-bound photo-page profile with existing palette indices only. Published crops, manifest and ledger: `translations/en/graphics/artwork/checkpoints/T39004-bs-v1/`; agent review separate in `T39004-bs-v1.agent.json`. Full decodes, `C_01_28.en.BS` and `ALL_C.en.NBN` live only in `.asset-cache/translation-assets/bs-drafts-v1/`.
- Blocked: P_01_02 headers (see the review-fix notes below; the original reason here was wrong about the trailing bytes).
- Root defaults applied, all revisable: no disc patch; edit region = edited macroblocks; tiny signs and P_01_03/03B calligraphy untranslated; strict +-1 ring; crops-only commit; census bindings as decisions.
- Not done: port MDEC replacement path (separate code ticket), code review against CODING_STANDARDS.md, human visual review. Nothing committed.

### 2026-10-29 review fixes (bs-backgrounds)

- Correction: P_01_02 has 1022 trailing bytes, of which the first 1018 are zero and only the last 4 (69 71 12 10, file offset 18428) are non-zero. The stream can grow by up to 1018 bytes. `mdec_splice.rebuild()` now lets the stream grow into that leading zero run, keeps the non-zero bytes at their original offsets, and records `leading_zero_bytes`. Growth past the original file end (no non-zero trailing bytes) is now its own policy and sets `file_extent_grew`; C_01_28 grows 566 bytes past its 18732-byte extent and still fits the 10-sector read.
- P_01_02 sweep is now reproducible: `python translation_bs_drafts.py sweep` (plan `P01_02_SWEEP`: headers read 1-A / 1-B / 1-C, lambda 8/12/16/24/32/50 x ring weight 50/200/1000, about 4.5 minutes). All 18 runs fit (stream 17356-17532 of 18428 bytes); none passes: ring max 6-7 against +-1, float-IDCT 31 (fixed-point wrap near the saturated white paper). catalog-130 stays missing, now for the right reason.
- ALL_C: one font instance for both drafts, so `font_provenance.glyphs` covers every drawn glyph (C, G, O, R, T, U, ! were missing). Text now centres vertically on the original lettering rows 205-222 (text rows 209-217) instead of the whole strip. Checks factored into `indexed_failures()`.
- `publish()` checks every crop hash first and assembles the checkpoint in a `.partial` sibling that is renamed into place; a failed publish leaves nothing.
- Tests: 24 in `test_translation_bs_drafts` (gate negatives, indexed-check negatives, zero-run growth, past-end growth flag, reader validation, changed source BS, ALL_C crop tamper, glyph coverage). Mutation spot-check: all nine targeted guards are killed.
- Checkpoint `T39004-bs-v1` republished (ALL_C crops and manifest changed; C_01_28 crop unchanged); agent review updated, still not human approval.
- Decisions: catalog-147 stays candidate-only with `coverage: partial` (inventory has no partial status); new revisable default D-7 leaves the C_01_28 month/day/weekday column in Japanese. catalog-135 reason corrected: the D_01_69 plaque is a legible ~80x20 sign, D-3 applied by name only and should be revisited by a human.
- Still missing: C_01_28 month/day/weekday placeholder column (D-7); P_01_02 headers; the 10 geometry-blocked signs; D_29_08 banner; catalog-145 booth sign; port MDEC replacement path; code review and human review.

## Comments
