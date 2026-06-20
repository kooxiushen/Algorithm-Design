# Code Explanation — CCP6214 Group Assignment

This document walks through the implementation of all **seven** programs delivered for the
CCP6214 Algorithm Design & Analysis assignment. Each program has its own section with the same
structure: what it does, its input/output contract, key data structures, a plain-English
algorithm walkthrough, annotated code snippets pulled directly from the source, and the design
decisions behind it.

The snippets are copied **verbatim** from the real `.cpp` files (only `// ←` pointer comments are
added) so that any group member can read a section and defend that program in the demo Q&A, and
so the explanations can be pasted into the report's code-explanation sections.

---

## 1. Overview

The seven programs split into four jobs:

| # | Program | Job |
|---|---|---|
| 1 | `dataset_generator` | Produces the input CSV: `n` unique random `(10-digit int, 5-letter string)` rows |
| 2 | `radix_sort` | LSD radix sort, base 10, sorts records by the integer key; prints running time |
| 3 | `radix_sort_step` | Same radix sort, but traces the array after each digit pass (correctness check) |
| 4 | `heap_sort` | In-place max-heap sort by the integer key; prints running time |
| 5 | `heap_sort_step` | Same heap sort, but traces the array after the build and each extract-max |
| 6 | `hash_table_search` | Open-addressing hash table (linear probing); times best/average/worst search |
| 7 | `hash_table_search_step` | Same table, but traces one found target and one not-found target |

**Shared conventions across all seven files:**

- Each `.cpp` is **self-contained** — no shared headers, no `common/` folder. Small bits of
  CSV-reading code are duplicated on purpose so every file compiles and is defended in isolation.
- `using namespace std;` is used in every file (no `std::` prefixes).
- A course/member header block sits at the top of every file (required by PDF §F.1.a).
- `long long` is used for `n`, positions, and keys because plain `int` overflows at the large
  input sizes the assignment expects.
- **Timing excludes I/O.** Where a program is timed, only the algorithm call is wrapped with
  `std::chrono::high_resolution_clock` — the CSV is read before the timer starts and the output
  is written after it stops (PDF §C.7.a).

**Compile (each program is standalone):**

```bash
g++ -std=c++17 -O2 src/dataset/dataset_generator.cpp -o dataset_generator
g++ -std=c++17 -O2 src/sorting/radix_sort.cpp        -o radix_sort
# ...same pattern for the other five
```

`-std=c++17` is **required** because the two radix programs use `<filesystem>`. `-O2` is required
so the experiment timings are realistic.

> **One real inconsistency to be aware of (and to state honestly in the demo):** only the radix
> lane writes its output into a `result/` subfolder. `dataset_generator`, both heap programs, and
> both hash programs write to the **current working directory**. The lanes were written
> separately and this convention was never unified. It is documented below per program.

---

## 2. dataset_generator

### What it does
Generates the input file `dataset_<n>.csv` containing `n` rows. Each row is a unique, random,
positive 10-digit integer (range 1,000,000,000–9,999,999,999) paired with a random 5-letter
lowercase string, written in random order. The random number generator is seeded with the group
leader's mapped student ID so the dataset is reproducible and differs from other groups.

### Input / output contract
- **Input:** one CLI argument `<n>` (a positive `long long`).
- **Output:** `dataset_<n>.csv` written to the **current directory**.
- **Console:** prints `Wrote <n> rows to dataset_<n>.csv`.
- No timer — generation is not part of the algorithm experiment.

### Key data structures
- `vector<long long> keys` and `vector<string> strs` — the output rows, kept in two parallel
  vectors (index `i` of each is one row).
- `vector<long long> seen(2 * n + 1, 0)` — a **hand-rolled hash set** used only to detect
  duplicate integers. Sized to about `2n` so it stays roughly half full, which keeps the linear
  probe short. The value `0` marks an empty slot (safe, because every real key is ≥ 1,000,000,000).

