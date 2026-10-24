#!/usr/bin/env python3
"""Render a self-contained browseable HTML gallery for exported media images."""
from __future__ import annotations

import argparse
import hashlib
import html
import json
import os
from pathlib import Path, PurePosixPath
import sys
from string import Template
from urllib.parse import quote
from typing import Any

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from assets.manifest import REPOSITORY, digest, output_root, write_bytes  # noqa: E402
from assets.visual_text import _stable_region_id  # noqa: E402

VISUAL_SCHEMA = "tokimemo.visual-text.v1"


class GalleryError(ValueError):
    """Manifest metadata or a linked local asset failed gallery validation."""


def _esc(value: Any) -> str:
    if value is None:
        return ""
    if isinstance(value, (dict, list)):
        value = json.dumps(value, ensure_ascii=True, sort_keys=True)
    return html.escape(str(value), quote=True)


def _validate_manifest(manifest: dict) -> list[dict]:
    if not isinstance(manifest, dict) or manifest.get("schema") != "tokimemo.media.v1":
        raise GalleryError("unsupported media manifest schema")
    assets = manifest.get("assets")
    if not isinstance(assets, list):
        raise GalleryError("media manifest assets must be a list")
    return assets


def _output_map(asset: dict) -> dict[str, dict]:
    outputs = asset.get("outputs") or []
    if not isinstance(outputs, list):
        raise GalleryError("asset outputs must be a list")
    result = {}
    for output in outputs:
        if not isinstance(output, dict):
            raise GalleryError("output record must be an object")
        path, sha = output.get("path"), output.get("sha256")
        if not isinstance(path, str) or not path or not isinstance(sha, str) or len(sha) != 64:
            raise GalleryError("output path and SHA-256 are required")
        name = PurePosixPath(path).name
        if name in result:
            raise GalleryError("duplicate output name in one asset: " + name)
        result[name] = output
    return result


def _region_for(asset: dict, variant: dict, metadata: dict | None = None) -> tuple[str, dict]:
    frame = variant.get("frame_index", variant.get("frame"))
    palette_index = variant.get("palette_index")
    rect_value = variant.get("region") or variant.get("rect") or {}
    metadata = metadata or {}
    if not isinstance(rect_value, dict):
        raise GalleryError("image region must be an object")
    rect = {
        "x": rect_value.get("x", 0),
        "y": rect_value.get("y", 0),
        "width": variant.get("width", rect_value.get("width", metadata.get("width"))),
        "height": variant.get("height", rect_value.get("height", metadata.get("height"))),
    }
    if not all(isinstance(value, int) and not isinstance(value, bool) and value >= 0
               for value in rect.values()) or not rect["width"] or not rect["height"]:
        raise GalleryError("image region needs nonnegative x/y and positive width/height")
    region_id = _stable_region_id(asset["id"], frame, palette_index, rect, variant.get("variant"))
    return region_id, rect


def _candidate_layer(source_path: str, profile: dict, variant: dict) -> str:
    if variant.get("preview_kind") == "source_index_diagnostic":
        return "diagnostic"
    variant_name = str(variant.get("variant", "")).lower()
    scope = str(variant.get("palette_variant_scope", "")).lower()
    preview_kind = str(variant.get("preview_kind", "")).lower()
    if ("source_page" in preview_kind or "source-page" in preview_kind or
            "source_texture_page" in preview_kind or "source-page" in variant_name or
            "source_page" in variant_name):
        return "source-page"
    if "candidate" in variant_name or "candidate" in scope or variant_name.startswith(("hud-", "icon-")):
        return "candidate"
    palette_index = variant.get("palette_index")
    if palette_index is not None and palette_index > 0:
        return "candidate"
    return "base"


def _category(source_path: str, profile: dict) -> str:
    lower = source_path.lower()
    role = str(profile.get("source_role", "")).lower()
    if any(token in lower for token in ("portrait", "bustup", "bust_up", "face")):
        return "portraits"
    if lower.endswith(".bs") or "background" in role:
        return "backgrounds"
    if profile.get("priority") is not None or "ui" in role or "calendar" in role or "title" in role:
        return "ui"
    return "other"


