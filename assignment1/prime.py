import time

def get_GCD(a, b):
    while(a!=0) :
        n = b%a
        b = a
        a = n
    return b

def count_Prime(min, max):
    cnt = 0
    PrimeTable = list();
    for i in range(0, max+1):
        PrimeTable.append(1)
        
    for i in range(2, max+1):
        if(PrimeTable[i] == 0):
            continue
        for j in range(2*i, max+1, i):
            PrimeTable[j] = 0
    
    for i in range(min, max+1):
        if(PrimeTable[i] != 0):
            cnt = cnt + 1
    
    print("Number of prime numbers between {}, {}: {}".format(min, max, cnt))
    return;


while True:
    print(">> Input the number of numbers to process:", end=' ');
    inputNum = int(input());
    
    if(inputNum < 2 or inputNum > 30) :
        print("Input count range should be \"1 < [INPUT] < 31\" ")
        continue;
    
    print(">> Input the numbers to be processed:")
    numList = list(map(int, input().split()))
    if(len(numList) != inputNum):
        print("Input Count shoud be {}".format(inputNum))
        continue
    
    uniqueList = set(numList)
    uniqueList = list(uniqueList)
    uniqueList.sort()

    InRange = True
    for i in range(len(uniqueList)):
        if(uniqueList[i] < 1 or uniqueList[i] > 100000):
            InRange = False
            break
    
    if(InRange == False):
        print("Range : 1 ~ 100,000")
        InRange = True
        continue
    
    startTime = time.time()

    GCD = uniqueList[0]
    for i in range(len(uniqueList) - 1):
        GCD = get_GCD(GCD, uniqueList[i])

    print("GCD of input number is {}".format(GCD))
    
    for i in range(len(uniqueList) -1):
        count_Prime(uniqueList[i], uniqueList[i+1])
        
    endTime = time.time()
    execTime = endTime - startTime
    
    print("\nTotal execution time using Python is {} seconds!".format(execTime))
    break

    