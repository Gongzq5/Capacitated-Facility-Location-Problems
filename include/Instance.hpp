#ifndef _INSTANCE_HPP_
#define _INSTANCE_HPP_

#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

class Problem {
public:
    Problem static constructFromFile(const char* filename)  {
        int J, I, tmp1, tmp2;
        float tmp3;
        Problem problem;
        ifstream ifs(filename);
        ifs >> problem.facilityNum >> problem.customerNum;
        for (int i=0; i<problem.facilityNum; i++) {
            ifs >> tmp1 >> tmp2;
            problem.capacity.push_back(tmp1);
            problem.openingCost.push_back(tmp2);
        }
        for (int i=0; i<problem.customerNum; i++) {
            ifs >> tmp3;
            problem.demandOfCustomer.push_back((int)tmp3);
        }
        for (int i=0; i<problem.facilityNum; i++) {        
            problem.assignmentCost.push_back(vector<int> ());
            for (int j=0; j<problem.customerNum; j++) {
                ifs >> tmp3;   
                problem.assignmentCost[i].push_back((int)tmp3);
            }
        }
        return problem;        
    }

    int customerNum;
    int facilityNum;
    vector<int> capacity;
    vector<int> openingCost;
    vector<int> demandOfCustomer;
    vector<vector<int>> assignmentCost;

    void show() {
        std::cout << facilityNum << " " << customerNum << std::endl;
        for (int i = 0; i < facilityNum; i++) {
            std::cout << capacity[i] << " " << openingCost[i] << std::endl;
        }
        for (int i = 0; i < customerNum; i++) {
            std::cout << demandOfCustomer[i] << "\t";
            if (0 == (i+1)%10) {
                std::cout << std::endl;
            }
        }
        std::cout << std::endl;
        for (int i = 0; i < facilityNum; i++) {
            for (int j = 0; j < customerNum; j++) {
                std::cout << assignmentCost[i][j] << "\t";
                if (0 == (j+1)%10) {
                    std::cout << std::endl;
                }
            }
        }
    }

private:
    Problem(int facilityNum, int customerNum, vector<int> capacity, vector<int> openingCost, 
            vector<int> demandOfCustomer, vector<vector<int>> assignmentCost)  {
        this->facilityNum = facilityNum;
        this->customerNum = customerNum;
        this->capacity = capacity;
        this->openingCost = openingCost;
        this->demandOfCustomer = demandOfCustomer;
        this->assignmentCost = assignmentCost;
    }
    Problem() {}
};

#endif