// dataset_generator.cpp
// Usage: ./dataset_generator <n>
// Output: dataset_<n>.csv with n unique random 10-digit ints + 5-letter strings.

#include "common/record.hpp"
#include "common/csv_io.hpp"
#include "common/rng.hpp"
#include <vector>
#include <iostream>

int main(int argc, char* argv[]) {
    seed_rng(2431324730ULL); // TODO: replace with leader's student ID digits

    int n = std::atoi(argv[1]);
    std::vector<Record> rows;
    rows.reserve(n);

    // TODO: generate n unique integers in [1e9, 9_999_999_999]
    // TODO: generate 5-letter lowercase string for each
    // TODO: shuffle so input is random order

    std::string fname = "dataset_" + std::to_string(n) + ".csv";
    write_csv(fname, rows);
    return 0;
}
