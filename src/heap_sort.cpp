//Heap_Sort
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

typedef pair<unsigned long long, string> HeapNode;



//TODO: Get root node
//TODO: Get parent node
//TODO: Get left child
//TODO: Get right child
//TODO: heap_delete
//TODO: heap_insert
//TODO: Heapify
//TODO: Print Runtime
//TODO: Create heap_sort_dataset_n.csv

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
    ifstream file("dataset_20.csv");
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

    heapSort(dataVect);

    cout<<endl<<"After sorting"<<endl;


    //Print the result array
    for (int i = 0;i < dataVect.size()|| i < 20; i++) {
    cout << "Key: " << dataVect[i].first << " | Value: " << dataVect[i].second << endl;
    }

    return 0;
}