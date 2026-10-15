---
type: research
updated: 2026-10-15
sources: ["raw/external-matching-research-2026-10-15.md"]
---

# External matching acceleration research

Ticket: [[tickets/T-11910-external-matching-research]]. Research and limited synthetic tests, not an implementation or a new game match. Main checkout and other matching workers were read-only. Recommendations are relative to [[decompile-workflow]], [[toolchain]], [[uopt-register-allocation]], [[ido-52-evaluation]] and [[matching-notes]].

## Recommendation

Start with native AS1 scheduling diagnostics and a small exact-score calibration cohort. Then evaluate the external workbench's UGEN instrumentation and widen related-function retrieval beyond exact instruction shapes. Do not replace the existing proof path, rerun the broad compiler-version hunt, or install another general permuter first.

| Priority | Proposed change | Expected benefit, not measured locally | Effort estimate |
|---|---|---|---|
| 1 | Native AS1 trace plus source-line grouping probes | Explain schedule-only failures and avoid irrelevant register searches | Half day for a small cohort |
| 2 | UGEN ownership and stack-home tracing from a pinned external workbench | Cover temporary/address/frame cases beyond current uopt traces | 1–3 days including fidelity controls |
| 3 | Exact-score calibration and a small diverse candidate frontier | Recover candidates excluded by stripped ranking and greedy search | 1–2 days for a bounded prototype |
| 4 | Edit-distance retrieval across matched functions | Find donors outside current constant-substitution families | One day for a read-only census |
| 5 | Failed-search classification and durable experiments | Stop repeating failed imports and unproductive mutation families | Half day for an initial ledger |
| 6 | SDK-specific assembler/compiler diagnosis | Separate PsyQ backend defects from source problems | Per-object, only for a demonstrated cluster |

These estimates are engineering judgments. No local throughput or matching-yield improvement was measured.

## 1. Native AS1 diagnostics before compiler surgery

