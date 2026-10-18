---
id: T-24068
title: Match ETC album interaction
status: In Progress
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[etc-family-results]]", "[[tickets/T-13103-etc-memory-card-family]]", "[[tickets/T-17300-types-first-etc]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal
Recover only ETC func_80140AE4 (224 bytes) in src/ovl/ETC/8013FF80.c. Preserve every other function, verified T-24064 model loop and unresolved T-24067 album-copy evidence.

## Acceptance criteria
- Reuse clean built checkout from 732e7c96d on codex/complete-etc-album-interaction; primary main6d1d52101 still has target INCLUDE_ASM and parent exclusively assigned it.
- Read current wiki and T-13103/T-17300 plus earlier 48-row controls before edits. Use siblings and target-driven type/control-flow evidence, not repeated blind sweeps.
- One existing GPT-6.1 Sol medium worker/compiler, Docker only and serial ninja -j1; no additional agents or Astra.
- Report exact shared-header/config scope before mutations. Actual C requires unguarded default body, explicit default linked literal proof, ETC checksum and interface guards plus unchanged-body audit.
- Parent owns independent review/integration; ticket remains In Review until reviewed.

## Notes
Prior GameState pointer transfer remains48 rows; ordinary switch controls54/58. Earlier unsigned-compare constant sharing, selector/result widths, clamp locals and temporary-order experiments failed. Do not repeat without concrete new evidence. T-24064 is independently published252-byte C; its other deferred scope is superseded only for this assigned function. T-24067 remains unresolved and receives no probes.

## Comments
Parent requests function-by-function work using the same sole worker. Clean checkpoint732e7c96d and independently published main6d1d52101 confirmed before starting; primary source target still assembly.

Fresh current baseline reproduces48 resolved rows,56 words/frame0x28, with real unk_F80 and documented unsigned byte compare. Required-call comma sequencing is neutral48. One DATE/EVENT-inspired neighboring selector read (D8+4=DC, word width/alignment preserved, FAKE unknown aggregate) is48 and flips another comparison operand; rejected without a base/type sweep. Existing sibling report has none>=0.70. Source-only body/replay/provenance retained in drafts/T-24068; production unchanged.

Existing read-only allocator tracer passes same-input whole-object identity SHAe4ce1543013767591dacbbed4d00f4889d9b3595e1773b10c82a549c92aba6ee. Selector bit0 priority0.5 takes cheapest cost0 color3(a0); constant1 bit27 priority0.666666687 takes color2(v1) cost1 first. Not an equal-priority tie. Trace only gives best costs, so v0 call-result conflict/liveness remains a specific question; no new source sweep or compiler changes. Ticket remains In Progress.

Parent approved exactly one target s32 fallthrough/no-return control: retained T-9560 totals and T-17300 void body do not establish whether this precise view was exhausted. Callee prototypes/all other choices fixed; result is neutral48 with exact baseline register differences, rejected/FAKE diagnostic retained via replay --variant target-return. No width/result sweep follows. Current tracer cannot distinguish v0 forbidden-mask interference from higher call cost; read-only source identifies exact mask/candidate-cost observation points, awaiting explicit narrow logging scope before any diagnostic binary build. Production unchanged, In Progress.

Parent approved exactly bit0/colors1..3 logging in a separate private binary. Whole-object and code/data/relocation fidelity PASS (samee4ce1543...; sections4048/3712/64/120). Mask60000000/0 forbids v0/v1 before cost evaluation, while a0 allowed/cost0. Static updateforbidden unions block register masks; selector units bb3/bb11. Specific fixed-range contributor is not identified by current logs. Derived ONE proposed C89 source control explicitly casts only final leading check result to void, retaining true callee prototype/all calls, to test discarded-result restriction before bb3. No result/production change claimed; source-only generator/hash/audit retained in drafts/T-24068.

Parent-approved explicit final-call void discard is neutral48 and whole-object identical to baseline; rejected and retained as replay --variant discard-final. No result/width sweep or instrumentation expansion. Next work is existing trace/source mapping only to identify bb3/bb11's fixed v0 contributor; any missing observation/scope must be reported before a new logging build. Production remains unchanged, In Progress.

Parent authorized one helper-only logging extension for bit0/bb3/11. Separate private binary passes pinned same-input whole-object and four-section equality before interpretation. updateforbidden receives bb3/class1/precolor0 source40000000/0 and adds forbidden v0; bb11 source0/0 adds nothing. Exact marker creation inside bb3 remains unobserved. One approved label-only boundary after E80/before selector is neutral48 and whole-object identical, rejected without a goto/label sweep. drafts/T-24068/trace_blocks.py and replay --variant label-boundary retain source-only provenance. No production, header, config or allocator change; no match/verified bytes. Next gap is the creation of bb3's fixed-v0 mask and a legal required-expression route, not a supported new source probe. In Progress pending parent direction.

Bounded read-only generated-source analysis finds makelivranges mask initialization at468BD4 (BB+52/+56 from1001E660) and candidate explicit register writers469748/46A230. The latter opcode0x64 path computes class/register from statement fields before ORing its mask; execution for bb3 is not observed. R0 offset2 bit4 alone does not establish E80's discarded return as the contributor. Smallest missing observation is creating writer PC plus statement opcode/memory type/offset for bb3. No new supported C89 source control, instrumentation build or production change; precise gap reported to parent, ticket In Progress.
