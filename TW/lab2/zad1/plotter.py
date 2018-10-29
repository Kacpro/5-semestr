import matplotlib.pyplot as plt
import numpy as np
import matplotlib.patches as pat


DIV = 100

f = open('data', 'r')
y = []

for line in f:
    for word in line.split():
        y.append(int(word))

N = int(len(y)/2)
print(len(y))
x = np.arange(1, N+1) * DIV
width = 0.3 * DIV

print(len(x))

ax = plt.subplot(111)
ax.set_yscale('log')

blue_patch = pat.Patch(color='blue', label='consumer')
red_patch = pat.Patch(color='red', label='producer')
plt.legend(handles=[blue_patch, red_patch])

ax.bar(x-width/2, y[:N], width, color="blue")
ax.bar(x + width/2, y[N:], width, color="red")
plt.show()
