# CCP6214 Assignment — Execution Plan (planning_pro.md)

> **Source of authority:** `CCP6214_Assignment.pdf` in this repo. If this plan ever contradicts the PDF, the PDF wins.
> **This document supersedes:** `PLANNING.md` (the first-pass plan). Kept for comparison only.

---

## 0. Header

| Field | Value |
|---|---|
| Course | CCP6214 — Algorithm Design & Analysis (2610) |
| Weight | 40% |
| Today | **Tue 9 June 2026** |
| Deadline | **Mon 22 June 2026, 11:59pm** (14 calendar days inclusive) |
| Group size | 3 members |
| Group leader | _TBD by group_ — leader's student ID is the RNG seed for the whole assignment |
| Members | Member 1, Member 2, Member 3 _(fill in names)_ |
| Submission zip | `T<lec>L_G<grp>.zip` (e.g. `T13L_G04.zip`) |

---

## 1. Goals & Non-Goals

### Goals
1. Implement Radix Sort (LSD, base 10), Heap Sort (maxheap), and Hash Table Search in standard C++ — **no library sort/search, no STL containers that sort/search internally**.
2. Comparative analysis: theoretical complexity + experimental running time across ≥10 input sizes.
3. Each member runs all algorithms on their own hardware; each captures screenshots.
4. Produce a `.docx` report scoring "Excellent" on all 6 rubric criteria, with APA7 citations.
5. Demo + Q&A: every member can present and answer for every algorithm.

### Non-Goals
- We will **not** attempt to physically reach 9-billion records — out of RAM on any normal laptop (~108GB). We cap at what hardware allows + justify in report. The rubric's "Excellent" tier accepts "smaller than max input size, runtime ≥ 6-hour".
- We will **not** implement the AVL BSTs — the rubric asks for a **theoretical** comparison only.
- We will **not** optimize for performance beyond `-O2`. Correctness + format matching > speed.

---

## 2. Lane Assignments

Each algorithm is owned end-to-end by one member: code, step variant, complexity write-up, and experiment results. Shared work (foundation + wrap-up) is done as a whole group to satisfy the "everyone touches everything" requirement and the rubric's "every group member needs to understand all the assignment submitted answers".

| Lane | Owner | Primary deliverables |
|---|---|---|
| **A — Radix Sort** | Member 1 | `radix_sort.cpp`, `radix_sort_step.cpp`, radix complexity section in report, radix experiment results |
| **B — Heap Sort** | Member 2 | `heap_sort.cpp`, `heap_sort_step.cpp`, heap complexity section in report, heap experiment results |
| **C — Hash Search** | Member 3 | `hash_table_search.cpp`, `hash_table_search_step.cpp`, hash complexity section, **AVL theoretical comparison**, hash experiment results |
| **Shared (Days 1-2)** | Whole group | Repo + OneDrive setup, `Record` struct, CSV I/O, RNG, `dataset_generator.cpp` |
| **Shared (Days 11-13)** | Whole group | Report cover/intro/conclusion, formatting pass, demo prep, zip + OneDrive upload |

**Workload balancing note:** Lane C carries the AVL theoretical comparison (~1-2 pages of theory) to offset the simpler hash experiment. Lanes A and B are roughly equal — radix has more passes to trace, heap has trickier in-place reasoning.

---

## 3. 13-Day Milestone Calendar

