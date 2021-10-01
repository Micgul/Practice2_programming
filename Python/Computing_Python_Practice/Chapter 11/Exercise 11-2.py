def average(numlist):
    sum = 0
    aver = 0
    
    for i in numlist:
        sum += i
        aver = sum / len(numlist)

    return aver

score = [90,80,70,60,50]
print(average(score))
