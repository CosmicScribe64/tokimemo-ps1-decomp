---
id: T-29014
title: Extend original content coverage to ordinary commands 1 through 6
status: In Progress
assignee: port-command-content-agent
created: 2026-10-25
updated: 2026-10-25
links: ["[[phase3]]", "[[tickets/T-29003-port-content-adapter]]", "[[tickets/T-29009-ordinary-week-continuation]]"]
---

## Goal

Make ordinary commands 1 through 6 usable through the same original-input content boundary as commands 0 and 7. Keep native eligibility, resource selection, animation and outcome semantics; do not substitute another command's art or timing.

## Acceptance criteria

- [ ] Resolve each command's background and SD selections from original executable tables and native callers, including applicable seasonal/conditional variants.
- [ ] Extend reusable readers/exporters and strict runtime descriptors with original input identities, actual geometry, palettes, sequence/frame domains and rendering metadata; exports remain ignored.
- [ ] Reject unavailable required variants before gameplay mutation, with exact diagnostics rather than fallback art.
- [ ] Integrate command capabilities and actual route behavior without changing other commands, save identities or RNG order.
- [ ] Review source, actual exports and negative cases independently; compare reachable gameplay checkpoints and rendered output against the original before claiming route acceptance.

## Notes

Prepare an isolated packet first; root serializes canonical application. Holiday content, HUD production export and rest-week/save fixes have separate owners. Reuse verified table readers and media validators. No emulator use without a serialized lease. Full seasonal coverage remains explicit even if the first implementation covers only the opening spring week. Gemini owns translation; original data never enters Git or res://.

## Comments

2026-10-25: opened to advance full-game command coverage in parallel with first-slice integration. An inventory alone is not completion.
