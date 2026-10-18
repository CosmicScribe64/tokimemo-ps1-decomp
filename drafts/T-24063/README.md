# T-24063 TACO short count pass

Zero of three matched; no new functions/bytes. Production C, existing guards, shared headers, config and compiler are unchanged. All three assigned functions remain guarded, default ASM. Existing one-job TACO baseline build passes. No fallback comparison is counted as actual C progress.

Read TACO family, T-19011, T-16563, T-19015/T-19026/T-22025, keep-lines and ido-52-evaluation before controls. All three have no matched sibling >=0.70 in the current census. Existing claims are disjoint, confirmed by parent. Five source measurements only; no repeated type/grid/permuter or compiler-option search.

func_8013506C (240 bytes): current guarded control29/29. Its array expression indexes128+i from GameState's128-word unk_163C, beyond that declared array. The target reads128 packed coordinate words in the following512-byte unk_183C buffer. random.c uses the actual member through an inline u32 word view. This corrects the source's array boundary without a shared-header change and compiles identically29/29. Moving x/y extraction into the nonzero conditional also29/29. Selected source retains the established loop/statement shape and valid word-buffer view, no FAKE workaround. Original first remainder lives in a3 and is saved/reloaded around the second RNG call at sp3C; current code keeps it in s1, making frame38 versus48. The later x/v ranges are swapped. T-19011 already measured failed reuse of one variable across both loops because loop2 becomes an independent CSE range; that failed route was not repeated. keep-lines already proves separate scalar views do not change this function, so no migration exception was proposed.

func_8015B1B8 (244 bytes): one current-control compile16 raw/16 aligned. This is the same eight changed instructions described historically as an8-row miss: the copy load/store register is currentt2 versus originalt3. No regression or new source lead. func_801472D4 (300 bytes): one current control20/20, the same copy register currentt7 versus originalt8 and corresponding flag-address temporary. Both guards already use the real SDK MATRIX and shared object view with correct frames. Prior measured compiler comparisons show 5.2/5.3 and4.1 differ in this post-loop copy-state behavior; this is evidence about the current blocker, not a request for per-function compiler exceptions or an absolute impossibility claim. Existing controls are preserved in ignored build storage instead of copying unchanged guarded bodies into the archive. No further copy variants were justified.

All loose C, JSON input, preparation scripts and instruction diagnostics remain preserved in ignored build/T-24063-probes. No instruction listing or binary is committed. The only new body is the corrected random.c source lead; historical drafts and production remain untouched.

Reproduce inside Docker after `tools/docker.sh ninja -j1 build/ovl/TACO.ok`:

```
tools/docker.sh python3 drafts/T-24063/reproduce.py
tools/docker.sh python3 drafts/T-24063/probe.py func_8013506C build/T-24063-probes/random-selected.json
```

The harness inserts the supplied body into a temporary full unit, selects guarded C under the unchanged compiler recipe, compares resolved instructions and writes traces only to ignored build storage. Candidate failures retain diagnostics and exit nonzero. A candidate score is not a default linked actual C proof. Source-only archive closure changes no interface and does not require repeating all27 builds.
