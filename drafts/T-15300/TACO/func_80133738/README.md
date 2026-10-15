# TACO func_80133738 unfinished structural probe

This source-only archive is nonmatching and is not used by the default build. The production source retains its original INCLUDE_ASM. `context.c` is the exact guarded translation unit that produced `structure-diagnostics.txt`; `header-context/` freezes its quoted header closure. The frozen first-attempt seed remains in T-14500's TACO catalog.

The m2c seed split one saved selector into a dead spill local and a separate selector, and its inferred scalar table pointer incorrectly scaled the index by 32 rather than 8. The experiment restores one selector initialized before calls, models the six eight-byte lookup entries using existing TcPair8, and replaces byte offset 0x4B8 with existing TcActor element 8's field at 0x78. These are ordinary data/flow corrections, not padding or forced registers.

The result still differs at eight instruction positions: four selector spills/reloads use stack +0x2C rather than +0x28; four lookup instructions use adjacent temporary registers. It is not a literal match. No further variants were tested after the user requested a handoff. Reordering the two genuine local declarations is an untested next lead; no result is claimed.

`seed-diagnostics.txt` records the canonical-global normalization issue. `state-diagnostics.txt` records the normalized raw seed; `structure-diagnostics.txt` records the latest compiled variant. `reproduce/` contains the actual local transformation scripts, which run in the project Docker image against a checkout with the guarded seed installed. No assembly files, original executables, or binary build outputs are included.
