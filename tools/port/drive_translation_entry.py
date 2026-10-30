#!/usr/bin/env python3
"""T-39004: capture source/English actual installed screens with physical input."""
import json
import os
from pathlib import Path
import subprocess
import time


def main():
    output = Path('/out')
    with (output/'godot.log').open('w') as log:
        process = subprocess.Popen(['godot','--path','/work/port/godot',
            '--audio-driver','PulseAudio','--rendering-method','gl_compatibility',
            '--script','res://tests/translation/'+('run_installed_name_tabs.gd' if os.environ.get('TOKIMEMO_CAPTURE_NAME_TABS')=='1' else 'run_installed_name_images.gd')],
            stdout=log,stderr=subprocess.STDOUT,env=dict(os.environ,GODOT_SILENCE_ROOT_WARNING='1'))
        deadline = time.monotonic()+180
        seen = set()
        try:
            while process.poll() is None and time.monotonic()<deadline:
                try:
                    status = json.loads((output/'entry-keyboard-status.json').read_text())
                    phase = status.get('phase')
                except (OSError,ValueError):
                    phase = None
                if phase and phase.startswith(('entry_','tabs_')) and phase not in seen:
                    windows = subprocess.check_output(['xdotool','search','--onlyvisible','--pid',str(process.pid)],text=True).splitlines()
                    if not windows: raise RuntimeError('actual Godot window missing')
                    subprocess.run(['xdotool','windowfocus','--sync',windows[-1]],check=True)
                    geometry = dict(line.split('=',1) for line in subprocess.check_output(
                        ['xdotool','getwindowgeometry','--shell',windows[-1]],text=True).splitlines() if '=' in line)
                    width,height = int(geometry['WIDTH']),int(geometry['HEIGHT'])
                    transform = status['native_canvas']
                    if transform['viewport_size'] != [width,height]: raise RuntimeError('physical window and observed viewport disagree')
                    if transform['scale'][0] != transform['scale'][1]: raise RuntimeError('native canvas has nonuniform scale')
                    scale = transform['scale'][0]
                    origin = transform['position']
                    (output/'physical-window.json').write_text(json.dumps({'width':width,'height':height,'native_scale':scale,'origin':origin}))
                    def click(x,y):
                        subprocess.run(['xdotool','mousemove','--window',windows[-1],str(round(origin[0]+x*scale)),str(round(origin[1]+y*scale))],check=True)
                        subprocess.run(['xdotool','click','1'],check=True)
                        time.sleep(.15)
                    if phase.startswith('tabs_'):
                        if phase in ('tabs_pause','tabs_resume','tabs_focus_away','tabs_focus_back'):
                            if phase=='tabs_pause':
                                subprocess.run(['xdotool','key','F10'],check=True)
                                subprocess.run(['xdotool','keydown','Right'],check=True)
                            elif phase=='tabs_resume':
                                subprocess.run(['xdotool','keyup','Right'],check=True)
                                subprocess.run(['xdotool','key','F10'],check=True)
                            elif phase=='tabs_focus_away':
                                subprocess.run(['xdotool','windowfocus','0'],check=True)
                                subprocess.run(['xdotool','keydown','Right'],check=True)
                            else:
                                subprocess.run(['xdotool','keyup','Right'],check=True)
                                subprocess.run(['xdotool','windowfocus',windows[-1]],check=True)
                            seen.add(phase)
                            continue
                        point={'tabs_0':(16,143),'tabs_1':(304,144),'tabs_2':(16,175),
                               'tabs_3':(64,184),'tabs_4':(48,96),'tabs_5':(64,42)}[phase]
                        if os.environ.get('TOKIMEMO_NAME_KEYBOARD')=='1':
                            target=(min(point[0],300),point[1])
                            for attempt in range(500):
                                try:
                                    live=json.loads((output/'name-pointer-live.json').read_text())
                                except (OSError,ValueError):
                                    time.sleep(.01);continue
                                if not live['available']:raise RuntimeError('source cursor inventory is unavailable')
                                x,y=live['pointer']['position']
                                dx,dy=target[0]-x,target[1]-y
                                if abs(dx)<=2 and abs(dy)<=2:break
                                key=('Right' if dx>0 else 'Left') if abs(dx)>2 else ('Down' if dy>0 else 'Up')
                                subprocess.run(['xdotool','keydown',key],check=True)
                                time.sleep(.018)
                                subprocess.run(['xdotool','keyup',key],check=True)
                                time.sleep(.035)
                            else:raise RuntimeError('physical directional cursor did not reach '+str(target))
                            subprocess.run(['xdotool','keydown','space'],check=True)
                            time.sleep(.05)
                            subprocess.run(['xdotool','keyup','space'],check=True)
                            time.sleep(.05)
                        else:click(*point)
                        seen.add(phase)
                        continue
                    if phase == 'entry_0':
                        click(48,80)
                        subprocess.run(['xdotool','click','3'],check=True)
                        time.sleep(.15)
                        click(64,80)  # Replace the first slot after mouse-only backtracking.
                    if phase in ['entry_1','entry_2']:
                        click(64,80)  # Explicit given/nickname; avoid random blank-name defaults.
                    if phase in ['entry_3','entry_5']:
                        click(68,88)  # January, selected month field.
                        click(48,118)  # Native day label changes field via recorded actions.
                        click(60,136)  # Day 1.
                        click(48,182)  # Blood field label.
                        click(72,200)  # A.
                    subprocess.run(['xdotool','mousemove','--window',windows[-1],'1000','750'],check=True)
                    subprocess.run(['xdotool','keydown','Return'],check=True)
                    time.sleep(.12)
                    subprocess.run(['xdotool','keyup','Return'],check=True)
                    seen.add(phase)
                time.sleep(.05)
            if process.poll() is None: raise TimeoutError('actual entry translation capture timed out')
            receipt = json.loads((output/'entry-keyboard-status.json').read_text())
            if process.returncode or receipt.get('phase') != 'passed': raise RuntimeError('capture failed: '+str(receipt))
            print('Actual installed entry capture passed: '+', '.join(sorted(seen)))
        finally:
            subprocess.run(['xdotool','keyup','Return'],check=False)
            subprocess.run(['xdotool','keyup','Right','Left','Up','Down','space'],check=False)
            if process.poll() is None:
                process.terminate()
                try: process.wait(timeout=5)
                except subprocess.TimeoutExpired:
                    process.kill(); process.wait()


if __name__ == '__main__':
    main()
