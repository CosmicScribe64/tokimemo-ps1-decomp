---
type: status
updated: 2026-10-22
---

# Status

The one page for "where are we". Update it when a wave, a local or cloud session, or a contributor PR changes the numbers (`tools/docker.sh python3 tools/progress.py`). Older status and handoff pages are in `archive/` ([[completion-run-2026-10-17]], [[stopping-point-2026-10-15]]).

## Latest verified integration (2026-10-22)

Nine functions verified this resumed effort, adding12,160bytes. Latest: MAIN parameter_up_down adds1,228 literal default-C bytes. Root independently checked all27 binary checksums and declaration guards, with no standards/spec review findings. Current game code: **6842/6952 functions;2116836/2276920 bytes (93.0%)**,110 functions/160084 bytes remaining. Active Astra assignments: T-25034 MAIN quadrant; T-25040 MAIN8006D138; T-25042 KANGEI80134724; T-25045 ETC80146D4C; T-25046 SHOUGATU8013EA98; T-25047 TACO801511C0. Parameter worker continues under reserved T-25048. Older counts below are historical.

## NEXT STEP (read this first)

The compiler-lever programme is complete and negative ([[tickets/T-25013-compiler-level-levers]], [[tickets/T-25015-lever-expansion-tooling]]: every lever rejected with evidence, the OPTN space closed, the build proven frozen on the matched corpus). [[tickets/T-25016-candidate-reconstruction]] then rebuilt the candidate pool: the near set stands at 2 to 7 funcdiff rows each with named residues (as1 scheduling outcomes, one register allocation, one constant fold), and the bulk pool is fully classified in the regenerated work queue (79 scored drafts, 43 blocked with named first blockers; packet scores flatter by about 10x, verified, so Tier 0 is the only near-term match path). Next work is per-function reconstruction from that queue, starting with the near set; do not restart the generic salvage passes or the source-shape sweeps, both converged at zero. A match still counts only by the standing rule: funcdiff MATCH plus the full local gate, with formal verdicts from CI or the host build.

## Progress (2026-10-20)

- Game code: 6,833 / 6,952 functions; 2,104,676 / 2,276,920 bytes (92.4%), independently measured in Docker on 2026-10-22 at 7c87f223e.
- Complete units: TT 303/303, DATE2 102/102, and every overlay not listed below.
- Remaining: 119 functions and 172,244 bytes. The previous 129-function unit breakdown was stale; reconcile unit counts with the current census before dispatch.
- What is left is mostly hard tails: guarded drafts a few rows off (compiler ordering and register choices, [[build-step-gaps]]) and large functions without a good draft. Find a function's history in [[notes/functions]].

## Active work

