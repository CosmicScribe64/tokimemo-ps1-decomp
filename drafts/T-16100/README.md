# Sol medium pass over flagged drafts — T16100

At the user’s request, three GPT-6.1 Sol medium workers each reviewed 55 disjoint scopes from the 165 compiling drafts flagged at ba824b48. All 165 alternatives compile as complete translation units with recorded frozen-header contexts, diagnostics and hashes. Original alternatives remain unchanged.

**93 bodies were repaired; 72 were preserved.** The unchanged audit now selects 950 audit-clear and 116 review-needed scopes across the 1066-scope census, compared with 901/165 before this pass. Thus 49 scopes newly clear the listed patterns. Remaining selected flags: 115 synthesized-stack-local, 72 inferred-copy-layout, 27 GameState-address-arithmetic, 37 stack-address-arithmetic. A draft can have multiple flags. Audit-clear is not proof of semantics.

Repairs include byte-based GameState offsets, scalar/array address scaling, observed copied-table extents and element widths, connecting stack initialization to the actual array reads, and copied callback tables. Exact per-scope evidence and remaining limitations are in drafts/T-16100/review.jsonl and the batch archives. Some corrected source produces a worse diagnostic score; source reasoning and matching proximity are separate. Unresolved copied types, alias relationships, synthetic stack reads and pointer strides remain documented. No renaming-only changes or weaker audit patterns were used to reduce the count.

Batch results: batch1 reviewed 55/changed 42; batch2 reviewed 55/changed 16; batch3 reviewed 55/changed 35. The historical initial scope list remains under drafts/T-16100/manifest.json. Three batch catalogs retain full bodies/declarations/contexts/provenance; summary.json records combined counts. The latest general ledger may prefer a lower-score older draft when both still have flags; consult the newer review notes before reusing those bodies.

No new zero-score candidate or production edit was made. Verified game bytes remain 1,452,192/2, 279,368 (63.7%); the 80% goal remains incomplete. Production matching requires independent default linked proof and affected checksum checks.

Reproduce metadata using `tools/docker.sh python3 tools/t16100_join.py`, then `tools/docker.sh python3 tools/refresh_draft_ledgers.py`. The join requires complete scope coverage and verifies recorded source/context hashes. Per-batch tools/t16100_batch1.py, batch2.py and batch3.py preserve the reconstruction procedure and compile evidence. Compiler objects, generated assembly, raw game bytes and disposable work directories are excluded.
