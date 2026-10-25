'use strict';

const stage = document.querySelector('#media-stage');
const timeline = document.querySelector('#timeline');
const fullscreenButton = document.querySelector('#fullscreen');
const previousButton = document.querySelector('#previous');
const nextButton = document.querySelector('#next');
const progressBadge = document.querySelector('#port-progress');
const progressValue = document.querySelector('#progress-value');
const progressArc = document.querySelector('#progress-arc');
let items = [];
let selectedId = null;
let lastNewestId = null;

function updateProgress(progress) {
  if (!progress || !Number.isInteger(progress.overall_percent) || progress.overall_percent < 0 || progress.overall_percent > 100 ||
      !Number.isInteger(progress.first_slice_percent) || progress.first_slice_percent < 0 || progress.first_slice_percent > 100 ||
      typeof progress.estimated !== 'boolean') return;
  const qualifier = progress.estimated ? 'Estimated' : 'Verified';
  progressValue.textContent = `${progress.estimated ? '~' : ''}${progress.overall_percent}%`;
  progressArc.style.setProperty('--dash-offset', String(97.39 * (1 - progress.overall_percent / 100)));
  progressBadge.setAttribute('aria-label', `${qualifier} overall Japanese port progress: ${progress.overall_percent} percent. First slice: ${progress.first_slice_percent} percent.`);
  progressBadge.title = `${qualifier} progress toward the complete Japanese port. First slice: ${progress.first_slice_percent}%.`;
}

function mediaUrl(item) {
  return `/media/${encodeURIComponent(item.file)}`;
}

function selectItem(item, focusThumb = false) {
  if (!item) return;
  selectedId = item.id;
  stage.replaceChildren();
  let media;
  if (item.kind === 'video') {
    media = document.createElement('video');
    media.controls = true;
    media.playsInline = true;
    media.preload = 'auto';
    media.src = mediaUrl(item);
  } else {
    media = document.createElement('img');
    media.alt = 'Port capture';
    media.decoding = 'async';
    media.src = mediaUrl(item);
  }
  media.dataset.itemId = item.id;
  stage.append(media);
  renderTimeline(focusThumb);
}

function renderTimeline(focusThumb = false) {
  timeline.replaceChildren();
  for (const item of items) {
    const button = document.createElement('button');
    button.type = 'button';
    button.className = 'thumb';
    button.setAttribute('aria-label', item.kind === 'video' ? 'Open video capture' : 'Open image capture');
    button.setAttribute('aria-current', item.id === selectedId ? 'true' : 'false');
    if (item.kind === 'image') {
      const image = document.createElement('img');
      image.loading = 'lazy';
      image.alt = '';
      image.src = mediaUrl(item);
      button.append(image);
    } else {
      const video = document.createElement('video');
      video.muted = true;
      video.preload = 'metadata';
      video.src = mediaUrl(item);
      button.append(video);
      const playMark = document.createElement('span');
      playMark.className = 'video-mark';
      playMark.setAttribute('aria-hidden', 'true');
      playMark.textContent = '▶';
      button.append(playMark);
    }
    button.addEventListener('click', () => selectItem(item));
    timeline.append(button);
  }
  if (focusThumb && selectedId) {
    timeline.querySelector('[aria-current="true"]')?.focus();
  }
}

async function refreshManifest() {
  try {
    const response = await fetch('/api/manifest.json', { cache: 'no-store' });
    if (!response.ok) return;
    const manifest = await response.json();
    if (!manifest || manifest.version !== 1 || !Array.isArray(manifest.items)) return;
    updateProgress(manifest.progress);
    const nextItems = manifest.items.filter(item => item && typeof item.id === 'string' && typeof item.file === 'string' && (item.kind === 'image' || item.kind === 'video'));
    const newest = nextItems[0] || null;
    const latestPreview = nextItems.find(item => item.kind === 'image') || newest;
    const changed = nextItems.map(item => item.id).join(',') !== items.map(item => item.id).join(',');
    if (!changed) return;
    const active = stage.querySelector('video');
    const preserveActive = active && !active.paused && !active.ended;
    items = nextItems;
    if (preserveActive && items.some(item => item.id === selectedId)) {
      renderTimeline();
    } else if (newest && newest.id !== lastNewestId) {
      lastNewestId = newest.id;
      selectItem(latestPreview);
    } else if (items.some(item => item.id === selectedId)) {
      renderTimeline();
    } else if (newest) {
      selectItem(latestPreview);
    } else {
      selectedId = null;
      stage.replaceChildren();
      renderTimeline();
    }
  } catch (_) {
    // A transient poll failure does not disturb the current media element.
  }
}

fullscreenButton.addEventListener('click', async () => {
  const target = stage.querySelector('img, video') || document.querySelector('.stage');
  if (document.fullscreenElement) {
    await document.exitFullscreen();
  } else {
    await target.requestFullscreen?.();
  }
});
function moveSelection(delta) {
  if (!items.length) return;
  const index = items.findIndex(item => item.id === selectedId);
  const next = items[(index + delta + items.length) % items.length];
  selectItem(next);
}
previousButton.addEventListener('click', () => moveSelection(-1));
nextButton.addEventListener('click', () => moveSelection(1));
document.addEventListener('keydown', event => {
  if (event.target instanceof HTMLVideoElement || event.target instanceof HTMLButtonElement) return;
  if (event.key === 'ArrowLeft') moveSelection(-1);
  if (event.key === 'ArrowRight') moveSelection(1);
});
refreshManifest();
window.setInterval(refreshManifest, 5000);
