---
id: T-39027
title: Frame pacing and hitch attribution receipt
status: In Review
assignee: playtest-qa agent
created: 2026-10-30
updated: 2026-10-30
links: ["[[phase4-design-2026-10-29]]", "[[playtest-pacing-checklist]]", "[[tickets/T-30001-modern-widescreen]]", "[[tickets/T-30002-modern-settings]]", "[[tickets/T-39004-english-asset-completion]]", "[[tickets/T-39008-synchronized-differential-captures]]"]
---

## Goal

WP-08: compare the original's vsyncs-per-iteration cadence per opening substep with the port's, and record host pacing metrics labelled by tier.

## Acceptance criteria

- [x] Report compares native vsyncs-per-iteration histograms (emulator per-vsync memory samples) with the port cadence per substep; iteration count and multiplicity must match (`tools/port/frame_pacing_report.py`).
- [x] Host metrics: process/render p50/p95/p99, hitches over 1.5x and 3x median with MDEC/STR/XA/VAB attribution, BGM underruns, launch-to-first-interactive; relative-only under Xvfb.
- [x] Manual T4 checklist in [[playtest-pacing-checklist]].
- [x] Unit tests on synthetic samples (15, negatives included).
- [ ] Service event hooks for attribution (needs the core-port owners; see the checklist page).
- [ ] Code review against CODING_STANDARDS.md.

## Notes

Files: `port/godot/tests/support/pacing_probe.gd`, `port/godot/tests/opening/run_presentation_pacing.gd`, `tools/port/frame_pacing_report.py`, `tools/port/test_frame_pacing_report.py`, `tools/port/run_presentation_pacing.sh`, `wiki/playtest-pacing-checklist.md`.

First run (pad 2): iteration counts equal on all 104 substeps; 19 substeps differ in vsync multiplicity. These are the original's load stalls, consistent with D3. Details are on the checklist page.

## Comments