| Days | Date | Phase | End-of-day checkpoint |
|---|---|---|---|
| **1–2** | Tue Jun 9 – Wed Jun 10 | **Foundation** (whole group, paired) | `dataset_generator` produces `dataset_1000.csv` matching the sample format |
| **3** | Thu Jun 11 | **Lane scaffolding** (split into lanes) | Each lane has a compiling binary skeleton |
| **4–5** | Fri Jun 12 – Sat Jun 13 | **Algorithm implementation** | All 6 algorithm files produce correct sort/search on `dataset_1000` |
| **6** | Sun Jun 14 | **Format verification + cross-review** | Step outputs match PDF samples byte-for-byte; each member has read another member's code |
| **7–8** | Mon Jun 15 – Tue Jun 16 | **Experiments** | Datasets generated at 10 sizes; each member has results up to their hardware ceiling (<6h per run) |
| **9–10** | Wed Jun 17 – Thu Jun 18 | **Report drafting** | Each lane owner's algorithm section drafted; AVL theoretical drafted |
| **11** | Fri Jun 19 | **Conclusion + integration** | Report at "Good" rubric level; uniform formatting; citations skeleton |
| **12** | Sat Jun 20 | **Polish + demo rehearsal** | Report at "Excellent" level; demo rehearsed end-to-end; OneDrive uploaded; zip prepared |
| **13** | Sun Jun 21 | **Buffer** | Reserved for slippage / last fixes / final rubric review |
| **14** | Mon Jun 22 | **Submit** before 11:59pm | Upload to tutor's platform |

### Day-by-day detail

**Day 1 (Tue Jun 9) — Kickoff meeting**
- Decide group leader → capture leader's student ID
- Compute seed: map student-ID letters via A=1, B=2, ..., J=0, K=1, ..., concat all digits, use as `srand((unsigned int)<digits>U);` or `mt19937_64 rng; rng.seed(<digits>);`
- Set up shared GitHub repo (already exists) + shared OneDrive folder
- Agree on file/folder structure (see §6)
- Assign lanes (Member ↔ Algorithm)
- Send open questions to tutor (see §10)

**Day 2 (Wed Jun 10) — Foundation code**
- Whole group (paired/mob): implement `Record` struct, CSV reader/writer, seeded RNG wrapper, `dataset_generator.cpp`
- Verify `dataset_generator 1000` produces a CSV with 1,000 unique 10-digit ints + 5-letter lowercase strings, randomized

**Day 3 (Thu Jun 11) — Lane scaffolding**
- Each lane: `main()`, argument parsing, read input CSV, call algorithm stub, write output file
- Commit even with the algorithm as a no-op

**Days 4–5 (Fri Jun 12 – Sat Jun 13) — Algorithm implementation**
- Lane A: implement radix sort (LSD, base 10, 10 passes); then implement step variant
- Lane B: build max-heap (Floyd), n-1 extract-max swaps; then step variant
- Lane C: open-addressing hash table with linear probing; full search (n searches with best/avg/worst timing); then step variant

**Day 6 (Sun Jun 14) — Format gate + cross-review**
- All lanes: `diff` step output vs PDF samples; fix until byte-for-byte match
- Cross-review pairs: A↔B, B↔C, C↔A
- Each member explains their algorithm to one other member

**Days 7–8 (Mon Jun 15 – Tue Jun 16) — Experiments**
- Group decides 10 input sizes (suggested baseline below; adjust top-2 to hardware)
- Generate datasets centrally; share via OneDrive
- Each member runs all 3 sort + 1 search experiments on their machine
- Capture: hardware screenshot, command-prompt screenshot per (algo × size)
- Big runs: start in evening, check next morning

**Days 9–10 (Wed Jun 17 – Thu Jun 18) — Report drafting**
- Each owner writes their algorithm section: pseudocode, complexity derivation, results table, plot
- Lane C drafts AVL theoretical comparison
- Group leader sets up report cover, structure, APA7 citation skeleton (CLRS, Sedgewick, Knuth)

**Day 11 (Fri Jun 19) — Integration**
- Group writes conclusion together: findings on same hardware, best sort for array-based implementation
- Stitch report — uniform terminology, consistent figure/table numbering
- Finish any leftover experiments

**Day 12 (Sat Jun 20) — Polish + demo**
- Polish report to "Excellent" rubric tier (re-check §9 DoD)
- Each member rehearses their algorithm presentation
- Cross-Q&A practice: random member, random algorithm
- Upload large files to OneDrive folder; test the share link in incognito browser
- Prepare zip; test extraction

