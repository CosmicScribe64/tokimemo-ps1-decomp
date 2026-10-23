# T-24502 typed sprite renderer checkpoint

Production remains INCLUDE_ASM. Scratch prologue for this body:

```
#define MAIN_API_OVERRIDE_func_80049A40
#include "common.h"
#include "ovl/TACO.h"
void func_80049A40();
```

The natural four-arm threshold chain restores the original redundant red-channel assignments; the decompiler's factored nested assignments do not. A wide fourth argument view and no explicit s16 argument cast remove the two narrowing instructions that the target lacks. Assigning each channel `*= value` improves its allocation, but mflo still chooses temporary registers rather than the original s0/t0/t1 for the first two channels; subsequent temporary-ring and scheduling differences propagate. The first UV constants are interchanged in schedule (same registers). Scratch has260 words, matching target length, and frame72 versus96. Resolved search cost154. u8 channels are better than s32 red; grouping all multiply assignments on one line changes nothing. No production header override was added.
