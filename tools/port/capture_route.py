#!/usr/bin/env python3
"""T-29002: Docker-only bounded native probes and original boundary capture.

Run in tokimemo-emu. Output must be beneath an ignored evidence directory.
The BIOS probe diverts RAM execution into invented instructions after boot; it
is an instruction/BIOS witness, never an ordinary gameplay-route capture.
"""

from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import sys
from types import SimpleNamespace


ROOT = Path(__file__).resolve().parents[2]
PROFILE = "openbios-d93840921b5ad7ed515e95b0e414f1b1e2038b1a"
BIOS_SHA256 = "9b15d285c3aadd7355edeaf0f32aa1d6eda536f02a5cfff40c1530044eaf2ff4"
ARITHMETIC_CASES = [
    ("multu", 0xFFFFFFFF, 0xFFFFFFFF), ("mult", 0x80000000, 0xFFFFFFFF),
    ("mult", 0xFFFFFFFF, 2), ("div", 0xFFFFFFF9, 3),
    ("div", 0x80000000, 0xFFFFFFFF), ("div", 0xFFFFFFFF, 0),
    ("div", 7, 0), ("divu", 0xFFFFFFFF, 0),
    ("divu", 0xFFFFFFFF, 3), ("sllv", 0x80000001, 32),
    ("srlv", 0x80000001, 33), ("srav", 0x80000001, 33),
    ("addu", 0x7FFFFFFF, 1), ("subu", 0x80000000, 1),
]


def evidence_directory(root: Path, destination: Path) -> Path:
    """Keep raw RAM/register/BIOS evidence out of public fixtures."""
    path = destination.resolve()
    allowed = [root / "build/emu", root / ".claude/orchestrator/state"]
    if not any(path.is_relative_to(base.resolve()) for base in allowed):
        raise ValueError("evidence output must be beneath build/emu or .claude/orchestrator/state")
    path.mkdir(parents=True, exist_ok=False)
    return path


def load_emulator(root: Path):
    spec = importlib.util.spec_from_file_location("tokimemo_emu", root / "tools/emu/emu.py")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def load_immediate(register: int, value: int) -> list[int]:
    value &= 0xFFFFFFFF
    return [(15 << 26) | (register << 16) | (value >> 16),
            (13 << 26) | (register << 21) | (register << 16) | (value & 0xFFFF)]


def bios_program(seeds: list[int], draws: int) -> tuple[list[int], int]:
    """Execute BIOS A0 calls directly, storing return and state after each draw."""
    words = load_immediate(16, 0x801D4000)  # s0: synthetic output buffer
    offset = 0
    for seed in seeds:
        words += load_immediate(4, seed)
        words += [0x24090030, 0x0C000028, 0]  # t1=30; jal 800000a0; nop
        for _ in range(draws):
            words += [0x2409002F, 0x0C000028, 0]  # t1=2f; jal 800000a0
            words += [(43 << 26) | (16 << 21) | (2 << 16) | offset]
            words += load_immediate(8, 0x0000850C)
            words += [0x8D030000, 0]  # lw v1,0(t0), load delay
            words += [(43 << 26) | (16 << 21) | (3 << 16) | (offset + 4)]
            offset += 8
    functions = {"mult": 24, "multu": 25, "div": 26, "divu": 27,
                 "sllv": 4, "srlv": 6, "srav": 7, "addu": 33, "subu": 35}
    for operation, left, right in ARITHMETIC_CASES:
        words += load_immediate(8, left) + load_immediate(9, right)
        if operation in ("mult", "multu", "div", "divu"):
            words += [(8 << 21) | (9 << 16) | functions[operation], 0x00001012, 0x00001810]
        elif operation.endswith("v"):
            words += [(9 << 21) | (8 << 16) | (2 << 11) | functions[operation], 0x00001821]
        else:
            words += [(8 << 21) | (9 << 16) | (2 << 11) | functions[operation], 0x00001821]
        words += [(43 << 26) | (16 << 21) | (2 << 16) | offset,
                  (43 << 26) | (16 << 21) | (3 << 16) | (offset + 4)]
        offset += 8
    stop_pc = 0x801D0000 + len(words) * 4
    words += [0x08000000 | ((stop_pc >> 2) & 0x03FFFFFF), 0]
    if len(words) * 4 >= 0x4000 or offset >= 0x4000:
        raise ValueError("probe exceeds its bounded synthetic RAM region")
    return words, stop_pc


