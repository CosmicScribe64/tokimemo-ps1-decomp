# T-16100 batch 3

All 55 reserved scopes were reviewed; all 55 archived full translation units compile and have resolved funcdiff scores. Thirty-five bodies changed. No source/include production files changed and no matching claims are made.

`review.jsonl` records every original flag, its outcome, the actual repairs and remaining issues. Raw copy layouts remain unresolved where only a copy extent is known. Several drafts still contain unrelated pointer-stride errors, synthetic global `sp` reads, or stack aliases; a compiling draft is not production-ready. Scores may worsen when invalid scratch locals are corrected, and originals are preserved at each `selected_path`.

Each scope retains body.c, declarations.h, context.c, full-context.c, diagnostics.txt, result.json, provenance.json, review.json, concise assembly-evidence.txt and a maximum 80-row difference excerpt. Evidence retains instruction addresses and mnemonics, with machine opcode words removed. result.json records hashes of the archived context and other artifacts.

Reproduce serially in the existing image:

    tools/docker.sh python3 tools/t16100_batch3.py
    tools/docker.sh python3 drafts/T-16100/batch-3/finalize.py

The selected prior full source context supplies the translation unit; preprocessing uses `drafts/T-14500/baseline/include`, the frozen 48362009 census header closure. Each provenance records the precise selected prior attempt; later selected attempts include their prior repairs. Compilation uses the existing order_search.compile_variant full-TU path, IDO 5.3, object layout and trailing padding. Scores compare against the existing expected object with funcdiff relocation resolution. No fast/stripped scoring is used. Disposable compiler intermediates are removed after each compile.

Review against CODING_STANDARDS.md: C89 aggregate repairs use fixed-width types; opaque copies and unresolved source hypotheses remain explicitly documented; no forced registers, audit weakening, dummy statements or local renaming was used. The catalog is draft-only and every item requires integration review and default linked verification before a production match claim.
