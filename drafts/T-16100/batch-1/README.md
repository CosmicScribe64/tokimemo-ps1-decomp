# T-16100 batch 1

Bounded evidence-based review of all 55 scopes assigned in `../batch-1.json`.
All 198 input flags have an outcome in `review.jsonl`. There are 42 changed source
alternatives and 13 preserved bodies; all 55 archived full translation units
compile with project IDO 5.3 in Docker and have nonzero diagnostic scores.
No production files were changed and no matching bytes are claimed.

`catalog.jsonl` contains the source scope, archive path, status, score and hashes.
Each scope directory preserves body, declarations, complete context, frozen
preprocessed full context, compiler diagnostics, target comparison, provenance,
and an individual review. `assembly-evidence.txt` contains selected textual
address/mnemonic evidence, without machine words or file offsets. Disposable
compiler intermediates are removed after each serial compilation.

The helper is `tools/t16100_batch1.py`. Run it through `tools/docker.sh`.
It preserves all original alternatives. It replaces only the target definition
in the selected archived context; existing preprocessed archives are used where
present, otherwise preprocessing uses `drafts/T-14500/baseline/include` from
48362009. Compilation and scoring use `order_search.compile_variant` and
funcdiff's resolved-function comparison. Scores are diagnostics, not linked
literal-byte proof, and semantic repairs are retained even if their score worsens.

Repairs cover GameState byte-offset scaling, copied dispatch tables previously
accessed through an invented global stack pointer, exact copy buffer extents,
projected-coordinate arrays and transform triplets, halfword tables, and
initialization aliases. Some fixes intentionally retain byte views where no
complete field mapping is established. Synthetic copy-prefix types, overlapping
stack grids, detached word aliases and wider generated pointer loops remain
explicit findings. A partially repaired flag is not a claim that the whole
function or original source layout has been recovered.

Final QA: 55 scope identities and 198 flag identities covered; 55 scored and
compile-verified archives, zero compiler failures, no work directories, and no
raw opcode words in the textual assembly evidence. These are source alternatives
for later review, not accepted production changes.
