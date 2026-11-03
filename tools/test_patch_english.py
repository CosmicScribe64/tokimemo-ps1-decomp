"""Tests for the English disc build (T-39507): encoder, word wrap, windows, CD layout, in-place
writes and the build report.

Synthetic fixtures need no game data. The end-to-end test runs the whole build
(tools/patch/build_english.py) when TOKIMEMO_ENGLISH_DISC=1 is set and the original track is in
disc/orig/; it takes several minutes and needs the image of tools/Dockerfile.patch.

  tools/docker.sh python3 -m unittest tools.test_patch_english -v
  IMG=tokimemo-patch tools/docker.sh env TOKIMEMO_ENGLISH_DISC=1 python3 -m unittest tools.test_patch_english -v
"""
import json
import os
import struct
import tempfile
import unittest
from collections import defaultdict
from pathlib import Path

from tools.patch import build_english, cd_layout
from tools.patch.text.containers import Container, Record
from tools.patch.text.english import (EMPTY_OPTION, LABEL_SPEECH, LABEL_THOUGHT, LINE_END, NAME_CODES, OPTION_END,
                                      EnglishEncoder, end_inline_label)
from tools.patch.text.image import Image, Slot, has_cd_trailer, write_cd_trailer
from tools.patch.text.inplace import patch_built
from tools.patch.text.retarget import Pool, References, Retargeter
from tools.patch.text.layout import english_payloads
from tools.patch.text.segments import Control, Text, encode
from tools.patch.text.windows import NAME_PX, WINDOWS, Window, window_for
from tools.patch.text import reflow as source_reflow, speech
from tools.patch.text.wrap import FontMetrics, label_end, wrap

REPOSITORY = Path(__file__).resolve().parents[1]
ORIG_TRACK = REPOSITORY / "disc" / "orig" / "track1.bin"

# Synthetic font: every glyph 6 px, space 4 px, Shift-JIS 14 px.
METRICS = FontMetrics({**{c: 6 for c in range(0x21, 0x7F)}, 0x20: 4, **{c: 8 for c in range(0xA0, 0xAA)}}, 14, 16)


def window(width, *, lines=3, scroll=True, wrap_rows=True, max_rows=None, reflow=False):
    return Window("test", 0, width, lines, scroll, wrap_rows, None, "test", max_rows, reflow)


class EncoderTest(unittest.TestCase):
    def setUp(self):
        self.encoder = EnglishEncoder()

    def test_ascii_is_itself(self):
        self.assertEqual(self.encoder.encode_text("Hi, q n P!"), b"Hi, q n P!")

    def test_extra_glyphs(self):
        self.assertEqual(self.encoder.encode_text("“A” ‘b’ — … ♥ ♪ é ★"),
                         bytes([0xA0, 0x41, 0xA1, 0x20, 0xA2, 0x62, 0xA3, 0x20, 0xA4, 0x20, 0xA5, 0x20,
                                0xA6, 0x20, 0xA7, 0x20, 0xA8, 0x20, 0xA9]))

    def test_typographic_variants_are_normalised(self):
        self.assertEqual(self.encoder.encode_text("a–b ♡ ♫ ☆ ！ è 　x"),
                         b"a-b \xa6 \xa7 \xa9 ! e  x")
        self.assertIn("–", self.encoder.folded)

    def test_shift_jis_kept_and_unmapped_reported(self):
        self.assertEqual(self.encoder.encode_text("「\U0001f600"), "「".encode("shift_jis") + b"?")
        notes = self.encoder.take_record_notes()
        self.assertEqual(notes, {"unmapped": {"\U0001f600": 1}, "kept_shift_jis": {"「": 1}})
        self.assertEqual(self.encoder.take_record_notes(), {})

    def test_controls(self):
        enc = self.encoder.encode_control
        self.assertEqual(enc(Control("separator", "row", b" ")), bytes([LINE_END]))
        self.assertEqual(enc(Control("separator", "choice_option", b"q")), bytes([OPTION_END]))
        self.assertEqual(enc(Control("separator", "choice_option", b" ")), bytes([LINE_END]))
        self.assertEqual(enc(Control("separator", "reply_variant", b"q")), bytes([OPTION_END]))
        self.assertEqual(enc(Control("row_separator", "row_separator", b" ")), bytes([LINE_END]))
        for index, letter in enumerate("PpmFYGSOWAM"):
            self.assertEqual(enc(Control("substitution", "x", letter.encode())), bytes([0x10 + index]))
        self.assertEqual(enc(Control("conditional_substitution", "x", b"G")), bytes([NAME_CODES["G"]]))
        self.assertEqual(enc(Control("conditional_choice_zero", "x", b"nn")), bytes([EMPTY_OPTION] * 2))
        self.assertEqual(enc(Control("reply_skip_marker", "x", b"n\x41")), bytes([EMPTY_OPTION, 0x41]))
        self.assertEqual(enc(Control("terminal_byte_before_nul", "x", b" ")), b"")
        self.assertEqual(enc(Control("inline_label", "l", b">")), bytes([LABEL_SPEECH]))
        self.assertEqual(enc(Control("inline_label", "l", b"-")), bytes([LABEL_SPEECH]))
        self.assertEqual(enc(Control("inline_label", "l", b"<")), bytes([LABEL_THOUGHT]))
        for kept in (Control("format_directive", "f", b"%d"),
                     Control("source_storage_control", "line_feed", b"\n"), Control("opaque", "o", b"\x82\xa0")):
            self.assertEqual(enc(kept), kept.raw)

    def test_segments_end_to_end(self):
        segments = [Text("Hello "), Control("substitution", "girl_name", b"G"), Text("."),
                    Control("separator", "row", b" "), Text("Bye")]
        self.assertEqual(encode(segments, self.encoder), b"Hello \x15.\nBye")


