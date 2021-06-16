def print_list(num_list) :
    for n in num_list :
        if (n != 0) :
            print(n, end=',')
        else:
            break

print(print_list([1,2,3,0]))
        
