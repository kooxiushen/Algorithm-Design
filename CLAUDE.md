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

Note the source tree is **split into two subfolders** — `src/dataset/` and `src/sorting/` — not a flat `src/`. Paths below reflect the actual layout.

| Path | Status |
|---|---|
| `src/dataset/dataset_generator.cpp` | ✅ implemented; takes `<n>`, writes `dataset_<n>.csv` to the **current dir** |
| `src/sorting/radix_sort.cpp`, `radix_sort_step.cpp` | ✅ implemented; take CLI args, write into a `result/` subfolder (uses `<filesystem>`) |
| `src/sorting/heap_sort.cpp`, `heap_sort_step.cpp` | ✅ implemented; **now take CLI args too** — `heap_sort <input_csv>`, `heap_sort_step <input_csv> <start> <end>`. Write `.csv`/`.txt` to the **current dir** (no `result/` subfolder) |
| `src/sorting/hash_table_search.cpp`, `hash_table_search_step.cpp` | ✅ implemented; take a CSV arg, write `.txt` to the current dir |
| `DEMO.md` | The single Windows/PowerShell run guide. Compiles all 7 (correct `src/dataset/` + `src/sorting/` paths), walks each program on `dataset_1000.csv` with **verified** console output (timing shown as a `<elapsed>` placeholder), then runs across all 10 sizes with a blank results table. This is the only run guide — `RUN.md` and `REPORT_RUN_WINDOWS.md` were removed as duplicates |
| `report.docx` | Written separately outside this repo |
| `planning_docs/` | Holds `CCP6214_Assignment.pdf` (the authoritative spec — **read it directly**), plus `planning_pro.md` and `PLANNING.md`. **Always ignore `planning_pro.md` and `PLANNING.md`** — they are stale first-pass plans that drift from the code; never cite or rely on them. The PDF is the only source of truth here |
| `sample/` | **Not in the `Testing`/`main` worktree** — lives on the separate `sample` branch (`origin/sample`). Holds reference step-trace outputs for byte-for-byte verification |

## The 7 programs (target deliverables)

| Program | Purpose |
|---|---|
| `dataset_generator` | Produces `dataset_<n>.csv` with n unique random `(10-digit int, 5-letter string)` rows |
| `radix_sort` / `radix_sort_step` | LSD radix sort by integer key, base 10. `_step` traces array after each digit pass |
| `heap_sort` / `heap_sort_step` | Maxheap, in-place. `_step` traces after each extract-max swap |
| `hash_table_search` / `hash_table_search_step` | Open-addressing hash table with linear probing. Full version times n searches (best/avg/worst); `_step` traces one probe path |

Output filenames are dictated by the PDF and **must match exactly** — see `planning_docs/CCP6214_Assignment.pdf` §D and the `sample` branch's `sample/results/` for examples.

## Cross-lane inconsistencies (important — verify before assuming)

The three lanes were authored separately and have only **partly** been reconciled.
The CLI-args drift is now gone (all programs take `argv`), but two differences remain.
When working across programs, do not assume fully uniform behaviour:

- **CLI args (now uniform):** all programs read from `argv`. `dataset_generator <n>`;
  `radix_sort`/`heap_sort`/`hash_table_search <input_csv>`; the `_step` variants take
  `<input_csv> <start_row> <end_row>` (hash `_step` takes just `<input_csv>`). The heap
  lane was **rewritten** — it no longer hardcodes `dataset_30.csv` or uses `int main()`.
- **Output location (still split):** only the radix lane creates a `result/` subfolder
  (via `filesystem::create_directories`). `dataset_generator`, `heap_sort`/`heap_sort_step`,
  and the hash programs write to the **current working directory**.
- **`<filesystem>` dependency:** the radix programs `#include <filesystem>`, so
  `-std=c++17` is mandatory for them (not just nice-to-have).

If the user asks you to "make them consistent" or align with the PDF, treat the **PDF
§D filename spec** as the source of truth, and flag which lane diverges rather than
silently picking one.

## Coding conventions (agreed by group)

