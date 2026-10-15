# T-14700 type evidence

Selector table copies 20 bytes plus one byte and is indexed by lbu: u8[21]. Target accesses sp+0x48 as lw, lhu and four bytes; existing GsWord union exactly models those operations. No scalar-to-union cast or invented sp base remains. Partially initialized packed bits reflect target read-modify-write behavior.

Original artifacts retained with original- prefix. Baseline source/header closure retained. Full TU uses unchanged production context. No production promotion or literal-byte match claim.
