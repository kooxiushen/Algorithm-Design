// hash_table_search_step.cpp
// Usage: ./hash_table_search_step dataset_<n>.csv <target>
// Output: dataset_<n>_hash_table_search_step_<target>.txt
// Prints the probe path for one target (found or not found).

#include "common/record.hpp"
#include "common/csv_io.hpp"
#include <fstream>

int main(int argc, char* argv[]) {
    auto data = read_csv(argv[1]);
    uint64_t target = std::stoull(argv[2]);

    // TODO: build hash table, search for target, log the probe path
    // If found: print "<target> = <target>/<str>"
    // If not found: print "-1 != <target>"

    return 0;
}