def _catalog_index(catalog: dict | None) -> dict[str, dict]:
    if catalog is None:
        return {}
    if not isinstance(catalog, dict) or catalog.get("schema") != VISUAL_SCHEMA:
        raise GalleryError("unsupported visual-text catalog schema")
    regions = catalog.get("regions")
    if not isinstance(regions, list):
        raise GalleryError("visual-text catalog regions must be a list")
    result = {}
    for region in regions:
        if not isinstance(region, dict) or not isinstance(region.get("region_id"), str):
            raise GalleryError("visual-text catalog region has no region id")
        if region["region_id"] in result:
            raise GalleryError("duplicate visual-text catalog region id")
        result[region["region_id"]] = region
    return result


def _card_html(asset: dict, variant: dict, outputs: dict[str, dict],
               profile: dict, catalog_regions: dict[str, dict], output_directory: Path) -> str:
    source = asset.get("source") or {}
    source_path = source.get("path")
    if not isinstance(source_path, str) or not source_path:
        raise GalleryError("image source path is required")
    png_name = variant.get("png")
    if not isinstance(png_name, str) or png_name not in outputs:
        raise GalleryError("image PNG is missing from the source output hashes")
    png_record = outputs[png_name]
    metadata = (asset.get("export") or {}).get("metadata") or {}
    if not metadata:
        candidates = ((asset.get("inspection") or {}).get("candidates") or [])
        metadata = candidates[0] if candidates else {}
    region_id, rect = _region_for(asset, variant, metadata)
    review = catalog_regions.get(region_id)
    if review is not None:
        reviewed_hash = (review.get("png") or {}).get("sha256")
        if reviewed_hash != png_record["sha256"]:
            raise GalleryError("visual-text review is stale for " + region_id)
        state = review.get("state", "unreviewed")
        context = review.get("screen_context") or {}
        confidence = review.get("confidence")
        work = review.get("replacement_work", ["undecided"])
        profile = review.get("source_profile") or profile
        if not profile and context.get("source_role"):
            profile = {"source_role": context.get("source_role"),
                       "priority": context.get("priority"),
                       "screen_context": context.get("contexts", [])}
    else:
        state, context, confidence, work = "unreviewed", {}, None, ["undecided"]
    if state not in {"unreviewed", "suspected", "confirmed_japanese_text", "confirmed_no_text", "confirmed_other_text"}:
        raise GalleryError("invalid visual-text review state: " + str(state))
    layer = _candidate_layer(source_path, profile, variant)
    category = _category(source_path, profile)
    image_href = _relative_asset_href(png_record["path"], output_directory)
    diagnostic = layer == "diagnostic"
    image_label = "Index diagnostic (grayscale)" if diagnostic else "Color preview"
    image_width, image_height = rect["width"], rect["height"]
    preferred_scale = 4 if image_width <= 64 else 2
    scale = max(1, min(preferred_scale, 320 // image_width, 420 // image_height))
    display_width = image_width * scale
    stp_name = variant.get("stp_png")
    stp_record = outputs.get(stp_name) if stp_name else None
    stp_link = ""
    if stp_record:
        stp_href = _relative_asset_href(stp_record["path"], output_directory)
        stp_link = (' <a class="debug-link" href="%s">STP blend mask (debug)</a>' % _esc(stp_href))
    original = asset.get("original")
    original_link = ""
    if isinstance(original, dict) and isinstance(original.get("path"), str):
        original_link = ' <a href="%s">source original</a>' % _esc(
            _relative_asset_href(original["path"], output_directory))
    frame = variant.get("frame_index", variant.get("frame"))
    variant_name = variant.get("variant", "")
    palette_index = variant.get("palette_index")
    palette_status = variant.get("palette_status")
    palette_note = ""
    if palette_index is not None:
        palette_note = "Stored palette %s; runtime selection unknown." % palette_index
    elif variant.get("palette_variant_scope"):
        palette_note = str(variant["palette_variant_scope"])
    if layer == "candidate":
        palette_note = (palette_note + "; " if palette_note else "") + "Candidate variant; applicability unconfirmed."
    diagnostic_note = ""
    if diagnostic:
        diagnostic_note = ('<p class="diagnostic-warning">PALETTE UNRESOLVED — numeric index diagnostic, not finished art</p>')
    contexts = context.get("contexts") or profile.get("screen_context") or []
    frame_role = context.get("frame_role")
    context_text = ", ".join(str(item) for item in contexts)
    if frame_role:
        context_text = (context_text + "; " if context_text else "") + str(frame_role)
    search = " ".join((source_path, asset["id"], str(frame), str(variant_name),
                       str(state), context_text, category)).lower()
    return f'''<article class="card {layer}" data-layer="{layer}" data-category="{_esc(category)}" data-state="{_esc(state)}" data-search="{_esc(search)}">
  <figure><a href="{_esc(image_href)}"><img loading="lazy" src="{_esc(image_href)}" style="width:{display_width}px;max-width:100%;height:auto" alt="{_esc(image_label)} for {_esc(source_path)} frame {_esc(frame)} variant {_esc(variant_name)}"></a></figure>
  <div class="card-body"><h2>{_esc(source_path)}</h2>
    <dl><dt>Asset</dt><dd><code>{_esc(asset["id"])}</code></dd>
      <dt>Frame</dt><dd>{_esc(frame) if frame is not None else "unknown"}</dd>
      <dt>Variant</dt><dd>{_esc(variant_name) or "default"}</dd>
      <dt>Palette</dt><dd>{_esc(palette_note) or _esc(palette_status) or "not specified"}</dd>
      <dt>Region</dt><dd>{rect["x"]}, {rect["y"]} · {rect["width"]} × {rect["height"]}</dd>
      <dt>Context</dt><dd>{_esc(context_text) or "unknown"}</dd>
      <dt>Text review</dt><dd>{_esc(state)}{(" · confidence " + _esc(confidence)) if confidence is not None else ""}</dd>
      <dt>Replacement work</dt><dd>{_esc(work)}</dd></dl>
    {diagnostic_note}<p class="links"><a href="{_esc(image_href)}">{_esc("Index diagnostic PNG" if diagnostic else "Color PNG")}</a>{stp_link}{original_link}</p>
  </div>
</article>'''


def _relative_asset_href(path: str, output_directory: Path) -> str:
    try:
        resolved = output_root(REPOSITORY / path)
    except (ValueError, OSError) as error:
        raise GalleryError("gallery asset must be beneath .asset-cache: " + path) from error
    relative = os.path.relpath(resolved, output_directory)
    return quote(PurePosixPath(relative.replace(os.sep, "/")).as_posix(), safe="/._-")


def _coverage_html(assets: list[dict]) -> str:
    rows = []
    has_bs = False
    has_portrait = False
    portrait_images = 0
    for asset in assets:
        source = asset.get("source") or {}
        path = source.get("path", "unknown source")
        suffix = Path(path).suffix.lower()
        is_portrait = any(word in str(path).lower() for word in ("portrait", "bustup", "bust_up"))
        has_portrait |= is_portrait
        has_images = bool((asset.get("export") or {}).get("images"))
        if is_portrait and has_images:
            portrait_images += len(asset["export"]["images"])
        if has_images:
            continue
        has_bs |= suffix == ".bs"
        rows.append("<li><code>%s</code> — %s / %s: %s</li>" % (
            _esc(path), _esc(asset.get("status", "unknown")), _esc(asset.get("format", "unknown")),
            _esc(asset.get("reason", "not exported as an image"))))
    if not rows:
        rows.append("<li>No non-image media records were supplied; source coverage is still incomplete.</li>")
    mdec = "MDEC background records are present but not decoded." if has_bs else "MDEC background decoding is not covered by this gallery."
    if portrait_images:
        portrait = "%d portrait preview variants are shown; portrait source coverage remains incomplete." % portrait_images
    elif has_portrait:
        portrait = "Portrait-named sources appear in unsupported coverage above; portrait decode is unresolved."
    else:
        portrait = "Portrait coverage is unresolved. No portrait-specific image decoder/source is identified."
    return ("<p><strong>Source coverage incomplete.</strong> This gallery shows exported previews only and makes no claim that all graphics or text were found.</p>"
            "<ul><li>%s</li><li>%s</li></ul><details><summary>Unexported or unsupported media entries (%d)</summary><ul>%s</ul></details>" %
            (mdec, portrait, len(rows), "".join(rows)))


def build_gallery(media_manifest: dict, catalog: dict | None = None,
                  output_path: str | Path = ".asset-cache/media/gallery/index.html") -> str:
    """Render gallery HTML from manifests only; output files are not read or written."""
    assets = _validate_manifest(media_manifest)
    regions = _catalog_index(catalog)
    output_file = output_root(Path(output_path) if Path(output_path).is_absolute()
                              else REPOSITORY / output_path)
    output_directory = output_file.parent
    cards = []
    for asset in assets:
        export = asset.get("export") or {}
        variants = export.get("images") or []
        if asset.get("status") not in {"exported", "partial"} or not variants:
            continue
        if not isinstance(asset.get("id"), str):
            raise GalleryError("image asset id is required")
        profile = (asset.get("source_profile") or
                   (asset.get("source") or {}).get("profile") or {})
        outputs = _output_map(asset)
        for variant in variants:
            if not isinstance(variant, dict):
                raise GalleryError("image variant must be an object")
            cards.append(_card_html(asset, variant, outputs, profile, regions, output_directory))
    if not cards:
        cards_markup = '<p class="empty">No exported color previews are available in this media manifest.</p>'
    else:
        cards_markup = "\n".join(cards)
    coverage = _coverage_html(assets)
    return _document(cards_markup, coverage, len(cards), media_manifest, catalog)


def _document(cards: str, coverage: str, count: int, media_manifest: dict,
              catalog: dict | None) -> str:
    source_hash = ((media_manifest.get("input") or {}).get("source_sha256") or "unknown")
    catalog_notice = "Visual review catalog attached; unmatched images remain unreviewed." if catalog else "No visual review catalog attached; all images default to unreviewed."
    template = Template('''<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8"><meta name="viewport" content="width=device-width, initial-scale=1">
<title>Tokimeki Memorial media gallery</title>
<style>
:root{color-scheme:light dark;font:15px/1.45 system-ui,sans-serif}body{margin:0;background:#10131a;color:#edf1f7}header{padding:1.3rem 2vw;background:#1b2230;position:sticky;top:0;z-index:3;box-shadow:0 2px 12px #0008}h1{margin:.1rem 0 .4rem;font-size:1.45rem}header p{margin:.25rem 0;color:#c1ccdb}.controls{display:flex;gap:.8rem;flex-wrap:wrap;align-items:center;margin-top:.8rem}.controls input,.controls select{padding:.5rem;border-radius:.35rem;border:1px solid #667;background:#111722;color:inherit}.controls label{display:flex;gap:.35rem;align-items:center}.coverage{margin:1rem 2vw;padding:1rem;background:#222b38;border-radius:.5rem}.gallery{display:grid;grid-template-columns:repeat(auto-fill,minmax(340px,1fr));gap:1rem;padding:1rem 2vw 3rem}.card{background:#202733;border:1px solid #3b4758;border-radius:.5rem;overflow:hidden;min-width:0}.card[hidden]{display:none!important}.card.candidate,.card.diagnostic,.card.source-page{display:none}.show-candidates .card.candidate,.show-diagnostics .card.diagnostic,.show-source-pages .card.source-page{display:block}.card figure{margin:0;padding:1rem;min-height:150px;display:grid;place-items:center;background-color:#fff;background-image:linear-gradient(45deg,#d8dce2 25%,transparent 25%),linear-gradient(-45deg,#d8dce2 25%,transparent 25%),linear-gradient(45deg,transparent 75%,#d8dce2 75%),linear-gradient(-45deg,transparent 75%,#d8dce2 75%);background-size:20px 20px;background-position:0 0,0 10px,10px -10px,-10px 0}.card img{display:block;max-width:100%;max-height:420px;width:auto;height:auto;object-fit:contain;image-rendering:pixelated}.card-body{padding:0 1rem 1rem}.card h2{font-size:.95rem;overflow-wrap:anywhere}.card dl{display:grid;grid-template-columns:7rem 1fr;gap:.2rem .5rem;margin:.6rem 0}.card dt{color:#aebbd0}.card dd{margin:0;overflow-wrap:anywhere}.links{display:flex;gap:.8rem;flex-wrap:wrap}.debug-link{display:none}.debug-on .debug-link{display:inline}.diagnostic-warning{font-weight:800;color:#ffbd5b}.candidate-warning{color:#ffd681}a{color:#8ecbff}code{overflow-wrap:anywhere}.empty{padding:2rem}.coverage details{margin-top:.8rem}.coverage ul{max-height:20rem;overflow:auto}.sr-only{position:absolute;width:1px;height:1px;padding:0;margin:-1px;overflow:hidden;clip:rect(0,0,0,0);white-space:nowrap;border:0}@media(max-width:550px){header{position:static}.gallery{grid-template-columns:1fr}}
</style>
</head>
<body>
<header><h1>Tokimeki Memorial — color media gallery</h1>
<p>${count} exported image variants. ${catalog_notice} Review states describe visual review only; no text is inferred from a filename or palette.</p>
<div class="controls">
<label>Search <input id="search" type="search" placeholder="Source, frame, context"></label>
<label>Category <select id="category"><option value="all">All categories</option><option>ui</option><option>portraits</option><option>backgrounds</option><option>other</option></select></label>
<label>Review state <select id="state"><option value="all">All states</option><option>unreviewed</option><option>suspected</option><option>confirmed_japanese_text</option><option>confirmed_other_text</option><option>confirmed_no_text</option></select></label>
<label><input id="candidates" type="checkbox"> Show candidate palette/frame variants</label>
<label><input id="source-pages" type="checkbox"> Show source texture pages</label>
<label><input id="diagnostics" type="checkbox"> Show numeric palette diagnostics</label>
<label><input id="debug" type="checkbox"> Show STP debug-mask links</label>
</div></header>
<section class="coverage"><h2>Coverage and limitations</h2>${coverage}<p>Input SHA-256: <code>${source_hash}</code>. Original and STP outputs are linked separately. All artwork remains local; nothing is generated or replaced.</p></section>
<main id="gallery" class="gallery">${cards}</main>
<script>
(() => {
 const body=document.body, search=document.getElementById('search'), category=document.getElementById('category'), state=document.getElementById('state');
 const update=()=>{const q=search.value.trim().toLowerCase();for(const card of document.querySelectorAll('.card')){card.hidden=!(card.dataset.search.includes(q)&&(category.value==='all'||card.dataset.category===category.value)&&(state.value==='all'||card.dataset.state===state.value));}};
 search.addEventListener('input',update);category.addEventListener('change',update);state.addEventListener('change',update);
 document.getElementById('candidates').addEventListener('change',e=>{body.classList.toggle('show-candidates',e.target.checked)});
 document.getElementById('source-pages').addEventListener('change',e=>{body.classList.toggle('show-source-pages',e.target.checked)});
 document.getElementById('diagnostics').addEventListener('change',e=>{body.classList.toggle('show-diagnostics',e.target.checked)});
 document.getElementById('debug').addEventListener('change',e=>{body.classList.toggle('debug-on',e.target.checked)});
})();
</script>
</body></html>''')
    return template.substitute({
        "count": str(count), "catalog_notice": _esc(catalog_notice), "coverage": coverage,
        "source_hash": _esc(source_hash), "cards": cards,
    })


def _verify_artifacts(media_manifest: dict, catalog: dict | None) -> int:
    """Verify every referenced preview/mask/original under the ignored cache."""
    verified = 0
    def check(item: dict, label: str) -> None:
        nonlocal verified
        if not isinstance(item, dict) or not isinstance(item.get("path"), str):
            raise GalleryError("invalid %s artifact reference" % label)
        expected = item.get("sha256")
        if not isinstance(expected, str) or len(expected) != 64:
            raise GalleryError("%s artifact SHA-256 is required" % label)
        path = output_root(REPOSITORY / item["path"])
        if not path.is_file():
            raise GalleryError("missing %s artifact: %s" % (label, item["path"]))
        actual = hashlib.sha256(path.read_bytes()).hexdigest()
        if actual != expected:
            raise GalleryError("stale %s artifact hash: %s" % (label, item["path"]))
        verified += 1
    for asset in _validate_manifest(media_manifest):
        export = asset.get("export") or {}
        variants = export.get("images") or []
        if not variants:
            continue
        outputs = _output_map(asset)
        for variant in variants:
            for name in (variant.get("png"), variant.get("stp_png")):
                if name:
                    if name not in outputs:
                        raise GalleryError("image output hash missing for " + name)
                    check(outputs[name], "image")
        for field in ("original", "paired_original"):
            if asset.get(field):
                check(asset[field], field)
    if catalog:
        for region in _catalog_index(catalog).values():
            if region.get("png"):
                check(region["png"], "catalog PNG")
            if region.get("stp"):
                check(region["stp"], "catalog STP")
    return verified


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--media-manifest", required=True)
    parser.add_argument("--visual-catalog")
    parser.add_argument("--output", default=".asset-cache/media/gallery/index.html")
    args = parser.parse_args(argv)
    try:
        media = json.loads(Path(args.media_manifest).read_text(encoding="utf-8"))
        catalog = json.loads(Path(args.visual_catalog).read_text(encoding="utf-8")) if args.visual_catalog else None
        html_text = build_gallery(media, catalog, args.output)
        verified = _verify_artifacts(media, catalog)
        output = write_bytes(args.output, html_text.encode("utf-8"))
        print("gallery: %s (%d image variants; %d linked files hash-verified; sha256 %s)" %
              (output["path"], html_text.count('<article class="card'), verified, output["sha256"]))
        return 0
    except (OSError, json.JSONDecodeError, GalleryError, ValueError) as error:
        print("media_gallery.py: %s" % error, file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