class WrapTest(unittest.TestCase):
    def test_phone_names_use_runtime_reservations_without_changing_talk_names(self):
        window = WINDOWS['telephone_narration']
        phone = METRICS.for_window(window)
        self.assertEqual(phone.width(bytes([0x10])), 116)
        self.assertEqual(phone.width(bytes([0x15])), 116)
        self.assertEqual(METRICS.width(bytes([0x10])), 88)
        for name in (0x10, 0x15):
            payload = b"I'm " + bytes([name]) + b". Nice to meet you too."
            result = wrap(payload, window, METRICS)
            self.assertEqual(result.payload.replace(b'\n', b' '), payload)
            self.assertTrue(all(phone.width(row) <= window.width
                                for row in result.payload.split(b'\n')))
        deliberate = b'Hello.\n(A thought.)'
        self.assertEqual(wrap(deliberate, window, METRICS, joined=True).payload, deliberate)

    def test_fitting_rows_and_explicit_breaks_stay(self):
        result = wrap(b"ab cd\nef", window(100), METRICS)
        self.assertEqual(result.payload, b"ab cd\nef")
        self.assertEqual((result.rows, result.breaks_added, result.overflow_rows), (2, 0, []))

    def test_long_row_breaks_at_the_last_fitting_space(self):
        # "aaaa bbbb cccc": 24 + 4 + 24 + 4 + 24 px; 60 px holds two words
        result = wrap(b"aaaa bbbb cccc", window(60), METRICS)
        self.assertEqual(result.payload, b"aaaa bbbb\ncccc")
        self.assertEqual((result.rows, result.breaks_added), (2, 1))

    def test_word_wider_than_the_window_is_reported(self):
        result = wrap(b"aaaaaaaaaaaa bb", window(30), METRICS)
        self.assertEqual(result.payload, b"aaaaaaaaaaaa\nbb")
        self.assertEqual(result.overflow_rows, [72])

    def test_choice_windows_do_not_wrap(self):
        result = wrap(b"aaaa bbbb\x0bcc\x0bdd\x0bee", window(30, scroll=False, wrap_rows=False), METRICS)
        self.assertEqual(result.payload, b"aaaa bbbb\x0bcc\x0bdd\x0bee")
        self.assertEqual(result.overflow_rows, [52])
        self.assertTrue(result.too_many_lines)

    def test_lines_count_per_option_block_in_message_windows(self):
        result = wrap(b"a\nb\nc\nd\x0be", window(50, scroll=False), METRICS)
        self.assertEqual(result.rows, 4)
        self.assertTrue(result.too_many_lines)
        self.assertFalse(wrap(b"a\nb\nc\nd", window(50), METRICS).too_many_lines)
        self.assertTrue(wrap(b"a\nb\nc\nd", window(50, max_rows=3), METRICS).too_many_lines)

    def test_widths_of_names_shift_jis_and_labels(self):
        self.assertEqual(METRICS.width(bytes([NAME_CODES["p"]])), NAME_PX)
        self.assertEqual(METRICS.width(bytes([NAME_CODES["G"]])), METRICS.width(b"Tatebayashi-san"))
        self.assertEqual(METRICS.width(bytes([NAME_CODES["Y"]])), 6 * 6)
        self.assertEqual(METRICS.width("あ".encode("shift_jis") + b"a"), 14 + 6)
        self.assertEqual(METRICS.width(b"\x0a\x0b\x0c"), 0)
        self.assertEqual(label_end(b"\x1cTeacher\x1fHi\nthere"), len(b"\x1cTeacher\x1f"))
        self.assertEqual(label_end(b"\x1cHi"), 1)
        self.assertEqual(label_end(b"Hi\x1fthere"), 0)
        # the label does not count against the row
        self.assertEqual(wrap(b"\x1cTeacher\x1faaaa", window(30), METRICS).payload, b"\x1cTeacher\x1faaaa")

    def test_inline_label_end(self):
        self.assertEqual(end_inline_label(b"\x1cTeacher: Ah, you\nthere"), b"\x1cTeacher\x1fAh, you\nthere")
        self.assertEqual(end_inline_label(b"\x1dMe:hmm"), b"\x1dMe\x1fhmm")
        self.assertEqual(end_inline_label(b"\x1cno colon"), b"\x1c\x1fno colon")
        self.assertEqual(end_inline_label(b"plain: text"), b"plain: text")

    def test_reflow_joins_rows_except_deliberate_breaks(self):
        flow = window(1000, reflow=True)
        self.assertEqual(wrap(b"Oh, right!\nIt's \x15's\nbirthday today!", flow, METRICS).payload,
                         b"Oh, right! It's \x15's birthday today!")
        # a parenthesis or a quotation opens a new row; a closing one ends a row
        self.assertEqual(wrap(b"I'm \x13.\nNice to meet you.\n(Odd girl.)\nWell.", flow, METRICS).payload,
                         b"I'm \x13. Nice to meet you.\n(Odd girl.)\nWell.")
        # a pause after a finished sentence keeps its row, an empty row stays
        self.assertEqual(wrap(b"No.\n...Fine.\n\nBye", flow, METRICS).payload, b"No.\n...Fine.\n\nBye")
        # every reply variant is its own page; then the joined text wraps to the width
        self.assertEqual(wrap(b"aa\nbb\x0bcc\ndd", flow, METRICS).payload, b"aa bb\x0bcc dd")
        self.assertEqual(wrap(b"aaaa\nbbbb\ncccc", window(60, reflow=True), METRICS).payload, b"aaaa bbbb\ncccc")
        # windows without reflow keep the rows
        self.assertEqual(wrap(b"aa\nbb", window(1000), METRICS).payload, b"aa\nbb")


