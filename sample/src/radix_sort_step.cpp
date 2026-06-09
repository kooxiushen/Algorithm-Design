// radix_sort_step.cpp
// Usage: ./radix_sort_step dataset_<n>.csv <start_row> <end_row>
// Output: dataset_<n>_radix_sorted_step_<start>_<end>.txt
// Same algorithm as radix_sort.cpp, but prints state after each digit pass.

#include "common/record.hpp"
#include "common/csv_io.hpp"
#include <fstream>

int main(int argc, char* argv[]) {
    auto data = read_csv(argv[1]);
    int start = std::atoi(argv[2]);
    int end   = std::atoi(argv[3]);

    std::ofstream out("dataset_X_radix_sorted_step_X_X.txt"); // build name from args

    // TODO: print [rows from start..end] labeled "original"
    // for d = 10 down to 1:
    //     perform one digit pass
    //     print [rows from start..end] labeled "d=<d>"

    return 0;
}
