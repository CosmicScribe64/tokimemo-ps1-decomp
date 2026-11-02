---
id: T-39526
title: Publish the verified review patcher and update repository information
status: Done
assignee: codex
created: 2026-11-02
updated: 2026-11-02
links: ["[[tickets/T-39512-browser-patcher]]", "[[tickets/T-39500-english-translation-patch]]"]
---

## Goal

The owner requested pushing available work, updating the README and GitHub description, and linking the patching page. Publish the verified phone-fix candidate as a review build while broader quality work continues.

## Acceptance criteria

- [x] Push verified source changes without including the pending boundary fix.
- [x] Publish the static patcher package without game discs or extracted originals.
- [x] Verify the public page, release manifest and patch hash.
- [x] Update the README and GitHub description with the live patcher link and accurate review status.
- [x] Apply Deslop and Humanizer to public prose and review the staged changes.

## Notes

Candidate `2f9fd663374c4e7aa55c4f46e3690be28624f806` passes both disc audits, browser checks and targeted visual review. Its full route completed through date and save/load. The later `ca7ff1e93b03` boundary candidate remains under visual review and is excluded from this publication.

## Verification

Pushed verified source and README updates to `main` and `patch/english` at `a916ad549`. The pending boundary code remains uncommitted. Published the static package on `gh-pages` at `96a1a438a025e7356530dcdd762d1a8d1a396156`.

The page is live at https://cosmicscribe64.github.io/tokimemo-ps1-decomp/ with HTTPS enforced. GitHub's Pages deployment succeeded. Downloaded all 14 served files and compared exact bytes with the tested package, including the patch and release manifest. A browser smoke check loaded the live page without script errors and showed the expected review-build identifier.

The repackaged page passes the browser acceptance suite. Inspected desktop and 390-pixel layouts. The README explains the current two-track input and review status. The GitHub description and website field both link the public patcher; read-back confirms both settings.

Deslop and Humanizer review covered the exact staged README and page copy, source comments, ticket prose and metadata assessments. Removed stale pending status from the phone-fix ticket. Protected historical log entries and license text were retained. Standards review checked the package contents, source/target hashes, local-only processing and exclusion of game discs. No unresolved publication issue remains. T-39527 tracks the owner's subsequent drag-and-drop and format-support request.

2026-11-02 follow-up: reopened for the owner's request to make the README title and opening describe the full repository, including the decompilation, English patch and browser patcher.

README follow-up complete: the title and opening now identify the matching C decompilation, English review patch, browser patcher, translation assets, extraction tools and Docker build tools. The opening links directly to the live patcher. Checked against the repository contents and coding standards; Deslop/Humanizer review removed a repeated completion statement. No code behavior changed.

2026-11-02 follow-up: the owner wants the introduction to emphasize translated artwork and the aim of remaining faithful to the original game.

The opening and English-patch section now describe text and artwork, including signs and interface graphics. Fidelity to meaning, character voices and visual style is stated as the project's aim, with review status retained. Reviewed the wording against the implemented asset work and coding standards; no code or game text changed.
