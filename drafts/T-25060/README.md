# T-25060 final TEL menu qualification

Owned target: `src/ovl/TEL/8013CCD0.c`, `func_8013D88C`. Work starts from
`596c2c269` on `codex/t25060`, after the independent T-25055 promotion. The
production target remains `INCLUDE_ASM`; these are source-only diagnostics.

The target has 2,972 progress bytes and 2,980 literal linked bytes. The latter
includes two trailing padding words, so comparisons cover 745 words.

## Baseline and lifetime evidence

Current full-TU replay of `drafts/T-24511/menu-reconstruction.c` produces
755 words, 689 changed positions and aligned cost 798. Its captured native
frame is 224 bytes; the uniform frame pass adds 16, giving a final 240-byte
frame against target 224. The old 232-byte observation describes earlier
controls, before the archived packed-word capture was added.

The current flag induction pointer already occupies v1. Reusing the coordinate
local for the packed flags creates a separate saved s2 range at priority 14,
above availability, index and constants. Moving the three packed captures into
the dead page-selector parameter removes that interference, while preserving
the observed early load before the month ladder. The freed fp is then taken by
the flag pointer. The target instead saves the x-array pointer there.

| Independent control | Words | Changed positions | Aligned cost |
| --- | ---: | ---: | ---: |
| Archived baseline | 755 | 689 | 798 |
| Capture packed word in dead arg0 | 751 | 702 | 786 |
| Same, narrow coordinate x | 751 | 702 | 786 |
| Negative literal multiply, baseline | 759 | 696 | 792 |
| Negative literal multiply, arg0 | 755 | 703 | 770 |
| Unsigned-false x-pointer read, arg0 | 750 | 698 | 851 |
| Unsigned-false index read, arg0 | 747 | 707 | 804 |

The negative multiply literal restores the target's negative quotient followed
by a negative multiply macro. A double unary negation in the earlier expression
does not produce that macro. The pointer/index reads are allocator diagnostics;
they do not establish a plausible original source or a matching result.

Three page-one packed reads still use signed aliases, unlike the remaining
unsigned reads. Only those three emit separate absolute addresses instead of
sharing the target's GameState base. The next control makes their unsigned
identity consistent. This also removes their signed-left-shift issue.

## Replay

Run inside the existing `tokimemo-decomp-arm64` image, mounting this isolated
tree at `/work` and the original disc read-only at `/work/disc`. Coordinate a
compiler slot with the parent before compiling. No shared tool edits or flags.

```
python3 drafts/T-25060/lifetime_controls.py
python3 drafts/T-25060/probe.py drafts/T-24511/menu-reconstruction.c --capture
python3 drafts/T-25060/probe.py drafts/T-25060/word-arg0.c
```

The generator records independent transformations. The probe replaces only the
owned target inside the current complete translation unit and performs resolved
default-C object comparisons. `--capture` uses the unchanged T-25055 forwarding
wrapper to retain compiler pass inputs privately. Generated objects, assembly,
full translation units and native traces remain under ignored `build/T-25060`.
