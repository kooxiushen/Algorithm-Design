# Code Explanation — CCP6214 Group Assignment

This document is organized to match the **report's question format**. Each question below uses the
same three blocks as the report:

- **Question tasks:** — what the program is meant to do
- **Screenshots, inputs, outputs, figures, explanations, tables, charts:** — the run artifacts you
  insert from your own machine (this file lists *what* to insert and suggested captions; the actual
  images/tables live in the report)
- **Code parts, explanations:** — ready-to-paste code explanation, with snippets copied **verbatim**
  from the real `.cpp` files (only `// ←` pointer comments added), the time/space complexity, and the
  design decisions

Mapping to the seven programs:

| Report question | Programs covered |
|---|---|
| **Q1** Dataset generation | `dataset_generator` |
| **Q2** Radix sort | `radix_sort` + `radix_sort_step` |
| **Q3** Heap sort | `heap_sort` + `heap_sort_step` |
| **Q4** Hash table search | `hash_table_search` + `hash_table_search_step` |

Q5 (Conclusion / AVL comparison), Q6 (Document completeness) and Q7 (Experiments) are **not code
sections** — see the pointer at the end of this file.

---

## Overview — shared conventions (read once)

**Conventions across all seven files:**

- Each `.cpp` is **self-contained** — no shared headers, no `common/` folder. Small bits of
  CSV-reading code are duplicated on purpose so every file compiles and is defended in isolation.
- `using namespace std;` is used in every file (no `std::` prefixes).
- A course/member header block sits at the top of every file (required by PDF §F.1.a).
- `long long` is used for `n`, positions, and keys because plain `int` overflows at the large
  input sizes the assignment expects.
- **Timing excludes I/O.** Where a program is timed, only the algorithm call is wrapped with
  `std::chrono::high_resolution_clock` — the CSV is read before the timer starts and the output is
  written after it stops (PDF §C.7.a).

**Compile (each program is standalone):**

```bash
g++ -std=c++17 -O2 src/dataset/dataset_generator.cpp -o dataset_generator
g++ -std=c++17 -O2 src/sorting/radix_sort.cpp        -o radix_sort
# ...same pattern for the other five
```

`-std=c++17` is **required** because the two radix programs use `<filesystem>`. `-O2` is required
so the experiment timings are realistic.

> **One real inconsistency to state honestly in the demo:** only the radix lane writes its output
> into a `result/` subfolder. `dataset_generator`, both heap programs, and both hash programs write
> to the **current working directory**. The lanes were written separately and this convention was
> never unified.

---

## Q1 [5] Dataset generation (group)

### Question tasks:
> Implement a dataset generator that produces `dataset_<n>.csv` with `n` **unique, random, positive
> 10-digit integers** (1,000,000,000–9,999,999,999), each paired with a random 5-letter lowercase
> string, written in random order. The random number generator is seeded with the group leader's
> mapped student ID (`2511325052`) so the dataset is reproducible and differs from other groups.

### Screenshots, inputs, outputs, figures, explanations, tables, charts:
- *[INSERT]* Screenshot or sample of `dataset_1000.csv` (first ~7 rows) — *Caption: "Sample of the
  generated dataset: unique 10-digit integer + 5-letter string per row."*
- *[INSERT]* Terminal screenshot showing `Wrote <n> rows to dataset_<n>.csv`.
- Explanation points to state: integers are **unique**, **randomized in order** (not sorted), each
  is a **10-digit** value in range, paired with a 5-letter lowercase string.
- *(No timing table — generation is not a timed experiment.)*

### Code parts, explanations:

**What it does.** Generates `dataset_<n>.csv` with `n` unique random rows, seeded with the leader's
mapped ID so the file is reproducible.

**Input / output contract:**
- **Input:** one CLI argument `<n>` (a positive `long long`).
- **Output:** `dataset_<n>.csv` in the **current directory**.
- **Console:** `Wrote <n> rows to dataset_<n>.csv`. No timer.

**Key data structures:**
- `vector<long long> keys`, `vector<string> strs` — output rows in two parallel vectors.
- `vector<long long> seen(2 * n + 1, 0)` — a **hand-rolled hash set** for duplicate detection,
  sized ~`2n` (load factor ~0.5); value `0` marks an empty slot (safe — real keys are ≥ 1e9).