- **Each `.cpp` is self-contained.** No helper headers, no shared `common/` folder. The 7 programs duplicate small bits of CSV read/write rather than share — this is intentional, makes each file easy to compile alone and easy to defend in the demo Q&A.
- **`using namespace std;` is used** in every `.cpp` (no `std::` prefixes).
- **Course/member header block at the top of every `.cpp`** — see `src/dataset/dataset_generator.cpp` for the exact format. Required by PDF §F.1.a.
- **`long long` for `n` and positions** — `int` overflows at large input sizes the assignment expects.
- **Hand-roll uniqueness tracking** — see the `seen` vector + linear probing pattern in `src/dataset/dataset_generator.cpp`. Do not use `std::unordered_set` (see hard constraint below).

## Hard constraints (from the PDF — read before coding)

- **No library sort/search**: no `std::sort`, `std::binary_search`, `std::find`, `std::lower_bound`, etc.
- **No STL containers that sort/search internally**: no `std::set`, `std::map`, `std::priority_queue`.
- ⚠️ **`std::unordered_set` and `std::unordered_map` are an OPEN question** — PDF §C.4 (page 2) bans "data structure that performs sorting or searching internally" but doesn't name them. A strict reading bans them; a lenient reading allows them for uniqueness/dedup utility use. **Until the tutor confirms otherwise, treat them as banned** — hand-roll the equivalent. This is the safer position.
- Allowed: `std::vector`, `std::array`, `std::string`, `std::chrono`, `<random>`, file streams.
- **Timing excludes I/O.** Wrap *only* the algorithm call with `std::chrono::high_resolution_clock` — read CSV before the timer starts, write output after it stops.
- **RNG seeded with `2511325052ULL`** — group leader's mapped ID. Set immediately after `main()` starts.
- **Step output format must match the PDF samples byte-for-byte.** Targets are in `sample/results/dataset_1000_*_step_*.txt`.

## Build

Each `.cpp` is a standalone program. No Makefile. The sources live under
`src/dataset/` and `src/sorting/`. Compile each where it sits, or `cd` into the folder
first. `DEMO.md` has the full, verified Windows/PowerShell build-and-run walkthrough.

```bash
cd src
g++ -std=c++17 -O2 dataset/dataset_generator.cpp -o dataset_generator
g++ -std=c++17 -O2 sorting/radix_sort.cpp        -o radix_sort
g++ -std=c++17 -O2 sorting/heap_sort.cpp         -o heap_sort
# ...same pattern for radix_sort_step, heap_sort_step, hash_table_search(_step)
./dataset_generator 1000             # → dataset_1000.csv in the current directory
./radix_sort dataset_1000.csv        # → result/radix_sorted_dataset_1000.csv
./heap_sort dataset_1000.csv         # → heap_sorted_dataset_1000.csv (cwd, no result/)
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

If asked to implement code, check the current branch name to infer which lane you're in. Don't write across lanes without confirming with the user. Changes to `src/dataset/dataset_generator.cpp` likely need to land on `main` first, then be merged into the algorithm branches.

## File-extension contract (don't get this wrong)

- `.csv` → both **input** datasets and **sorted output** of `radix_sort` / `heap_sort`
- `.txt` → step traces (`*_step_*.txt`) and the hash search timing summary
- `.docx` → the report (NOT `.doc` or `.pdf` — the rubric penalizes this)

## OneDrive overflow

PDF §F.4, §I.2 require a single OneDrive folder link in the report for big files. Anything above ~5 million rows (~85 MB CSV) belongs on OneDrive, not in git or the submission zip. The submission zip has a 99 MB cap.

## When working in this repo

- Edit existing `.cpp` files in place; don't create headers or `common/` folders — the group agreed against that pattern.
- The report is written outside this repo; don't try to draft or scaffold it here.
- If the user asks about assignment requirements, `planning_docs/CCP6214_Assignment.pdf` is authoritative — read it directly (it is in the repo). **Always ignore `planning_docs/planning_pro.md` and `planning_docs/PLANNING.md`** — they are stale planning drafts that drift from the actual code; do not cite or rely on them.
- Open tutor questions (e.g. the `unordered_set`/`unordered_map` ban above) are unresolved until the user confirms an answer — treat them as open regardless of what the stale planning docs say.
