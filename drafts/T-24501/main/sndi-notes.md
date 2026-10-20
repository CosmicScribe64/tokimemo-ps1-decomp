# T-24501 sndi substitution/induction checkpoint

Production remains ASM. Docker full-TU replay:

```sh
python3 drafts/T-24501/replay.py sndi
python3 drafts/T-24501/replay.py sndi drafts/T-24501/main/sndi-branch-reset.c
```

Expected resolved cost210,259 padded words for primary body; branch-reset211,260 words; target263 words/1052B. Each replay writes private full-TU source, object, resolved.diff and target/got text under build/scratch. No literal match claimed.

Recovered semantics: mode low nibble15 performs P then G substitution; other modes only P. Only even byte positions are placeholders. The first m2c join incorrectly discarded found before testing it: target loads old found, tests zero, then clears found for second scan. Correct source tests old found before resetting. Preserve this behavior even if a lower-cost draft suggests otherwise.

Target buffer bases: inputD4 (strncpy64 proves capacity at least64), quoteAC, stage84, result5C, name48; frame118. Adjacent intervals suggest40/40/40/20 capacities, but their exact original declarations remain uncertain and are FAKE-marked. Moving the real index scalar before arrays reproduces all five bases/frame without padding. Primary index reset is at first-stage join; alternate moves reset into found-P before final suffix append and failed-P before fallback copy, following target spill40 timing more closely. Both are semantically equivalent with nonescaping index.

Narrow induction question for consult: target first scan keeps indexv0, recomputes cursorv1=base+index at entry/latch (basea3/indexv0), reads LBU, masks old index parity and increments index. Target also retains duplicate buffer-base registers (inputa3/a1, stagea2/a1). Primary C instead strength-reduces cursor to a second IV, leaving indexa2 and cursorv0; it gives fewer scan words. This is structural source/IV recognition, not just requested physical registers. Need a plausible source control/address model that emits recomputation without semantic changes. No compiler defect established.

Bounded negatives:

- Natural indexed clean292; pointer/parity scan plus repeated arg2&15 expression278/259; existing index before arrays210/259.
- Guarded-do and unsigned index/modulo leave278. Signed zero-test view273 changes target LBU to LB, rejected. Actual signed remainder224/271 adds unwanted division/narrowing operations.
- Reusing input parameter as cursor286 and natural for-style292 do not fix cursor IV. Separate three cursor/parity ranges310/259 grows frame128.
- Conditional checks through a separately assigned parameter base216/265 restore indexv0 but retain cursor IV; local base308/265. Indexed body with parameter base235/262 has frame110 and still advances cursor.
- Separate old-index capture302/267 adds saved register/frame. Approved final conditional P/non-P increments303/262, frame120; still cursor IV. No gain, stop spelling variants.

No volatile scheduling tricks, arbitrary padding, shared-header edits, production compiler changes, full executable builds, or new agents. Preserve primary210 and alternate211 rather than promoting an unverified body.