**Day 13 (Sun Jun 21) — Buffer**
- Slippage absorption only — no new work planned
- Full rubric self-check
- If everything's done: submit early

**Day 14 (Mon Jun 22) — Submit**
- Final upload before 11:59pm
- Don't wait until evening — platform may be slow

---

## 4. Algorithm Implementation Notes

### 4.1 Radix Sort (LSD, base 10)
- Sort the records by the 10-digit **integer key only** (string is payload).
- 10 passes (one per digit, rightmost first). Use 10 buckets per pass.
- **Stable** sort required (essential for LSD correctness).
- Step output: print array state after each digit pass, labeled `d=10` down to `d=1`, in the exact bracketed format shown in the PDF sample.
- **Complexity:** O(d·(n + b)) where d=10 digits, b=10 base → effectively O(n).
- **Space:** O(n + b) for buckets.

### 4.2 Heap Sort (maxheap)
- In-place on the array of `Record`.
- Build max-heap using Floyd's algorithm (O(n)).
- Extract-max n-1 times: swap root with index `i`, decrement heap size, sift-down. Total O(n log n).
- Step output: print array state after each extract-max swap, labeled `i = n-1, n-2, ..., 1`, matching PDF sample format.
- Compare by integer key only.
- **Complexity:** O(n log n) worst, average, best.
- **Space:** O(1) extra (in-place).

### 4.3 Hash Table Search
- Hand-roll an open-addressing hash table with **linear probing** (the PDF mentions "linear collision resolution").
- Hash function on the integer key. Table size ≈ 1.3-2× n (prime preferred).
- Build: insert all n records. Search: probe until found or empty slot.
- `_step` variant: print the probe path for one target. Output `<key> = <key>/<string>` if found, `-1 != <key>` if not found (per PDF sample).
- Full variant: perform n searches (a single search is too fast to time), capture best, average, worst running times across the n searches.
- **Complexity (linear probing, load factor α):** average O(1/(1-α)) for successful search; worst O(n) (full table cluster).
- **Space:** O(table_size).

### 4.4 AVL theoretical comparison (Lane C, in report only)
Compare hash table search where the **bucket** is implemented as:
- (a) **Array-based AVL balanced BST** — contiguous storage, pointer arithmetic, cache-friendly, fixed/resizable array.
- (b) **Linked-list-based AVL balanced BST** — node-per-element with pointers, scattered memory, dynamic growth without resize cost.

Cover: time complexity (both O(log n) for search/insert/delete on balanced tree, same asymptotic), space overhead (linked = pointers per node; array = potential unused slots), cache locality, rebalancing cost, insertion/deletion patterns.

