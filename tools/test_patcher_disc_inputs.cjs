/* T-39527: input resolution without trusting file names as disc identity. Run in Docker. */
'use strict';
const assert = require('node:assert/strict');
const {File} = require('node:buffer');
const {rawTracks, cueTracks} = require('../patcher/disc-inputs.js');
const release = {source: {size: 4704}, audio: {size: 7056}};
const data = new File([new Uint8Array(4704).fill(17)], 'data.bin');
const audio = new File([new Uint8Array(7056).fill(29)], 'audio.bin');
function cue(text, name = 'disc.cue') { return new File([text], name); }
const twoFiles = 'FILE "data.bin" BINARY\n TRACK 01 MODE2/2352\n INDEX 01 00:00:00\n' +
    'FILE "audio.bin" BINARY\n TRACK 02 AUDIO\n INDEX 00 00:00:00\n INDEX 01 00:00:01\n';
(async () => {
    let result = await rawTracks([audio, data], release);
    assert.equal(result.source, data); assert.equal(result.audio, audio);
    result = await rawTracks([cue(twoFiles), audio, data], release);
    assert.deepEqual(await result.source.arrayBuffer(), await data.arrayBuffer());
    assert.deepEqual(await result.audio.arrayBuffer(), await audio.arrayBuffer());

    const combined = new File([data, audio], 'disc.img');
    result = await rawTracks([combined], release);
    assert.deepEqual(await result.source.arrayBuffer(), await data.arrayBuffer());
    assert.deepEqual(await result.audio.arrayBuffer(), await audio.arrayBuffer());
    assert.equal(result.extractedAudio, true);
    const combinedCue = cue('FILE "disc.img" BINARY\n TRACK 01 MODE2/2352\n INDEX 01 00:00:00\n' +
        'TRACK 02 AUDIO\n INDEX 00 00:00:02\n INDEX 01 00:00:03\n');
    result = await rawTracks([combinedCue, combined], release);
    assert.deepEqual(await result.source.arrayBuffer(), await data.arrayBuffer());
    assert.deepEqual(await result.audio.arrayBuffer(), await audio.arrayBuffer());

    const nestedData = new File([data], 'collection/DATA.BIN');
    const nestedAudio = new File([audio], 'collection/AUDIO.BIN');
    result = await rawTracks([cue(twoFiles, 'collection/disc.cue'), nestedData, nestedAudio], release);
    assert.deepEqual(await result.source.arrayBuffer(), await data.arrayBuffer());
    assert.deepEqual(await result.audio.arrayBuffer(), await audio.arrayBuffer());
    result = await rawTracks([new File([data], 'data.iso'), audio], release);
    assert.equal(result.source.size, data.size);

    await assert.rejects(rawTracks([], release), /Choose or drop/);
    await assert.rejects(rawTracks([data], release), /audio track/);
    await assert.rejects(rawTracks([cue(twoFiles)], release), /Also add/);
    await assert.rejects(rawTracks([data, new File([data], 'other.bin'), audio], release), /More than one/);
    await assert.rejects(rawTracks([cue(twoFiles), cue(twoFiles, 'other.cue'), data, audio], release), /More than one CUE/);
    await assert.rejects(rawTracks([cue(twoFiles), new File([data], 'a/data.bin'),
        new File([data], 'b/data.bin'), audio], release), /More than one selected file/);
    await assert.rejects(rawTracks([new File([new Uint8Array(2048)], 'cooked.iso')], release), /data-only ISO/);
    await assert.rejects(rawTracks([cue(twoFiles.replace('INDEX 00 00:00:00', 'INDEX 00 00:00:06')),
        data, audio], release), /beyond/);
    assert.throws(() => cueTracks(twoFiles.replace('MODE2/2352', 'MODE1/2048')), /MODE2/);
    assert.throws(() => cueTracks(twoFiles.replace('00:00:01', '00:60:00')), /invalid track time/);
    assert.throws(() => cueTracks(twoFiles.replace('INDEX 00', 'INDEX 01')), /repeats/);
    console.log('Disc input resolver checks pass. Source and audio hashes are verified by the patch worker.');
})().catch(error => { console.error(error); process.exitCode = 1; });
