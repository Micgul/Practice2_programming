apple_num = int(input("사과 개수 :"))
grape_num = int(input("포도 개수 :"))
pear_num = int(input("배 개수 :"))
tang_num = int(input("귤 개수 :"))

total_price = apple_num*1000 + grape_num*3000 + pear_num*2000 + tang_num*500
if grape_num >= 3 :
    total_price *= 0.9

print("총 금액은", total_price, "원 입니다.")
