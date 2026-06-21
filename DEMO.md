# Run Guide — All 7 Programs (Windows / PowerShell)

The single copy-paste guide to compile and run every program, and to capture the screens
for the report. Every console block below is **real, verified output** from an actual run.
The timing numbers (`... seconds`) are from one real run on a dev machine
(MSYS2 g++ 14.2, `-O2`) — **yours will differ slightly**; re-run to record your own.

> **Run everything from inside `src\`.** All programs read/write files relative to the
> current folder, so staying in one place keeps the generated files together.
> ```powershell
> cd C:\Users\Dell\Documents\GitHub\Algorithm-Design\src
> ```

---

## 1. Compile all 7 programs (one time)

The sources live in two subfolders — `src\dataset\` and `src\sorting\`. These compile each
into a `.exe` in the current `src\` folder.

```powershell
g++ -std=c++17 -O2 dataset/dataset_generator.cpp     -o dataset_generator.exe
g++ -std=c++17 -O2 sorting/radix_sort.cpp            -o radix_sort.exe
g++ -std=c++17 -O2 sorting/radix_sort_step.cpp       -o radix_sort_step.exe
g++ -std=c++17 -O2 sorting/heap_sort.cpp             -o heap_sort.exe
g++ -std=c++17 -O2 sorting/heap_sort_step.cpp        -o heap_sort_step.exe
g++ -std=c++17 -O2 sorting/hash_table_search.cpp     -o hash_table_search.exe
g++ -std=c++17 -O2 sorting/hash_table_search_step.cpp -o hash_table_search_step.exe
```

**No output = success.** `-std=c++17` is required (the radix programs use `<filesystem>`);
`-O2` is required. Confirm the binaries exist:

```powershell
Get-ChildItem *.exe | Select-Object Name
```
```
Name
----
dataset_generator.exe
hash_table_search.exe
hash_table_search_step.exe
heap_sort.exe
heap_sort_step.exe
radix_sort.exe
radix_sort_step.exe
```

---

## 2. Dataset structure (the 10 sizes)

Every dataset is generated with the same fixed seed (`2511325052`), so each size is
reproducible on any machine: re-running `dataset_generator <n>` always yields the same rows.

| #  | Size (n)    | CSV file                | ~CSV size | Where it lives            |
|----|-------------|-------------------------|-----------|---------------------------|
| 1  | 1,000       | `dataset_1000.csv`      | ~16 KB    | git (sample)              |
| 2  | 10,000      | `dataset_10000.csv`     | ~160 KB   | git                       |
| 3  | 100,000     | `dataset_100000.csv`    | ~1.6 MB   | git                       |
| 4  | 500,000     | `dataset_500000.csv`    | ~8 MB     | git                       |
| 5  | 1,000,000   | `dataset_1000000.csv`   | ~16 MB    | git                       |
| 6  | 5,000,000   | `dataset_5000000.csv`   | ~80 MB    | OneDrive (near 85 MB cap) |
| 7  | 10,000,000  | `dataset_10000000.csv`  | ~160 MB   | OneDrive                  |
| 8  | 50,000,000  | `dataset_50000000.csv`  | ~800 MB   | OneDrive                  |
| 9  | 100,000,000 | `dataset_100000000.csv` | ~1.6 GB   | OneDrive                  |
| 10 | 150,000,000 | `dataset_150000000.csv` | ~2.4 GB   | OneDrive                  |

The walkthrough in Section 3 uses **`n = 1000`**: it is small, fast, and matches the
hardcoded demo key in the hash step program.

---

## 3. Walkthrough on `dataset_1000.csv` (one screenshot per program)

Run in order. Each block is one screenshot for the report.

### 3.1 — Generate the dataset

```powershell
.\dataset_generator.exe 1000
```
```
Wrote 1000 rows to dataset_1000.csv
```

Show the `<10-digit int>,<5-letter string>` row format:

```powershell
Get-Content dataset_1000.csv -TotalCount 3
```
```
9794922582,igjkc
3556224529,zakoc
5453444626,kfisc
```

### 3.2 — Radix sort (full)

```powershell
.\radix_sort.exe dataset_1000.csv
```
```
Sorted 1000 rows in 8.01e-05 seconds.
Output: result/radix_sorted_dataset_1000.csv
```

> Radix is the only program that writes its sorted CSV into a `result\` subfolder (it
> creates it automatically). The others write to the current folder.

Show the result is ascending by key:

```powershell
Get-Content result\radix_sorted_dataset_1000.csv -TotalCount 3
```
```
1011011326,vpcbc
1013237162,aoway
1013987915,nearp
```

### 3.3 — Radix sort step trace (rows 1–7)

```powershell
.\radix_sort_step.exe dataset_1000.csv 1 7
```
```
Wrote step trace to result/dataset_1000_radix_sorted_step_1_7.txt
```

The trace shows `original`, then one line per digit pass `d=10` (ones digit) down to
`d=1` (10⁹ digit):

```powershell
Get-Content result\dataset_1000_radix_sorted_step_1_7.txt
```
```
[9794922582/igjkc, 3556224529/zakoc, 5453444626/kfisc, 5274043219/hsglz, 8891229348/oaksp, 6756314515/nlkgc, 4908958932/vjuab] original
[9794922582/igjkc, 4908958932/vjuab, 6756314515/nlkgc, 5453444626/kfisc, 8891229348/oaksp, 3556224529/zakoc, 5274043219/hsglz] d=10
[6756314515/nlkgc, 5274043219/hsglz, 5453444626/kfisc, 3556224529/zakoc, 4908958932/vjuab, 8891229348/oaksp, 9794922582/igjkc] d=9
[5274043219/hsglz, 8891229348/oaksp, 6756314515/nlkgc, 3556224529/zakoc, 9794922582/igjkc, 5453444626/kfisc, 4908958932/vjuab] d=8
[9794922582/igjkc, 5274043219/hsglz, 6756314515/nlkgc, 3556224529/zakoc, 5453444626/kfisc, 4908958932/vjuab, 8891229348/oaksp] d=7
[6756314515/nlkgc, 9794922582/igjkc, 3556224529/zakoc, 8891229348/oaksp, 5274043219/hsglz, 5453444626/kfisc, 4908958932/vjuab] d=6
[5274043219/hsglz, 3556224529/zakoc, 8891229348/oaksp, 6756314515/nlkgc, 5453444626/kfisc, 9794922582/igjkc, 4908958932/vjuab] d=5
[8891229348/oaksp, 5453444626/kfisc, 5274043219/hsglz, 9794922582/igjkc, 3556224529/zakoc, 6756314515/nlkgc, 4908958932/vjuab] d=4
[4908958932/vjuab, 5453444626/kfisc, 3556224529/zakoc, 6756314515/nlkgc, 5274043219/hsglz, 8891229348/oaksp, 9794922582/igjkc] d=3
[5274043219/hsglz, 5453444626/kfisc, 3556224529/zakoc, 6756314515/nlkgc, 9794922582/igjkc, 8891229348/oaksp, 4908958932/vjuab] d=2
[3556224529/zakoc, 4908958932/vjuab, 5274043219/hsglz, 5453444626/kfisc, 6756314515/nlkgc, 8891229348/oaksp, 9794922582/igjkc] d=1
```

### 3.4 — Heap sort (full)

```powershell
.\heap_sort.exe dataset_1000.csv
```
```
Sorted 1000 rows in 0.0002523 seconds.
Output: heap_sorted_dataset_1000.csv
```

> Heap sort writes to the **current folder** (no `result\` subfolder).

```powershell
Get-Content heap_sorted_dataset_1000.csv -TotalCount 3
```
```
1011011326,vpcbc
1013237162,aoway
1013987915,nearp
```

### 3.5 — Heap sort step trace (rows 1–7)

```powershell
.\heap_sort_step.exe dataset_1000.csv 1 7
```
```
Wrote step trace to dataset_1000_heap_sorted_step_1_7.txt
```

The trace shows `initial` (the built max-heap), then one line per extract-max step
`i = 6` down to `i = 1`:

```powershell
Get-Content dataset_1000_heap_sorted_step_1_7.txt
```
```
[9794922582/igjkc, 8891229348/oaksp, 6756314515/nlkgc, 5274043219/hsglz, 3556224529/zakoc, 5453444626/kfisc, 4908958932/vjuab] initial
[8891229348/oaksp, 5274043219/hsglz, 6756314515/nlkgc, 4908958932/vjuab, 3556224529/zakoc, 5453444626/kfisc, 9794922582/igjkc] i = 6
[6756314515/nlkgc, 5274043219/hsglz, 5453444626/kfisc, 4908958932/vjuab, 3556224529/zakoc, 8891229348/oaksp, 9794922582/igjkc] i = 5
[5453444626/kfisc, 5274043219/hsglz, 3556224529/zakoc, 4908958932/vjuab, 6756314515/nlkgc, 8891229348/oaksp, 9794922582/igjkc] i = 4
[5274043219/hsglz, 4908958932/vjuab, 3556224529/zakoc, 5453444626/kfisc, 6756314515/nlkgc, 8891229348/oaksp, 9794922582/igjkc] i = 3
[4908958932/vjuab, 3556224529/zakoc, 5274043219/hsglz, 5453444626/kfisc, 6756314515/nlkgc, 8891229348/oaksp, 9794922582/igjkc] i = 2
[3556224529/zakoc, 4908958932/vjuab, 5274043219/hsglz, 5453444626/kfisc, 6756314515/nlkgc, 8891229348/oaksp, 9794922582/igjkc] i = 1
```

### 3.6 — Hash table search (full — best / average / worst)

Runs n searches per case and times only the search loop (table build + file I/O excluded).

```powershell
.\hash_table_search.exe dataset_1000.csv
```
```
Best case time: 0.000000 seconds
Average case time: 0.000008 seconds
Worst case time: 0.000055 seconds
(Results written to hash_table_search_dataset_1000.txt)
```

> At `n = 1000` the best case is often so fast it prints `0.000000 seconds` — that is
> normal (too quick to measure), not an error. It becomes non-zero at larger sizes.

The same three lines are saved to the `.txt`:

```powershell
Get-Content hash_table_search_dataset_1000.txt
```

### 3.7 — Hash table search step (probe trace)

Two hardcoded demo targets: one that **exists** in `dataset_1000.csv`
(`9794922582` → `igjkc`, its first row) and one that does **not** (`123456789`).

```powershell
.\hash_table_search_step.exe dataset_1000.csv
```
```
9794922582 = 9794922582/igjkc
(Written to dataset_1000_hash_table_search_step_9794922582.txt)
-1 != 123456789
(Written to dataset_1000_hash_table_search_step_123456789.txt)
```

> The "found" target only exists in `dataset_1000.csv`. On any other size the first line
> becomes `-1 != 9794922582`. To trace a hit on another size, edit the two constants near
> the top of `sorting/hash_table_search_step.cpp` and recompile.

---

## 4. Run across all 10 sizes (fill in your own timings)

Only the **full** programs are timed; the `*_step` programs are correctness traces, not
timings. This one loop runs all 10 sizes in order — generate, then radix / heap / hash:

```powershell
foreach ($n in 1000, 10000, 100000, 500000, 1000000, 5000000, 10000000, 50000000, 100000000, 150000000) {
    Write-Host "================  n = $n  ================"
    .\dataset_generator.exe  $n
    .\radix_sort.exe         "dataset_$n.csv"
    .\heap_sort.exe          "dataset_$n.csv"
    .\hash_table_search.exe  "dataset_$n.csv"
}
```

Each size prints the same blocks as Section 3 (the `Wrote …`, `Sorted … in … seconds.`,
`Output: …`, and the three hash time lines).

> ⚠️ Sizes 6–10 are large CSVs (~80 MB → ~2.4 GB; n=150M needs ~7–8 GB RAM and the file
> belongs on OneDrive, not in git). If your machine can't hold them, trim the size list
> in the loop above to where it's comfortable.

Real measured numbers from one run (MSYS2 g++ 14.2, `-O2`). Rows 1–7 are actual output;
rows 8–10 (multi-GB, up to ~7–8 GB RAM) were not run here — fill them from your own machine.
**Your numbers will differ; the growth pattern is what matters.**

| n           | Radix (s) | Heap (s) | Hash best (s) | Hash avg (s) | Hash worst (s) |
|-------------|-----------|----------|---------------|--------------|----------------|
| 1,000       | 0.0000801 | 0.0002523| 0.000000      | 0.000008     | 0.000055       |
| 10,000      | 0.0007917 | 0.0035267| 0.000003      | 0.000095     | 0.000474       |
| 100,000     | 0.0078918 | 0.0451257| 0.000023      | 0.000957     | 0.006485       |
| 500,000     | 0.0434713 | 0.305252 | 0.000122      | 0.005643     | 0.047039       |
| 1,000,000   | 0.0900741 | 0.740306 | 0.000228      | 0.014060     | 0.190397       |
| 5,000,000   | 0.462651  | 4.44436  | 0.001156      | 0.095817     | 0.919049       |
| 10,000,000  | 0.914481  | 9.80834  | 0.002508      | 0.192881     | 1.806129       |
| 50,000,000  |           |          |               |              |                |
| 100,000,000 |           |          |               |              |                |
| 150,000,000 |           |          |               |              |                |

---

## 5. What each command produces

| Command | Reads | Writes |
|---|---|---|
| `.\dataset_generator.exe <n>` | — | `dataset_<n>.csv` (cwd) |
| `.\radix_sort.exe dataset_<n>.csv` | `dataset_<n>.csv` | `result\radix_sorted_dataset_<n>.csv` |
| `.\radix_sort_step.exe dataset_<n>.csv 1 7` | `dataset_<n>.csv` | `result\dataset_<n>_radix_sorted_step_1_7.txt` |
| `.\heap_sort.exe dataset_<n>.csv` | `dataset_<n>.csv` | `heap_sorted_dataset_<n>.csv` (cwd) |
| `.\heap_sort_step.exe dataset_<n>.csv 1 7` | `dataset_<n>.csv` | `dataset_<n>_heap_sorted_step_1_7.txt` (cwd) |
| `.\hash_table_search.exe dataset_<n>.csv` | `dataset_<n>.csv` | `hash_table_search_dataset_<n>.txt` (cwd) |
| `.\hash_table_search_step.exe dataset_<n>.csv` | `dataset_<n>.csv` | `dataset_<n>_hash_table_search_step_<target>.txt` ×2 (cwd) |

All four sort/search programs take the dataset path as a command-line argument. Timing in
the three full programs excludes file I/O — only the algorithm call is wrapped in
`chrono::high_resolution_clock`.

---

## 6. Cleanup (optional)

Removes generated results and compiled binaries; keeps the `.cpp` sources.

```powershell
Remove-Item result -Recurse -Force -ErrorAction SilentlyContinue
Remove-Item *.exe, dataset_*.csv, heap_sorted_*.csv, `
            *_step_*.txt, hash_table_search_*.txt -ErrorAction SilentlyContinue
```
