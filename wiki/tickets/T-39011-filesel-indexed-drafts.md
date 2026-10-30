---
id: T-39011
title: Produce FILESEL button and page drafts
status: In Progress
assignee: Claude (artwork worker)
created: 2026-10-29
updated: 2026-10-29
links: ["[[tickets/T-39004-english-asset-completion]]"]
---

## Goal

Produce English drafts for FILESEL buttons 0-3 with source-fit gradient reconstruction, and a region map for information pages 0-2.

## Acceptance criteria

- [ ] Draft candidates produced by a Docker-only, deterministic tool with source/output hashes, edit regions and glyph provenance; pixels outside edit regions equal the exact source.
- [ ] Docker tests (cache-free synthetic plus original-data rebuild) and mutation tests pass; translation_inventory check passes.
- [ ] Root defaults recorded as revisable decisions; agent review kept separate from human approval (none claimed).
- [ ] Code review against CODING_STANDARDS.md with all findings resolved.

## Notes

Design: judged plan from design workflow wf_7f549dff-def (2026-10-29).

### 2026-10-29 build (artwork worker)

- Tool `tools/translation_filesel_indexed.py`, tests `tools/test_translation_filesel_indexed.py` (6 cache-free synthetic + 6 original-data). Docker: `docker run --rm -v "$PWD":/work -w /work/tools tokimemo-recovery-splat python3 -m unittest test_translation_filesel_indexed` -> 12 OK. `python tools/translation_inventory.py check` still exits 0 (inputs unchanged).
- Buttons 0-3: removed 177/178 inside the 40x16 bevel interior; ordinal half-plane ramp model (p = 100x + n y). Leave-out gate (other buttons' lettering masks shifted -3/0/+3): 3692 held-out texels, 0.9458 index-exact, max 1 band, slopes 0.79/0.76/0.79/0.79. Reconstructed texels per frame 87/66/88/90, all flagged for magnified human review. NativeLatin 12 rows unscaled, gap 2 for all four words, one shared baseline. Source outline-rule deviations recorded: frame 0 missing 1, frame 2 extra 2, frames 1 and 3 exact (tested).
- Round trip in memory only (no patched NBN written): 1388 bytes changed, all inside declared rects; CLUTs and size unchanged; probe supported; 139 other exports byte-identical to original-filesel-v1; the original data re-exports to the cache exactly.
- Pages: region map `translations/en/graphics/filesel_page_regions.json` (human_confirmed false). Exact native-glyph cells: name, nickname, year/month/day suffixes, count suffix. Missing with reason: club (third glyph unmatched), clear-count label (hand-drawn katakana), system tab (hand-drawn), button panel (gradient lettering). String origin (160,120) inferred from label rows and digit positions; labels end before x 192, separators clear the digit sprites.
- Published `translations/en/graphics/artwork/checkpoints/T39004-filesel-v3/` (7 PNGs, manifest, ledger, separate review.agent.json; human_approved false everywhere). Status proposals for the integrator: `translations/en/graphics/decisions/filesel.json` (frames 0-3 and page 2 candidate-only; pages 0-1 stay missing with partial drafts).
- Frame 19-31 v2 drafts untouched.

Revisable decisions (root defaults):
1. Button words Copy/New/Clear/Load from catalog rows 99-102 (row 101 offers Clear / Delete; suggested graphic Clear). Order verified from source frames 0-3.
2. 1-band reconstruction errors accepted for candidate status because the leave-out gate passed; flagged for magnified human review.
3. Page candidates only for exact glyph-matched cells; date row uses "/" in the year and month cells, day cell blank, code-drawn digit positions kept; count suffix blanked (catalog row 103).
4. Region map built without human confirmation (human_confirmed false).

Open: page/sheet runtime key in translated_image_store.gd (follow-up ticket); human visual approval; CODING_STANDARDS review (not done). Reviewer note: the tool keeps the matched Japanese glyphs as data-table keys (no game text in names or comments).

### 2026-10-29 review fixes (artwork worker)

Agent review found four issues; all four were real and are fixed. No human approval is claimed.
- Untested guards: the tool now has `button_interiors`, `check_gate` and `check_exports` as separate functions. Four checks that could never fire were removed and replaced by comments: the kept-texel check, the outline-on-border check and both index-set-grew checks. The button and page code never writes outside removed or new texels, so those checks could not fail. 12 negative tests were added, and the original-data test now checks changed_bytes 1388, that verify_patch is called once, and the exact edit_regions. Mutation pass: each of the 27 `raise ValueError` guards was replaced with `pass`, and the verify_patch call was replaced with a constant. All 28 mutants fail the suite. Suite: 24 OK.
- Ledger ids: frames are now `filesel-frame-N.base`, which candidate_key already accepts, and page 2 is `filesel-page-information-2`. `decisions/filesel.json` `integration` gives the integrator the exact candidate_key line to add for page ids and the atom_overrides to change. A test builds the inventory from a temp copy of the decisions with that change, and all five items come out candidate-only.
- Partial pages: pages 0 and 1 (candidate false) are no longer published. They stay in the build output only. The ledger now has 5 assets, plus `unpublished_partial_builds` and a `source_pixels_note`. The checkpoint was republished; the 5 kept PNGs are byte-identical to before. New ledger sha256 8ab457d3..., manifest fe5653e5...
- CLI: added `--nbn` and `--exe`. Their defaults are the old fixed paths.

## Comments
