import numpy as np

for i in range(0,256):
    if i == 0:
        db = int(0)
    else:
        db = int(np.round(8*np.log2(i)))
    print("{:5d},".format(db),end = '')
    if (i+1) % 16 == 0:
        print("");
    
