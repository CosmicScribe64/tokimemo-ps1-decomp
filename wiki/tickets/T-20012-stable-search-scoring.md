---
id: T-20012
title: Keep search ranking consistent across instruction counts
status: Done
assignee: Codex
created: 2026-10-17
updated: 2026-10-17
links: ["[[matching-notes]]", "[[decompile-workflow]]"]
---

## Goal

Fix issue #58: https://github.com/CosmicScribe64/tokimemo-ps1-decomp/issues/58. viewsweep and order_search currently rank with printable diff row counts, which switch comparison methods at equal instruction counts. Use one aligned instruction cost for all lengths while preserving positional diagnostics and exact match proof. No production C changes or new matching bytes claimed.

## Acceptance criteria

- [x] Add synthetic regressions for equal-length insertion/deletion shifts and the extra-instruction ranking inversion.
- [x] Share a stable metric between viewsweep and order_search; keep zero equivalent to identical normalized keys.
- [x] Preserve printable positional evidence, document the score, and run relevant tool tests in Docker.
- [x] Record a real draft measurement and verify all production binary hashes remain unchanged.

## Notes

Start on main 567b5663, branch codex/stable-search-scoring. Levenshtein 0.27.1 is already pinned in tools/Dockerfile. Use insert/delete cost 1 and substitution cost 2 so score units remain differing instruction rows. Independent source research remains on local branches under claims #50/#57; no nonmatching code is promoted here.

## Result and validation

Added funcdiff.diff_score with consistent insert/delete cost 1 and substitution cost 2; viewsweep and order_search use it. Existing Levenshtein 0.27.1 suffices. Printable positional evidence and exact-byte proof are unchanged. viewsweep emits score_metric = aligned-insert-delete-v1 and recomputes missing/older cached metrics on resume. JSON score field names remain compatible.

230 tests pass in Docker across test_funcdiff, test_viewsweep, test_order_search and test_autotweak. Synthetic regressions show the length-boundary inversion, preserve positional scheduling evidence, and verify both scorers and current/legacy cache handling. Real full-unit order_search on the existing scene archive scores 811; compiled scene controls consistently score 811/791/781. viewsweep CLI on the already matched C helper returns matches-as-is at zero with one compile; independent default linked funcdiff confirms its existing 484-byte MATCH. These checks do not add newly verified C bytes.

Default ninja build passes (five steps), headers/globals OK; independent SHA1 checks pass for all 27 binaries; prototype branch guard reports 0 new / 15 known, branch check OK. Production src/include/config and Dockerfile are unchanged. Evidence is ignored under build/T-20012; tests export no game data.

## Review

Self-review against CODING_STANDARDS.md: Python 3 in Docker; pinned dependency reused; shared metric and focused search integration; synthetic tests for behavioral regression; zero still goes through the existing exact proof gate; diagnostic formatting unchanged; cache metric migration documented. No compiler settings, game C, generated assembly, raw assets, binaries, compiled objects or comparison instruction listings submitted. Ticket/kanban In Review; maintainer review pending.

- 2026-10-17 review (orchestrator, CODING_STANDARDS.md): tools only (funcdiff.diff_score, viewsweep and order_search scoring, cache metric versioning), no game C, Levenshtein already pinned in tools/Dockerfile. Merged as PR #59 in one clean build: 27/27 SHA1, headers, globals, prototypes, tool tests (1,514 OK, 3 skipped). Done.
