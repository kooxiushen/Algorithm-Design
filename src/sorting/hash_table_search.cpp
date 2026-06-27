// *********************************************************
// Program: hash_table_search.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC6L
// Tutorial Class: T22L
// Trimester: 2610
// Member_1: 242UC244M6  | YAP HUI CHI         | YAP.HUI.CHI@STUDENT.MMU.EDU.MY        | 0163225726
// Member_2: 243UC247CQ  | ERIC CHIN YAN HONG  | ERIC.CHIN.YAN.HONG@STUDENT.MMU.EDU.MY | 0168262342
// Member_3: 251UC25052  | KOO XIU SHEN        | KOO.XIU.SHEN@STUDENT.MMU.EDU.MY       | 01140454502
// *********************************************************
//
// Hash table (linear probing) on the integer key; times the search
// operation for the best, average and worst cases (n searches each).
// Also writes a query set = the first 10% of the dataset.
//
// Compile: g++ -std=c++17 -O2 hash_table_search.cpp -o hash_table_search
// Usage:   ./hash_table_search <dataset_file.csv>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
using namespace std;

// Smallest prime >= x (spreads keys more evenly).
long long nextPrime(long long x)
{
    if (x <= 2)
        return 2;
    if (x % 2 == 0)
        x++;
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
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " <dataset_file.csv>\n";
        return 1;
    }

    // Read the dataset (not timed).
    ifstream in(argv[1]);
    if (!in)
    {
        cerr << "Error: could not open " << argv[1] << "\n";
        return 1;
    }

    vector<long long> keys;
    vector<string> strs;

    string line;
    while (getline(in, line))
    {
        if (line.empty())
            continue;
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

    // Generate the query set = first 10% of the dataset (key,value rows).
    long long q = n / 10;
    if (q < 1)
        q = 1;
    string queryFile = "hash_search_" + to_string(q) + "_dataset.csv";
    ofstream qout(queryFile);
    for (long long i = 0; i < q; i++)
        qout << keys[i] << "," << strs[i] << "\n";
    qout.close();

    // Build the table (not timed). Size ~2n prime; slot 0 == empty.
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

    // Find best-case (shortest probe) and worst-case (longest probe) keys (not timed).
    long long bestKey = keys[0];
    long long worstKey = keys[0];
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

    // Blocks the optimizer from deleting the timed loops.
    volatile long long sink = 0;

    // BEST CASE: search the best key n times. (TIMED)
    auto startBest = chrono::high_resolution_clock::now();
    for (long long i = 0; i < n; i++)
    {
        long long pos = bestKey % SIZE;
        while (tableKey[pos] != 0)
        {
            if (tableKey[pos] == bestKey)
                break;
            pos = (pos + 1) % SIZE;
        }
        sink += pos;
    }
    auto endBest = chrono::high_resolution_clock::now();

    // AVERAGE CASE: search every key once. (TIMED)
    auto startAvg = chrono::high_resolution_clock::now();
    for (long long i = 0; i < n; i++)
    {
        long long key = keys[i];
        long long pos = key % SIZE;
        while (tableKey[pos] != 0)
        {
            if (tableKey[pos] == key)
                break;
            pos = (pos + 1) % SIZE;
        }
        sink += pos;
    }
    auto endAvg = chrono::high_resolution_clock::now();

    // WORST CASE: search the worst key n times. (TIMED)
    auto startWorst = chrono::high_resolution_clock::now();
    for (long long i = 0; i < n; i++)
    {
        long long pos = worstKey % SIZE;
        while (tableKey[pos] != 0)
        {
            if (tableKey[pos] == worstKey)
                break;
            pos = (pos + 1) % SIZE;
        }
        sink += pos;
    }
    auto endWorst = chrono::high_resolution_clock::now();

    (void)sink;

    // Running times in seconds (whole batch of n searches).
    double bestTime = chrono::duration<double>(endBest - startBest).count();
    double avgTime = chrono::duration<double>(endAvg - startAvg).count();
    double worstTime = chrono::duration<double>(endWorst - startWorst).count();

    // Write result file and print.
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
    cout << "(Query set of " << q << " rows written to " << queryFile << ")\n";

    return 0;
}
