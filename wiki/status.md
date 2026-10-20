---
type: status
updated: 2026-10-20
---

# Status

The one page for "where are we". Update it when a wave, a local or cloud session, or a contributor PR changes the numbers (`tools/docker.sh python3 tools/progress.py`). Older status and handoff pages are in `archive/` ([[completion-run-2026-10-17]], [[stopping-point-2026-10-15]]).

## Progress (2026-10-20)

- Game code: 6,809 / 6,952 functions, 2,084,280 / 2,276,920 bytes (91.5%), measured with Docker `tools/progress.py` after source integration `3741db7e2`. SDK libraries (728 functions) stay asm by design ([[psyq-sdk]]).
- Complete units: TT 303/303, DATE2 102/102, and every overlay not listed below.
- Remaining by unit (143 functions): main 58, TACO 14, GEKO 9, TAIIKU 9, ETC 10, GYOZI 6, RPG_BAT 4, ENDING 5, SHOUGATU 4, MASTER 4, NAME_ENT 4, VALEN 4, KANGEI 3, OPTION 3, BUNKA_SD 2, EN_NICHI 2, TEL 2.
- What is left is mostly hard tails: guarded drafts a few rows off (compiler ordering and register choices, [[build-step-gaps]]) and large functions without a good draft. Find a function's history in [[notes/functions]].

## Active work

- T-24500: Codex resumed with three Sol 6.1 medium workers: MAIN (T-24501), TACO (T-24502), GEKO/OPTION (T-24503). The user authorized covering issue #65 while its contributor is out of tokens until Friday. Record exact completed functions for handoff; Astra is reserved for specific blockers.
- Five new functions are integrated and pushed: MAIN `func_800654D0` (1,072 bytes), TACO `func_80140028` (828) and `func_8014FFDC` (2,292), GEKO `func_80134808` (432) and `func_80133308` (656). All passed default-C linked proofs, 27 binary hashes and declaration guards. The two GEKO completions belong in issue #65's eventual contributor handoff; no contributor message has been sent.
- Three further matches are on main: MAIN `make_color_bar16` (1,540 bytes), TACO `func_8013F468` (880), RPG_BAT `func_8014BB80` (1,124). Total this run: eight functions, 8,824 bytes. The third coordinator T-24520 owns TAIIKU, RPG_BAT and GYOZI/VALEN/SHOUGATU; T-24510 may reassign completed TEL/ENDING/ETC lanes to MASTER, NAME_ENT and BUNKA_SD/EN_NICHI/KANGEI. Root serializes integration; up to nine Sol workers across three chats.

- Eight additional matches are pushed through `3741db7e2`: RPG_BAT `func_801447E0` (924), TAIIKU `func_80134060` (1,348) and `func_80134F98` (1,256), SHOUGATU `func_80133B44` (408), GYOZI `func_80140F90` (788), TACO `func_8014CB24` (1,756) and `func_80144E3C` (1,888), MAIN `menu_bar_color` (84). Resumed-run total: **16 functions / 17,276 bytes**. All have root integration checks for 27 hashes, literal default-C output and declarations. New donors are being checked against remaining siblings; the earlier global sweep found no automatic twins.

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

T-24510 archived bounded TEL/ENDING/ETC and MASTER/NAME_ENT/BUNKA_SD/EN_NICHI/KANGEI investigations with zero new C bytes. Reviewed checkpoints preserve semantic corrections and replay evidence. Two Sol workers have restarted deeper MASTER and ETC reconstructions under T-24517 and T-24518; coordination remains In Progress.
