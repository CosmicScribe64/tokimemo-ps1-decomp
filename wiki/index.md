---
type: index
updated: 2026-10-26
---

# Wiki index

Read this first. One line per page, grouped by what you need. Tickets are not listed here; see [[tickets/catalog]] (generated) and the [[kanban]] board. The old full index is kept as [[archive/index-2026-10-20]].

## Start here

- [[overview]] - project goal, stack, pointers to fact pages
- [[status]] - progress, what is left by unit, active work
- [[plan-remaining]] - current completion plan (101 remaining functions on 2026-10-22; older census in [[data/remaining-2026-10-20]])
- [[playbook]] - the matching loop and the recipes that closed matches; read before matching
- [[phase2]] - phase 2 (readable code) work order, units, and review rule; conventions in [[phase2-naming]], terms in `GLOSSARY.md`
- [[phase2-naming]] - phase 2 naming decisions, scope table, disc-name map and prefix maps
- [[phase3]] - Godot 4.7.2 Japanese desktop port plan; readiness repairs and verification remain in progress
- [[phase4]] - Phase 4 modernization plan (widescreen presentation, modern settings & translation-pack selector, native achievements, player customization)
- [[phase4-design-2026-10-29]] - judged phase 4 design: widescreen, settings, English play, playtesting work packages
- [[switch-remake-reference]] - how the 2025 Switch release handles widescreen, fonts and modern features, compared with our phase 4 design
- [[switch-remake-footage]] - frame log from Switch release trailers and gameplay footage
- [[notes/functions]] - every function named in the matching notes, with links to its sections
- Repo root: `AGENTS.md` (rules), `CODING_STANDARDS.md` (conventions, review checklist), `README.md`, `ROADMAP.md`, `CONTRIBUTING.md`

## Port references

- [[port-pause-2026-10-29]] - latest user-requested WIP pause, translated-artwork publication and exact continuation instructions
- [[port-completion-plan]] - 2026-10-30 completion plan: memory model, engine API, generic renderer and disc pack first, then file-by-file porting
- [[port-finish-plan]] - earlier finish sequence, failure lessons and one-manager/one-implementer workflow
- [[port-manager-handoff]] - next manager's first assignment, retained candidates and local-page operations
- [[port-readiness-review]] - independent baseline blockers and actual-route acceptance requirements
- [[port-adversarial-review]] - fresh-context review of player routes, installation, packaging and contributor experience
- [[port-testing]] - testing layers, required route coverage and reviewed evidence gate
- [[port-text-layer]] - generic source-keyed runtime text translation layer, pack builder and screen adoption
- [[notes/name-entry-visual-checkpoints]] - original NAME entry state addresses and corresponding native/translated visual checkpoints
- [[archive/port-status-claims-2026-10-26]] - superseded port completion claims and historical checkpoints; not acceptance evidence

- [[port-checkpoint]] - reviewed low-usage publication checkpoint and exact remaining integration work

- [[widescreen-settings]] - reviewed original-art scaling and modern settings research; later implementation remains separate

- [[spec/index]] - source-backed unit port specs, applied ledgers, evidence and explicit unknowns
- [[spec/o-taiiku]] - Sports festival transitions, input/movement and girl-stat rewards
- [[spec/o-geko]] - GEKO mode entries, walk-home/birthday rules, invitations and appointments
- [[spec/o-shougatu]] - New Year/Christmas and annual-event rules, shrine choices and documented unknowns
- [[spec/o-tel-option]] - Telephone/options invitation eligibility, relationship effects and sound-test flow
- [[spec/o-master-ending]] - Mastery, companion selection, ending candidates and audio-level rules
- [[spec/o-bunkasd-shugaku]] - Stage-show dispatch, school-trip responses and evidence limits
- [[spec/o-small]] - Seven small-overlay loader entries, outcome rules and evidence limits
- [[spec/reconcile]] - Applied-ledger precedence, scheduler ownership decisions and executable-batch preparation
- [[spec/file-naming-main]] - Exhaustive main C filename mapping and integration proof
- [[spec/file-naming-overlays]] - Exhaustive overlay C filename mapping and review gates
- [[spec/header-split-plan]] - Declaration ownership and staged topic-header extraction
- [[spec/phase2-completion-audit]] - Final source acceptance, matching proof and scoped residue
- [[spec/core-followup]] - Applied core/daily-rule residue reconciliation and exact constant-site evidence

## Asset tooling

- [[asset-export]] - engine-independent original-input readers, full text export and modern media/localization inventories; no direct Godot work
- [[assets-input]] - Original-game input inventory and provenance
- [[assets-text]] - Text/script extraction coverage and fidelity gates
- [[assets-media]] - Image/audio readers, authentic palette galleries and visual-text catalog

## Matching