**Algorithm walkthrough:**
1. Read `n` from `argv`; reject `n ≤ 0`.
2. Seed `mt19937_64` with `2511325052ULL`.
3. Repeat until `n` rows exist: make a random 10-digit `k`; linear-probe `seen` to reject
   duplicates; build a random 5-letter string; append both.
4. Write all rows to the CSV.

**Annotated snippets.** The seed (every member must use this exact value):

```cpp
// Seed = group leader's student ID after the letter -> digit mapping.
// Original ID: 251UC25052
//   2=2 5=5 1=1 U=1 C=3 2=2 5=5 0=0 5=5 2=2  ->  2511325052
mt19937_64 rng(2511325052ULL);          // ← deterministic, reproducible dataset
```

The uniqueness check by linear probing (this replaces `std::unordered_set`):

```cpp
vector<long long> seen(2 * n + 1, 0);   // ← hand-rolled set; 0 == empty slot

while ((long long)keys.size() < n)
{
    // Make a random 10-digit number in [1,000,000,000 .. 9,999,999,999].
    long long k = 1000000000LL + (rng() % 9000000000LL);

    long long pos = k % (long long)seen.size();
    bool duplicate = false;
    while (seen[pos] != 0)              // ← walk forward while slots are occupied
    {
        if (seen[pos] == k) { duplicate = true; break; }   // ← already generated
        pos = (pos + 1) % (long long)seen.size();          // ← linear probe, wraps
    }
    if (duplicate)
        continue;                       // ← discard and retry with a new k
    seen[pos] = k;                      // ← mark this number as taken

    string s = "";
    for (int i = 0; i < 5; i++)
        s += (char)('a' + rng() % 26);  // ← random lowercase letter

    keys.push_back(k);
    strs.push_back(s);
}
```

**Time & space complexity:**
- **Time:** O(n) expected — each number is generated and inserted in expected O(1) (load factor
  ~0.5 keeps the probe short); duplicates are rare across a 9-billion-wide range.
- **Space:** O(n) — the `seen` table plus the two output vectors.

**Design decisions:**
- **Why hand-roll instead of `std::unordered_set`?** PDF §C.4 forbids library containers that
  search internally; `unordered_set::insert` does. The `vector` + linear probe does the same job
  without a banned container.
- **Why size `2 * n + 1`?** Load factor ~0.5 keeps the expected probe length to ~1–2 slots.
- **Why `long long`?** `int` overflows around 2.1 × 10⁹, inside the stated maximum input range.

---

## Q2 [5] Radix sort (group)

### Question tasks:
> Implement **LSD radix sort** (base 10, processing from the rightmost digit) that sorts the
> records by their 10-digit integer key, write the fully sorted CSV, and print the running time.
> The `radix_sort_step` variant traces the array after each digit pass so correctness can be
> verified byte-for-byte against the PDF sample.

### Screenshots, inputs, outputs, figures, explanations, tables, charts:
- *[INSERT]* Input sample — first rows of `dataset_1000.csv`.
- *[INSERT]* Output sample — first rows of `radix_sorted_dataset_1000.csv` (ascending by key).
- *[INSERT]* Step-trace screenshot — `dataset_1000_radix_sorted_step_1_7.txt` (passes `d=10` … `d=1`).
- *[INSERT]* **Radix running-time table** across all 10 input sizes.
- *[INSERT]* **Chart** — Radix runtime vs `n` (or Radix vs Heap overlaid; use a log-scale x-axis).
- *[INSERT]* Terminal running-time screenshot (`Sorted <n> rows in <seconds> seconds.`).

### Code parts, explanations:

**What it does.** LSD radix sort, base 10, exactly 10 passes (one per digit), sorting records
ascending by the integer key; the 5-letter string rides along as payload. Only the sort is timed.

**Input / output contract:**
- **Input:** `<input_csv>`.
- **Output:** `result/radix_sorted_<input_basename>` (the **`result/` subfolder** is auto-created —
  the only lane that uses one).
- **Console:** `Sorted <n> rows in <seconds> seconds.` then `Output: <path>`.

**Key data structures:**
- `vector<long long> keys`, `vector<string> strs` — input rows.
- `vector<long long> tmp_keys(n)`, `vector<string> tmp_strs(n)` — reused output buffers.
- `long long count[10]` — counting array, one slot per digit 0–9 (base 10).

