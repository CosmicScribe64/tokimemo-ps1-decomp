# T-24522 RPG_BAT source reconstruction

Two exact default C matches, 2,048 bytes total:

- `func_8014BB80`, 1,124 bytes, commit `bf7e243bd`: typed GameState pointer reads in the two choice switches split the condition's plain-field range. The actual pointer replaces the prior fake frame home. Removes the volatile read and the fake padding.
- `func_801447E0`, 924 bytes, commit `66f4bbe19`: make D_8015EBB0 the arithmetic variable, with explicit multiply/divide stages. Declare real locals k before lim. No new fake, header override or compiler change.

Both passed Docker private default linked literal-byte proof and private RPG_BAT checksum. All 27 hashes passed using that private output and 26 unchanged read-only reference outputs; this does not claim rebuilding all 27. Headers, global migration, prototype branch checks pass (0 new, 15 known). Worker and independent coordinator standards reviews passed.

## Useful unresolved source

`RPG_BAT/func_80144C84/body.c` is source only, 28 literal rows / 28 aligned in the full current translation unit. Production stays assembly. Starting T-17113 draft252/224. Direct output-global updates138/134; typed first-stat pointer92/88; frame home82/78; staged output-global arithmetic44/44; multiplication order40/40; sum operand wrappers30/30; matched sibling's comma-return recipe28/28. Every fake home/wrapper/comma-return is documented in the archived source.

The remaining third-arm field08/EBE0 reload and store scheduling differ. The build speculates EBE0's decrement before the nested enemy-kind branch and retains field08 in a register, whereas the target reloads both fields after that branch. Natural pointer threshold/clamp forms, smaller trailing-record pointer, field writebacks, distinct clamp local, switch/while/else spellings, signed casts and field alias casts did not close the gap. The reversed conditional46/38 worsens branch shape. Do not rerun these as a broad grid. No compiler-limitation claim is established.

`func_8014BFE4`: current T-17110 baseline128/128. A typed actor-base pointer207/201; address-taking update of argument x and the third comparison185/185; combined247/247. All worse, not promoted. The prior argument stack-home and loop-strength-reduction gaps remain.

`func_80154DD8` and `func_8015528C`: current recent T-24027 source-only drafts and concrete negatives were read. They remain broad state-machine/interpolation gaps (326/288 and302/246 in T-24027). No fresh hypothesis justified rerunning those measurements.

All experiments used the existing Docker arm64 image and one compiler process. Loose probe scripts, JSON candidate sets, full source candidates and diagnostics are preserved in ignored `build/T-24522/`; no generated target asm, binary, copyrighted data or production compiler change is committed.

## Bounded threshold-snapshot follow-up

After coordinator review, a read-only dependency map motivated one new structural hypothesis: capture the third threshold's EBE0/field08 inputs in a real two-word local aggregate, then mutate plain globals. Three meaningful forms were compiled at j1: third-threshold-only capture; refresh at both initial bound and third-threshold phase boundaries; reversed member/assignment order. All score34/34. Every form retains the exact original28-row code gap and adds6 limit-spill-offset rows(0x2C to0x28); the aggregate snapshot is optimized away. No new source form was promoted. Inputs and diagnostics remain ignored under `build/T-24522/forms55.json` and `55_*.c`/`55_*.diff`.

The minimal remaining dependency blocker is now explicit: target reloads field08 and EBE0 on the noncall successor of the nested enemy-kind branch; current C carries the threshold registers forward, moves extra-minus20 into the branch slot and spills field08 along the call path. The earlier initial-bound EBE0 load also moves relative to counter writebacks. Ordinary pointer forms, branch spellings and the explicit snapshot do not reproduce the target dependency. This is evidence about tested source forms, not proof that no C source exists.

## Fresh donor pass after both matches integrated

Compared only the two new donor originals with the four remaining target originals. The score uses `tools/siblings.py` opcode normalization (registers/immediates/shift amounts/branch and jump targets erased), donor as SequenceMatcher sequence A, target as B, `autojunk=False`. No whole-tree search or compiler run.

| Rank | Donor | Target | Opcode ratio | Registers retained ratio |
| --- | --- | --- | ---: | ---: |
| 1 | func_801447E0 | func_80144C84 | 0.280000 | 0.163636 |
| 2 | func_8014BB80 | func_80144C84 | 0.240000 | 0.080000 |
| 3 | func_8014BB80 | func_8014BFE4 | 0.222222 | 0.059524 |
| 4 | func_801447E0 | func_8015528C | 0.207921 | 0.118812 |
| 5 | func_8014BB80 | func_80154DD8 | 0.206186 | 0.106529 |
| 6 | func_801447E0 | func_8014BFE4 | 0.185022 | 0.079295 |
| 7 | func_8014BB80 | func_8015528C | 0.173780 | 0.070122 |
| 8 | func_801447E0 | func_80154DD8 | 0.157895 | 0.101504 |

