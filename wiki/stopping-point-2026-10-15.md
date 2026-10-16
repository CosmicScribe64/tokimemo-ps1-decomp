---
type: handoff
updated: 2026-10-15
---

# Agent handoff — 2026-10-15

The user requested all work on main and a clean stopping point, then authorized one GPT-6.1 Sol medium pass over the 165 flagged compiling drafts. That pass is complete. All workers are stopped; recurring integration monitoring remains paused. Do not start background work until requested. The 80% matching goal is unfinished.

## Current verified state

- **1,452,192 / 2, 279,368 game-code bytes matched: 63.7%.**
- 5,945 / 6,958 game functions matched; SDK library assembly excluded.
- 80% requires 1,823,496 bytes: **371,304 more verified bytes**.
- All **1,066 / 1,066 scopes** in the frozen unmatched-function census have a compiling full-translation-unit C alternative. This is a baseline census, not the current unmatched-function count.
- Current unchanged quality audit: **950 audit-clear, 116 review-needed**. Audit-clear is not proof of correctness or byte matching.
- The latest pass reviewed and compiled all 165 previously flagged scopes: **93 repaired bodies, 72 preserved, 49 newly audit-clear scopes**. No production source changes, new zero-score candidates, or verified matching bytes resulted from this pass.

## Where the work lives

Start with [[flagged-draft-pass]], [[exhaustive-drafts]], and drafts/README.md.

- drafts/T-14500/inventory.json and catalog.jsonl preserve the original 1,066-scope census at baseline 48362009. Initial outcomes are immutable: 29 zero candidates, 908 other scores, 129 initial failures. All 129 failures now have compiling alternatives.
- drafts/T-14500/best-quality-catalog.jsonl and quality-audit.jsonl identify currently selected contexts, remaining flags and all alternatives. best-catalog.jsonl favors diagnostic score; it can select older source with known reconstruction issues. Read newer review notes before using it.
- drafts/T-16100/review.jsonl records all 165 latest reviews. Its manifest.json freezes the inputs; summary.json records combined results. Batch1 changed 42/55, batch2 changed 16/55, batch3 changed 35/55. Batch folders retain bodies, declarations, complete/preprocessed contexts, diagnostics, scores, hashes and provenance.
- Earlier repair archives: T15700 TACO/TAIIKU 28, T15800 overlays 23, T15900 mixed 13 within T14500/repairs/t15900, and T16000 MAIN 13. These resolved the last 77 compiler failures.
- Large EVENT and other unfinished probes remain in T15200–T15600. Older uncommitted worktree source and research were preserved under drafts/preserved-worktrees-2026-10-15. Original checkouts remain intact; no game data or disposable compiler outputs are committed.

The 31 diagnostic zeros in the joined census do not indicate 31 new matches waiting to promote. The original 29 and ETC repair zero were already verified/promoted. EVENT17D80 is an additional archive using already matched production C.

## Remaining work and useful next steps

The 116 selected flagged scopes contain 115 synthesized-stack-local, 72 inferred-copy-layout, 27 GameState-address-arithmetic and 37 stack-address-arithmetic findings. Several flags can belong to one function. Per-scope notes also cover unresolved alias relationships, synthetic stack reads and other pointer strides that pattern matching cannot fully characterize.

Use assembly evidence to reconstruct real arrays, fields and callback tables. Do not rename locals to evade flags, weaken the audit, or treat compilation as correctness. Some source repairs worsen diagnostic scores because an earlier lower-score draft was semantically suspect. Preserve both the evidence and original alternatives.

For matching progress, prioritize useful byte gains and transferable family fixes together with diagnostic proximity. The largest EVENT dispatcher remains nonmatching (T15200 score 1930); its related handler is 26 rows / 13 register-only positions. T16100 EVENT800FF61C reports 8 rows, still unverified and nonmatching. Avoid repeating exhausted register-order probes without new evidence.

## Environment and validation

Integration checkout: `/Users/lila/Documents/Projects/tokimemo-wt/integration`, branch `codex/matching-integration`; published to origin/main. Original `/Users/lila/Documents/Projects/tokimemo-decomp` is intentionally not reset or overwritten; it retains its local state. Fetch main into a suitable checkout before resuming.

All Python/toolchains/dependencies/builds run through existing Docker and tools/docker.sh. Do not install host dependencies or copy disc images. Reuse immutable inputs; never hardlink generated writable assembly. Approximately 18 GiB disk space remained during this pass. Requested worker model is GPT-6.1 Sol, medium effort.

Metadata-only validation/reproduction:

```sh
tools/docker.sh python3 tools/t16100_join.py
tools/docker.sh python3 tools/refresh_draft_ledgers.py
tools/docker.sh python3 -m unittest discover -s tools -p 'test_*draft*.py'
```

The join verifies all 165 scope records and recorded archive hashes; refresh preserves the 1066-scope compiler census and updates quality selection. The three tools/t16100_batch*.py helpers show each reconstruction/compiler procedure. Use recorded frozen headers/contexts; create new alternatives for new experiments instead of overwriting archives.

Final pass validation: all 165 full-TU compilations succeeded, all 13 draft-tool tests passed, all joined archive hashes validated, and production C/headers stayed unchanged. Prior build/prototype validation passed (no pending build work; zero new prototype disagreements, 15 known). Prior matching promotions have default linked proofs and affected-unit checksum checks; latest shared-header validation passed all 27 binary checksums.

Only promote a draft after explicit default linked literal proof and appropriate checksum/header/global/prototype checks. A draft score of zero alone is insufficient. Push verified changes and useful drafts to main without force; user authorized direct integration and does not want PR review bureaucracy. Tickets awaiting semantic/standards review remain In Review rather than claiming Done.
