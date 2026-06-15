//Heap_Sort_step
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <chrono>

using namespace std;

typedef pair<unsigned long long, string> HeapNode;

void heapify(vector<HeapNode>&, int , int);

void printHeapStep(const vector<HeapNode>& arr, ofstream& out, string suffix) {
    out << "[";
    for (size_t j = 0; j < arr.size(); ++j) {
        out << arr[j].first << "/" << arr[j].second;
        if (j < arr.size() - 1) {
            out << ", ";
            // Wrap text every 3 items to match sample output
            if ((j + 1) % 3 == 0) out << "\n";
        }
    }
    out << "] " << suffix << "\n";
}

void swapNodes(HeapNode& a, HeapNode& b){
  HeapNode temp = a;
  a = b;
  b = temp;
}

void heapSort(vector<HeapNode>& arr,ofstream& stepFile){
  int maxHeapSize = arr.size();

  //Initializing the heapsort from the last parent with last leaf node and going back one by one
  for(int i = maxHeapSize/2 -1; i>=0; i--){
    heapify(arr, maxHeapSize, i);
  }

  printHeapStep(arr, stepFile, "initial");

  //Extract element from heap
  for(int i = maxHeapSize -1;i>0;i--){
    //Move largest value into the end of the arr
    swapNodes(arr[0],arr[i]);

    //call maxheap function to reduce the heap.
    heapify(arr,i,0);

    printHeapStep(arr, stepFile, "i = " + to_string(i));
  }

}


void heapify(vector<HeapNode>& arr, int n, int i){
  int parent = i; //largest
  int leftChild = 2*i +1;
  int rightChild = 2*i +2;

  if(leftChild < n && arr[leftChild].first > arr[parent].first){
    parent = leftChild;
  }
  if(rightChild < n && arr[rightChild].first > arr[parent].first){
    parent = rightChild;
  }
  if(parent != i){
    swapNodes(arr[i], arr[parent]);

    heapify(arr, n, parent);
  }
}


int main() {
  // Tutor specifies these variables
    int startRow = 1;     // Starting row to read
    int endRow = 7;       // Ending row to read

    string inputFileName = "dataset_30.csv";
    ifstream file(inputFileName);
    if (!file.is_open()) {
        cerr << "Error: Could not open the file." << endl;
        return 1;
    }

    vector<HeapNode> dataVect;
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string keyStr, value;

        if (getline(ss, keyStr, ',') && getline(ss, value)) {
            unsigned long long key = stoull(keyStr);
            dataVect.push_back({key, value}); // Insert directly into vector
        }
    }
    file.close();

    for (int i = 0;i < dataVect.size() || i < 20; i++) {
    cout << "Key: " << dataVect[i].first << " | Value: " << dataVect[i].second << endl;
    }

    // Create the text file for the step-by-step output
    string stepFileName = "dataset_" + to_string(dataVect.size()) + "_heap_sorted_step_" + to_string(startRow) + "_" + to_string(endRow) + ".txt";
    ofstream stepFile(stepFileName);

    if (!stepFile.is_open()) {
        cerr << "Error: Could not create step output file." << endl;
        return 1;
    }

    // Start the timer
    auto start = chrono::high_resolution_clock::now();

    heapSort(dataVect, stepFile);

    // Stop the timer
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> runtime = end - start;
    
    stepFile.close();
    cout << "Step-by-step trace saved to " << stepFileName << endl;
    cout << "Runtime: " << runtime.count() << " ms" << endl;

    return 0;
}