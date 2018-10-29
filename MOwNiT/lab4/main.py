import numpy as np
import random as rd
import time
import matplotlib.pyplot as plt
import matplotlib.patches as pat
import copy


def luFacDoolittle(A):
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


def luFacCroute(A):
    L = [[0 for _ in A[0]] for _ in A]
    U = [[0 for _ in A[0]] for _ in A]
    for i in range(len(A)):
        sum = 0
        U[i][i] = 1
        for k in range(i):
            sum += L[i][k] * U[k][i]
        L[i][i] = (A[i][i] - sum)

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


def luFacCholeski(A):
    L = [[0 for _ in A[0]] for _ in A]
    U = [[0 for _ in A[0]] for _ in A]
    for i in range(len(A)):
        sum = 0
        for k in range(i):
            sum += L[i][k] * U[k][i]
        U[i][i] = np.sqrt(A[i][i] - sum)
        L[i][i] = np.sqrt(A[i][i] - sum)

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

######################################################################

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



def genMatrix(columns,rows):
    return [[rd.randint(1, 50) for _ in range(rows)] for _ in range(columns)]

def genVector(rows):
    return [rd.randint(1, 50) for _ in range(rows)]

def genSymetricMatrix(rows):
    A = genMatrix(rows, rows)
    for i in range(rows):
        for j in range(i, rows):
            A[i][j] = A[j][i]
    return A

#######################################################################

def luMethod(A, b):
    L, U = luFacDoolittle(A)
    return solveUpper(U, solveLower(L, b))


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

#######################################################################

def runTests():

    X = []
    Y1 = []
    Y2 = []

    for i in range(1, 600, 10):
        A = genMatrix(i, i)
        b = genVector(i)
        X.append(i)

        start = time.time()
        gaussianMethod(copy.deepcopy((A)), copy.deepcopy(b))
        end = time.time()
        Y1.append(end - start)

        start = time.time()
        luMethod(copy.deepcopy((A)), copy.deepcopy(b))
        end = time.time()
        Y2.append(end - start)

        print(i)

    blue_patch = pat.Patch(color='blue', label='metoda Gaussa')
    orange_patch = pat.Patch(color='orange', label='faktoryzacja LU')
    plt.legend(handles=[blue_patch, orange_patch])

    plt.plot(X, Y1)
    plt.plot(X, Y2)
    plt.show()


def runSingleTest():
    # Macierz symetryczna i dodatnio okreslona
    A = [[4, 12, -16], [12, 37, -43], [-16, -43, 98]]

    L, U = luFacDoolittle(copy.deepcopy(A))
    print("Metoda Doolittle'a")
    for x in L: print(x)
    print()
    for x in U: print(x)
    print()

    print("Metoda Croute'a")
    L, U = luFacCroute(copy.deepcopy(A))
    for x in L: print(x)
    print()
    for x in U: print(x)
    print()

    print("Metoda Choleskiego")
    L, U = luFacCholeski(copy.deepcopy(A))
    for x in L: print(x)
    print()
    for x in U: print(x)
    print()


# runSingleTest()
# runTests()

