extends "res://tests/services/settings_test_tree.gd"
const TextPack = preload("res://services/text_translation_store.gd")
const VoicePack = preload("res://services/voice_pack_store.gd")
const Playback = preload("res://services/original_voice_playback.gd")
class Resolver extends RefCounted:
	var original: AudioStreamWAV
	var rejected := false
	func resolve_original_audio_event(_event: Dictionary) -> Dictionary:
		return {"error": "unauthorized"} if rejected else {"error": "", "stream": original}
func write_json(path: String, value: Dictionary) -> void:
	var file := FileAccess.open(path, FileAccess.WRITE); file.store_string(JSON.stringify(value)); file.close()
func _initialize() -> void: call_deferred("run")
func run() -> void:
	var folder := "user://language-tests"
	DirAccess.make_dir_recursive_absolute(ProjectSettings.globalize_path(folder))
	var source := "a".repeat(64)
	var text_path := folder.path_join("text.json")
	write_json(text_path, {"schema": TextPack.SCHEMA, "language": "en", "source_payload_sha256": source, "entries": {"0": {"speaker": "Player", "text": "A test translation."}}})
	var translated := TextPack.new()
	check(translated.open_pack(text_path, FileAccess.get_sha256(text_path)))
	check(translated.replacement(source, 0).text == "A test translation.")
	check(translated.replacement(source, 1).is_empty()); check(translated.replacement("b".repeat(64), 0).is_empty())
	check(not translated.open_pack(text_path, "b".repeat(64))); check(translated.language == "en")
	var stream := AudioStreamWAV.new(); stream.format = AudioStreamWAV.FORMAT_16_BITS; stream.mix_rate = 22050
	var samples := PackedByteArray(); samples.resize(4410)
	for index: int in 2205: samples.encode_s16(index * 2, int(sin(index * TAU * 440 / 22050.0) * 5000))
	stream.data = samples
	var wave_path := folder.path_join("voice.wav"); check(stream.save_to_wav(wave_path) == OK)
	var manifest_path := folder.path_join("voice.json")
	var clip := {"line_id": 0, "clip_index": 0, "speaker": 1, "source_sha256": source, "start_lba": 10, "end_lba": 20, "file_number": 1, "channel": 0, "path": "voice.wav", "sha256": FileAccess.get_sha256(wave_path)}
	write_json(manifest_path, {"schema": VoicePack.SCHEMA, "language": "en", "clips": [clip]})
	var voice := VoicePack.new(); check(voice.open_pack(manifest_path, FileAccess.get_sha256(manifest_path)))
	var event := {"line_id": 0, "clip_index": 0, "speaker": 1, "xa_request": {"source_sha256": source, "start_lba": 10, "end_lba": 20, "file_number": 1, "channel": 0}}
	check(voice.replacement(event) != null)
	var player := AudioStreamPlayer.new(); root.add_child(player)
	var playback := Playback.new(); playback.attach(player); playback.custom_voice_pack = voice
	var resolver := Resolver.new(); resolver.original = AudioStreamWAV.new(); resolver.original.data = samples; resolver.original.format = stream.format
	check(playback.play_event(resolver, event).is_empty()); check(player.stream != resolver.original); check(player.stream.data == samples)
	resolver.rejected = true; check(playback.play_event(resolver, event) == "unauthorized")
	resolver.rejected = false; event.clip_index = 1
	check(playback.play_event(resolver, event).is_empty()); check(player.stream == resolver.original)
	playback.stop_clip(); check(playback.is_voice_complete())
	var runtime := root.get_node("ModernSettings")
	check(runtime.select_text_pack(text_path).is_empty()); check(runtime.select_voice_pack(manifest_path).is_empty())
	check(runtime.select_image_pack("").is_empty()); check(runtime.text_pack.language == "en" and runtime.voice_pack.language == "en")
	check(runtime.select_voice_pack("").is_empty()); check(runtime.text_pack.language == "en" and runtime.voice_pack.language == "ja")
	check(runtime.select_text_pack("").is_empty()); check(runtime.text_pack.language == "ja")
	clip.path = "../escape.wav"; write_json(manifest_path, {"schema": VoicePack.SCHEMA, "language": "en", "clips": [clip]})
	check(not voice.open_pack(manifest_path, FileAccess.get_sha256(manifest_path)))
	check(voice.language == "en")
	playback.detach(); player.queue_free()
	await create_timer(0.1).timeout
	print("Language packs: authenticated selection, source identities, hot-switch independence, missing-entry Japanese fallback, playback override and traversal rejection passed")
	finish()
