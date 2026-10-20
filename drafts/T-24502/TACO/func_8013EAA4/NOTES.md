# T-24502 typed three-quad renderer checkpoint

Production remains INCLUDE_ASM. Fresh SDK-typed C compiles; old T-16560 draft did not. Scratch requires `extern u8 D_8015F560[8], D_8015F568[8], D_8015F570[8], D_8015F578[8];` before the body; these were not added to production headers.

Best typed body uses one o/j pair for all projection loops, distinct y0/y1/y2 accumulators. Resolved search cost125 (old fresh baseline415); scratch484 words versus target487, where target has four trailing nops. Frame232 versus target272. First-loop instructions/registers match except homes and initial zero scheduling; v/pt/pos homes already match C0/B0/A8, matrix is C8 versus E8, depth/flag A4/A0 versus94/90, kind/phase9C/98 versus88/84. Main remaining structural miss: middle projection loop promotes wave size in s5, step spills; original spills size at74 and keeps step in s2 plus outer row count in s4. Current optimizer replaces outer count with the derived byte offset. Its CLUT remainder/selector registers also differ. Third outer-loop latch compares derived offset8 rather than y accumulator512.

Bounded experiments: shared loop accumulators gave415; splitting only y gives125; splitting only o309, only j471, o+y367, all499. Splitting the middle o/size separately or writing the middle loop as an explicit pointer walk worsens allocation. No broad scheduler search or compiler edits performed. Continue from this body only with a concrete induction/spill hypothesis.


## Full-TU coordinate reconstruction, resumed

No match or production/header change. body.c preserves the original control. Its historical125/484-versus487 figures above were standalone, not full-TU bounds. Full-TU control: target491 words, draft488, aligned cost121. Target real function487 words plus four trailing nops. Do not count the padding as newly recovered instructions. Current coordinate.c: target/draft491 words, aligned cost186 and192 printed positional rows (96 differing words). Aligned cost is not the printed row count.

A real middle-row use, y1=o*height*2, retains original s4 row counter and slti<2 latch. Separating sine height from cosine width then restores original height spill, doubled-height s2 and Y offset s5. Middle projection instruction/register sequence agrees apart from stack homes. Final projection now controls the outer loop by Y!=512 and advances point index in its body; its endpoint matches and later temporary ring recovers. These are geometric/control-flow reconstructions, not zero-subtree or padding controls.

### Exact target/candidate home map

Offsets hexadecimal, sizes decimal bytes. Names identify current source values; original declarations remain unknown. Saved-register and argument areas agree. Source lifetimes explain actual uses, not the unexplained free frame regions.

| Value | Size | Target | Candidate | Actual source lifetime/use |
| --- | ---: | --- | --- | --- |
| Frame | 272 / 240 | 110 | F0 | Whole function |
| MATRIX matrix | 32 | E8 | D0 | Address passed to both matrix setup calls; reserved object home |
| SVECTOR v | 8 | C0 | C8 | Address passed in all three projection grids |
| s32 pt[4] | 16 | B0 | B8 | Reused output grid, each polygon consumes its points |
| TcPos pos | 8 | A8 | B0 | Initial particle copy; coordinate reads across all three grids |
| s32 depth | 4 | 94 | AC | Address passed to projection calls |
| s32 flag | 4 | 90 | A8 | Address passed to projection calls |
| s32 kind | 4 | 88 | A4 | Initial mask; first polygon lookup/UV stores |
| u32 phase | 4 | 84 | A0 | Initial shift; first lookup and later CLUT cycle |
| s32 height | 4 | 74 | 78 | Middle sine height, survives projection calls |
| doubled height IV | 4 | 6C | 6C | Compiler loop invariant, reloaded to s2 |
| Tc14 *particle | 4 | 64 | 64 | Used again for both trig arguments |
| s32 dist spill | 4 | 60 | 60 | Across polygon and matrix/texture helper calls |
| s32 clut | 4 | 5C | 5C | Second polygon CLUT reused for third |
| Point byte-offset IV | 4 | 54 | 58 | Compiler output-address IV, all grids |
| Third Y accumulator | 4 | 58 | 80 | Final grid outer Y control |

The scalar IV homes are compiler spills/temporaries, not extra declared arrays. o/y0/y1, width, poly and first-cycle frame also have register ranges; do not invent padding to account for these. Current MATRIX high endF0 coincides with frameF0; target high end108 is eight bytes below frame110. This observation does not prove a missing SVECTOR or unused declaration.

### Twelve differences without sp as base

The remaining96 differing positions classify as84 sp-based differences and12 others. Some sp-based pairs also use different load registers (kind at146; third latch401/402), so they are not all pure offsets. Zero-based positions below identify small decoded differences only; no instruction dump is archived.

- Positions59/60: first grid's projected-vector pointer copy and Y=0 initialization exchange order (2 words).
- Positions149,151,153,160,163,164,165,167: first CLUT range allocation (8 words). Target kind a3, palette-row base v0, modulo result a2; candidate a2, a1 and v0 respectively. Same lookup values and unsigned modulo/conditional semantics; source use/order remains unresolved.
- Positions404/405: final grid's point-offset+4 and Y+256 arithmetic exchange order (2 words). Target point arithmetic first; candidate Y first. Associated load/store roles at401/402/406/408 are counted among sp-based differences. Final Y512 branch condition and later temporary ring otherwise agree.

### Bounded lineage and rejected forms

All serial full-TU controls: middle pixel-only index491cost/480words; third Y bound on old control253/488; independent middle pixel and point counters125/488. Direct row geometry alone326/493 retains target row count but spills Y while shared height/width occupies s5. Separating the real dimensions gives186/491 (printed192 rows). Applying final Y endpoint gives196/491; using Y as the canonical for-loop control and point-index increment in its body gives current186/491 (printed192 rows). No zero-subtree, width-grid or frame padding was attempted. Old scalar splitting/pointer-walk negatives remain excluded.

Current private exact full TU/object/diff: build/T24502/eaa-third-coordinate-for/{guarded.c,guarded.o,diff.txt}. Control is build/T24502/eaa-resume-control. build/T24502/eaa-row-geometry/reg.txt traces the earlier shared-height form only: size priority33.5, o11. It is not a final-candidate trace and must not be used to diagnose current split-height ranges.

Replay from repository root inside Docker:

```
python3 drafts/T-24502/TACO/func_8013EAA4/replay.py --control
python3 drafts/T-24502/TACO/func_8013EAA4/replay.py
```

The helper retains the complete current TU and leaves other guards at default. Four texture-table declarations are scratch only; no production header changes. The candidate file is byte-for-byte the tested private body; no extra compile was performed merely for handoff. Narrow consult question: infer genuine local/aggregate/spill organization consistent with the home table and then explain the first CLUT web; do not run a padding/declaration/scheduling grid.
