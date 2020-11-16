import matplotlib.pyplot as plt
import numpy as np

data = np.loadtxt('build/result.txt')

reference = np.loadtxt('build/reference.txt')

plt.xlabel('f1')
plt.xlabel('f2')
plt.scatter(data[:, 0], data[:, 1], c='blue')
plt.scatter(reference[:, 0], reference[:, 1], c='red')
plt.show()