- 2026-10-22: T-25013, T-25014, and T-25015 are Done (merged in PR #66; all levers rejected, tooling landed). T-25016 closed out its census, engine, and bulk phases with no new matches (count unchanged at 6,833) and is In Review as the PR carrying its record. The regenerated work queue is the current map of what remains: Tier 0 near set, Tier 1 scored bulk drafts (a reconstruction backlog, verified 210+ resolved rows at best), Tier 2 blocked drafts with named blockers.

- 2026-10-20 session end: no agents running. Closest guards: main func_8005B43C 3 rows (permuter candidate in notes), GEKO func_80136C20 4, KANGEI x2 2 rows, parameter_up_down 11 (permuter output-252 unchecked), ETC func_80146D4C 12, GEKO func_80133C50 13 (masked-load lead in notes), mod_set_gpu 17.

- 2026-10-20 evening: T-24929 build pass (switch_compares, zero regressions) and T-25005 (several .data ranges per object) landed; matches ETC func_8013CCE4, main sndisp, func_80043510, func_8004443C.

- Plan for the last 127 functions, for any agent: [[plan-remaining]] (classes A-D, list in [[data/remaining-2026-10-20]]).
  Wave C-1 (T-24921..T-24924, isolated worktrees) produced no promotion; kept 9->3 (ETC func_8013CCE4),
  114->92 (ENDING func_801330E4) and two archived drafts (GEKO func_801367EC 44, OPTION func_801326BC 134).
  Wave C-2 (T-24925..T-24928) on the same functions: main func_8006C988 11->5 rows, mechanism
  diagnoses for ETC func_8013CCE4 (uopt zcopy, T-24929), GEKO func_80133024 (two day ranges) and
  BUNKA_SD func_80139290 (same-block order, lever banked in drafts/T-24927/).
  T-24929 closed the ETC func_8013CCE4 gap with a build step (`tools/cvt_pass.py` switch compares, zero
  regressions, [[matching-notes]] T-24929); the function is matched.
  Class A is running as T-24901..T-24908 (one function per file-disjoint worker, 45-minute box).
- as1 scheduling: rule and `volatile` lever in [[as1-scheduler-rule]] (T-24900); per-file options ruled out in [[as1-scheduling]] (T-24800).

- T-24700 fan-out (2026-10-20, before the new plan): no promotions; source-only improvements kept
  in guards: BUNKA_SD func_80139290 123->6, main func_80054E6C 84->19, GYOZI func_80138C9C 103->75,
  ETC func_8014790C 85->57, GYOZI func_8013907C 39->23, main func_800455C4 26->16,
  ETC func_8013CCE4 84->18, NAME_ENT func_801398EC 266->185, GYOZI func_80140E0C 9->5.

- Current handoff: user requested wrapping up and pushing this session. T-24607 OPTION `func_80139A30` adds1,036 literal default-C bytes; all27 hashes and declaration guards pass. Seven functions /6,516 bytes completed in this continuation;129 remain. Matching lanes have stopped, with unfinished evidence preserved in [[matching-notes#T-24600: Wrap-up reconstruction checkpoints]]. The full decompilation remains incomplete; earlier activity entries below are historical.

- T-24606 OPTION `func_8013A7EC` adds1,092 literal default-C bytes with all27 hashes, declaration guards and independent reviews passing. This continuation has completed six functions /5,480 bytes;130 remain. Root continues MAIN matrix reconstruction while agents continue OPTION numeric editing, TACO font rendering and NAME_ENT introduction. Completion goal remains active.

- T-24605 GEKO `func_801420E4` adds1,256 literal default-C bytes, with all27 hashes, declaration guards and independent reviews passing. This continuation has completed five functions /4,388 bytes;131 remain. Root continues MAIN; agents continue TACO, OPTION and NAME_ENT reconstruction. Earlier scratch checkpoints remain preserved without coverage credit.

- T-24604 MAIN `hizuke_init` adds 1,048 literal default-C bytes, with all 27 hashes, declaration guards and independent reviews passing. This continuation has completed four functions / 3,132 bytes; 132 remain. Current disjoint lanes: root schedule sort, agents MAIN flag dispatch, MAIN srn_set and TACO collision reconstruction. GEKO/NAME_ENT/OPTION scratch checkpoints are preserved without coverage credit.

- User-authorized T-24600 fan-out: root owns MAIN; agents own GEKO, NAME_ENT and TACO. T-24603 TACO `func_80153B38` adds 832 literal linked bytes with all 27 hashes and independent reviews passing. This continuation has completed three functions / 2,084 bytes; 133 remain.

- T-24602 GEKO `func_80144198` adds 216 literal linked bytes with the date-result array reconstruction. Full build, fresh 27 executable hashes, and independent standards/spec reviews pass. T-24600 continues with 134 functions remaining; its two completed functions add 1,252 bytes.

- T-24600 completion continues until the full goal is achieved. First verified promotion: T-24601 `get_fnt`, 1,036 default-C bytes; full 27 checksums, header/global/prototype guards and independent standards/spec reviews pass. MAIN `func_8006C988`, `func_80047560` and GEKO `func_80133024` remain unmatched after bounded scratch probes. See [[matching-notes#T-24601: get_fnt literal match]].

- Latest: MAIN `func_8004A814` adds 1,028 literal default-C bytes at `87df21291`; all 27 executable checksums and declaration guards pass, root source review has no findings. Resumed-run total: 23 functions / 24,564 bytes. Sol 6.1 medium continues the GPU family using the confirmed erased-coordinate-conversion behavior; Astra is idle after the bounded consultation. Other workers are idle to conserve usage. Source-only `func_8004A414` remains unmatched at resolved cost 132; no coverage credit for drafts. Earlier campaign entries below are historical.

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