**Algorithm walkthrough:**
1. Read CSV (before the timer).
2. Start timer.
3. For each of the 10 digit positions (least significant first): count digit frequencies →
   prefix-sum → place records **right-to-left** into buckets (keeps stability) → swap buffers,
   advance the divisor ×10.
4. Stop timer; write `result/radix_sorted_<basename>`.

**Annotated snippets.** The timed sort (timer starts *after* the file read):

```cpp
auto t_start = chrono::high_resolution_clock::now();   // ← timer starts AFTER file read

long long divisor = 1;
for (int pass = 0; pass < 10; pass++)
{
    // Count how many keys have each digit (0..9) at this position.
    for (int i = 0; i < 10; i++) count[i] = 0;
    for (long long i = 0; i < n; i++)
    {
        int d = (int)((keys[i] / divisor) % 10);
        count[d]++;
    }
    // Prefix sum -> ending positions of each bucket.
    for (int i = 1; i < 10; i++) count[i] += count[i - 1];
    // Stable distribution: walk right-to-left, place each into its bucket.
    for (long long i = n - 1; i >= 0; i--)             // ← right-to-left = STABLE
    {
        int d = (int)((keys[i] / divisor) % 10);
        count[d]--;
        tmp_keys[count[d]] = keys[i];
        tmp_strs[count[d]] = strs[i];
    }
    // Vector swap is O(1) (just swaps internal pointers).
    swap(keys, tmp_keys);                              // ← O(1), no element copying
    swap(strs, tmp_strs);
    divisor *= 10;                                     // ← advance to the next digit
}

auto t_end = chrono::high_resolution_clock::now();
double seconds = chrono::duration<double>(t_end - t_start).count();
```

The output path in a `result/` subfolder via `<filesystem>`:

```cpp
filesystem::create_directories("result");             // ← needs -std=c++17
size_t slash = in_name.find_last_of("/\\");
string base = (slash == string::npos) ? in_name : in_name.substr(slash + 1);
string out_name = "result/radix_sorted_" + base;
```

**`radix_sort_step` (correctness variant).** Same sort, but on a row slice `[start, end]`, writing
the array state after each pass. Usage: `radix_sort_step <input_csv> <start_row> <end_row>`; output
`result/<basename>_radix_sorted_step_<s>_<e>.txt`. The passes are labelled `d=10` (first pass, ones
digit) down to `d=1` to match the PDF sample:

```cpp
long long divisor = 1;
for (int d_label = 10; d_label >= 1; d_label--)        // ← label 10 (ones) -> 1 (10^9)
{
    // ...identical count / prefix-sum / right-to-left distribution as radix_sort...
    swap(keys, tmp_keys);
    swap(strs, tmp_strs);
    divisor *= 10;
    print_state(out, keys, strs, "d=" + to_string(d_label));   // ← trace this pass
}
```

```cpp
void print_state(ofstream &out, const vector<long long> &keys,
                 const vector<string> &strs, const string &label)
{
    out << "[";
    for (size_t i = 0; i < keys.size(); i++)
    {
        if (i > 0) out << ", ";
        out << keys[i] << "/" << strs[i];      // ← "<key>/<string>"
    }
    out << "] " << label << "\n";              // ← "[...] d=9"
}
```

**Time & space complexity:**
- **Time:** O(d · (n + b)) with `d = 10`, `b = 10` constants → effectively **O(n)**.
- **Space:** O(n + b) → **O(n)** (temp buffers size `n`, `count` size 10).
- *(The `_step` variant is the same on the chosen slice of `m` rows: O(m) time, O(m) space.)*

**Design decisions:**
- **Why right-to-left distribution?** Stability is essential for LSD; right-to-left with a
  decrementing `count[d]` preserves the previous pass's order for equal digits.
- **Why swap instead of copy?** `std::vector` swap is O(1) (pointer exchange); copying would be O(n)
  per pass and dominate large-`n` runtime.
- **Why 10 passes?** 10-digit keys, base 10 → 10 passes cover every digit; constant `d` and `b`
  make it effectively linear.
- **Why no `std::sort`?** PDF §C.3 forbids library sort routines.

---

## Q3 [5] Heap sort (group)

