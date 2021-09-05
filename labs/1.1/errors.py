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

x = np.array([1939.0, 1980.0, 2247.0, 2259.0, 2289.0, 2350.0, 2389.0])
y = np.array([534.60, 540.10, 585.00, 588.20, 594.50, 607.40, 616.40]) 