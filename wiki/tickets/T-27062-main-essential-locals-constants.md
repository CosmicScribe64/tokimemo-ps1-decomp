---
id: T-27062
title: Prepare essential main locals and proven constants
status: Done
assignee: parallel-main-chat
created: 2026-10-24
updated: 2026-10-25
links: ["[[phase2]]", "[[tickets/T-27050-parallel-main-reconciliation]]", "[[tickets/T-27059-all-main-c-file-names]]"]
---

## Goal

Revalidate the frozen 247 symbol rows after the main filename moves, then prepare separate source-backed packets for essential game-logic locals/parameters and meaningful constants in m-menutext, m-sysmenu, m-hud and m-holiday. Root owns application, builds, commits and shared wiki/board/log updates.

## Acceptance criteria

- [x] All four frozen symbol plans pass against logical filenames; concrete conflicts are recorded separately.
- [x] Essential local/parameter rows use supported function-scoped syntax and pass plans.
- [x] Proven constant sites have exact context, domain, value, signedness and existing-owner reuse; unsupported tooling sites are explicit.
- [x] Independent Sol review checks meanings, comparisons, writes and collisions before handoff.
- [x] No source/header/symbol/ledger edits, application, build, clean or commit in this lane.
## Notes

The earlier 247-row symbol tables and seven reviewed filename rows remain frozen. New supplementary packets and additions to the four owned specs are separate work. Current logical source names and applied ledgers win.

## Comments

2026-10-24: Root requested this follow-up while integrating the 32-file main batch. T-27062 is created before preparation; root registers its board card. All analysis/tool dependencies stay in the existing Docker image with --cpus=2.

2026-10-24: Preparation completed; status stays In Progress for root integration. Independent Sol review passed all four essential packets and corrected manifests after findings were resolved. Root owns board/log updates, application, source/header ownership, tool fixes, builds, review and commits.

## Prepared handoff

All proposal paths below are relative to `config/renames/proposals/`. Frozen `*-apply.tsv` tables are unchanged. Do not combine superseded analysis drafts with the canonical packets.

| Unit | Frozen symbol revalidation | Canonical supplementary packet | Local plan | Constant manifest records |
|---|---|---|---|---|
| menutext | 48 rows, 18 files, exit 0 | `m-menutext-essential.tsv` | 135 rows, 3 files, exit 0 | 190 |
| HUD | 52 rows, 122 files, exit 0 | `m-hud-essential.tsv` | 95 rows, 3 files, exit 0 | 46 |
| sysmenu | 98 rows, 11 files, exit 0 | `m-sysmenu-essential.tsv` | 39 rows, 2 files, exit 0 | 229 |
| holiday | 49 rows, 6 files, exit 0 | `m-holiday-locals.tsv` | 36 rows, 1 file, exit 0 | 43 |
| Total | 247 symbol rows | 305 local/parameter rows | no PROBLEM | 508 records |

The holiday author plan is reused: final changes only normalized TSV whitespace and clarified evidence. Other final local logs are ignored `T-27062-<unit>-essential-final-plan.txt` files in `.claude/orchestrator/state/`. Some inverse checks were skipped because proposed names already occur in comments/member identifiers; compiler error-set proofs passed. These dry plans do not substitute for apply/view/hash proofs or matching builds.

Final Docker packet validation passed: all canonical rows have five TSV fields, valid current function scopes and unique kind/scope/old keys. The local total is 104 parameter rows plus 201 local rows. All four constant manifests retain their five-column header and expected record counts. Evidence: ignored `.claude/orchestrator/state/T-27062-final-packet-validation.txt`.

Seven reviewed positive constant families have dry plans: SpeakerVoiceState 14 literals/1 file; TextSpeed 26/15; HUD failure count 2/1; HUD positive scales 27/2; SettingsClickMode 5/3; SettingsConfirmButton 16/6; SettingsCursorSpeed 19/6. All exit 0: 25 definitions cover 109 matched literal sites. There are five explicitly unresolved sites: three negative graph-scale calls and two confirm-button zero comparisons in compound expressions. These families have persisted values files listed in the four owned specs. Dry runs use existing `include/main_api.h` only as visibility root; future header ownership is not applied.

