#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt

num_spheres = 1000

iterations = []
swap_acceptance = []
translation_acceptance = []

with open("../../data/data/iterations.txt","r") as file:
    for line in file:
        iterations.append(float(line))
with open("../../data/data/swapAcceptance.txt","r") as file:
    for line in file:
        swap_acceptance.append(float(line))
with open("../../data/data/translationAcceptance.txt","r") as file:
    for line in file:
        translation_acceptance.append(float(line))

for i in range(28):
    del iterations[0]
    del swap_acceptance[0]
    del translation_acceptance[0]
    
iterations = np.array(iterations)
swap_acceptance = np.array(swap_acceptance)
translation_acceptance = np.array(translation_acceptance)

time = iterations / num_spheres

fig = plt.figure()
plt.semilogx(time, swap_acceptance)
plt.xlabel('Time')
plt.ylabel('Swap acceptance')
fig.savefig("visuals/SwapAcceptance.pdf", bbox_inches='tight')
plt.show()

fig = plt.figure()
plt.semilogx(time, translation_acceptance)
plt.xlabel('Time')
plt.ylabel('Translation acceptance')
fig.savefig("visuals/TranslationAcceptance.pdf", bbox_inches='tight')
plt.show()
