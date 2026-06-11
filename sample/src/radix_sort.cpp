// radix_sort.cpp
// Usage: ./radix_sort dataset_<n>.csv
// Output: radix_sorted_dataset_<n>.csv + prints running time.

#include "common/record.hpp"
#include "common/csv_io.hpp"
#include <vector>
#include <chrono>
#include <iostream>

void radix_sort(std::vector<Record>& v) {
    // TODO: LSD radix sort, base 10, 10 passes (digit d=1 ... d=10)
    // Sort by integer key. Stable. Buckets = 10 vectors<Record>.
}

int main(int argc, char* argv[]) {
    auto data = read_csv(argv[1]);              // I/O - before timer

    auto t0 = std::chrono::high_resolution_clock::now();
    radix_sort(data);                            // ONLY this is timed
    auto t1 = std::chrono::high_resolution_clock::now();

    write_csv("radix_sorted_" + std::string(argv[1]), data); // I/O - after timer

    double secs = std::chrono::duration<double>(t1 - t0).count();
    std::cout << "Running time: " << secs << " seconds\n";
    return 0;
}
