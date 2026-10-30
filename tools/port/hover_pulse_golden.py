"""Numbers-only fixture for the scaled hover-pulse golden (func_80098680 path).

Reads a run of .asset-cache/emu/hover-pulse-golden-v1 (golden.lua shots: the
2 MiB RAM and the scan-out at each listed vsync; set A command-stage1 state,
RIGHT at vsync 40) and writes, for every shot, the HUD values of the RAM one
and two vsyncs earlier (the scan-out shows a frame built 1-2 vsyncs before):
the hover countdown D_800B5AA0, the hovered icon's actor 80 + i
(scale, sequence step, and the step it drew), the cursor actor 0 (x, y, frame)
and, for all fourteen icons (hud_icon_bar_update, hud_panels.c:1101-1125), the
icon actor 80 + i flags, the frame actor 64 + i palette byte unk_43 and the
step actor 80 + i drew. The test redraws the whole HUD around the scaled quad
from these. No pixels.

  docker run --rm --name hover-pulse-fixture -v "$PWD:/work" \
    -v <main>/.asset-cache/emu/hover-pulse-golden-v1:/g:ro -w /work tokimemo-recovery \
    python3 -I tools/port/hover_pulse_golden.py /g/runs/setA-right40 \
      port/godot/tests/fixtures/hover_pulse_golden.json
"""
import json
import struct
import sys
from pathlib import Path

ACTOR_POOL = 0x8011ECD0  # 0x44-byte ActorFixed records
ACTOR_SIZE = 0x44
HOVER_COUNTDOWN = 0x800B5AA0
MENU_CURSOR = 0x800B5A9C  # previous cursor index (hud_icon_bar_update)


def main(run: str, output: str) -> int:
    run_dir = Path(run)
    shots = {}
    for line in (run_dir / 'checkpoints.jsonl').read_text().splitlines():
        record = json.loads(line)
        shots[int(record['vsync'])] = record

    def ram(vsync):
        return (run_dir / f'v{vsync}.ram.bin').read_bytes()

    def word(data, address, fmt='<i'):
        return struct.unpack_from(fmt, data, address & 0x1FFFFF)[0]

    def actor(data, index, offset, fmt):
        return word(data, ACTOR_POOL + index * ACTOR_SIZE + offset, fmt)

    rows = []
    for vsync in sorted(shots):
        if vsync - 2 not in shots or vsync - 1 not in shots:
            continue
        candidates = []
        for back in (1, 2):
            data = ram(vsync - back)
            icon = word(data, MENU_CURSOR)
            candidates.append({
                'ram_vsync': vsync - back, 'countdown': word(data, HOVER_COUNTDOWN), 'hover_icon': icon,
                'icon_scale': [actor(data, 80 + icon, 0x1A, '<h'), actor(data, 80 + icon, 0x1C, '<h')],
                'icon_step': actor(data, 80 + icon, 0x18, '<h'),
                # actor_draw_all draws, then advances the sequence: the step a
                # frame drew is the step in the RAM one vsync earlier.
                'icon_step_drawn': actor(ram(vsync - back - 1), 80 + icon, 0x18, '<h') if vsync - back - 1 in shots else None,
                'cursor': [actor(data, 0, 0x26, '<h'), actor(data, 0, 0x2A, '<h'), actor(data, 0, 0x18, '<h')],
                'cursor_flags': actor(data, 0, 0x02, '<B'),
                'icons': [[actor(data, 80 + i, 0x02, '<B'), actor(data, 64 + i, 0x43, '<B'),
                           actor(ram(vsync - back - 1), 80 + i, 0x18, '<h') if vsync - back - 1 in shots else None]
                          for i in range(14)],
            })
        rows.append({'vsync': vsync, 'frame_count': shots[vsync]['frame_count'], 'candidates': candidates})
    fixture = {'schema': 'tokimemo.hover-pulse-golden.v1', 'source': 'hover-pulse-golden-v1/runs/' + run_dir.name,
               'input': 'setA command-stage1.sstate, RIGHT at vsync 40 for 2', 'shots': rows}
    Path(output).write_text(json.dumps(fixture, indent=1, sort_keys=True) + '\n')
    return 0


if __name__ == '__main__':
    raise SystemExit(main(*sys.argv[1:]))
