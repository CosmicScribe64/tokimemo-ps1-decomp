---
id: T-10830
title: Big functions 3 (astra)
status: Done
assignee: Codex a16
created: 2026-10-12
updated: 2026-10-13
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal

Maximize byte-matching C among the assigned 37 large functions (65,536 bytes)
in six owned objects, biggest first. Spend at least four hours, use regdiff
for every register miss, and preserve reusable types and idioms for subsequent
batches. Time-box unresolved functions and keep broad drafts outside source.

## Acceptance criteria

- [x] Verified matches and near misses recorded; guarded drafts below about 30 differing lines.
- [x] Recovered layouts in overlay headers and idioms in the workflow wiki.
- [x] Final 27-unit build, prototype/global guards, and inline standards review pass.
- [x] Changes remain uncommitted and confined to the authorized files.

## Notes

Owned objects: BUNKASAI/8014ABC0, DATE/801461B0, EVENT/8011C650,
BUNKASAI/80132480, BUNKASAI/8013FD90, BUNKASAI/80143770.
Scratch: scratchpad/a16/. Shared brief: scratchpad/wave11-brief.md.
Session started 2026-10-13 06:30 UTC (2026-10-12 local).

## Comments

- Read the shared brief, standards, local instructions, workflow and prior large-handler findings. No baseline worktree edits.

- First twelve BUNKASAI conversions pass the whole overlay SHA-1 after adding
  four missing void helper prototypes. Standalone regdiff also matches each.
  The baseline and post-prototype full builds both pass 27/27 checksums.
- The largest two BUNKASAI handlers now match in scratch using the existing
  BunkaReturnFlags layout: set b1, assign unk_2_4, then save the three scene
  bytes. Applying only part of the flag view changes later temporary numbering.

- All 29 assigned BUNKASAI bodies are now C. The final source idiom for
  801417DC is assignment followed by passing the stored GameState member,
  which removes the seven scheduling swaps without a FAKE. Real verification
  is recorded in scratchpad/a16/verified29.log and build-bunka29.log.
- DATE 80147958 remains a research draft: changing color widths, splitting
  color locals, explicit counter temporaries, pointer/scalar state views,
  and return shapes did not close the residual. The new typed control and
  header views preserve its prior emitted instructions. An exploratory
  scratch permuter search is running; candidates are not accepted merely
  for improving the permuter's score.

- 2026-10-13: final BUNKASAI candidate passes resolved real funcdiff, and
  build-bunka29.log has all 27 checksum successes. The movie first-pass
  comparisons are in movie-baselines.log. The two inherited bounce guards
  still have 13 real differing lines (11 with branch offsets normalized),
  confined to own-data initialization; their data-island/YAML changes are
  outside this batch's authorized files.

- Movie refinement: DATE 80147958 is now 85 normalized / 171 real differing
  lines in exploratory candidate output-3790-1. EVENT 8011DDB8 reaches 158
  normalized lines with a word counter and RECT after the scalar locals.
  Both remain scratch-only, well above the guarded-draft threshold.

- Further review rejected both EVENT permuter candidates that rewrote a
  state transition incorrectly. The 97-normalized-line manual seed reduces
  frame/register differences, but still lacks the target's case-40 phase
  restore. Its score is research guidance, not an accepted implementation.
  The selector word-load correction and saved-phase behavior are documented
  in the workflow. DATE/Event scrolling searches remain bounded scratch work.

- Completed-header validation: full Docker build passes 27/27 checksums,
  headers/global guards pass, and the branch prototype check reports zero
  new disagreements (17 known). Inline review refined the func_80083440
  override to K&R style, backed by the callee's narrowing loads; 80145334
  still matches and BUNKASAI's post-review checksum passes. The subsequent
  prototype check also passes.
- Eight curated, compile-checked research seeds are in scratchpad/a16/handoff/.
  They label unexplained frame padding as FAKE and explicitly identify the
  void-return declaration blockers and non-integratable ownership probes.
  EVENT 8011C750's missing empty case 10 is fixed in its research seed. The
  handoff uses corrected seeds rather than invalid low-score experiments.

## Final result and validation

- 29 assigned BUNKASAI functions matched, adding 36,716 official progress
  bytes. Progress: `grand total 5215/6958 1003464/2279368 44.0%`.
- Worked from 2026-10-13 06:30 UTC through after 10:30 UTC. All bounded
  searches completed; the final EVENT run ended at 10:29:19 UTC. No later
  candidate met the source threshold. Eight primary research seeds and one
  clearly labelled alias experiment remain in scratchpad/a16/handoff/.
- The completed-header full build passes 27/27 checksums. The K&R review
  refinement passes resolved funcdiff and the BUNKASAI checksum. Closing
  checks report zero new prototype disagreements (17 known), globals OK,
  headers OK, and `ninja: no work to do.` Evidence: scratchpad/a16/
  final-build.log, review-45334.log, post-review-build.log, closing-*.log.
- The two inherited T-10040 bounce guards remain unchanged at 13 real diff
  lines each. Six broader drafts remain INCLUDE_ASM. Zero T-0018 rows added;
  unresolved cases still have structural/type or data-ownership blockers.

## Inline code review

Reviewed the accepted source/header diff against CODING_STANDARDS.md and the
batch brief: C89 declarations, SDK types, function order, field access widths,
header closure, FAKE marking, isolation, and raw-data rules. All 29 accepted
bodies have resolved byte-match evidence and introduce no new FAKEs. Header
changes are append-only; no tools, YAML, main_api.h, raw sources, or unrelated
files changed. The one declaration clarification was resolved by the verified
K&R view of func_80083440. No unresolved findings. `git diff --check` passes;
all changes remain uncommitted for the orchestrator.

Reusable layouts and idioms are in the overlay headers and
[[decompile-workflow]]. Unsafe exploratory assignment splitting and statement
reordering were documented and disabled in the final searches; none of their
incorrect candidates was accepted.
