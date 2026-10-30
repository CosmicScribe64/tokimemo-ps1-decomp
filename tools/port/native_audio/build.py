"""T-39002: pinned dependency recovery and isolated native-audio candidate build.

Invoke build.sh. Never copies a candidate into the production Godot project.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import platform
import shutil
import subprocess
import sys
import tarfile
import urllib.request
import zipfile

ROOT = Path(__file__).resolve().parents[3]
SOURCE = Path(__file__).resolve().parent
SOURCE_FILES = ('core.hpp', 'gaussian.hpp', 'extension.cpp', 'SConstruct', 'build-profile.json')


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def output_path(value):
    path = ROOT / value
    cache = ROOT / '.asset-cache'
    if '..' in path.parts or cache.resolve() not in path.resolve().parents:
        raise ValueError('output must be a new directory below .asset-cache')
    if any(part.is_symlink() for part in (path, *path.parents)):
        raise ValueError('output cannot traverse symlinks')
    if path.exists():
        raise ValueError('output exists; preserve it and choose a new --output')
    return path


def download(spec, path):
    # Default TLS verification is mandatory; content identity is checked too.
    with urllib.request.urlopen(spec['url'], timeout=60) as response:
        data = response.read()
    if len(data) != spec['bytes'] or hashlib.sha256(data).hexdigest() != spec['sha256']:
        raise ValueError('dependency size/hash differs: ' + spec['url'])
    path.write_bytes(data)


def extract(archive, target, kind):
    target.mkdir()
    if kind == 'tar':
        with tarfile.open(archive) as source:
            for member in source.getmembers():
                parts = Path(member.name).parts[1:]
                if not parts:
                    continue
                member.name = str(Path(*parts))
                source.extract(member, target, filter='data')
    else:
        with zipfile.ZipFile(archive) as source:
            for name in source.namelist():
                if Path(name).is_absolute() or '..' in Path(name).parts:
                    raise ValueError('invalid wheel member')
            source.extractall(target)


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', default='.asset-cache/port/native-audio-build')
    parser.add_argument('--jobs', type=int, default=2)
    args = parser.parse_args(argv)
    if platform.system() != 'Linux' or platform.machine() not in ('aarch64', 'arm64'):
        raise ValueError('use build.sh with a Linux arm64 Docker compiler image')
    if not 1 <= args.jobs <= 16:
        raise ValueError('jobs must be between 1 and 16')
    output = output_path(args.output)
    compiler = subprocess.check_output(['g++', '--version'], text=True).splitlines()[0]
    metadata_path = SOURCE / 'dependency-provenance.json'
    metadata = json.loads(metadata_path.read_text())
    output.mkdir(parents=True)
    deps = output / 'deps'
    deps.mkdir()
    for index, filename, directory, kind in (
        (0, 'godot-cpp.tar.gz', 'godot-cpp', 'tar'),
        (2, 'scons.whl', 'scons', 'zip'),
    ):
        spec = metadata['downloads'][index]
        archive = deps / filename
        download(spec, archive)
        extract(archive, deps / directory, kind)
        print('Verified and extracted ' + directory, flush=True)
    for name in SOURCE_FILES:
        shutil.copy2(SOURCE / name, output / name)
    receipt = {'scope': 'Linux arm64 candidate build; no playback or platform acceptance',
               'compiler': compiler, 'python': platform.python_version(), 'tls_verified': True,
               'dependency_manifest_sha256': digest(metadata_path),
               'downloads': [metadata['downloads'][i] for i in (0, 2)],
               'source_sha256': {name: digest(output / name) for name in SOURCE_FILES}}
    (output / 'build-inputs.json').write_text(json.dumps(receipt, indent=2) + '\n')
    command = [sys.executable, '-m', 'SCons', 'platform=linux', 'arch=arm64',
               'target=template_release', 'build_profile=build-profile.json',
               'disable_exceptions=no', '-j' + str(args.jobs)]
    with (output / 'build.log').open('w') as log:
        result = subprocess.run(command, cwd=output,
                                env=dict(os.environ, PYTHONPATH=str(deps / 'scons')),
                                stdout=log, stderr=subprocess.STDOUT)
    library = output / 'project/bin/liboriginal_audio.linux.template_release.arm64.so'
    receipt['exit_code'] = result.returncode
    if result.returncode == 0:
        receipt['library_sha256'] = digest(library)
    (output / 'build-result.json').write_text(json.dumps(receipt, indent=2) + '\n')
    if result.returncode:
        raise ValueError('native build failed; inspect ' + str(output / 'build.log'))
    print('Candidate library: ' + str(library.relative_to(ROOT)))
    print('Not installed. Review and test the candidate before publication.')
    return 0


if __name__ == '__main__':
    try:
        raise SystemExit(main())
    except (OSError, ValueError, subprocess.SubprocessError, tarfile.TarError, zipfile.BadZipFile) as error:
        print('Native audio build failed: ' + str(error), file=sys.stderr)
        raise SystemExit(2)
