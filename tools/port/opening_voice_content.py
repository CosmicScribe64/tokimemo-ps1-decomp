"""T-39002: export original prologue XA requests and PCM from the verified disc.

Uses the original packed clip tables, speaker offsets, and sound-driver filter
rules. Does not invent voice clips or depend on historical orchestration files.
"""
from __future__ import annotations
import argparse
import hashlib
import json
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile
import wave

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.assets.input import open_input
from tools.assets.media_stream_decode import _actual_sector, _classify
from tools.port.prologue_runtime_content import build_bundle

TRACK_SHA = '28c3c501b0c67b87ca1e30515da73619497d509ff6adc28ff722aa02897dfb72'
SOURCE_PINS = {
    'src/main/sound_driver.c': '3ad56a3fd1bef48f7d70f882012ae0b13b123c86f12b8d47992ba9b032ed638b',
    'src/main/cd_queue.c': '722050726a5240caffcc107cef6a8806dff428fdf4f92a087a081a7f4cf0f0e9',
    'src/ovl/name_entry/prologue.c': 'cce3a4abaf102b745e0b116fcd1331ec63b35234d45b5bab4cbd9c39fc10470e',
}


def sha(data):
    return hashlib.sha256(data).hexdigest()


def encode(value):
    return (json.dumps(value, sort_keys=True, indent=2) + '\n').encode()


def native_request(speaker, pair, offsets):
    """sound_driver.c audio_command_xa; fresh boot's D_80125D5D is zero."""
    index = speaker & 15
    if index == 15:
        raise ValueError('silent player line has no XA request')
    start, end = pair
    # The source is the entire raw track, whose first sector is absolute LBA 0.
    return {'source_sha256': TRACK_SHA, 'source_lba': 0,
            'start_lba': (start & 0xffffff) * 32 + offsets[index] - 16,
            'end_lba': (end & 0xffffff) * 32 + offsets[index] + 16,
            'file_number': (end >> 24) & 255, 'channel': (start >> 24) & 255,
            'native_evidence': 'src/main/cd_queue.c:cd_audio_request_set; src/main/sound_driver.c:audio_command_xa/audio_xa_play'}


def export(track, output):
    track, output = Path(track).absolute(), Path(output).absolute()
    for path in (track, output):
        if '..' in path.parts or any(p.is_symlink() for p in (path, *path.parents)):
            raise ValueError('input/output must not traverse links or parent segments')
    if not track.is_relative_to(ROOT / 'disc') or not output.is_relative_to(ROOT / '.asset-cache/port') or output.exists():
        raise ValueError('use original disc input and a new immutable .asset-cache/port output')
    for relative, expected in SOURCE_PINS.items():
        if sha((ROOT / relative).read_bytes()) != expected:
            raise ValueError('source evidence changed: ' + relative)
    with open_input(str(track)) as source:
        if source.describe().get('format') != 'mode2-2352' or source.describe().get('source_sha256') != TRACK_SHA:
            raise ValueError('original raw track identity differs')
        exe_entry = next(e for e in source.entries() if e['path'] == 'SLPM_86.053')
        exe = source.read(exe_entry)
    offsets = struct.unpack_from('<16I', exe, 0x72e48)
    prologue = build_bundle(str(track), ROOT)
    output.parent.mkdir(parents=True, exist_ok=True)
    stage = Path(tempfile.mkdtemp(prefix='.opening-voice-', dir=output.parent))
    clips, lines = {}, []
    try:
        with track.open('rb') as raw:
            for line in prologue['lines']:
                requests = []
                if line['speaker'] & 15 != 15:
                    pairs = [line['clip1']]
                    # prologue_line_run starts the second clip only when its
                    # packed starting word differs from the first.
                    if line['clip2'][0] != line['clip1'][0]:
                        pairs.append(line['clip2'])
                    for pair in pairs:
                        request = native_request(line['speaker'], pair, offsets)
                        key = sha(encode(request))
                        if key not in clips:
                            clips[key] = _export_clip(raw, request, stage / ('clip-' + key))
                        requests.append({'pack_directory': 'clip-' + key, 'manifest_sha256': clips[key]})
                lines.append({'line': line['line_id'], 'speaker': line['speaker'], 'requests': requests})
        manifest = {'schema': 'tokimemo.original-opening-audio-map.v1', 'lines': lines,
                    'source_track_sha256': TRACK_SHA, 'source_code': SOURCE_PINS,
                    'scope': 'fresh prologue, native sector windows and filters; audible timing requires runtime verification'}
        payload = encode(manifest)
        (stage / 'opening-audio.json').write_bytes(payload)
        if output.exists():
            raise ValueError('output appeared during export')
        stage.rename(output)
        return {'manifest_sha256': sha(payload), 'lines': len(lines), 'clips': len(clips), 'output': str(output)}
    finally:
        if stage.exists():
            shutil.rmtree(stage)


