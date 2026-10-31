---
id: T-39009
title: Produce MAINROM indexed English drafts
status: In Progress
assignee: Claude (artwork worker)
created: 2026-10-29
updated: 2026-10-30
links: ["[[tickets/T-39004-english-asset-completion]]"]
---

## Goal

Produce English draft replacements for the missing MAINROM.NBN magazine/HUD frames and weekday palette variants through the per-frame replacement key the port loader already uses.

## Acceptance criteria

- [ ] Draft candidates produced by a Docker-only, deterministic tool with source/output hashes, edit regions and glyph provenance; pixels outside edit regions equal the exact source.
- [ ] Docker tests (cache-free synthetic plus original-data rebuild) and mutation tests pass; translation_inventory check passes.
- [ ] Root defaults recorded as revisable decisions; agent review kept separate from human approval (none claimed).
- [ ] Code review against CODING_STANDARDS.md with all findings resolved.

## Notes

Design: judged plan from design workflow wf_7f549dff-def (2026-10-29).

### 2026-10-29 build (Claude, artwork worker)

- Added `tools/translation_mainrom_drafts.py` and `tools/test_translation_mainrom_drafts.py`. Edits are made in texel-index space per frame; each source variant is rendered from the edited indices with its own CLUT. Decode gate: 42 frames x 6 variants = 252 renders byte-identical to `.asset-cache/translation-assets/original-mainrom-v1`.
- Published 141 draft candidates (one per distinct source PNG of each frame) at `translations/en/graphics/artwork/checkpoints/T39004-pass3/` (ledger, `mainrom-indexed-drafts-v1/`, separate `agent-review.agent.json`). All style unreviewed, human_approved false. Contact sheet for human review: `.asset-cache/translation-assets/mainrom-drafts-v1-contact-6x.png` (local only).
- Coverage: 31 items with candidates (57-59, 61, 82-88, 121-133, 136, 137, 140, 151-154); 10 stay missing with reasons (60, 134, 135, 138, 139, 141-145); 118 deliberately untranslated. Proposals for the integrator: `translations/en/graphics/decisions/mainrom.json`.
- Defaults applied (revisable): strict variant rule (icon-51 and out-of-scope hud/icon variants marked not runtime-applicable with CLUT evidence, still produced); 57-59 read セーブ中, "Saving" needs panel7 g, so "Save"; badges 122-125 keep the original numeral texels; no new panel7 glyphs; 118 and 151-154 logos kept, strips replaced; frame 88 all variants.
- Findings: 57-59 part 0 is frame 56's card and part 1 is the bouncing lettering under it, so the draft clears part 1 and draws "Save" over the card with the part-1 bounce offset. Frame 61 English pixels extend where no part exists; an overlay layer with the label-strip CLUT (parts 25-27, asserted equal) carries them. Derived headline ink/shadow indices equal every catalog suggestion. 152-154 strips share texels and produce identical edits.
- Tests: Docker `python -m unittest test_translation_mainrom_drafts test_translation_legacy_remap test_translation_inventory test_translation_assets` -> 42 OK (rebuild byte-identical to the published checkpoint); `python translation_inventory.py check` passes (inputs unchanged; the inventory does not read pass3 until the integrator adds the ledger and an id pattern).
- Open: CODING_STANDARDS review not yet done; port binding of multi-part/STP frames and a magazine presenter need a separate port ticket.

### 2026-10-29 review fixes (Claude, artwork worker)

Code review raised four findings; all four were confirmed and fixed.

