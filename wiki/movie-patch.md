---
type: tool
updated: 2026-11-01
sources: ["tools/patch/movie/stream.py", "tools/patch/movie/mdec.py", "tools/patch/movie/pills.py", "tools/patch/movie/build.py", "tools/patch/disc.py", "tools/test_patch_movie.py"]
---

# English cast pills in the opening movie

`OPENXA.STR` shows each heroine's name and voice actress as pink pills burnt into the video. The English disc build re-encodes the macroblocks under those pills with the English art of the T-39004 checkpoint `movie-pills-v1` and writes the movie back into its own sectors. Ticket [[tickets/T-39509-opening-movie-pills]], part of [[english-build]].

## Running

```
IMG=tokimemo-patch tools/docker.sh python3 -m tools.patch.movie.build --frames   # about 70 s, alone
IMG=tokimemo-patch tools/docker.sh python3 -m tools.patch.build_english          # step "opening movie"
tools/docker.sh python3 -m unittest tools.test_patch_movie -v                     # synthetic + disc tests
IMG=tokimemo-patch tools/docker.sh env TOKIMEMO_MOVIE=1 python3 -m unittest tools.test_patch_movie -v
```

The CLI writes `build/en/movie/OPENXA.raw` (the new raw extent), `report.json` and with `--frames` one PNG per changed frame (original, composite, decoded). `build_english.py --no-movie` keeps the original movie. All outputs are game data.

## Stream facts (checked on read)

- `OPENXA.STR`: LBA 2080, 12,435 sectors, 829 frames of 288x192, BS v1, quantiser 1-7 per frame.
- Pattern: seven video sectors, then one XA audio sector (Form 2, submode 0x64, EDC 0). The 39 sectors after the audio ends have an empty subheader. The last video sector has the EOF bit.
- Video sectors are Form 1, submode 0x42, data: `0x0160`, `0x8001`, chunk index, chunk count, frame number (from 1), frame bytes (BS stream rounded to 4), width, height, a copy of the 8-byte BS header, 4 zero bytes, then 2016 bytes of frame data, zero after its end.
- Every frame has 13 or 14 sectors (26,208 or 28,224 bytes), whatever its size; the pill frames use 16,960-24,152 bytes. A new frame therefore fits its own sectors, and the interleave, the audio and the timing do not change.
- The player (`movie_play`, `src/main/movie_player.c`) has two VLC buffers 0x36000 bytes apart; the build checks each new frame's run/level size against that.

## Method

1. `stream.Stream` reads the extent, `frame_data`/`rewrite` give and replace a frame's bytes. Rewriting every frame with its own data gives the identical extent (test). `disc.Disc.replace_stream` accepts a stream of the same size where only video sectors differ, each with its original sync, address and subheader and a valid EDC/ECC; an audio or other sector that differs is refused.
2. `mdec` decodes with the fixed-point model of `tools/assets/image_mdec_model.py` over numpy arrays (identical to the model on every tested block, 8-bit channels as in 24-bit mode).
3. `pills` takes the cue table (8 cues, 29 pills, decoded frame i = STR frame i + 1). The pills do not move: the position search (+-2 texels) finds no shift in any frame. Each cue dissolves over about three frames each way, over a background that moves and cuts. The opacity per frame comes from a fit of high-pass luma to the Japanese caption (median of the full frames) and a caption-free frame; neighbouring cues are fitted jointly where one dissolves out as the next dissolves in. The composite removes the Japanese caption at that opacity and lays the English pill at the same opacity (formula in the module docstring).
4. `build.encode_frame` re-encodes only the macroblocks the English pills touch, at the frame's quantiser, starting from the original levels; a luma block whose texels do not change keeps its levels. `tools/assets/mdec_splice.rebuild` splices them in and keeps every other macroblock's bits. A frame that does not fit would be encoded again with a dead zone and then coarser steps for the changed macroblocks (`RATE_LADDER`); no frame needed it.
5. Every changed frame is decoded again from the new sectors and compared with the encoder's own decode; every non-video sector is compared with the original.

## Results (2026-11-01)

- 204 frames changed (STR frames 251-498 in eight runs), 14,179 macroblocks, 2,678 video sectors rewritten; 9,757 sectors byte-identical, among them all 1,515 audio sectors.
- Quality: pill texels against the composite PSNR median 23.9 dB (min 21.8); untouched texels inside changed macroblocks against the original decode median 26.9 dB (min 23.1); untouched macroblocks are bit-identical. The low pill figure is the frame quantiser (4-7 in most pill frames) on 1-texel white lettering over saturated magenta, with chroma at half resolution; the lettering stays legible.
- Opacity fits: caption-free frames fit 0.00 +- 0.03, the dissolve frames about 0.3 / 0.7 / 1.0. One caption texel (cast-6) is not under an opaque English texel.
- Emulator, states at the second `movie_play` call (frame 7208 Japanese, 7220 English, the prologue before it differs): shots at the same offsets show the same movie frames with the English pills; the movie ends (`vram_clear_framebuffers` from `movie_play+0x488`) 4,902 vsyncs after the start on the original and 4,901 on the English disc.
- The xdelta patch grows from about 1.4 MB to about 6.0 MB (the rewritten video sectors).

## Open

- The English art (from T-39004) is brighter than the original pills; the art is unreviewed.
- The title logo of the movie (decoded frames 186-246) is Japanese and is not part of this ticket.
