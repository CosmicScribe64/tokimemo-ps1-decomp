---
id: T-24017
title: Port three exact opcode clones
status: In Progress
assignee: Codex
created: 2026-10-17
updated: 2026-10-17
links: []
---

## Goal

Port EVENT func_80108B24 and GEKO func_8013FD88 (476 bytes each) from matched ENDING func_80139B48, and SHUGAKU func_80133038 (176 bytes) from matched DATE func_80149F48 / GEKO func_80137580. Exclusive source files: src/ovl/EVENT/80107FC0.c, src/ovl/GEKO/8013F360.c, src/ovl/SHUGAKU/80132000.c. Base 0f2b9818f8eff7c5c1ccb6a2f306a87ccb120444.

## Acceptance criteria

Unguarded C with no target INCLUDE_ASM, default linked literal proof and affected overlay checksum for every win. Docker tools, one compiler/ninja -j1, no toolchain exceptions, no push/merge. Independent standards review before Done.

## Notes

Read wiki/index, prior negative evidence and matched sibling recipes from T-20930, T-17205 and T-22010. Twenty-minute bound. Root fresh census build/complete-siblings.json.

## Comments

## GEKO checkpoint

func_8013FD88 matches 476 default linked literal bytes as unguarded C; no target INCLUDE_ASM. GEKO SHA1 check passes after ninja -j1. Exact ENDING SDK polygon sibling port with inherited FAKE masks/grouped colour trios, no new toolchain behavior. See [[geko-family-results]].

## SHUGAKU checkpoint

func_80133038 matches 176 default linked literal bytes as unguarded C, old guard/fallback removed. Exact DATE copy-chain recipe, no data-island or header changes and no FAKE. SHUGAKU SHA1 passes after ninja -j1; four pre-existing pointer warnings outside target remain. See [[shugaku-family-results]]. Total checkpoint 2 of 3 / 652 bytes.

## EVENT checkpoint

func_80108B24 matches 476 default linked literal bytes as unguarded C, no target INCLUDE_ASM. EVENT SHA1 passes after 22 header-dependent build steps. Legacy helper/global names retained, buffer stride64; shared main_api declarations and unchanged-layout EventStateView prefix fields coordinated with root. Inherited FAKE colour masks/grouped trios only. See [[event-family-results]] and [[data-types]]. All three targets now match: 1128 new verified bytes. Root will run all27 shared-header integration checks.
