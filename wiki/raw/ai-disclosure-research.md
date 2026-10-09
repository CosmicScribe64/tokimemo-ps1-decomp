# How other decomp repos word AI-assistance disclosures

Collected 2026-10-09 for [[tickets/T-0901-public-docs]]. Raw source note: quotes are short, everything else is paraphrased. Only public README and CONTRIBUTING text was read.

## Projects that disclose AI use

- CosmicScribe64/eds-decomp, README, "AI assistance": https://github.com/CosmicScribe64/eds-decomp . Says most of the decompilation and the wiki were written with AI coding agents directed by the maintainer, and that a function counts as decompiled only when its compiled code is byte-identical, checked by CI. Two sentences, no list of limits.
- sciaschi/CBFD-Recompiled, README, "AI assistance": https://github.com/sciaschi/CBFD-Recompiled . Longest of the group. Names the tool (Claude through Claude Code), lists what it covered, says what is verified (decompiled C only kept when it rebuilds identical bytes) and what is not (names, types and comments; address-suffixed names are best guesses). Also separates itself from the upstream project.
- Jovinull/sonicheroes, README, "AI assistance": https://github.com/Jovinull/sonicheroes . First person, three sentences. Nothing is accepted unverified: objdiff match per function and a SHA-1 check of the built DOL in CI.

## Projects that restrict or ban AI contributions

- SAT-R/sa2 (and sa3), README, "Strict No LLM / No AI Policy": https://github.com/SAT-R/sa2 . No LLM-written issues or pull requests. Reason given: the project exists so people can practise reverse engineering and own what they submit.
- pret/pokediamond, CONTRIBUTING, "AI Policy": https://github.com/pret/pokediamond/blob/master/CONTRIBUTING.md . Bans AI-generated contributions, allows disclosed mechanical chores and general questions.
- pangbox/rebang, CONTRIBUTING: https://github.com/pangbox/rebang . Code should be broadly human-written; LLM use for analysis and research is allowed and need not be disclosed.

## What this means for our wording

1. Disclosure sections are short and sit near the end of the README.
2. The strongest ones say what the byte-for-byte check proves and what it does not. Names, types and comments do not change the bytes, so they need their own caveat. The O.BIN names in this project fall in that group.
3. Some well-known decomp communities ban AI contributions. Our README says plainly that this project is AI-assisted, so people who object know before they invest time, and states that it is independent of other decomp projects.
4. Rules for outside contributors are a separate question from disclosure. Ours go in CONTRIBUTING.
