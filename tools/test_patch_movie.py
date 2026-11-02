"""Tests for the English opening movie (T-39509): STR sector rewrite, MDEC decode/encode,
caption measurement, frame selection and compositing, and the disc stream write.

Synthetic fixtures need no game data. The disc tests read OPENXA.STR from the original track in
disc/orig/ (or $TOKIMEMO_ORIG_DISC) and skip without it; the whole movie build runs only with
TOKIMEMO_MOVIE=1 (about 70 s).

  tools/docker.sh python3 -m unittest tools.test_patch_movie -v
  IMG=tokimemo-patch tools/docker.sh env TOKIMEMO_MOVIE=1 python3 -m unittest tools.test_patch_movie -v
"""
import os
import hashlib
import tempfile
import random
import struct
import unittest
from pathlib import Path

import numpy as np

from tools.assets import mdec_splice
from tools.patch import disc as disc_mod
from tools.patch import sector as cs
from tools.patch.movie import build, mdec, pills
from tools.patch.movie.stream import CHUNK, HEADER, Stream, StrError

REPOSITORY = Path(__file__).resolve().parents[1]
ORIG = Path(os.environ.get('TOKIMEMO_ORIG_DISC', REPOSITORY / 'disc' / 'orig'))
TRACK1 = ORIG / 'track1.bin'
HAVE_DISC = TRACK1.is_file()
VIDEO_SUB = bytes((1, 1, 0x42, 0x80))
AUDIO_SUB = bytes((1, 1, 0x64, 0x01))


def audio_sector(lba, rng):
    s = bytearray(cs.SIZE)
    s[0:12] = cs.SYNC
    s[12:16] = cs.lba_header(lba)
    s[16:24] = AUDIO_SUB * 2
    s[24:cs.FORM2_EDC] = bytes(rng.randrange(256) for _ in range(cs.FORM2_DATA))
    return bytes(s)  # EDC 0, as in the game's audio sectors


def video_sector(lba, number, index, chunks, data, width=32, height=16):
    payload = bytearray(cs.FORM1_DATA)
    HEADER.pack_into(payload, 0, 0x0160, 0x8001, index, chunks, number, len(data), width, height, data[:8], b'\0' * 4)
    part = data[index * CHUNK:(index + 1) * CHUNK]
    payload[0x20:0x20 + len(part)] = part
    return bytes(cs.make_form1(lba, VIDEO_SUB, bytes(payload)))


def synthetic_stream(lba=500, sizes=(3000, 100, 5000), chunks=3, seed=1):
    """Raw sectors: frames of `chunks` video sectors, an audio sector after every third video
    sector, an empty sector at the end. Returns (raw, {number: data})."""
    rng = random.Random(seed)
    sectors, frames, video = [], {}, 0
    for number, size in enumerate(sizes, 1):
        data = bytes(rng.randrange(256) for _ in range(size))
        frames[number] = data
        for index in range(chunks):
            sectors.append(video_sector(lba + len(sectors), number, index, chunks, data))
            video += 1
            if video % 3 == 0:
                sectors.append(audio_sector(lba + len(sectors), rng))
    sectors.append(bytes(cs.make_form1(lba + len(sectors), b'\0\0\0\0', b'')))
    return b''.join(sectors), frames


