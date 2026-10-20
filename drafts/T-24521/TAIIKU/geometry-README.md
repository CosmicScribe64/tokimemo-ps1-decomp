# T-24521 geometry checkpoints

These bodies compile but do not match. Production keeps its assembly fallbacks; neither body earns coverage or has proved semantic equivalence.

To reproduce, substitute one body at its original INCLUDE_ASM in `src/ovl/TAIIKU/80138950.c` at commit `89d2721f4`, then run `tools/funcdiff.py --resolve TAIIKU:func_8013E894` or `TAIIKU:func_8013E1FC` in Docker with default IDO 5.3 flags. The existing header supplies TaiikuCamera0C, TaiikuBall and all declarations; no header probe is required.

E894's real arrays and shared natural stage counter recover the target's separate equal-valued stage limits, spill the angle and give the exact 200-byte frame. Known homes agree: x 0xAC, y 0x9C, rotation inputs 0x94/0x90, angle 0x78. The saved body has 245 instructions versus 243 and 224 diagnostic rows. First structural difference is an extra `move a0,v0` at instruction 106. The shared x-index range takes v0 instead of the original camera-x range. Source-only stock/tracer comparison checked emitted section inventories and bytes before trusting the allocator trace; they agree. Trace executable was the existing `/private/tmp/t24047-uopt/variants/trace`, mounted read-only. No compiler was modified or rebuilt.

E1FC's saved explicit next-row reconstruction has 548 diagnostic rows and a 208-byte frame versus 200. Arrays have the right homes, but the next-angle home is 0x74 versus 0x60. Derived adjacent-angle induction regresses to 606 rows; spelling the derived loop bound changes no bytes. The saved body is the best explicit-next-row version, not that regression.

The ticket records the bounded alternatives and first-divergence evidence. Local diagnostic logs remain under `/private/tmp/t24521-E894-*.log` and `/private/tmp/t24521-E1FC-*.log`; these are session artifacts, not required source dependencies.


EC60 is one structural transplant from the E1FC body: positive straight bounds, all four straight points rotated by the camera, and curved input (0, -radius). Its original actor-102 x-coordinate store is expressed through DisplayActor44. The single compile reports 489 diagnostic rows, 458 instructions versus 447, frame 216 versus 200, and x/y homes 0xBC/0xAC versus 0xAC/0x9C. Its induction already differs at the prologue: constant 8 instead of the original zero/current-plus-one sequence. This broad result was archived without another compiler probe. Reproduce by replacing only the EC60 INCLUDE_ASM with `func_8013EC60/typed-arrays.c` at commit `4b7709f3c` and running the same Docker default funcdiff command for EC60. Production was restored.
