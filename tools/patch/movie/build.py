"""English OPENXA.STR: the cast pills of the opening movie in English (T-39509).

Usage (in Docker):
    python3 -m tools.patch.movie.build [--track disc/orig/track1.bin] [--out build/en/movie]

Steps:
1. Read the stream's raw sectors from track 1 (`stream.Stream` checks the layout).
2. Decode the frames around every caption cue (`mdec`), measure each cue's opacity and position
   per frame and composite the English pills (`pills`).
3. Re-encode only the macroblocks the pills touch, at the frame's own quantiser, and splice
   them into the frame (tools/assets/mdec_splice.py keeps every other macroblock's bits). A
   frame that no longer fits its sectors is encoded again with a dead zone, then with coarser
   steps for the changed macroblocks only (RATE_LADDER).
4. Write the frames back into their own sectors (`Stream.rewrite`), decode every changed frame
   again from the new sectors and check it against the encoder's own decode.

The library entry is `english_stream(track1, lba, sectors)` -> (raw bytes, report). The CLI
writes `OPENXA.raw` (the new raw extent), `report.json` and, with --frames, before/after PNGs of
the changed frames into --out, which is game data and stays out of git.
"""
import argparse
import json
import math
import os
import sys
import time
from pathlib import Path

import numpy as np

from tools.assets import mdec_splice
from tools.patch import sector as cs
from tools.patch.movie import mdec, pills
from tools.patch.movie.stream import Stream

REPOSITORY = Path(__file__).resolve().parents[3]
PATH = 'OPENXA.STR'
SIZE = (288, 192)
# (coarse step factor, dead zone) tried in order until a frame fits its sectors.
RATE_LADDER = ((1, 0.0), (1, 0.15), (1, 0.3), (2, 0.2), (3, 0.2), (4, 0.3))
# The player decodes run/level words into one of two VLC buffers 0x36000 bytes apart
# (src/main/movie_player.c, strSetDefDecEnv: vlcbuf[0] at +0x12D44, vlcbuf[1] at +0x48D44).
VLC_BUFFER_BYTES = 0x36000


class MovieError(ValueError):
    """A frame cannot be written without breaking the stream's layout."""


def process_pool(workers=None):
    """A process pool, imported without tools/ on sys.path: tools/patch/disc.py puts it there,
    and tools/queue.py would then stand in for the standard queue module the pool needs."""
    tools = str(REPOSITORY / 'tools')
    saved = sys.path[:]
    sys.path[:] = [p for p in sys.path if os.path.abspath(p or '.') != tools]
    try:
        from concurrent.futures import ProcessPoolExecutor
        return ProcessPoolExecutor(workers)
    finally:
        sys.path[:] = saved


def decoded(data):
    """(levels, quantiser, H x W x 3 uint8) of one frame's BS data."""
    parsed = mdec_splice.parse(data)
    levels = mdec.levels_array(parsed)
    return levels, parsed['quantization'], mdec.channels(levels, parsed['quantization'], *SIZE)


def _decode_job(args):
    number, data = args
    return number, decoded(data)


def psnr(a, b):
    if a.size == 0:
        return None
    mse = float(((np.asarray(a, float) - np.asarray(b, float)) ** 2).mean())
    return math.inf if mse == 0 else 10 * math.log10(255.0 ** 2 / mse)


def _rounded(value):
    return None if value is None else ('inf' if math.isinf(value) else round(value, 2))


