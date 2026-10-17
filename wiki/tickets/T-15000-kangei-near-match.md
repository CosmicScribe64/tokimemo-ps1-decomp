---
id: T-15000
title: Resolve KANGEI scene setup near match
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-17
links: ["[[matching-notes]]", "[[tickets/T-14501-remaining-draft-proof]]"]
---

## Goal

Resolve KANGEI func_80135CE0's six-score draft miss using genuine type and aggregate views. Prioritize incoming T-14500 zero candidates.

## Acceptance criteria

- [x] Default linked bytes and KANGEI SHA1 verified before promotion.
- [x] Existing C preserved; ordinary C or any required FAKE clearly marked.
- [x] Header/global/prototype checks pass.

## Notes

Frozen T-14500 archive candidate: src/ovl/KANGEI/801355F0.c, 432 inventory bytes, score 6. Work is isolated in the T-14501 proof checkout; census remains uninterrupted.

## Comments

2026-10-15: Claimed at root's request. Checking prior attempts and original aggregate access widths before bounded C experiments.

2026-10-15: The six-score miss was control-flow structure, not a type gap. m2c shared the final byte store through `goto block_4`; the target duplicated this store in the two branches. Ordinary if/else-if with an explicit final store in each branch matches 432 linked bytes (432 inventory coverage bytes). No forced registers, dummy statements, new declarations, header changes, or FAKE. Existing C preserved: the only removed source line is the target INCLUDE_ASM.

Docker verification: KANGEI SHA1 OK; `funcdiff.py --resolve --no-build --version default KANGEI:func_80135CE0` literal MATCH (432 bytes); headers OK, globals OK, branch prototypes zero new disagreements / 15 known. Evidence: /private/tmp/t15000-seed-diff.log, /private/tmp/t15000-structured-diff.log, /private/tmp/t15000-final-proof.log. Historical-attempt catalog contains no prior entry for this scope. Focused CODING_STANDARDS.md inspection passes; status In Review for root's final check.

2026-10-17: func_80135CE0 (432 bytes) is on main as ordinary if/else-if C with no FAKE or guard, and it kept matching through the later T-18400 type pass and T-20900; T-18400's review re-checked the KANGEI sha1. Review against CODING_STANDARDS.md found nothing, and all acceptance criteria are checked. Closed as Done.
