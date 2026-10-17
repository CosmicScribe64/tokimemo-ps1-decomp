# Binasm order diagnostics (T-17107)

Scratch tools from the closer pass on RPG_BAT `func_8013E54C`. They show the order in which ugen hands instructions to as1, and let you reorder those records and assemble again. Not part of the build. Run them inside Docker from the repo root (`tools/docker.sh sh -c '...'`).

- `ccdump.py SRC OUT.o`: compiles like `tools/cc.py` and saves ugen's binasm as `OUT.o.G`, its symbol table as `OUT.o.G.T` and the as1 arguments as `OUT.o.G.args`.
- `decode.py OUT.o.G [N] [START]`: prints N records of the first procedure on one line, with `.loc` lines as `L<n>`.
- `reas.py IN.G OUT.o OP...`: edits the records and runs the project's as1 on them. Operations: `sw:A:B` swaps two records, `perm:A:B:i,j,...` reorders a slice, `del:A` drops a record, `loc:SRC:POS:LINE` inserts a copy of `.loc` record SRC with another line, `sym:A:SYM:IMM` changes a record's symbol and offset.
- `scan.py G...`: lists the backward `bne`/`bltu` loops whose start pointer and limit both come from `la` in the preheader, and in which order.

Result: in `func_8013E54C` ugen emits `la start` before `la limit`. Swapping those two records reproduces the original's case 3. Across all 443 C files, none of the 190 matched loops of this kind has the limit first.
