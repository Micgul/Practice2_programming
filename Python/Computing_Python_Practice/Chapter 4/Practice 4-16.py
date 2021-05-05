hour = int(input("시각을 입력하시오. :"))
minute = int(input("분을 입력하시오. :"))
second = int(input("초를 입력하시오. :"))

total_second = hour*3600 + minute*60 + second
print(total_second, "초")