def probe_bios(args) -> None:
    emu = load_emulator(args.root)
    out = evidence_directory(args.root, args.out)
    bios = Path(emu.BIOS)
    bios_hash = hashlib.sha256(bios.read_bytes()).hexdigest()
    if bios_hash != BIOS_SHA256:
        raise ValueError("BIOS image differs from measured profile; recover and review the new profile first")
    words, stop_pc = bios_program(args.seed, args.draws)
    lua = r'''
local ffi = require('ffi')
local mem = PCSX.getMemPtr()
local rom = PCSX.getRomPtr()
local regs = PCSX.getRegisters()
local function u32(x) return tonumber(ffi.cast('uint32_t', x)) end
local function hex(p, n)
    return (ffi.string(p, n):gsub('.', function(c) return string.format('%02x', c:byte()) end))
end
local frame = 0
local started = false
local rand_entry, srand_entry, rand_code, srand_code
NativeProbeBreak = PCSX.addBreakpoint(STOP_PC, 'Exec', 4, 'port-native-probe', function()
    local words = ffi.cast('uint32_t*', mem + 0x1d4000)
    local rows = {}
    for i = 0, OUTPUT_WORDS - 1 do rows[#rows + 1] = string.format('%u', u32(words[i])) end
    local f = Support.File.open(OUTPUT_PATH, 'TRUNCATE')
    if f:failed() then error('cannot write native probe output') end
    f:write(string.format('{"rand_entry":%u,"srand_entry":%u,"rand_code":"%s","srand_code":"%s","words":[%s]}',
        rand_entry, srand_entry, rand_code, srand_code, table.concat(rows, ',')))
    f:close()
    PCSX.quit(0)
    return true
end, 'native-probe-end')
NativeProbeVsync = PCSX.Events.createEventListener('GPU::Vsync', function()
    frame = frame + 1
    if frame == 40 and not started then
        started = true
        PCSX.nextTick(function()
            rand_entry = u32(ffi.cast('uint32_t*', mem + 0x2bc)[0])
            srand_entry = u32(ffi.cast('uint32_t*', mem + 0x2c0)[0])
            if rand_entry ~= 0xbfc061d8 or srand_entry ~= 0xbfc06204 then
                error('BIOS dispatch differs from measured OpenBIOS profile')
            end
            rand_code = hex(rom + 0x61d8, 44)
            srand_code = hex(rom + 0x6204, 12)
            local code = ffi.cast('uint32_t*', mem + 0x1d0000)
            for i, word in ipairs(PROGRAM) do code[i - 1] = word end
            regs.GPR.n.sp = 0x801feff0
            regs.pc = 0x801d0000
        end)
    end
    if frame > 100 then error('native probe did not complete') end
end)
'''
    bindings = {"STOP_PC": stop_pc, "OUTPUT_WORDS": (len(args.seed) * args.draws + len(ARITHMETIC_CASES)) * 2,
                "OUTPUT_PATH": str(out / "native.json"), "PROGRAM": words}
    script = out / "probe.lua"
    script.write_text("\n".join(f"{key} = {emu.lua_value(value)}" for key, value in bindings.items()) + lua)
    environment = dict(os.environ, SDL_AUDIO_DRIVER="dummy", SDL_AUDIODRIVER="dummy", SDL_VIDEO_DRIVER="dummy")
    command = [emu.REDUX, "-no-ui", "-stdout", "-lua_stdout", "-run", "-interpreter", "-debugger",
               "-bios", str(bios), "-iso", emu.find_iso(SimpleNamespace(iso=args.iso)), "-dofile", str(script)]
    with (out / "emulator.log").open("w") as log:
        subprocess.run(command, cwd=out, env=environment, stdout=log, stderr=subprocess.STDOUT,
                       timeout=args.timeout, check=True)
    raw = json.loads((out / "native.json").read_text())
    streams = []
    cursor = iter(raw.pop("words"))
    for seed in args.seed:
        streams.append({"seed": seed & 0xFFFFFFFF,
                        "draws": [{"result": next(cursor), "state": next(cursor)} for _ in range(args.draws)]})
    result = {"schema": "tokimemo.native-bios-probe.v1", "profile": PROFILE,
              "bios_sha256": bios_hash,
              "method": "synthetic RAM program; BIOS A0 calls after boot frame 40",
              "gameplay_route": False, "streams": streams,
              "arithmetic": [{"operation": op, "left": left, "right": right,
                              "lo": next(cursor), "hi": next(cursor)}
                             for op, left, right in ARITHMETIC_CASES], **raw}
    (out / "witness.json").write_text(json.dumps(result, indent=2) + "\n")
    print(f"BIOS probe: {len(streams)} streams, {args.draws} draws each; {out / 'witness.json'}")


