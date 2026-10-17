---
id: T-16100
title: Evidence-based pass over 165 flagged compiling drafts
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-17
---

User requests a GPT-6.1 Sol medium pass over all165 flagged drafts. Three disjoint batches preserve initial alternatives and review every flagged scope. Repair only with target assembly/type/layout evidence; compile changed full translation units and retain provenance, scores and unresolved explanations. Do not rename locals or weaken audit patterns just to clear flags. No production matching claim without default linked proof. Existing Docker tooling only; no new worktrees or disc copies needed. Results go under drafts/T-16100, with original drafts untouched.

## Result
165/165 reviewed and full-TU compiled;93 changed alternatives. Unchanged quality audit reduces flagged scope count165→116. No production source edits or new matching bytes. Scope/hash completeness checks pass; see [[flagged-draft-pass]]. Further semantic/matching review remains open.

## Comments

2026-10-17: Research pass is complete and merged: 165 of 165 scopes reviewed, 93 repaired alternatives, no production change, and later wave-3 packages already consumed the archive. Remaining flagged scopes (116) are left to the types-first redrafts rather than a second review pass. Review found two issues: committed instruction listings under drafts/ and untested one-shot tools. Both are in the followup ticket, so this one moves to Done.

Followup opened: [[tickets/T-21011-drafts-listings-and-t16100-tools|T-21011]].
