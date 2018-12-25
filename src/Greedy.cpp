#include <bits/stdc++.h>
#include <chrono>
#include "../include/Instance.hpp"
#include "../include/Solution.hpp"

using namespace std;

Solution greedyInit(Problem& instance) {
    int customerNum = instance.customerNum;
    int facilityNum = instance.facilityNum;
    vector<int> capacity = instance.capacity;
    vector<int> openingCost = instance.openingCost;
    vector<int> demandOfCustomer = instance.demandOfCustomer;
    vector<vector<int>> assignmentCost = instance.assignmentCost;
    
    Solution solution(&instance);
    int cost;
    vector<bool> openingStates(facilityNum, false);
    vector<int> assignments(customerNum, -1);
    vector<int> leftCapacities(capacity);
    for (int i = 0; i < customerNum; i++) {
        // 对于每个人，都去离他最近的打开的仓库
        int minCost = INT_MAX, minIndex = -1;
        for (int j = 0; j < facilityNum; j++) {
            int cost = openingStates[j] ? assignmentCost[j][i] : openingCost[j] + assignmentCost[j][i];
            if (cost < minCost && leftCapacities[j] > demandOfCustomer[i]) {
                minCost = cost;
                minIndex = j;
            }
        }
        if (minIndex == -1) {
            return solution;
        } else {
            leftCapacities[minIndex] -= demandOfCustomer[i];
            openingStates[minIndex] = true;
            assignments[i] = minIndex;
        }
    }
    solution.openingStates = openingStates;
    solution.assignments = assignments;
    solution.leftCapacities = leftCapacities;
    return solution;
}

int main(int argc, char* argv[]) {
    char* fileName = argv[1];
    cout << "File : " << fileName << endl;
    auto start = chrono::system_clock::now();
    srand(time(NULL));
    Problem instance = Problem::constructFromFile(fileName);
    Solution solution = greedyInit(instance);
    
    auto end   = chrono::system_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    cout <<  "Used " << double(duration.count()) * chrono::microseconds::period::num / chrono::microseconds::period::den << "s" << endl;
    solution.show();
    
    return 0;
}