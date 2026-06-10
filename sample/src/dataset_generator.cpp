// *********************************************************
// Program: dataset_generator.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC4L
// Tutorial Class: T13L
// Trimester: 2610
// Member_1: 242UC244M6  | YAP HUI CHI  | YAP.HUI.CHI@STUDENT.MMU.EDU.MY | 0163225726
// Member_2: 243UC247CQ  | ERIC CHIN YAN HONG  | ERIC.CHIN.YAN.HONG@STUDENT.MMU.EDU.MY | 0168262342
// Member_3: 251UC25052 | KOO XIU SHEN | KOO.XIU.SHEN@STUDENT.MMU.EDU.MY | 01140454502
// *********************************************************

#include "common/record.hpp"
#include "common/csv_io.hpp"
#include "common/rng.hpp"
#include <vector>
#include <iostream>
#include <unordered_set>

int main(int argc, char* argv[]) {
    // Group leader student ID: 251UC25052
    // Seed: 2 5 1 U=1 C=3 2 5 0 5 2 = 2511325052
    seed_rng(2511325052ULL);

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <size>\n";
        return 1;
    }

    long long n = std::stoll(argv[1]);
    if (n <= 0) {
        std::cerr << "Error: Size must be positive\n";
        return 1;
    }

    // Random number range: 1,000,000,000 to 9,999,999,999
    std::uniform_int_distribution<uint64_t> key_dist(1000000000ULL, 9999999999ULL);
    std::uniform_int_distribution<int> chr_dist(0, 25); // a=0, z=25

    // Use unordered_set to ensure all numbers are unique
    std::unordered_set<uint64_t> used;
    used.reserve(n);

    std::vector<Record> rows;
    rows.reserve(n);

    std::cout << "Generating " << n << " records...\n";

    long long count = 0;
    while (count < n) {
        uint64_t key = key_dist(global_rng());

        // Skip if number already used
        if (used.count(key)) continue;
        used.insert(key);

        Record r;
        r.key = key;

        // Generate random 5-letter lowercase string
        for (int i = 0; i < 5; i++) {
            r.str[i] = 'a' + chr_dist(global_rng());
        }
        r.str[5] = '\0';

        rows.push_back(r);
        count++;

        // Show progress every 1 million records
        if (count % 1000000 == 0) {
            std::cout << count << " records done...\n";
        }
    }

    std::string fname = "dataset_" + std::string(argv[1]) + ".csv";
    write_csv(fname, rows);

    std::cout << "Done! File saved as: " << fname << "\n";
    return 0;
}