## Integration gates and deferrals

- Apply the frozen symbol packets and 305 canonical local rows through current ledgers; preserve declared types and source behavior. Resolve old scopes/site names through the resulting ledger.
- The extra essential HUD local `n -> calendar_actor_cursor` is excluded from the apply table and recorded in `m-hud-essential-deferred.tsv`. A function-external `CAL_MARK` macro captures `n`; a trial failed compiler proof. It is an actor allocation/index cursor.
- Fix and independently verify whole signed-literal replacement before inserting negative constants. A synthetic Docker proof shows the current matcher retains unary minus when replacing its positive leaf, reversing the value of a negative-named definition.
- The new `const --reuse ENUM` path supports existing nonnegative int enumerators at exact function-local comparison-right-operand and array-subscript sites. Prepare current function names, owning enum headers and unambiguous occurrences individually before use. The 508-record manifests are not executable reuse command lists. Unsupported assignments/switches, negative values and arithmetic/return forms still need tool support or a reviewed exact edit; generic local or GameState-wide replacements must not collapse unrelated domains.
- Two compound-expression confirm-button zeros remain unchanged; positive scale plans leave three negative calls unresolved. Holiday candidate constants require applied unique locals and a visible owner header, and their zero initializations remain unresolved.
- Apply reviewed positive families only after resolving their current selectors and owners; run appropriate apply proofs, matching builds and final review. No phase-2 completion or source/build result is claimed by this preparation lane.

The four specs contain source-backed semantics and detailed per-domain deferrals: `spec/m-menutext`, `spec/m-hud`, `spec/m-sysmenu` and `spec/m-holiday`.

## Post-main-ui application check

2026-10-24: Root reported actual application of all 247 frozen symbols through `config/renames/main-ui.txt` (129 affected files); matching build and post-apply review remain root's combined-wave work. This lane repeated all four supplementary local plans against that live ledger, serially in Docker. All passed with unchanged counts: menutext 135 rows/3 files, HUD 95/3, sysmenu 39/2 and holiday 36/1; exit 0 and no `PROBLEM`. Historical table scopes resolve through the ledger, so no speculative rewrite to future names was needed.

All seven positive constant dry plans were repeated after the symbol application. The live scalar selector is `speaker_voice_state`; the biorhythm call selector is `main:hud_biorhythm_value(arg3)`. Other selectors are unchanged. All exited 0 and retained 109 matched literals with the same five unresolved sites. Logs are `.claude/orchestrator/state/T-27062-<unit>-post-main-ui-plan.txt` and `T-27062-<family>-post-main-ui-const-plan.txt` (ignored).

Independent Sol follow-up review passed: all 305 local/parameter rows resolve to the same 114 current definitions, and all seven current constant selectors retain their meanings with no new collision issue. The new exact nonnegative enum-reuse subset is acknowledged above; legacy signed-literal, macro-capture and five unresolved-site deferrals remain. No source/header/ledger edit, application, build or Git mutation was performed by this lane.

## Root integration underway

2026-10-24: Root applied the 305 reviewed local/parameter rows together with the six reviewed
core follow-up rows using the aggregate `main-essential-311.tsv` and `main-essential` ledger.
The application wrote all rows, then its final guard detected that the new `mascot_choice`
parameter reused a retired global spelling. A scoped tool-generated chain to
`mascot_selection` resolves that collision while preserving the ledger history. Actual-source
review and the matching build are pending; this is not a completed integration claim.

## Final integration evidence (2026-10-25)

Reviewed source digest: `687b5ce789d59f154b527264d23227b741a08bc619e4a1f8c67d617257f4c4f3`. Final clean matching proof: 27/27 binaries, 6,952 functions, 2,276,920 bytes. Independent original Astra review and separate fixer Astra review passed this exact source digest. See `.claude/orchestrator/state/T-27000/final-tracking/criterion-evidence.md`.

