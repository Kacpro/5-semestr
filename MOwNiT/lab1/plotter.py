import matplotlib.pyplot as plt
import matplotlib.patches as pat

file1 = open("./src/float_simple_time")
X1 = []
Y1 = []
for line in file1:
    X1.append(float(line.split()[0]))
    Y1.append(float(line.split()[1]))

# file2 = open("./src/double_simple_time")
# X2 = []
# Y2 = []
# for line in file2:
#     X2.append(float(line.split()[0]))
#     Y2.append(float(line.split()[1]))

file3 = open("./src/float_rec_time")
X3 = []
Y3 = []
for line in file3:
    X3.append(float(line.split()[0]))
    Y3.append(float(line.split()[1]))

file4 = open("./src/kahan_time")
X4 = []
Y4 = []
for line in file4:
    X4.append(float(line.split()[0]))
    Y4.append(float(line.split()[1]))

blue_patch = pat.Patch(color='blue', label='algorytm naiwny')
orange_patch = pat.Patch(color='orange', label='algorytm rekurencyjny')
green_patch = pat.Patch(color='green', label='algorytm Kahana')
plt.legend(handles=[blue_patch, orange_patch, green_patch])

plt.scatter(X1, Y1, s=0.1)
# plt.scatter(X2, Y2, s=0.1)
plt.scatter(X3, Y3, s=0.1)
plt.scatter(X4, Y4, s=0.1)

plt.title('Wydajność czasowa algorytmów')

plt.show()