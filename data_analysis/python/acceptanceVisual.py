#!/usr/bin/python3

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

fig = plt.figure()
plt.plot(iterations, swap_acceptance)
plt.xlabel('Iteration')
plt.ylabel('Swap acceptance')
fig.savefig("visuals/SwapAcceptance.pdf", bbox_inches='tight')
plt.show()

fig = plt.figure()
plt.plot(iterations, translation_acceptance)
plt.xlabel('Iteration')
plt.ylabel('Translation acceptance')
fig.savefig("visuals/TranslationAcceptance.pdf", bbox_inches='tight')
plt.show()
