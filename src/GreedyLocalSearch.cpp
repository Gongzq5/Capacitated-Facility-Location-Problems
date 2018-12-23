#include <bits/stdc++.h>
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
    int start = time(NULL);
    srand(time(NULL));
    Problem instance = Problem::constructFromFile(fileName);
    // cout << "Construct over" << endl;
    Solution solution = greedyInit(instance);
    // cout << "Greedy init " << solution.calculateCost() << endl;
    int cost = solution.calculateCost();
    int minCost = cost;
    Solution minSolution = solution;
    int problemSize = instance.customerNum * instance.facilityNum;
    int noChange = 0;
    float badAception = 0;
    for (int _i; _i < problemSize * 2000; _i++) {
        Solution tmp = solution.generateNewSolution();
        int tmpCost = tmp.calculateCost();
        if (tmpCost < cost) {
            cost = tmpCost;
            solution = tmp;
            noChange = 0;
            badAception = 0;
        } else {
            if ((float) (rand()%100)/100 < badAception) {
                cost = tmpCost;
                solution = tmp;
                noChange = 0;
                badAception = 0;
            } else if (noChange > problemSize) {
                badAception += 0.0001;
                noChange = 0;
            }
        }
        if (cost < minCost) {
            minSolution = solution;
            minCost = cost;
        }
        noChange++;
    }

    cout << "Used " << time(nullptr) - start << "s" << endl;
    minSolution.show();
    return 0;
}