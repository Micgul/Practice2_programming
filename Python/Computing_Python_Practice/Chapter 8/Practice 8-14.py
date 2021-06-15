sales = {"soap":3, "toothbrush":5, "shampoo":2, "toothpaste":4, "lotion":5}
assessment = {"soap":2, "toothbrush":4, "shampoo":1, "toothpaste":4, "lotion":3}
stop_list = []

for key in sales.keys():
    if sales[key] >= 4 and assessment[key] >= 4:
        print(key,"는 우수 제품입니다.")

    
    
    if sales[key] < 4 and assessment[key] < 4:
        stop_list.append(key)
        # print(key,"는 판매 중지 목록에 들어갑니다.")
print()
#print(stop_list)
print("lotion은 판매 중지에 해당하는가? ")
print('lotion' in stop_list)
