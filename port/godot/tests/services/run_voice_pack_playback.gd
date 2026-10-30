extends "res://tests/services/settings_test_tree.gd"
## External original XA authentication plus a generated, unshipped test tone.
const Audio = preload("res://services/audio_service.gd")
const Voice = preload("res://services/voice_pack_store.gd")
const Playback = preload("res://services/original_voice_playback.gd")
func _initialize() -> void: call_deferred("run")
func run() -> void:
	var pack := OS.get_environment("SETTINGS_ORIGINAL_PACK").path_join("packs/opening_audio")
	var audio := Audio.new()
	check(audio.configure_original_opening_audio_pack(pack,FileAccess.get_sha256(pack.path_join("opening-audio.json"))).is_empty())
	var prepared := audio.prepare_original_line(5)
	check(prepared.error.is_empty() and not prepared.clips.is_empty())
	if failures > 0: finish(); return
	var event: Dictionary = prepared.clips[0].duplicate(true); event.line_id = 5; event.clip_index = 0
	var original := audio.resolve_original_audio_event(event)
	check(original.error.is_empty())
	var folder := "user://voice-playback-test"; DirAccess.make_dir_recursive_absolute(ProjectSettings.globalize_path(folder))
	var wave := AudioStreamWAV.new(); wave.mix_rate = 22050; wave.format = AudioStreamWAV.FORMAT_16_BITS
	var samples := PackedByteArray(); samples.resize(22050)
	for index: int in 11025: samples.encode_s16(index*2,int(sin(index*TAU*440.0/22050.0)*5000))
	wave.data = samples; check(wave.save_to_wav(folder.path_join("test.wav")) == OK)
	var clip: Dictionary = event.xa_request.duplicate(true)
	clip.merge({"line_id":5,"clip_index":0,"speaker":event.speaker,"path":"test.wav","sha256":FileAccess.get_sha256(folder.path_join("test.wav"))},true)
	var path := folder.path_join("voice.json")
	var file := FileAccess.open(path,FileAccess.WRITE); file.store_string(JSON.stringify({"schema":Voice.SCHEMA,"language":"test","clips":[clip]})); file.close()
	var custom := Voice.new(); check(custom.open_pack(path,FileAccess.get_sha256(path)))
	var player := AudioStreamPlayer.new(); player.bus = "Voice"; root.add_child(player)
	var playback := Playback.new(); playback.attach(player); playback.custom_voice_pack = custom
	var record := AudioEffectRecord.new(); AudioServer.add_bus_effect(0,record)
	await create_timer(2.0).timeout
	record.set_recording_active(true)
	var started := Time.get_ticks_msec()
	check(playback.play_event(audio,event).is_empty()); check(player.stream.data == samples)
	var active := player.stream
	await create_timer(0.12).timeout
	playback.custom_voice_pack = null
	check(player.stream == active and not playback.is_voice_complete())
	while not playback.is_voice_complete() and Time.get_ticks_msec()-started < 2000:
		await process_frame
	var elapsed := Time.get_ticks_msec()-started
	print("Voice timing observation: ",elapsed,"ms complete=",playback.is_voice_complete()," position=",player.get_playback_position()," length=",active.get_length()," driver=",AudioServer.get_driver_name())
	check(playback.is_voice_complete() and elapsed >= 350 and elapsed < 1500)
	record.set_recording_active(false)
	var recording := record.get_recording()
	check(recording != null and not recording.data.is_empty())
	var nonzero := false
	if recording != null:
		for byte: int in recording.data:
			if byte != 0: nonzero = true; break
	check(nonzero)
	check(playback.play_event(audio,event).is_empty()); check(player.stream == original.stream)
	await create_timer(0.05).timeout
	playback.stop_clip(); check(playback.is_voice_complete())
	playback.custom_voice_pack = custom
	var invalid := event.duplicate(true); invalid.xa_request.start_lba += 1
	check(not playback.play_event(audio,invalid).is_empty() and playback.is_voice_complete())
	playback.detach(); player.stream = null; player.queue_free()
	AudioServer.remove_bus_effect(0,AudioServer.get_bus_effect_count(0)-1)
	await create_timer(0.1).timeout
	print("Voice pack actual mixer: authenticated original event, 500ms custom clip completed in %dms, non-silent output, active-clip preservation, next-event Japanese fallback and cancellation" % elapsed)
	finish()
