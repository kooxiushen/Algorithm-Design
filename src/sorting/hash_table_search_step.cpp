// *********************************************************
// Program: hash_table_search_step.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC6L
// Tutorial Class: T22L
// Trimester: 2610
// Member_1: 242UC244M6  | YAP HUI CHI         | YAP.HUI.CHI@STUDENT.MMU.EDU.MY        | 0163225726
// Member_2: 243UC247CQ  | ERIC CHIN YAN HONG  | ERIC.CHIN.YAN.HONG@STUDENT.MMU.EDU.MY | 0168262342
// Member_3: 251UC25052  | KOO XIU SHEN        | KOO.XIU.SHEN@STUDENT.MMU.EDU.MY       | 01140454502
// *********************************************************
//
// Hash table (linear probing). Two outputs:
//  (A) trace one found target + one not-found target.
//  (B) search the 10% query set and write query,string,found,index to search_results.csv.
//
// Compile: g++ -std=c++17 -O2 hash_table_search_step.cpp -o hash_table_search_step
// Usage:   ./hash_table_search_step <dataset_file.csv>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

// Targets (the tutor edits these): one exists, one does not.
const long long FOUND_TARGET = 9794922582LL;     // exists in dataset_1000.csv
const long long NOT_FOUND_TARGET = 123456789LL;  // does not exist (only 9 digits)

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

    // Read the dataset.
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

    // Build the table. Size ~2n prime; slot 0 == empty.
    // tableRow stores each key's original 1-based dataset row number.
    long long SIZE = nextPrime(2 * n + 1);
    vector<long long> tableKey(SIZE, 0);
    vector<string> tableStr(SIZE);
    vector<long long> tableRow(SIZE, 0);

    for (long long i = 0; i < n; i++)
    {
        long long pos = keys[i] % SIZE;
        while (tableKey[pos] != 0)
            pos = (pos + 1) % SIZE;
        tableKey[pos] = keys[i];
        tableStr[pos] = strs[i];
        tableRow[pos] = i + 1; // 1-based row number
    }

    // ===== Output A: trace one found target + one not-found target. =====
    long long targets[2] = {FOUND_TARGET, NOT_FOUND_TARGET};
    for (int t = 0; t < 2; t++)
    {
        long long target = targets[t];

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

        string filename = "dataset_" + to_string(n) +
                          "_hash_table_search_step_" + to_string(target) + ".txt";
        ofstream out(filename);
        if (found)
            out << target << " = " << target << "/" << foundStr << "\n";
        else
            out << "-1 != " << target << "\n";
        out.close();

        if (found)
            cout << target << " = " << target << "/" << foundStr << "\n";
        else
            cout << "-1 != " << target << "\n";
        cout << "(Written to " << filename << ")\n";
    }

    // ===== Output B: search the 10% query set -> search_results.csv. =====
    long long q = n / 10;
    if (q < 1)
        q = 1;
    string queryFile = "hash_search_" + to_string(q) + "_dataset.csv";
    ifstream qin(queryFile);
    if (!qin)
    {
        cerr << "Error: query file " << queryFile
             << " not found. Run hash_table_search first.\n";
        return 1;
    }

    ofstream results("search_results.csv");
    results << "query,string,found,index\n";
    string qline;
    while (getline(qin, qline))
    {
        if (qline.empty())
            continue;
        size_t comma = qline.find(',');
        long long key = stoll(qline.substr(0, comma));
        string qstr = (comma == string::npos) ? "" : qline.substr(comma + 1);

        // Linear-probe search; report 1-based row number if found.
        long long pos = key % SIZE;
        long long foundRow = -1;
        while (tableKey[pos] != 0)
        {
            if (tableKey[pos] == key)
            {
                foundRow = tableRow[pos];
                break;
            }
            pos = (pos + 1) % SIZE;
        }

        if (foundRow != -1)
            results << key << "," << qstr << ",yes," << foundRow << "\n";
        else
            results << key << "," << qstr << ",no,-1\n";
    }
    qin.close();
    results.close();
    cout << "(Per-query comparison written to search_results.csv)\n";

    return 0;
}
