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

    // Seed = group leader's mapped ID (251UC25052 -> 2511325052).
    mt19937_64 rng(2511325052ULL);

    vector<long long> keys;
    vector<string> strs;

    // Hand-rolled set for duplicate detection; 0 means empty.
    vector<long long> seen(2 * n + 1, 0);

    while ((long long)keys.size() < n)
    {
        // Random 10-digit number.
        long long k = 1000000000LL + (rng() % 9000000000LL);

        // Linear-probe for duplicates.
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
            continue;
        seen[pos] = k;

        // Random 5-letter lowercase string.
        string s = "";
        for (int i = 0; i < 5; i++)
        {
            s += (char)('a' + rng() % 26);
        }

        keys.push_back(k);
        strs.push_back(s);
    }

    // Write to dataset_<n>.csv.
    string filename = "dataset_" + to_string(n) + ".csv";
    ofstream out(filename);
    for (long long i = 0; i < n; i++)
    {
        out << keys[i] << "," << strs[i] << "\n";
    }

    cout << "Wrote " << n << " rows to " << filename << endl;
    return 0;
}
