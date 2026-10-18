# T-24012 pending ETC source drafts

Source-only hand drafts for the three pending functions in `src/ovl/ETC/80132880.c`. Production keeps their original `INCLUDE_ASM` paths. These are **not verified matches** and are not part of the build. The accepted dispatcher is separately committed as `a0675cafe` and passed both independent review axes.

Each draft was inserted separately into a scratch copy of the complete reviewed translation unit and compiled in Docker with `tools/funcdiff_verify.py:compile_unit`. The cost below is `tools/funcdiff.py:diff_score` on resolved instruction words (insertion/deletion 1, substitution 2). Zero would still require explicit default linked-byte proof before promotion. Logs and full diagnostics are ignored under `build/etc24012/`; no game binary, original assembly dump, or build output is archived here.

| Function | Cost | Confirmed reconstruction | Remaining blocker |
| --- | ---: | --- | --- |
| `func_801328A4` | 217 | Existing `Rgb555Split` type; one real 822-record buffer used as three rolling rows of 274 records; SDK `RECT`; correct two-byte input/output stride | Frame and RECT position match, buffer starts at sp+0xA8 rather than sp+0x90; input and divide-constant saved registers swap; channel multiply/divide schedule differs |
| `func_80132C3C` | 180 | Four SDK `POLY_FT4` primitives; cached shared colour; SDK fields replace byte-offset stores; signed word coordinate locals | Built frame 0x40 versus original 0x70, y spill sp+0x3C versus sp+0x38; extra first colour load and coordinate-register allocation differ. Redundant shade writeback is explicitly marked FAKE |
| `func_80132FB0` | 150 | Existing pointer table `D_8014EA18[22]`; absorbed aliases expressed as actual indices 1, 11, 10, 13; ordinary case-local read/status structure | Compiler shares a single table base and adds offsets; original rematerializes label addresses. Factored status tails also differ from the original duplicated tails |

These costs were remeasured from the archived bodies in the current translation unit, `build/etc24012/archive-score.log`. To resume, replace only the target's `INCLUDE_ASM` in an ignored copy of the complete production C file with the draft body after its includes. Compile that copy with the normal full-TU helper. Do not compile the body as a separate production object, change generated assembly, weaken the shared type map, or interpret a diagnostic score as linked verification.

Bounded rejected hypotheses:

- Quad: register/unsigned cached-colour locals, moving the unchanged global writeback, and one long colour-assignment chain do not improve the 180 cost. The real source may have additional locals, but no guessed frame padding was added.
- RGB buffer: declaration order permutations do not improve the 217 cost. Preserving the m2c arithmetic temporary ordering worsens to 302 and changes saved-register allocation. The flat real array avoids crossing an inner row-array object through pointer arithmetic.
- Background reader: direct repeated status assignments cost 200, removing gotos alone costs 212, and factoring status after each special switch-case decision costs 150. An explicit byte-offset local also costs 150. Absolute-address aliases or duplicate scalar declarations were not introduced to defeat the compiler's common-base optimization.

Further work needs evidence for local-variable layout or the original table-address source expressions. The bounded form probes above should not be repeated without new evidence.
