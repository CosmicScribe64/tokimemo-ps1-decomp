---
id: T-30002
title: "Provide comprehensive modern settings"
status: Backlog
assignee:
created: 2026-10-25
updated: 2026-10-25
links: ["[[phase3]]"]
---

## Goal

Planned modernization; detailed graphics, input, audio and translation settings. Track after the faithful Phase 3 port; no implementation started.

## Acceptance criteria

- [ ] Provide display/window/fullscreen, resolution, aspect ratio, scaling/filtering, V-sync and applicable graphics options with safe apply/revert behavior.
- [ ] Provide keyboard/controller remapping, device prompts, deadzones and accessible navigation; persist settings and support restoring defaults.
- [ ] Provide master/music/voice/effects volume, text speed and appropriate accessibility options.
- [ ] Provide UI/text language and validated translation-pack selection, visible pack version/credits and original-language fallback; preserve stable content IDs and saves.
- [ ] Expose separate voice-language selection where audio exists, translated image-text variants where supplied, and a clear fallback for missing content. Never imply that selecting English supplies an English dub.
- [ ] Coordinate with the user-owned Gemini translation pipeline and later Phase 5 image replacements; do not modify translator work as part of tracking.

## Notes

Reviewed T-30005 recommendations: keep settings separate from simulation/save state; provide mouse, keyboard and controller navigation with visible focus and digital slider controls. Risky display changes need explicit Keep/Revert, a preselectable longer confirmation interval, focus-loss/timeout rollback and crash recovery to the last confirmed display. Validate and atomically persist settings in user storage. Show only graphics options verified on the actual renderer/platform; engine readback alone does not prove V-sync behavior. Language, image-text packs and voice selection remain independent, with explicit missing-content fallback. These are design inputs, not implemented settings.

Requested by the user on 2026-10-25. These additions do not replace full original-game port acceptance.

## Exploratory English voice pack

The user raised modern voice cloning as a possibility, not an implementation request. Explore an optional English dub with independent Japanese/English voice selection after the original audio works. Assess emotional performance, scene context, pronunciation, speaker consistency, edited translation alignment and timing on a small representative sample before any full production decision. Original-performer cloning requires permission; licensed synthetic voices or newly recorded performances are alternatives. Preserve Japanese audio and explicit missing-line fallback. No voice generation, performer cloning or service upload is authorized by this note.

User follow-up: follow modern game conventions with mouse/keyboard/controller navigation, clear Display/Graphics/Audio/Controls/Accessibility/Language categories, safe timed display confirmation, per-category defaults and persistent settings. Research is tracked in [[tickets/T-30005-widescreen-settings-feasibility]].