class StreamTest(unittest.TestCase):
    def test_identity_round_trip_is_byte_exact(self):
        raw, frames = synthetic_stream()
        stream = Stream(raw, 500)
        self.assertEqual(list(stream.frames), [1, 2, 3])
        self.assertEqual({n: stream.frame_data(n) for n in stream.frames}, frames)
        out, changed = stream.rewrite(frames)
        self.assertEqual(out, raw)
        self.assertEqual(changed, [])

    def test_rewrite_changes_only_that_frames_video_sectors(self):
        raw, frames = synthetic_stream()
        stream = Stream(raw, 500)
        new = bytes(range(256)) * 20  # 5120 bytes, more than the original 100
        out, changed = stream.rewrite({2: new})
        self.assertEqual(changed, stream.frames[2].sectors)
        for i in range(stream.count):
            a, b = raw[i * cs.SIZE:(i + 1) * cs.SIZE], out[i * cs.SIZE:(i + 1) * cs.SIZE]
            if i in changed:
                self.assertEqual(a[:cs.DATA], b[:cs.DATA])  # sync, address, subheader
                self.assertTrue(cs.form1_valid(b))
            else:
                self.assertEqual(a, b)
        again = Stream(out, 500)
        self.assertEqual(again.frame_data(2), new)
        self.assertEqual(again.frames[2].size, len(new))
        self.assertEqual(again.frame_data(1), frames[1])
        first = out[changed[0] * cs.SIZE:]
        self.assertEqual(first[cs.DATA + 0x14:cs.DATA + 0x1C], new[:8])  # BS header copy

    def test_frame_too_big_or_unaligned_is_refused(self):
        raw, _ = synthetic_stream()
        stream = Stream(raw, 500)
        with self.assertRaises(StrError):
            stream.rewrite({1: bytes(3 * CHUNK + 4)})
        with self.assertRaises(StrError):
            stream.rewrite({1: bytes(10)})
        with self.assertRaises(StrError):
            stream.rewrite({9: bytes(8)})

    def test_damaged_layout_is_refused(self):
        raw, _ = synthetic_stream()
        bad = bytearray(raw)
        bad[cs.SIZE * 1 + cs.DATA + 4] = 7  # chunk index of the second sector
        with self.assertRaises(StrError):
            Stream(bytes(bad), 500)
        last = bytearray(raw[2 * cs.SIZE:3 * cs.SIZE])  # chunk 2 of frame 1 holds bytes 4032..
        last[cs.DATA + 0x20 + 100] = 1  # frame byte 4132, after the 3000 bytes of data
        cs.finish_form1(last)
        with self.assertRaises(StrError):
            Stream(raw[:2 * cs.SIZE] + bytes(last) + raw[3 * cs.SIZE:], 500)
        with self.assertRaises(StrError):
            Stream(raw, 501)  # addresses differ


def random_levels(rng, count):
    levels = np.zeros((count, 6, 64), np.int64)
    levels[..., 0] = rng.integers(-150, 150, (count, 6))
    for k in range(1, 20):
        levels[..., k] = rng.integers(-6, 7, (count, 6)) * (rng.random((count, 6)) < 0.5)
    return levels


def bs_frame(levels, scale):
    """A BS v1 frame of the given macroblock levels."""
    bits, halfwords = [], 0
    for mb in levels:
        for block in mb:
            part, count = mdec_splice.encode_block([int(v) for v in block])
            bits.append(part)
            halfwords += count
    declared = (halfwords * 2 + 127) // 128 * 128 // 4
    data = struct.pack('<4H', declared, 0x3800, scale, 1) + mdec_splice._pack(''.join(bits) + mdec_splice.END_MARKER)
    return data + bytes(-len(data) % 4)


class MdecTest(unittest.TestCase):
    def test_decoder_equals_the_model(self):
        rng = np.random.default_rng(5)
        for scale in (1, 3, 7):
            levels = random_levels(rng, 8)
            mine = mdec.rgb555(mdec.macroblock_channels(mdec.block_outputs(levels, scale)))
            for k, mb in enumerate(levels):
                model = mdec_splice.macroblock_rgb5([mdec_splice.block_output(b.tolist(), scale) for b in mb])
                np.testing.assert_array_equal(mine[k], model)

    def test_cut_and_place_are_inverse(self):
        frame = np.arange(192 * 288 * 3).reshape(192, 288, 3)
        np.testing.assert_array_equal(mdec.place(mdec.cut(frame, 288, 192), 288, 192), frame)
        self.assertEqual(mdec_splice.macroblock_index(16, 32, 192), 12 + 2)
        self.assertTrue((mdec.cut(frame, 288, 192)[14][0, 0] == frame[32, 16]).all())

    def test_encode_reaches_a_representable_target(self):
        rng = np.random.default_rng(7)
        levels = random_levels(rng, 6)
        target = mdec.macroblock_channels(mdec.block_outputs(levels, 2))
        got = mdec.encode(np.zeros_like(levels), target, 2, passes=6)
        decoded = mdec.macroblock_channels(mdec.block_outputs(got, 2))
        self.assertLess(np.abs(decoded - target).mean(), 1.0)

    def test_encode_keeps_unchanged_luma_blocks(self):
        rng = np.random.default_rng(9)
        levels = random_levels(rng, 1)
        target = mdec.macroblock_channels(mdec.block_outputs(levels, 4)).astype(float)
        target[8:, 8:] = (250, 40, 246)  # only the lower right luma block (and chroma) change
        got = mdec.encode(levels, target, 4)
        np.testing.assert_array_equal(got[0, 2:5], levels[0, 2:5])

    def test_coarse_levels_are_multiples(self):
        rng = np.random.default_rng(3)
        levels = random_levels(rng, 2)
        target = np.full((2, 16, 16, 3), 128.0)
        target[:, 4:12, 4:12] = 255
        got = mdec.encode(levels, target, 3, coarse=3)
        self.assertTrue((got % 3 == 0).all())

    def test_frame_splice_keeps_untouched_macroblocks(self):
        rng = np.random.default_rng(11)
        levels = random_levels(rng, 216)
        data = bs_frame(levels, 3)
        _, _, original = build.decoded(data)
        area = np.zeros((192, 288), bool)
        area[20:40, 40:90] = True
        target = original.astype(float)
        target[area] = (250, 40, 246)
        number, out, after, facts = build.encode_frame((5, data, 26208, target, area))
        new_levels, _, decoded = build.decoded(out)
        np.testing.assert_array_equal(decoded, after)
        touched = mdec.cut(area[..., None].astype(np.uint8), 288, 192)[..., 0].reshape(216, -1).any(1)
        np.testing.assert_array_equal(new_levels[~touched], levels[~touched])
        self.assertGreater(facts['macroblocks_changed'], 0)
        self.assertLessEqual(facts['macroblocks_changed'], int(touched.sum()))
        self.assertGreater(facts['psnr_pill'], 30)
        self.assertEqual(len(out) % 4, 0)


