import numpy as np

for i in range(0,256):
    w_hann = int(np.round(2**14 - 2**14*np.cos(2*np.pi*i/256)))
    if w_hann >= 32768:
        w_hann = 32767
    print("{:5d},".format(w_hann),end = '')
    if (i+1) % 16 == 0:
        print("");

    