### Question tasks:
> Implement an **in-place max-heap sort** that sorts the records ascending by their integer key,
> write the sorted CSV, and print the running time. The `heap_sort_step` variant traces the array
> after the heap build and after each extract-max swap for byte-for-byte correctness checking.

### Screenshots, inputs, outputs, figures, explanations, tables, charts:
- *[INSERT]* Input sample — `dataset_1000.csv`.
- *[INSERT]* Output sample — `heap_sorted_dataset_1000.csv`.
- *[INSERT]* Step-trace screenshot — `dataset_1000_heap_sorted_step_1_7.txt` (`initial`, then `i = …`).
- *[INSERT]* **Heap running-time table** across all 10 sizes.
- *[INSERT]* **Chart** — Heap runtime vs `n` (or Radix vs Heap overlaid).
- *[INSERT]* Terminal running-time screenshot.

### Code parts, explanations:

**What it does.** In-place binary max-heap sort: build a max-heap over the whole array, then
repeatedly move the largest element to the end and shrink the heap. Only the sort is timed.

**Input / output contract:**
- **Input:** `<input_csv>`.
- **Output:** `heap_sorted_<input_basename>` in the **current directory** (no `result/` subfolder).
- **Console:** `Sorted <n> rows in <seconds> seconds.` then `Output: <name>`.

**Key data structures:**
- `typedef pair<unsigned long long, string> HeapNode;` — one record = (key, string).
- `vector<HeapNode> data` — both the heap and the final sorted output (in place). For index `i`:
  parent `(i-1)/2`, children `2i+1` / `2i+2`.

**Algorithm walkthrough:**
1. Read CSV (before the timer).
2. Start timer; `heapSort(data)`: **build** by sifting down from the last parent (`n/2 - 1`) to the
   root, then **extract** — for `i` from `n-1` down to `1`, swap root with index `i` and re-heapify
   `[0, i)`.
3. Stop timer; write the sorted rows.

**Annotated snippets.** The driver — build then repeated extract-max:

```cpp
void heapSort(vector<HeapNode> &arr)
{
    long long maxHeapSize = (long long)arr.size();

    // Build the max-heap: sift down from the last parent back to the root.
    for (long long i = maxHeapSize / 2 - 1; i >= 0; i--)
        heapify(arr, maxHeapSize, i);              // ← O(n) bottom-up build

    // Repeatedly move the largest (root) to the end, shrink, re-heapify.
    for (long long i = maxHeapSize - 1; i > 0; i--)
    {
        swapNodes(arr[0], arr[i]);                 // ← max goes to its final spot
        heapify(arr, i, 0);                        // ← restore heap on [0, i)
    }
}
```

The sift-down `heapify` — picks the largest of parent/children and recurses:

```cpp
void heapify(vector<HeapNode> &arr, long long n, long long i)
{
    long long parent = i; // index of the largest of the three
    long long leftChild = 2 * i + 1;
    long long rightChild = 2 * i + 2;

    if (leftChild < n && arr[leftChild].first > arr[parent].first)
        parent = leftChild;                        // ← compare on .first (the key)
    if (rightChild < n && arr[rightChild].first > arr[parent].first)
        parent = rightChild;
    if (parent != i)
    {
        swapNodes(arr[i], arr[parent]);
        heapify(arr, n, parent);                   // ← keep sifting down
    }
}
```

The timed call (timer wraps only the sort):

```cpp
auto t_start = chrono::high_resolution_clock::now();
heapSort(data);                                    // ← only this is timed
auto t_end = chrono::high_resolution_clock::now();
double seconds = chrono::duration<double>(t_end - t_start).count();
```

**`heap_sort_step` (correctness variant).** Same sort on a row slice; prints `initial` after the
build, then `i = <i>` after each extract-max. Usage:
`heap_sort_step <input_csv> <start_row> <end_row>`; output `<basename>_heap_sorted_step_<s>_<e>.txt`:

```cpp
for (long long i = maxHeapSize / 2 - 1; i >= 0; i--)
    heapify(arr, maxHeapSize, i);
print_state(stepFile, arr, "initial");         // ← state right after the build

for (long long i = maxHeapSize - 1; i > 0; i--)
{
    swapNodes(arr[0], arr[i]);
    heapify(arr, i, 0);
    print_state(stepFile, arr, "i = " + to_string(i));   // ← state after extract
}
```

