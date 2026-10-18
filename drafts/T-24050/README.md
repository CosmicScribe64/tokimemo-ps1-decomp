# T-24050 GEKO selection checkpoint

No new C match. Production src/ovl/GEKO/80134040.c and shared headers are unchanged. The current sibling census has no matched relative at similarity0.70 or higher for this target.

The T-17700 typed draft recorded136 edits. New natural C with the real word CharFlags.b1 test (rather than an incorrect byte-mask access), narrowed candidate state and ordinary for-loop scores103. Writing the flag access from the record array's own byte base avoids promotion of the shared GameState base and scores80. That source spelling is explicitly FAKE in the archive. It is still a broad mismatch, not a promoted production draft.

The target keeps next=i+1 in a saved register across getter calls, and uses a shared selection-update block. An explicit next-index/join form scores109, or102 with the separate array-base spelling. Historical control flow plus the word flag scores132. Combining current types and the old-style shared join scores88. Evidence-backed u8 versus u32 best-state views stay80/88 and are exhausted. No random register/statement permutation sweep ran.

Remaining blockers: induction and best-index copy lifetimes, selection-update CFG, and saved-register priorities differ. The original duplicate tie test is retained and its unknown rationale documented. The record-array address form fixes the unwanted base promotion but does not solve these other differences. Avoid repeating these forms without new evidence.

Reproduce with tools/docker.sh python3 drafts/T-24050/reproduce.py. This tested script writes only build/T24050/replay.c and invokes one normal compiler via viewsweep, budget1. It refuses if the production assembly fallback is absent or ambiguous. The expected current aligned cost is80. Normal unguarded default literal proof plus full GEKO checksum would be required for any later C claim. No assembly listings or raw assets are archived.
