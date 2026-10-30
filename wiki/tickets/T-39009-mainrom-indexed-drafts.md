---
id: T-39009
title: Produce MAINROM indexed English drafts
status: In Progress
assignee: Claude (artwork worker)
created: 2026-10-29
updated: 2026-10-29
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

## Comments
