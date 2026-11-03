---
id: T-39544
title: Preserve stage dialogue control prefixes
status: In Review
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39511-english-text-quality]]"]
---

## Goal

Preserve the nonprinting command byte before each prefixed BUNKA_SD dialogue row. The renderer skips that byte when drawing and reads it to advance the speech. The translated physical owner currently contains only visible English, so the first letter is skipped and terminal commands disappear.

## Acceptance criteria

- [x] Restore all 214 source-proven prefixes without changing approved prose.
- [x] Reject missing or changed prefixes in the actual-disc audit, with regression coverage.
- [x] Verify stage progression and speaker spacing in the emulator.
- [ ] Build, review and publish the corrected disc.

## Evidence

The source metadata identifies 214 prefixed views and their physical owners, including the protected byte and skip count. On the preceding release, the first Shiori table row points to `Three years` instead of a space followed by that text; the third row lacks its `g` command. The old derived audit checked pointer-to-payload equality but did not verify the source-proven prefix. A controlled stage fixture stalled after entering this renderer. The audit must check this invariant independently of the payload writer.

## Verification and standards review

The final candidate passes both actual-disc audits, all 27 default-binary hashes and all 15 browser checks. Of 70 focused tests, 69 pass and the opt-in full-build test is skipped; the production build was run separately. The fresh boot loads the existing save through run-local card copies. The stage fixtures verify the long player name and Kisaragi's name at the actual sampled coordinates. The player speech completes and returns to the festival. The initialized mascot menu shows all fourteen choices in two columns without covering the notebook divider or preview.

The command comparison proves that all 214 prefixed rows retain the same visible English. The independent audit rejects a missing or altered command even when writer output and pointer agree. The 39 actual-disc telephone, club and mascot labels pass their column bounds. Evidence is in `translations/en/reviews/stage-controls-and-menu-spacing-2026-11-02.json`.

Reviewed the C89 guards, original-build equivalence, field bounds, menu selection geometry, source-prefix provenance and negative audit tests against CODING_STANDARDS.md. No unresolved findings remain. Publication is pending.
