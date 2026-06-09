# CCP6214 Assignment — Planning Document

**Course:** Algorithm Design & Analysis (2610)
**Weight:** 40%
**Deadline:** Week 13 — 22 June 2026, 11:59pm
**Group size:** 3–4 students

---

## 1. Goals

1. Implement Radix Sort (LSD) and Heap Sort (maxheap) in standard C++ — no library sort/search, no STL containers that sort/search internally.
2. Implement Hash Table Search with theoretical + experimental analysis.
3. Run experiments at ≥10 input sizes (up to 9 billion elements OR ≥6h runtime).
4. Produce a `.docx` report with complexity analysis, results, conclusions, AVL theoretical comparison, and APA7 citations.
5. Demo + Q&A (30 min) — every member must understand everything.

---

## 2. Deliverables Checklist

### Code (`.cpp`)
- [ ] `dataset_generator.cpp` — generate `dataset_n.csv` with n unique random `(int, string)` rows
- [ ] `radix_sort.cpp` — full sort + time
- [ ] `radix_sort_step.cpp` — step-by-step trace (start row → end row)
- [ ] `heap_sort.cpp` — full sort + time
- [ ] `heap_sort_step.cpp` — step-by-step trace
- [ ] `hash_table_search.cpp` — best/avg/worst case timing over n searches
- [ ] `hash_table_search_step.cpp` — search path output for one target

### Output files (`.txt` / `.csv`)
- [ ] `dataset_<n>.csv` for each input size
- [ ] `radix_sorted_dataset_<n>.csv`
- [ ] `heap_sorted_dataset_<n>.csv`
- [ ] `hash_table_search_dataset_<n>.txt`
- [ ] Step trace files for radix, heap, hash search
- [ ] Command-prompt screenshots of every run

