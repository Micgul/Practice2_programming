def fare():
    print("춘천(운임 : 5000원) / 부산(운임 : 30000원) / 대구(운임 : 20000원) / 수원(운임 : 10000원)")
    dest = input("한곳을 입력하세요 : ")
    if dest == "춘천":
        print("춘천 까지의 금액은 5000원 입니다")
    elif dest == "부산":
        print("부산 까지의 금액은 30000원 입니다")
    elif dest == "대구":
        print("대구 까지의 금액은 20000원 입니다")
    else :
        print("수원 까지의 금액은 10000원 입니다")

fare()