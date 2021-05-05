current = int(input("현재 전기 용량: "))
total = int(input("전체 전기 용량: "))

if current < total*0.3 :
    print("전체 전기 총량의 30%보다 현재 전기량이 적으니, 휘발유로 주행해야 한다.")
