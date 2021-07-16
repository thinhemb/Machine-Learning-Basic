

import numpy as np
import matplotlib.pyplot as plt

# height (cm)
X = np.array([[147, 150, 153, 158, 163, 165, 168, 170, 173, 175, 178, 180, 183]]).T
# weight (kg)
y = np.array([[ 49, 50, 51,  54, 58, 59, 60, 62, 63, 64, 66, 67, 68]]).T

# display poin on the mal

plt.plot(X,y,'ro')
plt.axis([140, 190, 45, 75])
plt.xlabel('Height (cm)')
plt.ylabel('Weight (kg)')
plt.show()


one = np.ones((X.shape[0], 1))
print(one)


''' pt co dang : y = ax + b'''

''' moi hang la 1 cap gia tri 1 va x '''
Xbar = np.concatenate((one, X), axis = 1)

print(Xbar)

A = np.dot(Xbar.T,Xbar)

B = np.dot(Xbar.T,y)
res = np.dot(np.linalg.pinv(A),B)
print("Ket qua nghiem cong thuc : ", res)

print("Nhap chieu cao : ")
h = np.random.randint(140,185)
print("Chieu cao: " , h)
wo = res[0][0]
w1 = res[1][0]

w = w1*h + wo
print("Can nang du doan : " , w)

from sklearn import datasets, linear_model
res = linear_model.LinearRegression(fit_intercept=False).fit(Xbar,y)
print("KQ tu thu vien:" , res.coef_)

