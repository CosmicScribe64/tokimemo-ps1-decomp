# T-24521 runner display variants

These bodies compile but do not match; production remains INCLUDE_ASM. Substitute one at the original INCLUDE_ASM in `src/ovl/TAIIKU/80133C80.c` at commit `263294a8f`, then run `tools/funcdiff.py --resolve TAIIKU:func_80137C44` in Docker with default IDO 5.3 flags.

The typed reconstruction fixes the archived pointer increments to advance by runner and actor records. Comparisons report 241 diagnostic rows for `typed-for`, 229 for `typed-do`, and 223 for `typed-snapshots`. Counts exclude the two heading lines and final summary. Each probe compiled and returned a real DIFF.

The original hoists mode, selection, player phase and rival globals. These drafts reload them; uopt sinks explicit snapshot locals back into the loop. The original frame is 32 bytes with three saved registers; the drafts use 24 bytes and one saved register. They do not improve T-19004's roughly 100-row result. No compiler change was tested. Session logs remain at `/private/tmp/t24521-37C44*.log`.
