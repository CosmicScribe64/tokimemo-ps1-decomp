"""Native XA bit-field/sector arithmetic; synthetic inputs contain no game data."""
import unittest
from tools.port.opening_voice_content import native_request


class NativeRequestTests(unittest.TestCase):
    def test_channel_file_and_sector_padding(self):
        offsets = [0] * 16
        offsets[3] = 1000
        request = native_request(0x13, [0x07000002, 0x09000005], offsets)
        # (idx * 32) + channel (xa_params[5] = D_80125D5D) + speaker offset -/+ 16
        self.assertEqual((request['start_lba'], request['end_lba']), (1055, 1183))
        self.assertEqual((request['file_number'], request['channel']), (9, 7))
        self.assertEqual(request['source_lba'], 0)
        self.assertEqual(set(request), {'source_sha256', 'source_lba', 'start_lba',
                                      'end_lba', 'file_number', 'channel', 'native_evidence'})

    def test_channel_byte_shifts_both_bounds(self):
        # audio_command_xa stores xa_params[5] (0x80125D5D) from the start word's
        # top byte before scaling; the span stays (end - start) * 32 + 32.
        offsets = [0] * 16
        plain = native_request(13, [0x00000010, 0x0000001D], offsets)
        shifted = native_request(13, [0x18000010, 0x0F00001D], offsets)
        self.assertEqual(shifted['start_lba'] - plain['start_lba'], 24)
        self.assertEqual(shifted['end_lba'] - plain['end_lba'], 24)
        self.assertEqual(shifted['end_lba'] - shifted['start_lba'], 13 * 32 + 32)

    def test_silent_speaker_never_gets_a_request(self):
        for speaker in (15, 31):
            with self.assertRaises(ValueError):
                native_request(speaker, [0, 0], [0] * 16)
