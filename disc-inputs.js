/* T-39527: resolve complete raw disc tracks before the worker verifies their hashes. */
'use strict';
const DiscInputs = (() => {
    function pathOf(file) {
        return (file.relativePath || file.webkitRelativePath || file.name).replace(/\\/g, '/');
    }
    function frames(value) {
        const match = /^(\d+):(\d{2}):(\d{2})$/.exec(value);
        if (!match || +match[2] >= 60 || +match[3] >= 75) throw new Error('The CUE contains an invalid track time.');
        return (+match[1] * 60 + +match[2]) * 75 + +match[3];
    }
    function cueTracks(text) {
        const tracks = [];
        let file, track;
        for (const line of text.replace(/^\uFEFF/, '').split(/\r?\n/)) {
            let match;
            if ((match = /^\s*FILE\s+(?:"([^"]+)"|(\S+))\s+(\S+)\s*$/i.exec(line))) {
                if (match[3].toUpperCase() !== 'BINARY') throw new Error('This CUE needs raw BINARY tracks.');
                file = (match[1] || match[2]).replace(/\\/g, '/'); track = null;
            } else if ((match = /^\s*TRACK\s+(\d+)\s+(\S+)\s*$/i.exec(line))) {
                if (!file) throw new Error('The CUE names a track before its file.');
                track = {file, number: +match[1], mode: match[2].toUpperCase(), indices: {}};
                tracks.push(track);
            } else if ((match = /^\s*INDEX\s+(\d+)\s+(\S+)\s*$/i.exec(line))) {
                if (!track) throw new Error('The CUE has an index outside a track.');
                if (Object.hasOwn(track.indices, +match[1])) throw new Error('The CUE repeats a track index.');
                track.indices[+match[1]] = frames(match[2]);
            }
        }
        if (tracks.length !== 2 || tracks[0].number !== 1 || tracks[1].number !== 2 ||
            tracks[0].mode !== 'MODE2/2352' || tracks[1].mode !== 'AUDIO')
            throw new Error('This patch needs the PlayStation the Best disc with one MODE2/2352 data track and one audio track.');
        if (tracks.some(t => !Object.hasOwn(t.indices, 1))) throw new Error('The CUE is missing a track start.');
        return tracks;
    }
    function resolveFile(name, cue, files) {
        const directory = pathOf(cue).split('/').slice(0, -1).join('/');
        const wanted = (directory ? directory + '/' : '') + name;
        let matches = files.filter(f => pathOf(f).toLowerCase() === wanted.toLowerCase());
        if (!matches.length) {
            const base = name.split('/').pop().toLowerCase();
            matches = files.filter(f => pathOf(f).split('/').pop().toLowerCase() === base);
        }
        if (!matches.length) throw new Error(`Also add the track named ${name} in the CUE.`);
        if (matches.length !== 1) throw new Error(`More than one selected file is named ${name}. Choose one disc at a time.`);
        return matches[0];
    }
    async function rawTracks(files, release) {
        if (!files.length) throw new Error('Choose or drop your disc files first.');
        const cues = files.filter(f => /\.cue$/i.test(f.name));
        if (cues.length > 1) throw new Error('More than one CUE was found. Choose one disc at a time.');
        if (cues.length) {
            const cue = cues[0];
            if (cue.size > 1024 * 1024) throw new Error('The CUE is too large to be a track list.');
            const tracks = cueTracks(await cue.text());
            const dataFile = resolveFile(tracks[0].file, cue, files);
            const audioFile = resolveFile(tracks[1].file, cue, files);
            const dataStart = tracks[0].indices[1] * 2352;
            const audioStart = (tracks[1].indices[0] ?? tracks[1].indices[1]) * 2352;
            const dataEnd = dataFile === audioFile ? audioStart : dataFile.size;
            if (dataStart >= dataEnd || audioStart >= audioFile.size)
                throw new Error('The CUE track positions extend beyond the selected files.');
            return {source: dataFile.slice(dataStart, dataEnd), audio: audioFile.slice(audioStart),
                audioName: audioFile.name, extractedAudio: dataFile === audioFile || audioStart !== 0};
        }
        const images = files.filter(f => /\.(bin|img|iso)$/i.test(f.name));
        const combined = images.filter(f => f.size === release.source.size + release.audio.size);
        if (combined.length === 1 && images.length === 1) {
            return {source: combined[0].slice(0, release.source.size),
                audio: combined[0].slice(release.source.size), audioName: 'tokimemo-en-track2.bin', extractedAudio: true};
        }
        const data = images.filter(f => f.size === release.source.size);
        const audio = images.filter(f => f.size === release.audio.size);
        if (data.length > 1 || audio.length > 1 || combined.length > 1)
            throw new Error('More than one matching disc was found. Choose one disc at a time.');
        if (data.length === 1 && audio.length === 1)
            return {source: data[0], audio: audio[0], audioName: audio[0].name, extractedAudio: false};
        if (data.length === 1) throw new Error('The data track is here. Also add the original audio track, or use a complete disc archive.');
        if (images.some(f => /\.iso$/i.test(f.name) && f.size % 2048 === 0))
            throw new Error('This data-only ISO cannot supply the complete disc. Use a lossless BIN/CUE or CHD copy with its audio and movie data.');
        throw new Error('These files do not contain the supported PlayStation the Best disc. Add its CUE and tracks, or a complete disc archive.');
    }
    return {rawTracks, cueTracks};
})();
if (typeof module !== 'undefined') module.exports = DiscInputs;