class SourceReflowTest(unittest.TestCase):
    """tools/patch/text/reflow.py: breaks judged on the Japanese row before them."""

    def segments(self, *rows):
        out = []
        for i, row in enumerate(rows):
            if i:
                out.append(Control("separator", "row", b" "))
            out.append(Text(row))
        return out

    def test_judge(self):
        rows = ["あ" * 13, "いう。", "えお", "「か」"]
        kinds = [b.kind for b in source_reflow.judge(rows, 14)]
        self.assertEqual(kinds, ["forced", "intentional", "intentional"])
        self.assertEqual(source_reflow.judge(["あい", "う"], 14)[0].kind, "short")

    def test_full_rows_reflow_and_short_sentences_keep_their_break(self):
        segments = self.segments("あ" * 13, "いう。", "えお")
        payload, notes = source_reflow.reflow(b"What'll I do\nthis\nweek? Fine", 0, segments, "message_speech", "X")
        self.assertEqual(payload, b"What'll I do this week?\nFine")
        self.assertEqual((notes.breaks_seen, notes.forced, notes.kept_rule), (2, 1, 1))
        # different break counts: the kept break goes to the same sentence end in the English
        payload, notes = source_reflow.reflow(b"One. Two three", 0, self.segments("あい。", "う"), "message_speech", "X")
        self.assertEqual(payload, b"One.\nTwo three")
        # an unmappable intentional break is dropped and logged
        payload, notes = source_reflow.reflow(b"a b\nc", 0, self.segments("あい。", "う", "え。"),
                                              "message_speech", "X")
        self.assertEqual(notes.ambiguous, 1)

    def test_equal_row_counts_require_an_english_boundary(self):
        segments = self.segments("あい。", "う")
        for text in (b"Found a quiet\nplace.", b"What's so\nfunny?", b"Good work,\nDr. Reed.",
                     b"Hello, Mr.\nBrown."):
            payload, notes = source_reflow.reflow(text, 0, segments, "message_speech", "X")
            self.assertEqual(payload, text.replace(b"\n", b" "))
            self.assertEqual((notes.kept_rule, notes.ambiguous), (0, 1))
        for text in (b"Really?\nThanks.", b"Wait...\nOkay.", b'He said "yes."\nFine.'):
            payload, notes = source_reflow.reflow(text, 0, segments, "message_speech", "X")
            self.assertEqual(payload, text)
            self.assertEqual(notes.kept_rule, 1)
        payload, notes = source_reflow.reflow(b"Done.\n", 0, segments, "message_speech", "X")
        self.assertNotIn(b"\n", payload)
        self.assertEqual(notes.kept_rule, 0)

    def test_openers_and_reply_pages_keep_their_boundaries(self):
        segments = self.segments("あい", "（う）")
        payload, _ = source_reflow.reflow(b"All right\n(I hope.)", 0, segments, "message_speech", "X")
        self.assertEqual(payload, b"All right\n(I hope.)")
        segments = self.segments("あ。", "い") + [Control("separator", "reply_variant", b"\x0b")] \
            + self.segments("う。", "え")
        payload, _ = source_reflow.reflow(b"First\nreply.\x0bReally?\nYes.", 0, segments,
                                         "message_speech", "X")
        self.assertEqual(payload, b"First reply.\x0bReally?\nYes.")

    def test_balanced_rows_avoid_a_lone_word(self):
        # greedy would leave "dd" alone; balanced fills two rows evenly
        result = wrap(b"aaaa bbbb cccc dd", window(80, reflow=True), METRICS, joined=True)
        self.assertEqual(result.payload, b"aaaa bbbb\ncccc dd")


