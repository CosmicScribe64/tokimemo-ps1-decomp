# T-24902 KANGEI:func_80134724 (src/ovl/KANGEI/80134120.c)

Status: not matched. Guard kept, 2 rows (funcdiff `--resolve`: real difference, one moved
instruction; both bodies are 172 instructions, `vr.py` score 26 / 214 rows on its own count).

## The miss (as1 default-block hoist, the `sll` variant)

* Original: the range-check slot `beqz at, default` holds the dispatch `sll t1,t1,2`; the
  out-of-range default block keeps its head `sll t0,v0,2`.
* Draft: as1 moves the default block's `sll t0,v0,2` into that slot and the dispatch `sll`
  follows after the branch. Everything else matches.

## as1 trace (new this run)

Recipe: `drafts/T-24900-as1/` copied to `build/t24902/scripts/` (paths patched to
`/work/build/t24902`), `/private/tmp/t24047-uopt/variants/as1-probe` installed as
`build/t24902/bin/as1-x2` (it carries the XBBSCAN/XBBPROBE/XBBDETAIL/XBBRESULT prints).
Capture with `cap.py`, replay with `-R -xbbdbg 8`, cut with `fn.py`, score variants with
`vr.py`. All the capture files are under `build/t24902/cap/` (ignored).

Block facts for this function:

| block | weight | what |
| --- | --- | --- |
| bb 17 | 1 | switch head: `lui at`/`sh zero` (`D_80139AE0 = 0`), `lbu t1` selector, `sltiu at,t1,7`, `beqz at,default` (the stall) |
| bb 18 | 4 | out-of-range default, numbered **before** the dispatch: `sll t0,v0,2; addu; addiu; lui at; sh` |
| bb 19 | 4 | dispatch: `sll t1,t1,2; lui at; addu; lw; jr` |

Accepted moves in our build: `MOVETO bb 11 inst 18 <- bb 17 inst 4` (the selector `lui`,
which the original also has, so it is fine) and, the miss,
`MOVETO bb 17 inst 7 <- bb 18 inst 2` (`time: old 5, new 4`).

The scan for bb 17 is `(1,37) (1,38) (1,39) (4,18) (4,19) (7,6-stop)`. bb 18 comes first
because the weights are equal and its number is lower. The candidate at bb 18 slot 2 is
`op=00024080 reloc=0 sched=0/1 movable=1 graph_reject=0 path_accept=1` -- it passes, so the
stall is filled and the mover never reaches bb 19 (the trace has no `XBBSCAN dst=17 src=19`).
The original reached bb 19 instead, so in the original bb 18 was either heavier (weight 7,
i.e. its dominator the dispatch = a jump-table entry) or later in the scan than bb 19.

## Tried

* `volatile` on the default arm's global `D_80139ADC` (variant source + a header copy in
  `build/t24902/hdr/`, so the tree was not touched): `vr.py` identical to the baseline
  (score 26, 214 rows, len 172/172). Expected: the hoisted instruction is a plain `sll`
  (relocation 0), not the `lui` of the stored global, so the volatile mark cannot pin it.
  This is the ticket's named lever and it is now measured, not assumed.
* Nothing else this run: T-24201 already lists ~20 forms (default spellings/scopes/line
  layout, `case 7: default:`, second range check, selector casts/locals, explicit join,
  `goto end`, data placement), all neutral or worse, and the trace explains why a source
  spelling of the same instruction stream cannot move the pick.

## Next concrete hypothesis

Get bb 18's weight to 7 or bb 19's weight to 1 (or bb 19 numbered before bb 18) without
changing the instruction stream. Both are properties of the ugen/as1 block graph, so first
work out the weight pass: `func_429534` in `/private/tmp/t24047-uopt/variants/as1-probe.c`
(~line 51800) is the backwards weight computation (`succ weight - 3 + 1 or + 3`, min over
successors, writing block+72); the `+1 instead of +3` region test is the only free parameter
the wiki still lists as open. A form that gives the out-of-range default a second predecessor
without a table entry (the `case 7: default:` cost) would do it.
