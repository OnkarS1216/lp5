#include <iostream>     // for input-output (cout)
#include <vector>       // for dynamic arrays (vector)
#include <cstdlib>      // for rand()
#include <ctime>        // for time()
#include <omp.h>        // for OpenMP (parallel programming)

using namespace std;

#define SIZE 10000      // size of array (large to show performance difference)

// 🔹 Generate Random Data
void generateRandom(vector<int>& arr) {
    // fill array with random values (0 to 99999)
    for (int &x : arr)
        x = rand() % 100000;
}

// ===============================
// 🔵 Sequential Bubble Sort
// ===============================
void bubbleSortSeq(vector<int>& arr) {
    int n = arr.size();   // get size of array

    // outer loop → number of passes
    for (int i = 0; i < n - 1; i++) {

        // inner loop → compare adjacent elements
        for (int j = 0; j < n - i - 1; j++) {

            // if elements are in wrong order → swap
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
        }
    }
}

// ===============================
// 🔵 Parallel Bubble Sort
// (Odd-Even Transposition Sort)
// ===============================
void bubbleSortParallel(vector<int>& arr) {
    int n = arr.size();

    // perform n phases
    for (int i = 0; i < n; i++) {

        // 🔸 EVEN PHASE
        // compare (0,1), (2,3), (4,5) in parallel
        #pragma omp parallel for
        for (int j = 0; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
        }

        // 🔸 ODD PHASE
        // compare (1,2), (3,4), (5,6) in parallel
        #pragma omp parallel for
        for (int j = 1; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
        }
    }
}

// ===============================
// 🔴 Merge Function
// ===============================
void merge(vector<int>& arr, int l, int m, int r) {

    int n1 = m - l + 1;   // size of left subarray
    int n2 = r - m;       // size of right subarray

    vector<int> L(n1), R(n2);   // temporary arrays

    // copy data into left array
    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];

    // copy data into right array
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;

    // merge both arrays in sorted order
    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];   // take from left
        else
            arr[k++] = R[j++];   // take from right
    }

    // copy remaining elements (if any)
    while (i < n1)
        arr[k++] = L[i++];

    while (j < n2)
        arr[k++] = R[j++];
}

// ===============================
// 🔴 Sequential Merge Sort
// ===============================
void mergeSortSeq(vector<int>& arr, int l, int r) {

    if (l < r) {   // base condition

        int m = (l + r) / 2;   // find middle

        // recursively sort left half
        mergeSortSeq(arr, l, m);

        // recursively sort right half
        mergeSortSeq(arr, m + 1, r);

        // merge both halves
        merge(arr, l, m, r);
    }
}

// ===============================
// 🔴 Parallel Merge Sort
// ===============================
void mergeSortParallel(vector<int>& arr, int l, int r) {

    if (l < r) {

        int m = (l + r) / 2;   // find middle

        // run left and right halves in parallel
        #pragma omp parallel sections
        {
            #pragma omp section
            mergeSortParallel(arr, l, m);   // left part

            #pragma omp section
            mergeSortParallel(arr, m + 1, r); // right part
        }

        // merge results
        merge(arr, l, m, r);
    }
}

// ===============================
// 🔹 MAIN FUNCTION
// ===============================
int main() {

    vector<int> arr(SIZE), temp;   // main array + temporary copy

    srand(time(0));        // initialize random seed
    generateRandom(arr);   // fill array with random numbers

    double start, end;     // for measuring time

    // 🔵 Sequential Bubble Sort
    temp = arr;   // copy original data
    start = omp_get_wtime();   // start time
    bubbleSortSeq(temp);       // call function
    end = omp_get_wtime();     // end time
    cout << "Sequential Bubble Sort Time: " << (end - start) << " sec\n";

    // 🔵 Parallel Bubble Sort
    temp = arr;
    start = omp_get_wtime();
    bubbleSortParallel(temp);
    end = omp_get_wtime();
    cout << "Parallel Bubble Sort Time: " << (end - start) << " sec\n";

    // 🔴 Sequential Merge Sort
    temp = arr;
    start = omp_get_wtime();
    mergeSortSeq(temp, 0, SIZE - 1);
    end = omp_get_wtime();
    cout << "Sequential Merge Sort Time: " << (end - start) << " sec\n";

    // 🔴 Parallel Merge Sort
    temp = arr;
    start = omp_get_wtime();
    mergeSortParallel(temp, 0, SIZE - 1);
    end = omp_get_wtime();
    cout << "Parallel Merge Sort Time: " << (end - start) << " sec\n";

    return 0;   // program ends
}

/*
g++ -fopenmp -std=c++11 hpc2.cpp -o hpc2
hpc2.exe
 */