- High, 57-59 "Save" drawn into the card: confirmed. stamp() wrote into the top covering part, which is part 0, the card shared with frame 56. Each frame wrote at its own bounce offset, so 94 sheet texels got different values and the card border was erased. Fix: "Save" is now written into part 1 only, at one part-local rect (`SAVE_RECT`), so 57-59 write identical texels and the card is untouched. The card stays on top, as in the source (the renderer draws the last part first, see `tools/assets/image_packs.py`). "Save" is 20px of ink and the card interior is 19px, so 3 ink pixels per frame sit under the card border. The source lettering also sits partly under the card (54-74 pixels per frame). These pixels are allowed only under unchanged occluder part 0, and are counted in `hidden_under_card`. Revisable default: the alternative is to withdraw 57-59 to missing. Only the 18 outputs for 57-59 changed.
- Medium, shared sheet texels: confirmed. build() now maps every changed texel to its sheet address and refuses drafts that disagree. Each row gets a `sheet_scope`. These are frame-composite-only: 61 (116 overlay texels with no sheet source), 82 and 88 (texels also drawn by undrafted frame 60), and 136/137/140 (tag texels overlap each other and missing tags 135/138/139/141-143). All other drafts are sheet patches: 57-59 and 152-154 carry identical edits. This is recorded in decisions/mainrom.json for the integrator and the port ticket.
- Medium, untested guards: confirmed (the earlier mutation run also suffered from stale .pyc files when mutated files kept the same size). Added synthetic tests: clipping, hidden pixels and occluders, CLUT mismatch, clear index, overlay CLUT, lift transparency, lift reproduce check, lift colour with no index, the fit check, check() size, gate decoder, render_group, and sheet_scope. A mutation run (each `raise` disabled in turn, `PYTHONDONTWRITEBYTECODE=1`) now kills every reviewer-named guard. The 17 surviving guards are data-identity and provenance checks, which only the original-data rebuild exercises.
- Low, CLUT not enforced: confirmed (no output affected). New `put()` helper: it checks that the target part's CLUT equals the source part's CLUT in every variant, plus the clip and hidden checks. stamp() and plan_badge_digits both use it.
- Tests: Docker `python -m unittest test_translation_mainrom_drafts test_translation_legacy_remap test_translation_inventory test_translation_assets`: 55 OK, rebuild byte-identical to the republished checkpoint. `translation_inventory.py check` passes. Checkpoint T39004-pass3 republished; agent review and decisions JSON regenerated.

### 2026-10-30 port-made lettering (Claude, art-finish worker)

User decision 2026-10-30: finish asset translation "or creation"; where no usable original glyph exists, new lettering may be drawn in the game's own style, marked port-made. Outputs stay drafts (human_approved false).

- `tools/translation_mainrom_drafts.py --port-made` adds hand-drawn bitmap glyphs with per-glyph provenance (`origin` recovered-panel7, port-made or port-made-narrow, a description and the bitmap): 13 panel7-style glyphs (B I M N P R W b d g m w y), 10 narrow forms (H a b e g n o r s t) for tags too short for panel7 widths, and 7 small caps (A D E M O R Y, 5 rows) for frame 60. Nothing is scaled or generated; recovered panel7 glyphs cannot be replaced silently (narrow forms say what they replace).
- Tags (single ink, no shadow in 8-row tags; ink and (+1,+1) shadow derived from the source in the 16-row tags): 134 Comedy, 135 Romance, 138 Horror, 139 Anime, 141 Idol, 142 Rock, 143 New Music, 144 Pro / Wrestling (two lines), 145 Baseball (one line). Narrow forms per glyph: see the review fixes below.
- Frame 60: the game draws year, month and day digits above the 年/月/日 cells and the weekday frame inside the parentheses (`port/godot/ui/native_hud.gd` build_calendar). The cells are cleared and small-cap captions YEAR / MO / DAY in the kanji ink and shadow are set on an overlay layer with the kanji-cell CLUT (parts 3-5, asserted equal), centred on the digit column. First draft read "95 YEAR / 4 MO / 4 DAY / (Mon)"; replaced after review (below).
- Published 53 drafts (one per distinct source PNG of the ten frames; see review fixes for the alternates and month sprites added later) at `translations/en/graphics/artwork/checkpoints/T39004-mainrom-port-made-v1/` (ledger with `port_made` and `port_made_elements` per asset). All are frame-composite-only (`isolated_scope`): tag cells are shared between tags and frame 60 uses an overlay. Proposal for the integrator: `translations/en/graphics/decisions/mainrom-port-made.json` (LEDGERS entry plus ten atom overrides; shared inventory files not edited here).
- Weekday revisit: frames 82-88 have English for every source variant group (reviewed batch-002 or pass3 draft); none missing, nothing redrawn. Frames 57-59 keep "Save": "Saving" does not fit the 19px card interior even with narrow glyphs.
- Review: every one of the 53 x4 original/draft previews was inspected (`.asset-cache/art-finish-2026-10-30/mainrom-previews/`, local), plus a mock of frame 60 with game digits and the English Monday. One iteration: narrow g got a square bowl because "Wrestling" read as "Wrestlina"; word space 3.
- Tests: Docker `python -m unittest test_translation_mainrom_drafts test_translation_inventory test_translation_assets test_translation_legacy_remap` -> 65 OK (new: glyph tables, face provenance flags, no silent replacement, ink/shadow derivation, isolated scope, deterministic port-made rebuild equal to the published checkpoint with pixels outside edit regions identical, palette subset and ledger port-made flags). `translation_inventory.py check` passes (inventory still reports these ten missing until the integrator applies the proposal). CODING_STANDARDS review of this addition not yet done.

