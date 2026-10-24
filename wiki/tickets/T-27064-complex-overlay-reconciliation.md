---
id: T-27064
title: Reconcile EVENT TT RPG and TACO naming
status: In Progress
assignee: parallel-chat
created: 2026-10-24
updated: 2026-10-24
links: ["[[phase2]]", "[[tickets/T-27000-phase-2]]"]
---

## Goal

Prepare body-backed, independently reviewed executable symbol packets and complete port specs for o-event, o-tt, o-rpgbat, o-taco. Root alone applies sources and owns shared tracking/builds/commits. Existing draft rows are leads, not trusted evidence.

## Acceptance criteria

- [ ] Resolve current ledger precedence, source ownership and collisions; inventory every in-scope definition or justified hardware exception.
- [ ] Supported symbol-only tables plan cleanly in Docker and pass independent Standards/Spec review.
- [ ] Complete essential local/parameter/constants site inventory, source-backed transitions and persistent effects in specs.
- [ ] Root application, matching proof and final review before Done.

## Comments

2026-10-24: User authorized parallel chats, Luna for bounded work and Sol6.1 for harder analysis/review. No direct Godot work. No source apply/build/commit in this lane.

2026-10-24 reconciliation checkpoint: bounded Luna passes cover EVENT, RPG_BAT and TACO; the lane owner covers TT. Existing drafts contain unsupported `new:` type rows and offset pseudo-fields that cannot be passed to `rename.py`; these belong in explicit unresolved inventories. TT's English game/folder terminology remains unsettled and its current folder is retained; a60c31bb9 subsequently adopts its technical disc namespace without claiming contributor agreement. EVENT and main BSS at the same address have different ownership keys; numeric overlap alone does not make EVENT's own tables main-owned.

2026-10-24 validation resource checkpoint: concurrent Docker plans across chats are consuming substantial resources. The RPG helper reports exit 137 from its plan and no completed validation output. This lane is stopping its own obsolete duplicate plans and will freeze tables before a single final plan for each unit. No source application or matching proof is claimed. Root owns the shared log/board updates.

2026-10-24 independent review checkpoint: separate Sol 6.1 Standards and Spec agents reviewed pinned hashes. Both required changes. Concrete findings: TACO's broad particle/actor and dual-bank sound constant selectors conflate meanings; EVENT's skating-rink table has 31 entries, not 22; RPG's `var_a0` at 0x801477E0 bounds extra damage to the player, not enemy HP; TT repeated a CLUT field row. Additional convention, hypothesis, main-ledger precedence and residual-inventory findings are being corrected. Completion and application remain blocked by those gates and by incomplete essential-role analysis. Unsupported constant selectors will be retained as unresolved evidence rather than emitted.

2026-10-24 TT corrected candidate checkpoint: `o-tt-apply.tsv` has 957 rows (302 function, 166 data, 42 type, 141 field, 282 local, 24 parameter). Optional SDK `ratan2` and its lookup table remain unchanged; the technical namespace is adopted by a60c31bb9; English game/folder terminology still requires contributor agreement. SHA-256 `d077656faee1eb4ef5c277d593b1a206ab1d13f0aedf1ea3a655bd725143a623`. Its full Docker plan is this chat's sole heavy validation process; start hash is in `/tmp/t27064/tt-plan.sha256`, eventual results in `/tmp/t27064/tt-plan-final.log` and `/tmp/t27064/tt-plan.exit`. No result has been claimed yet.

2026-10-24 full TT plan result: the 957-row hash above exited 1. It identified seven already-named/no-op field rows and six local aliases colliding with another proposed role in the same body. Those 13 rows are removed from the current apply table; retained spellings and unresolved duplicate aliases are in the essentials inventory. The corrected TT table has 944 rows. The failed plan did not run the final whole-packet gcc proof, so it is not a clean validation result. EVENT's full plan is now the sole heavy Docker process; RPG/TACO and the corrected TT plan are queued. The earlier TACO result cannot be recovered (container/session absent, capture empty). No result is inferred from that absence.

## Frozen review manifest (2026-10-24, pre-validation corrections)

