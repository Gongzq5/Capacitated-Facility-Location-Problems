#ifndef _SOLUTION_HPP_
#define _SOLUTION_HPP_

#include "Instance.hpp"
#include <memory.h>
#include <iostream>
using namespace std;

class Solution {
public:
    Problem *instance;
    int cost;
    vector<bool> openingStates;
    vector<int> assignments;
    vector<int> leftCapacities;

    Solution(Problem* instance) {
        this->instance = instance;
        this->cost = 0;
        this->assignments.assign(instance->customerNum, 0);
        this->openingStates.assign(instance->facilityNum, false);
        this->leftCapacities.assign(instance->capacity.begin(), instance->capacity.end());
        // 随机+贪心生成一个解
        for (int i = 0; i < this->instance->customerNum; i++) {            
            // 客户i
            for (int j = 0; j < this->instance->facilityNum; j++) {                
                // 工厂j
                if (this->leftCapacities[j] > this->instance->demandOfCustomer[i]) {                    
                    this->leftCapacities[j] -= this->instance->demandOfCustomer[i];
                    assignments[i] = j;
                    openingStates[j] = true;
                    break;
                }                
            }            
        }        
    }

    Solution(const Solution& solution) {
        instance = solution.instance;
        cost = solution.cost;
        openingStates.assign(solution.openingStates.begin(), solution.openingStates.end());
        assignments.assign(solution.assignments.begin(), solution.assignments.end());
        leftCapacities.assign(solution.leftCapacities.begin(), solution.leftCapacities.end());
    }

    int calculateCost() {
        int cost = 0;
        // get the opening cost
        for (int i = 0; i < instance->facilityNum; i++) {
            if (openingStates[i]) {
                cost += instance->openingCost[i];
            }
        }
        // get the assignment cost
        for (int i = 0; i < instance->customerNum; i++) {
            cost += instance->assignmentCost[assignments[i]][i];
        }
        return cost;
    }

    void trim() {
        vector<int> left(instance->capacity.begin(), instance->capacity.end());        
        for (int i = 0; i < instance->customerNum; i++) {
            left[assignments[i]] -= instance->demandOfCustomer[i];
        }
        for (int i = 0; i < instance->customerNum; i++) {
            if (left[assignments[i]] == instance->capacity[assignments[i]]) {
                cout << "Change state to close <== " << assignments[i] << endl;
                this->openingStates[assignments[i]] = false;
            }
        }
        leftCapacities = left;        
    }


    bool isValid() {
        vector<int> left(instance->capacity.begin(), instance->capacity.end());        
        for (int i = 0; i < instance->customerNum; i++) {
            left[assignments[i]] -= instance->demandOfCustomer[i];
            if (left[assignments[i]] < 0 || !openingStates[assignments[i]]) {
                cerr << "====>>> " << assignments[i] << " " << left[assignments[i]] << endl;
                cerr << "====>>> " << openingStates[assignments[i]] << endl;
                return false;
            }
        }
        leftCapacities = left;
        return true;
    }

    Solution generateNewSolution();

