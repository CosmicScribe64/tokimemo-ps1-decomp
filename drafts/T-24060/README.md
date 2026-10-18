# T-24060 SHUGAKU short-selector source pass

Zero of two matched; no new C function/byte count. Production src/ovl/SHUGAKU/80138A60.c, headers, config and compiler remain unchanged; both targets remain plain INCLUDE_ASM. Baseline `ninja -j1 build/ovl/SHUGAKU.ok` passes, SHA1 e9c64e952a1ef5dfea5e9b3e6a659aa6ccb8eb8c. No fallback comparison is counted as recovered C.

Read current family, T-14500/T-18000 bodies and T-10310 selector negative evidence. Fresh sibling census lists no matched target sibling above0.70; the same-TU matched func_80139054 provides a real byte-variable reuse recipe, not a claim of a close complete clone. No broad sweep or new header declaration was promoted.

func_801392B4 (272 bytes), phase.c: canonical GameState fields, meaningful full-word previous phase/next state, structured if/else and valid success fallthrough instead of generated return-of-void expression. Corrected the archived phantom `check_para_limit(0x38)` to `check_para_limit()`: the actual matched definition in src/main/8007C030.c and main_api.h is void(void). The argument had forced a second constant56 and used a1 for the record stride; removing it restores the original a0 multiplication/call reuse. Fresh typed67/67, natural and GameState pointer63/63, no phantom argument56/56, word next-state plus phase-load order52/52. Global next-state store56/54, same meaningful value reused for constant6 and later location56/54, reversed two meaningful declarations54/52. Final structured source remains52/52. Original previous-phase home is sp28/current2C; constant6 is original v0/currentt9; the next-state result and later temporary rotation differ. No unused pads or forced registers added.

func_801398E8 (268 bytes), selector.c: meaningful word selection and one byte reused across girl/previous-phase/callback ranges gives62 raw rows/58 aligned, with the selector in v1, its copy to v0 and saved byte at sp2B correct. This is not a new historical best: T-10310 had a58-row report in another context. Girl load remains currentt6 versus originala1, the two loads reorder, and later constants shift. Fresh typed75/75, natural65/65, initial GameState pointer65/65. Shared byte and signed/unsigned word selection62/58; byte lvalue94/72, two chained assignments62/58, shared GameState pointer98/76, full-word girl value with separate saved byte64/60. Final valid void-call success fallthrough retains62/58. No new FAKE workaround selected; rejected redundant byte-view probes are FAKE marked in ignored storage.

Scratch prefix.h is necessary for the previously missing overlay-local s16 D_8013C970 declaration and later-defined void func_801399F4. Initial phase controls failed loudly before adding the scratch declaration, then all were measured successfully. The prefix includes common.h once, and no unrelated declarations. These are source-only bodies, not standalone TUs or actual default C proofs.

Reproduce inside Docker after baseline unit build:

```
tools/docker.sh python3 drafts/T-24060/reproduce.py
tools/docker.sh python3 drafts/T-24060/probe.py func_801392B4 build/T-24060-probes/phase-selected.json drafts/T-24060/prefix.h SHUGAKU
tools/docker.sh python3 drafts/T-24060/probe.py func_801398E8 build/T-24060-probes/selector-selected.json drafts/T-24060/prefix.h SHUGAKU
```

The full-unit harness selects guarded C under the unchanged compiler recipe, saves traces only in ignored build/T-24060-probes, and exits nonzero if any candidate fails. Every loose source, preparation script, input and diagnostic remains preserved there; no instruction listing or binary is committed. This archive changes no interface and does not justify repeating all27 builds or claiming a compiler impossibility.