class SpeechTest(unittest.TestCase):
    """tools/patch/text/speech.py: matched quotes and the speaker's text column."""

    def test_closing_quotes(self):
        self.assertEqual(speech.close_speech(b"Hi there", 0x1, None), (b'Hi there"', 1))
        self.assertEqual(speech.close_speech(b'Hi there"', 0x1, None), (b'Hi there"', 0))
        # the speech continues on the next record: no quote yet; the continuation closes it
        self.assertEqual(speech.close_speech(b"Hi", 0xC, 0xD), (b"Hi", 0))
        self.assertEqual(speech.close_speech(b"there", 0xD, None), (b'there"', 1))
        # the branch-zero tail stays last; skipped reply variants get nothing
        self.assertEqual(speech.close_speech(b"Hi\x0c\x0c", 0x0, None), (b'Hi"\x0c\x0c', 1))
        self.assertEqual(speech.close_speech(b"a\x0b\x0cb\x0bc", 0x2, None), (b'a"\x0b\x0cb\x0bc"', 2))
        # thoughts and narration are left to the runtime
        self.assertEqual(speech.close_speech(b"Hmm", 0xE, None), (b"Hmm", 0))
        self.assertEqual(speech.close_speech(b"\x1dMe\x1fhmm", 0x7, None), (b"\x1dMe\x1fhmm", 0))
        self.assertEqual(speech.close_speech(b"\x1cMe\x1fhi", 0x7, None), (b'\x1cMe\x1fhi"', 1))

    def test_trailing_thought_keeps_its_own_line(self):
        self.assertEqual(speech.separate_aside(b"All right... (That was odd.)"),
                         b"All right...\n(That was odd.)")
        self.assertEqual(speech.separate_aside(b"All right...\n(That was odd.)"),
                         b"All right...\n(That was odd.)")
        for text in (b"Meet at the park (east gate)", b"(Just a thought.)", b"A (small) detail."):
            self.assertEqual(speech.separate_aside(text), text)

    def test_unclose_removes_only_the_added_quotes(self):
        self.assertEqual(speech.unclose(b'Complete."', b'Complete. ', 0x1), b'Complete.')
        self.assertEqual(speech.unclose(b'Already closed."', b'Already closed." ', 0x1),
                         b'Already closed."')
        before = b'a b\x0bsaid "x"\x0b\x0cz'
        closed, n = speech.close_speech(before, 0x2, None)
        self.assertEqual(n, 1)
        wrapped = closed.replace(b"a b", b"a\nb")
        self.assertEqual(speech.unclose(wrapped, before, 0x2), b'a\nb\x0bsaid "x"\x0b\x0cz')

    def test_fixed_girl_width_keeps_the_club_question_together(self):
        base = WINDOWS["message_speech"]
        metrics = FontMetrics({**{i: 5 for i in range(32, 127)}, ord("T"): 20}, 14, 16)
        question = b"What's the Literature Club showing?\""
        widest = speech.speech_window(base, 0x1, question, metrics)
        self.assertEqual(wrap(question, widest, metrics, joined=True).rows, 2)
        fixed = speech.speech_window(base, 0x1, question, metrics, girl_label=b"Nijino")
        self.assertEqual(wrap(question, fixed, metrics, joined=True).rows, 1)
        for command in (0x0, 0xD, 0xA):
            self.assertEqual(speech.speech_window(base, command, question, metrics),
                             speech.speech_window(base, command, question, metrics,
                                                  girl_label=b"Nijino"))

    def test_text_column_follows_the_widest_speaker(self):
        base = WINDOWS["message_speech"]
        girl = speech.speech_window(base, 0x1, b"x", METRICS)
        self.assertEqual(girl.left, -0x80 + METRICS.width(b"Tatebayashi") + 8 + METRICS.width(b'"'))
        labelled = speech.speech_window(base, 0x7, b"\x1cMe\x1fhi", METRICS)
        self.assertEqual(labelled.left, base.left)  # a short label leaves the usual column
        self.assertIs(speech.speech_window(WINDOWS["choice"], 0x3, b"x", METRICS), WINDOWS["choice"])


