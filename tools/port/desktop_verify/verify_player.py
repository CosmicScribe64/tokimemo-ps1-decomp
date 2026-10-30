"""Check an exported Linux arm64 player inside the tokimemo-port-gui-test image.

Run by tools/port/verify_desktop.sh, which mounts the build at /build (read-only),
this directory at /verify, the new run directory at /run-out and, optionally, a
content bundle from tools/port/setup_content.sh at /bundle. Every check uses a
fresh per-user data directory inside the container.

Without a bundle:
  - an empty install routes to first-launch setup and the native extension loads;
  - the BGM bridge's native-component gate (TOKIMEMO_ENTRY_DEBUG line
    "NativeAudioGate:") accepts the library beside the executable;
  - a copy of the player without the library reports the extension load error and
    the gate rejects it, so both checks can fail.
With a bundle, additionally:
  - the bundle is published through InstalledPackStore.publish_stage
    (publish_installed.gd, run by the pinned editor binary because export
    templates ignore --script);
  - the headless player routes to the New Game entry with no failed load gate;
  - under Xvfb/PulseAudio, real X11 Start presses drive the player into the
    opening; it must keep running, log no "New Game stopped at", and the null-sink
    monitor must record non-silent PCM. This exercises the packaged library through
    the BGM bridge. Key timing after the entry screen loads is blind (the exported
    player has no observer), so a failure needs log inspection.
No check proves gameplay readiness, PCM parity, timing or hardware audio behavior.
Exit status 0 means every check passed; 1 means a check failed.
"""
from __future__ import annotations

import argparse
import array
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import time

STORE_NAME = "Tokimeki Memorial Port Foundation"
GATE_REJECTED = "original opening BGM native library differs from the reviewed component"
EXTENSION_ERROR = "Error loading extension"


class CheckFailed(Exception):
    pass


def environment() -> dict[str, str]:
    env = dict(os.environ)
    for key, path in (("HOME", "/tmp/home"), ("XDG_DATA_HOME", "/tmp/xdg-data"),
                      ("XDG_CONFIG_HOME", "/tmp/xdg-config"), ("XDG_CACHE_HOME", "/tmp/xdg-cache")):
        env[key] = path
        Path(path).mkdir(parents=True, exist_ok=True)
    env["GODOT_SILENCE_ROOT_WARNING"] = "1"
    return env


def run(out: Path, log: str, command: list[str], env: dict[str, str]) -> str:
    """Run a command to completion, keep its output in out/log and return it."""
    with open(out / log, "wb") as handle:
        status = subprocess.run(command, stdout=handle, stderr=subprocess.STDOUT,
                                env=env, timeout=90, check=False).returncode
    text = (out / log).read_text(errors="replace")
    if status != 0:
        raise CheckFailed(f"{log}: exit status {status}")
    if "SCRIPT ERROR" in text:
        raise CheckFailed(f"{log}: script error")
    return text


def need(text: str, needle: str, log: str, reason: str) -> None:
    if needle not in text:
        raise CheckFailed(f"{log}: {reason}")


def refuse(text: str, needle: str, log: str, reason: str) -> None:
    if needle in text:
        raise CheckFailed(f"{log}: {reason}")


def tagged_json(text: str, tag: str, log: str) -> dict:
    """Return the JSON object printed after the last line starting with tag."""
    found = [line[len(tag):].strip() for line in text.splitlines() if line.startswith(tag)]
    if not found:
        raise CheckFailed(f"{log}: no '{tag}' line")
    try:
        value = json.loads(found[-1])
    except ValueError as error:
        raise CheckFailed(f"{log}: malformed '{tag}' line: {error}") from error
    if not isinstance(value, dict):
        raise CheckFailed(f"{log}: '{tag}' is not an object")
    return value


def check_gate(text: str, log: str, library: Path, expected_error: str) -> None:
    gate = tagged_json(text, "NativeAudioGate:", log)
    if gate.get("library_path") != str(library):
        raise CheckFailed(f"{log}: bridge hashes {gate.get('library_path')!r}, not {library}")
    if gate.get("error") != expected_error:
        raise CheckFailed(f"{log}: native gate verdict {gate.get('error')!r}, expected {expected_error!r}")


