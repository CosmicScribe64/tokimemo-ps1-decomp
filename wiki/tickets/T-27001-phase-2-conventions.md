---
id: T-27001
title: "Phase 2 naming conventions: ADR, standards, glossary, plan"
status: Done
assignee: w19-conventions
created: 2026-10-24
updated: 2026-10-24
links: `[phase2`, [[tickets/T-27000-phase-2]], [[obin]], [[emulator]]]
---

## Goal

Write down the user's four phase 2 decisions of 2026-10-24 and the naming conventions that follow from them,
so that naming agents and reviewers work from one checkable rule set. Inputs: the T-25900 naming audit
(`drafts/T-25900/audit.md`) and the local overlay map and glossary seed (not committed).

## Acceptance criteria

- [x] `docs/adr/0001-phase-2-naming.md`: the six decisions with dates, the port-first scope, the conventions
      (case, the prefix and word rules, overlay and main prefix maps, names already in the tree, enums and
      constants, hypotheses, locals, offset comments, files and headers with the disc-name map), what stays
      out of scope, and the consequences.
- [x] `CODING_STANDARDS.md` section 4 (naming) and section 5 (files and headers) rewritten; section 8 (`unk_`
      rule) and 8a (header split) aligned; reviewer checklist lines updated.
- [x] `GLOSSARY.md` at the repo root: tables for people, parameters, calendar, places, clubs, events and
      scenes, systems and UI, mini games; each row with term, meaning, stem, keep or translate, source; the
      date, event and mode traps; the Module words table and the abbreviation table. Only proper names and
      short terms.
- [x] [[phase2]]: work order (steps 0-8), unit tables, how a unit runs, the review rule; line in [[index]].
- [x] Tickets T-27000 (In Progress), T-27003 and T-27004 (Backlog); T-27002 referenced, not created.
- [x] deslop check on CODING_STANDARDS.md, GLOSSARY.md and the ADR; real hits fixed.

## Notes

- Club ids follow `club_name_table` order, checked against the generated data asm (local build, not
  committed). The place, girl and parameter orders come from the seed's string survey.
- The hypothesis file names (`config/symbol_addrs_hypotheses.txt`, `config/symbols_<DISC>_hypotheses.txt`)
  are this ticket's choice; T-27002 has to place hypothesis rows there or the ADR changes with it.
- Decided by the user on 2026-10-24 after the first two reviews: `tools/rename.py` is the only way to rename
  once it has merged (audit question 5, now the binding rule of CODING_STANDARDS section 4); old names stay in
  the wiki's history and current pages use the new names (audit question 2); the scope is port-first. The ADR
  records all three, and it no longer has a Pending section. Still open: how to handle TT (its contributor
  first), listed in [[phase2]].
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
- 2026-10-24 (w19-conventions): third pass, after the second review (not approved, four blocking findings).
  User decisions recorded: rename.py is the only way to rename (decision 5, section 4 states it as binding; the
  Pending item is gone from the ADR, [[phase2]] and this ticket), old names stay as history while current pages
  use the new names (decision 6), and the port-first scope (ADR section Scope, [[phase2]], criteria of
  [[tickets/T-27000-phase-2]]). Review points: the prefix rule is now "a glossary stem, a listed abbreviation,
  or a row of the Module words table" in the ADR, section 4 and the glossary intro, with 32 module-word rows,
  and a script check shows all 78 map prefixes meet it; ordinary technical words need no row, game-term words
  do. Names already in the tree: non-O.BIN names are re-checked by each unit and renamed through rename.py when
  they fail (list in [[phase2]]); `buf`, `lba`, `init`, `src`, `dst` added as abbreviations. SDK exception
  recorded as the user's (SDK names optional), with the SDK-named aliases left to T-27003. `main_state_set` is
  an English-patch name, not an O.BIN name (ADR map and glossary fixed). Comments on O.BIN names go at the C
  definition; the SDK-side ones (no C definition) are optional and sit in `symbol_addrs_obin.txt`. `girl_`
  belongs to the 8004F6B0/8004F870 module only, `sound_` is no main prefix, `appointment_` and `club_` stay with
  their owners. File names get the HYPOTHESIS rule. `spot_` replaces `spots_`; the pass count is 24.
- 2026-10-24 (w19-conventions): code review passed (approved, no blocking findings; round 3 against
  `CODING_STANDARDS.md` and the ticket criteria). `tools/wiki.py lint`: 0 broken links; the 4 status mismatches
  (T-27000, T-27001, T-27003, T-27004 without a card) are cleared by the kanban sync at merge. Non-blocking notes:
  `pad_` meets the prefix rule through the `stems` column of the `pad` trap row, not through a Module words row
  (a row would remove the doubt for later reviewers); the 7 O.BIN stems in the main map (`game_main`, `srn_`,
  `mod_`, `k_disp_`, `set_kanji_`, `dec_bg_`, `normal_date_`) are exempt because O.BIN names keep their own
  stems, so "all 78 prefixes" holds (85 map entries minus these 7). Moved to Done.