### 4.5 What you may NOT use
- ❌ `std::sort`, `std::stable_sort`, `std::partial_sort`, `std::nth_element`
- ❌ `std::set`, `std::map`, `std::multiset`, `std::multimap`
- ❌ `std::unordered_set`, `std::unordered_map` (these sort/search internally)
- ❌ `std::priority_queue` (it's a heap — would trivialize heap sort)
- ❌ `std::binary_search`, `std::lower_bound`, `std::upper_bound`, `std::find`
- ❌ Any third-party library

### 4.6 What you MAY use
- ✅ `std::vector`, `std::array`, raw arrays
- ✅ `std::string`, C strings
- ✅ `std::chrono` for timing
- ✅ `<random>` (mt19937_64) for the RNG
- ✅ `std::ifstream`, `std::ofstream` for file I/O
- ✅ `std::cout`, `std::cerr`

---

## 5. Experiment Plan

### Input sizes (suggested baseline — adjust top 2 to hardware)
1. 1,000
2. 10,000
3. 100,000
4. 1,000,000
5. 5,000,000
6. 10,000,000
7. 50,000,000
8. 100,000,000
9. 500,000,000 *(may need to drop based on RAM)*
10. 1,000,000,000 *(may need to drop based on RAM)*

**Rubric note:** to score "Excellent" on dataset generation, integer range must be ≥10-million OR smaller-than-max but runtime ≥6h for the algorithm using this input. Pick top-2 sizes so radix vs heap runtimes **differ by at least 60 seconds** at the largest.

### Timing rules
- Read CSV into memory **before** starting the timer.
- Start timer → call algorithm → stop timer.
- Write output **after** stopping the timer.
- Use `std::chrono::high_resolution_clock`; report in seconds with 3+ decimals.

### Per-member capture (each of the 3 members)
- 1× hardware spec screenshot (Windows: `System Information`; Mac: About This Mac; Linux: `lscpu` + `free -h`)
- 30+ command-prompt screenshots (3 algorithms × 10 sizes)
- All output files (or OneDrive folder for big ones)

### Group's reported "max input size"
Take the **minimum** across the 3 members' hardware ceilings — that's the group's reported max. Otherwise one member can't reproduce.

---

## 6. Repository & File Layout (proposed)

```
Algorithm-Design/
├── src/
│   ├── common/
│   │   ├── record.hpp           # struct Record { uint64_t key; char str[6]; }
│   │   ├── csv_io.hpp           # read_csv() / write_csv() — NOT timed
│   │   └── rng.hpp              # seeded mt19937_64
│   ├── dataset_generator.cpp
│   ├── radix_sort.cpp
│   ├── radix_sort_step.cpp
│   ├── heap_sort.cpp
│   ├── heap_sort_step.cpp
│   ├── hash_table_search.cpp
│   └── hash_table_search_step.cpp
├── data/                         # generated CSVs — gitignored, on OneDrive
├── results/                      # output .txt + screenshots — partial in repo, big on OneDrive
├── docs/
│   ├── report.docx
│   └── references.bib            # APA7 sources
├── CCP6214_Assignment.pdf
├── PLANNING.md                   # original plan (kept for diff)
└── planning_pro.md               # this file
```

Compile each program standalone, e.g.:
```
g++ -std=c++17 -O2 -Wall src/radix_sort.cpp src/common/*.hpp -o radix_sort
```

---

## 7. Risk Register

| Risk | Mitigation | Owner |
|---|---|---|
| 9B records = ~108GB RAM, infeasible | Report max practical size per member's hardware; justify cap in report | Group leader |
| Step output doesn't match PDF sample format byte-for-byte | Day 6 hard gate: `diff` actual vs sample, fix until clean | All lane owners |
| Member falls behind on their lane | Day 13 buffer absorbs 1 day; if >1 day, group reassigns at Day 11 sync | Group leader |
| I/O accidentally inside timing window | Day 6 cross-review specifically checks timing boundary | Cross-reviewer |
| Tutor platform issues on submission day | Submit Sunday evening (Day 13) if possible, not Monday night | Group leader |
| Zip >99MB | Set up OneDrive folder on Day 1; upload Day 12; test share link before submission | Group leader |
| Different hardware → comparison invalid | Expected; report each member's hardware + their results separately, average where meaningful | Each member |
| One member doesn't show for demo | Rubric: zero mark for absentees only, but everyone must understand everything → enforce Day 6 cross-review | Group leader |
| Group can't meet on Day 1 | If kickoff delayed, Days 1-2 collapse — flag immediately, shift buffer | Group leader |

---

## 8. Coordination Protocol

- **GitHub repo:** code + report drafts + small outputs.
- **OneDrive folder:** big input/output files (datasets ≥10M rows, all screenshots). Get a single shareable folder link, paste into report.
- **Daily 15-min sync:** suggested 9pm in group chat — blockers only, not status theater. Format: "I'm doing X, blocked by Y, need Z."
- **Async chat:** WhatsApp/Discord/whatever the group already uses.
- **Code review:** Day 6 mandatory cross-review (A↔B, B↔C, C↔A). Informal review encouraged throughout.
- **Branching:** simple — each lane works on `main` or feature branches; merge daily. The codebase is small enough that this doesn't need a strict policy.

---

## 9. Definition of Done (submission gate)

Use this as the final pre-submission checklist on Day 12 or 13.

### Code
- [ ] All 7 programs compile with `g++ -std=c++17 -O2 -Wall` with **no warnings**
- [ ] Each program runs on `dataset_1000.csv` end-to-end and produces correct output
- [ ] Step outputs match PDF samples **byte-for-byte** on `dataset_1000`
- [ ] No forbidden STL containers used (see §4.5)
- [ ] Timing excludes file I/O

### Experiments
- [ ] 10 input sizes covered
- [ ] Each member has run all 3 algorithms (sort + sort + search) at all sizes their hardware supports
- [ ] Each member has a hardware spec screenshot
- [ ] All command-prompt screenshots captured
- [ ] At max input size, radix vs heap differ by ≥60s

### Report
- [ ] Cover page: lecture section ID, tutorial section ID, group number, group ID, leader name, member student IDs, member names alphabetical, task % per member (all 100%), task descriptions
- [ ] All 6 rubric criteria covered at ≥"Good", aiming "Excellent":
  - [ ] Dataset generation
  - [ ] Radix sort complexity + demo
  - [ ] Heap sort complexity + demo
  - [ ] Hash table search complexity + demo
  - [ ] Conclusion (best sort + AVL theoretical comparison)
  - [ ] Document clarity (≥80% required content + citations)
- [ ] APA7 citations
- [ ] OneDrive folder link for big files
- [ ] All members' hardware screenshots embedded
- [ ] All running-time screenshots embedded or linked

### Submission
- [ ] Zip filename matches `T<lec>L_G<grp>.zip` convention
- [ ] Zip ≤99MB OR OneDrive link present in doc and tested in incognito
- [ ] `.docx` (NOT `.doc` or `.pdf`)
- [ ] `.cpp` source files included
- [ ] `.csv` sample input + `.txt` sample output included

### Demo readiness
- [ ] Every member can present and answer Q&A for **any** algorithm
- [ ] Demo script rehearsed end-to-end (generate → sort_step → sort → search_step → search)
- [ ] Sample inputs prepared (`dataset_1000.csv`, `dataset_1000000.csv`, `dataset_10000000.csv`)

---

## 10. Open Questions for Tutor (ask Day 1)

1. Is `std::unordered_map` allowed in `dataset_generator` for uniqueness tracking, or must we hand-roll a hash set too? (The rule says no library that "sorts or searches internally" — `unordered_map` searches internally on lookup.)
2. Practical max input size — is 9-billion literal, or "as large as runtime allows"? What if our hardware can't reach 6h runtime even at 9B?
3. AVL theoretical comparison — paper section only, or do you want pseudocode + diagrams?
4. Linear collision resolution wording in §E.4 — does this refer to the hash table's collision strategy, or only to handling duplicate **seeds** between groups?
5. Where do you want submission (Google Classroom, MMU portal, email)?
6. Is our group cleared to register (all 3 from same lab section)?

---

## 11. Quick Reference

### Sample step output format (radix, dataset_1000, rows 1-7)
```
[1000000038/uoren, 1000000009/igerk, ...] original
[..., ...] d=10
[..., ...] d=9
...
[..., ...] d=1
```

### Sample step output format (heap, dataset_1000, rows 1-7)
```
[...] initial
[...] i = 6
[...] i = 5
...
[...] i = 1
```

### Sample search step (found / not found)
```
2008864030 = 2008864030/rdiea
-1 != 123456789
```

### Sample full-search timing output
```
Best case time: x seconds
Average case time: y seconds
Worst case time: z seconds
```

### Seed example
Student ID `243UC247CT` → digits via letter map (U=1, C=3, T=2):
`2431324730` → `srand((unsigned int)2431324730U);` or `mt19937_64 rng; rng.seed(24313247300ULL);`

---

*End of plan. If anything's unclear, ask the group leader before assuming.*