### Document (`.docx`)
- [ ] Cover info (section IDs, group #, leader, members alphabetical, task %)
- [ ] Hardware screenshot per member
- [ ] Complexity analysis (theoretical) for each algorithm
- [ ] Experiment results tables + charts (per member)
- [ ] Conclusions (best sort, AVL array vs linked-list comparison)
- [ ] APA7 citations
- [ ] OneDrive folder link for big files

### Submission
- [ ] Single zip: `T<lec>L_G<grp>.zip` (e.g. `T13L_G04.zip`)
- [ ] If >99MB → use OneDrive folder link in doc

---

## 3. Repository Structure (proposed)

```
Algorithm-Design/
├── src/
│   ├── common/
│   │   ├── csv_io.hpp           # CSV read/write (I/O excluded from timing)
│   │   ├── record.hpp           # struct Record { uint64_t key; char str[6]; }
│   │   └── rng.hpp              # seeded RNG (mt19937_64)
│   ├── dataset_generator.cpp
│   ├── radix_sort.cpp
│   ├── radix_sort_step.cpp
│   ├── heap_sort.cpp
│   ├── heap_sort_step.cpp
│   ├── hash_table_search.cpp
│   └── hash_table_search_step.cpp
├── data/                         # generated CSVs (gitignored)
├── results/                      # output .txt files + screenshots
├── docs/
│   ├── report.docx
│   └── references.md
├── scripts/
│   └── run_experiments.sh
├── PLANNING.md
└── README.md
```

---

## 4. Implementation Plan (phased)

### Phase 1 — Foundation (Week 1)
- [ ] Decide group leader → fix seed (map letters A=1..J=0, K=1..)
- [ ] Set up repo structure, .gitignore for `data/`
- [ ] Common utilities:
  - `Record` struct (10-digit int + 5-char string)
  - CSV reader / writer
  - Seeded RNG using `mt19937_64` with `srand2(<leader-id-digits>)`
- [ ] `dataset_generator` — generates unique random integers in [1e9, 9_999_999_999], 5 lowercase letters
  - Use hash-set or sorted-set approach for uniqueness (NOT a sort lib — write own or use linear scan for small; for large datasets, use bit-array of 9B is infeasible → use rejection with `unordered_set`? **Note:** `unordered_set` is a hash structure not a sort — likely allowed but **confirm with tutor**. Safer: implement own open-addressing hash for uniqueness check.

### Phase 2 — Sorting algorithms (Week 2)
- [ ] **Radix Sort (LSD)** — sort by 10-digit integer key, process rightmost digit first
  - Base 10 buckets (or base 256 for speed — discuss)
  - Stable; carry the 5-char string alongside
  - Complexity: O(d·(n+k)) where d=10, k=10 → O(n)
- [ ] **Heap Sort (maxheap)** — sort by integer key
  - Build max-heap O(n), then n extract-max O(n log n) total
  - In-place on array of Records
- [ ] **`_step` variants** — same algorithm but print array state at each pass/iteration to file
  - Radix: print after each digit pass (d=10 down to 1, or 1 to 10)
  - Heap: print after each `i = n-1, n-2, ..., 1` extract-max swap
  - Output only rows in [start_row, end_row]

### Phase 3 — Hash table search (Week 2–3)
- [ ] Implement hash table (array-based, open addressing or chaining via own linked list)
  - Hash function on integer key — discuss: modulo prime, or fold
  - Linear probing for collisions (assignment mentions "linear collision resolution")
- [ ] `hash_table_search` — perform n searches; capture best/avg/worst times
- [ ] `hash_table_search_step` — output search path for one target (found or not found)
- [ ] **Theoretical:** compare hash search using array-based AVL BST vs linked-list-based AVL BST as the bucket structure → write this section in report

### Phase 4 — Experiments (Week 3)
- [ ] Pick 10 input sizes spanning small → max (e.g. 1K, 10K, 100K, 1M, 10M, 100M, 500M, 1B, 5B, 9B — but cap by 6h runtime constraint, ensuring ≥60s gap between radix vs heap at largest size)
- [ ] Each member runs all algorithms on their hardware
- [ ] Capture command-prompt screenshots — running time printed by program
- [ ] Timing rule: **exclude file I/O**; wrap sort/search only with `chrono::high_resolution_clock`
- [ ] Store outputs in OneDrive if zip would exceed 99MB

### Phase 5 — Documentation (Week 3–4)
- [ ] Write complexity analysis sections (radix, heap, hash)
- [ ] Add experiment tables + plots (n vs time)
- [ ] Conclusions:
  - Findings on same hardware
  - Best sorting algorithm for array-based implementation
  - **Theoretical** comparison: hash search w/ array-AVL-BST vs linked-list-AVL-BST
- [ ] APA7 citations (CLRS, Sedgewick, Knuth etc.)
- [ ] Hardware specs screenshot per member
- [ ] OneDrive link to all big input/output files

### Phase 6 — Demo prep (Week 4)
- [ ] Each member can present ≥1 algorithm (dataset_generator / radix / heap / hash)
- [ ] Everyone understands every part — internal Q&A practice
- [ ] Verify demo script runs end-to-end on sample inputs:
  1. Generate dataset_10000000.csv
  2. Run radix_sort_step / heap_sort_step / hash_table_search_step on dataset_1000.csv
  3. Run full radix_sort / heap_sort / hash_table_search on dataset_1000000.csv

---

## 5. Key Design Decisions to Resolve

| # | Decision | Options | Notes |
|---|----------|---------|-------|
| 1 | Record storage | `struct{uint64_t; char[6]}` (12B) vs `pair<int,string>` (≥40B) | Memory matters at 9B elements → struct wins |
| 2 | Radix base | base 10 vs base 256 | Base 10 = simpler/matches spec wording; base 256 = faster |
| 3 | Uniqueness in generator | Own hash set vs rejection w/ bit-tracking | Bit array of 9B bits = ~1.1GB — feasible. Or open-addressing hash. **Cannot use std::set/unordered_set per rule 4** (sorts/searches internally) |
| 4 | Hash table collision | Linear probing vs separate chaining | Assignment mentions "linear collision resolution" for *seed collisions* but bucket strategy is ours to choose. Pick chaining with own linked list to discuss against AVL later |
| 5 | At 9B elements | RAM = ~108GB infeasible | Need external sort OR cap n at what RAM allows + justify in report |
| 6 | Step output format | Match sample exactly (`[a, b, c] d=10`) | Critical for marks — replicate spacing/format |

---

## 6. Risks & Mitigations

| Risk | Mitigation |
|------|-----------|
| Max input size won't fit in RAM | Set practical cap (e.g. 100M–1B) and ensure runtime crosses 6h OR ≥60s gap between sorts |
| Generating 9B unique 10-digit ints is slow | Use bit array (~1.1GB) for tracking, or shuffle first n of a generated range |
| Including I/O in timing inflates numbers | Strictly wrap only the algorithm call with timing; load to memory first |
| Different hardware → inconsistent results | That's expected — each member reports their own hardware specs |
| Step output doesn't match sample format | Build small unit test against sample_dataset_1000 first |
| Hitting 99MB zip limit | Pre-plan OneDrive folder upload; only put small samples in zip |
| Cross-platform compile issues | Stick to C++17, use `<chrono>` not platform-specific timers |

---

## 7. Task Allocation Template (fill in)

| Member | Tasks | % |
|--------|-------|---|
| Leader: [name] (ID: ____) | dataset_generator, seed setup, repo coordination | 25% |
| [name] | radix_sort + radix_sort_step + complexity write-up | 25% |
| [name] | heap_sort + heap_sort_step + complexity write-up | 25% |
| [name] | hash_table_search + step + AVL theoretical comparison | 25% |

Every member runs all 7 programs on their machine and contributes their results table.

---

## 8. Timeline (rough)

| Week | Milestone |
|------|-----------|
| W1 | Group formed, repo setup, common utils + dataset_generator |
| W2 | Both sorts (full + step) working on dataset_1000 |
| W2 | Hash search (full + step) working on dataset_1000 |
| W3 | Experiments on all 10 input sizes — every member runs |
| W3 | Draft report sections (complexity, results) |
| W4 | Finalize conclusions, AVL comparison, polish doc |
| W4 | Practice demo, submit zip |

---

## 9. Open Questions for Tutor

1. Is `std::unordered_map` allowed for uniqueness in dataset generator, or must we hand-roll hashing too?
2. What's the actual max practical input size for grading — is 9B literal, or "as large as runtime allows"?
3. For the AVL comparison — purely theoretical paper section, no implementation needed?
4. Same lab section requirement: are cross-section groups truly disallowed in 2026?

---

## 10. Definition of Done

- [ ] All 7 programs compile cleanly with `g++ -std=c++17 -O2 -Wall`
- [ ] Step outputs match sample format byte-for-byte on `dataset_1000`
- [ ] Each member has run all algorithms at all 10 input sizes with screenshots
- [ ] Report `.docx` complete with all 6 rubric sections at "Excellent" level
- [ ] Zip ≤99MB OR OneDrive link in doc
- [ ] Demo rehearsed; every member can answer for every algorithm
