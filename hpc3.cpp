#include <iostream>     // for input-output (cin, cout)
#include <vector>       // for dynamic array (vector)
#include <omp.h>        // for OpenMP parallel programming
using namespace std;

int main() {
    int n;

    // 🔹 Take number of elements from user
    cout << "Enter number of elements: ";
    cin >> n;

    // 🔹 Create vector of size n
    vector<int> arr(n);

    // 🔹 Take array elements as input
    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++)
        cin >> arr[i];

    // 🔹 Initialize variables
    int sum = 0;            // to store sum
    int min_val = arr[0];   // initialize min with first element
    int max_val = arr[0];   // initialize max with first element

    // 🔥 Parallel Reduction using OpenMP
    // Each thread calculates partial results and OpenMP combines them
    #pragma omp parallel for reduction(+:sum) reduction(min:min_val) reduction(max:max_val)
    for (int i = 0; i < n; i++) {

        // 🔹 Add elements to sum
        sum += arr[i];

        // 🔹 Find minimum value
        if (arr[i] < min_val)
            min_val = arr[i];

        // 🔹 Find maximum value
        if (arr[i] > max_val)
            max_val = arr[i];
    }

    // 🔹 Calculate average
    double avg = (double)sum / n;

    // 🔹 Print results
    cout << "\nSum = " << sum;
    cout << "\nMin = " << min_val;
    cout << "\nMax = " << max_val;
    cout << "\nAverage = " << avg << endl;

    return 0;   // end of program
}

/*
g++ -fopenmp -std=c++11 hpc3.cpp -o hpc3
./hpc3
*/