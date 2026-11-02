---
id: T-39509
title: Replace the heroine name pills in the opening movie
status: In Progress
assignee: claude
created: 2026-11-01
updated: 2026-11-01
links: ["[[tickets/T-39500-english-translation-patch]]", "[[tickets/T-39508-english-art-on-disc]]"]
---

## Goal

The opening movie `CDROM/OPENXA.STR` introduces each heroine with two pink pills drawn into the video: her name and her voice actress. The English pills are drawn (checkpoint `T39004-movie-pills-v1`). The owner asked on 2026-11-01 for them to ship: re-encode only the frames that show the pills, keep the interleaved XA audio and the stream timing intact, and write the movie into the English disc build.

## Acceptance criteria

- [ ] Every frame showing a pill is found and decoded; the English pill is composited at the right place for each frame (including fades and motion).
- [ ] Re-encoded frames fit their sector budget, or the stream is rebuilt with the same audio sector placement; untouched sectors stay byte-identical.
- [ ] The English disc build writes the movie; the emulator plays the opening with the English pills, in sync with the audio.
- [ ] The default build still matches 27/27.

## Notes

Part of [[tickets/T-39500-english-translation-patch]].

## Comments