Source-template inspection of the best donor for each target:

- 447E0 to C84: the longest original opcode block is15 words of multiply/divide arithmetic and final result storage. The useful staged-global transfer is already present in the28-row C84 body. The residual dependency gap occurs elsewhere; do not repeat this transfer.
- BB80 to BFE4: the longest block is6 words of generic comparison-switch dispatch. BFE4's selector is an input parameter; there is no switch/condition/switch GameState-field lifetime matching the donor's successful pointer alias split. Its stack-home and digit-loop gaps are different.
- BB80 to DD8: the longest block is7 words of call setup and a state load. DD8's inner EBAC selector is used only once, unlike the donor's repeated choice field around intervening calls. Outer state conditions are ordinary direct accesses in the donor itself. No supported pointer-template transplant follows from this block.
- 447E0 to5528C: the longest block is8 words of multiplication/division and HI/LO hazard spacing. The target computes separate interpolation arguments rather than a shared output global. Borrowing staged work-global mutations would affect callback-visible state and is not the donor's semantics.

No genuinely new structural source transplant was identified. Loose eight-pair scores and decoded top-block diagnostics remain in ignored `build/T-24522/donor-ratios.json` and donor scripts. No compiler-limit conclusion is established.

## Continued structural reconstruction: DD8 and5528C

Coordinator requested continued work after the donor pass. No production C or shared header changed. Read T-24027/T-17106/T-17114 first; all probes used one Docker compiler at a time.

DD8 ownership: EC68 is the existing RpgRec18 D_EC58.unk10, already accessed canonically by matched801321D0.c. Saved `RPG_BAT/func_80154DD8/body.c` retains this canonical duration view at326/288, same as current baseline. A typed path pointer also326/288; replacing another fake frame home with a real shared quotient331/293 adds8 frame bytes and does not preserve the desired quotient lifetime. An explicit original-derived shared sound-call label graph also326/288, identical to the canonical baseline. No no-op reference or predicate grid was repeated.

5528C source-only body is now `RPG_BAT/func_8015528C/body.c`:274 literal /206 aligned. Its preserved text was recompiled after replacing decompiler variable names with descriptive local names; metric remains274/206. It is not production coverage.

| Source hypothesis | Literal rows | Aligned rows | Observed result |
| --- | ---: | ---: | --- |
| T-24027 current baseline | 302 | 246 | Modulo factored after bounds and clamp |
| Shared quotient/start accumulator | 301 | 245 | Minimal scalar-lifetime improvement |
| Direct lower-bound product, remove redundant temporary | 298 | 242 | Small improvement, same modulo factoring |
| Independent negative-start clamps in both arms | 368 | 268 | Still factors modulo; source goto sharing alone insufficient |
| First-arm s8 remainder, other arm plain | 408 | 246 | Restores both modulo blocks and saved-register ownership; retains sll24/sra24/move |
| Conditionally approved s16 counterpart | 408 | 246 | Same causal result; real16-bit extension remains |
| First-arm remainder-times-one, other arm plain | 274 | 206 | Restores both modulo blocks without real extensions;375 words as target |
| Reuse start variable for initial raw progress | 412 | 392 | Worsens substantially; rejected |

The modulo domain[-2,2] proves both narrow conversions preserve semantics, but the compiler retains their extensions. The single multiply-by-one wrapper is an existing project idiom and is explicitly FAKE: it distinguishes the two source expressions long enough to keep modulo in both upper-bound arms, yet generates no real multiplication. This restores original ownership of divisor3(s5), stride(s6), phase pointer(s7) and progress pointer(fp), as well as callback-case block lengths and dispatch offsets. Remaining differences include initial work-base/raw-progress registers, operand choices, and color/call scheduling. No exact match or compiler-limit conclusion.

The work-array view remains explicitly FAKE because the global arena's real complete type is unresolved. Known callers use progress/phase pairs at work indexes50/51 and52/53, but the helper's interface takes separate indexes; no unproven adjacency assumption was introduced. Inputs, full candidates and diagnostics remain in ignored `build/T-24522/` (`dd8_*`, `dd9_*`, `p528_*` through `p533_*`).

## Current helper checkpoint:53 rows

`RPG_BAT/func_8015528C/body.c` now preserves the best53 literal/53 aligned source-only form; previous274/206 causal checkpoint is retained as `modulo-only.c`. Remove the named phase-decrement temporary and directly update/read-test actual `*phase`. This eliminates the competing uopt range and recovers original initial base(v1), raw value(v0), and ugen phase decrement(t4/t6). The helper is376 words versus original375. No production guard or coverage was added.

