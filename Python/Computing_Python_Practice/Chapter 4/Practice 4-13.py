#세점(a,b),(c,d),(e,f)의 좌표를 알때 직각삼각형의 넓이
#1/2 |(ad+cf+eb) - (bc+de+af)|
#1/2 |(ad) - (bc)|

x1 = float(input("좌표 x1 입력 : "))
y1 = float(input("좌표 y1 입력 : "))

x2 = float(input("좌표 x2 입력 : "))
y2 = float(input("좌표 y2 입력 : "))

area = abs((x1 * y2) - (y1 * x2)) / 2
print("직각삼각형의 넓이 :", area)
