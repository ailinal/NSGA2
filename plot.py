import matplotlib.pyplot as plt
import numpy as np

data1 = np.loadtxt('build/result_200500.txt')
data2 = np.loadtxt('build/result_2001000.txt')
data3 = np.loadtxt('build/result_5001000.txt')
data4 = np.loadtxt('build/result_5002000.txt')

reference = np.loadtxt('build/reference.txt')

fig, ax = plt.subplots(2,2)
axs = ax.flatten()



def plot_ax(ax, data, reference):
    ax.scatter(data[:, 0], data[:, 1], c='blue')
    ax.scatter(reference[:, 0], reference[:, 1], c='red')
#    ax.xlabel('f1')
#    ax.ylabel('f2')


plot_ax(axs[0],data1,reference)


plt.show()