The external workbench documents native `-Wa,-R,-xbbdbg,8`: `-R` exposes local scheduling DAGs, while `-xbbdbg` records cross-block motion. Its reader distinguishes accepted moves from the extra admission/rollback information that requires instrumentation. [Native AS1 motion documentation](https://github.com/akratch/n64-decomp-workbench/blob/1704b163e6d994e4fe368bbdadf4620207089ddd/docs/as1-motion.md).

**Local evidence:** two direct compiles of a synthetic integer/pointer/call function using the installed IDO 5.3 and `-EL -O2 -mips1 -G 0 -non_shared -Wo,-nokpicopt -cckr -Xcpluscomm` succeeded. Adding those diagnostic flags produced 155 log lines. Extracted `.text` was identical, SHA-256 `b688c3cbdaea2187d4224827d802f1ad14e51b5aece1515e99cc6d33b727bf3f`. This establishes flag availability and fidelity for one synthetic case, not coverage of cross-block events or the project's frame/asm-processor pipeline.

**Minimal next experiment:** select four schedule-only non-TT drafts and two matched controls from an unowned snapshot. Retain each full translation unit's preprocessed input, UGEN listing and binasm. Run stock and diagnostic AS1 on identical inputs with the existing frame/cvt/layout stages unchanged; require matching code/data and relocations before interpreting logs. Compare baseline, one-line-per-statement, grouped statements, and grouping plus an existing statement-order edit. Cap at 24 candidate compiles, one CPU. Final candidates still go through the normal whole-unit checksum and literal linked-byte proof.

The useful external lead is line assignment: token-identical regrouping can change scheduling, and grouped lines can interact with statement motion. The workbench supplies a `probe-lines` experiment with a blank-line control. Its strongest published examples are N64, including IDO 7.1; treat transfer to this K&R/MIPS I pipeline as a hypothesis. [Line probe](https://github.com/akratch/n64-decomp-workbench/blob/1704b163e6d994e4fe368bbdadf4620207089ddd/docs/line-assignment-probe.md), [field guide, levers 23/25](https://github.com/akratch/n64-decomp-workbench/blob/1704b163e6d994e4fe368bbdadf4620207089ddd/docs/field-guide.md).

Delta from existing tooling: `order_search.py` already moves statement groups and operands. Explicitly search group boundaries and combinations with statement moves; simply recommending order search again adds nothing. Preserve logical line metadata through preprocessing. A deliberate source trick still needs the project's FAKE review where applicable.

## 2. External workbench: trace UGEN and frame ownership

[N64 Decomp Workbench](https://github.com/akratch/n64-decomp-workbench) is Python, CC0-1.0, version 0.8.0 at inspected commit `1704b163e6d994e4fe368bbdadf4620207089ddd` (2026-10-08). It labels itself alpha. Comparison accepts whole objects and function selection; campaigns accept an explicit compiler wrapper. This is a plausible diagnostic companion to funcdiff, not a replacement verifier.

**Usability test:** the existing arm64 Docker image needs `tomli` for Python 3.10. Installing `tomli==2.2.1` only into a disposable container allowed both public fixtures to run. Relocation fixture: zero normalized differing words, two raw differing words. Register fixture: six register differences, mismatch exit 1; diagnostic correctly left owning pass unknown. Focused MIPS reference suite: 38 tests, 31 passed and seven game-image-dependent skips. No PS1 game object was tested.

The strongest compatibility fact is that its deep UGEN and AS1 producers target `ido-static-recomp` revision `9c242adc890beef098020149d9554f48208f699d`, the project's existing upstream pin. They additionally authenticate generated-source SHA-256 and anchors. Same upstream revision is promising, but does not establish that this image's generated files or build flags match those hashes. [UGEN operand ownership](https://github.com/akratch/n64-decomp-workbench/blob/1704b163e6d994e4fe368bbdadf4620207089ddd/docs/ugen-operand-owners.md), [AS1 predicates](https://github.com/akratch/n64-decomp-workbench/blob/1704b163e6d994e4fe368bbdadf4620207089ddd/docs/as1-motion-predicates.md).

**New capability:** correlate address emissions to observed UGEN input records and tree generations, distinguishing an upstream assigned register from UGEN's temporary choice. The stack-home producer traces selected UOPT/UGEN home conversions and refuses ambiguous rollback cases. It does not recover original C variable names or prove the target's unavailable compiler state. [Stack-home lineage](https://github.com/akratch/n64-decomp-workbench/blob/1704b163e6d994e4fe368bbdadf4620207089ddd/docs/stack-home-lineage.md).

**Minimal next experiment:** one temporary-order draft, one address-load draft, one frame-only draft, plus three matched controls. Build private instrumented copies only after source-hash admission. Compare stock, rebuilt pristine, tracing OFF and tracing ON through the full project path; preserve intermediate and final hashes. Limit to one build job and 1 GB initially. Require an actually reached trace event for each claimed capability. Never interpret an empty log as proof that the pass made no decision. Keep proprietary generated compiler source and traces ignored.

Retained-pass replay can localize which downstream stage causes a schedule difference without recompiling the frontend for each probe. Calibrate an unedited replay first. A diagnostic intermediate edit is not a matching C solution and must never become a per-function production patch. [Pass replay](https://github.com/akratch/n64-decomp-workbench/blob/1704b163e6d994e4fe368bbdadf4620207089ddd/docs/pass-replay.md).

The external compiler-law corpus explicitly limits its main observations to IDO 5.3 `-mips2` and a small set of N64 campaigns. Use its integer-ring, frame and operand-order hypotheses to design controlled probes; do not import floating-point behavior, fixed register-pool boundaries or universal frame formulas into PS1. [Scope and evidence tiers](https://github.com/akratch/n64-decomp-workbench/blob/1704b163e6d994e4fe368bbdadf4620207089ddd/docs/compiler-laws/ido-5.3.md).

## 3. Calibrate search against the full file; retain diversity

Local code inspection: `autotweak.py` uses greedy improvement with exact rescoring of selected candidates; `order_search.py` switches to a hill climb above its exhaustive budget. The current exact-top/threshold mechanism is valuable, but cannot prove that candidates excluded by stripped scores are unpromising. No claim here that this existing feature is missing.

**Proposal:** freeze 12 historical non-TT seeds, generate at most 32 plausible candidates per seed, and score every one both stripped and full-TU. Record rank inversions, exact winners excluded by the current policy, errors separately from bad scores, and wall time. Include source-line grouping, statement order, declaration order and a related-function seed. Compare the current policy with a four-member frontier preserving distinct emitted-code/trace shapes, including equal-score variants. Require a fixed compile budget and held-out seeds. This is an experiment, not evidence that beam search will improve yield.

For caching, include the preprocessed entire TU, includes/macros, guard selection, compiler and pass hashes, options, original object and linker/symbol/layout identity. Hashing only the target body is insufficient across sessions. Reuse the existing exact scorer and verifier. A stripped score of zero remains provisional; even a full-TU function score of zero needs linked proof.

External support is methodological: the BED paper combines source fragments and a population of candidates under compile-and-compare feedback. Its reported 10/19 small-program results are not a benchmark for PS1 or a maintained IDO solver. [Evolving Exact Decompilation](https://www.grammatech.com/wp-content/uploads/2023/12/Evolving-Exact-Decompilation-2.pdf). The MIT `cpp_permuter` provides concrete full-file splicing, checkpointing, composite transformations and equal-score retention, but targets MSVC C++; borrow design ideas rather than port its parser or adopt it as a MIPS replacement. [Repository](https://github.com/CriminalRETeam/cpp_permuter).

## 4. Extend donor retrieval, not the acceptance normalizer

Local `neardupes.py` requires the same instruction shape and register choices, masking selected immediates. It cannot retrieve a donor differing by an inserted statement, changed branch shape or register web. This is a real delta beyond the existing dupes/neardupes pipeline.

BFM's retrospective reports 22/352 reachable targets with exact hashing versus 75/352 using an edit-distance band, including relocation-only omissions. Those are upstream self-reported counts, not reproduced here. It also reports diminishing returns and recommends ranking by adaptation effort. Its tool/docs license is AGPL-3.0; do not copy code into this CC0 repository without preserving compatible licensing. [Method and counts](https://github.com/Druthulu/BFM-decomp/blob/main/docs/generic-decomp-package.md), [license boundaries](https://github.com/Druthulu/BFM-decomp/blob/main/THIRD_PARTY.md).

**Minimal next experiment:** a read-only donor census on 100 open functions. Use length bounds and bounded edit distance on decoded instructions; retain a separate relocation-aware channel and optionally a register-renamed retrieval channel. Inspect only the top 20 new pairs. Report overlap with current neardupes, new plausible donors, misleading boilerplate pairs and expected edit regions. Require every old exact pair to remain discoverable. Use similarity only to retrieve examples; the copied C must respect symbols, types, aggregate offsets and the exact build. Stop if the added candidates are mostly boilerplate. No automatic acceptance through masked comparisons.

## 5. Learn from failed searches

BFM's primary writeup classifies plateaus into missing transformations, routing mistakes, structurally bad seeds and scoped compiler obstacles. This suggests turning the existing farm output into evidence before adding more CPU. [Bootstrap retrospective](https://github.com/Druthulu/BFM-decomp/blob/main/docs/how-to-ai-decomp/03-bootstrap-order.md).

**Minimal next experiment:** classify 30 completed local runs using compiler errors, distinct emitted hashes, stripped/full-score disagreement, residual type and attempted mutation families. Measure whether the seed compiles through the real full file before allocating search time. An import failure is not a failed matching hypothesis. Repeated identical emitted bytes should reduce that family's budget; a new trace shape may justify keeping a tie. Distinguish exhausted tested variations from an impossible function. This can be implemented independently using current tools and an append-only JSONL ledger.

## 6. Keep PsyQ work separate

The existing [[gcc-objects]] and SDK signatures already address compiler identification. New evidence should trigger a small per-object audit, not a universal flag change. `maspsx` is an ASPSX compatibility transformation, not an IDO scheduling fix. Upstream requests minimal compiler/assembler reproductions for mismatches. [maspsx](https://github.com/mkst/maspsx).

A useful external warning: the Forbidden Memories project documents an extern-small-data load-delay mismatch between maspsx and GNU as with nonzero `-G`. This does not establish a problem in this project's `-G0` IDO game lane. [Project report](https://github.com/gonzaloberteri/ygofm-decomp). Test such a hypothesis only on an identified SDK object and its actual flags. Compile a synthetic narrow extern load/use sequence with the installed pinned pipeline and compare an authentic toolchain only if already available privately; never install or distribute SDK archives merely to speculate.

## Options deliberately not adopted

- Another IDO release: [[ido-52-evaluation]] already measured 5.2/5.3 equivalence and 4.1-UGEN regressions. External frontend fingerprints are a narrow follow-up only when object-level evidence demands one.
- `n64decomp/ido` as a production compiler: its debugger targets decompiled uopt, mainly 7.1, and documents 32-bit x86 correctness restrictions. Existing native traced uopt is more suitable here. [Upstream README](https://github.com/n64decomp/ido).
- Generic learned decompilers or symbolic equivalence engines: no inspected source demonstrated a maintained drop-in IDO 5.3/MIPS I exact pipeline. Semantic equivalence does not solve original register/schedule/layout identity.
- Additional asm viewers or distributed workers: existing objdiff/funcdiff and the farm already cover those needs. Shared capacity is constrained; classify and calibrate before expanding work.

## Delivery and verification boundary

All tests used existing Docker images, one CPU, at most 768 MB, and synthetic/public fixtures. No host dependencies, production tool changes, game edits, pushes, merges, resets or stashes. Research is retained in its own worktree. Ticket stays In Review pending the project's independent review gate. The next implementer should start with priority 1 and evaluate priority 3 on the same frozen cohort, then choose deeper instrumentation only for unresolved ownership.
