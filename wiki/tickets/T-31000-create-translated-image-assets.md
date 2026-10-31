---
id: T-31000
title: Create same-size translated image assets and comparisons
status: In Progress
assignee: root
created: 2026-10-26
updated: 2026-10-26
links: ["[[assets-media]]", "[[assets-text]]", "`phase4`"]
---

## Goal

User explicitly authorizes starting translated image production alongside baseline port repairs. Use the existing English image-text catalog and authentic extracted originals. Preserve exact pixel dimensions and the original visual theme. Use image editing and pixel-accurate compositing where generation alone cannot meet the requirement; AI reconstruction is an available tool, not a requirement for every asset.

## Acceptance criteria

- [ ] Map each draft to original source/frame/palette/hash and approved catalog wording; flag ambiguous text instead of inventing it.
- [ ] Keep original files immutable and all original/derived game art under ignored `.asset-cache/`.
- [ ] Preserve exact width/height, anchors, transparency and relevant mask/palette metadata; document intentional text-region edits and unresolved native rebuild constraints.
- [ ] Produce original/translated side-by-side previews, with native-size and nearest-neighbor enlarged inspection views in a separate translation gallery.
- [ ] Validate text accuracy, legibility at native resolution and unchanged non-text regions where applicable. Wrong-size or visually drifting generation remains a rejected draft.
- [ ] Review before runtime integration. Translation drafts must not be published as actual gameplay evidence.

## First batch

An isolated worker will select a small, representative batch from the current `translations/en/graphics/image_text_catalog` and authenticated exports, then create actual edits plus reproducible manifests/comparisons. Read `/Users/lila/.codex/skills/.system/imagegen/SKILL.md` for built-in generation/editing when used. The user separately permits image-editor workflows when generation is unsuitable; use existing Docker tools rather than host installs. Root owns canonical tooling/docs and publication.

## User typography rejection and verification changes

User rejected the first two draft comparisons as off-center, too small and unnecessarily abbreviated. V2 therefore remains a rejected typography draft, despite passing size/hash/non-text checks. Author is preparing V3 with explicit source text-cell/row centering, larger legible proportional lettering and full wording wherever feasible within unchanged geometry. No silent abbreviation or unreadable shrink-to-fit is permitted. A separate verifier is implementing centering, approved spelling, minimum-size, residue, alpha and unchanged-region checks with negative fixtures; native-size visual review remains mandatory. If full wording cannot fit honestly, report the exact constraint rather than declaring the asset ready.

## Astra image revision requested (2026-10-26)

User explicitly requested Astra and permits conventional image editing software. Root held partial V3 for illegible condensed labels and a small heading; geometric checks do not establish readability. Astra now owns a preserved-version V4 revision, with a separate verifier. Health and Sports are acceptable catalog-supported full words. Explore safe blank panel interior for wider labels without changing original canvas dimensions, numeric/arrow positions or decorative columns. Present native-size and enlarged original/new comparisons only after visual and technical review.

## User wording approval (2026-10-26)

User explicitly approved `Humanities` for the academic 文系 stat and `Arts` for 芸術, replacing Liberal Arts/Art in the next draft. Standard weekday abbreviation `Mon` is also approved for the 16-pixel asset. Full words are preferred when they fit; this is not a blanket ban on abbreviations. Astra is preparing a preserved successor with independent native-size readability and protected-pixel checks. V5 is superseded as the final wording candidate; originals remain immutable.

## Final wording choice for next preview (2026-10-26)

User selected **Culture** and **Arts**, superseding Humanities/Hum. comparison options; Mon remains approved. Culture was verified visually in the SNES translation team's official screenshot at https://www.translated.games/snes/heartthrob (reference wording only). Astra is preparing the next original/new comparison with conventional readable glyphs; retain original separators if the full label fits. Earlier options remain evidence, not selected assets.

### Live reviewer page (2026-10-26)

http://127.0.0.1:8766/ is running in persistent localhost-only Docker `tokimemo-translation-review-8766`, separately from gameplay8765. Browser verified 15 drafts with original/translated images, zoom, notes and decisions. Exact independent acceptance is in `.claude/orchestrator/state/T-31000/gallery-v1/standards-review/final-acceptance.md`; narrow-view summary CSS follow-up remains. Persistent local decisions live in ignored `.asset-cache/translation-images/review-gallery-state/`; no original images are changed by review. This page currently serves the reviewed isolated candidate; canonical tooling application is pending.