class WindowTest(unittest.TestCase):
    def test_ending_uses_subtitle_geometry_without_speaker_label_reservation(self):
        for command, left in ((0, -0x58), (6, -0x58), (1, -0x82), (0xC, -0x82),
                              (0xD, -0x82), (0xF, -0x82)):
            record = self.record("script_record", [Text("Subtitle")], command)
            record.id = "talk:CDROM/SERIFU/ENDING.BIN:table-9:g0000:l0001"
            record.unit["context"]["profile_id"] = "serifu-ending"
            window, _ = window_for("talk_script", record)
            self.assertEqual(window.left, left)
            self.assertEqual(window.lines, 2)
            self.assertTrue(window.scroll)
            self.assertEqual(speech.speech_window(window, command, b"Subtitle", METRICS), window)

    def test_ending_boxed_tables_keep_regular_message_windows(self):
        for table in (0, 3, 13, 15):
            for command in (0, 1, 6):
                record = self.record("script_record", [Text("Dialogue")], command)
                record.id = f"talk:CDROM/SERIFU/ENDING.BIN:table-{table}:g0000:l0001"
                record.unit["context"]["profile_id"] = "serifu-ending"
                window, _ = window_for("talk_script", record)
                self.assertEqual(window.name, "message_speech")
                self.assertEqual(window.lines, 3)

    def test_ending_quotes_close_only_complete_speeches(self):
        for command in (0, 1, 0xF):
            self.assertEqual(speech.close_ending_speech(b"First\nlast.", command), b'First\nlast."')
            self.assertEqual(speech.close_ending_speech(b'Already closed."', command), b'Already closed."')
        for command in (6, 0xC, 0xD):
            self.assertEqual(speech.close_ending_speech(b"Continued", command), b"Continued")

    def test_ending_closes_speech_before_a_trailing_thought(self):
        self.assertEqual(speech.close_ending_speech(b"Spoken.\n(A private thought.)", 0),
                         b'Spoken."\n(A private thought.)')
        self.assertEqual(speech.close_ending_speech(b'Spoken."\n(A private thought.)', 0),
                         b'Spoken."\n(A private thought.)')

    def test_ending_balances_short_final_rows_without_changing_words(self):
        text = b"One two three four five six seven eight nine ten eleven twelve."
        result = wrap(text, WINDOWS["ending_heroine"], METRICS, joined=True)
        rows = result.payload.split(b"\n")
        self.assertEqual(len(rows), 2)
        self.assertEqual(b" ".join(rows), text)
        widths = [METRICS.width(row) for row in rows]
        self.assertLessEqual(max(widths), WINDOWS["ending_heroine"].width)
        self.assertLess(max(widths) - min(widths), 50)

    def test_ending_summary_uses_its_actual_text_column(self):
        record = self.record("script_record", [Text("Summary")], 7)
        record.unit["context"]["profile_id"] = "serifu-epilog"
        window, _ = window_for("talk_script", record)
        self.assertEqual(window.name, "ending_summary")
        self.assertEqual(window.left, -0x48)
        self.assertEqual(window.width, 196)
        self.assertTrue(window.scroll)

    def record(self, kind, segments, command=None):
        unit = {"context": {"command": {"name": "c", "value": command}}} if command is not None else {}
        return Record("id", kind, "P", 0, b"", segments, unit=unit)

    def test_talk_commands(self):
        self.assertEqual(window_for("talk_script", self.record("script_record", [], 1))[0].name, "message_speech")
        self.assertEqual(window_for("talk_script", self.record("script_record", [], 9))[0].name, "message_narration")
        self.assertEqual(window_for("talk_script", self.record("script_record", [], 3))[0].name, "choice")
        labelled = [Control("inline_label", "inline_label", b">"), Text("A: b")]
        self.assertEqual(window_for("talk_script", self.record("script_record", labelled, 7))[0].name,
                         "message_speech")
        self.assertEqual(window_for("talk_script", self.record("script_record", [Text("x")], 7))[0].name,
                         "message_narration")

    def test_renderer_and_unknown_contexts(self):
        rows = [Text("a"), Control("row_separator", "row_separator", b" "), Text("b")]
        self.assertEqual(window_for("overlay", self.record("source_literal", rows))[0].name, "telephone_narration")
        self.assertIsNone(window_for("overlay", self.record("source_literal", [Text("a")]))[0])
        self.assertEqual(window_for("talk_script_supplemental", self.record("supplemental", []))[0].name,
                         "message_speech")

    def test_geometry(self):
        self.assertEqual(WINDOWS["message_speech"].width, 0x7C + 0x48)
        self.assertEqual(WINDOWS["message_narration"].width, 0x7C + 0x80)
        self.assertFalse(WINDOWS["choice"].wrap)


def in_place_container(data):
    slots = [Slot("a", 0x10, data[0x10:0x13], "in_place", capacity=8),
             Slot("b", 0x20, data[0x20:0x22], "in_place", capacity=4),
             Slot("c", 0x30, data[0x30:0x32], "in_place", capacity=4)]
    container = Container("CDROM/EXEDIR/T.EXN", "overlay", Image("T", data, 0x80132000, slots, [], growable=False))
    for slot, kind in zip(slots, ("dictionary_slot", "source_extent", "source_literal")):
        container.records[slot.id] = Record(slot.id, kind, "T", slot.offset, slot.payload, [])
    return container


class InPlaceTest(unittest.TestCase):
    def test_overflow_respects_final_slot_padding(self):
        """An earlier overflow must not make the last padded slot look rebuilt."""
        base = 0x80132000
        data = bytearray(256)
        struct.pack_into('<I', data, 0, base + 0x10)
        data[0x10:0x14] = b'abc\0'
        data[0x30:0x33] = b'xy\0'
        slots = [Slot('early', 0x10, b'abc', 'in_place', capacity=4),
                 Slot('last', 0x30, b'xy', 'in_place', capacity=32)]
        container = Container('T', 'overlay', Image('T', bytes(data), base, slots, [], growable=False))
        for slot in slots:
            container.records[slot.id] = Record(slot.id, 'source_extent', 'T', slot.offset, slot.payload, [])
        for translate_last in (False, True):
            with self.subTest(translate_last=translate_last):
                refs = References.__new__(References)
                refs.words = defaultdict(list, {base + 0x10: [base]})
                refs.pairs = defaultdict(list)
                refs.hi_targets = defaultdict(set)
                mover = Retargeter(refs, Pool(0x40, 0xFC, base))
                payloads = {'early': b'LONG ENGLISH'}
                if translate_last:
                    payloads['last'] = b'FINAL LABEL'
                result = patch_built(container, payloads, bytes(data), retarget=mover)
                self.assertEqual(result.moved_in_build, [])
                self.assertEqual(result.overflows, [])
                self.assertEqual(struct.unpack_from('<I', result.data)[0], base + 0x50)
                self.assertEqual(result.data[0x50:0x5D], b'LONG ENGLISH\0')
                expected = b'FINAL LABEL' if translate_last else b'xy'
                self.assertEqual(result.data[0x30:0x50], expected.ljust(32, b'\0'))

    def test_writes_moves_and_overflows(self):
        data = bytearray(2048)
        data[0x10:0x13], data[0x20:0x22], data[0x30:0x32] = b"abc", b"de", b"fg"
        write_cd_trailer(data)
        container = in_place_container(bytes(data))
        built = bytearray(data)
        built[0x20] = 0x55                                    # slot b moved in the English link
        write_cd_trailer(built)
        result = patch_built(container, {"a": b"ABCDEF", "b": b"XY", "c": b"ZZ"}, bytes(built))
        self.assertEqual(result.written, ["a"])
        self.assertEqual(result.moved_in_build, ["b"])
        self.assertEqual(result.data[0x10:0x18], b"ABCDEF\0\0")
        self.assertEqual(result.data[0x30:0x32], b"fg")       # C literals are compiled, not patched
        self.assertTrue(has_cd_trailer(result.data))
        over = patch_built(container, {"a": b"ABCDEFGHIJ"}, bytes(data))
        self.assertEqual([o["id"] for o in over.overflows], ["a"])


