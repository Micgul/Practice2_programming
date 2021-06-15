fruit = {"pear":3, "apple":5, "stbry":2, "ormel":5}
price = {"pear":2000, "apple":1500, "stbry":1800, "ormel":2300}

total_price = 0 # init
cost = 0 # init

for key in fruit.keys():
    if fruit[key] < 5 :
        cost = price[key] * (5 - fruit[key])
        print(key, cost, "원")
        total_price += cost
        
print("총 비용은", total_price, "원 입니다.")




