"""Invented BS Huffman vectors and independently predictable MDEC preview tests."""
import struct
import tempfile
import unittest
from pathlib import Path
from tools.assets.image_mdec import (ImageFormatError, _coefficients, _idct,
                                    export_mdec, probe_mdec)
from tools.test_asset_images import png_pixels


def bs(blocks, *, quant=1, version=1, halfwords=None):
    bits = ''.join(blocks) + ('1111111111' if version == 3 else '0111111111')
    bits += '0' * (-len(bits) % 16)
    payload = b''.join(struct.pack('<H', int(bits[i:i + 16], 2)) for i in range(0, len(bits), 16))
    if halfwords is None:
        halfwords = len(blocks) * 2
    rounded = ((halfwords * 2 + 127) // 128) * 128
    return struct.pack('<4H', rounded // 4, 0x3800, quant, version) + payload


def flat(count=720):
    return bs(['000000000010'] * count)


class MdecTests(unittest.TestCase):
    def test_complete_frame_end_count_requires_source_geometry(self):
        source = flat()
        info = probe_mdec(source, 'invented.BS')
        self.assertEqual(info['status'], 'unsupported')
        self.assertEqual(info['metadata']['macroblock_count'], 120)
        context = {'width': 320, 'height': 88, 'evidence': ['synthetic caller dimensions']}
        self.assertEqual(probe_mdec(source, 'invented.BS', context)['status'], 'supported')
        wrong = dict(context, height=160)
        self.assertEqual(probe_mdec(source, 'invented.BS', wrong)['status'], 'invalid')
        self.assertEqual(probe_mdec(source, 'invented.BS', {'width': 320, 'height': 88})['status'], 'unsupported')

    def test_huffman_sign_escape_run_zero_quirk_and_zigzag(self):
        # DC=0; escape run0/level0 consumes coefficient1, short +1 ->coefficient2.
        block = '0000000000' + '000001' + '0' * 16 + '110' + '10'
        source = bs([block] + ['000000000010'] * 5, quant=8, halfwords=14)
        blocks, metadata = _coefficients(source)
        self.assertEqual(blocks[0][1], 0)
        self.assertEqual(blocks[0][8], 16)
        self.assertEqual(metadata['zero_level_escape_codes'], 1)
        negative = '0000000000' + '111' + '10'
        blocks, _ = _coefficients(bs([negative] + ['000000000010'] * 5, quant=8, halfwords=13))
        self.assertEqual(blocks[0][1], -16)

    def test_dc_idct_constant_and_gray_opaque_preview(self):
        self.assertEqual(set(_idct([80] + [0] * 63)), {10})
        horizontal = _idct([0, 80] + [0] * 62)
        self.assertEqual(horizontal[:8], [14, 12, 8, 3, -3, -8, -12, -14])
        self.assertEqual(horizontal[:8], horizontal[8:16])
        with tempfile.TemporaryDirectory() as temporary:
            result = export_mdec(flat(), temporary, 'invented', source_path='invented.BS',
                                 context={'width': 320, 'height': 88, 'evidence': ['synthetic caller']})
            pixels = png_pixels(Path(temporary) / result['files'][0])
            self.assertEqual(pixels[1:5], bytes([132, 132, 132, 255]))
            self.assertEqual(len(pixels), (320 * 4 + 1) * 88)
            self.assertFalse(result['metadata']['hardware_pixel_parity_verified'])
            self.assertEqual(result['metadata']['source_output_bit15'], 1)
            mask = png_pixels(Path(temporary) / result['images'][0]['stp_png'])
            self.assertEqual(mask[1:321], bytes([255]) * 320)
            self.assertEqual(len(mask), (320 + 1) * 88)
            self.assertEqual(result['metadata']['padded_height'], 96)
            padded_png = Path(temporary) / result['metadata']['padded_decode']['png']
            padded_pixels = png_pixels(padded_png)
            self.assertEqual(len(padded_pixels), (320 * 4 + 1) * 96)
            active_stride = 320 * 4 + 1
            for row in range(88):
                self.assertEqual(pixels[row * active_stride:(row + 1) * active_stride],
                                 padded_pixels[row * active_stride:(row + 1) * active_stride])
            self.assertEqual(result['metadata']['padded_decode']['active_crop'], [0, 0, 320, 88])
            self.assertEqual(result['metadata']['padded_decode']['padded_rows_not_uploaded_by_caller'], 8)
            padded_mask = png_pixels(Path(temporary) / result['metadata']['padded_decode']['stp_png'])
            self.assertEqual(len(padded_mask), (320 + 1) * 96)

    def test_rejects_truncated_ac_overflow_marker_alignment_and_unknown_version(self):
        source = flat(6)
        self.assertEqual(probe_mdec(source[:-2], 'invented.BS')['status'], 'invalid')
        self.assertEqual(probe_mdec(bs(['000000000010']), 'invented.BS')['status'], 'invalid')
        overflow = '0000000000' + '000001' + format(63 << 10 | 1, '016b') + '10'
        self.assertEqual(probe_mdec(bs([overflow] + ['000000000010'] * 5), 'invented.BS')['status'], 'invalid')
        self.assertEqual(probe_mdec(bs(['000000000010'] * 6, version=99), 'invented.BS')['status'], 'unsupported')
        with self.assertRaises(ImageFormatError):
            export_mdec(flat(), '/unused', '../bad', context={'width': 320, 'height': 88, 'evidence': ['synthetic caller']})


class MdecV3Tests(unittest.TestCase):
    def test_differential_signs_and_independent_chroma_shared_luma_history(self):
        # Cr+4, Cb-8, Y+12, Y+0, Y-4, Y+8, then all predictors unchanged.
        dc_codes = ['011', '1001', '0111', '100', '000', '0110']
        dc_codes += ['00', '00'] + ['100'] * 4
        blocks, metadata = _coefficients(bs([code + '10' for code in dc_codes], version=3))
        self.assertEqual([b[0] for b in blocks], [8, -16, 24, 24, 16, 32, 8, -16, 32, 32, 32, 32])
        self.assertEqual(metadata['macroblock_count'], 2)
        # New frames reset all three DC predictors to zero.
        reset, _ = _coefficients(bs([code + '10' for code in ['00', '00'] + ['100'] * 4], version=3))
        self.assertEqual([b[0] for b in reset], [0] * 6)

    def test_large_signed_category_and_ac_escape(self):
        dc_codes = ['11111110' + '01111111', '00', '111110' + '1111111'] + ['100'] * 3
        encoded = [code + '10' for code in dc_codes]
        # Luma block gets a signed -3 AC escape at zigzag position 1.
        encoded[2] = dc_codes[2] + '000001' + format((-3) & 1023, '016b') + '10'
        blocks, metadata = _coefficients(bs(encoded, version=3, quant=8, halfwords=13))
        self.assertEqual(blocks[0][0], -1024)
        self.assertEqual(blocks[2][0], 1016)
        self.assertEqual(blocks[2][1], -48)
        self.assertEqual(metadata['escape_codes'], 1)

    def test_truncated_dc_invalid_prefix_and_end_marker_alignment(self):
        source = bs([code + '10' for code in ['00', '00'] + ['100'] * 4], version=3)
        self.assertEqual(probe_mdec(source[:-2], 'invented.BS')['status'], 'invalid')
        # End marker after one Cr block is not a complete macroblock.
        self.assertEqual(probe_mdec(bs(['0010'], version=3), 'invented.BS')['status'], 'invalid')
        invalid_luma = ['0010', '0010', '11111110']
        self.assertEqual(probe_mdec(bs(invalid_luma, version=3), 'invented.BS')['status'], 'invalid')

    def test_v3_gray_preview_with_actual_context_and_predictor_overflow_refusal(self):
        codes = (['00', '00'] + ['100'] * 4) * 120
        source = bs([code + '10' for code in codes], version=3)
        with tempfile.TemporaryDirectory() as temporary:
            result = export_mdec(source, temporary, 'invented-v3',
                                 context={'width': 320, 'height': 88, 'evidence': ['synthetic caller']})
            self.assertEqual(result['metadata']['bs_version'], 3)
            self.assertEqual(png_pixels(Path(temporary) / result['files'][0])[1:5], bytes([132, 132, 132, 255]))
        overflow = ['11111110' + '01111111', '00'] + ['100'] * 4
        overflow += ['010', '00'] + ['100'] * 4  # Cr-4 after Cr-512 exceeds profile.
        self.assertEqual(probe_mdec(bs([c + '10' for c in overflow], version=3), 'invented.BS')['status'], 'unsupported')


if __name__ == '__main__':
    unittest.main()
