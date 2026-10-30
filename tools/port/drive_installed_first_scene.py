#!/usr/bin/env python3
"""Drive the installed opening observer with real X11 keyboard events in Docker."""
import argparse
import json
import os
from pathlib import Path
import subprocess
import time


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--boundary', action='store_true', help='Observe the next unsupported source boundary')
    parser.add_argument('--settings', action='store_true', help='Verify opt-in reading holds in the actual installed opening')
    parser.add_argument('--rei', action='store_true', help='Observe source stage7 to stage9 Rei transition')
    parser.add_argument('--handoff-layout', action='store_true', help='Capture resized native entry/opening handoff')
    parser.add_argument('--entry-key', choices=['Return', 'g'], default='Return', help='Physical Start key; g exercises a saved custom binding in the settings observer')
    args = parser.parse_args()
    observer = 'run_installed_next_arrival_boundary.gd' if args.boundary else 'run_installed_first_scene.gd'
    if args.settings:
        observer = 'services/run_installed_settings_hold.gd'
    if args.rei:
        observer = 'opening/run_installed_rei_banner.gd'
    if args.handoff_layout:
        observer = 'opening/run_installed_handoff_layout.gd'
    output = Path('/out')
    env = dict(os.environ, GODOT_SILENCE_ROOT_WARNING='1')
    env['TOKIMEMO_TEST_START_KEY'] = args.entry_key
    with (output / 'godot.log').open('w') as log:
        process = subprocess.Popen([
            'godot', '--path', '/work/port/godot', '--audio-driver', 'PulseAudio',
            '--rendering-method', 'gl_compatibility', '--script',
            'res://tests/' + observer,
        ], stdout=log, stderr=subprocess.STDOUT, env=env)
        seen = set()
        deadline = time.monotonic() + 180
        try:
            while process.poll() is None and time.monotonic() < deadline:
                status = output / 'entry-keyboard-status.json'
                try:
                    phase = json.loads(status.read_text()).get('phase')
                except (OSError, ValueError):
                    phase = None
                if phase and phase.startswith('entry_') and phase not in seen:
                    windows = subprocess.check_output(
                        ['xdotool', 'search', '--onlyvisible', '--pid', str(process.pid)], text=True
                    ).splitlines()
                    if not windows:
                        raise RuntimeError('Godot has no visible X11 window')
                    subprocess.run(['xdotool', 'windowfocus', '--sync', windows[-1]], check=True)
                    subprocess.run(['xdotool', 'keydown', args.entry_key], check=True)
                    # Keep the final physical START held through source handoff.
                    if phase != 'entry_6':
                        time.sleep(0.12)
                        subprocess.run(['xdotool', 'keyup', args.entry_key], check=True)
                    seen.add(phase)
                    print('physical keyboard:', phase, flush=True)
                time.sleep(0.05)
            if process.poll() is None:
                raise TimeoutError('Installed opening exceeded 180 seconds; see godot.log')
            receipt = json.loads((output / 'entry-keyboard-status.json').read_text())
            if process.returncode or receipt.get('phase') != 'passed':
                raise RuntimeError('Installed opening failed; see godot.log and receipt')
        finally:
            subprocess.run(['xdotool', 'keyup', args.entry_key], check=False)
            if process.poll() is None:
                process.terminate()
                try:
                    process.wait(timeout=5)
                except subprocess.TimeoutExpired:
                    process.kill()
                    process.wait()


if __name__ == '__main__':
    main()