def capture_boundary(args) -> None:
    """Capture a supplied ordinary boundary; never synthesize gameplay RAM."""
    state = args.state.resolve()
    if not state.is_file():
        raise ValueError("a real emulator save state is required; no invented command boundary")
    bios_hash = hashlib.sha256(Path(load_emulator(args.root).BIOS).read_bytes()).hexdigest()
    if bios_hash != BIOS_SHA256:
        raise ValueError("boundary capture requires the measured BIOS profile")
    out = evidence_directory(args.root, args.out)
    command = [sys.executable, str(args.root / "tools/emu/emu.py"), "break",
               "settings_command_screen_icon_confirm", "--state", str(state),
               "--hits", "1", "--frames", str(args.frames), "--timeout", str(args.timeout),
               "--dump", "800e6280:4544", "--dump", "0000850c:4", "--ram-dump",
               "--save-state", str(out / "command_boundary.sstate"), "--out", str(out / "capture")]
    if args.input:
        command += ["--input", args.input]
    if args.iso:
        command += ["--iso", args.iso]
    subprocess.run(command, cwd=args.root, check=True)
    captured = json.loads((out / "capture/result.json").read_text())
    hits = captured.get("hits", [])
    if not isinstance(hits, list) or len(hits) != 1:
        raise ValueError("ordinary command-confirm boundary was not reached")
    hit = hits[0]
    receipt = {"schema": "tokimemo.port-boundary-receipt.v1", "profile": PROFILE,
               "bios_sha256": bios_hash,
               "state_sha256": hashlib.sha256(state.read_bytes()).hexdigest(),
               "target": "settings_command_screen_icon_confirm@MAIN", "frame": hit["frame"],
               "input": args.input, "capture_sha256": hashlib.sha256((out / "capture/result.json").read_bytes()).hexdigest(),
               "remaining": ["prove ordinary date/command eligibility", "capture executed command 0/7 branches",
                             "project intermediate mutations", "capture actor RNG and service phase schedule"]}
    (out / "receipt.json").write_text(json.dumps(receipt, indent=2) + "\n")
    print(f"Original boundary captured; command-route parity remains unproved: {out / 'receipt.json'}")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--root", type=Path, default=ROOT)
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--iso")
    parser.add_argument("--timeout", type=int, default=60)
    commands = parser.add_subparsers(dest="command", required=True)
    probe = commands.add_parser("bios-probe")
    probe.add_argument("--seed", type=lambda x: int(x, 0), action="append")
    probe.add_argument("--draws", type=int, default=8)
    boundary = commands.add_parser("boundary")
    boundary.add_argument("--state", type=Path, required=True)
    boundary.add_argument("--frames", type=int, default=600)
    boundary.add_argument("--input", default="")
    args = parser.parse_args()
    args.root = args.root.resolve()
    if args.command == "bios-probe":
        args.seed = args.seed or [0, 1, 0xFFFFFFFF, 0x80000000]
        if not 1 <= args.draws <= 32 or len(args.seed) > 8:
            parser.error("bounded probe accepts 1..32 draws and at most eight seeds")
        probe_bios(args)
    else:
        capture_boundary(args)


if __name__ == "__main__":
    try:
        main()
    except (ValueError, OSError, subprocess.SubprocessError) as error:
        sys.exit(str(error))
