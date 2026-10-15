# Compiling alternatives for 23 remaining overlay drafts

T-15800 covers the 23 nonreserved failed scopes assigned from the T-14500 best catalog: GYOZI 5, OPTION 5, SHOUGATU 6, SHUGAKU 3, MASTER 1 and NAME_ENT 3. All alternatives are archived C, outside the default build. Production sources and the immutable T-14500 attempts are unchanged.

Each function folder contains its final body, declarations, exact full translation unit, preprocessed compiler input, diagnostics, result metadata and provenance. Earlier attempts retain their input context and diagnostics under `attempts/`. `catalog.jsonl` maps every scope to its final folder and result. Header hashes reference the common frozen `drafts/T-14500/baseline/include` snapshot, from commit 4836200960c04659c0705aa566dc4da3b5749e7a. Compiler script and Dockerfile hashes are recorded in each result.

Repairs recover copied callback tables, coordinate and threshold arrays, byte-pointer arithmetic, existing K&R return views, and actual callee arguments. Callback sizes come from the observed copy lengths, corroborated by data-symbol contents; an enclosing data-symbol span can contain additional words beyond the copied callback table. NAME_ENT's phantom unset-register arguments were removed after callee inspection established that func_80054E6C takes no arguments. Provenance records the evidence for each repair. These are nonmatching drafts with unresolved layout and type details; successful compilation does not establish original-source equivalence.

Run through the existing project Docker wrapper, with local extracted game/build inputs already available:

```
tools/docker.sh python3 drafts/T-15800/prepare.py
tools/docker.sh python3 drafts/T-15800/compile_drafts.py --all
```

The serial driver skips recorded successes with `--all`; pass individual `UNIT:func_XXXXXXXX` scopes to replay them. With no arguments it replays every scope and refreshes scores, hashes and preprocessed contexts. Source-only preparation links existing asm/expected/disc inputs into an ignored analysis directory; it does not copy or modify them. Scores are full-TU resolved instruction-diff rows, not literal byte-match claims. No target ASM fallback, empty stub, or inline-assembly substitute is counted as a compiling alternative. No objects, original binaries, or assembly files are archived.
