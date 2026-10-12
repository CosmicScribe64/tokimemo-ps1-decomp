---
id: T-10630
title: Big functions (astra)
status: Done
assignee: Codex a14
created: 2026-10-12
updated: 2026-10-12
links: ["[[decompile-workflow]]"]
---

## Goal

Match the assigned 31 large functions in six owned objects, largest first,
with roughly one hour per difficult function and at least four hours of work.
Recover shared overlay types and document reusable source idioms.

## Acceptance criteria

- [x] Matches verified with resolved funcdiff and unit checksums.
- [x] Register misses investigated with regdiff; retained near misses guarded.
- [x] Types and idioms recorded in overlay headers and workflow.
- [x] Final build, prototype/global checks and inline standards review pass.

## Notes

Started 2026-10-12 19:46 UTC. Scope: DATE/801461B0, EVENT/8011C650,
BUNKAKEN/8014A6A0, MASTER/80133680, TEL/80132000, BUNKASAI/8013C430.
Follow wave11-brief.md isolation; leave uncommitted edits for orchestrator.

## Comments

- TEL `func_80132FC4`: MATCH (resolved), 0xD90 / 3,472 bytes; TEL SHA1 OK.
  `*(u8 *)&state` switch removes a redundant selector copy and fixes the
  initializer delay slot. Existing buffer-extent FAKE retained; new byte-view
  FAKE documented. Source and pointer-view sweeps saved under scratchpad/a14.
- TEL `func_80136724`: recovered four-byte entry and 21-column table; source
  forms and width variants under active investigation. Own-data scratch
  definitions did not improve the draft.
- DATE `func_80147958`: recovered Rec24/RECT loops; remaining counter,
  fade truncation and state-spill differences. Narrow-counter loop forms and
  per-candidate regdiff traces retained in scratch; not an accepted near miss.
- MASTER `func_801344E0`: recovered 0x44 sprite records and ten coordinate/
  speed pairs. Current draft has the target instruction count; five-minute
  permuter search had no exact result. Rejected its unsafe uninitialized-
  pointer candidate. Typed draft transferred to `func_801363F4` in scratch.
- BUNKASAI `func_8013DE3C`: structure matches after bit-field and message-ID
  cleanup; investigating competing scene-selector/message-ID register ranges.
- BUNKAKEN: six resolved matches (`8014BD80`, `8014B7B8`, `8014CB4C`,
  `8014A9A8`, `8014D0FC`, `8014DB34`), pending overlay SHA1 verification.
  Reused Rec38 bit-field for mixed word-read/byte-write flag access and a
  local GameState pointer. `8014D65C` is 26 differing lines; `8014B378`
  is six after restoring direct message assignments, with a permuter run
  in progress. Every candidate has a saved regdiff trace.
- First integration checkpoint: 14 new matches (seven BUNKAKEN, six
  BUNKASAI, one TEL); all 27 SHA1 checks pass. Scoped dupes: zero plans.
  BUNKASAI matches: `8013D1F8`, `8013E59C`, `8013D8E0`, `8013CCA8`,
  `8013C5A4`, `8013F5D4`. BUNKAKEN `8014B378` also matches after moving
  its message assignment ahead of the bit-field write.
- Verified first-checkpoint total: 14 functions / 21,408 bytes, using
  actual text sizes from the assembly headers. Detailed manifest:
  scratchpad/a14/matches.json.
- BUNKAKEN `8014D65C` now matches and passes its unit SHA1. One empty
  pointer test after the case-5 call result fixes the later GameState base
  register. It is safe, eliminated from assembly, and marked FAKE/T-10630.
  All eight assigned BUNKAKEN handlers now match (15 matches overall).
- Second integration checkpoint: all 27 SHA1 checks pass with fifteen new
  matches. Prototype branch check and global migration check pass. The
  EVENT `8011C750` draft also compiles and has regdiff evidence; its UV
  oscillation shares the signed-step/Rec24 layout recovered in DATE.
- MASTER `80133AA0` now matches (1,248 bytes), unit SHA1 OK. Recovered the
  original eleven-copy loop, palette counter reuse, declaration order, and
  packed two-bit increment. Total now 16 functions / 23,896 bytes.
