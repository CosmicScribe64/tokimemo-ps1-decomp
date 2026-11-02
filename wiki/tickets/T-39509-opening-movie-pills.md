---
id: T-39509
title: Replace the heroine name pills in the opening movie
status: In Review
assignee: claude
created: 2026-11-01
updated: 2026-11-01
links: ["[[tickets/T-39500-english-translation-patch]]", "[[tickets/T-39508-english-art-on-disc]]", "[[movie-patch]]", "[[english-build]]"]
---

## Goal

The opening movie `CDROM/OPENXA.STR` introduces each heroine with two pink pills drawn into the video: her name and her voice actress. The English pills are drawn (checkpoint `T39004-movie-pills-v1`). The owner asked on 2026-11-01 for them to ship: re-encode only the frames that show the pills, keep the interleaved XA audio and the stream timing intact, and write the movie into the English disc build.

## Acceptance criteria

- [x] Every frame showing a pill is found and decoded; the English pill is composited at the right place for each frame (including fades and motion).
- [x] Re-encoded frames fit their sector budget, or the stream is rebuilt with the same audio sector placement; untouched sectors stay byte-identical.
- [x] The English disc build writes the movie; the emulator plays the opening with the English pills, in sync with the audio.
- [x] The default build still matches 27/27.

## Notes

Part of [[tickets/T-39500-english-translation-patch]]. Method and results: [[movie-patch]].

- Code: `tools/patch/movie/` (stream.py, mdec.py, pills.py, build.py), `Disc.replace_stream` in `tools/patch/disc.py`, the step "opening movie" in `tools/patch/build_english.py` (`--no-movie` skips it), tests `tools/test_patch_movie.py` (18 tests: identity round trip of a synthetic stream and of OPENXA.STR, video-only disc write, decoder against the model, encoder, frame splice, opacity fit, frame selection, moved caption, compositing).
- 204 frames changed (STR 251-498), 14,179 macroblocks, 2,678 video sectors rewritten, 9,757 sectors identical (all 1,515 audio sectors). No frame needed a coarser setting; frames stay inside their 13 or 14 sectors.
- PSNR: pill texels median 23.9 dB (min 21.8) against the composite; untouched texels inside changed macroblocks median 26.9 dB (min 23.1) against the original; untouched macroblocks bit-identical.
- Emulator: from states at the opening `movie_play` call, shots at the same offsets on both discs show the same frames, English pills in place (`build/emu/movie-compare/`). Movie end 4,902 vsyncs (original) and 4,901 (English) after the start.
- Pills not replaced: none. One caption texel of cast-6 lies outside the opaque English pill. The movie's Japanese title logo (decoded 186-246) is out of scope.
- Default build 27/27 OK.

## Comments

- 2026-11-01 Codex: added SHA-256 verification of the entire encoded movie extent after the final disc write, including audio sectors. The report only promotes movie artwork after this check and complete registered-image coverage; container matching accepts the inventory's `CDROM/` prefix and fails if no item matches. Real build readback passed; mutation regression fails as expected. Existing movie playback evidence above remains applicable to the unchanged encoded movie bytes.

- 2026-11-01 claude: implemented and verified; waiting for code review against `CODING_STANDARDS.md`.
