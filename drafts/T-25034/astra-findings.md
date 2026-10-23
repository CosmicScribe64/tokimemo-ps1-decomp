# T-25034 Astra wrap-up

Stopped at the user's explicit wrap-up request. No production source, shared header, build rule, compiler or wiki edits by this worker. No commits or pushes. No workers spawned.

## Preserved candidates

- Entry checkpoint: astra-entry-best/body.c is the pre-experiment direct-low-channel source and all its original evidence, preserved before experiments: target674/built671 padded words, aligned367/printed389.
- Conservative improved checkpoint: astra-entry-order/body.c and full.c, guarded.o, diff.txt, target.txt, built.txt: target674/built671, aligned361/printed381. Reorders independent entry computations so texture_v/order_offset precede alpha/quadrant. FrameA8 is exact; alpha58/order_offset5C now match, texture_v60 vs target64 and order6C vs target7C remain.
- Latest lowest-cost checkpoint: astra-forward-uv-join/body.c and duplicated immutable astra-final-best/body.c, with full.c/guarded.o/diff.txt/target.txt/built.txt: target674/built670, aligned360/printed380. Based on entry-order; moves the existing shared final-u0 label from case0 to case3, so all explicit UV joins go forward. C semantics unchanged, but target repeats u0 stores in branch delay slots whereas this output still shares more tail. This is not a structural closure or match.
- Natural independent case breaks: astra-natural-case-breaks/body.c:674/673,411/429. Reproduces the forward break shape better but changes texture/counter/constant saved-register priorities. Preserve only as a diagnostic alternative if desired.

Padded target674 words includes3 trailing alignment NOPs; active target body671 words. Root independently verified these spans. Latest best candidate padded670/body667 is still16 bytes short of the target span. No raw artifact belongs in Git.

## Findings

The handoff frameB0 warning was stale: direct-low-channel, entry-order and forward-join actually have frameA8 and correct incoming argument homesB8/BC/C0/C4. All accepted candidates preserve the full-s32 TILE extent branch repair.

Color conversion reconstruction remains unresolved. Faithful source experiments show that unsigned masks/casts often retain the ANDI but produce sra into fp, ANDI into a temporary, then copy back to fp; target is sra into a temporary and ANDI directly into fp. Signed-byte controls retain matching register ranks but emit unwanted sign extension. Neither lower score nor low-byte-equivalent colors is a literal match.

A private normal-pipeline regdiff trace of astra-word-mask-tail shows the shared color expression as an outer CVT over UAND, and converted green/blue expressions with priority50. This motivates conversion/lifetime work, but no compiler defect or required tool change has been established. Trace output excludes trailing padding in its own headline, so use whole-TU run_one metrics above.

## Reproduction

Existing Docker image and complete translation unit:
tools/docker.sh python3 build/T-25034/run_one.py replay-final build/T-25034/astra-final-best/body.c

run_one.py uses funcdiff_verify.compile_unit and resolved target/built words, preserving the entire current translation unit and replacing only func_80046AC8's INCLUDE_ASM. The completed folders hold frozen full.c, so root may replay that source directly if later unrelated production changes would otherwise alter the reconstructed TU.

## Completed controls (target674; built/aligned/printed)

astra-unsigned-channels670/772/780
astra-word-unsigned-tail670/772/780
astra-word-half-tail654/830/864
astra-word-mask-tail670/772/780
astra-channel-assign668/750/752
astra-channel-unsigned-mask668/750/752
astra-direct-red670/772/780
astra-word-byte-cvt648/826/894
astra-unsigned-mask-literal670/782/790
astra-unsigned-mask-plain-rgb670/870/970
astra-byte-conversions662/868/898
astra-signed-word-byte644/824/878
astra-texture-param-end654/776/844
astra-scaled-row-order671/485/501
astra-entry-order671/361/381
astra-signed-color-view670/788/878
astra-cast-before-mask670/780/788
astra-u32-red-assignment670/780/788
astra-natural-case-breaks673/411/429
astra-assignment-byte-result664/932/956
astra-forward-uv-join670/360/380
astra-packed-unsigned670/780/788

All were private source controls. No candidate passed literal matching, no production promotion, no new coverage credit, and no full executable gate was run by this worker.
