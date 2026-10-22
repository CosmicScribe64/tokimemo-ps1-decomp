---
id: T-25054
title: Match ETC results scene
status: Done
assignee: Astra ETC worker
created: 2026-10-22
updated: 2026-10-22
links: ["[[tickets/T-25045-etc-ring-slots]]", "[[tickets/T-24909-func-8014790c-80146400]]", "[[playbook]]"]
---

## Goal

Match ETC func_8014790C in src/ovl/ETC/80146400.c. Exclusive ownership of this source continues after T-25045; root handles integration and full 27-binary verification. Do not change shared headers or compiler behavior without coordination. Preserve the accepted T-25045 pin and private evidence.

## Starting evidence

Use drafts/T-24909/ETC/func_8014790C/body.c and its older T-22029/T-24706 ledger, not a fresh m2c rewrite. Latest archive reports about 44 resolved rows; current full-TU baseline must be reproduced. Known gaps are switch v0/v1, duplicated D_8011ECD0 base versus a shared move, and record index arithmetic. The archive's mention of clear loops is not reflected in the saved body and will not be assumed correct.

## Acceptance

- Literal default-C match, affected ETC checksum and declaration checks.
- C89/standards review with all artificial source forms annotated FAKE.
- No inline assembly, patched instructions, per-function compiler exceptions or semantic shortcuts.
- Continue causal source/phase experiments until verified match or a concrete external blocker; historical time limits do not apply.

## Activity

2026-10-22: root assigned continuation and selected this function after comparing the only two remaining source guards; 801480D0 is much farther away. Re-read wiki index and T-24909 body/history. Ticket created before compiler experiments. No production change yet.

2026-10-22: Current full-TU archive baseline is 282/282 words with aligned cost 66; the archived44-row figure is stale. Removing the independent n counter and late-folding zero in `(i + 0x61) | ((u32)i < 0)` reproduces the loop's index arithmetic and registers (281 words, cost 21). First 124 instructions already match, including the historical switch issue. Phase capture shows uopt shares the base in s6 and copies to s0; the target separately materializes both bases. A three-instruction call scheduling difference remains. Pointer bias controls retain an add-zero but do not prevent base commoning; late branch definitions also fail. No production change yet. Private full-TU candidates and unmodified phase captures are in build/t25054/.

2026-10-22: A late-folded one-iteration `do { r = D_8011ECD0; } while ((u32)byte < 0)` creates the two independent target base loads, with all saved loop registers correct (282 words, cost 26). Using the switch byte as the condition perturbs its earlier live range; the next control uses a different defined operand. Remaining local address differences are now scheduling, rather than missing work. Ordinary address casts/array views did not affect commoning. An earlier loop-base-identity batch accidentally made no textual change; corrected replacements were measured and do not improve the best candidate.


## Result and validation

2026-10-22: Matched all 282 instructions / 1128 bytes in the default C build. The final source uses the canonical DisplayActor44 view for pool record 96 and the walked records. The one-iteration base initializer uses an always-false unsigned comparison; its unrelated byte operand avoids extending the switch byte's live range. The late-zero OR keeps i+97 intact before multiplication. Indexing all three result arrays supplies noalias metadata for the coordinate reloads, restoring the stack-argument delay slot. Moving the counter increment after the record walk reproduces the induction update order. The counter initialization and following do remain on one line for address scheduling.

Marked FAKE forms are the inherited oversized buffer, legacy non-void fallthrough (the sole caller ignores the result), one-iteration initializer, source-line grouping and zero OR. A void definition removes the target switch copy; canonical record fields preserve the match. No shared headers, compiler changes, inline assembly or instruction patches.

Validation: default funcdiff MATCH (1128 linked bytes), ETC SHA1 check OK (681c93b6e57c885f86e074c885a8bf6c3cca2319), headers OK, globals OK, prototype branch check 0 new / 15 known. Root owns independent source review and the full 27-binary integration gate. See drafts/T-25054/README.md for controls; private compiler captures and full translation units remain in build/t25054/.

2026-10-22: Root independently reviewed the final canonical record fields, loop bounds, string/call order, unsigned-false identities and non-void fallthrough. No standards/spec findings. The final edited source again passes default literal 1128-byte comparison, ETC checksum and declaration guards. Full27 integration remains with root.

Root integration accepted pin0d07f7dac: independent default-C linked1128-byte MATCH, fresh27/27 binary SHA1 checks, headers/globals and0new/15known prototype checks all pass. Final canonical source directly reviewed with no findings. Done; worker continues same-file801480D0 under T-25059.
