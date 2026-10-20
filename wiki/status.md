---
type: status
updated: 2026-10-20
---

# Status

The one page for "where are we". Update it when a wave, a local or cloud session, or a contributor PR changes the numbers (`tools/docker.sh python3 tools/progress.py`). Older status and handoff pages are in `archive/` ([[completion-run-2026-10-17]], [[stopping-point-2026-10-15]]).

## Progress (2026-10-20)

- Game code: 6,815 / 6,952 functions, 2,090,540 / 2,276,920 bytes (91.8%), measured with Docker `tools/progress.py` after source integration `97a004d39`. SDK libraries (728 functions) stay asm by design ([[psyq-sdk]]).
- Complete units: TT 303/303, DATE2 102/102, and every overlay not listed below.
- Remaining by unit (137 functions): main 54, TACO 13, GEKO 9, TAIIKU 9, ETC 10, GYOZI 6, RPG_BAT 4, ENDING 5, SHOUGATU 4, MASTER 3, NAME_ENT 4, VALEN 4, KANGEI 3, OPTION 3, BUNKA_SD 2, EN_NICHI 2, TEL 2.
- What is left is mostly hard tails: guarded drafts a few rows off (compiler ordering and register choices, [[build-step-gaps]]) and large functions without a good draft. Find a function's history in [[notes/functions]].

## Active work

- T-24520 isolated lanes delivered six verified default-C functions/5,848 bytes: TAIIKU T-24521 two, RPG_BAT T-24522 two, GYOZI/SHOUGATU T-24523 two. Originating chat serializes integration and owns global progress totals. Follow-up structural probes and fresh-donor comparisons yielded no additional matches; 27 functions remain across these lanes. Tickets remain In Progress while workers investigate concrete owned hypotheses. One private consolidated branch contains all three archive heads plus coordinator notes; source-only checkpoints and stale VALEN guard cleanup add no coverage.

- T-24500: Codex resumed with three Sol 6.1 medium workers: MAIN (T-24501), TACO (T-24502), GEKO/OPTION (T-24503). The user authorized covering issue #65 while its contributor is out of tokens until Friday. Record exact completed functions for handoff; Astra is reserved for specific blockers.
- Five new functions are integrated and pushed: MAIN `func_800654D0` (1,072 bytes), TACO `func_80140028` (828) and `func_8014FFDC` (2,292), GEKO `func_80134808` (432) and `func_80133308` (656). All passed default-C linked proofs, 27 binary hashes and declaration guards. The two GEKO completions belong in issue #65's eventual contributor handoff; no contributor message has been sent.
- Three further matches are on main: MAIN `make_color_bar16` (1,540 bytes), TACO `func_8013F468` (880), RPG_BAT `func_8014BB80` (1,124). Total this run: eight functions, 8,824 bytes. The third coordinator T-24520 owns TAIIKU, RPG_BAT and GYOZI/VALEN/SHOUGATU; T-24510 may reassign completed TEL/ENDING/ETC lanes to MASTER, NAME_ENT and BUNKA_SD/EN_NICHI/KANGEI. Root serializes integration; up to nine Sol workers across three chats.

- Eight additional matches are pushed through `3741db7e2`: RPG_BAT `func_801447E0` (924), TAIIKU `func_80134060` (1,348) and `func_80134F98` (1,256), SHOUGATU `func_80133B44` (408), GYOZI `func_80140F90` (788), TACO `func_8014CB24` (1,756) and `func_80144E3C` (1,888), MAIN `menu_bar_color` (84). Resumed-run total: **16 functions / 17,276 bytes**. All have root integration checks for 27 hashes, literal default-C output and declarations. New donors are being checked against remaining siblings; the earlier global sweep found no automatic twins.

- MAIN `func_80076000` adds 1,196 verified bytes at `8253c1d73`: current resumed-run total **17 functions / 18,472 bytes**. The corrected packed field and explicit flag extraction close a previously broad register mismatch. Header placement review resolved; all 27 integration hashes, literal default-C proof and declaration guards passed. MAIN/GEKO/TACO diagnostic archives are also preserved through `3c8582d6a`, without extra coverage.