**Time & space complexity:**
- **Time:** **O(n log n)** — build is O(n), then `n − 1` extract-max ops each O(log n).
- **Space:** **O(1)** auxiliary (in place); recursive `heapify` adds O(log n) stack depth.
- *(The `_step` variant is the same on the `m`-row slice: O(m log m) time, O(1) auxiliary.)*

**Design decisions:**
- **Why bottom-up build?** O(n), versus O(n log n) for `n` separate inserts.
- **Why in-place?** Heap sort's defining advantage is O(1) auxiliary space — heap and sorted output
  share the array.
- **Why `unsigned long long` keys?** Keys are always ≥ 1e9, so unsigned avoids negative-comparison
  edge cases.
- **Output in cwd (not `result/`):** a known cross-lane inconsistency, not a bug.

---

## Q4 [5] Hash table search (group)

### Question tasks:
> Build an **open-addressing hash table with linear probing** keyed on the integer, then measure
> the running time of the search operation for the **best, average, and worst cases**. Because a
> single search is too fast to time, each case runs `n` searches and times the whole batch. The
> `hash_table_search_step` variant traces the search path for one found target and one not-found
> target.

### Screenshots, inputs, outputs, figures, explanations, tables, charts:
- *[INSERT]* Input sample — `dataset_1000.csv`.
- *[INSERT]* Output — `hash_table_search_dataset_1000.txt` (best/average/worst case time).
- *[INSERT]* Step traces — `dataset_1000_hash_table_search_step_9794922582.txt` (found) and
  `..._123456789.txt` (not found).
- *[INSERT]* **Best/average/worst table** across all 10 sizes.
- *[INSERT]* **Chart** — best/average/worst vs `n`.
- *[INSERT]* Terminal running-time screenshot.

### Code parts, explanations:

**What it does.** Builds a linear-probing hash table on the integer key, then times searching in
three scenarios (best / average / worst), each running `n` searches. Only the search loops are
timed; reading and building the table are not.

**Input / output contract:**
- **Input:** `<dataset_file.csv>`.
- **Output:** `hash_table_search_dataset_<n>.txt` in the **current directory** (three lines, 6 dp).
- **Console:** the same three lines plus the filename.

**Key data structures:**
- `vector<long long> tableKey(SIZE, 0)`, `vector<string> tableStr(SIZE)` — the table; slot `0` =
  empty (safe — real keys ≥ 1e9).
- `long long SIZE = nextPrime(2 * n + 1)` — prime table size near `2n` (load factor ~0.5).
- `bestKey` / `worstKey` — keys with the shortest / longest probe distance.
- `volatile long long sink` — blocks the optimizer from deleting the timed loops.

**Algorithm walkthrough:**
1. Read CSV (not timed).
2. Build the table by linear probing (not timed).
3. Scan keys to find the shortest (best) and longest (worst) probe distance (not timed).
4–6. **Timed:** best loop (search `bestKey` `n` times), average loop (search every key once), worst
   loop (search `worstKey` `n` times).
7. Write the three times.

**Annotated snippets.** Prime table size:

```cpp
// Return the smallest prime number that is >= x.
// A prime table size spreads the keys out more evenly.
long long nextPrime(long long x)
{
    if (x <= 2) return 2;
    if (x % 2 == 0) x++;                  // primes above 2 are odd
    while (true)
    {
        bool isPrime = true;
        for (long long d = 3; d * d <= x; d += 2)
            if (x % d == 0) { isPrime = false; break; }
        if (isPrime) return x;
        x += 2;
    }
}
```

Building the table by linear probing:

```cpp
long long SIZE = nextPrime(2 * n + 1);    // ← load factor ~0.5, prime size
vector<long long> tableKey(SIZE, 0);      // ← 0 == empty slot
vector<string> tableStr(SIZE);

for (long long i = 0; i < n; i++)
{
    long long pos = keys[i] % SIZE;       // ← home slot
    while (tableKey[pos] != 0)            // ← occupied? probe forward
        pos = (pos + 1) % SIZE;
    tableKey[pos] = keys[i];
    tableStr[pos] = strs[i];
}
```

One timed search loop (average case), showing the probe and the `sink`:

