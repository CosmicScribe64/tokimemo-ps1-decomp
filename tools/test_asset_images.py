"""Synthetic TIM parser/PNG tests; no original game bytes (T-28002)."""
import struct
import tempfile
import unittest
import zlib
from pathlib import Path
from tools.assets.images import ImageFormatError, export_images, probe_images


def block(width, height, payload, x=0, y=0):
    return struct.pack('<I4H', len(payload) + 12, x, y, width, height) + payload


def tim(mode, payload, width, height=1, colors=None):
    data = struct.pack('<II', 0x10, mode | (8 if colors is not None else 0))
    if colors is not None:
        data += block(len(colors), 1, struct.pack('<' + 'H' * len(colors), *colors))
    return data + block(width, height, payload)


def png_pixels(path):
    data = path.read_bytes()
    assert data[:8] == b'\x89PNG\r\n\x1a\n'
    cursor = 8
    compressed = b''
    while cursor < len(data):
        size = struct.unpack_from('>I', data, cursor)[0]
        kind = data[cursor + 4:cursor + 8]
        payload = data[cursor + 8:cursor + 8 + size]
        assert zlib.crc32(kind + payload) & 0xffffffff == struct.unpack_from('>I', data, cursor + 8 + size)[0]
        if kind == b'IDAT':
            compressed += payload
        cursor += size + 12
    return zlib.decompress(compressed)


class ImageTests(unittest.TestCase):
    def export(self, data):
        directory = tempfile.TemporaryDirectory()
        self.addCleanup(directory.cleanup)
        output = Path(directory.name)
        return output, export_images(data, output, 'invented')

    def test_four_bit_low_nibble_and_stp_alpha(self):
        colors = [0, 0x001f, 0x8000, 0x83e0] + [0] * 12
        output, result = self.export(tim(0, b'\x10\x32', 1, colors=colors))
        self.assertEqual(result['metadata']['width'], 4)
        self.assertEqual(png_pixels(output / result['files'][0]), b'\0' + bytes([0, 0, 0, 0, 255, 0, 0, 255, 0, 0, 0, 255, 0, 255, 0, 255]))
        self.assertEqual(png_pixels(output / result['files'][1]), b'\0\0\0\xff\xff')
        self.assertEqual(result['images'][0]['palette_words'], colors)

    def test_eight_bit_multiple_palettes(self):
        colors = [0] * 512
        colors[255] = 0x7c00
        colors[511] = 0x001f
        output, result = self.export(tim(1, b'\xff\0', 1, colors=colors))
        self.assertEqual(len(result['images']), 2)
        self.assertEqual(result['images'][1]['clut_vram'], {'x': 256, 'y': 0})
        self.assertEqual(png_pixels(output / result['files'][0])[1:5], bytes([0, 0, 255, 255]))

    def test_direct_sixteen_bit(self):
        output, result = self.export(tim(2, struct.pack('<HH', 0x7fff, 0), 2))
        self.assertEqual(png_pixels(output / result['files'][0]), b'\0\xff\xff\xff\xff\0\0\0\0')

    def test_twenty_four_bit_rows_and_padding(self):
        output, result = self.export(tim(3, b'\x01\x02\x03\xff\x04\x05\x06\xff', 2, 2))
        self.assertEqual(result['metadata']['row_padding_bytes'], 1)
        self.assertEqual(png_pixels(output / result['files'][0]), b'\0\x01\x02\x03\xff\0\x04\x05\x06\xff')
        self.assertEqual(len(result['files']), 1)

    def test_fail_closed_headers_rectangles_and_payload(self):
        data = tim(2, b'\0\0', 1)
        for cut in range(len(data)):
            with self.assertRaises(ImageFormatError):
                export_images(data[:cut], '/unused', 'synthetic')
        for bad in (struct.pack('<II', 0x10, 4), tim(0, b'\0\0', 1),
                    struct.pack('<II', 0x10, 2) + block(1, 1, b'\0\0', 1024),
                    struct.pack('<II', 0x10, 2) + block(2, 1, b'\0\0'),
                    tim(0, b'\0\0', 1, colors=[0] * 15)):
            with self.assertRaises(ImageFormatError):
                export_images(bad, '/unused', 'synthetic')

    def test_no_magic_scanning_and_opaque_trailer(self):
        data = tim(2, b'\0\0', 1)
        self.assertEqual(probe_images(b'prefix' + data, 'invented')['status'], 'unsupported')
        _, result = self.export(data + b'opaque')
        self.assertEqual(result['metadata']['trailing_extent']['size'], 6)
        self.assertFalse(result['metadata']['round_trip_verified'])

    def test_safe_output_names(self):
        with self.assertRaises(ImageFormatError):
            export_images(tim(2, b'\0\0', 1), '/unused', '../escape')

    def test_palette_and_decoded_pixel_budgets(self):
        for clut_height, image_width, image_height in ((5, 1, 1), (4, 1024, 512)):
            data = struct.pack('<II', 0x10, 8)
            data += block(1024, clut_height, bytes(2048 * clut_height))
            data += block(image_width, image_height, bytes(2 * image_width * image_height))
            self.assertEqual(probe_images(data, 'invented')['status'], 'invalid')


if __name__ == '__main__':
    unittest.main()