class CdLayoutTest(unittest.TestCase):
    def test_scan_read_sites(self):
        with tempfile.TemporaryDirectory() as tmp:
            src = Path(tmp) / "src" / "ovl"
            src.mkdir(parents=True)
            (src / "a.c").write_text("void f(void) {\n    cd_read_sectors(7U, (void *)0x801E8000, 0xB037);\n"
                                     "    cd_read_sectors_nowait(0x29, 0x801E0000, 1234);\n"
                                     "    cd_read_sectors(n, 0x801E0000, lba);\n"
                                     "    if (cd_file_checksum_ok(0x2D, 0x80180000)) {}\n}\n")
            sites = cd_layout.scan_read_sites(Path(tmp) / "src")
            self.assertEqual([(s["sectors"], s["address"], s["lba"], s["checked"]) for s in sites],
                             [(7, 0x801E8000, 0xB037, True), (0x29, 0x801E0000, 1234, False)])
            self.assertEqual(sites[0]["source"], "src/ovl/a.c:2")
            checks = cd_layout.scan_checksum_sites(Path(tmp) / "src")
            self.assertEqual([(c["blocks"], c["address"]) for c in checks], [(0x2D, 0x80180000)])

    def plan(self, new_size, sites, checks=(), size=10 * 2048):
        limits = cd_layout.buffer_limits(sites)
        files = {100: ("F", 10), 110: ("NEXT", 4)}
        return cd_layout.plan_file("F", 100, size, new_size, sites, limits, list(checks), files)

    def test_plans(self):
        site = {"source": "s", "sectors": 10, "address": 0x801E8000, "lba": 100}
        bigger = {"source": "t", "sectors": 14, "address": 0x801E8000, "lba": 200}
        same = self.plan(10 * 2048, [site])
        self.assertTrue(same.accept)
        self.assertEqual(same.new_sectors, 10)
        grown = self.plan(12 * 2048, [site, bigger])
        self.assertTrue(grown.accept)
        self.assertEqual((grown.new_sectors, grown.problems), (12, []))
        too_big = self.plan(15 * 2048, [site, bigger])
        self.assertFalse(too_big.accept)
        self.assertIn("buffer_limit", too_big.reason)
        date = self.plan(25 * 2048, [dict(site, address=0x80198000)], size=20 * 2048)
        self.assertFalse(date.accept)
        self.assertIn("limit 24", date.reason)
        inside = self.plan(12 * 2048, [site, dict(site, source="u", lba=104, sectors=2)])
        self.assertIn("from inside the file", inside.problems[0])
        spanning = self.plan(12 * 2048, [dict(site, sectors=14), bigger])
        self.assertIn("also reads NEXT", spanning.problems[0])
        checked = self.plan(12 * 2048, [site, bigger], [{"source": "k", "blocks": 10, "address": 0x801E8000}])
        self.assertIn("checks 10 blocks", checked.problems[0])
        unknown = self.plan(12 * 2048, [])
        self.assertTrue(unknown.accept)
        self.assertIn("no literal read site", unknown.warnings[0])

    def test_remap_table(self):
        placed = [{"path": "B", "lba": 200, "sectors": 3, "new_lba": 300000, "new_sectors": 4},
                  {"path": "A", "lba": 100, "sectors": 2, "new_lba": 100, "new_sectors": 2}]
        rows = cd_layout.remap_entries(placed)
        self.assertEqual([r["path"] for r in rows], ["B"])
        asm = cd_layout.remap_asm(rows)
        self.assertIn("english_cd_remap_count:\n    .word 1", asm)
        self.assertIn(".word 200, 300000  # B", asm)
        self.assertIn(".byte 3, 4, 0, 0", asm)
        self.assertIn(".size english_cd_remap, 12", asm)
        self.assertIn(".word 0", cd_layout.remap_asm([]))
        with self.assertRaises(ValueError):
            cd_layout.remap_asm([dict(rows[0], new_sectors=256)])

    def test_cli_without_layout_writes_an_empty_table(self):
        with tempfile.TemporaryDirectory() as tmp:
            out = Path(tmp) / "gen" / "remap.s"
            self.assertEqual(cd_layout.main(["x", "remap-asm", str(Path(tmp) / "none.json"), str(out)]), 0)
            self.assertIn("    .word 0", out.read_text())


