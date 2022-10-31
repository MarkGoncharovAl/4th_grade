
import matplotlib.pyplot as plt
from mpl_toolkits.axes_grid1 import host_subplot
import mpl_toolkits.axisartist as AA
import numpy

x = []
y = []
y2 = []
if len(sys.argv) != 3:
    print('ERROR: files not passed as argument')

for line in open(sys.argv[1], 'r'):
    lines = [i for i in line.split()]
    x.append(lines[0])
    y.append(lines[1])
for line in open(sys.argv[2], 'r'):
    lines = [i for i in line.split()]
    y2.append(lines[1])

fig = plt.figure()
plt.xlabel('Width')
plt.ylabel('Height')
ax = fig.gca()
ax.plot(x, y, 'ro')
ax.plot(x, y2, 'ro')
plt.show()