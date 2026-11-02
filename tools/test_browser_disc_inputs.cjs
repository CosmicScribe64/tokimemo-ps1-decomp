/* T-39527: exercise local disc readers with actual disc bytes in Chromium. */
const {chromium} = require('playwright');
const fs = require('node:fs');
const path = require('node:path');
const http = require('node:http');
const assert = require('node:assert/strict');
const site = path.resolve(process.argv[2]);
const inputs = process.argv.slice(3).map(name => path.resolve(name));
const server = http.createServer((req, res) => {
    const name = path.resolve(site, '.' + new URL(req.url, 'http://localhost').pathname);
    if (!name.startsWith(site + path.sep) || !fs.existsSync(name) || !fs.statSync(name).isFile()) return res.writeHead(404).end();
    res.setHeader('Content-Type', name.endsWith('.js') ? 'text/javascript' : name.endsWith('.wasm') ? 'application/wasm' : name.endsWith('.html') ? 'text/html' : 'application/octet-stream');
    fs.createReadStream(name).pipe(res);
});
(async () => {
    await new Promise(resolve => server.listen(0, '127.0.0.1', resolve));
    const browser = await chromium.launch({headless: true});
    try {
        const page = await browser.newPage();
        await page.goto(`http://127.0.0.1:${server.address().port}/index.html`);
        await page.locator('#disc-files').setInputFiles(inputs);
        const result = await page.evaluate(async () => {
            const release = await (await fetch('release.json')).json();
            return new Promise(resolve => {
                const worker = new Worker('input-worker.js');
                worker.onerror = event => { worker.terminate(); resolve({error: event.message}); };
                worker.onmessage = async ({data}) => {
                    if (data.type === 'progress') return;
                    worker.terminate();
                    if (data.type === 'error') return resolve({error: data.message});
                    const hash = async blob => Array.from(new Uint8Array(await crypto.subtle.digest('SHA-1', await blob.arrayBuffer())), b => b.toString(16).padStart(2, '0')).join('');
                    resolve({source: await hash(data.source), audio: await hash(data.audio), audioName: data.audioName, extractedAudio: data.extractedAudio});
                };
                worker.postMessage({files: Array.from(document.getElementById('disc-files').files), release});
            });
        });
        assert(!result.error, result.error);
        const release = JSON.parse(fs.readFileSync(path.join(site, 'release.json')));
        assert.equal(result.source, release.source.sha1); assert.equal(result.audio, release.audio.sha1);
        console.log(JSON.stringify({inputs: inputs.map(name => path.basename(name)), ...result}));
    } finally { await browser.close(); server.close(); }
})().catch(error => { console.error(error); process.exitCode = 1; server.close(); });
