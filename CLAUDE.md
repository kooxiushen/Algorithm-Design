# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this repo is

University group assignment: **CCP6214 Algorithm Design & Analysis (MMU)**. Worth 40%, due 22 June 2026. The deliverable is a `.docx` report plus seven small C++ programs.

**Group (3 members):**
- YAP HUI CHI — `242UC244M6`
- ERIC CHIN YAN HONG — `243UC247CQ`
- KOO XIU SHEN — `251UC25052` *(group leader → RNG seed source)*

**Group leader's seed:** student ID `251UC25052` → letter map (U=1, C=3) → **`2511325052`**. All random generation uses this exact seed.

## Current state

All 7 programs now exist and compile. The current branch is usually `Testing` — an
**integration branch** that merges the per-lane branches (`radix-sort`, `heap-sort`,
`hash-table`). Because the lanes were written independently, their conventions have
**drifted** — see "Cross-lane inconsistencies" below before assuming all 7 behave alike.

| Path | Status |
|---|---|
| `src/dataset_generator.cpp` | ✅ implemented; writes `dataset_<n>.csv` to the **current dir** |
| `src/radix_sort.cpp`, `radix_sort_step.cpp` | ✅ implemented; take CLI args, write into a `result/` subfolder (uses `<filesystem>`) |
| `src/heap_sort.cpp`, `heap_sort_step.cpp` | ✅ implemented, but **take no CLI args** — input `dataset_30.csv` and the step row range are hardcoded |
| `src/hash_table_search.cpp`, `hash_table_search_step.cpp` | ✅ implemented; take a CSV arg, write `.txt` to the current dir |
| `RUN.md` | Worked end-to-end example for the radix lane. Note: it describes a `dataset/` output folder that `dataset_generator.cpp` does **not** actually create (it writes to cwd) — RUN.md is partly aspirational |
| `report.docx` | Written separately outside this repo |
| `planning_docs/` | Only `demo_hash_search.md` is tracked here. The assignment **PDF and `planning_pro.md` are NOT in git** (local-only / OneDrive) — they may be absent from your worktree |
| `sample/` | **Not in the `Testing`/`main` worktree** — lives on the separate `sample` branch (`origin/sample`). Holds reference step-trace outputs for byte-for-byte verification |

## The 7 programs (target deliverables)

| Program | Purpose |
|---|---|
| `dataset_generator` | Produces `dataset_<n>.csv` with n unique random `(10-digit int, 5-letter string)` rows |
| `radix_sort` / `radix_sort_step` | LSD radix sort by integer key, base 10. `_step` traces array after each digit pass |
| `heap_sort` / `heap_sort_step` | Maxheap, in-place. `_step` traces after each extract-max swap |
| `hash_table_search` / `hash_table_search_step` | Open-addressing hash table with linear probing. Full version times n searches (best/avg/worst); `_step` traces one probe path |

Output filenames are dictated by the PDF and **must match exactly** — see `planning_docs/CCP6214_Assignment.pdf` §D (if present locally) and the `sample` branch's `sample/results/` for examples.

## Cross-lane inconsistencies (important — verify before assuming)

The three lanes were authored separately and have **not** been reconciled into one
convention. When working across programs, do not assume uniform behaviour:

- **CLI args:** `radix_sort`/`radix_sort_step` and `hash_table_search`/`_step` read the
  input path (and row range) from `argv`. **`heap_sort`/`heap_sort_step` take no args** —
  `int main()` with input `dataset_30.csv` and step range hardcoded inside.
- **Output location:** only the radix lane creates a `result/` subfolder (via
  `filesystem::create_directories`). `dataset_generator`, `heap_sort`, and the hash
  programs write to the **current working directory**.
- **`<filesystem>` dependency:** the radix programs `#include <filesystem>`, so
  `-std=c++17` is mandatory for them (not just nice-to-have).

If the user asks you to "make them consistent" or align with the PDF, treat the **PDF
§D filename spec** as the source of truth, and flag which lane diverges rather than
silently picking one.

## Coding conventions (agreed by group)

- **Each `.cpp` is self-contained.** No helper headers, no shared `common/` folder. The 7 programs duplicate small bits of CSV read/write rather than share — this is intentional, makes each file easy to compile alone and easy to defend in the demo Q&A.
- **`using namespace std;` is used** in every `.cpp` (no `std::` prefixes).
- **Course/member header block at the top of every `.cpp`** — see `src/dataset_generator.cpp` for the exact format. Required by PDF §F.1.a.
- **`long long` for `n` and positions** — `int` overflows at large input sizes the assignment expects.
- **Hand-roll uniqueness tracking** — see the `seen` vector + linear probing pattern in `src/dataset_generator.cpp`. Do not use `std::unordered_set` (see hard constraint below).

