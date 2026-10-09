# Roadmap

The goal is a playable, readable, English-translated version of *Tokimeki Memorial: Forever with You* that does not
depend on the original code. It comes in five phases, in order. Later phases depend on earlier ones, and none has a date.

## 1. Byte-matching decompilation (in progress)

Write C for every function so that the rebuilt executable and overlays are identical to the originals.

## 2. Readable decompilation (not started)

Matching C is often awkward. This phase renames symbols, replaces magic numbers with constants, names and shares
types, and splits tangled functions where the match allows it. Some of this happens as functions are decompiled,
but the sweep through the whole code base happens after phase 1.

## 3. Godot port (not started)

Rebuild the game as a Godot project that runs on a modern desktop, using the readable code as the specification.
The PS1 hardware layer (GPU, sound chip, CD streaming) has to be replaced, not translated. This is the largest
phase after phase 1, and its design is undecided.

## 4. Modernize the port (not started)

Widescreen support first. The rest is to be decided once the port runs.

## 5. English translation (not started)

Translate the game into English, then fix the user interface that translation breaks (text boxes, menus, fonts) and
make the result look good in English.

The first translation will come from an AI, and the game and the repository will say so. Human translations are
wanted, and they will replace the AI text where they exist. A small tool for
submitting translations may come later. Nothing is built yet, and the format is undecided.