- [[playbook]] - short recipe list by symptom
- [[register-cheatsheet]] - full register-miss guide, symptom to C change (regdiff loop)
- [[notes/wins-2026-10]] - the 93 wave 8-11 matches (2026-10-22/23) by recipe, with the deciding change per function and the levers ranked by how often they closed a match
- [[decompile-workflow]] - matching sequence (queue, m2c, edit, build, funcdiff, commit), switches, and jump tables
- [[matching-notes]] - active notes page (append new sections here); older parts in `notes/`, index in [[notes/sections]]
- [[build-step-gaps]] - what the build cannot reproduce yet (uopt, as1, ugen block-copy state)
- [[as1-scheduler-rule]] - as1's global delay-slot filler (candidate order, weights, gates, the `volatile` lever, T-24900)
- [[as1-scheduling]] - per-file as1/driver options tested on the as1 order near misses (T-24800); tools/as1_flag_sweep.py
- [[keep-lines]] - proposed `keep` rule with a byte-match necessity proof
- [[cloud-matching]] - matching one C file in a cloud container from a match packet

## Types and data

- [[data-types]] - structs and arrays recovered from access patterns (`tools/type_recovery.py`)
- [[game-state]] - `GameState D_800E6280` field map, evidence, and `tools/migrate_globals.py`
- `families/` - per-overlay type maps and results in [[families/bunka-family-results]], [[families/bunka-sd-family-results]], [[families/bunkasai-family-results]], [[families/date-family-results]], [[families/en-nichi-family-results]], [[families/ending-family-results]], [[families/etc-family-results]], [[families/event-family-results]], [[families/geko-family-results]], [[families/kangei-family-results]], and [[families/main-family-results]]. Additional overlay results live in [[families/master-family-results]], [[families/master-types-pilot]], [[families/name-ent-family-results]], [[families/option-family-results]], [[families/rpg-bat-family-results]], [[families/scenes-family-results]], [[families/shougatu-family-results]], [[families/shugaku-family-results]], [[families/taco-family-results]], and [[families/taiiku-family-results]]
- [[ref-english-patch]] - facts-only address reference and independently checked dialogue-offset inventory
- Data tables: [[data/true-gaps]], [[data/t0018-cases]], [[data/shared-at-groups]], [[data/t13600-remaining-census]]

## Build and toolchain

- [[toolchain]] - Docker image, pinned versions, IDO 5.3 for game code, frame-layout pass
- [[build-system]] - configure.py / ninja pipeline, jump tables, rodata islands
- [[tooling]] - every tool under `tools/` with its ticket
- [[rename-tool]] - `tools/rename.py` phase 2 renames from a table, proofs, ledger, check, and replay
- [[emulator]] - PCSX-Redux + OpenBIOS in Docker (breakpoints, memory watches, call stacks, traces, input, screenshots, save states, T-20500)
- [[ci]] - GitHub Actions, encrypted game bundle, decomp.dev report
- [[gcc-objects]] - PsyQ-gcc objects inside overlays, per-object toolchain table
- [[psyq-sdk]] - SDK library versions and layout
- [[source-files]] - the `src/` files and original object boundaries

## The game binaries

- [[disc-layout]] - disc images, extraction, file list
- [[executable]] - SLPM_86.053 header, memory map, segments
- [[overlays]] - the 26 .EXN overlays (loader, load addresses, split)
- [[obin]] - O.BIN ECOFF symbol table and its mapping onto the exe
- [[versions]] - the five releases and their differences

## Compiler research

- [[original-compiler]] - which compiler built the game (IDO 5.2-generation, O.BIN stamps)
- [[ido-52-evaluation]] - IDO 5.2 and 4.1 tested against the game code
- [[compiler-mismatch-research]] - PS1 compilers, frame candidates, how other decomps handle mismatches
- [[uopt-register-allocation]] - allocator trace, formula, patch experiments
- [[register-allocation-wave7]] - register clusters and verified idioms
- [[ending-allocator-trace]] - reusable allocator tracer
- [[speedup-research-2026-10-16]] - would compiler reverse engineering help, and ways to speed up matching

## Past campaigns

- [[matching-breakthroughs]], [[broad-coverage-results]], [[acceleration-audit]], [[exhaustive-drafts]] - census and experiment write-ups
- `archive/` - superseded status and handoff pages in [[completion-run-2026-10-17]], [[stopping-point-2026-10-15]], [[flagged-draft-pass]], and [[archive/index-2026-10-20]]

## Wiki upkeep

- [[log]] - append-only log of the last week; older weeks in `log/` (`tools/wiki.py rotate-log`)
- [[SCHEMA]] - wiki layout and rules
- `tools/wiki.py` - `rotate-log`, `rotate-notes`, `catalog` (ticket catalog, notes sections, function index), `lint` (broken links, kanban mismatches, large pages)
- [[tickets/T-30001-modern-widescreen|T-30001]] Phase 4 (`P4.1`) widescreen presentation (`16:9` / `16:10` / `21:9` safe-frame apertures and responsive HUD/dialogue reflow) (Ready).
- [[tickets/T-30002-modern-settings|T-30002]] Phase 4 (`P4.2`) comprehensive modern settings, input remapping, 5-bus audio, and translation/voice pack selector (In Progress; bounded UI/playback checks pass, full-game/platform acceptance remains open).
- [[tickets/T-30004-modern-achievements|T-30004]] Phase 4 (`P4.3`) deterministic native achievement system and spoiler-aware gallery (Ready); see [[phase4]] and [[status]].
