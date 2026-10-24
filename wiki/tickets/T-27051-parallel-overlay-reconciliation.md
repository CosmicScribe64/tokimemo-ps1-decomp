---
id: T-27051
title: Parallel overlay reconciliation
status: In Progress
assignee: parallel-chat
created: 2026-10-24
updated: 2026-10-24
links: ["[[phase2]]", "[[tickets/T-27000-phase-2]]"]
---

## Goal

o-date, o-nameent and o-geko executable-table preparation and source-backed port-spec corrections; no source application.

## Acceptance criteria

- [x] Source-backed outputs with exact coverage and open work; no false completion claims.
- [x] Reconciled names respect applied ledgers and ADR ownership, Docker dry runs where relevant.
- [x] Independent review of prepared executable batches before application.
- [x] No conflicting source/shared-file edits; primary chat applies/commits and maintains shared wiki.

### Supplementary semantic continuation

- [ ] Full function/declaration and essential-literal census reconciled for all three units.
- [ ] New owned identifier packets and exact literal-site inventories prepared; prior apply rows unchanged.
- [ ] Exact supplementary packets pass independent Standards and Spec reviews and required dry plans.
- [ ] Primary applies/proves the supplements and resolves the recorded shared-owner/tooling residue.

## Comments

2026-10-24: User explicitly authorized fan-out across multiple chats. Shared-checkout worker edits are limited to assigned tables/specs and this ticket; root serializes applications, builds, indexes, board and log.

2026-10-24 (reconciliation underway): Split DATE, NAME_ENT and GEKO into three disjoint analysis lanes.
Docker source census found 668, 257 and 391 C function definitions respectively, with every owned
`func_` definition represented in its original proposal. These are proposal coverage counts, not
application or completeness claims. Each lane is preparing a separate `*-apply.tsv` containing only
owned `func`/`data` rows; types, fields, constants, moves and main-owned requests remain separate.

Independent source checks identified corrections being resolved before handoff: DATE's end-of-date
last-spot writes require a positive spot and use a different girl operand in their looks test from
the later bonus block (`src/ovl/DATE/80132000.c:3679`); its scoring helper writes complete 16.16 words,
contrary to the old spec's integer-only wording (`src/ovl/DATE/80132000.c:3415`). GEKO's invitation
override day limit applies only to the last month in several disjunctions, rather than all months
listed (`src/ovl/GEKO/80142760.c:542`). No source application or build has run in this lane.

Shared-wiki handoff requirement: the integration owner records these findings and final validation
in [[log]], updates shared indexes/board/status and owns any status transition. This lane keeps
its ticket In Progress while preparing the reviewable outputs.

2026-10-24 (NAME_ENT review complete): `config/renames/proposals/o-nameent-apply.tsv` has 478 rows
(255 function, 223 data), all NAME_ENT-owned, five columns, with all function placeholders covered.
Full Docker plan (`tokimemo-decomp-arm64 --cpus=2`, `--ledger o-nameent`) exited 0 with 9 files to
change and zero problems. Independent Sol 6.1 Standards/Spec review passed with zero unresolved
findings after fixing handler suffixes, glossary traps/abbreviations, unsupported story names,
character/byte units, iris threshold overshoot, exact line-player timer branches, three declaration
locators and the opening helper's unused meter average. The source/header census and all explicit
data deferrals (80 island-only, one comment-only endpoint, 176 generated-assembly-only labels)
are recorded in [[spec/o-nameent]], alongside separate type/field/constant/move/main-owner work.
Every function locator was independently checked against its actual definition in Docker.
No source application or clean build is claimed; root must rerun the live plan before application.

2026-10-24 (user layout/constants steering): The primary chat requested a second, explicit section
covering logical C filenames/topic headers and evidence-backed constants throughout each unit.
[[spec/o-nameent]] section 14 records the four original-object file destinations, five proposed
declaration topics, fixed-width/closure constraints and 17 grouped constant families with source
contexts. Sol 6.1 reviewed these recommendations with no blocking findings. The optional supported
move-only `config/renames/proposals/o-nameent-files.tsv` has six rows; its full Docker plan exited 0
(11 files to change, zero problems). One advisory is preserved: `tools/test_redraft.py` refers to the
old overlay header path. No moves or constant replacements were applied. Each source file move
still requires its own commit and the header split its prototype-view proof.

