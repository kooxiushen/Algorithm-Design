# How to Run — Worked Example

This file walks through one full scenario end-to-end using `dataset_1000.csv`
as the example. Copy-paste each block as-is — every command uses real filenames,
no placeholders.

All commands are run from inside the `src/` folder:

```bash
cd src
```

---

## Step 1 — Compile the 3 programs

```bash
g++ -std=c++17 -O2 dataset_generator.cpp -o dataset_generator
g++ -std=c++17 -O2 radix_sort.cpp        -o radix_sort
g++ -std=c++17 -O2 radix_sort_step.cpp   -o radix_sort_step
```

`-std=c++17` is required (we use `<filesystem>`).
`-O2` is required (without it, timings are misleadingly slow).

---

## Step 2 — Generate `dataset_1000.csv`

```bash
./dataset_generator 1000
```

What happens:
- Creates the `dataset/` folder if missing.
- Writes 1000 unique random rows to `dataset/dataset_1000.csv`.

Expected output on terminal:
```
Wrote 1000 rows to dataset/dataset_1000.csv
```

Check the file:
```bash
head -3 dataset/dataset_1000.csv
```
```
9794922582,igjkc
3556224529,zakoc
5453444626,kfisc
```

---

## Step 3 — Sort it with `radix_sort`

```bash
./radix_sort dataset/dataset_1000.csv
```

What happens:
- Creates the `result/` folder if missing.
- Sorts the 1000 rows in memory using LSD radix sort.
- Writes the sorted file to `result/radix_sorted_dataset_1000.csv`.
- Prints the elapsed seconds (timer wraps only the sort — file I/O is outside).

Expected output:
```
Sorted 1000 rows in 0.000126 seconds.
Output: result/radix_sorted_dataset_1000.csv
```

Verify the result is actually sorted:
```bash
head -3 result/radix_sorted_dataset_1000.csv
```
```
1011011326,vpcbc
1013237162,aoway
1013987915,nearp
```

The integer keys should be ascending.

---

## Step 4 — Trace the algorithm with `radix_sort_step`

```bash
./radix_sort_step dataset/dataset_1000.csv 1 7
```

What happens:
- Reads only rows 1–7 of the input.
- Sorts just those 7 rows using LSD radix sort.
- Writes the array state after each of the 10 digit passes to
  `result/dataset_1000_radix_sorted_step_1_7.txt`.

Expected output:
```
Wrote step trace to result/dataset_1000_radix_sorted_step_1_7.txt
```

Check the trace:
```bash
cat result/dataset_1000_radix_sorted_step_1_7.txt
```
You should see 11 lines: the original ordering, then one line per pass labeled
`d=10` down to `d=1`.

---

## Step 5 — Verify the algorithm is correct (PDF sample diff)

Run the step program against the sample input (the 7 rows from the PDF),
then diff your output against the expected sample output.

```bash
./radix_sort_step ../sample/data/dataset_1000.csv 1 7
diff result/dataset_1000_radix_sorted_step_1_7.txt \
     ../sample/results/dataset_1000_radix_sorted_step_1_7.txt
```

A clean (empty) diff = the radix algorithm is correct.

> Note: this byte-for-byte check only works against `sample/data/dataset_1000.csv`.
> Running against your own `dataset/dataset_1000.csv` produces a different
> (but still correctly sorted) trace because the input data is different.

---

## Step 6 — Clean up

Removes all generated files (datasets, results, compiled binaries).
Source `.cpp` files are kept.

```bash
rm -rf dataset result dataset_generator radix_sort radix_sort_step
```

---

## Reference — using a different dataset size

The scenario above used `n = 1000`. The pattern is identical for any size —
just swap the number consistently in steps 2, 3, 4. For example, `n = 100000`:

```bash
./dataset_generator 100000
./radix_sort        dataset/dataset_100000.csv
./radix_sort_step   dataset/dataset_100000.csv 1 20
```

Produces:
- `dataset/dataset_100000.csv`
- `result/radix_sorted_dataset_100000.csv`
- `result/dataset_100000_radix_sorted_step_1_20.txt`

---

## File layout after the worked example

```
src/
├── dataset_generator.cpp
├── radix_sort.cpp
├── radix_sort_step.cpp
├── dataset_generator                 ← compiled binary
├── radix_sort                        ← compiled binary
├── radix_sort_step                   ← compiled binary
├── dataset/
│   └── dataset_1000.csv
└── result/
    ├── radix_sorted_dataset_1000.csv
    └── dataset_1000_radix_sorted_step_1_7.txt
```
