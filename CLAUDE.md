# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this repo is

University group assignment: **CCP6214 Algorithm Design & Analysis (MMU)**. Worth 40%, due 22 June 2026. The work is implementing and comparing sorting/search algorithms in standard C++ — **not a general software project**. The deliverable is a `.docx` report plus seven small C++ programs.

The repo is currently in **planning phase**. No real source code exists yet — only:

- `planning_docs/` — the assignment PDF and execution plans (`planning_pro.md` is authoritative; `PLANNING.md` is a kept-for-comparison first draft)
- `sample/` — reference folder structure with **skeleton-only** `.cpp` files (function bodies are `// TODO`) and sample input/output files copied from the PDF spec to serve as a byte-for-byte verification target
- `test.cpp` — empty placeholder, ignore

## The 7 programs that must exist

| Program | Purpose |
|---|---|
| `dataset_generator` | Produces `dataset_<n>.csv` with n unique random `(10-digit int, 5-letter string)` rows |
| `radix_sort` / `radix_sort_step` | LSD radix sort by integer key, base 10. `_step` traces the array after each digit pass |
| `heap_sort` / `heap_sort_step` | Maxheap, in-place. `_step` traces after each extract-max swap |
| `hash_table_search` / `hash_table_search_step` | Open-addressing hash table with linear probing. Full version times n searches (best/avg/worst); `_step` traces one probe path |

Output filenames are dictated by the PDF and **must match exactly** (see `planning_docs/CCP6214_Assignment.pdf` §D and `sample/results/` for examples).

## Hard constraints (from the PDF — read before coding)

- **No library sort/search**: no `std::sort`, `std::binary_search`, `std::find`, `std::lower_bound`, etc.
- **No STL containers that sort/search internally**: no `std::set`, `std::map`, `std::unordered_set`, `std::unordered_map`, `std::priority_queue`.
- Allowed: `std::vector`, `std::array`, `std::string`, `std::chrono`, `<random>`, file streams.
- **Timing excludes I/O.** Wrap *only* the algorithm call with `std::chrono::high_resolution_clock` — read CSV before the timer starts, write output after it stops. See the pattern in `sample/src/radix_sort.cpp`.
- **RNG seed = group leader's student ID** mapped via the letter table in `planning_pro.md` §11. All randomness in `dataset_generator` must use this seed; the seed is set immediately after `main()` starts.
- **Step output format must match the PDF samples byte-for-byte.** Targets are in `sample/results/dataset_1000_*_step_*.txt`.

## Build

Each `.cpp` is a standalone program — no Makefile, no CMake. Compile individually:

```bash
g++ -std=c++17 -O2 src/radix_sort.cpp -o radix_sort
g++ -std=c++17 -O2 src/heap_sort.cpp -o heap_sort
# ...etc for the other 5
```

`-O2` is required — without it runtimes are artificially slow and the experiment numbers become misleading.

## Verifying correctness

Run the program on the canonical 7-row dataset and `diff` against the PDF samples:

```bash
./dataset_generator 1000                       # produces dataset_1000.csv
./radix_sort_step dataset_1000.csv 1 7         # produces step trace .txt
diff dataset_1000_radix_sorted_step_1_7.txt \
     sample/results/dataset_1000_radix_sorted_step_1_7.txt
```

A clean diff is the bar for "the algorithm is correct".

## Plan & lane ownership

`planning_docs/planning_pro.md` is the source of truth for what to build, in what order, by whom, and by when. Key things to know before making changes:

- 3 ownership lanes (Member 1 = radix, Member 2 = heap, Member 3 = hash + AVL theoretical write-up)
- Days 1-2 = paired work on shared foundation (`Record`, CSV I/O, RNG, `dataset_generator`)
- Days 11-13 = group converges on report + submission

If you're asked to implement code, check which lane it belongs to and confirm with the user before writing across lane boundaries.

## File-extension contract (don't get this wrong)

- `.csv` → both **input** datasets and **sorted output** of `radix_sort` / `heap_sort`
- `.txt` → step traces (`*_step_*.txt`) and the hash search timing summary
- `.docx` → the report (NOT `.doc` or `.pdf` — the rubric penalizes this)

## What lives where vs OneDrive

The PDF (§F.4, §I.2) requires a OneDrive folder link in the report for files too big to ship in the 99MB submission zip. Anything above ~5 million rows (~85 MB CSV) belongs on OneDrive, not in git. `data/` and `results/` are intended to be gitignored at large sizes.

## When working in this repo

- The `sample/` skeletons are **reference scaffolding**, not the deliverable. Real source code, once written, should live in a sibling `src/` at the repo root (per the layout in `planning_pro.md` §6).
- If the user asks about the assignment requirements, the PDF is authoritative — when in doubt, read `planning_docs/CCP6214_Assignment.pdf` rather than relying on the plan summaries.
- The plans mention several open questions for the tutor (e.g. is `std::unordered_map` allowed in `dataset_generator`?). Treat these as unresolved until the user confirms a tutor answer.
