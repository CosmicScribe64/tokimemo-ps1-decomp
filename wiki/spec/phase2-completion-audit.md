---
type: audit
updated: 2026-10-24
ticket: T-27052
sources: ["wiki/phase2.md", "wiki/tickets/T-27000-phase-2.md", "docs/adr/0001-phase-2-naming.md", "CODING_STANDARDS.md", "config/renames", "config/renames/proposals", "src", "include"]
---

# Phase 2 acceptance audit and closeout order

Phase 2 is **not complete**. The code needed to begin a school-day Godot slice is substantially more
readable after the applied core, school-day and scheduler batches. Neither the presence of all unit
drafts nor a matching build establishes the remaining naming and explanation criteria. This audit
separates the early port-start milestone from the accepted T-27000 exit; it does not amend that ticket.

Snapshot: 2026-10-24, checkout HEAD `c4151d9f6ccaa31bcdb879edf7a3c3e2ceba9edd`, with concurrent,
uncommitted integration and analysis changes. Inventories below were captured around 18:06 UTC. The
applying coordinator must refresh them after its final application. This lane made no source, tool,
symbol-file or ledger changes, ran no build, and makes no new checksum claim.

Priority update from the user (2026-10-24): logical C filenames and a logical topic-header breakdown
are first-class work now. The first concrete migration is the already-understood scheduler/school-day
pair in [[spec/header-split-plan]]: `src/main/8006CB30.c` -> `src/main/scheduler.c` and
`src/main/800737A0.c` -> `src/main/school_day.c`, with exact object intervals, unchanged signatures,
topic owners and type-view proof. Prepare this alongside further semantic batches; do not defer all
file/header work until all 28 units' naming backlogs are exhausted. Header extraction and C path moves
remain separate reviewed batches. For these two already-understood objects the concrete pilot delivers
the two path moves first, then owner-aware tooling and the topic-header extraction; no new semantic
identifier rename or object split is bundled into either path move.

## Acceptance state from artifacts

| T-27000 requirement | Evidence actually present | Remaining measurable gate |
|---|---|---|
| Conventions and rename tool | ADR 0001, glossary and standards exist; `tools/rename.py` implements ledger/replay/check and now accepts `enumerator` | Keep these rules when extracting executable batches; the historical status claim that enum-member renames are unsupported is stale |
| O.BIN/SDK alias pilot | `config/renames/obin.txt` and `sdk.txt` exist; status records the earlier 27/27 application | Verify ticket review and alias/check result when reconciling T-27000 checkboxes; comments on game-side O.BIN definitions are a separate unfinished requirement |
| Shared core | `core.txt` names the state and shared records; `game_enums.h` contains initial enums | Finish enum naming and semantic corrections, classify still-unknown used fields, complete essential flags and values, refresh live specs from the applied map |
| All main game logic and all 26 overlays named and explained | All 28 analysis-unit proposal/spec pairs exist; core, school-day and scheduler have applied ledgers | Resolve every in-scope definition/type/field/table into an applied factual name or an allowed, evidenced exception; complete state transitions and persistent effects in partial specs |
| Hardware module names and game-facing entry points | `h-graphics`, `h-cdio`, and the correctly identified audio-driver `m-dates-a` analyses exist | Apply reviewed entry-point names; name files/headers; enumerate remaining internal hardware placeholders as allowed residue |
| Essential locals, parameters, constants | School-day ledger includes essential roles in `parameter_up_down`; initial shared enums exist | Apply unit-specific command/result/flag/threshold names at exact semantic sites; preserve C89/K&R widths and signs |
| English source files/folders/topic headers | All 437 C files still have address basenames; both umbrella headers and 26 disc-named overlay headers exist | Complete ownership/header-tool work in [[spec/header-split-plan]], then separate file/dir batches and umbrella deletion |
| Final layer-aware residue and current documentation | Historical survey script exists under `drafts/T-25900/count_names.py`; living specs contain mixed applied/proposed aliases | Refresh survey for topic paths, classify every residual game placeholder, refresh current docs, resolve review findings and record final proof |
| 27/27 sha1 and 6,952 functions | Coordinator status records verified phase 1/core/school-day results; scheduler application and clean proof were in progress during this audit | Applying coordinator records clean proof after each accepted mutation; final proof at the exact completion revision, never inferred from source/doc inventories |

