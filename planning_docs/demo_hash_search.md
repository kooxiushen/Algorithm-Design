# Demo Guide — Hash Table Search (Member 3: KOO XIU SHEN)

Step-by-step commands to run during the 30-minute demo for the **Hash Table
Search** part of the assignment. Covers Demo **Step 4** (`hash_table_search_step`)
and Demo **Step 7** (`hash_table_search`) from the assignment PDF.

> All commands are PowerShell, run from the repo root:
> `C:\Users\Dell\Documents\GitHub\Algorithm-Design`

---

## 0. One-time prerequisites (do this before the demo)

**Check the compiler is available:**
```powershell
g++ --version
```
Expected: `g++.exe (Rev3, Built by MSYS2 project) 14.2.0`
If "not recognized": the g++ folder isn't on PATH. Use the full path instead:
`C:\msys64\mingw64\bin\g++.exe` in place of `g++` below.

**Check the datasets exist** (they live in `data\`, gitignored):
```powershell
Get-ChildItem data\dataset_*.csv | Select-Object Name, Length
```
If `dataset_1000.csv` is missing, regenerate it (Step 1 below).

---

## Step 1 — `dataset_generator` (only if a dataset is missing)

The tutor may ask you to generate a dataset live.

```powershell
cd data
g++ -std=c++17 -O2 ..\src\dataset_generator.cpp -o dataset_generator.exe
.\dataset_generator.exe 1000
```
**Output:** `dataset_1000.csv` (1,000 unique rows of `<10-digit int>,<5-letter string>`)

> Seed is the group leader's ID `251UC25052` → `2511325052`, hard-coded after
> `main`, so the dataset is identical every time.

---

## Step 4 — `hash_table_search_step` (proves search correctness)

Searches for **one key that exists** and **one that doesn't**, on `dataset_1000.csv`.

### 4a. The tutor picks the two targets
Open `src\hash_table_search_step.cpp` and edit these two lines near the top:
```cpp
const long long FOUND_TARGET     = 9794922582LL;   // a key that EXISTS
const long long NOT_FOUND_TARGET = 123456789LL;    // a key that does NOT exist
```
- To find a key that exists, show the tutor the file: `Get-Content data\dataset_1000.csv -TotalCount 5` and pick any number from it.
- A number with fewer than 10 digits (e.g. `123456789`) is always "not found".

### 4b. Compile and run
```powershell
cd data
g++ -std=c++17 -O2 ..\src\hash_table_search_step.cpp -o hash_table_search_step.exe
.\hash_table_search_step.exe dataset_1000.csv
```

### 4c. Expected output
```
9794922582 = 9794922582/igjkc
-1 != 123456789
```
Produces two trace files in `data\`:
- `dataset_1000_hash_table_search_step_9794922582.txt`  → `9794922582 = 9794922582/igjkc`
- `dataset_1000_hash_table_search_step_123456789.txt`   → `-1 != 123456789`

**Format meaning:**
- Found → `<target> = <key>/<string>`
- Not found → `-1 != <target>`

---

## Step 7 — `hash_table_search` (measures running time)

Performs **n searches** (a single search is too fast to time) and reports the
running time for **best, average, and worst** cases. Demo size is usually
`dataset_1000000.csv`, but the tutor may pick another.

### 7a. Compile and run
```powershell
cd data
g++ -std=c++17 -O2 ..\src\hash_table_search.cpp -o hash_table_search.exe
.\hash_table_search.exe dataset_1000000.csv
```

### 7b. Expected output (1,000,000 rows on this machine)
```
Best case time: 0.000227 seconds
Average case time: 0.016838 seconds
Worst case time: 0.175743 seconds
(Results written to hash_table_search_dataset_1000000.txt)
```
Also writes `hash_table_search_dataset_1000000.txt` with the three times.

### 7c. Run other sizes if asked
```powershell
.\hash_table_search.exe dataset_1000.csv
.\hash_table_search.exe dataset_10000000.csv
.\hash_table_search.exe dataset_200000000.csv
```

---

## Full experiment results (this machine: i7-12700H, 32 GB RAM)

| Input size (n) | Best (s) | Average (s) | Worst (s) |
|---:|---:|---:|---:|
| 1,000 | 0.000001 | 0.000021 | 0.000135 |
| 10,000 | 0.000005 | 0.000103 | 0.000599 |
| 100,000 | 0.000066 | 0.001518 | 0.007450 |
| 500,000 | 0.000129 | 0.005502 | 0.048302 |
| 1,000,000 | 0.000227 | 0.016838 | 0.175743 |
| 5,000,000 | 0.001477 | 0.098825 | 0.869204 |
| 10,000,000 | 0.002309 | 0.203365 | 1.757240 |
| 50,000,000 | 0.011062 | 1.024331 | 10.903397 |
| 100,000,000 | 0.022170 | 2.024132 | 27.914126 |
| 200,000,000 | 0.048269 | 4.508573 | 46.324900 |

---

## Q&A prep — likely tutor questions

**Q: What collision resolution do you use?**
Open addressing with **linear probing**. On collision, walk forward one slot at a
time (`pos = (pos + 1) % SIZE`) until the key or an empty slot is found.

**Q: What is your hash function?**
`key % SIZE`, where `SIZE` is the next prime ≥ `2n` (load factor ~0.5 keeps
probe chains short and spreads keys evenly).

**Q: Time complexity?**
- Best case: **O(1)** — key sits in its home slot, one comparison.
- Average case: **O(1)** at load factor ~0.5 (≈ `1/(1-α)` probes).
- Worst case: **O(n)** — one long probe cluster.
- Space: **O(table size)** = O(n).

**Q: How do you define best / average / worst in the timing program?**
- Best = search a key with the shortest probe distance (home slot), n times.
- Average = search every key in the dataset once.
- Worst = search the key with the longest probe distance, n times.

**Q: Why is the worst-case curve steeper than linear?**
Linear probing causes **primary clustering** — as the table fills, the longest
cluster grows, so the worst-case probe length per search increases with n. Doing
n such searches makes the total grow faster than linear.

**Q: Why is there a `volatile long long sink` in the timing loops?**
With `-O2`, the compiler would delete the search loops because their results look
unused, making the timer read ~0. Accumulating into a `volatile` variable forces
the searches to actually execute, so the timing is real.

**Q: Why doesn't `hash_table_search` store the 5-letter strings?**
Search is by the integer key only, and the timing output never prints strings.
Dropping them cuts memory from ~120 to ~24 bytes/row, so the 200-million-row
dataset fits in ~4.8 GB. The `_step` program DOES keep strings because it must
print the matched record.

**Q: Which STL did you avoid?**
No `std::unordered_map/set`, `std::map/set`, `std::sort`, `std::find`,
`std::binary_search` — the hash table is hand-written.

---

## Pre-demo checklist
- [ ] `g++ --version` works (or full path ready)
- [ ] `data\dataset_1000.csv` and `data\dataset_1000000.csv` present
- [ ] All three `.cpp` files compile with no warnings (`-Wall`)
- [ ] Can change the two targets in `hash_table_search_step.cpp` on request
- [ ] Hardware spec screenshot (`msinfo32`) ready
- [ ] 10 command-prompt screenshots of `hash_table_search` saved
- [ ] Two `_step` trace files saved
