"""Verify the original symbolic text-buffer contract used by ``talk_render``.

This reader proves source branches, matching instructions, and initial storage.
It does not infer runtime buffer values, write execution, or effective read lengths.
"""
from __future__ import annotations

import hashlib
import re
from pathlib import Path

from .text import source_path
from .text_source import _function_code_extent

_SOURCE_ROOT = "src/main/scene_library.c"
_ASM_PATH = "asm/matchings/main/scene_library/talk_render.s"
_DATA_PATHS = tuple(f"asm/data/main/scene_library.data/name_code_{suffix}.s"
                    for suffix in ("s", "o", "w", "a", "m"))
_EXECUTABLE = "SLPM_86.053"
_SYMBOL_ADDRESSES = "config/symbol_addrs.txt"
_MAIN_BASE = 0x80041000
_MAIN_HEADER = 0x800
_EXPECTED = {"S": "name_code_s", "O": "name_code_o", "W": "name_code_w",
             "A": "name_code_a", "M": "name_code_m"}
_BUFFERS = {
    "name_code_s": (0x800CA23C, 0x89A3C, 12),
    "name_code_o": (0x800CA248, 0x89A48, 8),
    "name_code_w": (0x800CA250, 0x89A50, 12),
    "name_code_a": (0x800CA25C, 0x89A5C, 36),
    "name_code_m": (0x800CA280, 0x89A80, 36),
}
_PINS = {
    _SOURCE_ROOT: "c8a0315a4d4542dc13c8e1788ad199fafd28d5104bce64f08bb3d3b63e4e1a18",
    _ASM_PATH: "0cb20df07b112a058e09061c87f720b376a3a5206f8c239b067da2ef9de01ada",
    _SYMBOL_ADDRESSES: "003c6169775031a58015faebba3d8674754a5d3ed166a9b06f9528055cbc3bfe",
    _DATA_PATHS[0]: "3064f7f6826bee50de5f874af1dac777de6072d53f29f5f4fc4974d99dde1d32",
    _DATA_PATHS[1]: "7d50f8c2b65c8938d52b9d12a8c3d46b5b5650e15602be08d1c461d6bf93e435",
    _DATA_PATHS[2]: "445b31453106a3ad82096d997ce7551f47b98d3405bb7fe308c86fcbef0a373e",
    _DATA_PATHS[3]: "4c1c6c4e5e3e422ea46a918c1e9de54a5958d96f56cc5b7659a6009b821e3e7c",
    _DATA_PATHS[4]: "1f64d5c673e5f7c46ec3be33164bb077424707ce6fa0378448a978ccf9d9ec0c",
    _EXECUTABLE: "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae",
}
_CONDITION = re.compile(r"if\s*\(\s*line\s*\[\s*out_index\s*\]\s*==\s*'([SOWAM])'\s*\)\s*\{")
_NEXT_BRANCH = re.compile(r"\belse\s+if\s*\(\s*line\s*\[\s*out_index\s*\]")
_SYMBOL = re.compile(r"\bname_code_(?:s|o|w|a|m)\b")


class SubstitutionProofError(ValueError):
    """Raised when the pinned source/image evidence is absent or inconsistent."""


def _zero_initialized_bytes(path: Path, symbol: str, expected_size: int) -> bytes:
    """Read a complete explicitly initialized .byte label without guessing gaps."""
    raw = path.read_bytes()
    source = raw.decode("utf-8", errors="strict")
    if source.encode("utf-8") != raw or "\ufffd" in source:
        raise SubstitutionProofError("data assembly is not strict UTF-8 round-trip text")
    starts = [i for i, line in enumerate(source.splitlines())
              if re.fullmatch(r"\s*dlabel\s+" + re.escape(symbol) + r"\s*", line)]
    if len(starts) != 1:
        raise SubstitutionProofError("buffer data label is missing or ambiguous")
    lines = source.splitlines()[starts[0] + 1:]
    values = bytearray()
    ended = False
    for line in lines:
        line = re.sub(r"/\*.*?\*/|#.*$", "", line).strip()
        if not line:
            continue
        next_label = re.fullmatch(r"dlabel\s+(\S+)", line)
        if next_label:
            if next_label[1] == symbol:
                raise SubstitutionProofError("buffer data label is duplicated")
            ended = True
            break
        end = re.fullmatch(r"enddlabel\s+(\S+)", line)
        if end:
            if end[1] != symbol:
                raise SubstitutionProofError("buffer label terminator names a different extent")
            ended = True
            break
        directive = re.fullmatch(r"\.byte\s+(.+)", line)
        if not directive:
            raise SubstitutionProofError("buffer label contains an unsupported or uninitialized directive")
        for item in directive[1].split(","):
            try:
                value = int(item.strip(), 0)
            except ValueError as exc:
                raise SubstitutionProofError("buffer byte initializer is not numeric") from exc
            if not 0 <= value <= 0xFF:
                raise SubstitutionProofError("buffer byte initializer is out of range")
            values.append(value)
            if len(values) > expected_size:
                raise SubstitutionProofError("buffer byte extent exceeds its source declaration")
    # The current symbol assembly emits these labels as a terminal file with no
    # explicit enddlabel; EOF is therefore the exact source boundary.
    if not ended and len(values) == expected_size:
        ended = True
    if not ended or len(values) != expected_size:
        raise SubstitutionProofError("buffer data extent differs from its source declaration")
    return bytes(values)


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

    symbols_raw = _safe_file(repo, _SYMBOL_ADDRESSES).read_bytes()
    symbols_text = symbols_raw.decode("utf-8", errors="strict")
    symbols = {name: int(address, 16) for name, address in re.findall(
        r"(?m)^(name_code_[sowam])\s*=\s*0x([0-9A-Fa-f]+);", symbols_text)}
    if symbols != {name: value[0] for name, value in _BUFFERS.items()}:
        raise SubstitutionProofError("buffer addresses disagree with the pinned symbol map")

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

    data_files = {relative: _safe_file(repo, relative) for relative in _DATA_PATHS}
    initial = []
    for symbol, (address, offset, length) in _BUFFERS.items():
        matches = [path for path in data_files.values()
                   if re.search(r"(?m)^\s*dlabel\s+" + re.escape(symbol) + r"\s*$", path.read_text(encoding="utf-8"))]
        if len(matches) != 1:
            raise SubstitutionProofError("buffer has no unique source-backed data extent")
        raw = _zero_initialized_bytes(matches[0], symbol, length)
        actual_offset = address - _MAIN_BASE + _MAIN_HEADER
        original = executable[actual_offset:actual_offset + length]
        if actual_offset != offset or len(original) != length or original != raw or any(raw):
            raise SubstitutionProofError("buffer extent disagrees with original initial storage")
        initial.append({"physical_id": f"D_{address:08X}", "symbol": symbol,
                        "address": address, "file_offset": offset,
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
        "data_assembly_paths": list(_DATA_PATHS),
        "buffers": initial,
        "runtime_contents": "unknown",
        "runtime_read_length_proven": False,
        "execution_claim": False,
        "capacity_is_translation_limit": False,
    }
