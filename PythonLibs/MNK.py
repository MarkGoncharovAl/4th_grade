# 1) Edit title/xlabel ...
# 2) create 'line(npArray_x, npArray_y)'
# 3) use
#       dump()    - printing MNK function
#       draw()    - drawing line and points
#       func(num) - calculate point in that line
#       revert()  - return revert line (x <-> y)


TITLE = "Название"
XLABEL = "Ось x"
YLABEL = "Ось y"
LABEL = "График"

import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt

mpl.rcParams['font.size'] = 16 # Управление стилем, в данном случаем - размером шрифта 
plt.figure(figsize=(8,7))
plt.grid(b=True, which='major', axis='both')
plt.grid(b=True, which='minor', axis='both')
plt.legend()

def MNK_mid(npArr):
    return npArr.sum() / len(npArr)

class line:
    def __init__(self, npAr1, npAr2):
        self.k = (MNK_mid(npAr1 * npAr2) - MNK_mid(npAr1) * MNK_mid(npAr2)) / (MNK_mid(npAr1 * npAr1) - MNK_mid(npAr1)**2)
        self.b = MNK_mid(npAr2) - self.k * MNK_mid(npAr1)
        self.k_sigma = ((MNK_mid(npAr2 * npAr2) - MNK_mid(npAr2)**2) / (MNK_mid(npAr1 * npAr1) - MNK_mid(npAr1)**2) - self.k**2)**0.5  / len(npAr1)**0.5
        self.b_sigma = self.k_sigma * ((MNK_mid(npAr1**2))**0.5)
        self.x = npAr1
        self.y = npAr2

    def dump(self):
        print("y = (", self.k, "+-", self.k_sigma, ")x + (", self.b, "+-", self.b_sigma, ")")
    
    def func(self, num):
        return self.k * num + self.b
    
    def draw(self):
        plt.title(TITLE)
        plt.ylabel(YLABEL)
        plt.xlabel(XLABEL)
        plt.plot(np.array([self.x[0], self.x[self.x.size - 1]]), np.array([self.func(self.x[0]), self.func(self.x[self.x.size - 1])]), "r", label=LABEL)
        plt.plot(self.x, self.y, "b^")
        plt.legend()
        plt.show()
    
    def reverse(self):
        return line(self.y, self.x)
    
class graphics:
    def __init__(self, lines):
        self.lines = np.array([lines])
#----------------------------------------------------------------
#----------------------------------------------------------------

x1 = np.array([2, 4, 6])
x2 = np.array([1, 2, 3])
ln = line(x1, x2)
ln.dump()
ln.draw()