2026-10-24 (GEKO review complete): `config/renames/proposals/o-geko-apply.tsv` has 1,061 rows
(391 function, 670 data), all GEKO-owned and marked HYPOTHESIS to preserve ADR confirmation status.
Independent Sol 6.1 Standards/Spec review passed with zero unresolved findings in its reviewed scope.
It independently checked all 176 explicit argument-role claims against actual calls; source/assembly
census agrees at 676/756 data labels, leaving six source and 80 assembly-only explicit deferrals.
Corrections include Yuina selector 7, visit-script load rather than group reset, invitation eligibility
using computed Sunday while sequential overrides use today's date, exact month/day precedence, all
matching availability writes, packed appointment bits and explicit birthday masks. Exact score changes
and ranking/tie rules were also checked. Full Docker plan passed against a frozen source snapshot
(1,061 rows, 26 files, zero problems); [[spec/o-geko]] records the command and snapshot hash. Live
planning is still required immediately before integration application because the shared core was
changing during the analysis. No build or emulator confirmation is claimed.

GEKO's second pass now has 22 exact logical C filenames, 21 proposed topic headers plus the umbrella,
and 20 proposed constant families alongside reused core enums. Sol reviewed those recommendations;
the counter suggestion now says clamp value (Miharu clamps to 4 and then increments to 5). File/header
and constant application remains separate from the symbol batch, with closure and match proof gates.

2026-10-24 (DATE final review and direct plan complete): The final executable table has 850 rows:
654 placeholder functions, three existing-name corrections, one internal alias (`func_80132004`),
and 192 data placeholders. Eleven already named script-fill functions remain unchanged. Independent
Sol 6.1 review resolved Standards and Spec findings, checked all 657 actual function definition
locators plus the alias evidence, read the complete scene-trigger chain/12 dispatch maps, compared
45 setup reward bodies with the spec, and sampled the remaining callbacks/data. Concrete corrections
include karaoke modulo-three table selection, exact planetarium/Yumi overrides, boss-first ambush
priority, ordinary-end positive-spot versus scene-end nonnegative-spot guards, fixed-point writes,
exact birthday masks, hint calendar/roll precedence and 237 scene-section pointer assignments.

The producer's final log was overwritten by a report-writing SyntaxError; its older successful log
still held two superseded target names. The parent reran the final table directly in Docker
(`tokimemo-decomp-arm64 --cpus=2`, `--ledger o-date`), exit 0, 850 rows, 25 files to change, zero
problems. Authoritative scratch evidence: `.claude/orchestrator/state/T-27051-review/date-final-plan.log`.
The reviewer independently compared every printed target tuple to the current TSV with no mismatch.

The exact DATE residue is 969 source/header data labels plus 107 assembly-only labels; all 1,076
deferred data labels are individually listed with no omissions, extras or duplicates. Four function-like
items are separately deferred (one stale alias prototype and three GTE helpers). The 402 labels absent
from the first residue draft were recovered by the independent census. [[spec/o-date]] also records
19 C object destinations plus an umbrella, 17 topic headers and all 158 proposed constants. The mixed
Yumi/Miharu object stays intact as `scene_yumi_miharu.c`; event-log-only IDs 37..40 are not date spots.
Sol reviewed these layout/constants recommendations without claiming move/constant application proof.

Preparation handoff: 2,389 executable symbol rows across the three units (1,304 function rows and
1,085 data rows), all owned, five columns, no duplicate old keys or case-insensitive targets within
each unit. Three independent review reports and exact plan evidence are in the authorized ignored
scratch directory. Original mixed proposals were preserved. This lane changed only its assigned
proposal/spec files, this ticket and ignored scratch. No source/header/symbol/ledger/tool/shared-wiki
application, build, raw-data change, git mutation or outbound chat message was performed.

The preparation criteria are checked; the ticket status remains In Progress until the primary chat
serializes its board/status transition. The primary chat owns applications, builds, commits, shared
wiki indexes/status/log, and final phase-2 completion. Unit residue, types/fields, essential locals,
constants, file/header moves and runtime confirmation remain explicit further work; this is not a
claim that these units or phase 2 are finished.

2026-10-24: Parallel chat 01a16f17-10c2-7373-b02f-dd924f8e7108 dispatched. No source application or shared wiki mutation permitted; primary chat integrates reviewed outputs.

2026-10-24 (semantic continuation): Primary requested new supplementary essential parameter/local
tables and exact proven constant sites for DATE, NAME_ENT and GEKO, plus a full-scope reconciliation
of remaining types, fields and gameplay definitions. The 2,389 frozen apply rows remain unchanged.
Three disjoint lanes prepare new `o-<unit>-essential.tsv` identifier tables and separate
`o-<unit>-essential-constants.tsv` literal-site inventories. Every function's parameter/local residue
must be classified; an old draft or filename census is not semantic-completeness proof. Source,
header, tool, ledger, shared-wiki changes, application, builds and commits remain primary-owned.
An independent Sol review will check the exact supplementary packets before their freeze.

