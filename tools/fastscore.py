"""Faster candidate scoring for the solvers (T-11250).

The single-function scorer of tools/autotweak.py (shared with tools/bulk_m2c.py, mirrored by
tools/order_search.py and tools/permute.py) spent about 1 s of its ~1.3 s per candidate in
`permute.strip_functions` (one regex compile and one scan of the whole translation unit per
function name), and ran every candidate one after the other. This module holds the two pieces
that remove that, both with identical results:

  BaseBuilder  `permute.build_base` from a text: gcc -E of the whole file (20 ms, unchanged),
               then the target definition is cut out and the stripped text before and after it
               comes from a cache keyed by the unstripped text outside the definition. A candidate
               only changes the definition, so the strip is done once per draft; anything that
               does not fit the cache (declarations outside the function changed, a name that looks
               like a definition inside the body, no definition found) takes the legacy path.
  Workers      a small thread pool (plain threading: `queue`, which concurrent.futures imports, is
               shadowed by tools/queue.py) with submit/cancel semantics for prefetching the scores
               a hill climb is about to ask for in order.

`--exact` on the solvers switches both off (the legacy path, one candidate at a time).
"""
import collections
import os
import tempfile
import threading

SENTINEL = "\n/*@@FASTSCORE_TARGET@@*/\n"


def default_jobs(cores=None):
    """Scoring threads by default: cores - 2, at least 1."""
    cores = cores or os.cpu_count() or 1
    return max(1, cores - 2)


class BaseBuilder:
    """permute.build_base(root, src, func) for a text, with the strip of the other functions cached.

    `permute` is the tools/permute module (injected so the logic can be tested without Docker);
    it needs sh, CPP, strip_include_asm, strip_functions, function_range, signature_names and
    def_regex."""

    CACHE_SIZE = 4

    def __init__(self, permute, root, func):
        self.permute, self.root, self.func = permute, root, func
        self.cache = collections.OrderedDict()   # (before, after) of the unstripped text -> (head, tail, names)
        self.lock = threading.Lock()
        self.compute = threading.Lock()
        self.hits = self.misses = 0

    def preprocess(self, text):
        p = self.permute
        with tempfile.NamedTemporaryFile("w", suffix=".c", dir=self.root, delete=False) as t:
            t.write(p.strip_include_asm(text))
            tmp = t.name
        try:
            return p.sh(p.CPP + [tmp], cwd=self.root)
        finally:
            os.unlink(tmp)

    def legacy(self, pre):
        """What permute.build_base does after the preprocessor (SystemExit when no definition)."""
        p = self.permute
        if not p.def_regex(self.func).search(pre):
            raise SystemExit("permute.py: no C definition of %s" % self.func)
        return p.strip_functions(pre, self.func)

    def build(self, text):
        return self.from_preprocessed(self.preprocess(text))

    def _lookup(self, key):
        with self.lock:
            entry = self.cache.get(key)
            if entry is not None:
                self.cache.move_to_end(key)
        return entry

    def _strip(self, pre, a, b, key):
        """Strip the other functions around pre[a:b]; None when the sentinel does not survive."""
        p = self.permute
        sentinel_pre = pre[:a] + SENTINEL + pre[b:]
        stripped = p.strip_functions(sentinel_pre, self.func)
        if stripped.count(SENTINEL) != 1:
            return None
        head, tail = stripped.split(SENTINEL)
        entry = (head, tail, p.signature_names(sentinel_pre))
        with self.lock:
            self.cache[key] = entry
            while len(self.cache) > self.CACHE_SIZE:
                self.cache.popitem(last=False)
        return entry

    def from_preprocessed(self, pre):
        p = self.permute
        rng = p.function_range(pre, self.func)
        if rng is None:
            return self.legacy(pre)
        a, b = rng
        key = (pre[:a], pre[b:])
        region_names = p.signature_names(pre[a:b]) - {self.func}
        entry = self._lookup(key)
        if entry is None:
            with self.compute:       # one strip at a time: threads that miss wait for the first
                entry = self._lookup(key)
                if entry is None:
                    entry = self._strip(pre, a, b, key)
                    if entry is None:
                        return self.legacy(pre)
                    self.misses += 1
                else:
                    self.hits += 1
        else:
            self.hits += 1
        head, tail, names = entry
        if not region_names <= names:
            # a name that looks like a definition inside the body would be stripped elsewhere too
            return self.legacy(pre)
        return head + pre[a:b] + tail


class Job:
    """A scheduled call: result() waits for it, cancel() drops it when it has not started."""

    def __init__(self, fn, arg):
        self.fn, self.arg = fn, arg
        self.done = threading.Event()
        self.value, self.error, self.state = None, None, "pending"   # pending running done cancelled

    def result(self):
        self.done.wait()
        if self.error is not None:
            raise self.error
        return self.value


class Workers:
    """`n` daemon threads that run Jobs first in, first out."""

    def __init__(self, n):
        self.todo = collections.deque()
        self.cv = threading.Condition()
        self.closed = False
        self.threads = [threading.Thread(target=self._run, daemon=True) for _ in range(max(1, n))]
        for t in self.threads:
            t.start()

    def submit(self, fn, arg):
        job = Job(fn, arg)
        with self.cv:
            self.todo.append(job)
            self.cv.notify()
        return job

    def cancel_pending(self):
        """Drop every job that has not started; returns how many."""
        with self.cv:
            dropped = list(self.todo)
            self.todo.clear()
        for job in dropped:
            job.state = "cancelled"
            job.done.set()
        return len(dropped)

    def _run(self):
        while True:
            with self.cv:
                while not self.todo and not self.closed:
                    self.cv.wait()
                if self.closed and not self.todo:
                    return
                job = self.todo.popleft()
            job.state = "running"
            try:
                job.value = job.fn(job.arg)
            except BaseException as exc:      # delivered to whoever asks for the result
                job.error = exc
            job.state = "done"
            job.done.set()

    def close(self):
        self.cancel_pending()
        with self.cv:
            self.closed = True
            self.cv.notify_all()
