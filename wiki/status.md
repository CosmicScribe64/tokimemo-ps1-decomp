---
type: status
updated: 2026-10-20
---

# Status

The one page for "where are we". Update it when a wave, a local or cloud session, or a contributor PR changes the numbers (`tools/docker.sh python3 tools/progress.py`). Older status and handoff pages are in `archive/` ([[completion-run-2026-10-17]], [[stopping-point-2026-10-15]]).

## Progress (2026-10-20)

- Game code: 6,785 / 6,952 functions, about 90.5% of bytes. SDK libraries (722 functions) stay asm by design ([[psyq-sdk]]).
- Complete units: TT 303/303, DATE2 102/102, and every overlay not listed below.
- Remaining by unit (functions left): main 66 (in 20 files), TACO 19, GEKO 11, TAIIKU 11, ETC 10, GYOZI 8, RPG_BAT 6, ENDING 5, OPTION 5, SHOUGATU 5, KANGEI 4, MASTER 4, NAME_ENT 4, VALEN 4, BUNKA_SD 2, EN_NICHI 2, TEL 2.
- What is left is mostly hard tails: guarded drafts a few rows off (compiler ordering and register choices, [[build-step-gaps]]) and large functions without a good draft. Find a function's history in [[notes/functions]].

## Active work

- Local matching sessions on file-disjoint lists: T-24301, T-24302, T-24303 merged (5 matches); next lists from a fresh inventory. Lists: `.claude/orchestrator/lists/l6-p0N-list.txt` (local).
- Cloud "match packets" ([[cloud-matching]]): T-24200..T-24214 done, 7 matches; paused (cloud usage spent).
- Wiki reorganization: T-24400.

## How work is run

- Matching recipes: [[playbook]] first, then [[register-cheatsheet]] and [[decompile-workflow]].
- Tickets: [[kanban]] (board) and [[tickets/catalog]] (every ticket by status). Rules: AGENTS.md.
- Orchestration (waves, merges, PR handling) uses local scripts in `.claude/orchestrator/` (not committed; see its README).
- Contributors coordinate through GitHub issues (claims) and Discussions; only matching work (phase 1) is merged for now, per `ROADMAP.md`.