These hashes pin proposal artifacts, not applied source. Separate Sol 6.1 Standards and Spec reviews identified concrete corrections; final correction verification is pending for this manifest. No unit is Done. Root owns application, build proof and shared tracking.

| unit | function | data | type | field | parameter | local | total |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| EVENT | 805 | 178 | 22 | 49 | 0 | 0 | 1054 |
| TT | 302 | 166 | 42 | 134 | 24 | 276 | 944 |
| RPG_BAT | 328 | 168 | 11 | 31 | 58 | 1 | 597 |
| TACO | 406 | 178 | 24 | 65 | 0 | 0 | 673 |

| artifact | SHA-256 |
| --- | --- |
| `config/renames/proposals/o-event-apply.tsv` | `ee609a7f47c92c7622f139b2a3b50aa8059d0316f08bb11c2929630350fa3a1f` |
| `config/renames/proposals/o-event-essential.tsv` | `347754a506433f81325a780c08a988bf196c00b93126e8fe22c45b5d0ace6c0d` |
| `wiki/spec/o-event.md` | `8eed5be1a0fcdd9573b32af6cda40d474e1d9df9ada8701a5028b6f650667814` |
| `wiki/spec/o-event-steps.md` | `b7e3e50fc72a9ad4853d6976b5ce045e3e9c6c2834b6d274dcbf52653297b6e2` |
| `config/renames/proposals/o-tt-apply.tsv` | `3bb3cbf4d95ba65e49cb681e5dd161409e5e6b46123eca79db2e5523b267ca98` |
| `config/renames/proposals/o-tt-essential.tsv` | `30e70cc2a6ab31147d582ae9d73859593c54f8e3396162dfe06ad6c5a43feda9` |
| `wiki/spec/o-tt.md` | `1be4fe016627bddb5c2779bbf9151060c6adde0a85ca9c26cbed62d5735cf691` |
| `config/renames/proposals/o-rpgbat-apply.tsv` | `1262793f4dcc18b3add51873748ffbea68b4ad08b745892a0881931283f7b84c` |
| `config/renames/proposals/o-rpgbat-essential.tsv` | `e64945e985e79193e1e559c55feb1d478f3b6ad3c22da66f99c6a35ef5bd5ea3` |
| `wiki/spec/o-rpgbat.md` | `3ec5d9a66b2168bef6bb5595b1484480436965dfc89eb3b3f365862ebc0a40af` |
| `config/renames/proposals/o-taco-apply.tsv` | `192140984af1430b7511983c88b69dd86878378e65d0933e6fc3acedef4282eb` |
| `config/renames/proposals/o-taco-essential.tsv` | `87c4066b235540fb579203f9d0ae50151c3193525381251eab6c35e042e97f3d` |
| `wiki/spec/o-taco.md` | `33ee00a9d1bce35d6e29b2f79165ecb4186357d598655af1b6681ab9ed214752` |

Essential gaps: EVENT inventories all 38 parameters/449 locals but only six have proposed role changes; 45 actual type aliases and six header declarations remain unresolved, as do seven old constant candidates. TT retains 162 pseudo-member proposals and ten nonexistent aggregate definitions as deferred structural work; current role aliases, ordinary short locals, actual unknown fields and remaining constants are not semantically complete. RPG has five withheld type aliases and an exact unresolved list spanning 67 definitions. TACO withholds 128 unsafe selectors (87 mixed actor/particle, nine boss modes, 24 dual-bank sounds, eight tuning values); 43 remaining constant candidates still require the separate const workflow. Its 406 function checks/395 m2c tokens do not cover all ordinary locals. TT technical `tt_`/`Tt`/`TT_` namespace is adopted by a60c31bb9; only English game/folder terminology awaits contributor agreement. No packet adopts a folder or main-owned type.

2026-10-24 final correction review: independent Sol 6.1 Standards and Spec agents each rechecked all 13 hashes above at start/end and reported no remaining actionable known review findings. Standards additionally checked row schemas, duplicate keys, no-op rows and duplicate local targets; all pass in the frozen candidates. Both reviews explicitly limit their conclusion to the reported corrections and selected body/consumer evidence, not exhaustive per-row binding or semantic completion. Neither ran builds or plans. Clean symbol plans, the root's application and matching proof, the essential gaps listed above remain outstanding; TT English game/folder terminology is a separate pending agreement. Ticket status stays In Progress.

