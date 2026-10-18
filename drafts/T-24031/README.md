# T-24031 paired particle-handler evidence

This bounded pass adds **zero verified bytes**. All four production functions retain their original assembly fallbacks. The files here are compiling source-only drafts, not default C matches or source-canonical claims. Existing BUNKASAI wins and all production C/header files are unchanged.

## Reproduction and measured result

From the repository root, run `tools/docker.sh python3 drafts/T-24031/measure.py`. It inserts each body into a private copy of its complete production translation unit and compiles serially with the normal IDO pipeline. The GYOZI scratch preprocessing changes the existing `void func_80144018()` declaration to an `s32` view to permit the archived fall-through body. The production header is untouched. A future production type change must be reported and verified with its callers; this archive does not authorize one.

| Unit / function | Literal target size | Resolved aligned cost | Printable rows |
| --- | ---: | ---: | ---: |
| SHOUGATU `func_80141D20` | 1,204 | 264 | 264 |
| GYOZI `func_80144018` | 1,204 | 266 | 266 |
| SHOUGATU `func_8014288C` | 1,300 | 412 | 508 |
| GYOZI `func_80144B70` | 1,300 | 412 | 508 |

The aligned cost is `tools/funcdiff.py:diff_score` on resolved instruction words (insertion/deletion cost 1, substitution 2). It is a diagnostic, not linked-byte verification. Even a zero would need actual default C installation, explicit linked proof and checksum checks. Ignored evidence is under `build/pairs24031/`: `archive-proof.log`, per-variant `diff.txt`, and the final default build/guard logs. No original assembly dumps, binaries or generated outputs are committed here.

## Source corrections and provenance

Start from T-24004's repaired SHOUGATU 1,204-byte body and T-17500/T-17600's 1,300-byte bodies. The existing setup neighbor `func_80141CD8` establishes count 1 and record 4 as the initial object. The exact opcode pairs allow transfer to the other overlay after correct object indexing.

- Spawn record is `Rec24[count + 3]`, its period and velocity are work-array entries `[count - 1]`. The old byte offset 122 is three records plus field 0x0E, not an unrelated byte object.
- Update records use `i` starting at 4; period, velocity, position and destination use `[i - 4]`. Entry pointers are formed at that valid index, rather than forming pointers beyond the 50-entry arrays and subtracting four later.
- The hide loop writes records 14 through 53 directly. The typed census's `Rec24_base + (i * 0x24)` multiplied the stride twice.
- Position is the existing `Fix32` view of a real word array. The reset is a **low-halfword clear**, followed by a signed high-halfword coordinate store and a **full-word** destination copy. The old full-word zero produced the same eventual bits only after later stores, but did not reproduce the actual mutation widths/order.
- SHOUGATU per-record bytes use the actual GameState `unk_10A5` array; GYOZI uses the known `GyoziWork.unk_F30` slice at index `0x145 + record`. All such accesses use one consistent local state pointer.
- Interior scalar labels `D_80121864` / `D_8012CAD4` are the existing Rec24 array's `[4].unk_04` field. The archived source uses that real aggregate view.
- Counter mutations operate on the actual shared s16 global, including stores before RNG calls and reloads afterward. Unneeded self-assignment scaffolding is removed. RNG calls have no invented argument values.
- The 1,300-byte source reuses one s16 index for its hide/update loops and one signed period temporary; dead/split m2c temporaries are removed. No padding, guessed aliases, volatile accesses or per-function compiler changes were added.

## Measured controls and remaining blockers

For the 1,204-byte SHOUGATU target, the T-24004 baseline reproduces cost 387 / 491 printable rows. Correcting the halfword reset and the explicit zero-count return gives 385. Direct shared-counter mutations improve it to 300; valid entry-pointer indices plus the fixed-point view reach 264. Its frame matches the target's 0x50, but the count starts in a0 instead of a1 and later array/loop ranges allocate differently across RNG calls. This is still a broad source-allocation difference, not an isolated scheduler fix.

For the 1,300-byte target, the repaired legacy source starts at 459. Direct counter mutations give 438. Joining the induction/period temporaries gives 412 and reduces the frame from 0x70 to 0x68, still above the target's 0x50. The target retains a stride-36 multiplication range and different loop/array bases. Explicit period/velocity/position/destination entry pointers worsen to 426 with frame 0x78. A compact rewrite gives 472; restoring its RNG/store ordering gives 473. These negatives do not justify declaring the function compiler-limited.

Consistent state-pointer views and the real constant Rec24 field preserve the best costs in both pairs. Reusing period/coordinate temporaries in the 1,204-byte source leaves 264; directly combining its word arithmetic worsens to 301. No broad permutation, old uniform redraft sweep, or guessed frame padding was run. Further work should recover the live induction/array webs around the RNG calls rather than repeat these controls.

Default GYOZI and SHOUGATU checksums and header/global/prototype guards are recorded separately as preservation checks. They do not verify the archived C, since the unchanged production targets still build from assembly.
