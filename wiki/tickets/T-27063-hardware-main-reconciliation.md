---
id: T-27063
title: Reconcile main flow and hardware entry points
status: Done
assignee: parallel-chat
created: 2026-10-24
updated: 2026-10-25
links: ["[[phase2]]", "[[tickets/T-27000-phase-2]]"]
---

## Goal

Prepare body-backed, independently reviewed executable symbol packets and complete port specs for m-flow, h-graphics, h-cdio, m-dates-a, m-dates-b. Root alone applies sources and owns shared tracking/builds/commits. Existing draft rows are leads, not trusted evidence.

## Acceptance criteria

- [x] Resolve current ledger precedence, source ownership and collisions; inventory every in-scope definition or justified hardware exception.
- [x] Supported symbol-only tables plan cleanly in Docker and pass independent Standards/Spec review.
- [x] Complete essential local/parameter/constants site inventory, source-backed transitions and persistent effects in specs.
- [x] Root application, matching proof and final review before Done.
## Comments

2026-10-24: User authorized parallel chats, Luna for bounded work and Sol6.1 for harder analysis/review. No direct Godot work. No source apply/build/commit in this lane.


## Packet reconciliation and frozen handoff

Current state: **In Progress**. The five assigned apply/essential packets are inventoried below. My earlier Docker plan attempts were interrupted and produced no result for these packets. Root owns application, any plan/build validation, and commits. The separate reported 403-overlay file-only plan is not evidence of validation for these main-unit packets.

A prior Sol 6.1 review passed the individual packets, but the later cross-unit/spec audit found one aggregate alias conflict and stale living-spec/map references. Those findings are corrected in `m-flow-apply.tsv`, `m-flow.tsv`, `wiki/spec/m-flow.md`, `wiki/spec/m-dates-b.md`, `src/main/README.md`, `wiki/spec/h-cdio.md`, and `wiki/spec/m-dates-a.md`. The D_80122D44 standalone row is deferred; it remains documented as TalkWork.unk_74 at +0x74 with direct-reference migration left open. Final independent Sol 6.1 Standards/Spec review: **PASS** for the corrected safe-subset packets, current prefix map and reviewed spec corrections, with no outstanding findings. One stable Docker batch ran all five apply plans: m-flow (169 rows/56 files), h-graphics (60/110), m-dates-a (95/12), and m-dates-b (71/117) exited cleanly. h-cdio reported 101 rows/107 files but exited 1 because two existing `dec_bg_*` labels were still listed by address; the apply table now uses the existing labels. A targeted h-cdio re-plan is still required. Root application and matching proof remain outstanding.

| Unit | Apply inventory | Essential inventory |
|---|---|---|
| m-flow | 87 funcs, 30 data, 44 fields, 1 type, 6 params, 1 local | 24 constants |
| h-graphics | 35 funcs, 23 data, 2 fields | empty |
| h-cdio | 41 funcs, 44 data, 4 types, 14 fields | 15 params, 24 const sites, 27 definition comments, 2 unresolved notes |
| m-dates-a | 52 funcs, 8 data, 1 type, 34 fields | 13 params, 4 locals, 66 const sites, 1 definition comment, 5 unresolved notes |
| m-dates-b | 20 funcs, 29 data, 22 fields | 25 params, 10 locals, 21 SpeakCode, 11 NameCode, 3 TextMark, 7 SceneWipe, 2 GirlMenu, 2 PortraitFile, 4 SdSet, 4 SceneTiming rows |

Proposal file SHA-256:

- `m-flow-apply.tsv`: `7053bc50c790e0e4ac8e291602bde3d504125249d759c764b04ed018c6761d6c`
- `m-flow-essential.tsv`: `37526062ac1657778707d956a4b14e73ffb332826ec3645367990db83652c155`
- `h-graphics-apply.tsv`: `808192e49738ba1b8dbc3a73d19e8f1b8a4af43591859da236401d18acad6b04`
- `h-graphics-essential.tsv`: `16890c0c1494b72cb4d634d43420856d2e6e84953fd396877295214fc7adfc2e`
- `h-cdio-apply.tsv`: `5d9429158d677ee90b0e4f10ae83aeb17be57a5d375f864837436acf1d8428cd`
- `h-cdio-essential.tsv`: `b7048489a78650ca64b608779e6b4199856874308fc7d9320245d4b1617a85fb`
- `m-dates-a-apply.tsv`: `57681c0b0580c35269c0089ec9fdd216167e83add0c63e080f8a7094d911ecab`
- `m-dates-a-essential.tsv`: `c379d2fa9f9029c175998b42656a58d74c5ee157ebc970a46137e3534bddbafe`
- `m-dates-b-apply.tsv`: `871aff01cf4a4c662c7135951ca0dd6d7cb785333c99452dc7ac4c071739c058`
- `m-dates-b-essential.tsv`: `479f0bbeb76a21462c75891691fa0b06e876d964cf4d8abeae1da477963db254`

Ignored-scratch handoff artifacts (patches checked with `git apply --check`; source files were not modified):

- `.claude/orchestrator/state/T-27063/README-prefix-ownership.patch` resolves the reviewed ownership policy: one `boot_` startup group including `boot_assets.c`, `debug_` for debug helpers in `debug_display.c` and `debug_hud_palette.c` with nondebug `prim_`/`color_` unchanged, and `audio_` including scene-library CD-level helpers.
- `.claude/orchestrator/state/T-27063/obin-definition-comments.patch` adds 21 comments across 7 files. Reconciliation against the T-27067 inventory: all 57 core candidate comments are already present; h-cdio has 27 rows (7 already present, 20 additions); m-dates-a has one missing row. The 57 list overlaps five background-image comments, counted only once.
- Patch SHA-256: README ownership `80cae3a401a5f3250842bef622cc4471223716cd4280af2a002863b09c1ece5e`; comment patch `30d1fbe1d9739fd46d7dcd5f7b15e18d45ca5d062fc38306adc59b4a3b8384aa`; reconciliation note `35e0da46b48d48fb8a646bc808a829fdbc268cd1ee29c779b4b2b6a3156e9e5b`.

Remaining blockers: root application, packet plan validation, matching proof, and any source-level review required by the acceptance criteria. The O.BIN definition comments remain in the root-review scratch patch; they are not applied source changes. Do not move this ticket to Done until those steps are complete.

## Final integration evidence (2026-10-25)

Reviewed source digest: `687b5ce789d59f154b527264d23227b741a08bc619e4a1f8c67d617257f4c4f3`. Final clean matching proof: 27/27 binaries, 6,952 functions, 2,276,920 bytes. Independent original Astra review and separate fixer Astra review passed this exact source digest. See `.claude/orchestrator/state/T-27000/final-tracking/criterion-evidence.md`.

