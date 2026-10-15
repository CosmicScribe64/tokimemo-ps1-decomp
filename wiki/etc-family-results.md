---
type: research
updated: 2026-10-15
---

# ETC bounded source-family transfer

Scope: [[tickets/T-13100-etc-families]] and children [[tickets/T-13101-etc-career-family]], [[tickets/T-13102-etc-exam-family]], [[tickets/T-13103-etc-memory-card-family]], from aa2b3158 in codex/etc-13100. Original checkout read only; all compilers/analysis used existing Docker image, one CPU/compiler. Production changes limited to four ETC C files and include/ovl/ETC.h.

## Accepted evidence

Eleven functions add **5900 literal linked bytes**:

| Source file | New functions | Linked bytes |
| --- | ---: | ---: |
| src/ovl/ETC/8014ABD0.c | 3 | 2400 |
| src/ovl/ETC/801492C0.c | 4 | 1132 |
| src/ovl/ETC/80146400.c | 1 | 176 |
| src/ovl/ETC/80140F50.c | 3 | 2192 |

All eleven pass tools/funcdiff.py --resolve --version default; final ETC SHA1 check passes. SHA1: 681c93b6e57c885f86e074c885a8bf6c3cca2319. Proof: build/etc13100/final-proof.log and final-build.log. Header/global checks pass; prototype check result recorded in umbrella ticket. A function-body text audit against aa2b3158 preserves **91 pre-existing C bodies** across all touched source files (build/etc13100/body-audit.log). No new FAKE idioms, overrides, compiler changes, assembly edits or NON_MATCHING guards.

GameState pointer transfer is confirmed on four functions: 8014B1B4 improves 4 -> 0 full-TU rows; 80140FD8 improves 15 -> 0, 80141E10 and held-out 80142574 each improve 104 -> 0. All aggregate accesses use the pointer consistently. The other seven accepted functions match with direct fields; the pointer is not a universal improvement. Actual pointer arrays recover the dialogue/career table loads, and void forward declarations reflect existing callee definitions. Existing CharFlags f9/b1 accesses improve the correctness of rejected exam drafts but produce no accepted match here.

## Bounded negative results

- Career 8014A360: 26 -> 4 rows after initializing the actual selector local and assigning the final default value after its call; remaining two HI16 instructions are swapped. Width changes, assignment-in-argument and same-line scheduling probes do not match. Original assembly retained.
- Career 80149F48: original typed draft 176/166 rows; structured reload/load-order source 140 rows. Not promoted.
- Exam 80146D4C: existing EtcWork40 view 310 rows, pointer form 192. Ranking draw 80148BC8 stays 40 rows with array/CharFlags correction and bounded operand/direct-argument forms. SDK SPRT drawing 80148F7C stays 336/338 rows. The other large rendering drafts need buffer/layout recovery; guessed padding not promoted.
- Album interaction 80140AE4: 48 rows; ordinary switch 54/58. Album loader 8013FFAC: six-word texture-buffer recovery 521/467 rows. A temporary missing shared-byte declaration used to compile that failed draft was removed; main_api.h is unchanged.
- Related card initializer 80142B80: 240 rows for both aggregate forms; ordinary case stores replacing m2c shared goto worsen to 272. This Bunka idiom does not transfer here.

All rejected variants stay in ignored build/etc13100. Do not repeat these bounded source-form probes or the earlier broad default sweep without new evidence. The next ETC gains need focused loop, buffer or shared data-layout reconstruction rather than another uniform pointer rewrite.

## Census boundary

Source census after this pass: ETC 349/394 functions, 70936/107284 bytes (66.1%, baseline 65036 bytes). Whole game 5870/6958 functions, 1399964/2279368 bytes (61.4%). SDK assembly excluded. These census numbers are not a fresh 27-binary checksum claim; this lane rebuilt and verified ETC. The broad 80% objective remains ongoing.

## Review and final gates

Repository code-review skill, aa2b3158...aa1c51ec: GPT-6.1 Sol medium standards and spec reviews both pass without findings. The spec review documentation observation was resolved by recording the completed prototype guard in all four tickets: **0 new / 15 known disagreements**, branch check OK. Tickets moved to Done after both review axes and guards passed. No production edits followed review. Final branch is codex/etc-13100, ready for integration; no push or PR.

## T-14800 album dispatch follow-up

[[tickets/T-14800-etc-801447b0]] accepts func_801447B0 (648 literal linked bytes). The census direct-field draft had only a final case-4 status-byte register difference (t9 versus v0, six diagnostic rows). Using the existing GameState through one consistent local pointer across state/status/counter fields yields zero full-TU diagnostic rows, then passes explicit default linked-byte proof and the ETC SHA1. This is a focused transfer from the confirmed pointer family above; no broader sweep was needed. Original census source remains archived unchanged.
