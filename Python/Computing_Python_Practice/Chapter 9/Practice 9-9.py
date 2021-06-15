user_input = int(input("1이상의 정수 입력: "))
print(user_input,"의 약수")
for i in range(user_input):
    i += 1
    if user_input % i == 0:
        print(i)
