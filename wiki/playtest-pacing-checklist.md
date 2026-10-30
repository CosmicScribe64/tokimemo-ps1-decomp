---
type: verification
updated: 2026-10-30
links: ["[[phase4-design-2026-10-29]]", "[[port-testing]]", "[[tickets/T-39027-frame-pacing-receipt]]", "[[tickets/T-39008-synchronized-differential-captures]]"]
---

# Playtest pacing receipt and desktop checklist

WP-08 of [[phase4-design-2026-10-29]] (Playtest), ticket [[tickets/T-39027-frame-pacing-receipt]]. Tiers and modes are defined in [[port-testing#Evidence tiers and modes]].

## The receipt

- `port/godot/tests/support/pacing_probe.gd` observes only. It records one entry per logical tick of the port's nominal 60/1 clock (`core/native_clock.gd`), with frame_count and opening substep, and one entry per presented host frame: interval, process and render time, ticks run in that frame, and MDEC/STR/XA/VAB events and BGM underruns noted during it.
- `port/godot/tests/opening/run_presentation_pacing.gd` is the T-39002 whole-opening frame timing route with the probe attached. It keeps every check of that route.
- `tools/port/run_presentation_pacing.sh <new-out> <pad> [native frames.jsonl]` runs the route under Xvfb, then writes `pacing-report.json` with `tools/port/frame_pacing_report.py`.
- The report compares, per opening substep up to 12/0/0, the iteration count and the histogram of vsyncs per iteration. The native side is the emulator's per-vsync `frames.jsonl`, keyed by the loop-boundary `loops.jsonl` next to it. The port side is the probe's logical ticks. An iteration that ran with no vsync wait (the original's catch-up after a long iteration) counts as a zero-vsync iteration. Gaps and backwards counters are rejected, never resynchronized.
- Host metrics are labelled `relative_only` unless the probe ran on a real desktop (`TOKIMEMO_REAL_DESKTOP=1`, not Xvfb).

## First result (2026-10-30, pad 2, T2 cadence plus Xvfb host metrics)

Inputs: `.asset-cache/emu/opening-timing-v4/fcb-start20` (frames and loops) and the port run in `.asset-cache/phase4-playtest-qa/pacing-pad2-v2`. Report: `.asset-cache/phase4-playtest-qa/pacing-pad2-report-final.json`. The timing route itself passed with the probe attached (104 substeps, 6,709 frames to 12/0/0, 0 mismatches), so attaching the probe did not change frame_count progression.

- Iteration counts match on all 104 substeps (6,709 iterations), and the substep order is equal.
- Vsync multiplicity differs on 19 substeps. In the original, 6,734 vsyncs pass over those 6,709 iterations. Iterations of 2 to 6 vsyncs occur at stage starts (for example 1/1/0, 2/0/0, 6/0/0 and 7/0/0, which take 3 to 5 vsyncs each), and one zero-vsync catch-up iteration usually follows. The port runs exactly one tick per iteration. This is the measured presentation-time cost of the original's loads, and is consistent with the D3 CD/XA service latency known_diff (not yet attributed per stall). Fidelity mode is meant to reproduce it by default; any fix belongs to the D3 sub-ticket, not to this receipt.
- Xvfb/llvmpipe host (relative only): median host frame interval 167 ms, with up to 8 logical ticks of catch-up per host frame. Launch to first interactive frame was 4.8 s. There were 85 hitches over 1.5x the median, all unattributed: the service event hooks are not wired yet. No BGM underruns were recorded, also because of the missing hooks. None of these numbers describe pacing on a real desktop.

## Hooks for the integrating agent

Hitch attribution needs production services to report events. Call `probe.note_event("mdec" | "str" | "xa" | "vab")` and `probe.note_bgm_underrun()` when a probe is attached. A clean hook is an optional `pacing_observer` property, null by default, on `first_arrival_media_bridge.gd`, `prologue_media_queue.gd` and `first_arrival_bgm_bridge.gd`, which those services call when it is set. Those files are core-port files owned elsewhere, so this package does not edit them. With no observer attached, behaviour is unchanged.

## T4 desktop checklist (the user, macOS reference)

Record each item with the date, the build hash, and the probe receipt when the item can produce one. An agent cannot tick these.

- [ ] Native refresh rate of the main display, windowed: opening from title to 12/0/0. Note visible stutter at stage starts and audio/visual sync at XA lines.
- [ ] The same in fullscreen (borderless), then exclusive.
- [ ] A second refresh rate (for example 120 Hz or ProMotion, and 60 Hz). Check that game speed stays the same and only the presentation cadence changes.
- [ ] Frame-rate cap settings 60, 120, 144 and uncapped: game speed unchanged.
- [ ] Focus loss during an XA line and during an MDEC/STR movie: pause and mute behaviour, no catch-up burst on return.
- [ ] Window resize and display change with a pending confirmation, then let it time out: it rolls back.
- [ ] macOS native audio: BGM, SE and XA voice audible and in sync. It stays "unvalidated" until this item is done.
- [ ] Launch to first interactive frame, measured by stopwatch, against the probe value.
