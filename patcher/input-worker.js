/* T-39527: unpack disc files locally, then leave hash verification to worker.js. */
'use strict';
importScripts('disc-inputs.js');
const discExtension = /\.(cue|bin|img|iso|chd)$/i;
self.onmessage = async ({data: {files, release}}) => {
    try {
        let unpacked = files;
        const archives = files.filter(file => /\.(zip|7z)$/i.test(file.name));
        if (archives.length) {
            if (archives.length !== 1 || files.length !== 1)
                throw new Error('Choose one archive at a time, without loose disc files.');
            postMessage({type: 'progress', message: 'Unpacking your disc…', progress: 4});
            importScripts('vendor/7z/7zz.umd.js');
            const zip = await SevenZip({locateFile: name => 'vendor/7z/' + name,
                print: () => {}, printErr: () => {}});
            const fs = zip.FS;
            fs.mkdir('/input'); fs.mkdir('/output');
            fs.mount(zip.WORKERFS, {files: [files[0]]}, '/input');
            const result = zip.callMain(['x', '/input/' + files[0].name, '-o/output', '-y', '-p', '-ssc-',
                '-ir!*.cue', '-ir!*.bin', '-ir!*.img', '-ir!*.iso', '-ir!*.chd']);
            if (result !== 0) throw new Error('The archive could not be unpacked. Check that it is complete and not password-protected.');
            unpacked = [];
            function collect(directory) {
                for (const name of fs.readdir(directory)) {
                    if (name === '.' || name === '..') continue;
                    const path = directory + '/' + name;
                    const stat = fs.stat(path);
                    if (fs.isDir(stat.mode)) collect(path);
                    else if (discExtension.test(name)) {
                        unpacked.push(new File([fs.readFile(path)], path.slice('/output/'.length)));
                        fs.unlink(path);
                    }
                }
            }
            collect('/output');
            if (!unpacked.length) throw new Error('No disc image or tracks were found in this archive.');
        }
        const chds = unpacked.filter(file => /\.chd$/i.test(file.name));
        if (chds.length) {
            if (chds.length !== 1 || unpacked.length !== 1) throw new Error('Choose one CHD at a time.');
            postMessage({type: 'progress', message: 'Opening your CHD…', progress: 6});
            const module = await new Promise((resolve, reject) => {
                self.Module = {noInitialRun: true, noExitRuntime: true,
                    locateFile: name => 'vendor/chdman/' + name,
                    print: () => {}, printErr: () => {},
                    onAbort: reason => reject(new Error(String(reason))),
                    onRuntimeInitialized: () => resolve(self.Module)};
                importScripts('vendor/chdman/chdman.js');
            });
            const fs = module.FS;
            fs.mkdir('/output');
            fs.writeFile('/disc.chd', new Uint8Array(await chds[0].arrayBuffer()), {canOwn: true});
            const result = module.callMain(['extractcd', '-i', '/disc.chd', '-o', '/output/disc.cue', '-ob', '/output/disc.bin']);
            if (result !== 0) throw new Error('The CHD could not be read.');
            unpacked = ['disc.cue', 'disc.bin'].map(name => new File([fs.readFile('/output/' + name)], name));
        }
        const tracks = await DiscInputs.rawTracks(unpacked, release);
        tracks.extractedAudio ||= archives.length > 0 || chds.length > 0;
        postMessage({type: 'complete', ...tracks});
    } catch (error) {
        postMessage({type: 'error', message: error.message || 'The disc files could not be read.'});
    }
};