def encode_frame(job):
    """New BS data for one frame. job: (number, data, capacity, target H x W x 3, area H x W)."""
    number, data, capacity, target, area = job
    levels, scale, original = decoded(data)
    blocks = mdec.cut(area[..., None].astype(np.uint8), *SIZE)[..., 0].reshape(len(levels), -1).any(axis=1)
    indices = np.nonzero(blocks)[0]
    goal = mdec.cut(np.rint(target).astype(np.int64), *SIZE)[indices]
    for coarse, deadzone in RATE_LADDER:
        new_levels = mdec.encode(levels[indices], goal, scale, coarse=coarse, deadzone=deadzone)
        replacements = {int(mb): new_levels[k].tolist() for k, mb in enumerate(indices)
                        if not np.array_equal(new_levels[k], levels[mb])}
        rebuilt, facts = mdec_splice.rebuild(data, replacements)
        out = rebuilt[:facts['stream_bytes']['new']]
        out += bytes(-len(out) % 4)
        if len(out) <= capacity and facts['declared_words']['new'] * 4 <= VLC_BUFFER_BYTES:
            break
    else:
        raise MovieError('frame %d does not fit %d bytes even at the coarsest setting' % (number, capacity))
    all_levels = levels.copy()
    for mb, six in replacements.items():
        all_levels[mb] = np.array(six)
    after = mdec.channels(all_levels, scale, *SIZE)
    changed_blocks = np.zeros(len(levels), bool)
    changed_blocks[list(replacements)] = True
    block_area = mdec.place(np.repeat(changed_blocks, 256).reshape(-1, 16, 16, 1), *SIZE)[..., 0]
    kept = block_area & ~area
    facts = {'frame': number, 'quantiser': scale, 'coarse': coarse, 'dead_zone': deadzone,
             'macroblocks_changed': len(replacements), 'bytes': {'original': len(data), 'new': len(out), 'capacity': capacity},
             'psnr_pill': psnr(after[area], target[area]),
             'psnr_kept_in_changed_macroblocks': psnr(after[kept], original[kept]),
             'kept_texels_in_changed_macroblocks': int(kept.sum())}
    return number, out, after, facts


def english_stream(track1, lba, sectors, root=REPOSITORY, workers=None, keep_frames=False):
    """(new raw extent, report, frames) for the stream at `lba` (`sectors` long) of `track1`.
    frames is {number: (original, target, after)} when keep_frames, else empty."""
    began = time.monotonic()
    with open(track1, 'rb') as handle:
        handle.seek(lba * cs.SIZE)
        raw = handle.read(sectors * cs.SIZE)
    stream = Stream(raw, lba)
    numbers = list(stream.frames)
    if numbers != list(range(1, len(numbers) + 1)):
        raise MovieError('frame numbers are not 1..%d' % len(numbers))
    table = pills.load_cues(root)
    if table['source']['decoded_frames'] != len(numbers) or tuple(table['source']['frame_size']) != SIZE:
        raise MovieError('the cue table was measured on another stream')
    needed = [i for i in pills.Captions.needed(table) if i < len(numbers)]
    with process_pool(workers) as pool:
        results = dict(pool.map(_decode_job, [(i + 1, stream.frame_data(i + 1)) for i in needed], chunksize=4))
        frames = {number - 1: result[2] for number, result in results.items()}
        captions = pills.Captions(table, frames, SIZE)
        jobs = []
        for index in sorted({i for cue in captions.cues for i in cue.alpha}):
            target = frames[index].astype(float)
            area = np.zeros(target.shape[:2], bool)
            for cue in captions.cues:
                if index in cue.alpha:
                    target = cue.composite(index, target)
                    area |= cue.area(index)
            number = index + 1
            jobs.append((number, stream.frame_data(number), stream.frames[number].capacity, target, area))
        encoded = list(pool.map(encode_frame, jobs, chunksize=2))
        new_raw, changed = stream.rewrite({number: data for number, data, _, _ in encoded})
        check = Stream(new_raw, lba)
        rechecked = dict(pool.map(_decode_job, [(n, check.frame_data(n)) for n, *_ in encoded], chunksize=4))
    report_frames = []
    pill_values, kept_values = [], []
    kept_frames = {}
    targets = {job[0]: job[3] for job in jobs}
    for (number, data, after, facts), job in zip(encoded, jobs):
        if not np.array_equal(rechecked[number][2], after):
            raise MovieError('frame %d decodes differently from the new sectors' % number)
        cues = {cue.id: round(cue.alpha[number - 1], 3) for cue in captions.cues if number - 1 in cue.alpha}
        facts['opacity'] = cues
        pill_values.append(facts['psnr_pill'])
        if facts['psnr_kept_in_changed_macroblocks'] is not None:
            kept_values.append(facts['psnr_kept_in_changed_macroblocks'])
        for key in ('psnr_pill', 'psnr_kept_in_changed_macroblocks'):
            facts[key] = _rounded(facts[key])
        report_frames.append(facts)
        if keep_frames:
            kept_frames[number] = (frames[number - 1], targets[number], after)
    audio = [i for i, kind in enumerate(stream.kinds) if kind != 'video']
    if any(new_raw[i * cs.SIZE:(i + 1) * cs.SIZE] != raw[i * cs.SIZE:(i + 1) * cs.SIZE] for i in audio):
        raise MovieError('a non-video sector changed')
    report = {
        'path': PATH, 'lba': lba, 'sectors': sectors, 'frames': len(numbers),
        'frames_changed': len(report_frames), 'sectors_rewritten': len(changed),
        'sectors_unchanged': sectors - len(changed), 'audio_and_other_sectors': len(audio),
        'macroblocks_changed': sum(f['macroblocks_changed'] for f in report_frames),
        'coarser_frames': sum(1 for f in report_frames if (f['coarse'], f['dead_zone']) != RATE_LADDER[0]),
        'psnr_pill_min': _rounded(min(pill_values)), 'psnr_pill_median': _rounded(float(np.median(pill_values))),
        'psnr_kept_in_changed_macroblocks_min': _rounded(min(kept_values)),
        'psnr_kept_in_changed_macroblocks_median': _rounded(float(np.median(kept_values))),
        'untouched_macroblocks': 'spliced bit for bit; decoded again from the new sectors and compared',
        'cues': [{'id': c.id, 'caption_window_decoded': list(c.window), 'str_frames': [i + 1 for i in sorted(c.alpha)],
                  'caption_texels': int(c.mask.sum()), 'caption_texels_not_under_opaque_english': c.uncovered,
                  'moved_str_frames': {str(i + 1): list(o) for i, o in sorted(c.offset.items()) if o != (0, 0) and i in c.alpha},
                  'fade_fit_by_str_frame': {str(i + 1): a for i, a in sorted(c.fit.items()) if not c.full(i)}}
                 for c in captions.cues],
        'frame_details': report_frames,
        'seconds': round(time.monotonic() - began, 1),
    }
    return new_raw, report, kept_frames


