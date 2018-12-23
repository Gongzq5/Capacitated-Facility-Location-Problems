# encoding=utf-8
import sys

# 0.[File : Instances/p1
# 1. Use 8 s
# 2.
# 3. 8848
# 4.
# 5. 1 1 1 1 1 0 1 0 1 0 
# 6. 8 2 1 6 3 8 2 4 4 1 4 0 3 2 8 3 2 0 6 4 3 4 6 4 2 6 1 6 0 1 2 6 0 3 4 4 4 3 0 4 3 8 1 6 4 0 2 0 4 0]


if __name__ == '__main__':
    summary = open('summary', 'w')
    result = open('result.md', 'w')
    nameBase = 'Solutions/p'
    result.write('|     | Result | Times(s)|\n')
    result.write('| --- | -----  | -----   |\n')
    for index in range(1, 72):
        reFile = open('final/p' + str(index), 'w')
        minCost = sys.maxsize
        minFile = ['']
        minUsedTime = sys.maxsize
        for i in range(1, 4):
            with open(nameBase + str(index) + '(' + str(i) + ')', 'r') as f:
                print(nameBase + str(index) + '(' + str(i) + ')')
                allFile = f.readlines()
                usedTimes = allFile[1].split()[1]
                cost = int(allFile[3])
                if cost < minCost:
                    minCost = cost
                    minFile = allFile
                    minUsedTime = usedTimes
        reFile.writelines(minFile[3])
        reFile.writelines(minFile[5:7])
        summary.writelines(minFile)
        summary.write('\n')
        result.write('|p' + str(index) + '|' + str(minCost) + '|' + str(minUsedTime) + '|\n')
        reFile.close()
    summary.close()