### Algorithm walkthrough
1. Read `n` from `argv`; reject `n ≤ 0`.
2. Seed `mt19937_64` with `2511325052ULL` (the leader's mapped ID).
3. Repeat until `keys.size() == n`:
   a. Generate a random 10-digit number `k`.
   b. Linear-probe `seen` from slot `k % size`: if the same number is already there, it is a
      duplicate — throw it away and try again; otherwise mark the slot taken.
   c. Build a random 5-letter string and append `k` and the string to the output vectors.
4. Write every row to `dataset_<n>.csv`.

### Annotated code snippets

The seed — this exact value must be used by every member so the datasets match:

```cpp
// Seed = group leader's student ID after the letter -> digit mapping.
// Original ID: 251UC25052
//   2=2 5=5 1=1 U=1 C=3 2=2 5=5 0=0 5=5 2=2  ->  2511325052
mt19937_64 rng(2511325052ULL);          // ← deterministic, reproducible dataset
```

The uniqueness check by linear probing (this is what replaces `std::unordered_set`):

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

### Time & space complexity
- **Time:** O(n) expected. Each of the `n` numbers is generated and inserted in expected O(1)
  (the ~0.5 load factor keeps the probe short); a duplicate just triggers a cheap retry, which is
  rare across a 9-billion-wide range. Building each 5-letter string is constant work.
- **Space:** O(n) — the `seen` table (size `2n+1`) plus the two output vectors.

### Design decisions
- **Why hand-roll the uniqueness set instead of `std::unordered_set`?** PDF §C.4 forbids language
  containers that sort or searching internally. A strict reading bans `unordered_set` (its
  `insert` searches internally for duplicates), so we hand-roll the same behaviour with a
  `vector<long long>` and linear probing — same job, no banned container.
- **Why size `2 * n + 1`?** A load factor near 0.5 keeps the expected probe length to about 1–2
  slots, so duplicate detection stays cheap even at large `n`.
- **Why `long long` for `n`?** Plain `int` overflows around 2.1 × 10⁹, well inside the assignment's
  stated maximum input range.

---

## 3. radix_sort

### What it does
Sorts the records into ascending order by their 10-digit integer key using **LSD (least
significant digit) radix sort**, base 10, in exactly 10 passes (one per digit). The 5-letter
string rides along as payload. Only the sort is timed.

### Input / output contract
- **Input:** `<input_csv>` (e.g. `dataset_1000.csv`).
- **Output:** `result/radix_sorted_<input_basename>` — note the **`result/` subfolder**, which is
  auto-created. (This is the only lane that uses a subfolder.)
- **Console:** `Sorted <n> rows in <seconds> seconds.` then `Output: <path>`.

### Key data structures
- `vector<long long> keys`, `vector<string> strs` — the input rows, in two parallel vectors.
- `vector<long long> tmp_keys(n)`, `vector<string> tmp_strs(n)` — output buffers for one pass,
  allocated once and reused.
- `long long count[10]` — a counting array, one slot per digit value 0–9 (10 entries because
  base 10).

### Algorithm walkthrough
1. Read the CSV into `keys` and `strs` (**before** the timer).
2. Start the chrono timer.
3. For each of the 10 digit positions (least significant first):
   a. Zero `count[]`.
   b. Count how many keys have each digit value (0–9) at the current position.
   c. Prefix-sum `count[]` so each entry points just past the end of its bucket.
   d. Walk the data **right-to-left**, placing each record into its bucket — this preserves the
      order from earlier passes, which is what makes LSD radix sort correct (stability).
   e. Swap the `keys`/`strs` vectors with the temp buffers (O(1) — only internal pointers swap),
      then multiply the divisor by 10 to move to the next digit.
4. Stop the timer.
5. Write the sorted rows to `result/radix_sorted_<basename>`.

### Annotated code snippets

The timed sort — note the timer starts *after* the file is read:

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

The output path, created in a `result/` subfolder via `<filesystem>`:

```cpp
filesystem::create_directories("result");             // ← needs -std=c++17
size_t slash = in_name.find_last_of("/\\");
string base = (slash == string::npos) ? in_name : in_name.substr(slash + 1);
string out_name = "result/radix_sorted_" + base;
```

### Time & space complexity
- **Time:** O(d · (n + b)) where `d = 10` digit passes and `b = 10` (the base). Because `d` and
  `b` are fixed constants here, this is effectively **O(n)** — linear in the number of records.
- **Space:** O(n + b) — the temp buffers `tmp_keys`/`tmp_strs` are size `n` and `count` is size 10,
  so **O(n)**.

### Design decisions
- **Why right-to-left distribution?** Stability is essential for LSD radix sort. Iterating
  right-to-left while decrementing `count[d]` first keeps records with the same digit in the
  relative order they had after the previous pass.
- **Why swap instead of copy?** `std::vector` swap is O(1) — it exchanges internal pointers.
  Copying buffers each pass would be O(n) extra work per pass and would dominate large-`n` runtime.
- **Why exactly 10 passes?** Keys are 10-digit integers, so 10 base-10 passes cover every digit.
  Because the digit count and base are fixed constants, the sort is effectively O(n).
- **Why no `std::sort`?** PDF §C.3 forbids library sort routines, so the counting/distribution
  loop is hand-written.

---

## 4. radix_sort_step

### What it does
Runs the **same** LSD radix sort as `radix_sort`, but only on a chosen slice of rows
`[start_row, end_row]` (1-indexed, inclusive), and writes the full array state after each digit
pass to a `.txt` file. This is the correctness check — its output is compared byte-for-byte
against the PDF sample.

### Input / output contract
- **Input:** `<input_csv> <start_row> <end_row>` (e.g. `dataset_1000.csv 1 7`).
- **Output:** `result/<basename_without_.csv>_radix_sorted_step_<start>_<end>.txt`.
- **Console:** `Wrote step trace to <path>`.
- No timer — this variant is about tracing, not measuring.

### Key data structures
- Same `keys` / `strs` / `tmp_keys` / `tmp_strs` / `count[10]` as `radix_sort`, but `keys`/`strs`
  hold only the selected slice of rows.
- A helper `print_state(out, keys, strs, label)` that writes one line: `[k1/s1, k2/s2, ...] label`.

### Algorithm walkthrough
1. Read only rows `[start_row, end_row]` from the CSV (skip rows before, stop after).
2. Write the `original` line.
3. Run the same 10-pass radix sort, **but** label the passes `d=10` (first pass, ones digit) down
   to `d=1` (last pass, 10⁹ digit) to match the PDF sample, writing the array state after each pass.

### Annotated code snippets

The pass loop is the same sort, with a label counter and a `print_state` after each pass:

```cpp
long long divisor = 1;
for (int d_label = 10; d_label >= 1; d_label--)        // ← label 10 (ones) -> 1 (10^9)
{
    for (int i = 0; i < 10; i++) count[i] = 0;
    for (long long i = 0; i < n; i++)
    {
        int d = (int)((keys[i] / divisor) % 10);
        count[d]++;
    }
    for (int i = 1; i < 10; i++) count[i] += count[i - 1];
    for (long long i = n - 1; i >= 0; i--)
    {
        int d = (int)((keys[i] / divisor) % 10);
        count[d]--;
        tmp_keys[count[d]] = keys[i];
        tmp_strs[count[d]] = strs[i];
    }
    swap(keys, tmp_keys);
    swap(strs, tmp_strs);
    divisor *= 10;
    print_state(out, keys, strs, "d=" + to_string(d_label));   // ← trace this pass
}
```

The trace line format:

```cpp
void print_state(ofstream &out,
                 const vector<long long> &keys,
                 const vector<string> &strs,
                 const string &label)
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

### Time & space complexity
- **Time:** same O(d · (m + b)) ≈ **O(m)**, where `m` is the number of rows in the slice
  `[start, end]`, plus the per-pass trace writing. (Identical algorithm to `radix_sort`; only the
  input size is the chosen slice.)
- **Space:** O(m + b) ≈ **O(m)** for the slice and temp buffers.

### Design decisions
- **Why the `d=10 … d=1` labels?** They mirror the PDF sample exactly so the trace can be diffed
  byte-for-byte. The first pass works on the ones digit (`divisor = 1`) and is labelled `d=10`;
  each later pass divides by a higher power of ten.
- **Why a row slice?** The PDF samples are produced from a small window of rows (e.g. rows 1–7),
  small enough to print and verify by eye.
- **Same core as `radix_sort`** — only the row slice and the per-pass printing differ, so a bug in
  one is a bug in both, which makes verification meaningful.

---

## 5. heap_sort

### What it does
Sorts the records in ascending order by their integer key using an **in-place binary max-heap**.
It first builds a max-heap over the whole array, then repeatedly moves the largest element to the
end and shrinks the heap. Only the sort is timed.

### Input / output contract
- **Input:** `<input_csv>`.
- **Output:** `heap_sorted_<input_basename>` written to the **current directory** (no `result/`
  subfolder — different from the radix lane).
- **Console:** `Sorted <n> rows in <seconds> seconds.` then `Output: <name>`.

### Key data structures
- `typedef pair<unsigned long long, string> HeapNode;` — one record = (key, string). `unsigned
  long long` because keys are always positive.
- `vector<HeapNode> data` — both the heap and the final sorted output (the sort is in place).
  For index `i`: parent is `(i-1)/2`, children are `2i+1` and `2i+2`.

### Algorithm walkthrough
1. Read the CSV into `vector<HeapNode> data` (**before** the timer).
2. Start the timer; call `heapSort(data)`:
   a. **Build phase:** sift down from the last parent (`n/2 - 1`) back to the root, turning the
      array into a max-heap.
   b. **Extract phase:** for `i` from `n-1` down to `1`, swap the root (largest) with index `i`,
      then re-heapify the shrunk heap `[0, i)`. After each step one more element is in its final
      sorted place at the back.
3. Stop the timer; write the sorted rows.

### Annotated code snippets

The driver — build then repeated extract-max:

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

### Time & space complexity
- **Time:** **O(n log n)** — the bottom-up build is O(n), then `n − 1` extract-max operations each
  cost O(log n) for the sift-down.
- **Space:** **O(1)** auxiliary — the sort is in place; the heap and the sorted output share the
  same array. (The recursive `heapify` adds O(log n) call-stack depth.)

### Design decisions
- **Why bottom-up build (sift down from `n/2 - 1`)?** Building the heap this way is O(n);
  inserting `n` elements one at a time would be O(n log n). For a one-shot build, bottom-up is
  strictly better.
- **Why in-place?** Heap sort's defining advantage is O(1) auxiliary space — the heap and the
  sorted result share the same array. Extracted maxima accumulate at the back as the heap shrinks
  at the front.
- **Why `unsigned long long` for the key?** Keys are always ≥ 1,000,000,000, so unsigned is a
  natural fit and avoids any negative-comparison edge cases.
- **Output in the current directory (not `result/`):** this differs from the radix lane. It is a
  known cross-lane inconsistency, not a bug — worth mentioning if asked to unify the programs.

---

## 6. heap_sort_step

### What it does
Runs the **same** in-place max-heap sort as `heap_sort`, on a chosen slice of rows
`[start_row, end_row]`, and writes the array state after the heap is built and after each
extract-max swap to a `.txt` file, for byte-for-byte correctness checking against the PDF sample.

### Input / output contract
- **Input:** `<input_csv> <start_row> <end_row>` (e.g. `dataset_1000.csv 1 7`).
- **Output:** `<basename_without_.csv>_heap_sorted_step_<start>_<end>.txt` in the **current
  directory**.
- **Console:** `Wrote step trace to <name>`.
- No timer.

### Key data structures
- Same `HeapNode` and `vector<HeapNode> data` as `heap_sort`, holding only the selected rows.
- A `print_state(out, arr, label)` helper writing `[k1/s1, k2/s2, ...] label`.

### Algorithm walkthrough
1. Read only rows `[start_row, end_row]`.
2. Build the max-heap, then print the `initial` state.
3. For each extract-max (`i` from `n-1` down to `1`): swap root with index `i`, re-heapify, and
   print the state labelled `i = <i>`.

### Annotated code snippets

The heap-sort driver, now taking the trace file and printing after each step:

```cpp
void heapSort(vector<HeapNode> &arr, ofstream &stepFile)
{
    long long maxHeapSize = (long long)arr.size();

    // Build the max-heap, then record the initial heap state.
    for (long long i = maxHeapSize / 2 - 1; i >= 0; i--)
        heapify(arr, maxHeapSize, i);
    print_state(stepFile, arr, "initial");         // ← state right after the build

    // Each extract-max: move root to the end, shrink, re-heapify, record.
    for (long long i = maxHeapSize - 1; i > 0; i--)
    {
        swapNodes(arr[0], arr[i]);
        heapify(arr, i, 0);
        print_state(stepFile, arr, "i = " + to_string(i));   // ← state after extract
    }
}
```

### Time & space complexity
- **Time:** same **O(m log m)** on the `m`-row slice, plus O(m) per recorded state for the trace
  writing.
- **Space:** **O(1)** auxiliary for the sort itself (the trace is streamed straight to the file).

### Design decisions
- **Why print `initial` then `i = <i>`?** It matches the PDF sample's labelling (the heap state
  after the build, then after each extract-max) so the trace diffs cleanly.
- **Same heapify/heapSort core as `heap_sort`** — only the row slice and the `print_state` calls
  differ, keeping the verification meaningful.
- Output is written to the current directory, consistent with `heap_sort` (and, again, different
  from the radix lane's `result/`).

---

## 7. hash_table_search

### What it does
Builds an **open-addressing hash table with linear probing** keyed on the integer, then measures
how long searching takes in three scenarios — best, average, and worst case. Because a single
search is far too fast to time, each case runs `n` searches (where `n` is the dataset size) and
times the whole batch. Only the search loops are timed; reading the file and building the table
are not.

### Input / output contract
- **Input:** `<dataset_file.csv>`.
- **Output:** `hash_table_search_dataset_<n>.txt` in the **current directory**, containing three
  lines (best/average/worst case time, 6 decimal places).
- **Console:** the same three lines plus the output filename.

### Key data structures
- `vector<long long> tableKey(SIZE, 0)` and `vector<string> tableStr(SIZE)` — the hash table, in
  two parallel arrays. Slot value `0` means "empty" (safe — real keys are ≥ 1,000,000,000).
- `long long SIZE = nextPrime(2 * n + 1)` — a **prime** table size near `2n`, so the load factor
  is ~0.5 and keys spread out evenly.
- `bestKey` / `worstKey` — the dataset keys with the shortest and longest probe distance, found by
  scanning the table once.
- `volatile long long sink` — a dummy accumulator that stops the optimizer from deleting the timed
  loops.

### Algorithm walkthrough
1. Read the CSV into `keys` / `strs` (not timed).
2. Build the table: for each key, compute `home = key % SIZE`, then linear-probe forward to the
   first empty slot and insert (not timed).
3. Scan every key to find the one with the smallest probe distance (best case) and the one with
   the largest probe distance (worst case) (not timed).
4. **Timed loop 1 (best):** search `bestKey` `n` times.
5. **Timed loop 2 (average):** search every key in the dataset once.
6. **Timed loop 3 (worst):** search `worstKey` `n` times.
7. Write the three batch times to the `.txt` file and the console.

### Annotated code snippets

Choosing a prime table size:

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

One of the three timed search loops (the average case), showing the probe and the `sink`:

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

### Time & space complexity
- **Build:** O(n) expected to insert all `n` keys (the ~0.5 load factor keeps probes short).
- **Single search:** best **O(1)** (key in its home slot), average **O(1)** (short clusters at
  ~0.5 load), worst **O(n)** (one long probe chain through a full cluster).
- **Measured batches:** each timed loop runs `n` searches, so the best and average batches are
  O(n) overall and the worst batch approaches O(n²) in the pathological case.
- **Space:** **O(n)** — the prime-sized table arrays `tableKey` and `tableStr`.

### Design decisions
- **Why `n` searches instead of one?** A single hash lookup is in the nanosecond range — below the
  clock's useful resolution. Looping `n` times amplifies the signal into a measurable duration.
- **Why the `volatile sink`?** At `-O2`, the compiler can see the search results are unused and
  delete the entire timed loop, which would make the measured time meaningless. Accumulating into a
  `volatile` value forces the work to actually happen.
- **Why linear probing?** PDF §E.4 mandates "linear collision resolution".
- **Why load factor ~0.5 and a prime size?** A half-full prime-sized table keeps clusters short, so
  the average successful search stays close to O(1).
- **Why slot value 0 for empty?** All real keys are ≥ 1,000,000,000, so 0 can never collide with a
  real key — no separate "occupied" flag array is needed.
- **How are best/worst found?** By measuring each key's probe distance once: distance 0 (sits in
  its home slot) is the best case; the longest probe chain is the worst case.

---

## 8. hash_table_search_step

### What it does
Builds the **same** linear-probing hash table as `hash_table_search`, then searches for two fixed
targets — one that exists in the dataset and one that does not — and writes the result of each
search to its own file in the exact PDF sample format. This demonstrates the search path for a hit
and a miss.

### Input / output contract
- **Input:** `<dataset_file.csv>` only — the targets are hardcoded constants in the source.
- **Output (one file per target):** `dataset_<n>_hash_table_search_step_<target>.txt`
  - Found → `<target> = <target>/<string>` (e.g. `2008864030 = 2008864030/rdiea`)
  - Not found → `-1 != <target>` (e.g. `-1 != 123456789`)
- **Console:** echoes each result line and its filename.

### Key data structures
- Same prime-sized `tableKey` / `tableStr` arrays and `nextPrime` helper as
  `hash_table_search`.
- Two compile-time constants holding the targets.

### Algorithm walkthrough
1. Read the CSV and build the table (same as the full version).
2. For each of the two targets: probe from `target % SIZE` forward until the key is found or an
   empty slot is hit.
3. Write the result line (found or not-found) to the target's own file and echo it to the console.

### Annotated code snippets

The hardcoded targets (the tutor edits these in the file for the demo):

```cpp
// ===== TARGETS (the tutor edits these in the code file) ==============
// One key that exists in the dataset, and one that does not.
const long long FOUND_TARGET = 9794922582LL;     // exists in dataset_1000.csv
const long long NOT_FOUND_TARGET = 123456789LL;  // does not exist (only 9 digits)
// ====================================================================
```

The probe-and-report loop:

```cpp
long long pos = target % SIZE;
bool found = false;
string foundStr;
while (tableKey[pos] != 0)               // ← stop at an empty slot = not found
{
    if (tableKey[pos] == target)         // ← key matched = found
    {
        found = true;
        foundStr = tableStr[pos];
        break;
    }
    pos = (pos + 1) % SIZE;              // ← linear probe forward
}

// Build the output line in the exact sample format.
if (found)
    out << target << " = " << target << "/" << foundStr << "\n";
else
    out << "-1 != " << target << "\n";
```

### Time & space complexity
- **Build:** O(n) expected.
- **Per-target search:** best/average **O(1)**, worst **O(n)** along the probe chain — same as the
  full version, just for the two fixed targets.
- **Space:** **O(n)** for the table.

### Design decisions
- **Why hardcoded targets?** The PDF demo expects specific found/not-found values, and the tutor
  may change them in the code during the demo, so they live as named constants rather than CLI args.
- **⚠️ Important caveat:** `FOUND_TARGET = 9794922582` only exists in `dataset_1000.csv`. On any
  other dataset that key won't be present and the first line becomes `-1 != 9794922582`. To trace a
  hit on a different dataset, edit the constants and recompile.
- **Same table build and probe as the full version** — the only differences are the fixed targets
  and the trace output format.

---

## 9. Cross-cutting design decisions

These choices apply to all seven programs and are worth being ready to defend in the demo Q&A.

### 9.1 `using namespace std;` in every file
Chosen as a group convention to keep the code uncluttered for readers new to C++ (no `std::`
prefixes). The trade-off — possible name collisions — is not a concern at this small, single-file
scale.

### 9.2 Self-contained `.cpp` files, no shared headers
We deliberately did **not** extract a shared `Record` type, CSV reader, RNG, or `nextPrime` into a
`common/` header. Each program compiles on its own (`g++ file.cpp`), and each can be opened and
defended in isolation during the demo. The cost is a little duplication (the ~10-line CSV-read
loop appears in several files); the benefit is independence, which the group judged more valuable
for an assignment graded per-program.

### 9.3 `long long` / `unsigned long long` throughout
Plain `int` overflows around 2.1 × 10⁹, inside the assignment's stated maximum input range. We use
`long long` for `n`, table positions, and digit math, and `unsigned long long` for heap keys (which
are always positive).

### 9.4 Hand-rolled containers instead of `std::unordered_set` / `std::unordered_map`
PDF §C.4 bans any library data structure that "performs sorting or searching internally" without
naming these two. A strict reading bans them (`unordered_set::insert` searches internally), so the
safer position is to hand-roll the equivalent — the `seen` vector in `dataset_generator` and the
`tableKey`/`tableStr` arrays in the hash programs both use a plain `vector` plus linear probing.
This is an open tutor question; until it is confirmed otherwise we treat them as banned.

### 9.5 Timer placement — outside file I/O
PDF §C.7.a requires the measured running time to exclude I/O. Every timed program follows the same
pattern: **read CSV → start timer → run the algorithm → stop timer → write output.** Wrapping the
whole of `main()` instead would add the file-read cost (which grows with `n`) to the measurement
and would hide the real algorithmic difference between radix and heap sort.

---

*Note: the asymptotic complexity analysis for each algorithm belongs in the corresponding section
of the main report. Because `dataset_generator` uses the fixed seed `2511325052`, the sorted
outputs and step traces are byte-identical across all three members' machines — only the measured
runtimes differ.*
