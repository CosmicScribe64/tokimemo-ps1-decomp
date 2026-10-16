# T-16550: TAIIKU redraft after the types-first pass

One directory for each of the 58 unmatched TAIIKU functions (of 64 at the start) that got a compiling new draft; `func_80134060`, `func_80134F98`, `func_80135F3C`, `func_80132BD0`, `func_80132E20` and `func_80147B14` did not. Each holds the best new draft:

- `body.c`: the function.
- `declarations.h`: declarations the draft needs that the headers lack.
- `context.c`: the full translation unit that was scored (the C file with the draft in place of its `INCLUDE_ASM`).
- `result.json`: `score` (full-translation-unit diff lines, as in `drafts/T-14500`; 0 is a candidate match, not a verified one), `census_score` and `census_path` from `drafts/T-14500/best-quality-catalog.jsonl`, `variants` that produced this body, and provenance.

`func_80141CDC` is matched in `src/ovl/TAIIKU/80141C00.c`; its directory keeps the 18-row draft it came from.

## Reproduce

Run in Docker from the repo root (`tools/docker.sh`):

```
python3 drafts/T-16550/redraft.py --variant fold --out build/t16550/fold
python3 drafts/T-16550/redraft.py --variant types --out build/t16550/types
python3 drafts/T-16550/redraft.py --variant fold --fold-scope main --out build/t16550/foldmain
python3 drafts/T-16550/redraft.py --variant types --guarded --out build/t16550/guarded
python3 drafts/T-16550/redraft.py --variant body --from DIR --out OUT    # rescore other bodies
```

The variants are described in the script's docstring and in `wiki/taiiku-family-results.md`. These drafts were rescored with the header of the commit named in each `result.json`.