- MASTER `80133F80` now matches (1,376 bytes), unit SHA1 OK. The unsigned
  threshold type, two explicit swap loads, reused local, and indexed fill
  loop reproduce all instructions. Total: 17 functions / 25,272 bytes.
- Scoped follow-on queue: MASTER `801359F0` matches (772 bytes), with
  resolved diff and unit SHA1 passing. Reused `MasterWords11`; marked the
  two-word frame padding FAKE. Total: 18 functions / 26,044 bytes (17 from
  the assigned list plus this same-object neighbor).
- MASTER `80137044` (1,444 bytes) and `801375E8` (1,396 bytes) now match;
  all 27 SHA1 checks pass. The shared sprite view prevents flag-load hoists.
  Added the canonical `D_801217D0` main-memory base to the allowed type-symbol
  list after the linker exposed its absence. Total: 20 / 28,884 bytes, with
  19 assigned functions and one neighboring handler. One T-0018 row added.
- BUNKASAI `8013DE3C` now matches (1,888 bytes); resolved diff and unit
  SHA1 pass. All seven assigned BUNKASAI functions now match. Moving the
  packed flag updates before the saved-state copies fixes the final residual.
  Total: 21 functions / 30,772 bytes (20 assigned plus one neighbor).
- Scoped follow-on matches: BUNKASAI `8013CA64` (580 bytes), BUNKAKEN
  `8014AF48` (1,072) and `8014DFB8` (1,060). All resolved comparisons pass;
  the recovered Rec38 flag view removes shift/test differences in both
  BUNKAKEN handlers. Total: 24 / 33,484 bytes (20 assigned, four neighbors).
- Entered inline review against `CODING_STANDARDS.md` and the shared brief.
  Fresh resolved checks pass for all 24 matches; prototype branch check has
  zero new disagreements, and globals check passes. Wide drafts are scratch-only;
  original DATE/EVENT guarded data-island near misses are unchanged.
- Four more scoped BUNKASAI matches: `8013EBDC` (988), `8013EFB8` (636),
  `8013F234` (928), `8013FA28` (560). All pass resolved comparisons.
  Total: 28 functions / 36,596 bytes (20 assigned, eight neighbors).
  The new handlers reuse Rec38 bit-fields and the local GameState pointer.
- MASTER neighboring `80136E24` matches (544 bytes). Exposed bit 30 in
  `MasterClubBits`; its bit-field assignment fixes the final 26-line residual.
  The bounded permuter did not solve it. Total: 29 / 37,140 bytes.
  Aggregate consistency review also converted earlier MASTER sprite labels
  to field accesses; both resolved comparisons still pass.
- BUNKAKEN `8014A6A0` (776 bytes) and BUNKASAI `8013C430` (372)
  match with the shared two-word dispatch view. Total: 31 / 38,288 bytes.
  Refined the MASTER sprite +0x28 position into a signed 16.16 union; the
  large animation drafts and neighboring helper provide the evidence.
- MASTER `80137BF4` matches (928 bytes), including the recovered fixed-point
  sprite word. Source load order fixes the saved-register bases; the wide X
  local/narrow third argument is marked FAKE. Total: 32 / 39,216 bytes.
  Both owned BUNKAKEN and BUNKASAI objects now contain no assembly fallbacks.
  The preceding 31-match checkpoint passed all 27 SHA1 checks.
- Final regdiff audit confirms the unchanged DATE `801489D0` and EVENT
  `8011EDFC` guards each have 11 normalized differing lines (13 raw diff
  lines, including the shifted branch). No wide draft remains in source.

## Inline review

Reviewed the complete diff against HEAD, `CODING_STANDARDS.md`, and the shared
brief. Resolved findings: completed struct offset/size comments, converted
remaining new MASTER sprite accesses to the shared view, removed wide drafts
from source, marked compiler-shaping choices FAKE with reasons/tickets, and
removed one trailing blank. No unresolved code findings remain.

The C changes stay in the four owned objects that gained matches; DATE and
EVENT source files are unchanged. Header changes are appended declarations
and views, plus the generated EVENT override required by the generated main
API addition. The one type-symbol linker name is appended. No tool, YAML,
raw-data, dependency, or Git-state changes were made. All new functions use
C89-compatible declarations and the established SDK/GameState types. Unsafe
permuter candidates were rejected rather than integrated.

