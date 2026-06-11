// hash_table_search.cpp
// Usage: ./hash_table_search dataset_<n>.csv
// Output: hash_table_search_dataset_<n>.txt with best/avg/worst times.

#include "common/record.hpp"
#include "common/csv_io.hpp"
#include <chrono>
#include <iostream>
#include <fstream>

class HashTable {
    // TODO: open-addressing with linear probing.
    // Table size ~= 1.5 * n, prime.
public:
    void insert(const Record& r);
    int  search(uint64_t key);  // returns index or -1
};

int main(int argc, char* argv[]) {
    auto data = read_csv(argv[1]);
    HashTable ht;
    for (auto& r : data) ht.insert(r);

    double best = 1e18, worst = 0, sum = 0;
    for (auto& r : data) {                          // n searches
        auto t0 = std::chrono::high_resolution_clock::now();
        ht.search(r.key);
        auto t1 = std::chrono::high_resolution_clock::now();
        double s = std::chrono::duration<double>(t1 - t0).count();
        best = std::min(best, s);
        worst = std::max(worst, s);
        sum += s;
    }

    std::ofstream out("hash_table_search_" + std::string(argv[1]) + ".txt");
    out << "Best case time: " << best << " seconds\n"
        << "Average case time: " << sum / data.size() << " seconds\n"
        << "Worst case time: " << worst << " seconds\n";
    return 0;
}
