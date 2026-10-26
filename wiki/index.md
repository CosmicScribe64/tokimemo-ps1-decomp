---
type: index
updated: 2026-10-25
---

# Wiki index

Read this first. One line per page, grouped by what you need. Tickets are not listed here: see [[tickets/catalog]] (generated) and the [[kanban]] board. The old full index is kept as [[archive/index-2026-10-20]].

## Start here

- [[overview]] - project goal, stack, pointers to fact pages
- [[status]] - progress, what is left by unit, active work
- [[plan-remaining]] - current completion plan: 101 remaining functions on 2026-10-22; older census in [[data/remaining-2026-10-20]]
- [[playbook]] - the matching loop and the recipes that closed matches; read before matching
- [[phase2]] - phase 2 (readable code): work order, units, review rule; conventions in [[phase2-naming]], terms in `GLOSSARY.md`
- [[phase2-naming]] - phase 2 naming decisions, scope table, disc-name map and prefix maps
- [[phase3]] - reviewed Astra Godot port plan, native behavior/content boundaries, playable milestones and first implementation tickets
- [[notes/functions]] - every function named in the matching notes, with links to its sections
- Repo root: `AGENTS.md` (rules), `CODING_STANDARDS.md` (conventions, review checklist), `README.md`, `ROADMAP.md`, `CONTRIBUTING.md`

## Port references

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

- [[asset-export]] - engine-independent original-input readers, comprehensive text export and modern media/localization inventories; no direct Godot work
- [[assets-input]] - Original-game input inventory and provenance
- [[assets-text]] - Text/script extraction coverage and fidelity gates
- [[assets-media]] - Image/audio readers, authentic palette galleries and visual-text catalog

## Matching

- [[playbook]] - short recipe list by symptom
- [[register-cheatsheet]] - full register-miss guide, symptom to C change (regdiff loop)
- [[notes/wins-2026-10]] - the 93 wave 8-11 matches (2026-10-22/23) by recipe, with the deciding change per function and the levers ranked by how often they closed a match
- [[decompile-workflow]] - queue -> m2c -> edit -> build -> funcdiff -> commit; switches, jump tables
- [[matching-notes]] - active notes page (append new sections here); older parts in `notes/`, index in [[notes/sections]]
- [[build-step-gaps]] - what the build cannot reproduce yet (uopt, as1, ugen block-copy state)
- [[as1-scheduler-rule]] - as1's global delay-slot filler: candidate order, weights, gates, the `volatile` lever (T-24900)
- [[as1-scheduling]] - per-file as1/driver options tested on the as1 order near misses (T-24800); tools/as1_flag_sweep.py
- [[keep-lines]] - proposed `keep` rule with a byte-match necessity proof
- [[cloud-matching]] - matching one C file in a cloud container from a match packet

## Types and data

- [[data-types]] - structs and arrays recovered from access patterns (`tools/type_recovery.py`)
- [[game-state]] - `GameState D_800E6280`: field map, evidence, `tools/migrate_globals.py`
- `families/` - per-overlay type maps and results: [[families/bunka-family-results]], [[families/bunka-sd-family-results]], [[families/bunkasai-family-results]], [[families/date-family-results]], [[families/en-nichi-family-results]], [[families/ending-family-results]], [[families/etc-family-results]], [[families/event-family-results]], [[families/geko-family-results]], [[families/kangei-family-results]], [[families/main-family-results]], [[families/master-family-results]], [[families/master-types-pilot]], [[families/name-ent-family-results]], [[families/option-family-results]], [[families/rpg-bat-family-results]], [[families/scenes-family-results]], [[families/shougatu-family-results]], [[families/shugaku-family-results]], [[families/taco-family-results]], [[families/taiiku-family-results]]
- [[ref-english-patch]] - facts-only address reference and independently checked dialogue-offset inventory
- Data tables: [[data/true-gaps]], [[data/t0018-cases]], [[data/shared-at-groups]], [[data/t13600-remaining-census]]

## Build and toolchain

- [[toolchain]] - Docker image, pinned versions, IDO 5.3 for game code, frame-layout pass
- [[build-system]] - configure.py / ninja pipeline, jump tables, rodata islands
- [[tooling]] - every tool under `tools/` with its ticket
- [[rename-tool]] - tools/rename.py: phase 2 renames from a table, proofs, ledger, check and replay
- [[emulator]] - PCSX-Redux + OpenBIOS in Docker: breakpoints, memory watches, call stacks, traces, input, screenshots, save states (T-20500)
- [[ci]] - GitHub Actions, encrypted game bundle, decomp.dev report
- [[gcc-objects]] - PsyQ-gcc objects inside overlays, per-object toolchain table
- [[psyq-sdk]] - SDK library versions and layout
- [[source-files]] - the `src/` files and original object boundaries

## The game binaries

- [[disc-layout]] - disc images, extraction, file list
- [[executable]] - SLPM_86.053 header, memory map, segments
- [[overlays]] - the 26 .EXN overlays: loader, load addresses, split
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
- `archive/` - superseded status and handoff pages: [[completion-run-2026-10-17]], [[stopping-point-2026-10-15]], [[flagged-draft-pass]], [[archive/index-2026-10-20]]

## Wiki upkeep

- [[log]] - append-only log of the last week; older weeks in `log/` (`tools/wiki.py rotate-log`)
- [[SCHEMA]] - wiki layout and rules
- `tools/wiki.py` - `rotate-log`, `rotate-notes`, `catalog` (ticket catalog, notes sections, function index), `lint` (broken links, kanban mismatches, large pages)
- [[tickets/T-25011-class-a-closers-as1-levers|T-25011]] Class A closers: eight guarded near misses (10 rows or fewer) with the as1 scheduler levers, one Sonnet 5.5 subagent (In Progress).
- [[tickets/T-25012-class-c-d-reconstruction|T-25012]] Class C/D reconstruction (missing drafts, class C, class D smallest first), one Sonnet 5.5 subagent (In Progress).
- [[tickets/T-25013-compiler-level-levers|T-25013]] NEXT STEP for the project: compiler-level levers (uopt/ugen/as1 rules, one uniform build rule at a time) to unblock the last ~119 functions; see [[status]] (Ready).