T-27000 still has unchecked pilot/core criteria despite recorded earlier integration. This is tracker
reconciliation, not evidence that no work happened. Conversely, checking those boxes cannot close the
all-units, file/header or residue requirements.

## What the 28-table inventory establishes

The assignment inventory is `.claude/orchestrator/state/p2-units.json`: 12 shared/main/hardware units
and 16 overlay units, the latter collectively covering all 26 configured overlays. Each has its own
`config/renames/proposals/<unit>.tsv` and `wiki/spec/<unit>.md` at this snapshot. This is existence
coverage only. Supplemental `*-apply`, cross-main request and deferred tables are not extra units and
must not be double-counted as coverage.

The 28 base tables contain **11,013 five-column rows**, with zero malformed tab-column rows in the
mechanical inventory. Their kinds are: 4,579 `func`, 2,384 `data`, 996 `field`, 353 `type`, 2,007 `const`,
331 `comment`, 304 `file`, two `dir`, three `param`, 34 `local`, and 20 `function` rows. A five-column
row is not necessarily a valid rename row:

- `tools/rename.py:115` accepts `func`, not `function`, and has no `comment` row kind. The 20 `function`
  rows are in `o-bunkasd-shugaku` (seven) and `o-small` (13). They require reviewed extraction into
  executable `func` rows; do not mass-apply these base tables.
- `const` is a supported special mechanism for literal replacement, with values and semantic sites
  supplied to `rename.py const`. Old proposal rows that describe a desired enum or a prose comment
  are not proof of an executable const operation. Inspect their payload and use the dedicated command.
  Ordinary `plan` rejects const rows; 2,358 rows total need extraction/conversion rather than ordinary
  table planning (2,007 const + 331 comment + 20 function). The other 8,655 merely have recognized
  ordinary row kinds, not proven semantics/ownership/collision-free plans.
- Many files mix identifier and `file`/`dir` requests; `rename.py` requires file/dir tables to hold
  only moves. Header splits are structural moves and need their own reviewed changes.
- Applied ledgers are authoritative and not named after every proposal unit: `core`, `obin`, `sdk`,
  `school-day` and `scheduler` exist here. A missing `<proposal-unit>.txt` does not prove its symbols
  are unapplied. Resolve each old name by unit/address and ledger history before planning a batch.
- Historical conflicts (135 conflicting keys/four collisions in the earlier 10,085-row snapshot)
  are not current totals. Recompute against today's source and applied map before applying.

Reproduce the inventory with ignored scratch
`.claude/orchestrator/state/audit27052-inventory.py` under Docker; its JSON output records per-unit
row kinds. That scratch is an audit aid, not a committed tool or acceptance dependency. The prepared
reconciliation recipe in [[spec/reconcile]] is the pattern for executable subsets.

| Base analysis unit | Draft rows | Base analysis unit | Draft rows |
|---|---:|---|---:|
| core | 751 | o-date | 1,103 |
| m-scheduler | 115 | o-event | 1,136 |
| m-weekday | 71 | o-tt | 995 |
| m-holiday | 140 | o-bunkasai | 344 |
| m-dates-a | 202 | o-bunkaken | 457 |
| m-dates-b | 255 | o-rpgbat | 650 |
| m-hud | 201 | o-taco | 912 |
| m-sysmenu | 263 | o-etc | 779 |
| m-menutext | 153 | o-nameent | 639 |
| m-flow | 324 | o-taiiku | 38 |
| h-graphics | 263 | o-geko | 975 |
| h-cdio | 187 | o-shougatu | 15 |
| o-tel-option | 15 | o-master-ending | 10 |
| o-bunkasd-shugaku | 7 | o-small | 13 |

The first inventory's ledger snapshot has 780 rows: core 596, O.BIN 104, SDK 24, scheduler 40 and school-day
16. The scheduler ledger contains 34 functions/six data names; school-day contains nine functions,
three parameters/four locals. Ledger row totals and historical applied-row totals are different
snapshots/metrics and must not be combined. No base-table coverage percentage follows from them.
Additional new `*-apply.tsv` files prepared by the main/overlay lanes are still proposals until their
application and proof appear in the ledger/source and applying ticket.

