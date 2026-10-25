"""Verify the original symbolic text-buffer contract used by ``talk_render``.

This reader proves source branches, matching instructions, and initial storage.
It does not infer runtime buffer values, write execution, or effective read lengths.
"""
from __future__ import annotations

import hashlib
import re
from pathlib import Path

from .text import source_path
from .text_binary import _spans
from .text_source import _function_code_extent

_SOURCE_ROOT = "src/main/8007C030.c"
_ASM_PATH = "asm/matchings/main/8007C030/talk_render.s"
_DATA_PATH = "asm/data/main/8007C030.data.s"
_EXECUTABLE = "SLPM_86.053"
_MAIN_BASE = 0x80041000
_MAIN_HEADER = 0x800
_EXPECTED = {"S": "D_800CA23C", "O": "D_800CA248", "W": "D_800CA250",
             "A": "D_800CA25C", "M": "D_800CA280"}
_BUFFERS = {
    "D_800CA23C": (0x800CA23C, 0x89A3C, 12),
    "D_800CA248": (0x800CA248, 0x89A48, 8),
    "D_800CA250": (0x800CA250, 0x89A50, 12),
    "D_800CA25C": (0x800CA25C, 0x89A5C, 36),
    "D_800CA280": (0x800CA280, 0x89A80, 36),
}
_PINS = {
    _SOURCE_ROOT: "59d58351fcf8ef55aee74518801ab8a090593e080b725fc5809007c7c8a6a865",
    _ASM_PATH: "9a94866bad9df66ad465e28dccde6c40f039450ab79aafaddc03caf2022d4619",
    _DATA_PATH: "9a2ed8fd32dcabb30aaf19f50ddd5c0de7d3c3fd39a4c36b8bb13c2fdc934b36",
    _EXECUTABLE: "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae",
}
_CONDITION = re.compile(r"if\s*\(\s*line\s*\[\s*out_index\s*\]\s*==\s*'([SOWAM])'\s*\)\s*\{")
_NEXT_BRANCH = re.compile(r"\belse\s+if\s*\(\s*line\s*\[\s*out_index\s*\]")
_SYMBOL = re.compile(r"\bD_800CA(?:23C|248|250|25C|280)\b")


class SubstitutionProofError(ValueError):
    """Raised when the pinned source/image evidence is absent or inconsistent."""


def _safe_file(root: Path, relative: str) -> Path:
    source_path(relative)
    rel = Path(relative)
    if rel.is_absolute() or ".." in rel.parts:
        raise SubstitutionProofError("evidence path is not repository-relative")
    path = (root / rel).resolve(strict=True)
    if not path.is_relative_to(root) or not path.is_file():
        raise SubstitutionProofError("evidence file escapes its selected root")
    return path


def _marker_symbol_pairs(source: str) -> list[tuple[str, str]]:
    """Read marker branches and their first buffer reference without C execution."""
    pairs = []
    starts = list(_CONDITION.finditer(source))
    for index, match in enumerate(starts):
        end = starts[index + 1].start() if index + 1 < len(starts) else len(source)
        branch = source[match.end():end]
        next_branch = _NEXT_BRANCH.search(branch)
        if next_branch:
            branch = branch[:next_branch.start()]
        symbols = _SYMBOL.findall(branch)
        if len(set(symbols)) != 1:
            raise SubstitutionProofError("marker branch lacks one unambiguous buffer symbol")
        pairs.append((match.group(1), symbols[0]))
    return pairs


def verify_symbolic_substitution_contract(repository, files_root) -> dict:
    """Verify marker mapping, original code bytes, and initial buffer storage.

    Returned metadata deliberately marks runtime contents and read lengths
    unknown. No game text or runtime values are returned.
    """
    repo = Path(repository).resolve(strict=True)
    inputs = Path(files_root).resolve(strict=True)
    if not repo.is_dir() or not inputs.is_dir():
        raise SubstitutionProofError("repository and original-input roots must be directories")
    pinned = {}
    for relative, expected in _PINS.items():
        root = inputs if relative == _EXECUTABLE else repo
        path = _safe_file(root, relative)
        raw = path.read_bytes()
        digest = hashlib.sha256(raw).hexdigest()
        if digest != expected:
            raise SubstitutionProofError("pinned source or original executable changed")
        pinned[relative] = digest

    source_path_obj = _safe_file(repo, _SOURCE_ROOT)
    source_bytes = source_path_obj.read_bytes()
    source = source_bytes.decode("utf-8", errors="strict")
    if source.encode("utf-8") != source_bytes or "\ufffd" in source:
        raise SubstitutionProofError("source C file is not strict UTF-8 round-trip text")
    pairs = _marker_symbol_pairs(source)
    observed = {marker: set() for marker in _EXPECTED}
    for marker, symbol in pairs:
        observed[marker].add(symbol)
    if observed != {marker: {symbol} for marker, symbol in _EXPECTED.items()}:
        raise SubstitutionProofError("source marker branches disagree with the pinned mapping")
    if any(sum(marker == item[0] for item in pairs) != 2 for marker in _EXPECTED):
        raise SubstitutionProofError("source marker branch count differs from pinned contract")
    if "set_kanji_string(" not in source:
        raise SubstitutionProofError("source renderer handoff is missing")

    executable = _safe_file(inputs, _EXECUTABLE).read_bytes()
    asm_path = _safe_file(repo, _ASM_PATH)
    code = _function_code_extent(asm_path, executable, _MAIN_BASE, _MAIN_HEADER)
    if code.get("status") != "verified_instruction_bytes" or code.get("instruction_words") != 1397 or code.get("verified_instruction_words") != 1397:
        raise SubstitutionProofError("talk_render instruction proof differs from pinned original")

    data_path = _safe_file(repo, _DATA_PATH)
    binary = (executable, _MAIN_BASE, _MAIN_HEADER, _PINS[_EXECUTABLE], _EXECUTABLE)
    spans = list(_spans(data_path, repo, binary))
    initial = []
    for symbol, (address, offset, length) in _BUFFERS.items():
        matches = [span for span in spans if span["label"] == symbol and span["address"] == address]
        if len(matches) != 1:
            raise SubstitutionProofError("buffer has no unique source-backed data extent")
        span = matches[0]
        raw = bytes(span["raw"])
        actual_offset = address - _MAIN_BASE + _MAIN_HEADER
        if actual_offset != offset or len(raw) != length or any(raw):
            raise SubstitutionProofError("buffer extent disagrees with original initial storage")
        initial.append({"symbol": symbol, "address": address, "file_offset": offset,
                        "initial_bytes": length, "initial_storage": "zero_initialized"})
    if sum(item["initial_bytes"] for item in initial) != 104:
        raise SubstitutionProofError("initial buffer storage size differs from pinned contract")

    return {
        "schema": "tokimemo.symbolic-substitutions.v1",
        "status": "verified_source_and_original_image_pins",
        "source_pins": pinned,
        "marker_branches": [{"marker": marker, "buffer_symbol": symbol}
                            for marker, symbol in pairs],
        "renderer_handoff": "set_kanji_string",
        "code_proof": {"assembly_path": _ASM_PATH,
                       "instruction_words": code["instruction_words"],
                       "verified_instruction_words": code["verified_instruction_words"],
                       "extent_sha256": code["sha256"]},
        "buffers": initial,
        "runtime_contents": "unknown",
        "runtime_read_length_proven": False,
        "execution_claim": False,
        "capacity_is_translation_limit": False,
    }
