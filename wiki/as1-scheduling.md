---
type: research
updated: 2026-10-20
---

# as1 scheduling: per-file options (T-24800)

Question: do the order-only near misses come from per-file compiler options in the original makefile? Eight guarded
drafts differ from the original only in as1 scheduling (delay-slot fills, `lui` hoists across blocks). T-20620 had
tested as1 options for the whole build and found no rule; [[tickets/T-20700-ido41-as1-test]] showed that IDO 4.1's as1
equals 5.3's. Ticket: [[tickets/T-24800-as1-per-file-flags]].

## Verdict

Per-file options do not explain the family. One option, as1's `-noxbb` (no cross-basic-block scheduling; the driver
passes it as `-Wb,-noxbb`), is the only one that fixes any of the eight:

- VALEN `80133C70.c`: `func_80134030` matches, and so does every other function of the file. The VALEN.bin sha1
  stays OK. It is now built that way (`config/toolchains.txt`, marked FAKE). The proof is weak, though: `-noxbb`
  changes none of the file's other six C functions, so the file shows no positive sign of the option. It only does
  not contradict it.
- GYOZI `80139920.c`: `func_80139E4C` matches, but `func_801399C0` breaks. The original hoists `lui t4/t5` across a
  block boundary in `func_801399C0`, so cross-block scheduling was on for that file. Within one file, one function
  shows the cross-block scheduler and another does not. A per-file flag cannot do that.
- The other six: no option helps, and `-noxbb` breaks 1 to 48 matched functions in each file.

Across the build, `-noxbb` changes 2,450 of the 6,814 unguarded C functions, in 384 of 435 IDO files. Only 51 files are
unaffected, and VALEN `80133C70.c` is one of them. The original was built with the cross-block scheduler on.
The two switch near misses (VALEN `func_80134030`, GYOZI `func_80139E4C`) show it skipping one case: the first `lui`s
of a one-predecessor default block are not hoisted into the selector load delay or the range-check slot.

Lead: the difference is a per-function heuristic in as1's `xbb` pass (`do_xbb_opt`, source file `as1xbb.c` in the
binary's strings), or in what ugen hands it. For a jump-table switch, the original may give the default block more
than one predecessor (for example jump-table entries that point at the default label), and then xbb would not move
its `lui`s. To test this, trace `as1 -R -xbbdbg 8` (the [[matching-notes]] recipe) for VALEN `func_80134030` and GYOZI
`func_80139E4C`. Then edit the binasm block structure of the switch in a scratch shim and see whether the original
schedule comes out with `-noxbb` off.

## Harness

`tools/as1_flag_sweep.py` (in Docker, from the repository root):

    python3 tools/as1_flag_sweep.py src/ovl/VALEN/80133C70.c --standard -j 4 --json build/scratch/as1flags/v.json
    python3 tools/as1_flag_sweep.py FILE.c -V=as1:-noxbb -V="-Olimit 3000" -V="cc-:-O2 -O1" --show-args

It preprocesses the file once with `NON_MATCHING` defined, then compiles the unit through the build pipeline
(`tools/cc.py` and its passes, trailing pad, object trim) once per variant. It compares every function with the
original-side object using funcdiff's resolved rows. Variant tokens:

- `as1:OPT` adds OPT to as1's command line, through an as1 shim that runs the frame pass;
- `as1-:OPT` removes OPT from as1's command line;
- a bare `-OPT` adds a driver option;
- `cc-:OPT` removes a driver option.

Use `-V=...` because argparse would read `-V -g3` as an option. The baseline uses the object's own
`config/toolchains.txt` options. The baseline as1 command line is `as1 -elf -G 0 -p0 -EL -g0 -O2 <in> -o <out> -t <sym>`.
The as1 options were read from the binary's strings, which are 4-byte-swapped in the static recompilation. The
binary has `-noxbb`, `-aggr_xbb`, `-nopeep`, `-noswpipe`, `-nosymregs`, `-nobopt`, `-noglobal`, `-no_const_opts`,
`-no_lui_opts`, `-no_div_rem_opts`, `-no_at_compression`, `-no_branch_target`, `-nopool`, `-newhilo`, `-fpstall_nop`,
`-r3000/-r4000/-r6000`, `-pic0..2`, `-big_got`, `-trapuv`, `-Olimit N`, `-O0..3`, `-g0..3`, `-p0..3`, and the
debug options `-xbbdbg`, `-peepdbg` and `-swpdbg`. `-Wb,` options reach as1 directly (`cc -v`: `as1 ... -noxbb ...`).

## Results

