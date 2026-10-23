# T-25118 private copy-state diagnostics

Not build tools. They reproduce the measurements in `../NOTES.md` from a scratch copy of the
toolchain under `build/` (gitignored). Run from the repository root, inside Docker:

```
mkdir -p build/T-25118/diag && cp -R tools build/T-25118/diag/tools
patch -d build/T-25118/diag -p1 < drafts/T-25118/main/strCallback/diag/ugen_pass-diag.diff
cp drafts/T-25118/main/strCallback/diag/*.py drafts/T-25118/main/strCallback/diag/run.sh build/T-25118/diag/
```

- `run.sh SRC TAG` with `DIAG_STATE=F|B|none|INV|INVSMALL|RULE1` compiles one file with NON_MATCHING
  through the scratch shim; `DIAG_DUMP=FILE` writes the post-uopt ucode listing.
- `census.py FILES` (`CENSUS_MODE`, default INV) compiles each file normally and in the given mode
  and lists the functions whose bytes change; `DIAG_LOG` lines give each `Umov` with its state.
- `census2.py LIST` scores guarded functions under each of `MODES` against their target `.s`.
- `join.py` tabulates the census logs by state and context.
- `CC_PATH` points both census scripts at another scratch tree, for example
  `build/T-25118/diag2/tools/cc.py` with `ugen_pass-rule2-diag.diff` applied: there the candidate
  rule runs only under `DIAG_STATE=RULE2`, with the size limit `DIAG_SMAX` (default 32).
