import numpy as np
N = 256

for i in range(0,N):
    w_hann = int(np.round(2**14 - 2**14*np.cos(2*np.pi*i/N)))
    print("{},".format(w_hann))
