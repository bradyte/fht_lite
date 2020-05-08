import numpy as np
import matplotlib.pyplot as plt

MAX_WL = 780
MIN_WL = 380

wl = np.arange(start = MIN_WL, stop = MAX_WL)



R = []
G = []
B = []

ax = plt.subplot(111)

for i in wl:
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
        R.append(int(87 - 168 * (i - 780) / 80))
        G.append(0)
        B.append(0)
    ax.plot([i, i], [255+8, 320], color=
        [R[i - MIN_WL] / 255, G[i - MIN_WL] / 255, B[i - MIN_WL] / 255])


ax.plot(wl,R,'r',lw=1)
ax.plot(wl,B,'b',lw=1)
ax.plot(wl,G,'g',lw=1)
ax.axis([350, 800, 0, 320])
ax.set_xlabel('Wavelength (nm)')
ax.set_ylabel('RGB value (0-255)')
ax.set_title('Visible Light Spectrum Mapped to RGB Values')
ax.grid(True)
ax.set_yticks(np.arange(start = 8-1, stop = 256+1, step = 8))
ax.tick_params(axis="y", labelsize=5)
ax.set_xticks(np.arange(start = MIN_WL, stop = MAX_WL+1, step = 10))
ax.tick_params(axis="x", labelsize=6)
plt.xticks(rotation=90)
plt.show()

