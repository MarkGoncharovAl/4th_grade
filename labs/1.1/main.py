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

def b_sigma(x, y):
    return k_sigma(x, y) * ((mid(x**2))**0.5)

# calibr
x = np.array([1939.0, 1980.0, 2247.0, 2259.0, 2289.0, 2350.0, 2389.0])
y = np.array([534.60, 540.10, 585.00, 588.20, 594.50, 607.40, 616.40]) 

x_approx = np.linspace(1900, 2400, 1000)
y_approx = k_coef(x, y) * x_approx + b_coef(x, y)
x_error = np.array([2] * len(x))
mpl.rcParams['font.size'] = 16
plt.figure(figsize=(16,9))
plt.xlabel("Барабан, гр")
plt.ylabel("Длина волны, нм")
#plt.errorbar(x, y, xerr = x_error, fmt = '.')
plt.plot(x, y, 'o', markersize=8)
plt.plot(x_approx, y_approx)
plt.minorticks_on()
plt.grid(which='major', axis='both')
plt.grid(which='minor', axis='both', linestyle = ':')
plt.legend()
plt.savefig("calibr")


# main calculations
V_552 = np.array([0.720, 0.223, 0.219, 0.206, 0.163, 0.143, 0.134, -0.089,-0.112, -0.150, -0.175, 
                 -0.2, -0.252, -0.273, -0.339, -0.404, -0.470, -0.542, -0.598, -0.643, -0.668, -0.710])

Vi_552 = np.array([0.637, 0.636, 0.635, 0.634, 0.629, 0.628, 0.627, 0.599, 0.595, 0.590, 0.585, 
                   0.580, 0.575, 0.564, 0.543, 0.521, 0.487, 0.420, 0.274, 0.125, 0.060, -0.013])      

V_588 = np.array([0.728, 0.505, 0.361, 0.153, -0.110, -0.176, -0.244, -0.307, 
                 -0.370, -0.450, -0.490, -0.526, -0.561, -0.586, -0.637, -0.710])
Vi_588 = np.array([0.637, 0.637, 0.637, 0.635, 0.594, 0.578, 0.558, 0.535,
                   0.507, 0.422, 0.302, 0.192, 0.078, 0.023, -0.006, -0.009])

V_615 = np.array([0.728, 0.368, 0.288, 0.143, -0.110, -0.250, -0.335, 
                 -0.385, -0.441, -0.475, -0.493, -0.532, -0.617, -0.710])
Vi_615 = np.array([0.637, 0.636, 0.636, 0.635, 0.585, 0.534, 0.444, 
                   0.365, 0.211, 0.083, 0.023, -0.015, -0.017, -0.009])  

V_642 = np.array([0.728, 0.255, 0.143, -0.110, -0.273, -0.326, 
                 -0.355, -0.381, -0.397, -0.451, -0.558, -0.651])
Vi_642 = np.array([0.636, 0.636, 0.631, 0.561, 0.401, 0.251, 
                   0.172, 0.098, 0.042, 0.003, -0.003, -0.008])                            

#graph 1
x = V_552[17:23]
y = Vi_552[17:23]
V_552_approx = np.linspace(-0.7, -0.5, 1000)
Vi_552_approx = k_coef(x, y) * V_552_approx + b_coef(x, y)
v01 = -1 * b_coef(x, y) / k_coef(x, y)

#graph 2
x = V_588[10:14]
y = Vi_588[10:14]
V_588_approx = np.linspace(-0.6, -0.42, 1000)
Vi_588_approx = k_coef(x, y) * V_588_approx + b_coef(x, y)
v02 = -1 * b_coef(x, y) / k_coef(x, y)

#graph 3
x = V_615[7:11]
y = Vi_615[7:11]
V_615_approx = np.linspace(-0.5, -0.34, 1000)
Vi_615_approx = k_coef(x, y) * V_615_approx + b_coef(x, y)
v03 = -1 * b_coef(x, y) / k_coef(x, y)

#graph 4
x = V_642[4:9]
y = Vi_642[4:9]
V_642_approx = np.linspace(-0.42, -0.24, 1000)
Vi_642_approx = k_coef(x, y) * V_642_approx + b_coef(x, y)
v04 = -1 * b_coef(x, y) / k_coef(x, y)

mpl.rcParams['font.size'] = 16
plt.figure(figsize=(16,9))
plt.xlabel("V, В")
plt.ylabel("VI, В")
#plt.errorbar(x, y, xerr = x_error, fmt = '.')
plt.plot(V_552, Vi_552, 'o', label="552 нм", color="k", markersize=8)
plt.plot(V_588, Vi_588, 'o', label="588 нм", color="b", markersize=8)
plt.plot(V_615, Vi_615, 'o', label="615 нм", color="g", markersize=8)
plt.plot(V_642, Vi_642, 'o', label="642 нм", color="r", markersize=8)
plt.plot(V_552_approx, Vi_552_approx, 'o', color="k", markersize=2)
plt.plot(V_588_approx, Vi_588_approx, 'o', color="b", markersize=2)
plt.plot(V_615_approx, Vi_615_approx, 'o', color="g", markersize=2)
plt.plot(V_642_approx, Vi_642_approx, 'o', color="r", markersize=2)
plt.minorticks_on()
plt.grid(which='major', axis='both')
plt.grid(which='minor', axis='both', linestyle = ':')
plt.legend()
plt.savefig("main")                              

# V0(frec)
x = np.array([3413, 3204, 3063, 2935])
y = -np.array([v01, v02, v03, v04])

print(k_coef(x, y))
print(k_sigma(x, y))
print(b_coef(x, y))
print(b_sigma(x, y))

x_approx = np.linspace(2800, 3500, 1000)
y_approx = x_approx * k_coef(x, y) + b_coef(x, y)

y_error = np.array([0.04, 0.02, 0.05, 0.02])
x_error = np.array([10.20, 10.23, 10.26, 10.28])

mpl.rcParams['font.size'] = 16
plt.figure(figsize=(16,9))
plt.xlabel("V, В")
plt.ylabel("VI, В")
plt.errorbar(x, y, xerr = x_error, yerr = y_error, fmt = '.', color="k")
plt.plot(x, y, 'o', markersize=8, color="k")
plt.plot(x_approx, y_approx, 'o', markersize=1)
plt.minorticks_on()
plt.grid(which='major', axis='both')
plt.grid(which='minor', axis='both', linestyle = ':')
plt.legend()
plt.savefig("last")                              


