#!/usr/bin/env python3
"""Dispatch X11 keyboard events to the isolated Godot save-controls test window.

Uses existing libX11 through ctypes. It never calls the app or constructs a
Godot InputEvent. Run in Docker against a dedicated Xvfb Unix socket.
"""
import argparse
import ctypes as c
import json
import pathlib
import time

class KeyEvent(c.Structure):
    _fields_ = [('type', c.c_int), ('serial', c.c_ulong), ('send_event', c.c_int),
                ('display', c.c_void_p), ('window', c.c_ulong), ('root', c.c_ulong),
                ('subwindow', c.c_ulong), ('time', c.c_ulong), ('x', c.c_int),
                ('y', c.c_int), ('x_root', c.c_int), ('y_root', c.c_int),
                ('state', c.c_uint), ('keycode', c.c_uint), ('same_screen', c.c_int)]

class Event(c.Union):
    _fields_ = [('key', KeyEvent), ('pad', c.c_long * 24)]

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--display', default=':97')
    parser.add_argument('--status', required=True, type=pathlib.Path)
    parser.add_argument('--timeout', default=45.0, type=float)
    parser.add_argument('--entry', action='store_true', help='drive installed name entry with seven explicit Enter actions')
    parser.add_argument('--hold-final-entry', action='store_true', help='retain the final Enter press across the real entry handoff')
    args = parser.parse_args()
    if args.hold_final_entry and not args.entry:
        parser.error('--hold-final-entry requires --entry')
    x = c.CDLL('libX11.so.6')
    x.XOpenDisplay.argtypes = [c.c_char_p]
    x.XOpenDisplay.restype = c.c_void_p
    x.XDefaultRootWindow.argtypes = [c.c_void_p]
    x.XDefaultRootWindow.restype = c.c_ulong
    x.XQueryTree.argtypes = [c.c_void_p, c.c_ulong, c.POINTER(c.c_ulong), c.POINTER(c.c_ulong), c.POINTER(c.POINTER(c.c_ulong)), c.POINTER(c.c_uint)]
    x.XQueryTree.restype = c.c_int
    x.XFetchName.argtypes = [c.c_void_p, c.c_ulong, c.POINTER(c.c_char_p)]
    x.XFetchName.restype = c.c_int
    x.XFree.argtypes = [c.c_void_p]
    x.XSetInputFocus.argtypes = [c.c_void_p, c.c_ulong, c.c_int, c.c_ulong]
    x.XRaiseWindow.argtypes = [c.c_void_p, c.c_ulong]
    x.XMapRaised.argtypes = [c.c_void_p, c.c_ulong]
    x.XSync.argtypes = [c.c_void_p, c.c_int]
    x.XStringToKeysym.argtypes = [c.c_char_p]
    x.XStringToKeysym.restype = c.c_ulong
    x.XKeysymToKeycode.argtypes = [c.c_void_p, c.c_ulong]
    x.XKeysymToKeycode.restype = c.c_ubyte
    x.XSendEvent.argtypes = [c.c_void_p, c.c_ulong, c.c_int, c.c_long, c.POINTER(Event)]
    x.XSendEvent.restype = c.c_int
    x.XFlush.argtypes = [c.c_void_p]
    x.XCloseDisplay.argtypes = [c.c_void_p]
    deadline = time.monotonic() + args.timeout
    display = None
    while not display and time.monotonic() < deadline:
        display = x.XOpenDisplay(args.display.encode())
        if not display: time.sleep(0.05)
    if not display: raise RuntimeError('isolated Xvfb display did not become available')
    root = x.XDefaultRootWindow(display)
    window = None
    window_title = ''
    observed_titles = set()
    while window is None and time.monotonic() < deadline:
        root_result, parent, count = c.c_ulong(), c.c_ulong(), c.c_uint()
        children = c.POINTER(c.c_ulong)()
        if x.XQueryTree(display, root, c.byref(root_result), c.byref(parent), c.byref(children), c.byref(count)):
            for index in range(count.value):
                name = c.c_char_p()
                if x.XFetchName(display, children[index], c.byref(name)) and name.value:
                    title = name.value.decode('utf-8', errors='replace')
                    observed_titles.add(title[:120])
                    if 'Tokimeki' in title or 'Godot' in title or (args.entry and title.startswith('New Game')):
                        window = int(children[index])
                        window_title = title
                    x.XFree(name)
                    if window is not None: break
            if children: x.XFree(children)
        if window is None: time.sleep(0.05)
    if window is None: raise RuntimeError('isolated Godot window did not become available; titles=' + repr(sorted(observed_titles)))
    x.XMapRaised(display, window)
    x.XSync(display, 0)
    x.XRaiseWindow(display, window)
    x.XSetInputFocus(display, window, 2, 0)
    x.XFlush(display)

    def send(name, pressed):
        code = x.XKeysymToKeycode(display, x.XStringToKeysym(name.encode()))
        if not code: raise RuntimeError('X11 key mapping unavailable: ' + name)
        event = Event()
        event.key = KeyEvent(2 if pressed else 3, 0, 1, display, window, root, 0, 0,
                             1, 1, 1, 1, 0, code, 1)
        if not x.XSendEvent(display, window, 0, 1 if pressed else 2, c.byref(event)):
            raise RuntimeError('X11 key dispatch failed')
        x.XFlush(display)

    actions = {'ready_f5': [('F5', True), ('F5', False)],
               'right_down': [('Right', True)], 'right_up': [('Right', False)],
               'enter_down': [('Return', True)], 'f9_held': [('F9', True), ('F9', False)],
               'enter_up': [('Return', False)], 'f9_released': [('F9', True), ('F9', False)]}
    if args.entry:
        actions = {'entry_%d' % index: [('Return', True), ('Return', False)] for index in range(7)}
        if args.hold_final_entry:
            actions['entry_6'] = [('Return', True)]
    dispatched = set()
    try:
        while time.monotonic() < deadline:
            try: status = json.loads(args.status.read_text())
            except (FileNotFoundError, json.JSONDecodeError):
                time.sleep(0.05)
                continue
            phase = status.get('phase')
            if phase == 'failed' or status.get('failed'): raise RuntimeError('Godot keyboard test reported a failed assertion')
            if phase == 'passed':
                print(json.dumps({'status': 'PASS', 'dispatch': 'XSendEvent/libX11', 'key_phases': len(dispatched), 'window_title': window_title}))
                return
            if phase in actions and phase not in dispatched:
                for name, pressed in actions[phase]: send(name, pressed)
                dispatched.add(phase)
            time.sleep(0.03)
        raise TimeoutError('keyboard test did not complete; phases=' + ','.join(sorted(dispatched)))
    finally:
        if args.hold_final_entry:
            send('Return', False)
        x.XCloseDisplay(display)

if __name__ == '__main__':
    main()
