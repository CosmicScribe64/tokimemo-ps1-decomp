---
id: T-0601
title: Review medium and low confidence O.BIN mappings
status: Backlog
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[obin]]", "[[tickets/T-0201-obin-format-and-symbols]]"]
---

## Goal

[[obin]] maps 319 names with medium and 1015 with low confidence (not in `config/symbol_addrs_obin.txt`). Promote or reject them using evidence from decompiled functions (call graph, strings, roles), starting with the ones already spot-checked (`game_main`, `load_func_main`, `check_sum_check`, `cdreaddata2`).

## Acceptance criteria

- [ ] Accepted names go through the splat symbol files; rejected ones are listed in [[obin]].

## Notes

Medium class calibrated at 84% on PsyQ anchors, low at 46%; do not apply either without a second piece of evidence.

## Comments
