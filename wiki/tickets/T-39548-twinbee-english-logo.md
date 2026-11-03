---
id: T-39548
title: Replace TwinBee title lettering with English
status: In Review
assignee: codex
created: 2026-11-03
updated: 2026-11-03
links: ["[[tickets/T-39500-english-translation-patch]]", "[[tickets/T-39510-no-japanese-left-check]]"]
---

## Goal

Replace the Japanese TwinBee title lettering with English while preserving its colorful style, existing animation and English subtitle. The owner explicitly requested the change after viewing the original title screen.

## Acceptance criteria

- [x] Integrate legible English lettering without changing neighboring artwork or sprite attributes.
- [x] Inspect the assembled title and animation in the emulator.
- [ ] Pass release checks, standards review and publication verification.

## Implementation

Generated English lettering and fitted it to the original 192×64 canvas. The writer uses the original CLUT and changes only the 128×64 and 64×64 title pieces in `CDROM/TWINSGD/TBTA.DAT`. Native palette preview is legible. Two source-backed checks pass; the final-disc check is pending the rebuild. The first asset-location probe missed the 150-sector MSF lead-in and failed before writing anything; the corrected probe identifies the container at LBA 48692. Pillow's installed version uses `Image.LANCZOS`, which the preparation step now uses.

The art suite passes 27 checks with the final-disc test deferred until the rebuilt track is ready. Its first invocation lacked the existing art modules' tools import path; rerunning with `PYTHONPATH=/work/tools` completed successfully. The native preview uses only the original palette, with transparent index zero matching the game's CLUT upload behavior.

## Verification and review

The rebuilt target is `b4e84775009f141b335574ae1268e49af5128fb7`. Four final-disc checks pass, including preservation of all twelve TwinBee sprite attribute arrays. The script, executable and generated-source audits pass; all 27 default binaries retain their original hashes. All 15 browser checks pass against the packaged disc.

Inspected eleven native captures covering the entrance, settled title and first-stage gameplay. The lettering is legible, the subtitle is unchanged, and the sprite pieces enter normally. The controlled mode-entry fixture shows texture data during loading; frames 300, 420 and 480 match the earlier build byte for byte. This fixture does not establish a normal menu-to-title transition. Evidence: `translations/en/reviews/twinbee-english-logo-2026-11-03.json`.

Standards review checked the writer, integration and tests against `CODING_STANDARDS.md`. Source hashes are guarded; actual-disc verification rejects unexpected changes; independent boundary tests protect the palette, subtitle and neighboring pixels. The only new image staged is generated English lettering, with no extracted original artwork. No C, symbols or matching code changed. No unresolved findings. Publication readback remains the last gate.