The first spec-index snapshot omitted the newly created `o-master-ending`, `o-bunkasd-shugaku` and
`o-small` pages; root has since registered those pages. The omission was not missing underlying
analysis files. DATE and DATE2
are distinct overlays, while SHOUGATU/GYOZI and TEL/OPTION each share a unit/spec. Counting index links
without this mapping produces a false overlay-coverage total.

## Residue: diagnostic, not a to-do count

The existing read-only survey reported 6,952 game functions, with **6,515 `func_` names**; 16,672 data
labels, with 16,586 number names; 2,152 struct/union members, with 1,172 unknown/padding/field patterns;
661 types, with 409 shape/address patterns; 11,151 m2c-name occurrences; 52,372 hex literals >=0x10;
and 437 C files. Its main row has 830 functions/410 placeholders and 34 C files. SDK residue is
reported separately: 722 assembly functions/479 placeholders.

These are mixed-layer diagnostics. Function/data label counts depend on generated `asm/` from the
existing build; header scanning hardcodes the current umbrellas/disc headers. The main unit contains
hardware, and unknown/padding is partly allowed. Do not subtract proposal rows from these totals, call
all remaining labels game logic, or treat every hex literal/m2c temp as a phase-2 requirement. The
final survey must follow renamed topic headers and identify the layer and reason for each exemption.

Current invocation (inside the existing image, no build/install):

```sh
docker run --rm --cpus=2 -v "$PWD:/work" -w /work tokimemo-decomp-arm64 \
  python3 drafts/T-25900/count_names.py --tsv
```

There is no `tools/count_names.py` at this snapshot; plan text using that basename refers to the draft
survey, not an already-integrated completion checker. Promote or replace it in a separate tooling
ticket, with path-aware tests, before relying on it after migration.

## Concrete source-file naming queue

The user specifically prioritizes logical file names. The first accepted design target is the
scheduler/school-day pair, not another unnamed future sweep. [[spec/header-split-plan]] gives its exact
header declaration manifest and migration sequence. The paths below are the 34 existing proposal
destinations, reconciled to the **actual 34 objects** in `config/objects/main.txt`; all intervals are
start-inclusive/end-exclusive. Both file columns are relative to `src/main/`, so for example the first
row means `src/main/80041000.c` -> `src/main/game_init.c`. Preserve one C file per existing object.