    void show() {
        cout << calculateCost() << endl << endl;
        for (int i = 0; i < this->instance->facilityNum; i++) {
            std::cout << this->openingStates[i] << " ";
        }
        cout << endl;
        for (int i = 0; i < this->instance->customerNum; i++) {
            std::cout << this->assignments[i] << " ";
            // if ((i+1)%10 == 0) std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    // 将一个人挪到另一个仓库里
    Solution generateMethod1() {
        Solution res(*this);
        // 让一个用户换一个仓库
        // 设置Over控制while次数，因为可能找不到替换的人
        // 如果找到了替换的就把Over设为0，退出循环
        int over = 100;
        while (over--) {
            int luckyDog = rand()%instance->customerNum;
            int init = rand()%instance->facilityNum;
            if (res.openingStates[init] && res.leftCapacities[init] > instance->demandOfCustomer[luckyDog]) {
                // 随机仓库Init可以接收这个luck dog
                res.leftCapacities[init] -= instance->demandOfCustomer[luckyDog];
                res.leftCapacities[res.assignments[luckyDog]] += instance->demandOfCustomer[luckyDog];
                // 如果当前可以，那么判断之前的是不是变成了空的，是的话就关掉
                if (res.leftCapacities[res.assignments[luckyDog]] == instance->capacity[res.assignments[luckyDog]]) {
                    res.openingStates[res.assignments[luckyDog]] = false;
                }
                // 切换仓库
                res.assignments[luckyDog] = init;
                over = 0;
            } else {
                // 不可以就循环查找，直到找到一个可以接收的仓库i
                for (int i = init+1; i != init; i=(i+1)%instance->facilityNum) {
                    if (res.openingStates[i] && res.leftCapacities[i] > instance->demandOfCustomer[luckyDog]) {
                        res.leftCapacities[i] -= instance->demandOfCustomer[luckyDog];
                        res.leftCapacities[res.assignments[luckyDog]] += instance->demandOfCustomer[luckyDog];
                        // 如果当前可以，那么判断之前的是不是变成了空的，是的话就关掉
                        if (res.leftCapacities[res.assignments[luckyDog]] == instance->capacity[res.assignments[luckyDog]]) {
                            res.openingStates[res.assignments[luckyDog]] = false;
                        }
                        res.assignments[luckyDog] = i;
                        over = 0;
                        break;
                    }
                }
            }
        }
        return res;
    }

    // 打开一个仓库
    Solution generateMethod2() {
        // cout << "Method 2" << endl;
        Solution res(*this);
        // 挑一个仓库改变状态
        // 随机选择1-3个仓库改变
        int over = rand() % 3 + 1;
        // over = 1; cout << " ==> " << over << " " << this->isValid() << endl;
        int times = instance->facilityNum*10;
        // while (times-- && over) {
            over--;
            int r = rand()%instance->facilityNum;
            if (res.openingStates[r]) {
                // 试着关一下，把它的客户都挪走
                bool success = true;
                for (int i = 0; i < instance->customerNum; i++) {
                    if (res.assignments[i] == r) {
                        bool removeOk = false;
                        // 挪走,试一下
                        int to = rand()%instance->facilityNum;
                        // cout << "to = " << to << endl;
                        if (res.openingStates[to] && res.leftCapacities[to] > instance->demandOfCustomer[i] && to != r) {
                            // 可以放到to仓库里
                            res.leftCapacities[r] += instance->demandOfCustomer[i];
                            res.leftCapacities[to] -= instance->demandOfCustomer[i];
                            res.assignments[i] = to;
                            removeOk = true;
                            // cout << "ok change i = " << i << " to = " << to << " " << res.leftCapacities[to] << endl;
                        } else {
                            for (int j=(to+1)%instance->facilityNum; j!=to; j=(j+1)%instance->facilityNum) {
                                if (res.openingStates[j] && res.leftCapacities[j] > instance->demandOfCustomer[i] && j != r) {
                                    // 可以放到j仓库里
                                    res.leftCapacities[r] += instance->demandOfCustomer[i];
                                    res.leftCapacities[j] -= instance->demandOfCustomer[i];
                                    res.assignments[i] = j;
                                    removeOk = true;
                                    break;
                                }
                            }
                            if (removeOk == false && rand()%100 > 50) {
                                for (int j=(to+1)%instance->facilityNum; j!=to; j=(j+1)%instance->facilityNum) {
                                    if (!res.openingStates[j] && res.leftCapacities[j] > instance->demandOfCustomer[i]) {
                                        // 可以放到j仓库里
                                        res.leftCapacities[r] += instance->demandOfCustomer[i];
                                        res.leftCapacities[j] -= instance->demandOfCustomer[i];
                                        res.assignments[i] = j;
                                        res.openingStates[j] = true;
                                        removeOk = true;
                                        break;
                                    }
                                }
                            }
                        }
                        success = success && removeOk;
                        if (!success) break;
                    }
                }
                if (success) {
                    res.openingStates[r] = false;
                    over--;
                }
            } else {
                // 仓库是关闭的，那么打开仓库
                // 随机的给他分配一个人
                int rp = rand()%instance->customerNum;
                bool changeOK = false;
                if (instance->demandOfCustomer[rp] < res.leftCapacities[r]) {
                    res.leftCapacities[r] -= instance->demandOfCustomer[rp];
                    res.leftCapacities[assignments[rp]] += instance->demandOfCustomer[rp];
                    if (res.leftCapacities[assignments[rp]] == instance->capacity[assignments[rp]]) {
                        res.openingStates[assignments[rp]] = false;
                    }
                    res.assignments[rp] = r;
                    changeOK = true;
                } else {
                    for (int i = (rp+1)%instance->facilityNum; i != rp; i=(i+1)%instance->customerNum) {
                        if (instance->demandOfCustomer[i] < res.leftCapacities[r]) {
                            res.leftCapacities[r] -= instance->demandOfCustomer[i];
                            res.leftCapacities[assignments[i]] += instance->demandOfCustomer[i];
                            if (res.leftCapacities[assignments[i]] == instance->capacity[assignments[i]]) {
                                res.openingStates[assignments[i]] = false;
                            }
                            res.assignments[i] = r;
                            changeOK = true;
                            break;
                        }
                    }
                }
                if (changeOK) {
                    res.openingStates[r] = true;
                }
            }
        // }
        
        return res;
    }

    // 随机选两个人交换仓库
    Solution generateMethod3() {
        Solution res(*this);
        int times = instance->customerNum;
        while (times--) {
            int r1 = rand()%instance->customerNum, r2 = rand()%instance->customerNum;
            if (instance->demandOfCustomer[r1] < res.leftCapacities[assignments[r2]] + instance->demandOfCustomer[r2] &&
                instance->demandOfCustomer[r2] < res.leftCapacities[assignments[r1]] + instance->demandOfCustomer[r1]) {
                res.leftCapacities[assignments[r1]] = res.leftCapacities[assignments[r1]] + 
                                        instance->demandOfCustomer[r1] - instance->demandOfCustomer[r2];
                res.leftCapacities[assignments[r2]] = res.leftCapacities[assignments[r2]] + 
                                        instance->demandOfCustomer[r2] - instance->demandOfCustomer[r1];
                int tmp = res.assignments[r1];
                res.assignments[r1] = res.assignments[r2];
                res.assignments[r2] = tmp;
                break;
            }
        }       
        return res;
    }

    // 随机选3个人轮换仓库
    Solution generateMethod4() {
        Solution res(*this);
        int times = instance->customerNum;
        while (times--) {
            int r1 = rand()%instance->customerNum, 
                r2 = rand()%instance->customerNum, 
                r3 = rand()%instance->customerNum;
            if (r1 != r2 && r2 != r3 && r1 != r3 && res.assignments[r1] != res.assignments[r2] &&
                res.assignments[r1] != res.assignments[r3] &&
                res.assignments[r3] != res.assignments[r2] &&
                instance->demandOfCustomer[r1] < res.leftCapacities[assignments[r2]] + instance->demandOfCustomer[r2] &&
                instance->demandOfCustomer[r2] < res.leftCapacities[assignments[r3]] + instance->demandOfCustomer[r3] &&
                instance->demandOfCustomer[r3] < res.leftCapacities[assignments[r1]] + instance->demandOfCustomer[r1]) {
                res.leftCapacities[assignments[r1]] = res.leftCapacities[assignments[r1]] + 
                                        instance->demandOfCustomer[r1] - instance->demandOfCustomer[r3];
                res.leftCapacities[assignments[r2]] = res.leftCapacities[assignments[r2]] + 
                                        instance->demandOfCustomer[r2] - instance->demandOfCustomer[r1];
                res.leftCapacities[assignments[r3]] = res.leftCapacities[assignments[r3]] + 
                                        instance->demandOfCustomer[r3] - instance->demandOfCustomer[r2];
                int tmp = res.assignments[r1];
                res.assignments[r1] = res.assignments[r2];
                res.assignments[r2] = res.assignments[r3];
                res.assignments[r3] = tmp;
                break;
            }
        }

        return res;
    }

    Solution generateMethod5() {
        Solution res(*this);
        int times = instance->customerNum;
        while (times--) {
            int r1 = rand()%instance->customerNum, 
                r2 = rand()%instance->customerNum, 
                r3 = rand()%instance->customerNum,
                r4 = rand()%instance->customerNum;
            if (r1 != r2 && r2 != r3 && r3 != r4 && r4 != r1 && r1 != r3 && r2 != r4 && 
                assignments[r1] != assignments[r2] && 
                assignments[r2] != assignments[r3] && 
                assignments[r3] != assignments[r4] && 
                assignments[r4] != assignments[r1] && 
                assignments[r1] != assignments[r3] && 
                assignments[r2] != assignments[r4] &&
                instance->demandOfCustomer[r1] < res.leftCapacities[assignments[r2]] + instance->demandOfCustomer[r2] &&
                instance->demandOfCustomer[r2] < res.leftCapacities[assignments[r3]] + instance->demandOfCustomer[r3] &&
                instance->demandOfCustomer[r3] < res.leftCapacities[assignments[r4]] + instance->demandOfCustomer[r4] &&
                instance->demandOfCustomer[r4] < res.leftCapacities[assignments[r1]] + instance->demandOfCustomer[r1]) {
                res.leftCapacities[assignments[r1]] = res.leftCapacities[assignments[r1]] + 
                                        instance->demandOfCustomer[r1] - instance->demandOfCustomer[r4];
                res.leftCapacities[assignments[r2]] = res.leftCapacities[assignments[r2]] + 
                                        instance->demandOfCustomer[r2] - instance->demandOfCustomer[r1];
                res.leftCapacities[assignments[r3]] = res.leftCapacities[assignments[r3]] + 
                                        instance->demandOfCustomer[r3] - instance->demandOfCustomer[r2];
                res.leftCapacities[assignments[r4]] = res.leftCapacities[assignments[r4]] + 
                                        instance->demandOfCustomer[r4] - instance->demandOfCustomer[r3];
                int tmp = res.assignments[r1];
                res.assignments[r1] = res.assignments[r2];
                res.assignments[r2] = res.assignments[r3];
                res.assignments[r3] = res.assignments[r4];
                res.assignments[r4] = tmp;
                break;
            }
        }
        return res;
    }

    bool operator==(const Solution& s) const {
        return this->openingStates == s.openingStates &&
                this->assignments == s.assignments &&
                this->instance == s.instance;
    }
};

Solution Solution::generateNewSolution() {
    Solution res(*this);
    float probability = (float) (rand()%100)/100;
    if (probability < 0.30) {
        return generateMethod1();
    } else if (probability < 0.5) {
        return generateMethod4();
    } else if (probability < 0.7) {
        return generateMethod3();
    } else if (probability < 0.85) {
        return generateMethod5();
    } else {
        return generateMethod2();
    }
}

// declare hash<Solution>
namespace std {
    template<> struct hash<Solution> {
        std::size_t operator()(const Solution& s) const {
            std::size_t h = 54189789566;
            for (int i = 0; i < s.openingStates.size(); i++) {
                h = h ^ std::hash<std::string>{}(std::to_string(s.openingStates[i]))<<((i*3+10)%(sizeof(long long)*8));
            }
            for (int i = 0; i < s.assignments.size(); i++) {
                h = h ^ std::hash<std::string>{}(std::to_string(s.assignments[i]))<<((i*5+3)%(sizeof(long long)*8));
            }
            return h;
        }
    };
}

#endif