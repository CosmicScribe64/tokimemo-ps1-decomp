---
id: T-25051
title: Match TACO wipe quad store schedule
status: In Review
assignee: Astra
created: 2026-10-22
updated: 2026-10-22
links: ["[[tickets/T-24905-func-8013b460-8013B170]]", "[[tickets/T-24200-cloud-packet-taco-8013b460]]"]
---

## Goal

Match `func_8013B460` (1328 bytes) in exclusively owned `src/ovl/TACO/8013B170.c` using source-only controls and unchanged headers/toolchain.

## Acceptance criteria

- [ ] Literal default C and affected TACO checksum match.
- [ ] Declaration guards and independent standards/spec review pass.

## Notes

Read historical T-19030/T-24200/T-24705/T-24905 and latest T-25011/T-25016 ledgers before experiments. Baseline two y3 halfword stores are early. A y3 temporary recovers late store order but changes constant register allocation; old source grouping/order grids are excluded. Investigate source-local promotion/liveness controls informed by T-25043/T-25047 rather than repeat permutations.

## Comments

- 2026-10-22: Parent assigned next unclaimed TACO lane after 80149F90.c completion. Own only func_8013B460 in 8013B170.c; same isolated t25043-taco worktree/branch. Ticket created before experiments.

- 2026-10-22 fresh baseline332 words/cost4 confirms only the two displaced halfword stores. New empty/unsigned-false y3 reads after UV stores remain332 words but cost376, disrupting saved-register allocation; before UV cost372. Reusing the pad home as an explicit y3 local remains332/cost178, reproducing the known constant interference plus downstream rotor shift. An unnamed arithmetic expression spanning UV comma side effects (+, OR, XOR with comma-zero) remains332/cost52: UV side effects move before coordinate loads, so not adopted. A late-zero result in that expression grows336/cost332. Next step: actual full-TU allocator trace of local y3 and constants, not another order grid. Production remains unchanged.

- 2026-10-22: New dependency and lifetime ledger: direct s16 or s16/u16 roundtrip casts remain332/cost4; explicit 0xFFFF mask remains332/cost164 with +1 temporary rotor but early store unchanged. Direct late-zero arithmetic grows336/344 and worsens allocation. Repeated y3 self-store or OR-zero after UV grows336/cost166. Empty unsigned-false arg0/p tests after y3 are332/cost358/372. Separate struct-member local matches the s16 temporary's332/cost44; a zero-use in AddPrim preserves44, extension through the next call grows340/cost194, and false tail read gives332/cost364. Unsigned16 temporary gives332/cost198. Deferred-lvalue UV side effects move the UV writes before coordinate reads (332/cost56); subtraction/division expression forms similarly52. Equivalent signed/unsigned halfword pointer lvalues leave the baseline4. No production edits; rejected mechanisms are not to be repeated.

- 2026-10-22: Captured actual full-TU stock ugen/as1 inputs for baseline and s16 temporary in ignored build/t25051/cap-*. Baseline second quad's y3 store node17 and UV nodes9–16 all have aftercycles6, and all block the following mode load node7; the y3 arithmetic releases node17, which wins the equal-priority tie before the remaining UV stores. New halfword conversion boundaries (signed16 then mask, unsigned16 then mask, narrowing before -120) retain332 words but spend extra rotating temporaries and keep the early store. Scoped const/register/auto temporary initialization all reproduce44. Token-level newlines leave4. Immediate conditional-goto joins reproduce the empty-if disruption; a plain immediate goto retains332 but changes saved constant allocation (38) and leaves the store early. A dead pad assignment inside the arithmetic expression anchors evaluation but introduces the same unwanted uopt allocation (178). All are rejected source-only probes, not compiler changes or candidate promotions.

- 2026-10-22: Further controls: direct pointer-based unsigned-false zero expressions, with or without a named zero, fold back to baseline4 or temporary44. Parameter-based zero's earlier disruption was parameter liveness, not a useful scheduling effect. Ternary/logical identical-zero arms add code (340/cost402/382); duplicate UV arms grow356/380. One-element local array gives336/170 and late-zero indirect index340/332. Mask/conversion combinations on the scalar temporary retain332 but preserve its unwanted v0 core allocation (cost186–198); unsigned32 local reproduces178, unsigned32 plus signed16 conversion44. Reversible XOR/complement and shift encodings retain real extra instructions (336), so rejected. Immediate-goto lifetime extension changes regional allocation and remains worse (332/362 or324/438). Stock as1's reconstructed local selection routine func_433ed0 (around L434340 in the existing as1-list.c) compares earliest ready time, then larger aftercycles/latency, then original instruction position at node+48, then line at node+12. This supports targeting emitted instruction order rather than more formatting controls. No synthetic assembler manipulation was performed.


## Wrap-up requested by user

- 2026-10-22: Parent relayed the user instruction to freeze current work, commit, and push. This is a requested research checkpoint, not a match or external-blocker claim. Production src/ovl/TACO/8013B170.c is unchanged and remains guarded. Literal-C/checksum acceptance criteria remain unchecked because no candidate was promoted.
- Final controls showed that const views of the y3 coordinate or mode loads do not alter alias scheduling (332/cost52 for the post-UV direct store). A volatile view of y3 or only u0 leaves baseline4, while all UV stores volatile gives332/cost32. No qualifiers were adopted.
- Source-only reproduction scripts and interpretation are archived in drafts/T-25051/. Private compiler traces, objects, preprocessed TUs, and game inputs remain ignored and uncommitted. Ticket moved In Review. Parent independently reviewed the research scope and reported no findings; the function remains unmatched.

- Docker validation parsed all 27 archived Python files without executing experiments. Wiki lint reported zero broken links and zero status mismatches, with two existing oversized-page warnings. The direct-file prose check found no strong tells. Its weak findings were corrected; staged mode could not resolve the host worktree metadata from Docker. Git whitespace checks passed, and this checkpoint changes no production source, headers, or build tools.