def headless_checks(build: Path, out: Path, env: dict[str, str]) -> tuple[Path, Path]:
    player = build / "player/tokimemo-port.arm64"
    libraries = sorted((build / "player").glob("liboriginal_audio*.so"))
    if len(libraries) != 1:
        raise CheckFailed("the export must hold exactly one liboriginal_audio library beside the player")
    debug = dict(env, TOKIMEMO_ENTRY_DEBUG="1")
    text = run(out, "empty-install.log", [str(player), "--headless", "--quit-after", "300"], debug)
    need(text, "BootRouter: setup -> res://app/first_launch_setup.tscn", "empty-install.log",
         "empty install did not route to first-launch setup")
    refuse(text, EXTENSION_ERROR, "empty-install.log", "packaged native audio extension did not load")
    check_gate(text, "empty-install.log", libraries[0], "")
    print("PASS: empty install routes to first-launch setup; extension loaded; bridge gate accepts "
          + libraries[0].name)

    bare = Path("/tmp/no-library")
    bare.mkdir()
    for name in ("tokimemo-port.arm64", "tokimemo-port.pck"):
        shutil.copy2(build / "player" / name, bare / name)
    text = run(out, "no-library.log", [str(bare / "tokimemo-port.arm64"), "--headless", "--quit-after", "30"],
               debug)
    need(text, EXTENSION_ERROR, "no-library.log", "missing library was not detected")
    check_gate(text, "no-library.log", bare / libraries[0].name, GATE_REJECTED)
    print("PASS: a player without the library reports the load error and the bridge gate rejects it")
    return player, libraries[0]


def publish(build: Path, out: Path, bundle: Path, env: dict[str, str]) -> None:
    store = Path(env["XDG_DATA_HOME"]) / "godot/app_userdata" / STORE_NAME / "content-install"
    (store / ".staging").mkdir(parents=True)
    shutil.copytree(bundle, store / ".staging/prepared-original", symlinks=True)
    project = Path("/tmp/project")
    shutil.copytree(build / "project", project, symlinks=True)
    text = run(out, "publish.log", ["godot", "--headless", "--path", str(project), "--script",
                                     "/verify/publish_installed.gd"],
               dict(env, TOKIMEMO_VERIFY_STORE_ROOT=str(store)))
    result = tagged_json(text, "TOKIMEMO_DESKTOP_PUBLISH", "publish.log")
    if not (result.get("valid") and result.get("launch_ready")):
        raise CheckFailed(f"publish.log: publication was not launch-ready: {result.get('message')!r}")
    print("PASS: content bundle published into the fresh per-user root")


def installed_entry(player: Path, library: Path, out: Path, env: dict[str, str]) -> None:
    text = run(out, "installed-entry.log", [str(player), "--headless", "--quit-after", "300"],
               dict(env, TOKIMEMO_ENTRY_DEBUG="1"))
    need(text, "BootRouter: playable -> res://app/new_game_entry.tscn", "installed-entry.log",
         "installed boot did not choose the New Game entry")
    refuse(text, EXTENSION_ERROR, "installed-entry.log", "packaged native audio extension did not load")
    check_gate(text, "installed-entry.log", library, "")
    entry = tagged_json(text, "T29012_ENTRY_LOAD_DIAGNOSTIC", "installed-entry.log")
    if entry.get("failed_gate") != "":
        raise CheckFailed(f"installed-entry.log: New Game entry failed gate {entry.get('failed_gate')!r}")
    print("PASS: exported player reached the New Game entry with no failed load gate")


def wait_for(predicate, seconds: float, reason: str):
    deadline = time.monotonic() + seconds
    while time.monotonic() < deadline:
        value = predicate()
        if value:
            return value
        time.sleep(0.25)
    raise CheckFailed(reason)


def xdotool(*args: str, env: dict[str, str]) -> str:
    return subprocess.run(["xdotool", *args], env=env, capture_output=True, text=True,
                          timeout=20, check=False).stdout.strip()


