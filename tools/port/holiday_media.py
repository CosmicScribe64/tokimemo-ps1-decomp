"""Add the finite original April 9 holiday media set to a staged route pack."""
from tools.assets.holiday_content_profile import ASSETS, NATIVE_ROUTE, build_holiday_profile, route_tables
from tools.assets import media_mdec_export
from tools.port.media_components import export_sd_component, export_background_component


def export_holiday_components(source, entries, exe, root, stage, copy_png):
    profile = build_holiday_profile(source, entries, root)
    by_path = {entry["path"]: entry for entry in entries}
    result = {}
    for asset in ASSETS:
        entry = by_path[asset["path"]]
        data = source.read(entry)
        if asset["path"].endswith(".BS"):
            rec = next(x for x in profile["assets"] if x["path"] == asset["path"])
            context = {"width": 320, "height": 88, "padded_height": 96,
                       "source_profile": profile["source_profile"], "geometry": profile["geometry"],
                       "evidence": rec["source_evidence"]}
            component = export_background_component(data, stage, entry, asset["id"], context, NATIVE_ROUTE, copy_png)
            component["native_read_extent"] = media_mdec_export._native_cd_read_extent(source, entry, source.describe()["source_sha256"], 10)
            key = "holiday_room_background" if asset["role"] == "seasonal_room" else "holiday_command_0_background"
        else:
            component, caller = export_sd_component(data, exe, stage, entry, asset["id"], "holiday", 1, NATIVE_ROUTE, copy_png)
            component["caller_profile"] = caller
            component["source_profile"] = profile["source_profile"]
            key = "holiday_command_0_sd"
        result[key] = component
    return result, route_tables(exe)
