# 1) Edit title/xlabel ...
# 2) create 'line(npArray_x, npArray_y)'
# 3) use
#       dump()    - printing MNK function
#       draw()    - drawing line and points
#       func(num) - calculate point in that line
#       revert()  - return revert line (x <-> y)

import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import xlrd
import pandas as pd

import scipy.constants as spc

mpl.rcParams['font.size'] = 16 # Управление стилем, в данном случаем - размером шрифта 
plt.figure(figsize=(16,9))
plt.grid(b=True, which='major', axis='both')
plt.grid(b=True, which='minor', axis='both')

def MNK_mid(npArr):
    return npArr.sum() / len(npArr)

class points:
    def __init__(self, npAr1, npAr2, name = "points", options="r^"):
        self.x = npAr1
        self.y = npAr2
        self.name = name
        plt.plot(self.x, self.y, options, label = self.name)

class line:
    def __init__(self, npAr1, npAr2, name = "line", options_pt="-b", options_ln="b^"):
        self.k = (MNK_mid(npAr1 * npAr2) - MNK_mid(npAr1) * MNK_mid(npAr2)) / (MNK_mid(npAr1 * npAr1) - MNK_mid(npAr1)**2)
        self.b = MNK_mid(npAr2) - self.k * MNK_mid(npAr1)
        self.k_sigma = ((MNK_mid(npAr2 * npAr2) - MNK_mid(npAr2)**2) / (MNK_mid(npAr1 * npAr1) - MNK_mid(npAr1)**2) - self.k**2)**0.5  / len(npAr1)**0.5
        self.b_sigma = self.k_sigma * ((MNK_mid(npAr1**2))**0.5)
        self.x = npAr1
        self.y = npAr2
        self.name = name
        plt.plot(np.array([self.x[0], self.x[self.x.size - 1]]), np.array([self.func(self.x[0]), self.func(self.x[self.x.size - 1])]), options_pt, label = self.name + " points")
        plt.plot(self.x, self.y, options_ln, label = self.name + " line")
        #plt.errorbar(x, y, xerr = x_error, yerr = y_error, fmt = '.', color="k")

    def dump(self, name = "line"):
        print(name, ": y = (", self.k, "+-", self.k_sigma, ")x + (", self.b, "+-", self.b_sigma, ")")
    
    def func(self, num):
        return self.k * num + self.b
    
    def reverse(self):
        return line(self.y, self.x)

        
def DRAW(TITLE = "TITLE", XLABEL = "XLABEL", YLABEL = "YLABEL"):
    plt.title(TITLE)
    plt.ylabel(YLABEL)
    plt.xlabel(XLABEL)
    plt.legend()
    plt.minorticks_on()
    plt.show()

#----------------------------------------------------------------
#----------------------------------------------------------------
