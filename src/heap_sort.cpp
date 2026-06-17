//Heap_Sort
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <chrono>

using namespace std;

typedef pair<unsigned long long, string> HeapNode;

void heapify(vector<HeapNode>&, int , int);

void swapNodes(HeapNode& a, HeapNode& b){
  HeapNode temp = a;
  a = b;
  b = temp;
}

void heapSort(vector<HeapNode>& arr){
  int maxHeapSize = arr.size();

  //Initializing the heapsort from the last parent with last leaf node and going back one by one
  for(int i = maxHeapSize/2 -1; i>=0; i--){
    heapify(arr, maxHeapSize, i);
  }

  //Extract element from heap
  for(int i = maxHeapSize -1;i>0;i--){
    //Move largest value into the end of the arr
    swapNodes(arr[0],arr[i]);

    //call maxheap function to reduce the heap.
    heapify(arr,i,0);

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
    ifstream file("dataset_30.csv");
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

    // Start the timer
    auto start = chrono::high_resolution_clock::now();
  
    //Sort the Data Set
    heapSort(dataVect);

    // Stop the timer
    auto end = chrono::high_resolution_clock::now();

    // Calculate the duration in milliseconds
    chrono::duration<double, milli> runtime = end - start;
    cout << "\nRuntime: " << runtime.count() << " ms" << endl;

    cout<<endl<<"After sorting"<<endl;


    //Print the result array
    for (int i = 0;i < 10; i++) {
    cout << "Key: " << dataVect[i].first << " | Value: " << dataVect[i].second << endl;
    }


    //TODO: Generate a new 
        // Construct the filename using the vector size
    string outFileName = "heap_sort_dataset_" + to_string(dataVect.size()) + ".csv";
    ofstream outFile(outFileName);

    if (outFile.is_open()) {
        for (int i = 0; i < dataVect.size(); i++) {
            outFile << dataVect[i].first << "," << dataVect[i].second << "\n";
        }
        outFile.close();
        cout << "\nSuccess: Sorted data saved to " << outFileName << endl;
    } else {
        cerr << "Error: Could not create output file." << endl;
    }

    return 0;
}