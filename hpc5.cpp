#include <iostream>      // for input/output
#include <vector>        // for dynamic arrays
#include <cmath>         // for sqrt()
#include <algorithm>     // for sort()
#include <omp.h>         // for OpenMP (parallelism)

using namespace std;

// 🔹 Structure to represent a data point
struct Point {
    double x, y;   // coordinates
    int label;     // class label (0 or 1)
};

// 🔹 Function to calculate Euclidean distance
double euclideanDistance(const Point &a, const Point &b) {
    // distance = √((x1-x2)^2 + (y1-y2)^2)
    return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

// 🔹 KNN classification function
int knn(const vector<Point> &dataset, const Point &query, int K) {

    int n = dataset.size();   // total number of data points

    // vector to store (distance, label)
    vector<pair<double, int>> dist(n);

    // 🔵 PARALLEL DISTANCE CALCULATION
    // Each thread computes distance independently
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        dist[i] = {euclideanDistance(dataset[i], query), dataset[i].label};
    }

    // 🔹 Sort distances in ascending order
    sort(dist.begin(), dist.end());

    // 🔹 Count labels among K nearest neighbors
    int count0 = 0, count1 = 0;

    // 🔵 PARALLEL VOTING USING REDUCTION
    #pragma omp parallel for reduction(+:count0, count1)
    for (int i = 0; i < K; i++) {
        if (dist[i].second == 0)
            count0++;   // class 0
        else
            count1++;   // class 1
    }

    // 🔹 Return class with majority votes
    return (count1 > count0) ? 1 : 0;
}

int main() {

    // 🔹 Dataset (training data)
    vector<Point> dataset = {
        {1,2,0}, {2,3,0}, {3,1,0},   // Class 0
        {6,5,1}, {7,7,1}, {8,6,1}    // Class 1
    };

    // 🔹 Query point (unknown class)
    Point query = {5, 5, -1};

    int K = 3;   // number of nearest neighbors

    // 🔹 Call KNN function
    int result = knn(dataset, query, K);

    // 🔹 Output result
    cout << "Query Point: (" << query.x << ", " << query.y << ")\n";
    cout << "Predicted Class: " << result << endl;

    return 0;
}