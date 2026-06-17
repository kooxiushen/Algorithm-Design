// *********************************************************
// Program: radix_sort_step.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC4L
// Tutorial Class: T13L
// Trimester: 2610
// Member_1: 242UC244M6  | YAP HUI CHI         | YAP.HUI.CHI@STUDENT.MMU.EDU.MY        | 0163225726
// Member_2: 243UC247CQ  | ERIC CHIN YAN HONG  | ERIC.CHIN.YAN.HONG@STUDENT.MMU.EDU.MY | 0168262342
// Member_3: 251UC25052  | KOO XIU SHEN        | KOO.XIU.SHEN@STUDENT.MMU.EDU.MY       | 01140454502
// *********************************************************
//
// Same LSD radix sort as radix_sort.cpp, but operates on rows
// [start_row, end_row] of the input (1-indexed, inclusive) and
// writes the array state after each digit pass to a .txt file.
// Used to verify algorithm correctness against the PDF samples.
//
// Compile: g++ -std=c++17 -O2 radix_sort_step.cpp -o radix_sort_step
// Usage:   ./radix_sort_step <input_csv> <start_row> <end_row>
// Example: ./radix_sort_step dataset_1000.csv 1 7
// Output:  <input_basename_without_ext>_radix_sorted_step_<start>_<end>.txt

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
using namespace std;

// Print "[k1/s1, k2/s2, ...] <label>" on one line.
void print_state(ofstream &out,
                 const vector<long long> &keys,
                 const vector<string> &strs,
                 const string &label)
{
    out << "[";
    for (size_t i = 0; i < keys.size(); i++)
    {
        if (i > 0) out << ", ";
        out << keys[i] << "/" << strs[i];
    }
    out << "] " << label << "\n";
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        cerr << "Usage: " << argv[0] << " <input_csv> <start_row> <end_row>\n";
        return 1;
    }

    string in_name = argv[1];
    long long start_row = atoll(argv[2]);
    long long end_row = atoll(argv[3]);
    if (start_row < 1 || end_row < start_row)
    {
        cerr << "Error: need 1 <= start_row <= end_row.\n";
        return 1;
    }

    ifstream in(in_name);
    if (!in)
    {
        cerr << "Error: cannot open " << in_name << "\n";
        return 1;
    }

    // Read only rows [start_row, end_row] (1-indexed, inclusive).
    vector<long long> keys;
    vector<string> strs;
    string line;
    long long row = 0;
    while (getline(in, line))
    {
        if (line.empty()) continue;
        row++;
        if (row < start_row) continue;
        if (row > end_row) break;
        size_t comma = line.find(',');
        long long k = stoll(line.substr(0, comma));
        string s = line.substr(comma + 1);
        keys.push_back(k);
        strs.push_back(s);
    }
    in.close();
    long long n = (long long)keys.size();
    if (n == 0)
    {
        cerr << "Error: no rows read in range " << start_row << ".." << end_row << "\n";
        return 1;
    }

    // Build output filename: "result/<basename_without_.csv>_radix_sorted_step_<s>_<e>.txt".
    filesystem::create_directories("result");
    size_t slash = in_name.find_last_of("/\\");
    string base = (slash == string::npos) ? in_name : in_name.substr(slash + 1);
    // Strip a trailing ".csv" if present.
    if (base.size() >= 4 && base.substr(base.size() - 4) == ".csv")
        base = base.substr(0, base.size() - 4);
    string out_name = "result/" + base + "_radix_sorted_step_"
                      + to_string(start_row) + "_" + to_string(end_row) + ".txt";

    ofstream out(out_name);
    print_state(out, keys, strs, "original");

    // LSD radix sort, base 10, 10 passes.
    // Label convention: d=10 is the first pass (rightmost / ones digit),
    // d=1 is the final pass (leftmost / 10^9 digit). Matches PDF sample.
    vector<long long> tmp_keys(n);
    vector<string> tmp_strs(n);
    long long count[10];
    long long divisor = 1;
    for (int d_label = 10; d_label >= 1; d_label--)
    {
        for (int i = 0; i < 10; i++) count[i] = 0;
        for (long long i = 0; i < n; i++)
        {
            int d = (int)((keys[i] / divisor) % 10);
            count[d]++;
        }
        for (int i = 1; i < 10; i++) count[i] += count[i - 1];
        for (long long i = n - 1; i >= 0; i--)
        {
            int d = (int)((keys[i] / divisor) % 10);
            count[d]--;
            tmp_keys[count[d]] = keys[i];
            tmp_strs[count[d]] = strs[i];
        }
        swap(keys, tmp_keys);
        swap(strs, tmp_strs);
        divisor *= 10;
        print_state(out, keys, strs, "d=" + to_string(d_label));
    }
    out.close();

    cout << "Wrote step trace to " << out_name << endl;
    return 0;
}