| Current file | Logical file | Text interval | Content evidence / owning draft |
|---|---|---|---|
| `80041000.c` | `game_init.c` | 0x80041000-0x800420D0 | `gpu_init`, `game_state_init`, record/BG initialization; m-flow |
| `800420D0.c` | `main_loop.c` | 0x800420D0-0x80042540 | entry, `boot_loop`, per-frame dispatch and timer service; m-flow |
| `80042540.c` | `game_mode.c` | 0x80042540-0x80042A00 | `game_mode_dispatch`, mode/stage/step helpers; m-flow |
| `80042A00.c` | `pad_input.c` | 0x80042A00-0x80043510 | pad/mouse polling, soft reset, secret code; m-flow |
| `80043510.c` | `frame_render.c` | 0x80043510-0x800451D0 | `draw_frame`, upload queues and audio-bank loading in the same object; h-graphics |
| `800451D0.c` | `cd_queue.c` | 0x800451D0-0x80046500 | `cd_command_pump`, CD/audio request operations; h-cdio |
| `80046500.c` | `reset_iris_wipe.c` | 0x80046500-0x80047550 | `game_mode_reset_to_title`, numeric helpers, iris transition; h-cdio |
| `80047550.c` | `actor_sprites.c` | 0x80047550-0x800490C0 | actor clear/draw, texture blocks and sprite rendering; h-graphics |
| `800490C0.c` | `gpu_prims.c` | 0x800490C0-0x80049FF0 | `SetWorkBase`/`GetWorkBase`, primitive setup; h-graphics |
| `80049FF0.c` | `gpu_effects.c` | 0x80049FF0-0x8004AD60 | gradient/rectangle/disc/flash drawing paths; h-graphics |
| `8004AD60.c` | `debug_hud_palette.c` | 0x8004AD60-0x8004C3F0 | `draw_debug_number`, `col2sepia`, palette helpers; h-graphics |
| `8004C3F0.c` | `screen_layers.c` | 0x8004C3F0-0x8004E500 | `srn_*`, `mod_*`, `get_fnt`; h-graphics |
| `8004E500.c` | `text_system.c` | 0x8004E500-0x8004F6B0 | typewriter/glyph/text functions `disp_string`, `check_end_k`; m-menutext |
| `8004F6B0.c` | `debug_vram_view.c` | 0x8004F6B0-0x8004F870 | one developer VRAM-view function; m-menutext |
| `8004F870.c` | `menu_speaker_girl.c` | 0x8004F870-0x80053650 | menus, speaker lines and girl queries including birthdays share one object; m-menutext |
| `80053650.c` | `memory_card.c` | 0x80053650-0x80056300 | `Hw_*`/`Sw_*`, file/save jobs, two MDEC helpers; m-flow |
| `80056300.c` | `reload_exe.c` | 0x80056300-0x800563F0 | sole `exit_function` shuts down and reloads executable; h-cdio |
| `800563F0.c` | `movie_player.c` | 0x800563F0-0x80057390 | STR callback/stream player and timed text; h-cdio |
| `80057390.c` | `dec_bg.c` | 0x80057390-0x80058D20 | `dec_bg_*` read/decode/draw/cache interface; h-cdio |
| `80058D20.c` | `model_3d.c` | 0x80058D20-0x80059710 | 3D view/light/coordinate and model slots; h-cdio |
| `80059710.c` | `mouse.c` | 0x80059710-0x80059A20 | `InitMouse`, `MouseState`, `RangeMouse`, `SenseMouse`; h-cdio |
| `80059A20.c` | `debug_info.c` | 0x80059A20-0x80059B40 | reads mode/stage/step/timers for debug numbers; separate error reporter; h-cdio |
| `80059B40.c` | `boot_mode.c` | 0x80059B40-0x8005A0B0 | boot resource reads, page/palette relocation and HUD initialization; h-cdio |
| `8005A0B0.c` | `home_room.c` | 0x8005A0B0-0x80061710 | weekly commands, settings, clubs, rumors and holiday preparation; m-sysmenu |
| `80061710.c` | `title_screen.c` | 0x80061710-0x80062CD0 | title selection and 3D photo intro/start mode; m-flow |
| `80062CD0.c` | `hud_panels.c` | 0x80062CD0-0x800674B0 | portrait, parameters, date and message panels, command icons; m-hud |
| `800674B0.c` | `hud_screens.c` | 0x800674B0-0x8006CB30 | calendar/magazine/save/appointment/club screens, command roll; m-hud |
| `8006CB30.c` | **`scheduler.c` (first migration)** | 0x8006CB30-0x800737A0 | 56 definitions: queue build/read, calendar advance, encounter selection and weekly health; m-scheduler |
| `800737A0.c` | **`school_day.c` (first migration)** | 0x800737A0-0x80075320 | 19 definitions: `week_day`, `parameter_up_down`, vacation setup and week-end exit; m-weekday |
| `80075320.c` | `holiday.c` | 0x80075320-0x800789E0 | holiday command screen, appointments/phone/magazine/club paths; m-holiday |
| `800789E0.c` | `overlay_loader.c` | 0x800789E0-0x80079B10 | loader dispatch, sector/entry tables and TT loader; m-flow |
| `80079B10.c` | `sound_driver.c` | 0x80079B10-0x8007C030 | `audio_command_pump`, sound banks/music/effects/XA; m-dates-a is audio, despite its historical unit label |
| `8007C030.c` | `scene_library.c` | 0x8007C030-0x80085E30 | scene steps, portraits/talk/choices, SD tables, limits/day helpers; m-dates-b |
| `80085E30.c` | `scene_display.c` | 0x80085E30-0x80086810 | fades, finger cursor, scroll/timer/display helpers; h-graphics |

These are exact path candidates, not 34 approved rename plans. Two cautions before execution:

- Several original file rows use whole paths as `old/new`, and several `file main - include/...`
  rows actually request a new split header. Neither is the syntax of a C-object rename. The applying
  agent extracts `file main 8006CB30 scheduler` and `file main 800737A0 school_day` into a move-only
  table; a new header is a structural extraction, not a rename of `-`. No table is applied directly
  from this summary.
- Some draft notes propose future C splits (`scheduler/day_events/calendar/health_check`, or separate
  HUD/audio files). That changes original object boundaries and is outside this filename migration.
  Header topics may be finer than C objects. `menu_speaker_girl.c` deliberately makes its mixed
  content explicit; its APIs belong to several topic headers while the object stays intact.

