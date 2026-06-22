# How to Run — Hash Table Search (test_run)

Run guide for the **hash table** part only (the updated version with the 10% query set +
`query,found,index` comparison). Commands are for macOS / Linux (Terminal). Run everything from
inside this `test_run/` folder.

```bash
cd "/Users/huichiyap/Documents/Algorithms Design Project/Algorithm-Design/test_run"
```

---

## 1. Compile

```bash
g++ -std=c++17 -O2 hash_table_search.cpp      -o hash_table_search
g++ -std=c++17 -O2 hash_table_search_step.cpp -o hash_table_search_step
```

`-O2` is required (without it the timing is artificially slow). C++17 is fine on Mac (Apple Clang).

---

## 2. Make sure you have a dataset

A sample `dataset_1000.csv` is already in this folder. To make a different size, compile and run the
generator from the repo:

```bash
g++ -std=c++17 -O2 ../src/dataset/dataset_generator.cpp -o dataset_generator
./dataset_generator 1000        # -> dataset_1000.csv  (change 1000 to any size)
```

---

## 3. Run — order matters

Run `hash_table_search` **first**, then `hash_table_search_step` (the step program reads the query
file that the first program generates).

```bash
./hash_table_search      dataset_1000.csv
./hash_table_search_step dataset_1000.csv
```

---

## 4. What each program does + its outputs

### `hash_table_search dataset_1000.csv`  → 2 outputs
Builds an open-addressing hash table (linear probing) and:

| Output file | What it is |
|---|---|
| `hash_table_search_dataset_1000.txt` | Best / average / worst search time (each over n searches) |
| `hash_search_100_dataset.csv` | The **first 10%** of the dataset (100 rows for n=1000), used as the query set |

Console:
```
Best case time: 0.000007 seconds
Average case time: 0.000049 seconds
Worst case time: 0.000143 seconds
(Results written to hash_table_search_dataset_1000.txt)
(Query set of 100 rows written to hash_search_100_dataset.csv)
```

> The filename scales with size: `dataset_1000000.csv` → `hash_search_100000_dataset.csv` (10%).

### `hash_table_search_step dataset_1000.csv`  → 2 outputs
Reads the query set, searches each key, and:

| Output file | What it is |
|---|---|
| `dataset_1000_hash_table_search_step_9794922582.txt` | Found-target trace: `9794922582 = 9794922582/igjkc` |
| `dataset_1000_hash_table_search_step_123456789.txt` | Not-found trace: `-1 != 123456789` |
| `search_results.csv` | One line per query: `query,found,index` |

`search_results.csv` looks like:
```
query,found,index
9794922582,yes,1
3556224529,yes,2
...
5381976225,yes,100
```
- **query** = the searched key
- **found** = `yes` / `no`
- **index** = the key's **1-based row number** in the original dataset (`-1` if not found)

Because the queries are the first 10% of the dataset, every one is `found=yes` and the index runs
`1..100`.

---

## 5. Quick checks

```bash
# query set = exactly 10% and equals the first 100 dataset rows
wc -l hash_search_100_dataset.csv                       # -> 100
diff <(head -100 dataset_1000.csv) hash_search_100_dataset.csv   # -> no output = identical

# search_results.csv = header + 100 rows, all found
wc -l search_results.csv                                # -> 101 (header + 100)
grep -c ',yes,' search_results.csv                      # -> 100
```

---

## Notes
- Timing excludes file I/O — only the search loops are timed.
- A `volatile sink` keeps the `-O2` compiler from deleting the timed loops (so times are never 0).
- This is the `test_run/` prototype; the real source lives in `../src/sorting/` (unchanged until
  these are approved).
