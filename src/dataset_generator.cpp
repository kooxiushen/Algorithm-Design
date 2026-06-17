// *********************************************************
// Program: dataset_generator.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC4L
// Tutorial Class: T13L
// Trimester: 2610
// Member_1: 242UC244M6  | YAP HUI CHI         | YAP.HUI.CHI@STUDENT.MMU.EDU.MY        | 0163225726
// Member_2: 243UC247CQ  | ERIC CHIN YAN HONG  | ERIC.CHIN.YAN.HONG@STUDENT.MMU.EDU.MY | 0168262342
// Member_3: 251UC25052  | KOO XIU SHEN        | KOO.XIU.SHEN@STUDENT.MMU.EDU.MY       | 01140454502
// *********************************************************
//
// Produces dataset_<n>.csv with n unique random rows.
// Each row: <10-digit integer>,<5-letter lowercase string>
//
// Compile: g++ -std=c++17 -O2 dataset_generator.cpp -o dataset_generator
// Usage:   ./dataset_generator <n>

#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <string>
using namespace std;

int main(int argc, char *argv[])
{
    // Basic input check.
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " <n>\n";
        return 1;
    }
    long long n = atoll(argv[1]);
    if (n <= 0)
    {
        cerr << "Error: n must be a positive number.\n";
        return 1;
    }

    // Seed = group leader's student ID after the letter -> digit mapping.
    // Original ID: 251UC25052
    //   2=2 5=5 1=1 U=1 C=3 2=2 5=5 0=0 5=5 2=2  ->  2511325052
    mt19937_64 rng(2511325052ULL);

    // Storage for the rows we'll output.
    vector<long long> keys;
    vector<string> strs;

    // "seen" is used to remember which numbers we've already generated,
    // so we don't add duplicates. Size 2*n+1 means it's about half full
    // when finished, which keeps lookups fast. Value 0 means "empty".
    vector<long long> seen(2 * n + 1, 0);

    while ((long long)keys.size() < n)
    {
        // Make a random 10-digit number in [1,000,000,000 .. 9,999,999,999].
        long long k = 1000000000LL + (rng() % 9000000000LL);

        // Check if we've seen this number before.
        // Start at position k % size, walk forward until we find an empty slot
        // or the same number.
        long long pos = k % (long long)seen.size();
        bool duplicate = false;
        while (seen[pos] != 0)
        {
            if (seen[pos] == k)
            {
                duplicate = true;
                break;
            }
            pos = (pos + 1) % (long long)seen.size();
        }
        if (duplicate)
            continue;  // try a new random number
        seen[pos] = k; // mark this number as taken

        // Make a 5-letter lowercase string.
        string s = "";
        for (int i = 0; i < 5; i++)
        {
            s += (char)('a' + rng() % 26);
        }

        keys.push_back(k);
        strs.push_back(s);
    }

    // Write the rows to "dataset_<n>.csv".
    string filename = "dataset_" + to_string(n) + ".csv";
    ofstream out(filename);
    for (long long i = 0; i < n; i++)
    {
        out << keys[i] << "," << strs[i] << "\n";
    }

    cout << "Wrote " << n << " rows to " << filename << endl;
    return 0;
}