2026-10-24 EVENT full symbol plan: existing Docker image `tokimemo-decomp-arm64`, `--cpus=2 --memory=2g`, `python3 -u tools/rename.py plan config/renames/proposals/o-event-apply.tsv` exited **0**, reporting **1054 rows, 22 files to change**, no `PROBLEM:` entries. Start/current table SHA-256 both equal the frozen manifest's `ee609a7f...`. Evidence: `/tmp/t27064/event-plan-final.log`, `/tmp/t27064/event-plan.sha256`, `/tmp/t27064/event-plan.exit`. No source was applied. RPG_BAT is now the sole heavy plan; TACO and corrected TT remain queued.

2026-10-24 RPG full symbol plan: frozen table `1262793f...` planned **597 rows, 65 files to change** but exited **1** with one proof-2 diagnostic: old path `src/ovl/RPG_BAT/801525B0.c:502`, `GameState` has no member `unk_3A`. The root's 403-file move occurred during the run, so the proof compares a cached old-path source with a fresh live-tree baseline. The unchanged current body is at `src/ovl/RPG_BAT/labels_intro_and_wave_transition.c`, function 0x80153A60, and still uses `*(s8 *)&game_state.unk_3A`. The applied core calls offset 0x03A `screen_brightness`. No source repair is made in this lane. Evidence: `/tmp/t27064/rpg-plan-final.log`, `/tmp/t27064/rpg-plan.sha256`, `/tmp/t27064/rpg-plan.exit`; a one-file gcc baseline check is captured in `/tmp/t27064/rpg-baseline.log`. This is not a clean pass; rerun after the filename flush settles and request any genuine source repair from the root. TACO is now the sole heavy full plan.

2026-10-24 residual census refinement: apply tables are unchanged. RPG's residual table actually contains 140 identifier entries across 67 definitions (67 arguments, six local intermediates, 48 temporaries and 19 spill slots), replacing the earlier mistaken count of rows-with-arguments as arguments. Seven original launch-selector constants are now explicit unresolved entries alongside the other 73 candidates. Comment-excluded current-source/header scans additionally record 83 RPG data identifiers and three TACO identifiers outside the named-data packets; they may be aggregate aliases and are not asserted to be independent storage definitions. Source-local types are included: RPG has 18 named type definitions with 20 remaining unknown members outside the field packet; TACO has 24 with ten unknown members. The four metadata hashes above are updated; independent verification of this census refinement is pending. No code or apply-row semantics changed.

2026-10-24 TACO full symbol plan: frozen table `19214098...` exited **0**, **673 rows, 45 files to change**, no problems. Evidence: `/tmp/t27064/taco-plan-final.log`, `/tmp/t27064/taco-plan.sha256`, `/tmp/t27064/taco-plan.exit`. Existing image, `--cpus=2 --memory=2g`, no apply/build. The corrected 944-row TT plan is now the sole heavy validation. Its table is unchanged at `cfc6ea90...`; the English game/folder terminology gate remains; technical prefix adoption is settled by a60c31bb9.

2026-10-24 census review correction: Standards found eight RPG fields already proposed under renamed type scopes were counted as unresolved under their old typedef names. The census now resolves inverse type aliases first: RPG's unknown members outside the field packet are **12**, not 20; TACO remains ten. Spec independently reproduced the 140 identifier entries, seven omitted launch constants, 83/3 data-reference residuals and 18/24 actual type definitions. The manifest above pins the corrected metadata for final verification. Apply hashes/rows remain unchanged. A small ignored RPG source/config snapshot is being prepared to avoid another live-tree filename race; it contains no disc files and performs no build or source application.

2026-10-24 final metadata verification: both independent Sol 6.1 reviewers verified the four corrected metadata hashes against the current manifest at start/end and reported no remaining actionable known findings. Spec independently reproduced all final census totals; Standards verified inverse type-scope normalization and inventory columns. Apply hashes are unchanged. Their conclusions remain bounded correction reviews, not exhaustive semantic proof or completion. The RPG snapshot at `/tmp/t27064/rpg-plan-snapshot` is ready; it copies only `src/`, `include/`, `config/` and `tools/`, with input tool/table hashes in `/tmp/t27064/rpg-snapshot-input.sha256`. Its queued dry run will use a stable baseline, including the pre-existing brightness-field gcc diagnostic.

