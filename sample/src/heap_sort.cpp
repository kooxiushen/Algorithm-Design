// heap_sort.cpp
// Usage: ./heap_sort dataset_<n>.csv
// Output: heap_sorted_dataset_<n>.csv + prints running time.

#include "common/record.hpp"
#include "common/csv_io.hpp"
#include <chrono>
#include <iostream>

void heap_sort(std::vector<Record>& v) {
    // TODO: build max-heap with Floyd's method (sift-down from n/2-1 to 0)
    // for i = n-1 down to 1:
    //     swap v[0] and v[i]
    //     sift-down on heap of size i, rooted at 0
}

int main(int argc, char* argv[]) {
    auto data = read_csv(argv[1]);

    auto t0 = std::chrono::high_resolution_clock::now();
    heap_sort(data);
    auto t1 = std::chrono::high_resolution_clock::now();

    write_csv("heap_sorted_" + std::string(argv[1]), data);

    double secs = std::chrono::duration<double>(t1 - t0).count();
    std::cout << "Running time: " << secs << " seconds\n";
    return 0;
}
