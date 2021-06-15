user_input = int(input("Enter the even number (1~30) : "))

for i in range(user_input,30):
    i += 1
    if i % 2 == 0:
        print(i)
