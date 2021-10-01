score_list = [90,80,70,60,50]

sum = 0
maximum = 0
average = 0

for i in score_list :
    if maximum < i :
        maximum = i
    sum += i
    average = sum / len(score_list)

if average >= 60 :
    print('average :', average, 'maximum :', maximum, "You are Pass")
else :
    print('average :', average, 'maximum :', maximum, "You are Fail")
