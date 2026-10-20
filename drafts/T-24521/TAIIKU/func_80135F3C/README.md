# T-24521 selector reconstruction

This body compiles but does not match; production remains INCLUDE_ASM. To reproduce, substitute it in `src/ovl/TAIIKU/80133C80.c` at commit `263294a8f`, add `void func_80135F3C(void);` to `include/ovl/TAIIKU.h` for its earlier caller, and run `tools/funcdiff.py --resolve TAIIKU:func_80135F3C` in Docker.

The default full-unit comparison reports 543 diagnostic rows. The frame is 288 bytes versus the original 296, tables sit 16 bytes low, and the draft saves one fewer register. It uses the existing aggregates and nested selection loops. The initial store of rejected player value 19 is still missing, so this is incomplete source evidence. No compiler, flag or generated assembly change was tested.