For the first two files, the exact linked ownership is: scheduler rodata
0x800B1140-0x800B14F0 and initialized data 0x800B6740-0x800B6760; school-day rodata
0x800B14F0-0x800B1590 and initialized data 0x800B6760-0x800B6990. Neither has a BSS row in the
object map. The current yaml has named `c`/`.rodata` subsegments for these objects, **no named
`.data` subsegment for either**: both initialized-data ranges lie inside the generic `data_800B5950`
segment. Rename the two named subsegments per object through `rename.py`; keep that generic data
segment and neighboring islands unchanged. Include the exact text
interval and `config/objects/main.txt` pointer in each C file's first comment. No new boundary analysis,
asset data, source behavior change or SDK-signature cleanup is part of either move.

The two valid move rows were dry-run in Docker against the live checkout with a read-only mount:

```sh
docker run --rm --cpus=2 -e PYTHONDONTWRITEBYTECODE=1 -v "$PWD:/work:ro" -w /work \
  tokimemo-decomp-arm64 python3 tools/rename.py plan \
  .claude/orchestrator/state/audit27052-main-file-pilot.tsv --ledger main-files
```

Result: **exit 0, two rows/six files to change (dry run)**. Each row changes two yaml lines, neither
changes an asm-path string or config key. The only existing source-comment path reference rewritten
is `src/ovl/KANGEI/80132E40.c:319`, from `src/main/800737A0.c` to `src/main/school_day.c`. The six
changed paths count the two old and two new C paths, `config/SLPM_86.053.yaml`, and that KANGEI comment.
The tool also adds the original-object first comments. No source/ledger was written by this plan.
The scratch TSV is ignored; root should put these reviewed rows in its owned application table and
repeat plan against the final source state. Plan exit 0 is not the application/view/build proof.

## Verified rule dependencies and actual open work

### A school-day Godot slice can start before full phase 2 closes

Use [[spec/m-weekday]], [[spec/m-scheduler]], [[spec/m-sysmenu]], [[spec/m-hud]], [[spec/m-menutext]],
[[spec/core]] and [[spec/m-flow]] as the source map. Resolve their identifiers through applied ledgers;
draft prose explicitly uses sibling proposals. The minimal complete slice contract is:

1. Initial state and the shared fixed-width/16.16 record view; player parameters, club and health
   fields, calendar/day kind, command bits, queue record interpretation and indices.
2. Command availability/selection -> queue construction from tomorrow -> fixed poor/companion choice
   at queue-build time -> record dispatch -> apply command once -> clamp effects -> advance calendar
   and return to command selection at the queue terminator. Include weekend health/girl updates for a
   multiweek slice. State explicitly if the first slice ends before those transitions.
3. Exact signed arithmetic, integer truncation and RNG call order for command effects/poor results;
   rest/club/restricted-health/vacation branches and persistent club practice increments.
4. Input and presentation completion interfaces that Godot may implement with its own UI, animation,
   audio and resource loaders. PS1 renderer internals do not block this contract.

This is a port-start gate, not permission to mark T-27000 Done. The accepted ticket still requires all
game modules/overlays and the final header/file work. No human decision is needed to keep working in
the stated order. A reduced final definition of Done would require an explicit change to T-27000/ADR;
do not infer that change from the phrase "start Godot".

Concrete source residue even in this slice: `src/main/800737A0.c:7` still defines the game-logic
week-end exit as `func_800737A0`. `parameter_up_down` at line 453 has good role parameters and readable
delta arrays, but the health value 3, commands 4/5/6/7, parameter slot 8, flag nibbles and fixed-point
units remain literal. At lines 484-518 positive gains scale differently from nonpositive gains, and
stress has its own negative/nonnegative branches plus the poor-result term. Preserve those exact
directions; a single "multiply all deltas" port formula is wrong. The local `h` represents first the
health nibble and then the club word, so its remaining readability must be judged by those uses, not
by an m2c-name grep. Record essential constants at exact sites through the naming tool.

The newly applied scheduler has no remaining `func_` definitions in its owning object, but that does
not close its essential locals/flags, O.BIN comments, file/header or hypothesis work.

### The remaining overlay drafts need semantic completion