2026-10-24 corrected TT full plan: frozen table `cfc6ea90...` exited **0**, reporting **944 rows, 34 files to change**, no problems. Evidence: `/tmp/t27064/tt-corrected-plan.log`, `/tmp/t27064/tt-corrected-plan.sha256`, `/tmp/t27064/tt-corrected-plan.exit`. Existing image, `--cpus=2 --memory=2g`, no apply/build. TT technical prefix adoption is settled by a60c31bb9; English game/folder terminology still awaits contributor agreement. The stable RPG replan is now the only heavy process in this chat.

2026-10-24 stable RPG full plan: snapshot table SHA-256 equals the live frozen `1262793f...`; `python3 -u tools/rename.py plan config/renames/proposals/o-rpgbat-apply.tsv` exited **0**, reporting **597 rows, 65 files to change**, no new problems. Evidence: `/tmp/t27064/rpg-stable-plan.log`, `/tmp/t27064/rpg-stable-plan.sha256`, `/tmp/t27064/rpg-stable-plan.exit`. Both baseline and proposed views include the existing `GameState.unk_3A` gcc error at function 0x80153A60, so the pass proves no new errors, not a generally clean source baseline. Root must resolve or account for that pre-existing spelling through the approved source/tool workflow before claiming a clean build; preserve its signed-byte cast.

2026-10-24 handoff verification: final Docker metadata audit matched all 13 frozen hashes; all four apply tables contain only supported five-column kinds and have no duplicate keys, no-op rows or duplicate local targets. Inventories have the documented five columns (EVENT/TT/RPG) or six columns (TACO). Exact current rows are **3268 total**. Final full plans all exit 0: EVENT 1054/22 files, TT 944/34, RPG 597/65 (stable snapshot), TACO 673/45. The lane has not applied source, changed headers/YAML/ledgers, built, committed or edited the root-owned shared index/status/board/log. No owned heavy validation remains running. Root should record the shared log entry and preserve this ticket In Progress until essential semantics, root application and matching proof are complete; English TT game/folder terminology remains reserved.

Final residual highlights: TT has 260 unresolved local rows, 128 unresolved parameters, 53 remaining actual unknown fields and four data declarations; 64 exact sound-call sites cover 38 distinct source command values, while 125 other original constant candidates remain unresolved. Its ten nonexistent aggregate proposals and 162 pseudo-member rows are structural work, not executable renames. EVENT's six outcome-factor role requests stay in its separate essentials inventory; the apply packet does not claim they have already been applied or planned as part of the 1054 symbol rows. RPG's current 83 data identifiers and TACO's three are explicit reference/declaration gaps, not a fabricated count of independent definitions. The bounded independent reviews do not substitute for exhaustive per-row semantic binding verification.

2026-10-24 namespace clarification: root committed `a60c31bb9` after source/policy review. TT technical `tt_`/`Tt`/`TT_` is adopted solely for TT.EXN; bare `shooter_` belongs to TACO. English game/folder terminology still awaits contributor agreement and no agreement is claimed. All 944 TT executable rows already use the correct namespace; only the obsolete gate comment and living policy descriptions changed. Exact parsed row bytes before/after are identical. New primary text hash 3bb3cbf4d95ba65e49cb681e5dd161409e5e6b46123eca79db2e5523b267ca98; parsed-row SHA-256 0393f26b2344f7f7d176c429ee0a4ae5c0c11e4963a1bc63eba791fba7e09906. Fresh plan and focused delta review are queued.

2026-10-24 root reference fix: RPGlabels_intro_and_wave_transition.c used nonexistentgame_state.unk_3A. Reboundtotheexistingbytefieldscreen_brightnessat0x03Awhilepreserving*(s8*)&. Independentreviewverifiedoriginalassemblybytewrite0x800E62BAandfreshRPGchecksumexact. Thiscorrectsareference,doesnotrenameafielddefinition; newpacketplansshouldusecurrentvalidbinding.
