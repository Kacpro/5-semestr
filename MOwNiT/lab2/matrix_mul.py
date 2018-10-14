import random
import time
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as pat

def simple_mul(m1, m2):
    start = time.time()
    result = []
    for i in range(len(m1)):
        row = []
        for j in range(len(m2[0])):
            s = 0
            for k in range(len(m1[0])):
                s += m1[i][k] * m2[k][j]
            row.append(s) 
        result.append(row)
    end = time.time()
    #print((end - start))
    return (end - start)


def gen_matrices(r1, c2, c1_r2):
    m1 = []
    m2 = []
    for i in range(r1):
        row = []
        for j in range(c1_r2):
            row.append(random.randint(0, 100))
        m1.append(row)
    for i in range(c1_r2):
        row = []
        for j in range(c2):
            row.append(random.randint(0, 100))
        m2.append(row)
    return m1, m2


def numpy_mul(m1, m2):
    start = time.time()
    result = np.matmul(m1, m2)
    end = time.time()
    return (end - start)


def run_tests():
    X = []
    Y1 = []
    Y2 = []
    Y3 = [] 
    for i in range(1, 100, 1):
        print(i)
        a,b = gen_matrices(i,i,100*i)
	a_t = np.transpose(a).tolist()
	b_t = np.transpose(b).tolist()
        X.append(i)
        Y1.append(simple_mul(a,b))
        Y2.append(simple_mul(b_t,a_t))
        Y3.append(numpy_mul(a,b))
	
    blue_patch = pat.Patch(color='blue', label='algorytm naiwny')
    orange_patch = pat.Patch(color='orange', label='algorytm naiwny (odwrocony)')
    green_patch = pat.Patch(color='green', label='funkcja biblioteczna')
    plt.legend(handles=[blue_patch, orange_patch, green_patch])

    plt.plot(X, Y1)
    plt.plot(X, Y2)
    plt.plot(X, Y3)
    plt.show()


run_tests()

