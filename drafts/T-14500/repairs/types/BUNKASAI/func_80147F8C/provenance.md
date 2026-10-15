# T-14700 type evidence

Target copies 0x129 bytes (296-byte word loop plus byte tail). Address passed to func_800AE0F0 uses byte strides 0x63 and 0x21. Earlier unprototyped TU call implies s32 return under -cckr; no return added.

Original artifacts retained with original- prefix. Baseline source/header closure retained. Full TU uses unchanged production context. No production promotion or literal-byte match claim.

Natural offset local declared before copied table follows T-3330. The offset is used in the message pointer calculation; no unused padding local. Previous full-TU score was 2 (table base sp+0x2C versus sp+0x28).

Offset-local alternative scored 14 and is preserved under alternatives/natural-offset/. Selected the previous direct-expression form (score 2).
