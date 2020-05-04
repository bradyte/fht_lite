for i in range(0, 15):
    for j in range(0, 15-i):
        tmp = bin(j)[-1:1:-1]
        b1 = bin(15-i) + (tmp + (4 -len(tmp))*'0')
        tmp = b1[-1:1:-1]
        b2 = tmp + (8 -len(tmp))*'0'
        print('{:3d},{:3d},  //{:08b}  {:08b}'.format(
            int(b1,2), int(b2,2), int(b1,2), int(b2,2)))




