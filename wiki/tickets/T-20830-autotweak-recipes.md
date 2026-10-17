---
id: T-20830
title: autotweak edit kinds for the wave-3 recipes (GameState pointer, statement runs, chain stores, multi-use local)
status: In Progress   # Backlog | Ready | In Progress | In Review | Done  (must match column in wiki/kanban.md)
assignee: tr-autotweak-recipes
created: 2026-10-17
updated: 2026-10-17
links: ["[[decompile-workflow]]", "[[register-cheatsheet]]", "[[matching-notes]]", "[[tickets/T-11170-cheat-sheet-auto-tweaker]]"]
---

## Goal

Make four recipes that closed wave-3 functions by hand into autotweak edit kinds, so the search finds them and the matchers stop re-deriving them. autotweak ran in about 10 wave-3 notes (686 edits on `func_80140A90`, 411 on `func_801518B8`, 228 on `func_80072D70`, 183 s on `func_80144094`, 150 s on `func_8004B358`) and mostly exhausted without gain, because the recipes that closed functions were not edit kinds.

- `gstate`: `GameState *g = &D_800E6280;` with every read through `g` (7 matches: main `func_8006EB64`, `func_8006EEC4`, `func_80072998`, `func_800726F0`; TACO `func_80133670`, `func_80133728`; DATE `func_80152E34`; 5 or more misses; 48 of the 101 guarded drafts read `D_800E6280` directly twice or more with no `g`).
- `group`: several statements on one source line (as1 schedules by line): TACO `func_80140ADC` (eight UV stores), `func_8013CCD0`, main `func_80055A38`, GYOZI `func_8013E638`, DATE `func_80149F48`. `t_lines` joins only an adjacent pair; a run of 5 to 8 is out of reach of the depth-2 search.
- `chain`: `a = b = c = v;` for equal stores (DATE `func_80155004`), comma-line colour stores (`func_80152E34`).
- `inline_multi`: a local that copies a global and is tested twice replaced by direct reads (main `func_800726F0`); `t_temp` inline handles exactly one definition and one use.

## Acceptance criteria

- [ ] `tools/recipes.py` with the four edit kinds (pure text transforms on the `Func`/`Variant` interface), registered by one line in `tools/autotweak.py` (the `typefix.register` pattern).
- [ ] `HINT_EDITS`: tie += group; ugen += group, chain; priority += gstate, inline_multi; structure and argument += gstate. `--only-kinds a,b`; `--list` prints per-kind counts.
- [ ] `tools/test_recipes.py` (synthetic bodies shaped like `func_80140ADC`, `func_80055A38`, `func_8006EB64`, `func_800726F0`; no-op guards; variant counts bounded). Full tool suite passes.
- [ ] Docs: `wiki/register-cheatsheet.md` (each row names its edit kind), `wiki/decompile-workflow.md`, `wiki/matching-notes.md`, log, index.
- [ ] First use: `autotweak.py --all --only-kinds gstate,group,chain,inline_multi --time 30` over the guarded drafts and over the wave-3 archive drafts; hit table in the ticket and the log.
- [ ] Code review against `CODING_STANDARDS.md`, findings resolved, then In Review.

## Notes

Rules for this ticket: only `tools/`, `wiki/` and new `drafts/T-20830/` folders change; `src/` and `include/` stay as they are, so a MATCH found by the first use is saved as plain C under `drafts/T-20830/<UNIT>/` for the lead to apply (run with `--dry-run`).

## Comments
