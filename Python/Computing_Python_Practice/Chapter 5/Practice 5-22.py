# bank = 'pi'
# way = account or card
# money = cash or check

bank = 'pi'
way = 'card'
money = 'check'

if bank == 'pi' and ((way == 'account') or (way == 'card')) and money == 'cash':
    print("입금할 수 있다.")
else :
    print("입금할 수 없다.")
