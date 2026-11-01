"""Text-container writers for the English patch (T-39503).

API (all pure functions over the original files; nothing here writes disc/):

* ``containers.build_containers(disc_root, units, supplemental)`` -> ``{path: Container}``.
  ``units`` are the rows of ``.asset-cache/text/units/units.jsonl``
  (``tools.assets.text_export`` then ``tools.assets.text_units``); ``supplemental`` the
  rows of ``translations/en/supplemental.jsonl``.
* ``Container.image.write({record id: payload bytes})`` -> ``image.WriteResult``
  (new bytes, placements, overflows). Payloads exclude the terminator.
* ``segments.source_segments(unit)`` / ``translations.target_of(row, source_segments)``
  give ``Text``/``Control`` lists; ``segments.encode(segments, encoder)`` makes payloads.
  Encoders: ``ShiftJisEncoder`` (original), ``ProvisionalAsciiEncoder`` (size model);
  any object with ``encode_text`` and ``encode_control`` works.
* ``c_literals.locate_sites`` / ``rewrite_sources`` write ``source_literal`` records into src/.
* ``coverage`` and ``sizes`` build the coverage map, gap scan and size report.

CLI: ``python3 -m tools.patch_text`` (see that module).
"""
