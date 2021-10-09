import numpy as np

class LU_dec:

    # 1) Preparing LU matrix that consists of L and U
    def __init__(self, A): #A = np.matrix
        n = A.shape[0]
        if (n != A.shape[1]):
            print("A must be square!")
            return

        self.LU = np.matrix(np.zeros([n, n]))
        
        for k in range(n):
            for j in range(k, n):
                # U-matrix
                self.LU[k, j] = A[k, j] - self.LU[k, :k] * self.LU[:k, j]
            for i in range(k + 1, n):
                # L-matrix
                self.LU[i, k] = (A[i, k] - self.LU[i, : k] * self.LU[: k, k]) / self.LU[k, k]

    # 2) L and U debug information about concrete matrixes
    def L(self):
        L = self.LU.copy()
        
        for i in range(L.shape[0]):
            L[i, i] = 1
            L[i, i+1 :] = 0
        return L

    def U(self):
        U = self.LU.copy()
        for i in range(1, U.shape[0]):
            U[i, :i] = 0
        return U

    # 3) Solving 2 linear equations
    def solve(self, f): # f - np.array
        if (f.size != self.LU.shape[0]):
            print("f must have the same size as matrix!")
            return

        n = f.size
        solution = f.copy()
        for i in range(1, n, 1):
            for j in range(0, i, 1):
                solution[i] = solution[i] - solution[j] * self.LU[i, j]

        for i in range(n - 1, -1, -1):
            for j in range(i + 1, n, 1):
                solution[i] -= solution[j] * self.LU[i, j]
            solution[i] /= self.LU[i, i]
        return solution

# 4) Checking for Jacobi-algorithm opportunity
def diag_dom(A):
    n = A.shape[0]
    if (n != A.shape[1]):
        print("A must be square!")
        return False

    for i in range(n):
        sum = 0
        for j in range(n):
            sum += np.abs(A[i, j])
        if (2 * np.abs(A[i, i]) <= sum):
            return False
    return True

class Jac:

    # 5) Preparing B and D matrixes

    def __init__(self, A):
        n = A.shape[0]
        if (n != A.shape[1]):
            print("A must be square!")
            return
        if (not diag_dom(A)):
            print("A must be diagonal predominance!")
            return
        
        self.D = np.matrix(np.zeros([n, n]))
        A_copy = A.copy()
        for i in range(n):
            self.D[i, i] = -1. / A[i, i]
            A_copy[i, i] = 0
        
        # Now A = L + U and D is diagonal 

        self.B = self.D * A_copy 

    # 6) Making iterations

    def solve(self, f, iter = 100):
        if (f.size != self.B.shape[0]):
            print("f must have the same size as matrix!")
            return        
        
        n = f.size
        F = np.matrix(np.zeros([n, 1]))
        for i in range (n):
            F[i, 0] =  -1. * f[i]
        F = self.D * F
        sol = np.array(np.zeros([n, 1]))
        for i in range(iter):
            sol = self.B * sol + F

        out = np.array(np.zeros(n))
        for i in range(n):
            out[i] = sol[i, 0]
        return out



A = np.matrix([[7, 1, 1], 
               [1, 5, -1],
               [0, 1, 6]])
f = np.array([9., 5., 7.])

LU = LU_dec(A)
Z = Jac(A)

#Checking for correct LU decomposition
print("A\n", A, "\n")
print("f\n", f, "\n")

print("LU checking\n", LU.L() * LU.U(), "\n")

sol1 = LU.solve(f)
sol2 = Z.solve(f)

print("Solution 1: ", sol1)
print("Solution 2: ", sol2)