import random

def randomNum(): #중복없는 랜덤숫자 만들기 (배열로 추출 )
    result = []
    for i in range(3): #자릿수
        a = random.randint(0,9)
        while a in result :
            a = random.randint(0,9) #중복방
        result.append(a)
    return result
#단 두줄로 끝....
#pt = random.sample(range(1, 10),3)
#print(pt)

#게임 시작
print("Welcome To Baseball Game!")
print("Push the three number")

ranNum = randomNum() # 랜덤숫자 만듬
print(ranNum) # 테스트용
s_ct = 0 #스트라이크 카운트
b_ct = 0 #볼 카운트

#반복입력하면서 중복방지와 카운 (배열로)
while(s_ct < 3):
    user_num = input("input : ")  #문자열로 추출
    #중복방지
    if(user_num[0] == user_num[1] == user_num[2]):
        print("Input the non-duplicated number")
        continue
    elif(user_num[0] == user_num[1]):
        print("Input the non-duplicated number")
        continue
    elif(user_num[0] == user_num[2]):
        print("Input the non-duplicated number")
        continue
    elif(user_num[1] == user_num[2]):
        print("Input the non-duplicated number")
        continue
    else:
        pass        #다른 예외 처리 필요

    for i in range(0, 3):
        for j in range(0, 3):
            if(i == j and str(ranNum[i]) == user_num[j]):
                s_ct += 1
            elif(i != j and str(ranNum[i]) == user_num[j]):
                b_ct += 1

    print(s_ct, "strike", b_ct,  "ball")
    if(s_ct == 3):
        print("Congratulation. Number is correct!")
        break
    s_ct = 0 #스트라이크 초기화
    b_ct = 0 #볼 초기화
