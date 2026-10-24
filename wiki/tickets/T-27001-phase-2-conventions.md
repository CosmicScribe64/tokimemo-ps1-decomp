---
id: T-27001
title: "Phase 2 naming conventions: ADR, standards, glossary, plan"
status: In Review
assignee: w19-conventions
created: 2026-10-24
updated: 2026-10-24
links: [[[phase2]], [[tickets/T-27000-phase-2-readable-codebase]], [[obin]], [[emulator]]]
---

## Goal

Write down the user's four phase 2 decisions of 2026-10-24 and the naming conventions that follow from them,
so that naming agents and reviewers work from one checkable rule set. Inputs: the T-25900 naming audit
(`drafts/T-25900/audit.md`) and the local overlay map and glossary seed (not committed).

## Acceptance criteria

- [x] `docs/adr/0001-phase-2-naming.md`: the decisions, the conventions (case, overlay prefixes, main subsystem
      prefixes, enums and constants, hypotheses, locals, offset comments, files and headers with the disc-name
      map), what stays out of scope, and the consequences.
- [x] `CODING_STANDARDS.md` section 4 (naming) and section 5 (files and headers) rewritten; section 8 (`unk_`
      rule) and 8a (header split) aligned; reviewer checklist lines updated.
- [x] `GLOSSARY.md` at the repo root: tables for people, parameters, calendar, places, clubs, events and
      scenes, systems and UI, mini games; each row with term, meaning, stem, keep or translate, source; the
      date, event and mode traps. Only proper names and short terms.
- [x] [[phase2]]: work order (steps 0-8), unit tables, how a unit runs, the review rule; line in [[index]].
- [x] Tickets T-27000 (In Progress), T-27003 and T-27004 (Backlog); T-27002 referenced, not created.
- [x] deslop check on CODING_STANDARDS.md, GLOSSARY.md and the ADR; real hits fixed.

## Notes

- Club ids follow `club_name_table` order, checked against the generated data asm (local build, not
  committed). The place, girl and parameter orders come from the seed's string survey.
- The hypothesis file names (`config/symbol_addrs_hypotheses.txt`, `config/symbols_<DISC>_hypotheses.txt`)
  are this ticket's choice; T-27002 has to place hypothesis rows there or the ADR changes with it.
- Not decided by the user: rewriting old names in the wiki (the plan follows the audit's lookup proposal,
  audit question 2), the rewritten CODING_STANDARDS section 4 (audit question 5, needs the user's approval) and
  how to handle TT (its contributor first). The ADR lists the first two under Pending.
- Step 2's enum list differs from the audit's on purpose; the reasons are in [[phase2]].

## Comments

- 2026-10-24 (w19-conventions): written; docs only, no build change. Waiting for review against
  CODING_STANDARDS.md.
- 2026-10-24 (w19-conventions): deslop check (`find_tells.py`) on CODING_STANDARDS.md, GLOSSARY.md and the ADR:
  the 2 strong hits (bold labels that restated their line) and the real weak ones (hinge colons, long
  sentences, passives in new text) fixed. Kept: label colons in reference bullets, "primitive" (GPU term), and
  hits in unchanged sections. `tools/wiki.py lint`: 0 broken links; the 4 new tickets wait for the kanban sync.
- 2026-10-24 (w19-conventions): fixes for the first review. Glossary: an abbreviation table (`cd`, `gpu`, `sys`,
  `vsync`, `misc` and the others in use), `card` and `pad` traps, rows for encounter, phone, talk, annual event,
  culture-festival tour, name entry, prologue, sound test and debug menu; prefixes may be short forms of stems
  that the map lists. Main prefixes `game_mode_` (was `mode_`) and `memory_card_` (was `card_`); EVENT and GYOZI
  marked "confirm unused"; `date_sasoi_judge` noted as the kept O.BIN exception. One rule for renames before
  the tool: none (ADR and section 4 agree); the T-20500 GameState pilot moved to T-27004. Hypotheses: the
  EVENT-range exception (`config/symbol_addrs_main_hypotheses.txt`, matching T-27002's `symtab.py`) and the
  check over every loaded file. File renames: the `.rodata`/`.data` subsegments, inner asm paths and
  `INCLUDE_RODATA` strings change too. The ledger keeps each row's note. Section 10 states the no-game-text
  rule. The umbrella headers are deleted in step 8 (new T-27000 criterion). T-27000's first box unticked
  until this ticket is Done.
