// heap_sort_step.cpp
// Usage: ./heap_sort_step dataset_<n>.csv <start_row> <end_row>
// Output: dataset_<n>_heap_sorted_step_<start>_<end>.txt

#include "common/record.hpp"
#include "common/csv_io.hpp"
#include <fstream>

int main(int argc, char* argv[]) {
    auto data = read_csv(argv[1]);
    int start = std::atoi(argv[2]);
    int end   = std::atoi(argv[3]);

    // TODO: build max-heap, then print state labeled "initial"
    // for i = n-1 down to 1:
    //     swap v[0] and v[i], sift-down
    //     print [rows from start..end] labeled "i = <i>"

    return 0;
}
