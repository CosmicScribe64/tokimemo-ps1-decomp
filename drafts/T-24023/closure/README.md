# EVENT handler closure

`func_801103A0` is now default C. All 1,712 instructions / 6,848 linked bytes match, and the full EVENT SHA1 is ff7184d7e92b7aff372450ac3b8c2bdd42e55a5e. Header, global and prototype guards pass. The canonical return declaration is `s32 func_801103A0(void)`; its sole C caller ignores the result.

The final `body.c` reproduces production source. Run the full-TU replay inside Docker:

`python3 drafts/T-24023/closure/replay.py`

The replay compiles the match, independently links it for literal comparison, and measures three negative controls plus the earlier 214-cost checkpoint. It never modifies production source or the compiler.

Distinct month identity operands prevent a shared Boolean range. A positive date guard preserves both failure paths and fixes tail scheduling. A u32 selector copy per arm fixes most branch operand order; four comparison constants use the existing byte-gap local. The remaining nine pairs close with the byte view `((u8 *)&D_801243A4)[0xC]`, which addresses D_801243B0 through adjacent storage. The aggregate remains unknown. This view is explicitly marked FAKE under standards section 8, as are the identity operands and constant captures. Literal and full-overlay verification establish the address and emitted bytes.

No diagnostic call-argument changes were accepted. Experimental scripts and objects remain ignored under `build/t24023-*`. The previous `../body.c` and bounded research scripts remain historical evidence. Formal review and integration belong to the orchestrator.

The s32 view is intentional. Early exits return zero; the target's final path calls `func_80011DFC` and returns with v0 left by that callee. The C body falls through on that path, and its sole caller discards the result. Adding a tail return would change the target behavior.
