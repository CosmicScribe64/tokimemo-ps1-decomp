---
id: T-28007
title: Complete text evidence and prepare translator handoff
status: In Progress
assignee: Astra text completion agent
created: 2026-10-25
updated: 2026-10-25
links: ["[[assets-text]]", "[[tickets/T-28001-text-script-export]]"]
---

## Goal

Finish the remaining original-text reader/context work and give the user a checked, practical package they can translate outside this repository. The user has a translator ready and requests notification as soon as the handoff is usable.

## Acceptance criteria

- [x] Independently checked first handoff contains verified Japanese source, stable IDs, editable empty translation fields, script grouping/context and protected substitutions/control tokens, with explicit review status and concise editing instructions.
- [x] Handoff preserves all current source rows and bytes in the evidence export; unresolved entries are separate from the initial translation queue. Counts describe records rather than unique dialogue claims.
- [x] Synthetic round-trip tests and an actual original-input export prove IDs/source/control integrity and detect accidentally edited protected fields. No original textbox capacity is enforced on translations.
- [ ] Remaining text extraction, native renderer/consumer context, runtime substitutions and SPEAKTST uncertainties are addressed with evidence or retained as specific open engineering items; no inventory-only completeness claim.
- [ ] Actual tools and exports pass independent Standards and source/spec review before acceptance. Original data remains immutable and generated text stays ignored.

## Notes

The first translator handoff can precede complete engineering coverage. The current 55,403-row export has 31,346 ready-for-review rows and 24,057 flagged rows, including conditional SPEAKTST views. These counts are not a count of unique translated lines. Existing source/control/bytes/identity evidence must remain available without burdening the editable translation view.

Root serializes canonical writes and publication; Astra prepares finite ignored packets and proof. No actual translation or copied fan translation is requested. This work runs alongside the Godot port without modifying matching game sources.

## Comments

2026-10-25: user explicitly requested an Astra agent for the remaining text work and immediate notification when the translator handoff is ready. Prioritize a reviewed usable handoff, then continue semantic coverage work. T-28001 remains open.

2026-10-25: first handoff accepted after independent Standards and Spec reviews, root archive/source checks and actual installed-tool replay. Generated directory `.asset-cache/text/translator-handoff-v3/` contains 332 UTF-8 TSV batches of at most 400 records: 31,346 ready-for-review and 24,057 unresolved. All 55,403 IDs are unique, targets/notes start empty and full original catalog/units remain exact compressed evidence. Actual 19 synthetic tests and all 332 blank return batches pass; independent actual-ID long Unicode/multiline and note-only returns preserve source/controls and reject protected edits. User notified promptly. They prefer repository/worktree operation rather than ZIP delivery; generate an editable ignored copy in their worktree using the documented exporter. The frozen first review delivery remains a reference. Astra continues the remaining native/context work; comprehensive coverage is still false and this ticket stays In Progress.

2026-10-25 DATE context integration: both independent review axes pass the three-file additive exporter/profile/test packet. Root applied exact reviewed hashes, passed fourteen installed tests and replayed all 293 sections / 5,237 existing IDs from `disc/files` plus the immutable source witness into `.asset-cache/text/date-context-current/`. The initial command used the wrong input directory and failed before output; replay with the actual extracted source directory passed. Existing units, Gemini's editable translations and return contract are unchanged. Runtime speaker/loader/lifetime and remaining coverage stay open. See [[assets-text]].

2026-10-25: user reports Gemini completed all nonempty handed-off rows, 508 supplemental disc strings and image-text/layout proposals. Root located the translation worktree and independently validated all 332 batches against immutable main units: 55,403 returns, 54,786 targets; supplied and rebuilt overlay hashes both `28c2c8fbf6f31b8a56f8415328c94fc3aebed39dbe96e114096193602c4dd533`. Source/ID/control-marker contracts pass. Supplement and image reconciliation remain in independent audit; structural acceptance does not promote unresolved source contexts or prove linguistic/whole-corpus coverage. No translation files were edited or applied to the port.