Final validation at the 32-match checkpoint: all resolved comparisons passed
(the last new helper separately), all 27 SHA1 checks passed, header/global
stamps exist, branch prototype check reports zero new disagreements, globals
check passes, and `git diff --check` is clean. Scoped dupes applied zero plans.
The assigned-list audit gives 20 functions / 30,000 bytes; twelve scoped
neighbors add 9,216 bytes, for 32 / 39,216 total. Progress:
`grand total   5162/6958   958768/2279368  42.1%`.

The final bounded MASTER search ended without a valid improvement. The
requested four-hour work window is complete; all review findings are resolved.

## Handoff

The eleven unmatched assigned drafts have compile-checked copies in
`scratchpad/a14/handoff/`, with `scores.tsv`, `README.txt`, and corresponding
`handoff-traces-*` regdiff directories. These use the final headers, including
`MasterFixed32`. The two paired-movie return-type experiments remain separate;
the handoff copies retain the inherited public declarations.

Normalized residuals: DATE 47958=120, 462B0=605, 489D0=11, 4718C=621;
EVENT 1DDB8=599, 1C750=737, 1D5C4=601, 1EDFC=11; MASTER 344E0=350,
363F4=423; TEL 36724=825. Wide drafts are intentionally absent from source.

## Verified match manifest

Sizes count function text, excluding adjacent rodata.

| Unit | Function | Bytes | Scope |
| --- | --- | ---: | --- |
| BUNKAKEN | `func_8014A6A0` | 776 | neighbor |
| BUNKAKEN | `func_8014A9A8` | 1440 | assigned |
| BUNKAKEN | `func_8014AF48` | 1072 | neighbor |
| BUNKAKEN | `func_8014B378` | 1088 | assigned |
| BUNKAKEN | `func_8014B7B8` | 1480 | assigned |
| BUNKAKEN | `func_8014BD80` | 1524 | assigned |
| BUNKAKEN | `func_8014CB4C` | 1456 | assigned |
| BUNKAKEN | `func_8014D0FC` | 1376 | assigned |
| BUNKAKEN | `func_8014D65C` | 1240 | assigned |
| BUNKAKEN | `func_8014DB34` | 1156 | assigned |
| BUNKAKEN | `func_8014DFB8` | 1060 | neighbor |
| BUNKASAI | `func_8013C430` | 372 | neighbor |
| BUNKASAI | `func_8013C5A4` | 1216 | assigned |
| BUNKASAI | `func_8013CA64` | 580 | neighbor |
| BUNKASAI | `func_8013CCA8` | 1360 | assigned |
| BUNKASAI | `func_8013D1F8` | 1768 | assigned |
| BUNKASAI | `func_8013D8E0` | 1372 | assigned |
| BUNKASAI | `func_8013DE3C` | 1888 | assigned |
| BUNKASAI | `func_8013E59C` | 1600 | assigned |
| BUNKASAI | `func_8013EBDC` | 988 | neighbor |
| BUNKASAI | `func_8013EFB8` | 636 | neighbor |
| BUNKASAI | `func_8013F234` | 928 | neighbor |
| BUNKASAI | `func_8013F5D4` | 1100 | assigned |
| BUNKASAI | `func_8013FA28` | 560 | neighbor |
| MASTER | `func_80133AA0` | 1248 | assigned |
| MASTER | `func_80133F80` | 1376 | assigned |
| MASTER | `func_801359F0` | 772 | neighbor |
| MASTER | `func_80136E24` | 544 | neighbor |
| MASTER | `func_80137044` | 1444 | assigned |
| MASTER | `func_801375E8` | 1396 | assigned |
| MASTER | `func_80137BF4` | 928 | neighbor |
| TEL | `func_80132FC4` | 3472 | assigned |

## Completion

Completed 2026-10-12 23:47 UTC after more than four hours. Final result: 32
verified matches / 39,216 bytes (20 assigned / 30,000 bytes and 12 scoped
neighbors / 9,216 bytes). One T-0018 row was added. The top separate blocker
is own-data/YAML isolation; inherited movie return types also need later
review. No unsafe search output was integrated. All changes remain
uncommitted for the orchestrator.
