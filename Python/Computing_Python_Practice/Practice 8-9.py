menu = {'Americano':'2000원','Cafe latte':'2500원','Green Tea latte':'3000원','Mocha latte':"3500원"}

print("Menu: Americano")

if 'Americano' in menu:
    print("Price:", menu['Americano'])
else:
    print("This menu does not exit")

print("Menu: Vanila latte")

if 'Vanila latte' in menu:    
    print("Price:", menu['Vanila latte'])
else:
    print("This menu does not exit")
