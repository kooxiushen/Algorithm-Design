// *********************************************************
// Program: heap_sort_step.cpp
// Course: CCP6214 Algorithm Design and Analysis
// Lecture Class: TC4L
// Tutorial Class: T13L
// Trimester: 2610
// Member_1: 242UC244M6  | YAP HUI CHI         | YAP.HUI.CHI@STUDENT.MMU.EDU.MY        | 0163225726
// Member_2: 243UC247CQ  | ERIC CHIN YAN HONG  | ERIC.CHIN.YAN.HONG@STUDENT.MMU.EDU.MY | 0168262342
// Member_3: 251UC25052  | KOO XIU SHEN        | KOO.XIU.SHEN@STUDENT.MMU.EDU.MY       | 01140454502
// *********************************************************
//
// Max-heap sort on rows [start,end]; traces the array after build and each extract.
//
// Compile: g++ -std=c++17 -O2 heap_sort_step.cpp -o heap_sort_step
// Usage:   ./heap_sort_step <input_csv> <start_row> <end_row>
// Output:  <basename>_heap_sorted_step_<start>_<end>.txt

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

typedef pair<unsigned long long, string> HeapNode;

void heapify(vector<HeapNode> &, long long, long long);

// Print "[k1/s1, k2/s2, ...] label" on one line.
void print_state(ofstream &out, const vector<HeapNode> &arr, const string &label)
{
    out << "[";
    for (size_t i = 0; i < arr.size(); i++)
    {
        if (i > 0) out << ", ";
        out << arr[i].first << "/" << arr[i].second;
    }
    out << "] " << label << "\n";
}

void swapNodes(HeapNode &a, HeapNode &b)
{
    HeapNode temp = a;
    a = b;
    b = temp;
}

void heapSort(vector<HeapNode> &arr, ofstream &stepFile)
{
    long long maxHeapSize = (long long)arr.size();

    // Build the max-heap, then record the initial state.
    for (long long i = maxHeapSize / 2 - 1; i >= 0; i--)
        heapify(arr, maxHeapSize, i);
    print_state(stepFile, arr, "initial");

    // Extract max each step, then record.
    for (long long i = maxHeapSize - 1; i > 0; i--)
    {
        swapNodes(arr[0], arr[i]);
        heapify(arr, i, 0);
        print_state(stepFile, arr, "i = " + to_string(i));
    }
}

void heapify(vector<HeapNode> &arr, long long n, long long i)
{
    long long parent = i;
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

    // Read only rows [start_row, end_row] (1-indexed).
    vector<HeapNode> data;
    string line;
    long long row = 0;
    while (getline(in, line))
    {
        if (line.empty()) continue;
        row++;
        if (row < start_row) continue;
        if (row > end_row) break;
        size_t comma = line.find(',');
        if (comma == string::npos) continue;
        unsigned long long k = stoull(line.substr(0, comma));
        string s = line.substr(comma + 1);
        data.push_back({k, s});
    }
    in.close();
    if (data.empty())
    {
        cerr << "Error: no rows read in range " << start_row << ".." << end_row << "\n";
        return 1;
    }

    // Output filename.
    size_t slash = in_name.find_last_of("/\\");
    string base = (slash == string::npos) ? in_name : in_name.substr(slash + 1);
    if (base.size() >= 4 && base.substr(base.size() - 4) == ".csv")
        base = base.substr(0, base.size() - 4);
    string out_name = base + "_heap_sorted_step_"
                      + to_string(start_row) + "_" + to_string(end_row) + ".txt";

    ofstream out(out_name);
    heapSort(data, out);
    out.close();

    cout << "Wrote step trace to " << out_name << endl;
    return 0;
}