### 2026-10-30 review fixes (Claude, art-finish worker)

Review of the port-made patch raised three findings; all three were confirmed and fixed.

- Medium, Horror used the 2px narrow r: confirmed (all three r were narrow, lettering x1..21). The item-wide `panel7-narrow` face is gone. `fit_face` starts from panel7-compact and swaps in a narrow form one letter at a time, only for letters of a line that does not fit, in `NARROW_ORDER` (bowls first: H o a e b g n s, then r t, whose narrow forms lose a stroke). A letter narrowed for one line is narrow in every line of that item. Results: 138 Horror uses narrow H and o with the recovered 3px panel7 r, ink x0..23 (24px exactly). 144 uses narrow e g n s r t, so its output is unchanged. 145 Baseball now narrows only a (B, s, e, b, l at panel7 width). Nov narrows o. The other tags are unchanged. The manifest `face` names the swaps (e.g. `panel7-compact+narrow-Ho`).
- Low, frame-60 date form was not the "Apr 4" default: confirmed. The default draft (`mainrom-draft-0060.*`) now clears the three kanji cells. Twelve new port-made month sprites (`mainrom-port-month-apr.png` .. `-mar.png`, 16x16) are drawn in panel7 mixed case in the calendar digit CLUT (frames 0x44..0x4D, checked equal to the kanji-cell CLUT), ink 9 and (+1,+1) shadow 10. New caps D F J O were added for them. The port draws the sprite in place of month digit actors 19/20 at frame-60 rect (9, 40, 16, 16), i.e. screen (0x89, -0x2A), so the panel reads "95 / Apr / 4 / (Mon)". Without the overlay the panel reads "95 / 4 / 4 / (Mon)". Revisable: this form needs a port change in `native_hud.gd` build_calendar that is not made here.
- Low, YEAR shadow touched the border: confirmed. The caption form is kept as an alternate (`mainrom-alt-0060-captions.*`, ledger `alternates`, not an inventory candidate) and now reads "YR / MO / DAY". YR ink is x13..19 with its shadow at x20, 4px clear of the border.
- Checkpoint `T39004-mainrom-port-made-v1` republished: 53 candidate assets, 6 alternates and 12 `port_overlay_assets`, all with human_approved false and port-made descriptions. `decisions/mainrom-port-made.json` regenerated (hashes, ledger sha, frame-60 form).
- Previews: all 65 per-output x4 previews were inspected, plus `mainrom-port-months.png` (game digits beside each month name) and `review-0060-date-forms-mock.png` (game digits drawn on the panel), all in `.asset-cache/art-finish-2026-10-30/mainrom-previews/`.
- Tests: Docker `python -m unittest test_translation_mainrom_drafts test_translation_inventory test_translation_assets test_translation_legacy_remap`: 66 OK. New tests cover fit_face narrowing (Horror keeps the recovered r) and check the month sprites and alternates. `translation_inventory.py check` passes.

## Comments
