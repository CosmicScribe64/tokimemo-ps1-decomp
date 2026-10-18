# T-24022 TEL experiment checkpoint

Base source: `drafts/T-23001/TEL/func_80136724/hand.c`, current full translation unit from base 8052373f7.
Run scripts from repository root inside the existing Docker image, sequentially. `probe_tel.py --full` is the measured path. Outputs go under ignored `build/t24022/`.

All scores in results.jsonl are aligned insertion/deletion costs, not historical printed rows. Baseline 1038 (1643 instructions against 1645, printed rows 1048). Conditional-store variant 990 (1645 instructions, printed rows 1746) is retained only as an alternative: its conditional stores have the wrong placement and it does not restore the month global register. It is not a preferred reconstruction or a match.

Seven normalization forms including baseline, eight access/local-type forms, and eight structural forms compiled. No month promotion or new verified C bytes. The volatile local is diagnostic only and is not retained as candidate source. Bit-field line grouping did not change output. No compiler or production changes.
