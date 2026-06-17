# Demo Runbook — All 7 Programs, All 10 Dataset Sizes (Windows / PowerShell)

A copy-paste script for the live demo. Two parts:

- **Part A** — a detailed walkthrough on `dataset_1000.csv`, with the **expected console
  output** under every command, so you can show what each program does and confirm it on
  the spot.
- **Part B** — the same timed programs run across **all 10 dataset sizes** (1K → 200M) to
  show how the running time scales, with a results table to fill in.

> **Run everything from inside `src\`.** All programs read/write files relative to the
> current directory, so staying in one folder keeps the generated files together.
> ```powershell
> cd src
> ```

> **Shell note:** commands below are for **Windows PowerShell**. Programs are invoked as
> `.\program.exe` (e.g. `.\radix_sort.exe dataset_1000.csv`). For the Git Bash / WSL /
> macOS / Linux versions (`./program`, `head`, `cat`, `for`), see the git history of this
> file.

---

## 0. Compile all 7 programs

`-std=c++17` is mandatory (the radix programs use `<filesystem>`).
`-O2` is mandatory (without it the timing numbers are meaningless).

```powershell
g++ -std=c++17 -O2 dataset_generator.cpp      -o dataset_generator
g++ -std=c++17 -O2 radix_sort.cpp             -o radix_sort
g++ -std=c++17 -O2 radix_sort_step.cpp        -o radix_sort_step
g++ -std=c++17 -O2 heap_sort.cpp              -o heap_sort
g++ -std=c++17 -O2 heap_sort_step.cpp         -o heap_sort_step
g++ -std=c++17 -O2 hash_table_search.cpp      -o hash_table_search
g++ -std=c++17 -O2 hash_table_search_step.cpp -o hash_table_search_step
```

No output = success. (On Windows, g++ produces `program.exe` for each.)

---

## 1. The 10 datasets

All 10 sizes are pre-generated in `src\` (they are git-ignored). The RNG is seeded with
the group leader's mapped ID (`2511325052`), so every size is reproducible on any machine.

| # | Size (n) | CSV file | ~CSV size | Where it lives |
|---|---|---|---|---|
| 1 | 1,000 | `dataset_1000.csv` | ~16 KB | git (sample) |
| 2 | 10,000 | `dataset_10000.csv` | ~160 KB | git |
| 3 | 100,000 | `dataset_100000.csv` | ~1.6 MB | git |
| 4 | 500,000 | `dataset_500000.csv` | ~8 MB | git |
| 5 | 1,000,000 | `dataset_1000000.csv` | ~16 MB | git |
| 6 | 5,000,000 | `dataset_5000000.csv` | ~80 MB | **OneDrive** (near the 85 MB line) |
| 7 | 10,000,000 | `dataset_10000000.csv` | ~160 MB | OneDrive |
| 8 | 50,000,000 | `dataset_50000000.csv` | ~800 MB | OneDrive |
| 9 | 100,000,000 | `dataset_100000000.csv` | ~1.6 GB | OneDrive |
| 10 | 200,000,000 | `dataset_200000000.csv` | ~3.2 GB | OneDrive |

To (re)generate any size: `.\dataset_generator.exe <n>` → `dataset_<n>.csv` in the current
folder. For example:
```powershell
.\dataset_generator.exe 1000
```
```
Wrote 1000 rows to dataset_1000.csv
```

To regenerate them all:
```powershell
foreach ($n in 1000,10000,100000,500000,1000000,5000000,10000000,50000000,100000000,200000000) {
  .\dataset_generator.exe $n
}
```
```
Wrote 1000 rows to dataset_1000.csv
Wrote 10000 rows to dataset_10000.csv
... (one line per size) ...
Wrote 200000000 rows to dataset_200000000.csv
```

> ⚠️ **RAM note (sizes 9–10).** Peak memory ≈ **5–6 GB at n=100M**, **10–11 GB at n=200M**.
> Generate the giant sizes on the machine with the most RAM, one at a time. The 200M file
> is ~3.2 GB on disk — keep it on OneDrive, not in the submission zip (99 MB cap).

---

# PART A — Walkthrough on `dataset_1000.csv`

The first row of `dataset_1000.csv` is `9794922582,igjkc` — that's the key the hash search
finds in step A3.

## A1. Radix sort (LSD, base 10)

### Full sort + timing
```powershell
.\radix_sort.exe dataset_1000.csv
```
```
Sorted 1000 rows in 0.000126 seconds.
Output: result/radix_sorted_dataset_1000.csv
```
Show the result is ascending by key:
```powershell
Get-Content result\radix_sorted_dataset_1000.csv -TotalCount 3
```
```
1011011326,vpcbc
1013237162,aoway
1013987915,nearp
```
> Radix is the only program that writes its sorted CSV into a `result\` subfolder (it
> creates it automatically). The others write to the current directory.

### Step trace (rows 1–7)
```powershell
.\radix_sort_step.exe dataset_1000.csv 1 7
```
```
Wrote step trace to result/dataset_1000_radix_sorted_step_1_7.txt
```
Show the trace — `original`, then one line per digit pass `d=10` (ones digit) down to
`d=1` (10^9 digit):
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

## A2. Heap sort (max-heap, in-place)

### Full sort + timing
```powershell
.\heap_sort.exe dataset_1000.csv
```
```
Sorted 1000 rows in 0.000268 seconds.
Output: heap_sorted_dataset_1000.csv
```
```powershell
Get-Content heap_sorted_dataset_1000.csv -TotalCount 3
```
```
1011011326,vpcbc
1013237162,aoway
1013987915,nearp
```

### Step trace (rows 1–7)
```powershell
.\heap_sort_step.exe dataset_1000.csv 1 7
```
```
Wrote step trace to dataset_1000_heap_sorted_step_1_7.txt
```
Show the trace — `initial` (the built max-heap), then one line per extract-max step
`i = 6` down to `i = 1`:
```powershell
Get-Content dataset_1000_heap_sorted_step_1_7.txt
```
```
[9794922582/igjkc, 8891229348/oaksp, 6756314515/nlkgc, 5274043219/hsglz, 3556224529/zakoc, 5453444626/kfisc, 4908958932/vjuab] initial
[8891229348/oaksp, 5274043219/hsglz, 6756314515/nlkgc, 4908958932/vjuab, 3556224529/zakoc, 5453444626/kfisc, 9794922582/igjkc] i = 6
...
[3556224529/zakoc, 4908958932/vjuab, 5274043219/hsglz, 5453444626/kfisc, 6756314515/nlkgc, 8891229348/oaksp, 9794922582/igjkc] i = 1
```

## A3. Hash table search (linear probing)

### Timing — best / average / worst case
Runs n searches per case and times only the search loop (table build + I/O excluded).
```powershell
.\hash_table_search.exe dataset_1000.csv
```
```
Best case time: 0.000000 seconds
Average case time: 0.000009 seconds
Worst case time: 0.000059 seconds
(Results written to hash_table_search_dataset_1000.txt)
```

### Step trace — one found target, one not-found target
The two targets are hardcoded near the top of `hash_table_search_step.cpp`:
`FOUND_TARGET = 9794922582` (row 1 of the leader-seed `dataset_1000.csv`) and
`NOT_FOUND_TARGET = 123456789` (only 9 digits — can't exist).
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
> becomes `-1 != 9794922582` (different first row). For the found case on another size,
> edit the two constants in the `.cpp` and recompile.

---

# PART B — Run the 3 timed programs across all 10 sizes

This produces the running-time numbers for the report. Only the **full** programs are
timed (`radix_sort`, `heap_sort`, `hash_table_search`); the `*_step` programs are
correctness traces, not timings, and must not be run on the big sizes.

### Sizes 1–5 (safe to loop locally)
```powershell
foreach ($n in 1000,10000,100000,500000,1000000) {
  Write-Host "================  n = $n  ================"
  Write-Host "-- radix --"; .\radix_sort.exe        "dataset_$n.csv"
  Write-Host "-- heap  --"; .\heap_sort.exe         "dataset_$n.csv"
  Write-Host "-- hash  --"; .\hash_table_search.exe "dataset_$n.csv"
}
```

Output you should see (the times will differ on your machine; the format is what matters):
```
================  n = 1000  ================
-- radix --
Sorted 1000 rows in 9.72e-05 seconds.
Output: result/radix_sorted_dataset_1000.csv
-- heap  --
Sorted 1000 rows in 0.0002567 seconds.
Output: heap_sorted_dataset_1000.csv
-- hash  --
Best case time: 0.000000 seconds
Average case time: 0.000009 seconds
Worst case time: 0.000059 seconds
(Results written to hash_table_search_dataset_1000.txt)
================  n = 10000  ================
...
```

### Sizes 6–10 (run one at a time — don't loop the multi-GB files)
```powershell
$n = 5000000      # then repeat with 10000000, 50000000, 100000000, 200000000
.\radix_sort.exe        "dataset_$n.csv"
.\heap_sort.exe         "dataset_$n.csv"
.\hash_table_search.exe "dataset_$n.csv"
```

> Run each size 2–3 times and take the median — the first run is often slower (cold
> cache) and the small sizes have scheduling jitter.

### Reference results (seconds)

Measured on one machine (MSYS2 g++ 14.2, `-O2`, 32 GB RAM). **Your numbers will differ** —
record your own — but the growth pattern should match.

| n | Radix | Heap | Hash (best) | Hash (avg) | Hash (worst) |
|---|---|---|---|---|---|
| 1,000 | 0.0000972 | 0.000257 | 0.000000 | 0.000009 | 0.000059 |
| 10,000 | 0.000841 | 0.003538 | 0.000003 | 0.000093 | 0.000474 |
| 100,000 | 0.008423 | 0.047811 | 0.000025 | 0.001010 | 0.007016 |
| 500,000 | 0.044857 | 0.338216 | 0.000114 | 0.006310 | 0.048231 |
| 1,000,000 | 0.089700 | 0.735983 | 0.000247 | 0.015668 | 0.185847 |
| 5,000,000 | 0.476268 | 4.81446 | 0.002097 | 0.150098 | 1.502132 |
| 10,000,000 | 1.60138 | 12.5083 | 0.002291 | 0.200537 | 1.858689 |
| 50,000,000 | 4.3816 | 61.6423 | 0.027363 | 1.197116 | 12.199964 |
| 100,000,000 | 9.17507 | 135.257 | 0.023358 | 2.068495 | 30.859477 |
| 200,000,000 | 20.2878 | 303.808 | 0.047580 | 4.611313 | 49.829994 |

The shapes match the theory: radix ≈ O(n) (10 fixed digit passes — note it's ~20s even at
200M), heap ≈ O(n log n) (the steepest curve — 5 min at 200M), hash average ≈ O(1) per
lookup while the worst case grows with the longest probe run.

---

## Cheat sheet — what each command produces

| Command | Reads | Writes |
|---|---|---|
| `.\dataset_generator.exe <n>` | — | `dataset_<n>.csv` (cwd) |
| `.\radix_sort.exe dataset_<n>.csv` | `dataset_<n>.csv` | `result\radix_sorted_dataset_<n>.csv` |
| `.\radix_sort_step.exe dataset_<n>.csv 1 7` | `dataset_<n>.csv` | `result\dataset_<n>_radix_sorted_step_1_7.txt` |
| `.\heap_sort.exe dataset_<n>.csv` | `dataset_<n>.csv` | `heap_sorted_dataset_<n>.csv` (cwd) |
| `.\heap_sort_step.exe dataset_<n>.csv 1 7` | `dataset_<n>.csv` | `dataset_<n>_heap_sorted_step_1_7.txt` (cwd) |
| `.\hash_table_search.exe dataset_<n>.csv` | `dataset_<n>.csv` | `hash_table_search_dataset_<n>.txt` (cwd) |
| `.\hash_table_search_step.exe dataset_<n>.csv` | `dataset_<n>.csv` | `dataset_<n>_hash_table_search_step_<target>.txt` ×2 (cwd) |

All four sort/search programs take the dataset path as a command-line argument and run on
any size. Timing in all three full programs excludes file I/O — only the algorithm call
is wrapped in `chrono::high_resolution_clock`.

---

## Cleanup (after the demo)

Removes generated results and compiled binaries; keeps the `.cpp` sources and the
pre-generated `dataset_*.csv` inputs.

```powershell
Remove-Item -Recurse -Force result -ErrorAction SilentlyContinue
Remove-Item -Force heap_sorted_dataset_*.csv, `
                   hash_table_search_dataset_*.txt, dataset_*_step_*.txt, `
                   dataset_generator.exe, radix_sort.exe, radix_sort_step.exe, `
                   heap_sort.exe, heap_sort_step.exe, hash_table_search.exe, `
                   hash_table_search_step.exe -ErrorAction SilentlyContinue
```

> To also remove the datasets, add `dataset_*.csv` to the list — but the large ones take a
> while to regenerate.