| Unit(s) | Evidence-backed partial coverage | Concrete next gate |
|---|---|---|
| TAIIKU | 213 definitions/19 objects; draft names 15 functions and describes selected runner/controller/reward paths | Trace completion/results and both `D_801499D4` result conditions, all callbacks in `D_80149E08`, remaining input/win paths; distinguish scene titles from proven mechanics |
| GEKO | Five entries, invitation/walk-home/birthday predicates and persistent effects in the partial spec | Complete each reachable callback/state/table, relationship and appointment writes; confirm ADR folder/context via a reproducible emulator run |
| SHOUGATU/GYOZI | Candidate/rank/fortune/birthday branches, with explicit sibling differences | Establish shrine/Christmas eligibility and all route/results; use each sibling's own thresholds; repeated loader search for GYOZI before `_unused` becomes a factual folder claim |
| TEL/OPTION | Selected invitation eligibility, relationship effects and sound-test flow | Finish invitation rejection/success/cancel, appointment and persistent writes, girl-profile routes, options/extras/debug state machines; essential flags and IDs |
| MASTER/ENDING | Ten proposed function names; mastery threshold and candidate selection described literally | Name remaining callbacks/data, resolve outcome flags and confession/final result persistence, preserve rank/points tie directions and fallback order |
| BUNKA_SD/SHUGAKU | Primary dispatch and selected setup/result paths | Complete per-show/per-trip branches, companion selection, stage tables and persistent reward/flag effects |
| KANGEI/DATE2/EN_NICHI/OLH/OMIMAI/RENSYU/VALEN | Seven-overlay checkpoint with selected source-backed predicates and exits | Per overlay, finish actual reachable state/table/results; confirm KANGEI content and VALEN White-Day claim before adopting factual narrative names |
| DATE/EVENT/BUNKASAI/BUNKAKEN/ETC/NAME_ENT/RPG_BAT/TACO/TT | Longer analysis specs/table proposals exist | Reconcile every row against current declarations and owning source, apply reviewed subsets, then audit rule/table coverage; length and ticket status do not certify exhaustiveness |

TAIIKU's concrete persistent writes are at `src/ovl/TAIIKU/80138950.c:438`: under result-array entry
0 being zero all 13 girls gain four tokimeki and the selected girl another two plus ten affection;
the entry-1-only branch gives two to all, another one and five affection to the selection. These are
observable changes. Their competition/rank labels need the producers, not a speculative win name.

ENDING provides another exact contract at `src/ovl/ENDING/80133030.c:37`: the ordinary candidate
loop starts at 499 and replaces only on a strictly larger met-girl points value, so eligibility is
at least 500 and ties keep the earlier index. The preceding Rei branch specifically tests Shiori's
points below 500, girl 12's fourth flag byte at least 70 and scene flag 0x4F equal to 2. Later Miharu
fallback is a separate conditional random branch. The current partial spec records these directions;
it does not establish that all later confession and outcome flags are understood.

The initial `o-small`/`o-bunkasd-shugaku` drafts use `function` and have prose-first hypothesis notes;
for a semantic hypothesis the row note must begin `HYPOTHESIS:` so the naming tool preserves it.
Writing the word midway through an evidence note does not trigger the tool's marker mechanism.

### Mode meaning is a separate correction from enum style

`include/game_enums.h:175` at the audit snapshot defines `MainMode`: 0x11 is `MODE_PROLOGUE`, 0x13 is
`MODE_NAME_ENTRY`, 0x14/0x15 are `MODE_NAME_ENT_3/4`. The original prefix-only `game-mode-apply.tsv`
would preserve these meanings. In contrast, [[spec/o-nameent]] section 1 establishes 0x11 attract opening, 0x13 file
select, 0x14 name entry and 0x15 voiced prologue. The loader at `src/main/800789E0.c:358` selects
entry 0x80144AD0 for 0x11 and 0x8014AF28 for 0x15. NAME_ENT's distinct bodies/callers are the semantic
evidence; merely renaming `MODE_*` to `GAME_MODE_*` would leave this disagreement intact.

