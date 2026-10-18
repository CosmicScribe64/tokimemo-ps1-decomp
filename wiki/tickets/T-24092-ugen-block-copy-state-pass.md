---
id: T-24092
title: Prototype and evaluate ugen block-copy state (F vs B) uniform pass
status: In Progress
priority: high
type: tooling
created: 2026-10-18
updated: 2026-10-18
---

## Summary

Investigate and prototype a uniform fix for the IDO 5.3 `ugen` block-copy state (`F` vs `B`) difference documented in `[[build-step-gaps]]`, `[[ido-52-evaluation]]`, `[[tickets/T-20710-remaining-build-step-gaps]]`, and `[[tickets/T-24085-event-geko-twins-b]]` (reproducers in `tools/ido_eval_cases/ugen_copy_state/`).

Specifically:
1. Reverse-engineer how IDO 5.3 `ugen` (`/opt/ido/5.3/usr/lib/ugen`) and IDO 4.1 `ugen` (`/opt/ido/4.1/usr/lib/ugen`) handle `Umov` / struct block-copy emission and why IDO 5.3 switches a struct copy to state `F` at a basic-block boundary or resets to state `B` after a direct global store.
2. Prototype either a surgical binary patch to `ugen` (like `tools/uopt_patch.py`) or a binasm/ucode pass, and test it in `build/scratch/t24092/` against:
   - The 6 reproducers in `tools/ido_eval_cases/ugen_copy_state/` (`DATE:func_801518B8`, `SHUGAKU:func_801357AC`, `SHUGAKU:func_80135EA4`, `TACO:func_801472D4`, `main:func_80054C74`, `GEKO:func_801406CC` / `EVENT:func_80109404`).
   - All 35 matched functions with F-state struct copies (must not regress any currently matched function per `CODING_STANDARDS.md` section 7a).
3. If the pass satisfies the section 7a adoption rule (uniform, evidenced, zero regressions on matched functions, unit-tested), prepare it for integration and test closing the unblocked functions.
