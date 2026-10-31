"""Route driver, emulator side: post-process a run directory written by player.lua.

  <tag>.shot.raw -> <tag>.png       (PCSX-Redux screenshot geometry from checkpoints.jsonl; the same
                                     rules as tools/emu/emu.py and the newgame golden's shots.py)
  <tag>.ram.bin  -> <tag>.decoded.json  with --decoder: the newgame golden's decode.py functions, so the
                                     fields carry the source header names that compare_newgame_golden.py
                                     reads (checkpoints without RAM are skipped)

usage (Docker, tokimemo-recovery): python3 -I tools/emu/route_driver/postprocess.py RUN [--decoder decode.py --repo ROOT]
"""
import argparse
import importlib.util
import json
from pathlib import Path
import struct
import sys

from PIL import Image


def convert_shots(run: Path) -> list:
    done = []
    for line in (run / "checkpoints.jsonl").read_text().splitlines():
        rec = json.loads(line)
        shot = rec.get("shot")
        raw_path = run / ("%s.shot.raw" % rec["tag"])
        if not shot or not raw_path.exists():
            continue
        w, h, bpp = shot["width"], shot["height"], shot["bpp"]
        raw = raw_path.read_bytes()
        if not (0 < w <= 1024 and 0 < h <= 512) or len(raw) != w * h * bpp // 8:
            print(rec["tag"], "rejected screenshot geometry", w, h, bpp, len(raw))
            continue
        if bpp == 16:
            px = bytearray()
            for i in range(0, len(raw), 2):
                v = raw[i] | raw[i + 1] << 8
                px += bytes(((v & 31) << 3, (v >> 5 & 31) << 3, (v >> 10 & 31) << 3))
            image = Image.frombytes("RGB", (w, h), bytes(px))
        else:
            image = Image.frombytes("RGB", (w, h), raw)
        image.save(run / ("%s.png" % rec["tag"]))
        done.append(rec["tag"])
    return done


def decode_ram(run: Path, decoder: Path, repo: Path) -> list:
    spec = importlib.util.spec_from_file_location("golden_decode", decoder)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    types = module.Types(repo)
    syms = module.load_symbols(repo)
    decls = module.load_decls(repo)
    globs = module.session_globals(repo, syms, decls)
    done = []
    for line in (run / "checkpoints.jsonl").read_text().splitlines():
        cp = json.loads(line)
        ram_path = run / ("%s.ram.bin" % cp["tag"])
        if not ram_path.exists():
            continue
        ram = ram_path.read_bytes()
        gs_off = module.GAME_STATE - module.RAM_BASE
        gs = types.decode("GameState", ram, gs_off)
        gsb = ram[gs_off:gs_off + module.GAME_STATE_SIZE]
        bday = struct.unpack_from("<I", gsb, 0xF8)[0]
        derived = {"player_birthday_bits": {"month": bday & 0xF, "day": (bday >> 4) & 0x1F, "blood_type": (bday >> 9) & 3}}
        sd = types.decode("SystemData", ram, module.SYSTEM_DATA - module.RAM_BASE)
        doc = {"checkpoint": cp, "ram_bin": ram_path.name,
               "game_state": {"addr": "0x%08X" % module.GAME_STATE, "type": "GameState (include/game/game_state.h)",
                              "fields": gs, "derived": derived},
               "system_data": {"addr": "0x%08X" % module.SYSTEM_DATA, "type": "SystemData (include/game/system_data.h)", "fields": sd},
               "session_globals": {g["name"]: module.decode_global(types, g, ram) for g in globs}}
        (run / ("%s.decoded.json" % cp["tag"])).write_text(json.dumps(doc, indent=1, ensure_ascii=False))
        done.append(cp["tag"])
    return done


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("run", type=Path)
    parser.add_argument("--decoder", type=Path)
    parser.add_argument("--repo", type=Path)
    args = parser.parse_args(argv)
    if not (args.run / "checkpoints.jsonl").exists():
        print("no checkpoints in", args.run)
        return 0
    shots = convert_shots(args.run)
    decoded = decode_ram(args.run, args.decoder, args.repo) if args.decoder else []
    print("postprocess: %d png, %d decoded" % (len(shots), len(decoded)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
