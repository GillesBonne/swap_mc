import numpy as np
import matplotlib.pyplot as plt

iterations = []
swap_acceptance = []
translation_acceptance = []

with open("../../data/iterations.txt","r") as file:
    for line in file:
        iterations.append(float(line))
with open("../../data/swapAcceptance.txt","r") as file:
    for line in file:
        swap_acceptance.append(float(line))
with open("../../data/translationAcceptance.txt","r") as file:
    for line in file:
        translation_acceptance.append(float(line))

del iterations[0]
del swap_acceptance[0]
del translation_acceptance[0]

plt.plot(iterations, swap_acceptance)
plt.show()
plt.plot(iterations, translation_acceptance)
plt.show()