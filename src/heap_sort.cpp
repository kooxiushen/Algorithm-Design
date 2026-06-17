// *********************************************************
// Program: heap_sort.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC4L
// Tutorial Class: T13L
// Trimester: 2610
// Member_1: 242UC244M6  | YAP HUI CHI         | YAP.HUI.CHI@STUDENT.MMU.EDU.MY        | 0163225726
// Member_2: 243UC247CQ  | ERIC CHIN YAN HONG  | ERIC.CHIN.YAN.HONG@STUDENT.MMU.EDU.MY | 0168262342
// Member_3: 251UC25052  | KOO XIU SHEN        | KOO.XIU.SHEN@STUDENT.MMU.EDU.MY       | 01140454502
// *********************************************************
//
// In-place max-heap sort, ascending by the 10-digit integer key.
// The 5-letter string is carried along as payload.
//
// Compile: g++ -std=c++17 -O2 heap_sort.cpp -o heap_sort
// Usage:   ./heap_sort <input_csv>
// Example: ./heap_sort dataset_1000.csv
// Output:  heap_sorted_<input_csv>  (in the current directory)

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
using namespace std;

typedef pair<unsigned long long, string> HeapNode;

void heapify(vector<HeapNode> &, long long, long long);

void swapNodes(HeapNode &a, HeapNode &b)
{
    HeapNode temp = a;
    a = b;
    b = temp;
}

void heapSort(vector<HeapNode> &arr)
{
    long long maxHeapSize = (long long)arr.size();

    // Build the max-heap: sift down from the last parent back to the root.
    for (long long i = maxHeapSize / 2 - 1; i >= 0; i--)
        heapify(arr, maxHeapSize, i);

    // Repeatedly move the largest (root) to the end, shrink, re-heapify.
    for (long long i = maxHeapSize - 1; i > 0; i--)
    {
        swapNodes(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

void heapify(vector<HeapNode> &arr, long long n, long long i)
{
    long long parent = i; // index of the largest of the three
    long long leftChild = 2 * i + 1;
    long long rightChild = 2 * i + 2;

    if (leftChild < n && arr[leftChild].first > arr[parent].first)
        parent = leftChild;
    if (rightChild < n && arr[rightChild].first > arr[parent].first)
        parent = rightChild;
    if (parent != i)
    {
        swapNodes(arr[i], arr[parent]);
        heapify(arr, n, parent);
    }
}

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

    // Read CSV: each line is "<10-digit int>,<5-letter string>".
    // Read BEFORE the timer starts (I/O is excluded from timing).
    vector<HeapNode> data;
    string line;
    while (getline(in, line))
    {
        if (line.empty()) continue;
        size_t comma = line.find(',');
        if (comma == string::npos) continue;
        unsigned long long k = stoull(line.substr(0, comma));
        string s = line.substr(comma + 1);
        data.push_back({k, s});
    }
    in.close();
    long long n = (long long)data.size();

    // --- Time only the sort ---
    auto t_start = chrono::high_resolution_clock::now();
    heapSort(data);
    auto t_end = chrono::high_resolution_clock::now();
    double seconds = chrono::duration<double>(t_end - t_start).count();
    // --- End of timed section ---

    // Build output filename: "heap_sorted_<basename of input>" in the current dir.
    size_t slash = in_name.find_last_of("/\\");
    string base = (slash == string::npos) ? in_name : in_name.substr(slash + 1);
    string out_name = "heap_sorted_" + base;

    ofstream out(out_name);
    for (long long i = 0; i < n; i++)
        out << data[i].first << "," << data[i].second << "\n";
    out.close();

    cout << "Sorted " << n << " rows in " << seconds << " seconds.\n";
    cout << "Output: " << out_name << endl;
    return 0;
}
