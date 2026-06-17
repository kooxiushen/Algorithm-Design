// *********************************************************
// Program: hash_table_search.cpp
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
// integer key, then measures the running time of the search operation
// for the best, average and worst cases.
//
// A single search is too fast to measure, so each case performs n
// searches (n = number of rows in the dataset). Only the search loop
// is timed; reading the file and building the table are excluded.
//
//   Best case    : search a key that sits in its home slot (no probing).
//   Average case : search every key in the dataset once.
//   Worst case   : search the key with the longest probe sequence.
//
// Compile: g++ -std=c++17 -O2 hash_table_search.cpp -o hash_table_search
// Usage:   ./hash_table_search <dataset_file.csv>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
using namespace std;

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

    // ----- Read the dataset into memory (this is I/O, NOT timed). -----
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

    // ----- Build the hash table (linear probing). NOT timed. -----
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

    // ----- Find a best-case key and a worst-case key. NOT timed. -----
    // The probe distance of a key = how many slots we walk from its home
    // slot before we find it. Distance 0 means it sits in its home slot.
    long long bestKey = keys[0];  // smallest probe distance (best case)
    long long worstKey = keys[0]; // largest probe distance (worst case)
    long long minDist = -1, maxDist = -1;

    for (long long i = 0; i < n; i++)
    {
        long long home = keys[i] % SIZE;
        long long pos = home;
        long long dist = 0;
        while (tableKey[pos] != keys[i])
        {
            pos = (pos + 1) % SIZE;
            dist++;
        }
        if (minDist == -1 || dist < minDist)
        {
            minDist = dist;
            bestKey = keys[i];
        }
        if (maxDist == -1 || dist > maxDist)
        {
            maxDist = dist;
            worstKey = keys[i];
        }
    }

    // sink stops the optimizer from deleting the timed search loops,
    // since otherwise their results would look unused.
    volatile long long sink = 0;

    // ----- BEST CASE: search the best key n times. (TIMED) -----
    auto startBest = chrono::high_resolution_clock::now();
    for (long long i = 0; i < n; i++)
    {
        long long pos = bestKey % SIZE;
        while (tableKey[pos] != 0)
        {
            if (tableKey[pos] == bestKey)
                break; // found
            pos = (pos + 1) % SIZE;
        }
        sink += pos;
    }
    auto endBest = chrono::high_resolution_clock::now();

    // ----- AVERAGE CASE: search every key in the dataset once. (TIMED) -----
    auto startAvg = chrono::high_resolution_clock::now();
    for (long long i = 0; i < n; i++)
    {
        long long key = keys[i];
        long long pos = key % SIZE;
        while (tableKey[pos] != 0)
        {
            if (tableKey[pos] == key)
                break; // found
            pos = (pos + 1) % SIZE;
        }
        sink += pos;
    }
    auto endAvg = chrono::high_resolution_clock::now();

    // ----- WORST CASE: search the worst key n times. (TIMED) -----
    auto startWorst = chrono::high_resolution_clock::now();
    for (long long i = 0; i < n; i++)
    {
        long long pos = worstKey % SIZE;
        while (tableKey[pos] != 0)
        {
            if (tableKey[pos] == worstKey)
                break; // found
            pos = (pos + 1) % SIZE;
        }
        sink += pos;
    }
    auto endWorst = chrono::high_resolution_clock::now();

    (void)sink; // value itself is unimportant; it just blocks optimization

    // ----- Running times in seconds (for the whole batch of n searches). -----
    double bestTime = chrono::duration<double>(endBest - startBest).count();
    double avgTime = chrono::duration<double>(endAvg - startAvg).count();
    double worstTime = chrono::duration<double>(endWorst - startWorst).count();

    // ----- Write the result file and print to the console. -----
    string filename = "hash_table_search_dataset_" + to_string(n) + ".txt";
    ofstream out(filename);
    out.setf(ios::fixed);
    out.precision(6);
    out << "Best case time: " << bestTime << " seconds\n";
    out << "Average case time: " << avgTime << " seconds\n";
    out << "Worst case time: " << worstTime << " seconds\n";
    out.close();

    cout.setf(ios::fixed);
    cout.precision(6);
    cout << "Best case time: " << bestTime << " seconds\n";
    cout << "Average case time: " << avgTime << " seconds\n";
    cout << "Worst case time: " << worstTime << " seconds\n";
    cout << "(Results written to " << filename << ")\n";

    return 0;
}