Tool constraint found during continuation: `tools/rename.py:1922-2120` accepts member, bare-variable
and call-argument constant sites, with replacement scope determined by a header's user closure.
It has no function-local or line filter and does not name arbitrary arithmetic operands. Its
call-site parser accepts a unit but the visitor compares only the callee name. Exact semantic
sites that cannot be selected safely by those mechanisms must be recorded as tooling/application
residue rather than presented as executable constant coverage. No tool change is made in this lane.

Continuation source findings: main `func_80043448` in `src/main/pad_input.c:288-306` proves
`D_800B3CFC` is set by an 18-entry input sequence with byte index/timer control; DATE's former
untraced-writer question is resolved at that technical level. ETC's `func_80144F74` in
`src/ovl/ETC/80143240.c:738-750` increments both `GameState.girl_cleared` and `SystemData.flags`
under 99, while main memory-card code copies the 16-byte system counter array. NAME_ENT displays
those counts through the already-applied `SystemData`/`AlbumSlotRecord` types. These findings are
filed in [[spec/o-date]] and [[spec/o-nameent]] as shared-owner reconciliation; no main field or
symbol row is applied or added to the owned identifier packets.

Follow-up serialization evidence corrects the original NAME_ENT proposal: `AlbumSlotRecord.flags`
is the reader view of `GameSaveImage.unk_488`, sourced from `GameState.girl_cleared` by main
memory-card save/load loops (`src/main/memory_card.c:834,950`). The album offset is 0x200 + 0x488 =
0x688. It is not a copy of the persistent `SystemData.flags` array. The separate 14-byte reader
and 16-byte serialized declarations remain unchanged; [[spec/o-nameent]] records the exact
shared-owner requests and supersedes the old proposal's provenance claim.

GEKO continuation resolved the birthday candidate sentinel at source level: `func_80134808`
calls `get_g_zyotai_s(255)` before skipping the sentinel, but the two main score helpers reduce
the argument modulo 13, so this call reads Yuko's index 8 rather than going out of bounds. The
later sentinel guard still skips the candidate; no fallback choice is inferred. [[spec/o-geko]]
records the shared-helper trace and removes this issue from the unproved bounds questions.

NAME_ENT source inspection identified a possible negative-literal constant bug: `literal_value`
returns the signed value but the inner positive Constant node (`tools/rename.py:1934-1944`),
while the edit replaces only that node's token. Mapping -1 to an enum member valued -1 may leave
the original unary minus and invert the result. Negative-valued families are held as tooling
residue pending a synthetic fixture; their semantic sites remain inventoried. No tool patch or
constant application is made in this lane.

Synthetic proof now confirms the negative-literal defect. Actual `const_plan` on an isolated
fixture changes `x = -1` to `x = -NEGATIVE_TEST_NONE` and the comparison likewise, while declaring
the enum member as -1; the result is +1 and the tool reports zero problems. Proof is
`.claude/orchestrator/state/T-27051-essential/nameent/negative-literal-proof.json` and its generated
source. Negative-valued constant families must remain tooling-blocked until the primary repairs
the unary-node replacement and proves it. This lane changes no tool or game source.

Independent interim reviews found two Standards naming issues (American `Program` spelling and
new unapproved syoushin stem derivatives) and two Spec preparation gaps (DATE comment text selected
as literal operands; GEKO/DATE known gameplay sites left without semantic symbols). All findings
are accepted for correction. The constant inventories are not frozen handoffs while these gaps
remain. Retained-declaration rechecks also recovered 15 DATE and 17 NAME_ENT essential roles;
their separate full plans pass without changing the earlier packet membership. The corrected
DATE base targets require a fresh exact plan; unchanged packets reuse their matching proofs.

2026-10-24 root applied-symbol milestone: main-ui247rows andscene-symbols2389rows (2636total) nowlive. Bothfullplan/apply/ledgerchecks pass3922totalledgerrows. IndependentStandards/Spec actual-source reviews confirm scopedidentifier/guard-only changes, originalbodies/literals/types/widths/K&Rviews preserved, all437ABIviews exactunder scopednamemapping. CleanDocker27/27and6952functions/2276920bytes, independentrehash27/27. NoO.BINnameschanged. Fullunitessential/type/literal/headerworkremains separatelytracked; thissource milestone is notPhase2complete.
