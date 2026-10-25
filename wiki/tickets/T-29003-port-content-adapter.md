---
id: T-29003
title: "Build the validated original-game content pack adapter"
status: Ready
assignee:
created: 2026-10-25
updated: 2026-10-25
links: ["[[phase3]]", "[[tickets/T-29000-plan-godot-port]]"]
---

## Goal

Reuse existing asset readers to create a versioned external pack for the first route, without copying original payload into engine source.

## Owned paths

- `tools/port/import_content.py`
- `tools/port/validate_content.py`
- `tools/port/schemas/`
- `port/godot/services/content_store.gd`
- `port/godot/tests/content/`

## Acceptance criteria

- [ ] Accept the verified original release/input capabilities and reject wrong versions or missing raw-track dependencies with precise diagnostics.
- [ ] Reuse tools/assets readers; no duplicate decoder or ignored research-file dependency in the production path.
- [ ] Define tokimemo.port-content.v1 with component versions/hashes, source locators, semantic IDs, route capabilities, uncertainty and original byte/render/audio/script metadata.
- [ ] Import read-only inputs into immutable ignored .asset-cache/port content roots; validate before publication and preserve previous packs on failure.
- [ ] Test bounds, corrupt/truncated data, stale or unknown schemas, missing mandatory capabilities, relative-path containment and symlink escapes with invented fixtures.
- [ ] Complete runtime pack loading with T-29001 and frozen T-29002 contracts before Done; Python adapter/schema work can start in parallel.
- [ ] Independent Standards and spec review findings resolved; actual evidence recorded before Done.

## Notes

Suggested bounded owner: gpt-6-luna; escalate a concrete decoder/source ambiguity to Sol. Existing text/media teams retain tools/assets ownership. Consume only reviewed source revisions, including the canonical NBN cell identity and VAB size fixes where a route needs them. Root serializes shared source applications, wiki, staging and commits. Original inputs stay immutable; real exports and captures remain ignored.

## Comments

2026-10-25: created from the independently reviewed Astra Phase 3 plan. No implementation or runtime validation is claimed by this ticket.
