---
id: T-24016
title: Recover BUNKASAI festival state machines
status: In Progress
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[bunkasai-family-results]]"]
---

## Goal
Recover matching default C for func_80135C4C and func_80158764 in the existing complete-bunkaken worktree without changing other functions.

## Acceptance criteria
- [ ] Inspect previous drafts and recover plausible types and sibling forms.
- [ ] Verify any promoted C with literal default funcdiff and BUNKASAI checksum in Docker.
- [ ] Preserve useful source-only drafts and negative evidence; leave In Review for root review.

## Notes
Bounded 25-minute pass, single compiler and ninja -j1. Starting head 15133a84a6c4803c6a5064a2b0831d37252b3d60. Targets remain INCLUDE_ASM. Previous best scores: 264 and 332.

## Comments

### First match
func_80135C4C: unguarded default C, 1,508 literal target bytes (census size, rather than the 1,664-byte queue estimate). Direct dialogue-field updates replace the redundant signed temporary; GameState pointer form follows sibling code. Correct table entry is unk_66C[1], address 0x800E68F0. No FAKE, prototype change, or build adjustment.

Docker verification: `ninja -j1 build/ovl/BUNKASAI.ok` reports BUNKASAI.bin OK; `python3 tools/funcdiff.py func_80135C4C --unit BUNKASAI --resolve --no-build --version default` reports compared default build, linked function bytes equal target (1508 bytes).

Production-install correction: bulk_m2c.insert_draft adds NON_MATCHING, unlike fresh_context used by the scorer. Initial production proof therefore still read the fallback and was not C evidence. Removed the guard and fallback from the actual source, then repeated ninja -j1 (CC/LD/OBJCOPY ran): BUNKASAI checksum OK and literal default linked funcdiff MATCH (1508 bytes). Only this second proof establishes default C progress.