PINK, WHITE = (240, 60, 230), (255, 255, 255)
SIZE = (64, 48)


def synthetic_cue():
    """A cue (window 10..30) with one pill and its Japanese caption and per-frame opacity."""
    japanese = np.zeros((48, 64, 3))
    caption = np.zeros((48, 64), bool)
    caption[10:22, 8:40] = True
    japanese[caption] = PINK
    japanese[13:19, 12:36:3] = WHITE  # lettering strokes
    art = np.zeros((16, 40, 4), np.uint8)
    art[..., :3] = (230, 120, 240)
    art[4:12, 6:34:4, :3] = 255
    art[..., 3] = 255
    art[0, 0, 3] = art[0, -1, 3] = 128  # soft corners
    cue = {'id': 'test', 'caption_window': [10, 30],
           'pills': [{'id': 'p', 'x': 4, 'y': 8, 'cover_rect': [6, 9, 42, 23], 'art': art}]}
    opacity = {8: 0.33, 9: 0.67, 31: 0.67, 32: 0.33}
    opacity.update({i: 1.0 for i in range(10, 31)})
    return cue, japanese, caption, opacity


def synthetic_frames(japanese, caption, opacity, seed=2):
    rng = np.random.default_rng(seed)
    ys, xs = np.mgrid[0:48, 0:64]
    frames, backgrounds = {}, {}
    for i in range(0, 45):
        b = np.stack([40 + xs + i, 90 + ys * 2 + 0 * xs, 60 + 0 * xs + i], -1).astype(float)
        b += rng.normal(0, 3, b.shape)
        b = np.clip(b, 0, 255)
        a = opacity.get(i, 0.0)
        o = b.copy()
        o[caption] = (1 - a) * b[caption] + a * japanese[caption]
        frames[i] = np.rint(o).astype(np.uint8)
        backgrounds[i] = b
    return frames, backgrounds


class PillsTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.cue, cls.japanese, cls.caption, cls.opacity = synthetic_cue()
        cls.frames, cls.backgrounds = synthetic_frames(cls.japanese, cls.caption, cls.opacity)
        cls.captions = pills.Captions({'cues': [cls.cue]}, cls.frames, SIZE)

    def test_frames_selected_are_the_visible_ones(self):
        cue = self.captions.cues[0]
        self.assertEqual(sorted(cue.alpha), sorted(self.opacity))
        self.assertEqual(cue.uncovered, 0)
        self.assertTrue(all(o == (0, 0) for o in cue.offset.values()))

    def test_fade_opacity_is_measured(self):
        cue = self.captions.cues[0]
        for i in (8, 9, 31, 32):
            self.assertAlmostEqual(cue.alpha[i], self.opacity[i], delta=0.06)

    def test_composite_replaces_the_caption_and_keeps_the_rest(self):
        cue = self.captions.cues[0]
        english, alpha = pills.english_layer(self.cue, SIZE)
        for i in (9, 20, 32):
            a = cue.alpha[i]
            out = cue.composite(i, self.frames[i])
            outside = alpha == 0
            np.testing.assert_array_equal(out[outside], self.frames[i][outside])
            want = (1 - a * alpha[..., None]) * self.backgrounds[i] + a * alpha[..., None] * english
            inside = cue.mask
            self.assertLess(np.abs(out[inside] - want[inside]).mean(), 4.0)
            if a == 1.0:
                np.testing.assert_allclose(out[alpha == 1], english[alpha == 1])

    def test_moved_caption_is_followed(self):
        cue, japanese, caption, opacity = synthetic_cue()
        moved = {i: pills.shifted(f, 1, 1) if 18 <= i <= 22 else f for i, f in
                 synthetic_frames(japanese, caption, opacity)[0].items()}
        measured = pills.Captions({'cues': [cue]}, moved, SIZE).cues[0]
        self.assertEqual(measured.offset[20], (1, 1))
        self.assertEqual(measured.offset[9], (0, 0))

    def test_needed_frames_cover_margins_and_references(self):
        needed = pills.Captions.needed({'cues': [self.cue]})
        self.assertEqual(needed[0], 0)  # 10 - MARGIN - REFERENCE_REACH, not below the first frame
        self.assertEqual(needed[-1], 30 + pills.MARGIN + pills.REFERENCE_REACH)


