T-25007 pass on main func_8005938C. Best draft is still drafts/T-17017/main/func_8005938C-closer/body.c
(124 funcdiff rows, all ugen temporaries; frame, stack offsets and s registers agree).
- Each 32-byte MATRIX copy takes two ring slots in the original (one skipped, one data register: t4/t5,
  t6/t7, t5/t6) and three in the build (t4,t5 skipped, t6 data). Matched copies of the same
  `*(MATRIX *)D_80125C60` elsewhere (ETC func_8013FA7C, TACO 80146D60.c) take three slots in the original
  too, so the build reproduces those; this function's original differs.
- Tried, no change (124): `rot.vz = ...` without the pointer p; `*(MATRIX *)&D_80125C60`. Worse: a
  `MATRIX *g` source pointer (148), `sm = m = ...` (226).
