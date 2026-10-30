"""Native XA bit-field/sector arithmetic; synthetic inputs contain no game data."""
import unittest
from tools.port.opening_voice_content import native_request


class NativeRequestTests(unittest.TestCase):
    def test_channel_file_and_sector_padding(self):
        offsets = [0] * 16
        offsets[3] = 1000
        request = native_request(0x13, [0x07000002, 0x09000005], offsets)
        self.assertEqual((request['start_lba'], request['end_lba']), (1048, 1176))
        self.assertEqual((request['file_number'], request['channel']), (9, 7))
        self.assertEqual(request['source_lba'], 0)
        self.assertEqual(set(request), {'source_sha256', 'source_lba', 'start_lba',
                                      'end_lba', 'file_number', 'channel', 'native_evidence'})

    def test_silent_speaker_never_gets_a_request(self):
        for speaker in (15, 31):
            with self.assertRaises(ValueError):
                native_request(speaker, [0, 0], [0] * 16)
