"""T-39302: helpers for capture_frame.sh.

  rects <tag.gpu.json> <real.json>   rectangles to fill with noise: the draw area the renderer ended
                                     with (compare_capture's "area") and the display area GP1 selected
                                     (control words 5 and 8 of the GPU block), as x0,y0,x1,y1 (inclusive)
  same <real.json> <noisy.json>      exit 1 unless both runs walked the same ordering table (nodes, words,
                                     primitive counts); the two RAM images differ in timers and the like
"""
import json
import sys


def display_rect(control):
    start = control.get('5', 0)
    mode = control.get('8', 0)
    widths = (256, 320, 512, 640)
    width = 368 if mode & 0x40 else widths[mode & 3]
    height = 480 if (mode & 4 and mode & 0x20) else 240
    x, y = start & 0x3FF, (start >> 10) & 0x1FF
    return [x, y, min(x + width - 1, 1023), min(y + height - 1, 511)]


def main(argv):
    if argv[1] == 'rects':
        gpu = json.load(open(argv[2]))
        real = json.load(open(argv[3]))
        found = []
        for rect in (real['area'], display_rect(gpu['control_before'])):
            if rect not in found:
                found.append(rect)
        print(' '.join('%d,%d,%d,%d' % tuple(r) for r in found))
        return 0
    if argv[1] == 'same':
        a = json.load(open(argv[2]))
        b = json.load(open(argv[3]))
        keys = ('nodes', 'words', 'kinds')
        if any(a[k] != b[k] for k in keys):
            print('the noisy run drew a different ordering table: ' + ', '.join(
                '%s %s != %s' % (k, a[k], b[k]) for k in keys if a[k] != b[k]), file=sys.stderr)
            return 1
        return 0
    return 2


if __name__ == '__main__':
    sys.exit(main(sys.argv))
