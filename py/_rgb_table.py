import numpy as np

MAX_WL = 780
MIN_WL = 380
stp = (MAX_WL-MIN_WL)/256


R = []
G = []
B = []

for j in range(0,256):
    i = MIN_WL + j*stp
    if ((i >= 380) and (i <= 410)):
        R.append(int(102 -  51 * (i - 380) / 30))
        G.append(0)
        B.append(int(103 + 152 * (i - 380) / 30))
    elif ((i >= 410) and (i <= 440)):   
        R.append(int(51  -  51 * (i - 410) / 30))
        G.append(0)
        B.append(255)
    elif ((i >= 440) and (i <= 490)):   
        R.append(0)
        G.append(int(255 - 255 * (490 - i) / 50))
        B.append(255)
    elif ((i >= 490) and (i <= 510)):   
        R.append(0)
        G.append(255)
        B.append(int(255 * ((510 - i) / 20)))
    elif ((i >= 510) and (i <= 580)): 
        R.append(int(255 - 255 * (580 - i) / 70))
        G.append(255)
        B.append(0)
    elif ((i >= 580) and (i <= 640)): 
        R.append(255)
        G.append(int(255 * ((640 - i) / 60)))
        B.append(0)
    elif ((i >= 640) and (i <= 700)): 
        R.append(255)
        G.append(0)
        B.append(0)
    elif ((i >= 700) and (i <= 780)): 
        R.append(int(102 - 153 * (i - 780) / 80))
        G.append(0)
        #B.append(0)
        B.append(int(103 + 103 * (i - 780) / 80))
        

    print("{:5d},".format(B[j]),end = '')
    if (j+1) % 16 == 0:
        print("");
