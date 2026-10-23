---
type: archive
---

# Matching notes, part 8

Sections moved verbatim from [[matching-notes]] by `tools/wiki.py rotate-notes`, oldest first. Find a function in [[notes/functions]] and a section in [[notes/sections]].

## T-24501/T-24570 metric aggregate escalation (2026-10-20)

After Luna's union-word view negative, one fresh full-TU true `GsRec0FC` assignment probe for `func_80054E6C` gives105/90 padded words versus baseline84/87; frame stays40, saves18/1C. Record alignment2 introduces halfword copy operations instead of target word copy. Target80-frame has only saved18/1C and CSE30/34 SP accesses, no surviving local-buffer or incoming-argument use; three memcpy arguments do not explain64 unused bytes. No evidence-backed array/argument structure recovered, no fabricated padding or compiler-defect claim. Production/header unchanged. Exact focused replay and source uncertainties: `drafts/T-24501/metric_aggregate.py`, `drafts/T-24501/main/func_80054E6C-notes.md`.


### T-24501 text substitution reconstruction, ongoing (2026-10-20)

`sndi` (1,052 bytes) scratch reconstruction uses the observed64B input, three40B stage/quote/result intervals and20B name interval. Exact capacities other than64 are inferred from adjacent target homes, not yet proved original declarations. Corrected m2c's lost sequencing at the first substitution join: test previous found flag before resetting it for the G scan. Natural clean292; explicit cursor/parity scan with repeated arg2&15 expression278/259words (target263). Moving the real index scalar before buffers reproduces frame118 and all buffer addresses, gives210/259. Branch-specific index resets before final P suffix append and failed-scan copy give211/260, preserving target reset timing more closely. Both sources retained privately. Signed zero-test view273 rejected because target LBU becomes LB; unsigned index/modulo and guarded-do leave278. Separate cursor/parity locals for three scans give310/259 and grow frame128. Remaining induction/cursor priority and found/index spill joins need actual source recovery; no production changes.


T-24501 `sndi` induction packet frozen: best210/259words/frame118 and branch-reset211/260, target263. Two-base parameter scan216/265 and indexed235/262 do not remove second cursor IV; last approved branch-local increments303/262/frame120 also fail. All target character accesses remain LBU; signed condition view is rejected. Exact source-only primary/alternate and bounded negatives under `drafts/T-24501/main/sndi*`, replay via `drafts/T-24501/replay.py`. Correct found-flag semantics and observed buffer homes preserved; capacities beyond input64 explicitly remain inferred. No production/header edit. Focused consultation question is source/address control preventing cursor strength reduction, not a blanket backend claim.

T-24501 `sndi` continuation stops at150/265words, unverified. Astra integer-address cursor restores recomputation; existing-local ordering gives183/262 with exact index40/found3C and buffers/frame118. Byte parity150 adds three old-index MOVEs; trace identifies parity20 versus cursor11 priorities and base7.333 versus constant6.667. Current source-only primary archived, no production/header change. User-directed stop prevents further spelling variants; diagnostic scores are not verified-byte gains.


### T-24501 save-image loader aggregate recovery (2026-10-20)

`func_800552DC` (672 bytes) replaces the inherited scalarized copy draft with two whole record assignments. Target accesses prove Work+0x2C..0x3F is a 20-byte object (including its last-byte protection flag), Work+0x40..0x57 is a 24-byte object preserving existing bits/names/three words, and their serialized bank positions are 0x28/0x3C. `SaveImageView` documents the 0xF00 bank layout, including byte-aligned 0x800 records and aligned metrics at 0xA98; view types live in game.h without changing any canonical Work fields or externs. The failure path returns -1; the original success path leaves the final call's return register intact, so no success return was invented.

Typed pointer arithmetic gives 110 edit cost, 168 target/candidate words. Merely grouping the pointer offset is identical. Integer-address sum `(selector*0xF00+0x200)+(u32)D_80123120` prevents pointer reassociation of 0x200, restores the target's bank calculation and every subsequent temporary number: resolved 0, 168 words. The integer expression is explicitly FAKE T-24501. Default-C complete-TU private compilation/link proves all 672 literal bytes equal target; headers, prototypes and migrated globals pass. Parent reviewed source/header against CODING_STANDARDS with no findings. The inverse `func_80054FC0` is a conditional follow-up using the same verified record boundaries; no compiler change or padding grid.

Affected MAIN rebuild `ninja -j2 build/SLPM_86.053.ok` passes its immutable target SHA1 after header promotion. Parent will independently repeat full 27-unit integration verification.