## Hard constraints (from the PDF — read before coding)

- **No library sort/search**: no `std::sort`, `std::binary_search`, `std::find`, `std::lower_bound`, etc.
- **No STL containers that sort/search internally**: no `std::set`, `std::map`, `std::priority_queue`.
- ⚠️ **`std::unordered_set` and `std::unordered_map` are an OPEN question** — PDF §C.4 (page 2) bans "data structure that performs sorting or searching internally" but doesn't name them. A strict reading bans them; a lenient reading allows them for uniqueness/dedup utility use. **Until the tutor confirms otherwise, treat them as banned** — hand-roll the equivalent. This is the safer position.
- Allowed: `std::vector`, `std::array`, `std::string`, `std::chrono`, `<random>`, file streams.
- **Timing excludes I/O.** Wrap *only* the algorithm call with `std::chrono::high_resolution_clock` — read CSV before the timer starts, write output after it stops.
- **RNG seeded with `2511325052ULL`** — group leader's mapped ID. Set immediately after `main()` starts.
- **Step output format must match the PDF samples byte-for-byte.** Targets are in `sample/results/dataset_1000_*_step_*.txt`.

## Build

Each `.cpp` is a standalone program. No Makefile. Build from inside `src/` (where
`RUN.md`'s worked example assumes you are), or pass the `src/` path explicitly.

```bash
cd src
g++ -std=c++17 -O2 dataset_generator.cpp -o dataset_generator
# ...same pattern for radix_sort.cpp, heap_sort.cpp, hash_table_search.cpp, etc.
./dataset_generator 1000        # → dataset_1000.csv in the current directory
./radix_sort dataset_1000.csv   # → result/radix_sorted_dataset_1000.csv
./heap_sort                     # NO args — reads hardcoded dataset_30.csv
```

`-std=c++17` is required (radix programs use `<filesystem>`). `-O2` is required — without
it runtimes are artificially slow and experiment numbers are misleading.

## Verifying correctness

Compare your step trace against the sample byte-for-byte. The samples live on the
`sample` branch, so check it out (e.g. into a worktree) first:

```bash
git worktree add ../sample-ref sample      # or: git checkout sample
./radix_sort_step dataset_1000.csv 1 7
diff result/dataset_1000_radix_sorted_step_1_7.txt \
     ../sample-ref/sample/results/dataset_1000_radix_sorted_step_1_7.txt
```

A clean diff is the bar for "the algorithm is correct". Note: this only works when the
input is the **sample's** `dataset_1000.csv` (the one the PDF's sample outputs were
derived from). A dataset you generate yourself — even with the leader's seed — will
produce a different but still-correct trace if it doesn't byte-match the sample input.

## Branch / lane ownership

Work is split into 3 lanes, one per member:

- **Lane A (Radix)** — `radix-sort` branch
- **Lane B (Heap)** — `heap-sort` branch
- **Lane C (Hash + AVL theoretical write-up)** — `hash-table` branch
- `main` — shared foundation (`dataset_generator`, conventions)
- `Testing` — integration branch where the lane branches are merged together (often the checked-out branch)
- `sample` — holds the `sample/` reference-output tree, kept separate from code branches

If asked to implement code, check the current branch name to infer which lane you're in. Don't write across lanes without confirming with the user. Changes to `src/dataset_generator.cpp` likely need to land on `main` first, then be merged into the algorithm branches.

## File-extension contract (don't get this wrong)

- `.csv` → both **input** datasets and **sorted output** of `radix_sort` / `heap_sort`
- `.txt` → step traces (`*_step_*.txt`) and the hash search timing summary
- `.docx` → the report (NOT `.doc` or `.pdf` — the rubric penalizes this)

## OneDrive overflow

PDF §F.4, §I.2 require a single OneDrive folder link in the report for big files. Anything above ~5 million rows (~85 MB CSV) belongs on OneDrive, not in git or the submission zip. The submission zip has a 99 MB cap.

## When working in this repo

- Edit existing `.cpp` files in place; don't create headers or `common/` folders — the group agreed against that pattern.
- The report is written outside this repo; don't try to draft or scaffold it here.
- If the user asks about assignment requirements, `planning_docs/CCP6214_Assignment.pdf` is authoritative — but it is **not tracked in git**, so it may be missing from your worktree; ask the user for it rather than guessing. Plans summarize but may drift.
- Open tutor questions (e.g. the `unordered_set`/`unordered_map` ban above) were tracked in `planning_docs/planning_pro.md`, which is also not in git. Treat such questions as unresolved until the user confirms an answer.
