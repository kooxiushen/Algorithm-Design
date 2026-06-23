# Hash Table Search — Logic Explained

A plain-language explanation of how the hash table search works and what every number/file means.
Use this to understand the design and to answer questions in the demo.

---

## The big idea in one line

> Build a hash table from the **whole dataset (1000 rows)**, then **search 10% of the keys (100)**
> in it and record the results. The table holds everything; the 10% is just the list of things you
> look up.

---

## The 3 numbers (this is the main source of confusion)

| Number | Meaning | Where you see it |
|---|---|---|
| **1000** | the **dataset size** — how many rows are stored in the table | `dataset_1000.csv`, `hash_table_search_dataset_1000.txt` |
| **100** | the **query count** = 10% of 1000 — how many keys you search for | `hash_search_100_dataset.csv` |
| a long key like **9794922582** / **123456789** | a **specific key being searched** (the found / not-found demo) | `dataset_1000_hash_table_search_step_9794922582.txt` |

They are **not** the same thing. `1000` = the data; `100` = how many lookups; the long number = one
target.

---

## Why we need the full 1000

The hash table is the **searchable database**. It must contain **all 1000 rows**, because a search
must be able to find *any* key. If only 100 rows were in the table, the other 900 could never be
found. So the table is always built from the **entire** dataset.

## Why only 10% (100) for the comparison

The PDF requires *"perform many searches — a single search is too fast to be captured."* But
searching **every** row and writing one result line each would create a huge file for large
datasets (e.g. a million result rows). To keep this practical, the lecturer caps the search set at
**10% of the dataset**:

- 1,000 rows  → search 100
- 1,000,000 rows → search 100,000

You are **not** shrinking the data — the table still holds all rows. You are only choosing 10% of
the keys *to look up*.

---

## The flow (with dataset_1000.csv)

```
dataset_1000.csv  (1000 rows)
        │
        │  hash_table_search:
        ├──► builds the hash table from ALL 1000 rows
        ├──► times best / average / worst search  → hash_table_search_dataset_1000.txt
        └──► writes the first 10% (100 rows)       → hash_search_100_dataset.csv
                                                       (the "query set")

        │  hash_table_search_step:
        ├──► trace 1 found + 1 not-found target    → dataset_1000_hash_table_search_step_<target>.txt
        └──► search each of the 100 query keys
             in the table built from the 1000      → search_results.csv  (query,string,found,index)
```

**The "comparison" = the 100 queries are looked up in the full 1000-row table.** Every query is
`found = yes`, because the 100 keys are taken *from* the dataset. `search_results.csv` is the proof.

---

## Decoding every output file name

| File | Read it as |
|---|---|
| `hash_table_search_dataset_1000.txt` | timing on the **1000**-row dataset |
| `hash_search_100_dataset.csv` | the **100**-key query set (10% of 1000) |
| `dataset_1000_hash_table_search_step_9794922582.txt` | on the **1000** dataset, trace search for key **9794922582** (found) |
| `dataset_1000_hash_table_search_step_123456789.txt` | on the **1000** dataset, trace search for key **123456789** (not found) |
| `search_results.csv` | the 100 lookups: `query,string,found,index` |

> The `1000` in `..._step_123456789.txt` is **the dataset size**, not the query count. The
> `123456789` is the **key being searched**. So the file means "search 123456789 in the 1000-row
> dataset" — and it's not found because it only has 9 digits while all real keys have 10.

---

## What `search_results.csv` columns mean

```
query,string,found,index
9794922582,igjkc,yes,1
3556224529,zakoc,yes,2
```
- **query** = the 10-digit key searched
- **string** = its 5-letter value (payload)
- **found** = `yes` / `no`
- **index** = the **1-based row number** in the original dataset (`-1` if not found)

All 100 are `yes` because the query set is the first 100 rows of the dataset.

---

## Demo talking points

- "The table holds the **whole dataset**; we search **10%** of its keys to satisfy the
  *many-searches* rule without producing a giant output."
- "`search_results.csv` shows each of the 100 queries was found, and at which row — that's the
  comparison between the 10% query set and the full dataset."
- "Filenames use the **dataset size** (1000); only `hash_search_100_dataset.csv` uses the 10%
  count."
