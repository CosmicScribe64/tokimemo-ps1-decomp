# T-14700 type evidence

Existing EVENT.h EvFnTbl44 donor exactly covers 0xB0 bytes (0xA8 block plus two words). Dispatch and callback comparison use .f entries.

Original artifacts retained with original- prefix. Baseline source/header closure retained. Full TU uses unchanged production context. No production promotion or literal-byte match claim.

Review correction: removed the residual m2c pseudo-global sp. The selected dispatch now reads the recovered local EvFnTbl44 directly; full-TU rescoring after this correction is 82 rows.