class ProfileLabelsTest(unittest.TestCase):
    def test_labels_fit_without_touching_other_fields(self):
        from tools.patch.text import profile
        from tools.patch.text.image import WriteResult, has_cd_trailer, write_cd_trailer
        original = bytearray(0x13000)
        original[0x1229C:0x12F00] = b"\x55" * (0x12F00 - 0x1229C)
        write_cd_trailer(original)
        payloads = {}
        for table, count in (("club_labels", 13), ("zodiac_labels", 12)):
            for i in range(count):
                payloads[f"dictionary:{profile.PATH}:{table}:{i:02d}"] = f"Example {i}".encode()
        result = WriteResult(bytes(original), {}, [{"id": next(iter(payloads))}])
        profile.write_labels(bytes(original), result, payloads)
        self.assertEqual(len(result.data), len(original))
        self.assertEqual(result.data[0x1229C:0x12F00], original[0x1229C:0x12F00])
        self.assertTrue(has_cd_trailer(result.data))
        self.assertFalse(result.overflows)
        self.assertEqual(result.data[0x12F00:0x12F0A], b"Example 0\0")
        self.assertEqual(result.data[0x12200:0x1220A], b"Example 0\0")
        payloads[next(iter(payloads))] = b"x" * 18
        with self.assertRaises(ValueError):
            profile.write_labels(bytes(original), result, payloads)


class ReportTest(unittest.TestCase):
    def test_stage_control_survives_physical_owner_translation(self):
        for prefix in (b' ', b'g', b'E'):
            with self.subTest(prefix=prefix):
                path = 'CDROM/EXEDIR/BUNKA_SD.EXN'
                bank = Container(path, 'executable', None)
                bank.records['physical'] = Record('physical', 'source_extent', path, 0,
                                                   prefix + b'Original', [Text('Original')])
                context = {'source_inventory_record_ids': ['physical'], 'stage_context': {
                    'body_relative_offset': 1, 'protected_prefix_hex': prefix.hex()}}
                bank.records['view'] = Record('view', 'stage_text_record', path, 1,
                    b'Original', [Text('Original')], role='alias', owner='physical',
                    unit={'context': context})
                result = english_payloads({path: bank}, {'physical': {
                    'target_text': 'good days', 'protected_tokens': []}}, METRICS)
                self.assertEqual(result.payloads[path]['physical'], prefix + b'good days')
                context['stage_context']['protected_prefix_hex'] = ''
                with self.assertRaisesRegex(ValueError, 'Missing stage command'):
                    english_payloads({path: bank}, {}, METRICS)

    def test_reviewed_trip_rows_preserve_wording_and_scope(self):
        sentence = "Wanna go out again the day after tomorrow?"
        for path, target, applied in (
                ("CDROM/SERIFU/SHUGAKU.BIN", sentence, True),
                ("CDROM/SERIFU/T.BIN", sentence, False),
                ("CDROM/SERIFU/SHUGAKU.BIN", sentence.replace("again", "together"), False)):
            with self.subTest(path=path, target=target):
                talk = Container(path, "talk_script", None)
                unit = {"context": {"command": {"name": "player", "value": 0}}}
                talk.records["t1"] = Record("t1", "script_record", path, 0, b"",
                                            [Text("x")], unit=unit)
                result = english_payloads({path: talk},
                    {"t1": {"target_text": target, "protected_tokens": []}}, METRICS)
                payload = result.payloads[path]["t1"]
                self.assertEqual(payload.replace(b"\n", b" "), target.encode())
                self.assertEqual("reviewed_rows" in result.notes["t1"], applied)
                self.assertEqual(result.notes["t1"]["status"], "placed")
                if applied:
                    self.assertIn(b"\nthe day after tomorrow?", payload)

    def test_payloads_statuses_and_report(self):
        talk = Container("CDROM/SERIFU/T.BIN", "talk_script", None)
        unit = {"context": {"command": {"name": "girl", "value": 1}}}
        talk.records["t1"] = Record("t1", "script_record", talk.path, 0, b"", [Text("x")], unit=unit)
        talk.records["t2"] = Record("t2", "script_record", talk.path, 8, b"", [Text("x")], unit=unit)
        talk.records["t3"] = Record("t3", "script_record", talk.path, 16, b"", [Text("x")], unit=unit)
        talk.records["t4"] = Record("t4", "script_record", talk.path, 24, b"", [Text("x")], role="alias")
        targets = {"t1": {"target_text": "Hello there", "protected_tokens": []},
                   "t2": {"target_text": "W" * 40, "protected_tokens": []},
                   "t4": {"target_text": "alias", "protected_tokens": []}}
        text = english_payloads({talk.path: talk}, targets, METRICS)
        # the closing quote is measured but drawn by the runtime (tools/patch/text/speech.py)
        self.assertEqual(text.payloads[talk.path], {"t1": b"Hello there", "t2": b"W" * 40})
        self.assertEqual(text.notes["t1"]["status"], "placed")
        self.assertEqual(text.notes["t2"]["status"], "unwrappable")
        self.assertEqual(text.notes["t3"], {"path": talk.path, "kind": "script_record", "container": "talk_script",
                                            "status": "kept_original", "reason": "no_translation_row"})
        self.assertNotIn("t4", text.notes)
        literals = {"literals_reverted": {}, "files": [], "literals_written": 0, "files_reverted": {}}
        data = {"overflows": {}, "kept_original": [{"path": talk.path, "reason": "buffer_limit: x", "records": 2}],
                "placed": [], "unchanged": 0}
        records = build_english.record_report(text, {talk.path: talk}, literals, data, {"in_place": {}})
        self.assertEqual(records["totals"], {"file_kept_original": 2, "kept_original": 1})
        self.assertEqual(records["by_container"][talk.path]["file_kept_original"], 2)
        self.assertEqual([i["id"] for i in records["records"]["file_kept_original"]], ["t1", "t2"])
        report = {"records": records, "data_files": dict(data, placed=[
            {"path": "X", "lba": 1, "new_lba": 9, "bytes": 10, "new_bytes": 30}]),
            "c_literals": literals, "characters": {"unmapped": {}, "kept_shift_jis": {}},
            "disc": {"track1_sha1": "ab" * 20, "sectors_added": 3, "xdelta_bytes": 99},
            "timings": {"total": 1.5}}
        lines = build_english.headline(report)
        self.assertIn("data files: 1 written (+20 bytes), 1 moved, 1 kept original, 0 unchanged", lines)
        self.assertTrue(lines[0].startswith("records: file_kept_original 2, kept_original 1"))


