# T-25021 source checkpoint

`recovered-typed-baseline.c` is the recovered T-25016 m2c body with current GameState fields, signed halfword buffer accesses at +2, actual selection-buffer word accesses, and callee argument lists checked against matching definitions and original assembly.

`body.c` additionally reuses nonoverlapping roles, preserves full-width F61+2/F61+3 comparisons, reads the signed GameState mode field through u8 as required by target lbu, and orders eight declarations to reproduce the original 64-byte frame and spill homes (result +0x34, text buffer +0x30, selection buffer +0x28).

Measured in Docker as the guarded full src/main/8004F870.c translation unit at base 7bc6d8d99. Final candidate: expected315 words, built313 words, resolved aligned cost138 (insert/delete1, substitution2), printed diff lines138. Not a match. Baseline has96-byte frame and325 printed diff lines; no stable aligned score was saved for that baseline. Intermediate72-byte frame has315 printed diff lines; eight-role64-byte frame before home/type correction has315/313 words, aligned226, printed230.

Original production source is restored. No header edits. No game data, objects, assembly or raw diagnostic traces are stored here. Private measurement artifacts remain in /private/tmp/t25019-taco/build/t25021-*.

Next causal source issue: mode is cached across k_disp_inc in the candidate, whereas target reloads GameState+0x3C after that call before the zero test. First raw instruction differences are earlier register ownership of phase/count values. A fresh checkpoint should address the actual read boundary before attempting register variations.
