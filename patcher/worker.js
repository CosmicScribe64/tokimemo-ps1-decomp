/* T-39512: all disc processing stays in this worker. Vendored modules are pinned. */
importScripts('vendor/BinFile.js', 'vendor/HashCalculator.js', 'vendor/RomPatcher.format.vcdiff.js');

function adler32(file, offset, length) {
    return HashCalculator.adler32(file._u8array.buffer, offset, length);
}
async function digest(bytes, algorithm) {
    const hash = await crypto.subtle.digest(algorithm, bytes);
    return Array.from(new Uint8Array(hash), b => b.toString(16).padStart(2, '0')).join('');
}
function stage(message, progress) { postMessage({type: 'progress', message, progress}); }

onmessage = async ({data: {source, audio, patch, release}}) => {
    try {
        if (source.size !== release.source.size || audio.size !== release.audio.size)
            throw new Error('These track sizes do not match the supported PlayStation the Best disc. Select the original two BIN tracks.');
        stage('Checking the original data track…', 10);
        const sourceBytes = await source.arrayBuffer();
        if (await digest(sourceBytes, 'SHA-1') !== release.source.sha1)
            throw new Error('The data track does not match SLPM-86053, PlayStation the Best. Use an unmodified original track 1.');
        stage('Checking the audio track…', 30);
        if (await digest(await audio.arrayBuffer(), 'SHA-1') !== release.audio.sha1)
            throw new Error('The audio track does not match this disc. Select its original track 2.');
        stage('Checking the English patch…', 40);
        if (patch.byteLength !== release.patch.size || await digest(patch, 'SHA-256') !== release.patch.sha256)
            throw new Error('The patch download is incomplete or damaged. Reload the page and try again.');
        const magic = new Uint8Array(patch, 0, 4);
        if (magic[0] !== 0xd6 || magic[1] !== 0xc3 || magic[2] !== 0xc4 || magic[3] !== 0)
            throw new Error('This is not a supported xdelta patch.');
        stage('Applying the English patch…', 55);
        const output = VCDIFF.fromFile(new BinFile(patch)).apply(new BinFile(sourceBytes), true);
        if (output.fileSize !== release.target.size)
            throw new Error('The patched track has an unexpected size. No download was created.');
        stage('Verifying the finished track…', 85);
        const sha1 = await digest(output._u8array.buffer, 'SHA-1');
        if (sha1 !== release.target.sha1)
            throw new Error('The finished track failed verification. No download was created.');
        postMessage({type: 'complete', bytes: output._u8array.buffer, sha1}, [output._u8array.buffer]);
    } catch (error) {
        postMessage({type: 'error', message: error.message || 'Patching failed. Try again in a desktop browser.'});
    }
};