def pcm_audible(path: Path) -> tuple[bool, str]:
    data = path.read_bytes()
    pcm = array.array("h")
    pcm.frombytes(data[:len(data) // 2 * 2])
    peak = max((abs(sample) for sample in pcm), default=0)
    nonzero = sum(1 for sample in pcm if sample)
    summary = f"samples={len(pcm)} peak={peak} nonzero={nonzero}"
    # 48 kHz stereo: require over half a second of samples, mostly non-zero.
    return len(pcm) > 48000 and peak > 0 and nonzero * 2 > len(pcm), summary


def opening(player: Path, library: Path, out: Path, env: dict[str, str]) -> None:
    display = dict(env, DISPLAY=":99", TOKIMEMO_ENTRY_DEBUG="1")
    helpers = [subprocess.Popen(["Xvfb", ":99", "-screen", "0", "1280x1024x24", "-nolisten", "tcp"],
                                stdout=open(out / "xvfb.log", "wb"), stderr=subprocess.STDOUT)]
    run(out, "pulse.log", ["pulseaudio", "--start", "--exit-idle-time=-1"], env)
    log = out / "opening.log"
    # Release exports do not flush stdout per print; line-buffer it so the entry
    # diagnostic can be awaited while the player runs.
    process = subprocess.Popen(["stdbuf", "-oL", str(player), "--audio-driver", "PulseAudio", "--rendering-method",
                                "gl_compatibility"], stdout=open(log, "wb"), stderr=subprocess.STDOUT,
                               env=display)
    holding = False
    try:
        def log_text() -> str:
            return log.read_text(errors="replace")
        wait_for(lambda: "T29012_ENTRY_LOAD_DIAGNOSTIC" in log_text() or process.poll() is not None,
                 60, "opening.log: the New Game entry did not load")
        if process.poll() is not None:
            raise CheckFailed(f"opening.log: player exited with status {process.returncode}")
        check_gate(log_text(), "opening.log", library, "")
        window = wait_for(lambda: xdotool("search", "--onlyvisible", "--pid", str(process.pid),
                                          env=display).splitlines()[-1:],
                          20, "exported player has no visible X11 window")[0]
        xdotool("windowfocus", "--sync", window, env=display)
        for _ in range(6):
            xdotool("keydown", "Return", env=display)
            time.sleep(0.12)
            xdotool("keyup", "Return", env=display)
            time.sleep(2)
        xdotool("keydown", "Return", env=display)
        holding = True
        time.sleep(4)
        sink = subprocess.run(["pactl", "get-default-sink"], env=env, capture_output=True, text=True,
                              timeout=20, check=True).stdout.strip()
        with open(out / "opening.pcm", "wb") as pcm:
            recorder = subprocess.Popen(["parec", "-d", sink + ".monitor", "--format=s16le",
                                         "--rate=48000", "--channels=2"], stdout=pcm, env=env)
            time.sleep(20)
            recorder.terminate()
            recorder.wait(timeout=10)
        if process.poll() is not None:
            raise CheckFailed(f"opening.log: player exited during the opening ({process.returncode})")
        stops = [line for line in log_text().splitlines()
                 if "New Game stopped at" in line or "SCRIPT ERROR" in line]
        if stops:
            raise CheckFailed("opening.log: opening stopped: " + stops[0])
        audible, summary = pcm_audible(out / "opening.pcm")
        print("opening PCM: " + summary)
        if not audible:
            raise CheckFailed("opening.pcm: no audible output recorded during the opening")
    finally:
        if holding:
            xdotool("keyup", "Return", env=display)
        process.kill()
        process.wait(timeout=10)
        for helper in helpers:
            helper.kill()
    print("PASS: exported player passed the BGM bridge gates and produced audible opening output")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--build", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    parser.add_argument("--bundle", type=Path)
    args = parser.parse_args()
    env = environment()
    try:
        player, library = headless_checks(args.build, args.out, env)
        if args.bundle is not None:
            publish(args.build, args.out, args.bundle, env)
            installed_entry(player, library, args.out, env)
            opening(player, library, args.out, env)
    except (CheckFailed, subprocess.TimeoutExpired) as error:
        print(f"FAIL: {error} (logs in the run directory)", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