- TACO `func_8013A790` adds 1,492 verified bytes at `735e50bd9`: current resumed-run total **18 functions / 19,964 bytes**. Root checks cover all 27 hashes, literal default-C output and declaration guards. Source-only MASTER copy-state consultation and deeper GEKO scene reconstruction are preserved through `036407cd5`; neither is counted as a match. Active Sol workers continue; Astra is inspecting MASTER's isolated copy-end issue, with MAIN767A4's stack-only issue queued.

- MAIN `func_800767A4` adds 932 verified bytes at `f294c7f36`: resumed-run total **19 functions / 20,896 bytes**. Astra's bounded consultation found two separated padding slots that preserve every stack home. Root reviewed the C, padding annotations, canonical prototype and exact string data; all 27 hashes, literal default-C proof and declaration guards passed. Source archives and the incorrect VALEN draft removal are preserved through `8c52b2b0a`, without coverage credit. Astra's subsequent TACO color-decode consultation found no match and is complete; Sol workers continue their owned targets.

- Refreshed Docker twin scan at 6,811 matched / 141 remaining found no exact, near, lenient or wider-shape copies and no identical unmatched groups. Focused sibling work continues; this scan preceded the newest MAIN match and does not claim that all semantic reuse is exhausted.

- MASTER `func_80135FB8` adds 1,084 verified bytes at `ca34dcf7f`: resumed-run total **20 functions / 21,980 bytes**. The matched sibling supplied initialization and shuffle structure; the preceding fade callee's return view closed the final register difference. Root reviewed the source and scoped override; literal default C, all 27 hashes and declaration guards pass. GEKO eligibility, TACO fan/projection and MAIN CD archives are preserved through `f1e2511ff`, with no extra coverage credit. Astra's RPG pointer consultation found a simpler equivalent source form but no literal match; TACO projection is the active bounded consultation, with MAIN CD queued.

- MAIN `func_80046754` adds 884 verified bytes at `6a2c6eb4b`: resumed-run total **21 functions / 22,864 bytes**. Natural packed-texture loops, byte fill color and halfword upload coordinates match without artificial padding or allocation tricks. Root review, literal default C, all 27 hashes and declaration guards pass. Current bounded consultations are complete without further matches; their negative results are being preserved. MAIN continues the adjacent renderer; the former TACO worker now owns NAME_ENT, after preserving TACO lifetime evidence and correcting standalone/full-file scope. Other Sol lanes continue independently.

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

## Usage-conserving mode (2026-10-20)

The user requested lower usage before capacity runs out. Keep only two active Sol6.1 medium workers: MAIN T-24501 and the former TACO worker's NAME_ENT reconstruction. All other workers/coordinators finish their current bounded experiment, preserve checkpoints, and remain idle. Astra stays idle unless a specific narrow blocker warrants escalation. Do not spawn extra work merely to fill slots. Workers prove literal C and affected-unit/declaration checks; root runs the full27 integration gate once. Reduce polling, repeated context reads, duplicate builds and verbose diagnostics. The100% goal remains active.

Latest user steering supersedes the earlier Luna-first experiment: use **Sol 6.1 medium workers with Astra for specific hard blockers**. No new Luna assignments. Keep at most two active matching workers and preserve their existing context. Judge progress by verified bytes added, not diagnostic score reductions; avoid repeated source-spelling experiments without new causal evidence.

### Current assignments after archive integration

Two Sol 6.1 medium workers are active: the MAIN worker preserves the latest `sndi` reconstruction and briefly ranks concrete shared blockers before selecting its next implementation; the former TACO worker resumes NAME_ENT/TACO with separate file ownership. The user rejected the recent low-yield method. `sndi` corrected buffers and found-flag semantics are preserved at83439c5aa; subsequent Astra/Sol induction and stack-home improvements remain unmatched and are being checkpointed. Luna and other coordinators remain idle. Root centralizes integration and all27 checks; diagnostic archives earn no coverage.
