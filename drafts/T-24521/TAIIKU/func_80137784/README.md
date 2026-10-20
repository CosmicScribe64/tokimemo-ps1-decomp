# T-24521 runner transition foundation

This source compiles but does not match; semantic equivalence is unproved. Production keeps INCLUDE_ASM. Replace that line in `src/ovl/TAIIKU/80133C80.c` at commit `fef889358` with `typed.c`, and add `declarations.h`'s four declarations to the canonical overlay header. Run `tools/funcdiff.py --resolve --version default TAIIKU:func_80137784` in Docker with default IDO 5.3 flags.

The body replaces T-16550's raw actor/runner offsets with existing types and fixes its clear-loop bound from 0xCC actor records to three. Each original accumulator data symbol owns 16 bytes, with slot*4 indexing. The 4984C object's word update and signed halfword+2 load use existing Fix32. Shared player/cpu indexing retains an explicit FAKE comment, as in matched func_80138680.

One corrected foundation gives 396 diagnostic rows, 296 instructions versus 304, and a 120-byte frame versus 56. The original shares a 68 multiplier through multu; pointer indexing produces shifts/adds. The three-record clear loop rolls, but register ranges and call spills remain broad. No declaration/view grid followed. Production and header were restored. The local diagnostic is `/private/tmp/t24521-37784-typed.log`.
