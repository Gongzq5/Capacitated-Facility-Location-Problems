# encoding=utf-8
import sys

#  0 ['File : Instances/p1\n',
#  1 'Use 22s\n', .
#  2 '\n',
#  3 '9011\n',
#  4 '\n',
#  5 '1 1 1 1 1 1 0 0 1 1 \n',
#  6 '\n',
#  7 '2 2 1 5 3 8 2 4 4 1 \n',
#  8 '9 8 3 2 8 3 4 0 9 4 \n',
#  9 '3 8 9 4 2 5 1 5 0 5 \n',
# 10 '2 5 0 3 9 4 4 3 0 4 \n',
# 11 '1 8 1 5 4 0 4 0 4 0 \n',
# 12 '\n']

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
        for i in range(1, 4):
            with open(nameBase + str(index) + '(' + str(i) + ')', 'r') as f:
                allFile = f.readlines()
                usedTimes = allFile[1].split()[1]
                if 'trim' in allFile[3]:
                    cost = int(allFile[4])
                else:
                    cost = int(allFile[3])
                if cost < minCost:
                    minCost = cost
                    minFile = allFile
        reFile.writelines(minFile)
        summary.writelines(minFile)
        result.write('| p' + str(index) + ' | ' + str(cost) + ' | ' + str(usedTimes) + '|\n')
        reFile.close()
    summary.close()