### Context-specific date glyphs (2026-10-26)

User explicitly requires separate weekday and date-unit English assets. `src/main/hud_panels.c:719` selects weekday frames82–88; line733 selects date chrome frame60. Source frame60's month suffix shares original texels with Monday frame82; its day suffix shares texels with Sunday frame88. A global atlas-texture replacement would therefore corrupt month/day labels. Keep frame/context overrides or duplicate the shared glyph region and separately localize date chrome. Current English drafts are standalone images and have not changed runtime atlas data. Acceptance must exercise Monday plus numeric month, and Sunday plus numeric day, together. Image author is preparing exact source metadata evidence and continuing the next bounded menu batch.

Gallery tooling is now applied canonically. Installed Docker suite14/14 passes. Reviewed UIv3 defaults to translated drafts and fixes narrow-screen summary overlap. User subsequently requested a Needs-review queue that removes approved/commented items without losing scroll position; finite UIv4 is in preparation. Persistent reviews survive server restart.

Exact shared-glyph evidence is now retained at `.asset-cache/translation-images/inventory/date-context-proof.json`, bound to catalog SHA `d155a2958e7e8694a9b5444412ef71843a0229b15503e713e911a9c1b5f8b4d6`: month frame60 part4 and Monday frame82 share sheet offset32768, texture14, UV112,0; day frame60 part5 and Sunday frame88 share texture42, UV80,16. Localization must preserve the distinct meanings despite this byte reuse. The browser confirms all15 current drafts have actual saved style approvals; these records must not be reset by the pending Needs-review/Reviewed interface update.

### Needs-review queue installed (2026-10-26)

Reviewed queuev4 is now canonical and live at http://127.0.0.1:8766/. Default Needs review contains only translated drafts with no current review (including stale revisions); Reviewed contains approvals, change requests and saved comments. Saving removes only the affected card, preserving other DOM textareas and scroll anchor. Independent review resolved a delayed-save/newer-unsaved-note race;15 backend tests and12 source predicate checks pass. Root disposable browser QA approved one draft while preserving a different unsaved note, saved that note, then verified both rows in Reviewed. Production browser confirms15 existing approvals remain and default queue is empty until new batches arrive. Acceptance `.claude/orchestrator/state/T-31000/gallery-review-queue-v4/standards-review/report.md`; packet4661a51a99118d26bfaab0042880614fa6ca5250edf18db080ef71c046ca5bbf. Batch003 has18 new menu drafts awaiting independent image acceptance and index registration.

### Menu batch003 available for user review (2026-10-26)

Root registered frozen manifest `8ecde63ef266dc3c1f93cc38575e3a43bc373858fa5731d238e7f75801005b80` after independent technical18/18 PASS (`verification-v1/review-batch003.md`, reportSHA996076294efc56f667c43b933878437383923f91f91d687125ac410441ac8311). All nine word pairs preserve original gold/gray appearance and exact widths×16;9px English glyphs are smaller than original11px and await user style review. Browser8766 confirms18 Needs-review drafts,33total,15previousapprovals retained. No runtime replacement or selected-state interpretation is accepted. Batch004 is separately being reviewed; root alone appends live batch index.

Batch004 (20 additional labels) now registered after independent technical PASS, manifest31c4cb512a1df4daf1544a052f73499540304d9a17af020e79b1550c699ab184; review `verification-v1/review-batch004.md`. These remain user-unreviewed drafts. User questioned option-label-0027: source `settings_sound_test_and_extras.c:1005` selects frame0x1B for first input-settings row; selecting it enters the confirm-button selector (1082–1108), which assigns left/right mouse confirmation. “Confirm Button” is a setting name, not an action labeled “Confirm.” Keep this context separate from user-authored review notes; immutable manifest context enrichment is pending.

### User-found alignment correction (2026-10-26)

User flags option-label-0042 and0043. Root independent Docker pixel measurements confirm original non-shadow ink starts(0,0), while translated Default (frames41/42) starts(13,1) and Edit(frames43/44) starts(6,1). Source ink heights11 versus translated9; original widths53/25 versus translated30/16. The prior technical check enforced centered ink in the text band and therefore did not preserve the original left alignment. Treat prior technical acceptance as insufficient for this visual requirement. Astra is preparing immutable corrected paired variants and checking the cohort; no existing user approvals/notes or frozen artifacts are overwritten. Future checks must derive alignment anchor from source/context rather than universally center translated text.

