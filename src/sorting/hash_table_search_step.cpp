// *********************************************************
// Program: hash_table_search_step.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC4L
// Tutorial Class: T13L
// Trimester: 2610
// Member_1: 242UC244M6  | YAP HUI CHI         | YAP.HUI.CHI@STUDENT.MMU.EDU.MY        | 0163225726
// Member_2: 243UC247CQ  | ERIC CHIN YAN HONG  | ERIC.CHIN.YAN.HONG@STUDENT.MMU.EDU.MY | 0168262342
// Member_3: 251UC25052  | KOO XIU SHEN        | KOO.XIU.SHEN@STUDENT.MMU.EDU.MY       | 01140454502
// *********************************************************
//
// Reads dataset_<n>.csv, builds a hash table (linear probing) on the
// integer key, then searches for the targets below and writes the
// result of each search to its own file.
//
// Output file per target: dataset_<n>_hash_table_search_step_<target>.txt
//   Found     ->  <target> = <key>/<string>     e.g.  2008864030 = 2008864030/rdiea
//   Not found ->  -1 != <target>                e.g.  -1 != 123456789
//
// Compile: g++ -std=c++17 -O2 hash_table_search_step.cpp -o hash_table_search_step
// Usage:   ./hash_table_search_step <dataset_file.csv>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

// ===== TARGETS (the tutor edits these in the code file) ==============
// One key that exists in the dataset, and one that does not.
const long long FOUND_TARGET = 9794922582LL;     // exists in dataset_1000.csv
const long long NOT_FOUND_TARGET = 123456789LL;  // does not exist (only 9 digits)
// ====================================================================

// Return the smallest prime number that is >= x.
// A prime table size spreads the keys out more evenly.
long long nextPrime(long long x)
{
    if (x <= 2)
        return 2;
    if (x % 2 == 0)
        x++; // primes above 2 are odd
    while (true)
    {
        bool isPrime = true;
        for (long long d = 3; d * d <= x; d += 2)
        {
            if (x % d == 0)
            {
                isPrime = false;
                break;
            }
        }
        if (isPrime)
            return x;
        x += 2;
    }
}

int main(int argc, char *argv[])
{
    // Basic input check.
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " <dataset_file.csv>\n";
        return 1;
    }

    // ----- Read the dataset into memory. -----
    ifstream in(argv[1]);
    if (!in)
    {
        cerr << "Error: could not open " << argv[1] << "\n";
        return 1;
    }

    vector<long long> keys; // the 10-digit integer of each row
    vector<string> strs;    // the 5-letter string of each row

    string line;
    while (getline(in, line))
    {
        if (line.empty())
            continue;
        // Each line is "<integer>,<string>". Split at the comma.
        size_t comma = line.find(',');
        if (comma == string::npos)
            continue;
        long long k = stoll(line.substr(0, comma));
        string s = line.substr(comma + 1);
        keys.push_back(k);
        strs.push_back(s);
    }
    in.close();

    long long n = (long long)keys.size();
    if (n == 0)
    {
        cerr << "Error: dataset is empty.\n";
        return 1;
    }

    // ----- Build the hash table (linear probing). -----
    // Table size ~2n (load factor ~0.5) and prime for good spreading.
    // Slot value 0 means "empty" (real keys are >= 1,000,000,000).
    long long SIZE = nextPrime(2 * n + 1);
    vector<long long> tableKey(SIZE, 0);
    vector<string> tableStr(SIZE);

    for (long long i = 0; i < n; i++)
    {
        long long pos = keys[i] % SIZE;
        while (tableKey[pos] != 0)
            pos = (pos + 1) % SIZE;
        tableKey[pos] = keys[i];
        tableStr[pos] = strs[i];
    }

    // ----- Search each target and write its result file. -----
    long long targets[2] = {FOUND_TARGET, NOT_FOUND_TARGET};
    for (int t = 0; t < 2; t++)
    {
        long long target = targets[t];

        // Probe from the home slot until we find the key or hit an empty slot.
        long long pos = target % SIZE;
        bool found = false;
        string foundStr;
        while (tableKey[pos] != 0)
        {
            if (tableKey[pos] == target)
            {
                found = true;
                foundStr = tableStr[pos];
                break;
            }
            pos = (pos + 1) % SIZE;
        }

        // Build the output line in the exact sample format.
        string filename = "dataset_" + to_string(n) +
                          "_hash_table_search_step_" + to_string(target) + ".txt";
        ofstream out(filename);
        if (found)
            out << target << " = " << target << "/" << foundStr << "\n";
        else
            out << "-1 != " << target << "\n";
        out.close();

        // Echo to the console as well.
        if (found)
            cout << target << " = " << target << "/" << foundStr << "\n";
        else
            cout << "-1 != " << target << "\n";
        cout << "(Written to " << filename << ")\n";
    }

    return 0;
}
