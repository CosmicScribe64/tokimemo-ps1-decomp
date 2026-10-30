extends RefCounted
## T-39008 numerical-model identity, not a hardware or Redux parity assertion.
const ID := "tokimemo-mdec-fixed-point-v1"
const IMPLEMENTATIONS := {"image_mdec.py": "6513b1e6434bdd1c65c804e5d637ae3db1f4fa11f69dd80b495cf05a2f8828ec", "image_mdec_model.py": "3b8e5611bf6f763171b8bf3d5a22269211048b0ec21dc342088e90ec043d2222"}
static func valid(metadata: Variant) -> bool:
	if not metadata is Dictionary or not metadata.get("decoder_model") is Dictionary: return false
	var model: Dictionary = metadata.decoder_model
	if model.get("id") != ID or model.get("implementation_sha256") != IMPLEMENTATIONS: return false
	for flag: String in ["hardware_pixel_parity_verified", "redux_aan_pixel_parity_verified", "game_runlevel_transport_independently_verified"]:
		if not model.get(flag) is bool or model[flag]: return false
	return true
