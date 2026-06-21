// *********************************************************
// Program: radix_sort.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC4L
// Tutorial Class: T13L
// Trimester: 2610
// Member_1: 242UC244M6  | YAP HUI CHI         | YAP.HUI.CHI@STUDENT.MMU.EDU.MY        | 0163225726
// Member_2: 243UC247CQ  | ERIC CHIN YAN HONG  | ERIC.CHIN.YAN.HONG@STUDENT.MMU.EDU.MY | 0168262342
// Member_3: 251UC25052  | KOO XIU SHEN        | KOO.XIU.SHEN@STUDENT.MMU.EDU.MY       | 01140454502
// *********************************************************
//
// LSD radix sort, base 10, sorts records by the 10-digit integer key.
//
// Compile: g++ -std=c++17 -O2 radix_sort.cpp -o radix_sort
// Usage:   ./radix_sort <input_csv>
// Output:  result/radix_sorted_<input_csv>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <filesystem>
using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " <input_csv>\n";
        return 1;
    }

    string in_name = argv[1];
    ifstream in(in_name);
    if (!in)
    {
        cerr << "Error: cannot open " << in_name << "\n";
        return 1;
    }

    // Read CSV (before the timer; I/O excluded).
    vector<long long> keys;
    vector<string> strs;
    string line;
    while (getline(in, line))
    {
        if (line.empty())
            continue;
        size_t comma = line.find(',');
        long long k = stoll(line.substr(0, comma));
        string s = line.substr(comma + 1);
        keys.push_back(k);
        strs.push_back(s);
    }
    in.close();
    long long n = (long long)keys.size();

    // Reused buffers.
    vector<long long> tmp_keys(n);
    vector<string> tmp_strs(n);
    long long count[10];

    // Time only the sort.
    auto t_start = chrono::high_resolution_clock::now();

    long long divisor = 1;
    for (int pass = 0; pass < 10; pass++)
    {
        // Count each digit (0..9) at this position.
        for (int i = 0; i < 10; i++)
            count[i] = 0;
        for (long long i = 0; i < n; i++)
        {
            int d = (int)((keys[i] / divisor) % 10);
            count[d]++;
        }
        // Prefix sum.
        for (int i = 1; i < 10; i++)
            count[i] += count[i - 1];
        // Stable distribution, right-to-left.
        for (long long i = n - 1; i >= 0; i--)
        {
            int d = (int)((keys[i] / divisor) % 10);
            count[d]--;
            tmp_keys[count[d]] = keys[i];
            tmp_strs[count[d]] = strs[i];
        }
        // O(1) vector swap.
        swap(keys, tmp_keys);
        swap(strs, tmp_strs);
        divisor *= 10;
    }

    auto t_end = chrono::high_resolution_clock::now();
    double seconds = chrono::duration<double>(t_end - t_start).count();

    // Output to result/radix_sorted_<basename>.
    filesystem::create_directories("result");
    size_t slash = in_name.find_last_of("/\\");
    string base = (slash == string::npos) ? in_name : in_name.substr(slash + 1);
    string out_name = "result/radix_sorted_" + base;

    ofstream out(out_name);
    for (long long i = 0; i < n; i++)
    {
        out << keys[i] << "," << strs[i] << "\n";
    }
    out.close();

    cout << "Sorted " << n << " rows in " << seconds << " seconds.\n";
    cout << "Output: " << out_name << endl;
    return 0;
}