During this audit, the coordinator revised `game-mode-apply.tsv` to use the four correct semantic
names (`GAME_MODE_OPENING`, `GAME_MODE_FILE_SELECT`, `GAME_MODE_NAME_ENTRY`, `GAME_MODE_PROLOGUE`)
at the unchanged 0x11/0x13/0x14/0x15 values. T-27043 is active integration work: root is applying this
reviewed batch, including the enum comment corrections and clean proof. This is no longer an unresolved
naming decision. Refresh to the durable current source/ledger state after its proof completes.

The core-owned atomic semantic enum batch keeps the four numeric values unchanged; finish its
lookup-based living-spec refresh after application. `o-nameent.tsv`'s historical proposal
to delete/recreate core constants is not an instruction to hand-edit names or duplicate enum values.
Other numeric mode suffixes (`OPTION_2/3`, `ETC_C2/C3/C4`) also need their body/caller meanings or explicit
hypotheses; style compliance alone is not full game-rule readability.

### O.BIN comments and runtime evidence

Preserve every actual O.BIN spelling, including `_schedule_init` and `schdeule_girls_param`. A `comment`
proposal is not an applied comment. Examples lacking the required nearby one-line English definition
comment at this snapshot: `week_day` (`src/main/800737A0.c:18`), `_schedule_init`
(`src/main/8006CB30.c:16`) and `schdeule_girls_param` (`src/main/8006CB30.c:1477`).
`parameter_up_down` has its required English summary. This is a source-comment audit, not alias debt.

ADR confirmation entries are BUNKAKEN, ETC (0xC2-0xC4), GEKO, KANGEI, SHOUGATU, TT and VALEN
(White Day); EVENT/GYOZI require another loader search before retaining `_unused`. The TT analysis
explicitly reserves applying names/folder until agreement with its contributor. No such agreement is
established by this audit. Analysis and mechanical planning may continue; root must resolve that
existing agreement gate or obtain an explicit scope/decision change before final TT naming.

For each pending hypothesis, record (a) defining producer and all meaning-relevant consumers,
(b) factual operation name usable immediately, (c) exact unproved narrative claim, (d) command and
inputs/state required to prove it. Use [[emulator]]'s Docker wrapper; save states/results/images stay
ignored. Runtime evidence must include reproducible subcommand, state prerequisites, mode/overlay,
memory/input changes and observed transition/write. A screenshot or inherited anecdote alone is not
the rename-note evidence required by ADR 0001. Already proven operational names need no unnecessary
runtime test; preserve hypotheses honestly where the port contract does not depend on their meaning.

## Dependency-ordered closeout gates

1. **Pin coverage and current names.** Root records exact commit plus applied-but-uncommitted batches;
   export/lookup the ledger. Map 28 analysis units to every main object and all 26 overlays. For each
   owned definition, type/field, data table and state callback, mark factual/applied, hypothesis,
   allowed unknown/unused, hardware internal or SDK. Exit: no unowned in-scope item and no draft-only
   name counted as applied. Complete the active T-27043 proof/spec refresh for the corrected modes.
2. **Finish the first port contract.** Complete the school-day multiweek contract above, preserving
   exact arithmetic/signs/ties, queue timing and persistent writes. Extract remaining readable core,
   weekday, command-screen, calendar/HUD and girl-query dependencies first. Exit: each rule resolves
   to its current C definition and an explicit state/input/output contract; remaining presentation
   dependencies have named interfaces. This allows Godot implementation to begin.
3. **Reconcile and apply main units.** Main ownership first; overlay requests for main symbols go to
   the owner. Separate function/data, type/field, essential constants/local/param and O.BIN-comment
   work. Exit per table: body-backed independent review, supported executable rows, plan exit 0,
   apply/inverse/compiler-view proofs, ledger/check/replay, clean 27/27 and 6,952. Refresh its spec
   immediately from the applied map. Do not repeat completed batch work.
   Keep main-bss symbols in EVENT's overlap (0x800F6000-0x80126000) in main-only symbol/hypothesis
   files; `check_symbols.py` must continue rejecting them from files EVENT loads.
4. **Complete and apply all overlay game rules.** Prioritize live narrative/calendar/date/relationship
   paths before optional mini-game internals, while keeping all accepted unit criteria open until
   complete. Close the partial paths in the table above and confirm flagged contexts. Exit per
   overlay: full transition/callback/data ownership map, exact choices/results/persistent effects,
   factual names applied, essential locals/constants applied, hypotheses documented/marked, and the
   same review/matching gates. Resolve TT agreement and unused-overlay naming gates explicitly.