First stock/tracer comparison used the existing coordinator-provided trace binary read-only, no compiler rebuild/patch. The274-row candidate was exactly1500 bytes with identical resolved instruction/relocation keys, SHA2565fc82f9a125b0855aea6790e2c29e1e6d3925e55c34c0c680e202e41c2118a29. Initial work-base priority0.5, raw-progress1.5, competing named phase-decrement3 led directly to the53-row result.

A fresh stock/tracer comparison for the best53 source also passes exact function bytes and resolved keys:1504 bytes, SHA256ffb6038d8aa8ab017d1681a1a9960a13bed8ae79a51dee653f146ff7736acf0b. Its first allocation mismatch is now loop load59: start range bit39 has priority45 and is constrained/precolored a0; target holds quotient in v0 before start a0. Next expression range bit52 has priority30 and occupies v0. First structural mismatch is extra move at instruction74. Initial pointer adds also have reversed operands. Ignored trace evidence is in `build/T-24522/trace536/first-report.txt`; no full instruction listing is committed.

Additional causal tests, all negative:

- Post-join direct global phase initializer274/206, byte-for-byte identical diagnostic to the274 checkpoint; no base lifetime change.
- New remainder identity on original separate quotient/start baseline447/231.
- Restored original quotient+product locals with direct phase update359/227.
- Explicit initial work-plus-index pointer spelling53/53, operands unchanged.
- Share initial raw value with distinct loop quotient369/225: incorrectly couples both into v1 and moves initial base to v0.
- Add only a genuine loop quotient local359/215: still wrong quotient ownership and callback schedule.

No unsigned initial-read view was run: trace proved initial raw progress and loop quotient were not originally the same source web in this draft, so the proposed view split lacked its prerequisite. No declaration-order, padding, type or reference grid followed these results.

## Two new MAIN donors, eight original comparisons

Same normalized-opcode method as the earlier donor pass, only MAIN76000 and767A4 against the four remaining RPG_BAT originals; no compiler search.

| Rank | MAIN donor | RPG_BAT target | Opcode ratio | Registers retained ratio |
| --- | --- | --- | ---: | ---: |
| 1 | func_800767A4 | func_80154DD8 | 0.265918 | 0.131086 |
| 2 | func_800767A4 | func_8014BFE4 | 0.206140 | 0.092105 |
| 3 | func_800767A4 | func_80144C84 | 0.195652 | 0.119565 |
| 4 | func_80076000 | func_8014BFE4 | 0.176245 | 0.057471 |
| 5 | func_80076000 | func_80154DD8 | 0.150000 | 0.113333 |
| 6 | func_800767A4 | func_8015528C | 0.141447 | 0.128289 |
| 7 | func_80076000 | func_80144C84 | 0.113269 | 0.100324 |
| 8 | func_80076000 | func_8015528C | 0.097923 | 0.068249 |

Every target's best donor block is generic signed-division trap handling plus nearby call/register setup (10-11 words), rather than shared game control or data structure. MAIN76000's current source was inspected read-only: its u16 packed icon field and signed menu-selector lifetime do not correspond to any of the four remaining target blockers. MAIN767A4 source was still INCLUDE_ASM in root at8c52b2b0a during this pass; final source-template comparison waits for its pin. No new structural transplant justified a compiler run from these donor scores. Exact scores and decoded block diagnostics remain ignored in `build/T-24522/main-donor-ratios.json` and donor script output.

## Current helper checkpoint: two pointer operand swaps

The main source-only body now scores4 literal/4 aligned, exactly375 words; previous53-row ownership checkpoint is ownership-53.c. First, repeating actual progress/10 expressions instead of naming the quotient gives64/32 and exact375-word length. Independent end-candidate computation before phase-sum load gives46/10. Finally, compute each arm's modulo before assigning end and sentinel100:4/4. All373 other instructions match. The two residual instructions are pointer additions at rows6 and42: original base then scaled index, current scaled index then base. Callback cases, divisions, traps, loop latch and tail are exact. No production guard or new coverage.

Approved arg6-times-one and initial arg7-times-one wrappers (postjoin phase access stays plain) regress230/210, changing initial base v1 to t0, raw progress v0 to v1 and saved-pointer allocation. They do not fix operand order. No identity-operation grid followed; byte-addressing is a proposed, uncompiled distinct addressing form. Source-only FAKE arena view and first-arm remainder identity remain explicit.

The two subsequently authorized addressing forms also fail: byte-offset indexing through u8* gives230/210 (same initial owner shift), while32-bit scalar integer-address addition gives373/229. Both replace only the initial progress and correction-arm phase address forms, leaving postjoin phase indexing plain. The4-row source remains the best; no further address/identity variants were run. No toolchain-limit conclusion follows.
