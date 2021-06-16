score_list = [45, 24, 56, 100, 84, 98]

def high_score(list):
    n = len(list)
    highest = 0
    for i in range(n):
        if highest < score_list[i] :
            highest = score_list[i]
    print("최고 점수는", highest,"점 입니다.")


high_score(score_list)
        
    
