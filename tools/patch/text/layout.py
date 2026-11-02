"""English payloads of every record: encode, then wrap for the record's window (T-39507).

``english_payloads(containers, targets, metrics)`` encodes the English target of every owner and
source-only record with ``EnglishEncoder`` and wraps it with ``wrap.wrap`` when ``window_for``
knows its window. It returns the payloads by container path (the input of ``Image.write`` and of
``c_literals.rewrite_sources``) and one note per record for the build report: the window and why,
rows, breaks added, rows still too wide, characters kept as Shift-JIS or not mappable, and the
reason when the original payload is kept.

SPEAKTST files are out of scope for the first release (T-39500) and get no payloads.
"""
from __future__ import annotations

from collections import Counter
from dataclasses import dataclass, field

from .containers import Container
from .english import EnglishEncoder
from .segments import Control, encode
from .translations import target_of
from .windows import window_for
from .wrap import FontMetrics, wrap

SKIPPED_KINDS = ("speaktst", "reference_only")


@dataclass
class EnglishText:
    payloads: dict = field(default_factory=dict)
    notes: dict = field(default_factory=dict)
    encoder: EnglishEncoder | None = None

    def counts(self) -> Counter:
        return Counter(note["status"] for note in self.notes.values())


def english_payloads(containers: dict[str, Container], targets: dict[str, dict], metrics: FontMetrics,
                     encoder: EnglishEncoder | None = None) -> EnglishText:
    encoder = encoder or EnglishEncoder()
    out = EnglishText(encoder=encoder)
    for path, container in sorted(containers.items()):
        if container.kind in SKIPPED_KINDS:
            continue
        for record in container.records.values():
            if record.role not in ("owner", "source_only"):
                continue
            note = {"path": path, "kind": record.kind, "container": container.kind}
            out.notes[record.id] = note
            row = targets.get(record.id)
            if row is None:
                note["status"] = "kept_original"
                note["reason"] = "no_translation_row"
                continue
            if any(isinstance(s, Control) and s.kind == "opaque" for s in record.segments):
                note["status"], note["reason"] = "kept_original", "opaque_source_bytes"
                continue
            segments, problem = target_of(row, record.segments)
            if problem:
                note["problem"] = problem
            if segments is None:
                note["status"], note["reason"] = "kept_original", problem
                continue
            encoder.take_record_notes()
            payload = encode(segments, encoder)
            note.update(encoder.take_record_notes())
            window, basis = window_for(container.kind, record)
            note["status"] = "placed"
            if window is not None:
                result = wrap(payload, window, metrics)
                payload = result.payload
                note.update(window=window.name, basis=basis, rows=result.rows,
                            breaks_added=result.breaks_added)
                if result.long_rows:
                    note["rows_over_row_buffer"] = result.long_rows
                if result.overflow_rows or result.too_many_lines:
                    note["status"] = "unwrappable"
                    note["overflow_px"] = [px - window.width for px in result.overflow_rows]
                    note["too_many_lines"] = result.too_many_lines
            else:
                note["width_px"] = metrics.width(payload)
            out.payloads.setdefault(path, {})[record.id] = payload
    return out
