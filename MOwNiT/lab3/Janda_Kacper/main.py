import numpy as np
import random as rd
import time
import matplotlib.pyplot as plt
import matplotlib.patches as pat
import copy

# Przykładowe macierze testowe znajdują się na końcu pliku


def gaussianMethod(A, b):
    for k in range(len(A)):
        for i in range(k + 1,  len(A)):
            div = A[i][k] / A[k][k]
            for j in range(k, len(A[0])):
                A[i][j] = A[i][j] - div * A[k][j]
            b[i] = b[i] - div * b[k]
    x = [0 for _ in A[0]]
    for i in range(len(A) - 1, -1, -1):
        sum = 0
        for j in range(i+1, len(A)):
            sum += A[i][j] * x[j]
        x[i] = (b[i] - sum)/A[i][i]
    return x

###########################################################################

def numpyMethod(A, b):
    return np.linalg.solve(A, b).tolist()

###########################################################################

def luFactorisation(A):
    L = [[0 for _ in A[0]] for _ in A]
    U = [[0 for _ in A[0]] for _ in A]
    for i in range(len(A)):
        sum = 0
        L[i][i] = 1
        for k in range(i):
            sum += L[i][k] * U[k][i]
        U[i][i] = (A[i][i] - sum)

        for j in range(i+1, len(A)):
            sum = 0
            for k in range(i):
                sum += L[j][k] * U[k][i]
            L[j][i] = (A[j][i] - sum)/U[i][i]

            sum = 0
            for k in range(i):
                sum += U[k][j] * L[i][k]
            U[i][j] = (A[i][j] - sum)/L[i][i]
    return L, U


def solveUpper(U, b):
    x = [0 for _ in U[0]]
    for i in range(len(U) - 1, -1, -1):
        sum = 0
        for j in range(i+1, len(U)):
            sum += U[i][j] * x[j]
        x[i] = (b[i] - sum)/U[i][i]
    return x

def solveLower(L, b):
    x = [0 for _ in L[0]]
    for i in range(len(L)):
        sum = 0
        for j in range(i):
            sum += L[i][j] * x[j]
        x[i] = (b[i] - sum)/L[i][i]
    return x


def luMethod(A, b):
    L, U = luFactorisation(A)
    return solveUpper(U, solveLower(L, b))

###########################################################################

def luScalingMethod(A, b):
    p = [i for i in range(0, len(A))]
    s = [max([abs(e) for e in x]) for x in A]

    for k in range(len(A)-1):
        r = 0
        m = abs(A[p[0]][k]) / s[p[0]]
        for j in range(k, len(A)):
            if abs(A[p[j]][k]) / s[p[j]] > m:
                r = j
                m = abs(A[p[j]][k]) / s[p[j]]
        p[k], p[r] = p[r], p[k]
        for i in range(k+1, len(A)):
            z = A[p[i]][k] / A[p[k]][k]
            A[p[i]][k] = z
            for j in range(k+1, len(A)):
                A[p[i]][j] = A[p[i]][j] - z * A[p[k]][j]

    for k in range(len(A) - 1):
        for i in range(k+1, len(A)):
            b[p[i]] = b[p[i]] - A[p[i]][k] * b[p[k]]

    x = [0 for _ in range(len(A))]
    for i in range(len(A) - 1, -1, -1):
        sum = 0
        for j in range(i+1, len(A)):
            sum += A[p[i]][j] * x[j]
        x[i] = (b[p[i]] - sum)/A[p[i]][i]

    return x

###########################################################################

def genMatrix(columns,rows):
    return [[rd.randint(1, 50) for _ in range(rows)] for _ in range(columns)]

def genVector(rows):
    return [rd.randint(1, 50) for e in range(rows)]



def runTests():
    X = []
    Y1 = []
    Y2 = []
    Y3 = []
    Y4 = []

    for i in range(1, 500, 10):
        A = genMatrix(i, i)
        b = genVector(i)

        X.append(i)

        start = time.time()
        luScalingMethod(copy.deepcopy(A), copy.deepcopy(b))
        end = time.time()
        t4 = end - start
        Y4.append(t4)

        start = time.time()
        gaussianMethod(copy.deepcopy(A), copy.deepcopy(b))
        end = time.time()
        t1 = end - start
        Y1.append(t1)

        start = time.time()
        luMethod(copy.deepcopy(A), copy.deepcopy(b))
        end = time.time()
        t2 = end - start
        Y2.append(t2)

        start = time.time()
        numpyMethod(copy.deepcopy(A), copy.deepcopy(b))
        end = time.time()
        t3 = end - start
        Y3.append(t3)

        print(i, t1, t2, t3, t4)

    blue_patch = pat.Patch(color='blue', label='metoda Gaussa')
    orange_patch = pat.Patch(color='orange', label='faktoryzacja LU')
    green_patch = pat.Patch(color='green', label='funkcja biblioteczna')
    red_patch = pat.Patch(color='red', label='skalowana faktoryzacja LU')
    plt.legend(handles=[blue_patch, orange_patch, green_patch, red_patch])

    plt.plot(X, Y1)
    plt.plot(X, Y2)
    plt.plot(X, Y3)
    plt.plot(X, Y4)
    plt.show()


def single_test(A, b):
    try:
        start = time.time()
        x = luScalingMethod(copy.deepcopy(A), copy.deepcopy(b))
        end = time.time()
        t4 = end - start
        print("Skalowana faktoryzacja LU: ", t4, x)
    except:
        print("Error")

    try:
        start = time.time()
        x = gaussianMethod(copy.deepcopy(A), copy.deepcopy(b))
        end = time.time()
        t1 = end - start
        print("Metoda Gaussa: ", t1, x)
    except:
        print("Error")

    try:
        start = time.time()
        x = luMethod(copy.deepcopy(A), copy.deepcopy(b))
        end = time.time()
        t2 = end - start
        print("Faktoryzacja LU: ", t2, x)
    except:
        print("Error")

    try:
        start = time.time()
        x = numpyMethod(copy.deepcopy(A), copy.deepcopy(b))
        end = time.time()
        t3 = end - start
        print("Funkcja biblioteczna: ", t3, x)
    except:
        print("Error")




# Zero na przekątnej
# A = [[0, 1], [1, 2]];   b = [1,2];   single_test(A, b)

# Przykład błędów numerycznych
# A = [[1e-20, 1], [1, 1]];   b = [1,2];   single_test(A, b)


#runTests()