User expanded alignment audit to all images, including already approved, and specifically Confirm Button. Root confirms frames27/28 source ink(0,0,64,10), English(3,1,66,9) in72×16. Complete menu pixel-origin audit is `.claude/orchestrator/state/T-31000/alignment-audit/menu-ink-bounds.json`: all38 menu outputs have different ink origins, horizontal delta0–33; source origins vary(0,0),(1,0),(1,1),(2,0), so glyph bearings and source layout must be distinguished rather than blindly forcing every inkbox tozero. Astra audits all53 drafts including earlier panel/weekdays/heading; independent reviewer audits alignment policy and changed-image approval invalidation. No historical user decisions are erased; corrected image revisions require renewed review. Further new-batch approvals are paused until contextual alignment is checked.

Context audit distinguishes intentional approved changes from errors: V7 stat-panel expanded label cells are user-approved art direction, not native-exact glyph centers. Magazine heading preserves the source left anchor and optical vertical center with different font height. OPTION menu rows require source left anchor and baseline, not generic text-band centering; first six corrections target Confirm Button/Default/Edit paired appearances while retaining the accepted glyph shapes. Weekday placement needs actual date-bar context because shared glyph/frame origins differ from the full16px standalone canvas; no blanket all-left correction is authorized. See independent reviewer's forthcoming53-image context report.

First six corrections are now registered through batch003 manifest-r2 SHA0e232d2f289543b364ad0d5410543f2d387dc4eb04cc2494c73beae05a686f29, superseding8ecde63e. Independent `.claude/orchestrator/state/T-31000/alignment-audit-v1/first-six-review.json` verifies frames27/28/41/42/43/44 keep exact9px glyph/colors but align left0/baseline10; old images fail new anchor checks, other12 outputs unchanged. Previous approved versions/history are preserved and changed revisions require review. User clarified spot checks indicate possible major issues, not a mandate to eliminate every pixel difference; intentional panel/heading designs stay separate.

Batch004r2 registered (SHA2a63c84cc712b81fad428ed1c8fd1c1b84857b28b921e3ccee7473696196936e) after independent `alignment-audit-v1/next-four-review.md` PASS. Frames29–32 preserve source1px bearing and baseline10, exact existing glyph/color pixels, and16unchangedoutputs. Source options input-row actor/menu geometry confirms shared left-column intent. Live8766 API confirms allfour are reviewable in Needs review. Prior versions/review history remain intact. Firstsix003r2 were already registered and subsequently approved by the user; do not reset those fresh decisions again.

### Review-before-user quality gate tightened (2026-10-26)

User reports labels37–56 also misaligned and explicitly expects these issues caught before user review. Root holds further menu-batch publication; Astra and independent reviewer must assess all remaining published OPTION labels in source-backed full-menu layout, not isolated glyph bounding boxes. Include fixed actor/frame origins, row/column anchors, baseline and neighboring content; separate missing/unpublished IDs45–48 from actual inventory. Previously approved corrected41–44 remain unless context identifies a real defect; do not erase history or claim original approvals cover changes. The previous pixel-template/center gate was insufficient and must not authorize another comparable batch. Weekday context also reveals potential overlap with frame60 parentheses, requiring actual date-bar composition rather than a standalone image fit test.

Cohesive r3 publication: independent full-menu check reconstructs all38 OPTION labels in six original320×240 menu layouts from original frame parts and C actor positions;28changed/10priorr2images unchanged. Protected border/frame121 punctuation/ON-OFF/Album neighbors have zero new-ink collision. Root inspected input/extras compositions; independent reviewer inspected allsix native comparisons. Proof `alignment-audit-v1/menu-cohort-independent.json`, reproducer `verify_menu_cohort.py`. Batch003r3 e1b0f7cb7717579b5a1063faa92b771eec2095e4764847a7124fbea9549f92fd and004r3 d5b863714de3ecb412a707e879be0051ec350b48c254256ee3fc2b664a8c7290 now supersede r2 through append-only index; changed versions need review, unchanged approvals persist. These are source-layout mocks, not runtime/GPU acceptance. Weekday/date-bar context remains separate open work.