```cpp
auto startAvg = chrono::high_resolution_clock::now();
for (long long i = 0; i < n; i++)
{
    long long key = keys[i];
    long long pos = key % SIZE;
    while (tableKey[pos] != 0)
    {
        if (tableKey[pos] == key) break;  // found
        pos = (pos + 1) % SIZE;           // ← linear probe
    }
    sink += pos;                          // ← keeps the loop from being optimized away
}
auto endAvg = chrono::high_resolution_clock::now();
```

**`hash_table_search_step` (trace variant).** Same table; searches two hardcoded targets and writes
each result. Usage: `hash_table_search_step <dataset_file.csv>`; output per target
`dataset_<n>_hash_table_search_step_<target>.txt`:

```cpp
const long long FOUND_TARGET = 9794922582LL;     // exists in dataset_1000.csv
const long long NOT_FOUND_TARGET = 123456789LL;  // does not exist (only 9 digits)
```

```cpp
long long pos = target % SIZE;
bool found = false; string foundStr;
while (tableKey[pos] != 0)               // ← stop at an empty slot = not found
{
    if (tableKey[pos] == target)         // ← key matched = found
    { found = true; foundStr = tableStr[pos]; break; }
    pos = (pos + 1) % SIZE;              // ← linear probe forward
}
if (found) out << target << " = " << target << "/" << foundStr << "\n";
else       out << "-1 != " << target << "\n";
```

**Time & space complexity:**
- **Build:** O(n) expected.
- **Single search:** best **O(1)**, average **O(1)** (clusters short at ~0.5 load), worst **O(n)**.
- **Measured batches:** each timed loop is `n` searches → best/average O(n) overall, worst → O(n²)
  in the pathological case.
- **Space:** **O(n)** — the prime-sized table arrays.

**Design decisions:**
- **Why `n` searches?** A single lookup is below the clock's resolution; `n` searches amplify it.
- **Why the `volatile sink`?** At `-O2` the compiler would delete the timed loop as dead code;
  accumulating into a `volatile` forces the work to happen.
- **Why linear probing?** PDF §E.4 mandates linear collision resolution.
- **Why ~0.5 load factor + prime size?** Keeps clusters short → average search ~O(1).
- **Why slot value 0 = empty?** Real keys are ≥ 1e9, so 0 never collides — no separate flag array.
- **⚠️ `_step` caveat:** `FOUND_TARGET = 9794922582` only exists in `dataset_1000.csv`; on any other
  dataset the line becomes `-1 != 9794922582` unless the constants are edited and recompiled.

---

## Cross-cutting design decisions (defend in the demo)

These apply to all seven programs and belong wherever code is discussed (or briefly in Q6).

- **`using namespace std;` in every file** — group convention to keep code uncluttered; name
  collisions are not a concern at this single-file scale.
- **Self-contained `.cpp` files, no shared headers** — each compiles alone and is defended in
  isolation; the cost is ~10 lines of duplicated CSV-read code.
- **`long long` / `unsigned long long` throughout** — `int` overflows around 2.1 × 10⁹.
- **Hand-rolled containers instead of `std::unordered_set`/`std::unordered_map`** — PDF §C.4 bans
  containers that search internally; the safer reading bans these, so we hand-roll the equivalent
  (`seen` vector; `tableKey`/`tableStr`). Open tutor question — treated as banned until confirmed.
- **Timer placement outside file I/O** — PDF §C.7.a: read CSV → start timer → run algorithm → stop
  timer → write output. Wrapping all of `main()` would add the file-read cost and hide the real
  algorithmic difference.

---

## Pointer — non-code questions (Q5–Q7)

These are **not code-explanation** sections, so they are not detailed here:

- **Q5 [5] Conclusion** — findings on the same hardware, the best sorting algorithm for the
  array-based implementation, and the **array-based vs linked-list-based AVL** theoretical
  comparison (PDF §C.8). *(Drafted separately.)*
- **Q6 [5] Document clarity & completeness** — `.docx`, APA7 references, hardware-spec screenshot
  per member, OneDrive link, running-time proofs, all figures captioned.
- **Q7 [5] Experiments (individual)** — each member's 10-size runtime tables, charts, and
  command-prompt screenshots. Needs a **chart** (table-only scores low).

*Note: because `dataset_generator` uses the fixed seed `2511325052`, the sorted outputs and step
traces are byte-identical across all three members' machines — only the measured runtimes differ.*