def _export_clip(raw, request, destination):
    start, end = request['start_lba'], request['end_lba']
    if start < 0 or end <= start or end - start > 32768:
        raise ValueError('native XA sector interval is out of bounds')
    sectors, coding = [], None
    raw.seek(start * 2352)
    for lba in range(start, end):
        sector = raw.read(2352)
        record = _actual_sector(sector, lba - start, start)
        if record['file_number'] != request['file_number'] or record['channel'] != request['channel']:
            continue
        if _classify(record) != 'audio':
            continue
        if coding is not None and coding != record['coding']:
            raise ValueError('XA coding changes inside a native voice clip')
        coding = record['coding']
        sectors.append(sector)
    if not sectors:
        raise ValueError('native request selects no original XA audio sectors')
    channels, rate = (2 if coding & 1 else 1), (18900 if coding & 4 else 37800)
    destination.mkdir()
    with tempfile.TemporaryDirectory(prefix='tokimemo-xa-') as scratch:
        source = Path(scratch) / 'clip.raw2352'
        pcm_path = Path(scratch) / 'clip.pcm'
        source.write_bytes(b''.join(sectors))
        subprocess.run(['ffmpeg', '-nostdin', '-hide_banner', '-loglevel', 'error', '-xerror',
            '-threads', '1', '-f', 'psxstr', '-i', str(source), '-map', '0:a:0', '-vn',
            '-frames:a', str(len(sectors)), '-c:a', 'pcm_s16le', '-f', 's16le', str(pcm_path)], check=True, timeout=60)
        pcm = pcm_path.read_bytes()
    frames = len(sectors) * (2016 if channels == 2 else 4032)
    if len(pcm) != frames * channels * 2:
        raise ValueError('decoded sample count differs from original sector coding')
    wav_path = destination / 'voice.wav'
    with wave.open(str(wav_path), 'wb') as wav:
        wav.setparams((channels, 2, rate, 0, 'NONE', 'not compressed'))
        wav.writeframes(pcm)
    wav_bytes = wav_path.read_bytes()
    record = {'schema': 'tokimemo.original-xa-pcm.v1', 'capability': 'original_xa_software_pcm',
        'request': request, 'raw_selected_sha256': sha(b''.join(sectors)), 'selected_sectors': len(sectors),
        'descriptor': {'clip_id': 'xa-' + sha(pcm), 'path': 'voice.wav', 'sha256': sha(wav_bytes),
                       'byte_size': len(wav_bytes), 'sample_rate': rate, 'channels': channels, 'format': 'pcm16'}}
    payload = encode(record)
    (destination / 'original-audio.json').write_bytes(payload)
    return sha(payload)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--track', required=True)
    parser.add_argument('--output', required=True)
    args = parser.parse_args()
    try:
        print(json.dumps(export(args.track, args.output), sort_keys=True))
    except (OSError, ValueError, subprocess.SubprocessError) as exc:
        parser.exit(2, 'Opening voice export failed: ' + str(exc) + '\n')