Columns: functions lost (matched under the baseline, not under the variant) and guarded functions gained (last four
hex digits). Baseline: KANGEI 36 of 37, VALEN `80132760` 14 of 15, VALEN `80133C70` 5 of 7 (the second miss,
`func_801342E8`, is the draft's missing instruction showing up as a trailing `nop`), GYOZI 9 of 10, GEKO 54 of 55,
ENDING 15 of 16, TACO `8013B170` 5 of 6, TACO `80149F90` 65 of 67.

| variant | KANGEI 80137B90 | VALEN 80132760 | VALEN 80133C70 | GYOZI 80139920 | GEKO 80134D10 | ENDING 801396E0 | TACO 8013B170 | TACO 80149F90 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| `as1:-O0` / `-O1` | -13 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `as1:-O3`, `-g0`, `-g3`, `-p0`, `-p1` | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `as1:-g1` / `-g2` | -32 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `as1:-noxbb` (= `-Wb,-noxbb`) | -5 | -2 | 0, +4030 +42E8 | -1, +9E4C | -11 | -4 | -1 | -48 |
| `as1:-aggr_xbb` | 0 | 0 | 0 | 0 | 0 | 0 | 0 | -10 |
| `as1:-nopeep` | -9 | -3 | 0 | -1 | -11 | -6 | -1 | -38 |
| `as1:-nobopt` | -4 | -1 | -1 | 0 | -12 | -3 | -1 | -42 |
| `as1:-noglobal` | -13 | -4 | 0 | -1 | -19 | -8 | -1 | -57 |
| `as1:-no_at_compression` | 0 | -1 | 0 | 0 | -7 | -1 | -1 | -29 |
| `as1:-no_branch_target` | -4 | -1 | -1 | 0 | -16 | -3 | -2 | -43 |
| `as1:-r4000` | -17 | -5 | -2, +42E8 | -4 | -29 | -8 | -5 | -45 |
| `as1:-Olimit 50` | n/a | n/a | 0 | 0 | -2 | 0 | 0 | -2 |
| `as1:-Olimit 200/500/1000`, `-Olimit 3000` | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `-noswpipe`, `-nosymregs`, `-no_const_opts`, `-no_lui_opts`, `-no_div_rem_opts`, `-nopool`, `-newhilo`, `-fpstall_nop`, `-r3000`, `-pic0`, `-pic1`, `-trapuv` | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `-Wo,-loopunroll,0` | 0 | 0 | 0 | 0 | 0 | 0 | 0 | -1 |
| `-Wb,-noshortcut`, `-Wo,-noloopopt`, `-p` | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
| `-noxbb` + `-nobopt` | -6 | -2 | -1, +4030 +42E8 | -1, +9E4C | -19 | -5 | -2 | -52 |
| `-noxbb` + `-no_lui_opts` or `-nosymregs` | -5 | -2 | 0, +4030 +42E8 | -1, +9E4C | -11 | -4 | -1 | -48 |
| `-noxbb` + `-O1` | -13 | -2 | 0, +4030 +42E8 | -1, +9E4C | -11 | -4 | -1 | -48 |
| `-noxbb` + `-nopeep` | -12 | -3 | 0 | -1 | -15 | -8 | -1 | -55 |
| `-noxbb` + `-noglobal` | -13 | -4 | 0 | -1 | -19 | -8 | -1 | -57 |

Variants that do not compile (no result):

- Driver `-O1`: `tools/ugen_pass.py` rejects the `swp` opcode in ugen's input. `-g2` fails the same way.
- Driver `-O3`, `-g` and `-g3`: asm-processor accepts only `-O0`, `-O1`, `-O2` or `-g`, and only `-O1`/`-O2` with `-mips1`.
- `-mips2`: the driver refuses it next to `-mips1`. Removing `-mips1` would allow R4000 encodings, so it was not tried.
- The Olimit cells marked n/a ran before the shim knew `-Olimit` takes a value, so they were not measured. Driver
  `-Olimit 100` fails in KANGEI, ENDING and TACO `80149F90`.

The as1-level `-O`/`-g`/`-p` levels agree with T-20620: none fixes a near miss.

The `-noxbb` census used a scratch script (`build/scratch/as1flags/census.py`, not committed). It builds every IDO
file from its production source with `as1:-noxbb` and compares each unguarded C function with the build object.

## Build support for per-object options

`config/toolchains.txt` lines may now carry IDO pass options after the version: `<object> ido 5.3 -W<pass>,<option>`.
`configure.py` accepts only `-W` options there. `tools/cc.py` adds them to that object's IDO command line.
`cc.toolchain_of` / `cc.toolchain_of_source` give the tools that compile outside ninja the object's own toolchain:
`funcdiff_verify.compile_unit` (funcdiff `--version c`, viewsweep), `order_search` and `permute`'s `compile.sh`.
CODING_STANDARDS section 7a counts an option change for one file as a fakematch. So the VALEN line and
`func_80134030` both carry a `FAKE` comment.
