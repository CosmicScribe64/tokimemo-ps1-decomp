"""English art for the disc build (T-39508): see tools/patch/art/build.py and wiki/english-build.md."""


class ArtBuildError(RuntimeError):
    """An art container or the executable is not what the art step needs; the message says why."""
