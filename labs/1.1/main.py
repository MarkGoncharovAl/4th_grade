import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl

def mid(npArr):
    return npArr.sum() / len(npArr)

def k_only(x, y):
    return mid(x * y) / mid(x * x)

def k_only_sigma(x, y):
    return ((mid(x * x) * mid(y * y) - mid(x * y)**2) / (len(x) * mid(x * x)**2))**0.5

def k_coef(x, y):
    return (mid(x * y) - mid(x) * mid(y)) / (mid(x * x) - mid(x)**2)

def b_coef(x, y):
    return mid(y) - k_coef(x, y) * mid(x)

def k_sigma(x, y):
    return ((mid(y * y) - mid(y)**2) / (mid(x * x) - mid(x)**2) - k_coef(x, y)**2)**0.5  / len(x)**0.5

# calibr
x = np.array([1939.0, 1980.0, 2247.0, 2259.0, 2289.0, 2350.0, 2389.0])
y = np.array([534.60, 540.10, 585.00, 588.20, 594.50, 607.40, 616.40]) 

#graph 1
x = np.array([-0.668, -0.643, -0.598, -0.542])
y = np.array([0.06, 0.125, 0.274, 0.42])
v01 = -1 * b_coef(x, y) / k_coef(x, y)
print (v01)

#graph 2
x = np.array([-0.586, -0.561, -0.526, -0.49])
y = np.array([0.023, 0.078, 0.192, 0.302])
v02 = -1 * b_coef(x, y) / k_coef(x, y)
print (v02)

#graph 3
x = np.array([-0.475, -0.441, -0.405])
y = np.array([0.083, 0.211, 0.365])
v03 = -1 * b_coef(x, y) / k_coef(x, y)
print (v03)

#graph 4
x = np.array([-0.381, -0.365, -0.350])
y = np.array([0.098, 0.172, 0.251])
v04 = -1 * b_coef(x, y) / k_coef(x, y)
print (v04)

# V0(frec)
x = 6.28 * 3 * 10**8 * 10**9 / np.array([552, 588, 615, 642])
y = np.array([v01, v02, v03, v04])
print (k_coef(x, y) * 1.602 * 10**(-19))

