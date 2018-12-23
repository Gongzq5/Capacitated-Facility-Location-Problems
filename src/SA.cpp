#include <bits/stdc++.h>

#include "../include/Solution.hpp"
#include "../include/Instance.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    char* fileName = argv[1];
    cout << "File : " << fileName << endl;
    int start = time(NULL);
    srand(time(NULL));
    Problem instance = Problem::constructFromFile(fileName);
    Solution solution(&instance);

    Solution minSolution(solution);
    int minCost = INT_MAX;
    int noChangeTimes = 0;
    int currCost = solution.calculateCost();
    int problemSize = instance.customerNum * instance.facilityNum;
    float T = 20;
    float innerLoop = problemSize;
    float thseardHolder = T/2;
    int noChangeTimesCount = 0;
    while (T > 10e-3) {
        for (int i = 0; i < innerLoop; i++) {
            Solution re = solution.generateNewSolution();
            int innerCost = re.calculateCost();
            if (innerCost < currCost) {
                solution = re;
                currCost = innerCost;
                if (currCost < minCost) {
                    minCost = currCost;
                    minSolution = solution;
                }
            } else if (exp((float)(currCost-innerCost)/(T*5)) > (float) (rand()%100)/100) {
                solution = re;
                currCost = innerCost;
            } else {
                noChangeTimes++;                
            }
        }
        if (noChangeTimes >= problemSize * 500) {
            noChangeTimes = 0;
            noChangeTimesCount++;
            if (T < thseardHolder) {
                T = thseardHolder;
                thseardHolder /= 2;
            }
            if (noChangeTimesCount == problemSize / 50) {
                solution = Solution(&instance);
                currCost = solution.calculateCost();
            }
        }
        T *= 0.99;
        innerLoop *= 1.00001;
        
        // cout << T << " " << currCost << endl;
    }
    int end = time(NULL);
    cout << "Use " << end-start << " s" << endl << endl;
    minSolution.trim();
    minSolution.show();
    return 0;
}