class LiteralSiteTest(unittest.TestCase):
    def test_moved_literal_is_found_by_its_bytes(self):
        from tools.patch.text import c_literals
        with tempfile.TemporaryDirectory() as tmp:
            src = Path(tmp) / "src" / "main"
            src.mkdir(parents=True)
            text = 'void f(void) {\n#ifdef X\n    g();\n#endif\n    h("ab"); h("cd"); h("ab");\n}\n'
            (src / "a.c").write_text(text)
            first, second = text.index('"ab"'), text.index('"cd"')

            def unit(uid, raw, offset):
                return {"id": uid, "kind": "source_literal", "raw_hex": raw.hex(),
                        "context": {"source_locator_id": "c:src/main/a.c:1:1", "source_character_offset": offset}}
            # the export saw the file without the three #ifdef lines (28 characters)
            units = [unit("u1", b"ab", first - 28), unit("u2", b"cd", second)]
            sites = {s.id: s.start for s in c_literals.locate_sites(tmp, units)}
            self.assertEqual(sites, {"u1": first, "u2": second})
            with self.assertRaises(c_literals.LiteralError):
                c_literals.locate_sites(tmp, [unit("u3", b"zz", 5)])


class GrowArrayTest(unittest.TestCase):
    def test_sized_array_grows_for_a_longer_literal(self):
        from tools.patch.text.c_literals import _grow_array
        self.assertEqual(_grow_array("    u8 name[27] = ", 30), "    u8 name[30] = ")
        self.assertEqual(_grow_array("    u8 name[0x20] = ", 30), "    u8 name[0x20] = ")
        self.assertEqual(_grow_array("    f(", 30), "    f(")


class EnglishLinkFreeTest(unittest.TestCase):
    def test_free_link_has_anchors_and_no_pins(self):
        """T-39505: the English script names every input section and pins nothing, so the section
        labels after a shrinking or growing object simply follow it."""
        import tools.english_link as english_link
        with tempfile.TemporaryDirectory() as tmp:
            ld, out, mapfile = Path(tmp) / "a.ld", Path(tmp) / "b.ld", Path(tmp) / "a.map"
            ld.write_text("SECTIONS\n{\n    X_START = .;\n    .X 0x80132000 : AT(0)\n    {\n"
                          "        build/a.o(.rodata);\n        X_RODATA_END = .;\n        X_DATA_START = .;\n"
                          "        build/b.o(.data);\n    }\n    X_VRAM_END = .;\n}\n")
            mapfile.write_text("")
            english_link.cmd_ld(["X", "build/en", str(ld), str(mapfile), str(out), "--redirect", "build/a.o"])
            lines = [line.strip() for line in out.read_text().splitlines()]
            self.assertIn("build/en/a.o(.rodata);", lines)
            self.assertIn("__en_X_0 = ALIGN(., 2);", lines)
            self.assertIn("__en_X_1 = ALIGN(., 2);", lines)
            self.assertFalse(any(line.startswith(". = 0x") for line in lines))


@unittest.skipUnless(os.environ.get("TOKIMEMO_ENGLISH_DISC") == "1" and ORIG_TRACK.is_file(),
                     "set TOKIMEMO_ENGLISH_DISC=1 with disc/orig/track1.bin to run the disc build")
class EndToEndTest(unittest.TestCase):
    def test_disc_build(self):
        from tools.patch import disc as disc_mod
        out = "build/en/disc-test"
        self.assertEqual(build_english.main(["--out", out, "--no-patch"]), 0)
        report = json.loads((REPOSITORY / out / "report.json").read_text())
        self.assertGreater(report["records"]["totals"].get("placed", 0), 30000)
        with open(REPOSITORY / out / "track1.bin", "rb"):
            pass
        track = disc_mod.Track(str(REPOSITORY / out / "track1.bin"))
        try:
            iso = disc_mod.Iso(track)
            for row in report["cd_remap"]:
                entry = iso.find(row["path"])
                self.assertEqual((entry.lba, entry.sectors), (row["new_lba"], row["new_sectors"]))
                self.assertTrue(has_cd_trailer(iso.read(row["path"]) + bytes(-entry.size % 2048))
                                or not (REPOSITORY / "disc" / "files" / row["path"]).read_bytes().endswith(b"\x12\x10"))
            exe = iso.read("SLPM_86.053")
            english = (REPOSITORY / "build" / "en" / "SLPM_86.053.bin").read_bytes()
            self.assertEqual(exe[:0x800], english[:0x800])
            self.assertEqual(struct.unpack_from("<I", exe, 0x10)[0], struct.unpack_from("<I", english, 0x10)[0])
        finally:
            track.close()


if __name__ == "__main__":
    unittest.main()
