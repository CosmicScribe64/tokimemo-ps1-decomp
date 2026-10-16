from pathlib import Path
import subprocess
files=['src/ovl/BUNKAKEN/80137640.c','src/ovl/BUNKAKEN/80146560.c','src/ovl/BUNKAKEN/80150D30.c']
for f in files:
 p=Path(f)
 s=p.read_text().replace('/* bulk_m2c declarations (T-11190) */\n/* end bulk_m2c declarations */\n','')
 p.write_text(s)
p=Path('wiki/tickets/T-14501-remaining-draft-proof.md')
p.write_text('''---
id: T-14501
title: Verify and integrate remaining-function drafts
status: In Review
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[matching-notes]]", "[[tickets/T-14500-all-remaining-drafts]]"]
---

## Goal

Promote ordinary C from the exhaustive T-14500 draft census only after default linked-byte proof and the enclosing binary checksum pass. Preserve existing C.

## Acceptance criteria

- [x] BUNKAKEN: 12 functions / 8,384 bytes verified using Docker `tools/funcdiff.py --resolve --no-build`, without `NON_MATCHING`.
- [x] BUNKAKEN overlay SHA1 passes.
- [x] Header/global checks and prototype branch check pass.
- [x] Existing C preserved; source diff replaces only 12 INCLUDE_ASM lines.
- [ ] Required code-review skill axes completed before Done.

## Notes

BUNKAKEN functions: func_80137804 (564), func_80139184 (480), func_80139364 (488), func_80146700 (732), func_801469DC (688), func_80148094 (788), func_80148594 (584), func_80148A28 (1,088), func_801513E8 (848), func_80151738 (832), func_80151EC0 (500), func_80152300 (792).

Local evidence: build/t14500/rows.json, build/t14500/proof.txt, /private/tmp/t14500-proof-build.log, /private/tmp/t14500-protos.log. Generated drafts remain in the separate T-14500 catalog. No asm or game data added.

## Comments

2026-10-15: Claimed proof lane under parent T-14500. Focused CODING_STANDARDS.md inspection: C89, fixed-width local types, ordinary state-machine C; no forced registers, dummy operations, oversized locals, or FAKE patterns. No new declarations or overrides. Removed empty generated declaration-marker comments. Ticket remains In Review for the full code-review gate. Prototype check: zero new disagreements, 15 known.
''')
p=Path('wiki/kanban.md'); s=p.read_text().replace('## In Review\n','## In Review\n\n- [ ] [[tickets/T-14501-remaining-draft-proof|T-14501 Verify and integrate remaining-function drafts]]\n',1);p.write_text(s)
p=Path('wiki/index.md');s=p.read_text().replace('## Tickets\n','## Tickets\n- [[tickets/T-14501-remaining-draft-proof|T-14501]] Verify exhaustive draft candidates: BUNKAKEN 12 literal matches / 8,384 bytes (In Review).\n',1);p.write_text(s)
p=Path('wiki/log.md');p.write_text(p.read_text()+'''\n## [2026-10-15] ticket | T-14501 proof lane claimed and first batch ready for review

Created [[tickets/T-14501-remaining-draft-proof]] for promotion of T-14500 census candidates. Twelve BUNKAKEN functions (8,384 bytes) pass Docker default linked literal-byte proof; build/ovl/BUNKAKEN.bin SHA1 passes; prototype branch check reports zero new disagreements. Existing C is preserved. Focused standards inspection found ordinary state-machine C and removed empty generated declaration markers. Status In Review pending the required review gate; [[kanban]] and [[index]] updated.
''')