def place(disc, root=REPOSITORY):
    """Write the English movie over OPENXA.STR on a tools/patch/disc.Disc. Returns the report."""
    entry = disc.iso.find(PATH)
    try:
        raw, report, _ = english_stream(disc.track.path, entry.lba, entry.sectors, root)
    except (pills.PillError, mdec_splice.SpliceError) as exc:
        raise MovieError(str(exc)) from exc
    report['sectors_written_on_disc'] = disc.replace_stream(PATH, raw)
    return report


def summary(report):
    return ('movie: %d frames changed, %d macroblocks, %d sectors rewritten (%d unchanged); '
            'pill PSNR min %.1f / median %.1f dB, kept texels in changed macroblocks min %.1f / median %.1f dB; '
            '%d frames needed a coarser setting' % (
                report['frames_changed'], report['macroblocks_changed'], report['sectors_rewritten'],
                report['sectors_unchanged'], report['psnr_pill_min'], report['psnr_pill_median'],
                report['psnr_kept_in_changed_macroblocks_min'], report['psnr_kept_in_changed_macroblocks_median'],
                report['coarser_frames']))


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('--track', default='disc/orig/track1.bin', help='original data track (MODE2/2352)')
    parser.add_argument('--out', default='build/en/movie')
    parser.add_argument('--frames', action='store_true', help='also write original|English|decoded PNGs of each changed frame')
    args = parser.parse_args(argv)
    os.chdir(REPOSITORY)
    from tools.patch.disc import Iso, Track
    track = Track(args.track)
    entry = Iso(track).find(PATH)
    track.close()
    try:
        raw, report, frames = english_stream(args.track, entry.lba, entry.sectors, keep_frames=args.frames)
    except (MovieError, pills.PillError, mdec_splice.SpliceError) as exc:
        print('movie: %s' % exc, file=sys.stderr)
        return 1
    out = Path(args.out)
    out.mkdir(parents=True, exist_ok=True)
    (out / 'OPENXA.raw').write_bytes(raw)
    (out / 'report.json').write_text(json.dumps(report, indent=1) + '\n')
    if args.frames:
        from PIL import Image
        (out / 'frames').mkdir(exist_ok=True)
        for number, (original, target, after) in frames.items():
            row = np.concatenate([original, np.rint(target).astype(np.uint8), after], axis=1)
            Image.fromarray(row).save(out / 'frames' / ('%04d.png' % number))
    print(summary(report))
    print('report: %s/report.json' % args.out)
    return 0


if __name__ == '__main__':
    sys.exit(main())
