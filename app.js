/* T-39512: static, browser-local patcher. No disc bytes enter a network request. */
'use strict';
const byId = id => document.getElementById(id);
let release, worker, request, selected = [], urls = [];
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
    byId('audio-download').removeAttribute('href');
    byId('audio-download').hidden = true;
}
function busy(active) {
    byId('apply').disabled = active || !release;
    byId('source').disabled = byId('audio').disabled = byId('disc-files').disabled = active;
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
    stop(); clearResult(); status('Cancelled.');
});
for (const id of ['source', 'audio']) byId(id).addEventListener('change', () => {
    selected = []; byId('disc-files').value = ''; byId('selection').textContent = 'Using separate BIN tracks'; clearResult();
});
function selectFiles(files) {
    if (worker || request) return;
    selected = Array.from(files); byId('source').value = byId('audio').value = '';
    byId('selection').textContent = selected.length ? selected.map(file => file.name).join(', ') : '';
    clearResult();
}
byId('disc-files').addEventListener('change', event => selectFiles(event.target.files));
for (const type of ['dragenter', 'dragover']) byId('drop-zone').addEventListener(type, event => {
    event.preventDefault(); if (!worker && !request) byId('drop-zone').classList.add('dragging');
});
byId('drop-zone').addEventListener('dragleave', () => byId('drop-zone').classList.remove('dragging'));
byId('drop-zone').addEventListener('drop', event => {
    event.preventDefault(); byId('drop-zone').classList.remove('dragging'); selectFiles(event.dataTransfer.files);
});
function prepareFiles(files) {
    return new Promise((resolve, reject) => {
        worker = new Worker('input-worker.js');
        worker.onerror = () => reject(new Error('The disc could not be opened. Close other tabs and try again on a desktop computer.'));
        worker.onmessage = ({data}) => {
            if (data.type === 'progress') { status(data.message); byId('progress').value = data.progress; }
            else if (data.type === 'error') reject(new Error(data.message));
            else if (data.type === 'complete') { worker.terminate(); worker = null; resolve(data); }
        };
        worker.postMessage({files, release});
    });
}
byId('patch-form').addEventListener('submit', async event => {
    event.preventDefault();
    if (!release || worker || request) return;
    clearResult();
    const separate = !selected.length;
    const files = separate ? [byId('source').files[0], byId('audio').files[0]].filter(Boolean) : selected;
    if (!files.length) { status('Choose or drop your disc files first.', true); return; }
    busy(true); byId('progress').value = 2;
    status('Reading your disc files…');
    const controller = new AbortController(); request = controller;
    try {
        const tracks = separate && files.length === 2
            ? {source: files[0], audio: files[1], audioName: files[1].name, extractedAudio: false}
            : await prepareFiles(files);
        if (controller.signal.aborted) return;
        const {source, audio} = tracks;
        const audioName = tracks.extractedAudio ? 'tokimemo-en-track2.bin' : tracks.audioName.split(/[\\/]/).pop();
        if (/["\r\n]/.test(audioName) || audioName.toLowerCase() === 'tokimemo-en-track1.bin')
            throw new Error('Rename the audio track to track2.bin, then select it again.');
        status('Loading the English patch…');
        const response = await fetch(release.patch.file, {signal: controller.signal});
        if (!response.ok) throw new Error('The English patch could not be downloaded. Please try again.');
        const patch = await response.arrayBuffer();
        if (controller.signal.aborted) return;
        request = null;
        worker = new Worker('worker.js');
        worker.onerror = () => { stop(); status('Patching stopped. Close other tabs and try again on a desktop computer.', true); };
        worker.onmessage = ({data}) => {
            if (data.type === 'progress') { status(data.message); byId('progress').value = data.progress; }
            else if (data.type === 'error') { stop(); status(data.message, true); }
            else if (data.type === 'complete') {
                download('track-download', new Blob([data.bytes], {type: 'application/octet-stream'}));
                const cue = `FILE "tokimemo-en-track1.bin" BINARY\n  TRACK 01 MODE2/2352\n    FLAGS DCP\n    INDEX 01 00:00:00\nFILE "${audioName}" BINARY\n  TRACK 02 AUDIO\n    FLAGS DCP\n    INDEX 00 00:00:00\n    INDEX 01 00:02:00\n`;
                download('cue-download', new Blob([cue], {type: 'text/plain'}));
                if (tracks.extractedAudio) {
                    download('audio-download', audio); byId('audio-download').hidden = false;
                    byId('save-instructions').textContent = 'Save all three downloads in the same folder. Open tokimemo-en.cue in your emulator.';
                } else {
                    byId('save-instructions').textContent = `Save both downloads in the same folder as ${audioName}, your unchanged audio track. Open tokimemo-en.cue in your emulator.`;
                }
                stop(); status('Finished.');
                byId('result').hidden = false; byId('result-title').focus();
            }
        };
        worker.postMessage({source, audio, patch, release}, [patch]);
    } catch (error) {
        if (error.name !== 'AbortError') { stop(); status(error.message, true); }
    }
});
fetch('release.json', {cache: 'no-cache'}).then(async response => {
    if (!response.ok) throw new Error('The English patch is unavailable. Please try again later.');
    const data = await response.json();
    if (data.format !== 1 || !data.source?.sha1 || !data.audio?.sha1 || !data.target?.sha1 || !data.patch?.sha256)
        throw new Error('The patch could not be loaded. Reload this page.');
    if (!/^assets\/[a-zA-Z0-9._-]+\.xdelta$/.test(data.patch.file)) throw new Error('The patch could not be loaded. Reload this page.');
    release = data;
    busy(false); status('');
}).catch(error => status(error.message, true));