5. **Migrate headers safely, beginning with the current pilot.** Execute [[spec/header-split-plan]]:
   prioritize the exact scheduler/school-day paths and declaration owners; teach declaration/view guards topic
   ownership before removing central declarations; split type dependencies then APIs/overlays;
   migrate all consumers and overrides with unchanged views. Exit: topic ownership unique, snapshot
   compare unchanged, guard/test coverage does not disappear, direct includes complete. Keep umbrella
   shims until the last consumer is migrated.
6. **Name files/folders from understood contents.** Deliver the two pilot path moves now as specified
   above; remaining units still choose their file names after their contents are understood.
   Use content-derived C89-compatible names, one original object per C
   file, separate move-only tool batches/commits, original-range comments, object map and disc-name
   README maps; retain disc identity in yaml/artifact roots. Exit: 437 original objects accounted
   for, no address source basename or unresolved folder claim except an explicit accepted exception,
   no stale config/source path, 27/27 and 6,952 after each accepted change.
7. **Remove umbrellas and finish layer audit.** With no source/tool references requiring them, delete
   `game.h`/`main_api.h`; update Standards 8a and its checklist. Run the path-aware residue survey,
   check aliases/headers/prototypes/migration/renames and refresh living docs (`source-files`,
   `game-state`, `data-types`, spec/index/status/playbook as relevant). Exit: zero unexplained game
   placeholders, listed hardware/SDK residue, no essential unnamed rule local/constant, exact final
   clean-build proof, independent Standards and Spec reviews resolved, ticket/board/log consistent.

Gate 5 can proceed on understood owners while gates 3-4 continue, using one serialized source integrator;
the filename-priority pilot explicitly advances the two established path moves. Do not run independent builds or applications in this shared
checkout. These are remaining work gates, not a claim that every historical 11,013-row proposal should
land. Reject wrong, duplicate, speculative or out-of-scope rows; record their disposition.

## Audit limits and handoff

This is an evidence inventory and implementation sequence, not a second decompilation or semantic
review of 6,952 bodies. Representative bodies and the explicit scope limits establish that the exit
criteria are unmet. A complete per-definition residual classification remains gate 1; neither a regex
census nor the prose above can replace it. This lane does not claim a new runtime observation.

The integration owner registers this page and [[spec/header-split-plan]], updates shared ticket/board
and log, and incorporates current application outcomes. Old ticket/log names remain historical.

Independent Sol 6.1 content review completed on 2026-10-24 with no unresolved findings. The reviewer
reproduced all 28 base-table totals and 780 ledger rows in read-only Docker, confirmed that the unit
manifest covers all 34 main objects/all 26 overlays, and checked the cited body/tool evidence. The
mode-proposal paragraph was updated during review to distinguish the initial style-only candidate
from the coordinator's revised semantic proposal. The header-plan review corrected TaiikuBig's owner
from overlay-private to shared, based on its main/DATE/ETC/TAIIKU users. This is review of the audit/design
artifacts; implementation batches and final acceptance still require their own review and proof.

The filename/header-priority extension was independently re-reviewed by Sol 6.1 with no unresolved
findings. Read-only Docker checks reproduced all 34 object intervals and the two-row/six-path rename
plan; the exact first-header manifest has 63 existing declarations, all 75 definitions accounted for,
34 intentional shared-prototype omissions and six unchanged GYOZI `()` compatibility entries.
The pilot can deliver logical C filenames before the broader shared-type migration without changing
any original object boundary or implicit/prototyped call view.

## Root integration update after this snapshot

2026-10-24: the snapshot above remains historical. T-27044/T-27054/T-27057/T-27058 are now Done with independent reviews resolved: scheduler.c/school_day.c, canonical topic-header tooling, cached closure proof lookup, and63declarations in three topic leaves are integrated. All437declaration views remain exact, clean27/27 and6952functions. T-27059 has applied the other32main filename rows; main now has34logical basenames and exact path-mapped437views, with fresh clean matching proof running. T-27060 has403independently reviewed overlay file mappings in26tables committed at c3053b47c; source application is pending the main clean build and live aggregate plan. src/main/README.md now holds the reviewed living object/prefix map. Main/overlay semantic completion and wider header/type migration remain open, as do final distinct Astra review/fix gates.
