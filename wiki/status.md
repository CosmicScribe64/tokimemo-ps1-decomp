---
type: status
updated: 2026-10-20
---

# Status

The one page for "where are we". Update it when a wave, a local or cloud session, or a contributor PR changes the numbers (`tools/docker.sh python3 tools/progress.py`). Older status and handoff pages are in `archive/` ([[completion-run-2026-10-17]], [[stopping-point-2026-10-15]]).

## Progress (2026-10-20)

- Game code: 6,798 / 6,952 functions, 2,072,284 / 2,276,920 bytes (91.0%), measured with Docker `tools/progress.py` after source integration `e51680da0`. SDK libraries (728 functions) stay asm by design ([[psyq-sdk]]).
- Complete units: TT 303/303, DATE2 102/102, and every overlay not listed below.
- Remaining by unit (154 functions): main 60, TACO 17, GEKO 9, TAIIKU 11, ETC 10, GYOZI 7, RPG_BAT 6, ENDING 5, SHOUGATU 5, MASTER 4, NAME_ENT 4, VALEN 4, KANGEI 3, OPTION 3, BUNKA_SD 2, EN_NICHI 2, TEL 2.
- What is left is mostly hard tails: guarded drafts a few rows off (compiler ordering and register choices, [[build-step-gaps]]) and large functions without a good draft. Find a function's history in [[notes/functions]].

## Active work

- T-24500: Codex resumed with three Sol 6.1 medium workers: MAIN (T-24501), TACO (T-24502), GEKO/OPTION (T-24503). The user authorized covering issue #65 while its contributor is out of tokens until Friday. Record exact completed functions for handoff; Astra is reserved for specific blockers.
- Five new functions are integrated and pushed: MAIN `func_800654D0` (1,072 bytes), TACO `func_80140028` (828) and `func_8014FFDC` (2,292), GEKO `func_80134808` (432) and `func_80133308` (656). All passed default-C linked proofs, 27 binary hashes and declaration guards. The two GEKO completions belong in issue #65's eventual contributor handoff; no contributor message has been sent.

- Local matching sessions on file-disjoint lists: T-24301, T-24302, T-24303 merged (5 matches); next lists from a fresh inventory. Lists: `.claude/orchestrator/lists/l6-p0N-list.txt` (local).
- Cloud "match packets" ([[cloud-matching]]): T-24200..T-24214 done, 7 matches; paused (cloud usage spent).
- Wiki reorganization: T-24400.

## How work is run

- Matching recipes: [[playbook]] first, then [[register-cheatsheet]] and [[decompile-workflow]].
- Tickets: [[kanban]] (board) and [[tickets/catalog]] (every ticket by status). Rules: AGENTS.md.
- Orchestration (waves, merges, PR handling) uses local scripts in `.claude/orchestrator/` (not committed; see its README).
- Contributors coordinate through GitHub issues (claims) and Discussions; only matching work (phase 1) is merged for now, per `ROADMAP.md`.

### Additional isolated overlay lanes (2026-10-20)

T-24510 coordinates user-authorized Sol 6.1 medium workers TEL T-24511, ENDING T-24512, ETC T-24513 from `23e1a20fb`. Trees are isolated under /private/tmp, with modest j2 worker load and no coordinator compiler search. Main integration remains in the original orchestrator chat. Results will be pinned with literal C and linked-byte/checksum evidence; no new coverage yet.
