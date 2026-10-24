---
type: spec
unit: reconciliation
updated: 2026-10-24
sources: ["config/renames/core.txt", "config/renames/school-day.txt", "src/main/8006CB30.c", "config/objects/main.txt", "config/renames/proposals/m-scheduler.tsv", "config/renames/proposals/m-scheduler-apply.tsv"]
---

# Phase 2 proposal reconciliation

[[tickets/T-27009-p2-reconcile|T-27009]] owns executable batches reconciled against the applied ledgers.
An analysis proposal is evidence to revisit, not a list to apply unchanged. The source declarations and
`rename.py lookup`/`export` establish the current spellings. The owning object's body establishes the
meaning; callers alone do not prove a game interpretation.

## Scheduler batch

`config/renames/proposals/m-scheduler-apply.tsv` contains 40 executable rows: all 33 remaining
placeholder functions defined in `src/main/8006CB30.c`, the pre-phase-2 non-O.BIN name
`special_event_select`, and six globals in the object's owned data range
`0x800B6740..0x800B6760` (exclusive end). `config/objects/main.txt` records code ownership as
`0x8006CB30..0x800737A0`. This batch changes symbol names and their uses only; it does not rename
files or split headers.

The original analysis is [[m-scheduler]]. Its long-form rule descriptions remain useful, but its
proposed identifier spellings and core aliases are stale. This reconciliation page and the executable
table override those spellings. Refresh the original spec after application, using the applied ledger
for exact names. Until then, the names below are proposals, not applied code.

Every new name starts with `schedule_` or `day_event_`, the ADR's prefixes for this owning object.
Memory-card polling, command icons, meetings and health handlers keep the scheduler prefix even when
other objects call them. Mode handlers end in `_mode_handler`. Existing applied-core names remain
authoritative, including `calendar_advance_day`, `girl_counters_clamp` and
`girl_flags_bit0_clear`; this batch does not reopen the core ledger's decisions.

The body pass made these corrections to the original proposal:

- `special_event_select` is in `config/symbol_addrs.txt`, not the O.BIN file. It becomes
  `schedule_special_scene_select`; the O.BIN names, including `_schedule_init` and the developer's
  `schdeule_girls_param` typo, stay unchanged.
- `D_800B6740` becomes `schedule_last_picked_girl`. `geko_judge` and `date_sasoi_judge` each reject
  only the last result and then replace it. A sequence A, B, A remains possible. The original
  `week_picked_girl` note's claim that a girl could never repeat within one build was too strong.
  Moreover, `schedule_girls_gakko` can discard an invitation after its judge has already changed this
  global; that prevents the following encounter from selecting the just-discarded girl, not all later
  encounters in the entire build.
- `func_8006EA40` becomes `schedule_miharu_scene4_ready`, describing the queue builder's actual
  girl 11/scene 4 output. Calling it a final scene was not proved by this predicate.
- `func_80071110` becomes `schedule_yuina_highest_points_check`. Its body proves a highest-points
  test, a 500-point threshold and the Rei/Shiori exception. It does not prove that these points mean an
  ending-candidate rank. Girl 12's fourth flag byte remains uninterpreted in the note.
- `func_8006D138` becomes `schedule_progress_state_update`, a factual description of its writes.
  The date-place-tip interpretation remains a hypothesis in the old analysis; it is not built into
  the executable name.
- `func_8006D038` clears every girl's scene bits as well as expiring last-date spots 0x23/0x24.
  `calendar_advance_day` calls it every day in quarterly months, rather than only on month entry.
- The five rank-bar globals receive command-specific names tied to their actual switch arms, rather
  than opaque suffixes a through e. The separate Shiori athletics bar is distinguished from the
  liberal-arts/athletics bar used for girls 1, 5 and 6.

Representative ledger reconciliation: `lookup func_8006D7EC` reports
`func_8006D7EC -> day_events_build_week -> day_event_queue_build_week` in `config/renames/core.txt`.
The executable table does not reintroduce either old alias. `lookup special_event_select` reports no
ledger entry; its current definition and symbol config establish the old name for that row. `lookup
func_800737A0` also has no entry; this out-of-range school-day handler is left alone. The applied
`school-day.txt` ledger names other functions in that file and supersedes their old proposal aliases.

All 40 notes give body evidence. No row needs a hypothesis symbol file: each name expresses the observed
operation and leaves uncertain interpretations out. Imported emulator anecdotes were omitted because
the proposal did not provide reproducible commands for those runs.

Final Docker `rename.py plan` passed after the Yuina-name refinement: **40 rows, 10 files to change**,
exit 0. Independent table review is required before application. Application, inverse/gcc/views
proofs, the clean build and 27/27 checksum verification belong to the applying agent; they have not been
performed as part of preparing this table.

## Deferred ownership and row kinds

- `D_800B3C6C` (`debug_menu_active` hypothesis in the old table) is outside this object's data range;
  the old spec identifies OPTION's debug page as its topic. Its owning main unit must resolve the name
  and prefix. This batch keeps the original symbol.
- `D_80122DD0` (`overlay_scratch` in the old table) is a cross-overlay scratch area without scheduler
  ownership established by this object's data map. Defer to shared-core/overlay ownership analysis.
- Shared types, fields and constants are not copied from the scheduler table. Core already renamed
  many; remaining flag meanings such as girl flag b2 need separate review against all users. A shared
  field's rename must use its current type name and identifier from the ledger, not a stale alias.
- Comment requests on O.BIN definitions, enum proposals, local/parameter work, the proposed
  `scheduler.c` file rename and any header move remain separate batches. No `comment`, `file`, `type`,
  `field` or `const` row is present in this executable table.

## Recipe for the next main unit

1. Establish object code/data ownership from `config/objects/main.txt`; list definitions in its C file.
2. Export the applied ledger and use `lookup` for old proposal aliases. Keep applied names unless a
   specific reviewed correction is needed; do not silently replay old proposals.
3. Separate owned functions/data from cross-unit requests and unsupported or later-stage row kinds.
   Preserve every actual O.BIN name by checking the O.BIN symbol file itself.
4. Read each function body, relevant struct/enum declarations and the callers that establish its outputs.
   Choose the owner's prefix. Prefer observable operations to unproved narrative meanings; otherwise
   start the note with `HYPOTHESIS:` and retain the hypothesis marking through application.
5. Put only the executable rows into a fresh `*-apply.tsv`; retain evidence and deferrals separately.
   Run `rename.py plan` under Docker `--cpus=2` until it exits 0, then obtain independent review.
6. Apply only the reviewed table through `rename.py`, preserve its generated ledger, run the clean
   matching build, and refresh the living unit spec using the resulting current-name map. File/header
   work follows in separate reviewed batches.
