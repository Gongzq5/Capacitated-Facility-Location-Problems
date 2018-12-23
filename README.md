# Capacitated-Facility-Location-Problems
Solutions of capacitated facility location problems 

[TOC]

## 问题背景

### Capacitated Facility Location Problem
* Suppose there are n facilities and m customers. We wish to choose:
  * which of the n facilities to open
  * the assignment of customers to facilities 
* The objective is to minimize the sum of the opening cost and the assignment cost.
* The total demand assigned to a facility must not exceed its capacity. 

###  Problem Instance 

![1545555046846](https://github.com/Gongzq5/Capacitated-Facility-Location-Problems/blob/master/assets/1545555046846.png?raw=true)

## 问题参考

这个网站：[http://www.di.unipi.it/optimize/Data/MEX.html](http://www.di.unipi.it/optimize/Data/MEX.html)

包含了问题的已知最优解

## 问题求解

### 解法1：模拟退火 SA

#### 代码：

该解法代码文件位于[`src/SA.cpp`](https://github.com/Gongzq5/Capacitated-Facility-Location-Problems/blob/master/src/SA.cpp)中。

#### 简介

就是普通的模拟退火，进行了如下的一些改进，并且参数经过了多次试验性的调整；

* **邻域搜索策略：**

采用**5种**邻域搜索策略

1. 随机将一个客户移动到另一个仓库
2. 随机改变一个仓库的状态
   * 若该仓库是打开的，那么尝试将该仓库的所有用户分配给其他仓库，然后关闭它；如果无法将所有用户都分配走，那么不恢复已被挪走的用户，也不改变该仓库的状态；
   * 若该仓库是关闭的，那么将该仓库打开，并且给这个仓库随机分配一个客户；
3. 随机选择2个客户交换仓库
4. 随机选择3个客户轮换仓库
5. 随机选择4个客户轮换仓库

以上的策略在执行后都会验证，如果被挪走客户的仓库此时没有任何一个其他客户使用，那么将其关闭。

* **额外的改进：**

1. 降温开始时记录一个阈值，在多次迭代不发生改变时，将温度升高至该阈值，然后阈值减半；

2. 根据问题规模确定搜索次数（内循环次数），可以有效地处理问题规模增长后效果变差的情况。

#### 基本参数设置

| 参数         | 设置                            | 备注                                       |
| ------------ | ------------------------------- | ------------------------------------------ |
| 初温         | $20$                            | 初始温度                                   |
| 降温速率     | $0.99$                          | 每轮循环后 $T_{k+1} = 0.99 T_{k}$          |
| 概率接受函数 | $random() < e^{\Delta H/(T*5)}$ | 随机数$r$小于该值时接受差解                |
| 内循环次数   | $problemSize  \cdot 1.0001^n$   | 初值根据问题规模确定，每次增加到$1.0001$倍 |

--------

### 解法2：贪心+局部搜索 Greedy + Local search

#### 代码

该解法代码文件位于[`src/GreedyLocalSearch.cpp`](https://github.com/Gongzq5/Capacitated-Facility-Location-Problems/blob/master/src/GreedyLocalSearch.cpp)中。

#### 简介

首先写了**贪心**的算法，贪心的算法在[`src/GreedyLocalSearch.cpp`](https://github.com/Gongzq5/Capacitated-Facility-Location-Problems/blob/master/src/GreedyLocalSearch.cpp)文件中有一个单独的函数可供查看；

然后写了**局部搜索**的算法；

单独贪心和局部搜索的结果都不是很好，想到可以将两者结合起来进行计算。**首先先使用贪心法求一个初始解，然后使用局部搜索在此初始解附近迭代，试图找到最优解；**

局部搜索使用的**邻域搜索策略**和模拟退火是相同的。

**额外的改进：**

1. 为了避免问题的规模增加后代码运行次数不足的情况，我增加了根据问题规模决定循环次数的代码；

    ```cpp
    int problemSize = instance.customerNum * instance.facilityNum;
    for (int _i; _i < problemSize * 2000; _i++) {...}
    ```

2. 为了避免陷入局部最优，我增加了接受差解的代码，以期跳出局部最优；

    * 首先将接受差解的概率设置为0；
    * 在限定的循环次数内，如果有一定次数没有发现更优的解，会线性的增加接受差解的概率；
    * 由于参数的设定机制，接受差解的概率最多不会超过0.2，在实践中，通常不会超过0.01。但是取得的效果是显著的，在陷入局部最优（可能是全局最优）后会以较大的概率跳出该解，整个解检索的过程大致呈锯齿状分布；

    ```pseudocode
    接受差解概率 badAcception = 0 
    FOR i FROM 0 TO (problemSize * 2000)
        {生成新解}
        IF {新解效果好} THEN
            {接收新解}
            badAcception = 0
        ELSE 
            {以概率badAcception接受差解}
            IF {不改变的次数大于 problemSize} THEN
                badAcception += 0.0001;
            END IF
        END IF
    END FOR
    ```

#### 基本参数设置

| 参数         | 设置                        | 备注                                                       |
| ------------ | --------------------------- | ---------------------------------------------------------- |
| 问题规模     | `customerNum * facilityNum` | 问题规模为客户数量乘以工厂的数量，用来衡量该问题的计算规模 |
| 差解接受概率 | 0                           | 初值为0，不接受差解；每n次不产生新解则将其递增0.0001       |

-------------

### 附贪心算法的策略：

每次从客户的角度出发，贪心离他最近的工厂；

其判断到每个工厂的代价，计算为

```pseudocode
IF facility[j] is open THEN
	cost = assignment cost
ELSE
	cost = assignment cost + opening cost
END IF
```

每次判断一个用户，如果该用户得到的最小代价是一个未打开的工厂的，那么就把这个工厂打开；如果是一个已打开的工厂，那么就只需要把用户指向这个工厂就好了。

## 实验结果

考虑到随机算法效果的随机性，为了充分考量算法的效果，我对每个测例进行了三次运行，结果放在结果文件夹的**allSolutions**文件夹中(命名为p1(1), p1(2)…)，并挑出了其中最好的解放在**optimizeSolutions**文件夹中。

结果表格请查看**result.md**文件；

我还额外提供了一个结果矩阵的汇总的，命名为 **summary**。

链接如下：

|            | 模拟退火                                                     | 贪心+局部搜索                                                |
| ---------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| 所有实验解 | [模拟退火三次实验的所有结果](https://github.com/Gongzq5/Capacitated-Facility-Location-Problems/tree/master/results/SA/allSolutions) | [局部搜索三次实验的所有结果](https://github.com/Gongzq5/Capacitated-Facility-Location-Problems/tree/master/results/Greedy%20and%20Local%20search/allSolutions) |
| 所有最优解 | [模拟退火每个测例的最优解](https://github.com/Gongzq5/Capacitated-Facility-Location-Problems/tree/master/results/SA/optimizeSolutions) | [局部搜索每个测例的最优解](https://github.com/Gongzq5/Capacitated-Facility-Location-Problems/tree/master/results/Greedy%20and%20Local%20search/optimizeSolutions) |
| 结果汇总   | [模拟退火每个测例具体结果的汇总](https://github.com/Gongzq5/Capacitated-Facility-Location-Problems/blob/master/results/SA/summary) | [局部搜索每个测例具体结果的汇总](https://github.com/Gongzq5/Capacitated-Facility-Location-Problems/blob/master/results/Greedy%20and%20Local%20search/summary) |
| 结果表格   | [模拟退火实验结果的表格](https://github.com/Gongzq5/Capacitated-Facility-Location-Problems/blob/master/results/SA/result.md) | [局部搜索实验结果的表格](https://github.com/Gongzq5/Capacitated-Facility-Location-Problems/blob/master/results/Greedy%20and%20Local%20search/result.md) |

以下附结果表格

### 模拟退火结果如下

|      | Result | Times(s) |
| ---- | ------ | -------- |
| p1   | 8848   | 8        |
| p2   | 7914   | 8        |
| p3   | 9314   | 8        |
| p4   | 10795  | 6        |
| p5   | 8838   | 8        |
| p6   | 7777   | 7        |
| p7   | 9488   | 7        |
| p8   | 11088  | 7        |
| p9   | 8593   | 5        |
| p10  | 7627   | 7        |
| p11  | 8938   | 7        |
| p12  | 10252  | 8        |
| p13  | 8294   | 18       |
| p14  | 7137   | 17       |
| p15  | 8862   | 17       |
| p16  | 10524  | 20       |
| p17  | 8294   | 17       |
| p18  | 7152   | 16       |
| p19  | 8907   | 16       |
| p20  | 10514  | 17       |
| p21  | 8171   | 17       |
| p22  | 7178   | 16       |
| p23  | 8774   | 14       |
| p24  | 10463  | 19       |
| p25  | 11639  | 106      |
| p26  | 10776  | 99       |
| p27  | 12336  | 101      |
| p28  | 13736  | 105      |
| p29  | 12548  | 102      |
| p30  | 11337  | 139      |
| p31  | 13378  | 189      |
| p32  | 15376  | 182      |
| p33  | 11632  | 101      |
| p34  | 10635  | 103      |
| p35  | 12235  | 100      |
| p36  | 13835  | 99       |
| p37  | 11326  | 99       |
| p38  | 10594  | 98       |
| p39  | 11951  | 107      |
| p40  | 13059  | 125      |
| p41  | 6737   | 11       |
| p42  | 5755   | 32       |
| p43  | 5302   | 32       |
| p44  | 7107   | 14       |
| p45  | 6580   | 25       |
| p46  | 5980   | 36       |
| p47  | 7077   | 13       |
| p48  | 6239   | 25       |
| p49  | 5609   | 38       |
| p50  | 8848   | 13       |
| p51  | 7414   | 36       |
| p52  | 9246   | 15       |
| p53  | 8531   | 38       |
| p54  | 8838   | 23       |
| p55  | 7654   | 34       |
| p56  | 21748  | 140      |
| p57  | 26379  | 155      |
| p58  | 38059  | 160      |
| p59  | 27694  | 143      |
| p60  | 21241  | 137      |
| p61  | 25596  | 139      |
| p62  | 33662  | 143      |
| p63  | 25397  | 142      |
| p64  | 21400  | 164      |
| p65  | 25794  | 137      |
| p66  | 32688  | 145      |
| p67  | 25231  | 142      |
| p68  | 22086  | 137      |
| p69  | 26100  | 143      |
| p70  | 33316  | 147      |
| p71  | 25679  | 144      |



### 局部搜索结果如下

|      | Result | Times(s) |
| ---- | ------ | -------- |
| p1   | 8848   | 6s       |
| p2   | 7914   | 5s       |
| p3   | 9314   | 6s       |
| p4   | 10859  | 7s       |
| p5   | 8838   | 6s       |
| p6   | 7777   | 5s       |
| p7   | 9488   | 6s       |
| p8   | 11088  | 6s       |
| p9   | 8462   | 7s       |
| p10  | 7617   | 5s       |
| p11  | 8932   | 6s       |
| p12  | 10138  | 7s       |
| p13  | 8252   | 13s      |
| p14  | 7182   | 12s      |
| p15  | 8862   | 14s      |
| p16  | 10612  | 17s      |
| p17  | 8227   | 13s      |
| p18  | 7188   | 12s      |
| p19  | 8887   | 12s      |
| p20  | 10513  | 13s      |
| p21  | 8068   | 13s      |
| p22  | 7154   | 13s      |
| p23  | 8806   | 13s      |
| p24  | 10327  | 14s      |
| p25  | 11639  | 77s      |
| p26  | 10781  | 78s      |
| p27  | 12340  | 77s      |
| p28  | 13736  | 79s      |
| p29  | 12450  | 153s     |
| p30  | 11391  | 155s     |
| p31  | 13367  | 217s     |
| p32  | 15387  | 245s     |
| p33  | 11706  | 76s      |
| p34  | 10635  | 73s      |
| p35  | 12235  | 75s      |
| p36  | 13835  | 75s      |
| p37  | 11258  | 116s     |
| p38  | 10551  | 48s      |
| p39  | 11824  | 69s      |
| p40  | 13024  | 72s      |
| p41  | 6640   | 7s       |
| p42  | 5750   | 14s      |
| p43  | 5369   | 20s      |
| p44  | 7462   | 7s       |
| p45  | 6396   | 13s      |
| p46  | 6083   | 20s      |
| p47  | 6836   | 9s       |
| p48  | 5910   | 14s      |
| p49  | 5584   | 19s      |
| p50  | 8861   | 8s       |
| p51  | 7471   | 17s      |
| p52  | 9253   | 7s       |
| p53  | 8589   | 18s      |
| p54  | 8834   | 19s      |
| p55  | 7663   | 21s      |
| p56  | 22384  | 67s      |
| p57  | 27321  | 68s      |
| p58  | 38651  | 69s      |
| p59  | 28015  | 69s      |
| p60  | 23122  | 67s      |
| p61  | 25995  | 66s      |
| p62  | 34816  | 67s      |
| p63  | 25279  | 68s      |
| p64  | 23637  | 66s      |
| p65  | 25958  | 134s     |
| p66  | 33124  | 68s      |
| p67  | 27487  | 68s      |
| p68  | 22317  | 68s      |
| p69  | 25258  | 68s      |
| p70  | 33102  | 68s      |
| p71  | 26433  | 71s      |