@unittest.skipUnless(HAVE_DISC, 'original track 1 is unavailable')
class DiscTest(unittest.TestCase):
    def setUp(self):
        self.disc = disc_mod.Disc(str(TRACK1))
        self.addCleanup(self.disc.close)
        self.entry = self.disc.iso.find(build.PATH)
        with open(TRACK1, 'rb') as f:
            f.seek(self.entry.lba * cs.SIZE)
            self.raw = f.read(self.entry.sectors * cs.SIZE)

    def test_openxa_identity_round_trip_is_byte_exact(self):
        stream = Stream(self.raw, self.entry.lba)
        self.assertEqual(len(stream.frames), 829)
        out, changed = stream.rewrite({n: stream.frame_data(n) for n in stream.frames})
        self.assertEqual(out, self.raw)
        self.assertEqual(changed, [])
        self.assertEqual(self.disc.replace_stream(build.PATH, out), 0)

    def test_stream_write_takes_video_and_refuses_audio(self):
        stream = Stream(self.raw, self.entry.lba)
        frame = stream.frames[400]
        out, changed = stream.rewrite({400: stream.frame_data(400)[:-4]})
        self.assertEqual(changed, frame.sectors)
        self.assertEqual(self.disc.replace_stream(build.PATH, out), len(frame.sectors))
        audio = stream.kinds.index('audio')
        bad = bytearray(self.raw)
        bad[audio * cs.SIZE + 100] ^= 1
        other = disc_mod.Disc(str(TRACK1))
        self.addCleanup(other.close)
        with self.assertRaises(disc_mod.DiscError):
            other.replace_stream(build.PATH, bytes(bad))
        with self.assertRaises(disc_mod.DiscError):
            other.replace_stream(build.PATH, self.raw[:-cs.SIZE])
        with self.assertRaises(disc_mod.DiscError):
            other.replace_stream('SLPM_86.053', b'')


class MovieReadbackTest(unittest.TestCase):
    def test_final_disc_extent_must_match_encoded_stream(self):
        raw = bytes(range(256)) * 9 + bytes(48)
        report = {'lba': 2, 'sectors': 1, 'raw_sha256': hashlib.sha256(raw).hexdigest()}
        with tempfile.TemporaryDirectory() as folder:
            track = Path(folder) / 'track.bin'
            track.write_bytes(bytes(2 * cs.SIZE) + raw)
            build.verify_written(track, report)
            self.assertTrue(report['readback_verified'])
            track.write_bytes(bytes(2 * cs.SIZE) + raw[:-1] + b'\x01')
            with self.assertRaises(build.MovieError):
                build.verify_written(track, report)


@unittest.skipUnless(HAVE_DISC and os.environ.get('TOKIMEMO_MOVIE') == '1', 'set TOKIMEMO_MOVIE=1 with the original track')
class MovieBuildTest(unittest.TestCase):
    def test_english_movie(self):
        track = disc_mod.Track(str(TRACK1))
        entry = disc_mod.Iso(track).find(build.PATH)
        track.close()
        raw, report, _ = build.english_stream(str(TRACK1), entry.lba, entry.sectors)
        self.assertEqual(len(raw), entry.sectors * cs.SIZE)
        self.assertEqual(len(report['cues']), 8)
        self.assertGreater(report['frames_changed'], 150)
        self.assertEqual(report['coarser_frames'], 0)
        for cue in report['cues']:
            self.assertLessEqual(cue['caption_texels_not_under_opaque_english'], 2)


if __name__ == '__main__':
    unittest.main()
