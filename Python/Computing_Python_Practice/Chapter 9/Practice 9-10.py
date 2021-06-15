result = 0
print("덧셈을 하고 싶은 양의 정수들을 입력하세요. 0 입력시 종료.")
while True:
    user_input = int(input())
    result += user_input
    if user_input == 0:
        print("총 합은", result, "입니다.")
        break

                 
