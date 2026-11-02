/* T-39512: static, browser-local patcher. No disc bytes enter a network request. */
'use strict';
const byId = id => document.getElementById(id);
let release, worker, request, urls = [];
function status(message, error = false) {
    byId('status').textContent = message;
    byId('status-area').classList.toggle('error', error);
}
function clearResult() {
    for (const url of urls) URL.revokeObjectURL(url);
    urls = [];
    byId('result').hidden = true;
    byId('track-download').removeAttribute('href');
    byId('cue-download').removeAttribute('href');
}
function busy(active) {
    byId('apply').disabled = active || !release;
    byId('source').disabled = byId('audio').disabled = active;
    byId('cancel').hidden = !active;
    byId('progress').hidden = !active;
    byId('patch-form').setAttribute('aria-busy', String(active));
}
function stop() {
    request?.abort(); request = null;
    if (worker) { worker.onmessage = null; worker.onerror = null; worker.terminate(); }
    worker = null;
    busy(false);
}
function download(id, blob) {
    const url = URL.createObjectURL(blob); urls.push(url); byId(id).href = url;
}
byId('cancel').addEventListener('click', () => {
    stop(); clearResult(); status('Cancelled. Your original tracks have not changed.');
});
for (const id of ['source', 'audio']) byId(id).addEventListener('change', clearResult);
byId('patch-form').addEventListener('submit', async event => {
    event.preventDefault();
    if (!release || worker || request) return;
    clearResult();
    const source = byId('source').files[0], audio = byId('audio').files[0];
    if (!source || !audio) return;
    if (/["\r\n]/.test(audio.name) || audio.name.toLowerCase() === 'tokimemo-en-track1.bin') {
        status('Rename the audio track to track2.bin, then select it again.', true); return;
    }
    busy(true); byId('progress').value = 2;
    status('Loading the English patch…');
    const controller = new AbortController(); request = controller;
    try {
        const response = await fetch(release.patch.file, {signal: controller.signal});
        if (!response.ok) throw new Error('The English patch could not be downloaded. Please try again.');
        const patch = await response.arrayBuffer();
        if (controller.signal.aborted) return;
        request = null;
        worker = new Worker('worker.js');
        worker.onerror = () => { stop(); status('The patch worker stopped. Try again in a desktop browser with enough free memory.', true); };
        worker.onmessage = ({data}) => {
            if (data.type === 'progress') { status(data.message); byId('progress').value = data.progress; }
            else if (data.type === 'error') { stop(); status(data.message, true); }
            else if (data.type === 'complete') {
                download('track-download', new Blob([data.bytes], {type: 'application/octet-stream'}));
                const cue = `FILE "tokimemo-en-track1.bin" BINARY\n  TRACK 01 MODE2/2352\n    FLAGS DCP\n    INDEX 01 00:00:00\nFILE "${audio.name}" BINARY\n  TRACK 02 AUDIO\n    FLAGS DCP\n    INDEX 00 00:00:00\n    INDEX 01 00:02:00\n`;
                download('cue-download', new Blob([cue], {type: 'text/plain'}));
                byId('audio-name').textContent = audio.name;
                byId('output-hash').textContent = data.sha1;
                stop(); status('Finished. All verification checks passed.');
                byId('result').hidden = false; byId('result-title').focus();
            }
        };
        worker.postMessage({source, audio, patch, release}, [patch]);
    } catch (error) {
        if (error.name !== 'AbortError') { stop(); status(error.message, true); }
    }
});
fetch('release.json', {cache: 'no-cache'}).then(async response => {
    if (!response.ok) throw new Error('This preview has not been packaged with a patch yet.');
    const data = await response.json();
    if (data.format !== 1 || !data.source?.sha1 || !data.audio?.sha1 || !data.target?.sha1 || !data.patch?.sha256)
        throw new Error('Build information is invalid. Reload this page.');
    if (!/^assets\/[a-zA-Z0-9._-]+\.xdelta$/.test(data.patch.file)) throw new Error('Invalid patch location.');
    release = data; byId('build-label').textContent = data.label;
    busy(false); status('Choose both original tracks to begin.');
}).catch(error => status(error.message, true));
