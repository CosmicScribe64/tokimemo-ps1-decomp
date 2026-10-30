"""T-39002: restore original tracks and generated evidence in Docker, then export.

Never overwrites an existing installed bundle or extracts arbitrary archive paths.
Run through setup_content.sh; existing disc inputs are verified before reuse.
"""
from pathlib import Path
import argparse
import json
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.extract_disc import CUE, Image, extract_image


def run(*command):
    subprocess.run(command, cwd=ROOT, check=True)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('archive')
    parser.add_argument('--output', default='.asset-cache/port/installed-fresh')
    args = parser.parse_args()
    disc = ROOT / 'disc'
    output = ROOT / args.output
    cache = ROOT / '.asset-cache/port'
    if '..' in Path(args.output).parts or cache.resolve() not in output.resolve().parents:
        raise ValueError('output must be a new directory below .asset-cache/port')
    for component in (output, *output.parents):
        if component.is_symlink():
            raise ValueError('output cannot traverse symlinks')
    if output.exists():
        raise ValueError('output already exists; preserve it and retry with --output .asset-cache/port/installed-retry')
    if not disc.exists():
        disc.mkdir()
        # Flatten only selected track filenames, never archive-provided directories.
        run('7z', 'e', args.archive, '-o' + str(disc), '-ir!*.bin')
        for number in (1, 2):
            tracks = list(disc.glob(f'*(Track {number}).bin'))
            if len(tracks) != 1:
                raise ValueError(f'Expected exactly one Track {number}; inspect disc/ before retrying')
            tracks[0].rename(disc / f'track{number}.bin')
        (disc / 'disc.cue').write_text(CUE)
    track = disc / 'track1.bin'
    run(sys.executable, 'tools/identify_version.py', str(track))
    if not (disc / 'files/SLPM_86.053').exists():
        print(extract_image(Image(str(track)), str(disc)))
    configs = [ROOT / 'config/SLPM_86.053.yaml', *sorted((ROOT / 'config/overlays').glob('*.yaml'))]
    for config in configs:
        run(sys.executable, '-m', 'splat', 'split', str(config.relative_to(ROOT)))
    run(sys.executable, 'tools/port/build_installed_runtime.py', '--input', str(track),
        '--track', str(track), '--executable', str(disc / 'files/SLPM_86.053'), '--output', str(output))
    missing = json.loads((output / 'manifest.json').read_text())['coverage']['unavailable_roles']
    if missing:
        print('Partial content saved; setup is incomplete: ' + ', '.join(sorted(missing)), file=sys.stderr)
        return 3
    return 0


if __name__ == '__main__':
    try:
        sys.exit(main())
    except (OSError, ValueError, subprocess.CalledProcessError) as exc:
        print('Content setup failed: ' + str(exc), file=sys.stderr)
        sys.exit(2)
