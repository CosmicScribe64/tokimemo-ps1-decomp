/* T-39512: actual-disc browser acceptance test; run in tools/Dockerfile.patcher. */
const { chromium } = require('playwright');
const fs = require('node:fs');
const path = require('node:path');
const http = require('node:http');
const crypto = require('node:crypto');
const assert = require('node:assert/strict');
const [siteArg, originalArg, buildArg] = process.argv.slice(2);
if (!siteArg || !originalArg || !buildArg) throw new Error('Usage: SITE ORIGINAL_TRACK_DIR BUILT_DISC_DIR');
const site = path.resolve(siteArg), original = path.resolve(originalArg), built = path.resolve(buildArg);
const out = path.resolve('build/patcher-test'); fs.mkdirSync(out, {recursive: true});
const types = {'.html': 'text/html', '.js': 'text/javascript', '.css': 'text/css', '.json': 'application/json'};
const server = http.createServer((req, res) => {
    const file = path.resolve(site, '.' + (new URL(req.url, 'http://localhost').pathname === '/' ? '/index.html' : new URL(req.url, 'http://localhost').pathname));
    if (!file.startsWith(site + path.sep) || !fs.existsSync(file) || !fs.statSync(file).isFile()) { res.writeHead(404).end(); return; }
    res.setHeader('Content-Type', types[path.extname(file)] || 'application/octet-stream');
    fs.createReadStream(file).pipe(res);
});
const sha1 = file => new Promise((resolve, reject) => {
    const hash = crypto.createHash('sha1'); const stream = fs.createReadStream(file);
    stream.on('data', data => hash.update(data)); stream.on('end', () => resolve(hash.digest('hex'))); stream.on('error', reject);
});
(async () => {
    await new Promise(resolve => server.listen(0, '127.0.0.1', resolve));
    const url = 'http://127.0.0.1:' + server.address().port;
    const browser = await chromium.launch({headless: true});
    try {
        const page = await browser.newPage({viewport: {width: 1120, height: 1000}, acceptDownloads: true});
        const errors = []; page.on('pageerror', error => errors.push(error.message));
        page.on('console', msg => { if (msg.type() === 'error') console.log('browser:', msg.text()); });
        const requests = []; page.on('request', req => requests.push({url: req.url(), method: req.method()}));
        await page.goto(url); await page.getByRole('button', {name: 'Create English disc'}).waitFor();
        await page.waitForFunction(() => !document.getElementById('apply').disabled);
        await page.screenshot({path: path.join(out, 'desktop.png'), fullPage: true});
        await page.setViewportSize({width: 390, height: 844});
        await page.screenshot({path: path.join(out, 'mobile.png'), fullPage: true});
        assert(await page.evaluate(() => document.documentElement.scrollWidth <= innerWidth), 'mobile horizontal overflow');
        await page.setViewportSize({width: 1120, height: 1000});
        await page.locator('#source').setInputFiles({name: 'wrong.bin', mimeType: 'application/octet-stream', buffer: Buffer.from('wrong')});
        await page.locator('#audio').setInputFiles(path.join(original, 'track2.bin'));
        await page.locator('#apply').click();
        await page.waitForFunction(() => document.getElementById('status').textContent.includes('sizes do not match'));
        assert(await page.locator('#result').isHidden());
        await page.locator('#source').setInputFiles(path.join(original, 'track1.bin'));
        await page.locator('#apply').click(); await page.locator('#cancel').click();
        await page.waitForFunction(() => document.getElementById('status').textContent.startsWith('Cancelled'));
        assert(await page.locator('#result').isHidden());
        console.log('Wrong source and cancellation checks passed. Applying real patch…');
        await page.locator('#apply').click();
        await page.waitForFunction(() => !document.getElementById('result').hidden || document.getElementById('status-area').classList.contains('error'), null, {timeout: 240000});
        assert(await page.locator('#result').isVisible(), await page.locator('#status').textContent());
        await page.screenshot({path: path.join(out, 'complete.png'), fullPage: true});
        const trackEvent = page.waitForEvent('download'); await page.locator('#track-download').click();
        const trackDownload = await trackEvent; await trackDownload.saveAs(path.join(out, 'track1.bin'));
        assert.equal(await sha1(path.join(out, 'track1.bin')), await sha1(path.join(built, 'track1.bin')));
        const cueEvent = page.waitForEvent('download'); await page.locator('#cue-download').click();
        await (await cueEvent).saveAs(path.join(out, 'tokimemo-en.cue'));
        const expectedCue = fs.readFileSync(path.join(built, 'tokimemo-en.cue'), 'utf8').replace('"track1.bin"', '"tokimemo-en-track1.bin"');
        assert.equal(fs.readFileSync(path.join(out, 'tokimemo-en.cue'), 'utf8'), expectedCue);
        const badSource = path.join(out, 'wrong-same-size.bin');
        const fd = fs.openSync(badSource, 'w'); fs.ftruncateSync(fd, fs.statSync(path.join(original, 'track1.bin')).size); fs.closeSync(fd);
        await page.locator('#source').setInputFiles(badSource);
        await page.locator('#apply').click();
        await page.waitForFunction(() => document.getElementById('status').textContent.includes('does not match SLPM'), null, {timeout: 60000});
        assert(await page.locator('#result').isHidden());
        fs.unlinkSync(badSource);
        await page.locator('#source').setInputFiles(path.join(original, 'track1.bin'));
        await page.route('**/*.xdelta', route => {
            const damaged = fs.readFileSync(path.join(site, 'assets/tokimemo-en.xdelta'));
            damaged[damaged.length - 1] ^= 1;
            return route.fulfill({status: 200, body: damaged});
        });
        await page.locator('#apply').click();
        await page.waitForFunction(() => document.getElementById('status').textContent.includes('incomplete or damaged'), null, {timeout: 60000});
        assert(await page.locator('#result').isHidden());
        assert(requests.every(r => r.method === 'GET' && r.url.startsWith(url + '/')), 'Unexpected external request or upload');
        assert.deepEqual(errors, []);
        fs.writeFileSync(path.join(out, 'result.json'), JSON.stringify({status: 'passed', target_sha1: await sha1(path.join(out, 'track1.bin')), checks: ['wrong size', 'wrong source hash', 'corrupted patch', 'cancel', 'full patch', 'download hash', 'cue', 'no upload', 'responsive layout'], requests}, null, 2));
        console.log('PASS: browser download equals command-line build; CUE, cancellation, invalid source, local-only processing verified.');
    } finally { await browser.close(); }
})().catch(error => { console.error(error); process.exitCode = 1; }).finally(() => server.close());
