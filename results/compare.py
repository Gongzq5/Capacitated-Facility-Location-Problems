# encoding=utf-8
import sys

if __name__ == '__main__':
    resultCompare = open('resultCompare.md', 'w')
    fileNames = ['SA/result.md', 'LocalSearch/result.md', 'Greedy and Local search/result.md', 'Greedy/result.md']
    resultCompare.write('|     | SA | Times(s)| LocalSearch | Times(s)| Greedy and Local search | Times(s)| Greedy | SA Times(s)|\n')
    resultCompare.write('| --- | --- | --- | --- | --- | --- | --- | --- | --- |\n')
    
    files = [open(fileNames[i]) for i in range(4)]
    fileLines = [files[i].readlines() for i in range(4)]

    for i in range(1, len(fileLines[0])):
        resultCompare.write(fileLines[0][i])
        resultCompare.write(fileLines[1][i])
        resultCompare.write(fileLines[2][i])
        resultCompare.write(fileLines[3][i])
        resultCompare.write('\n\n\n')

    for file in files:
        file.close()
