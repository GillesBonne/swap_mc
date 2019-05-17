#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt

num_spheres = 1000

iterations = []
energy = []

with open("../../data/data/iterations.txt","r") as file:
    for line in file:
        iterations.append(float(line))
with open("../../data/data/energy.txt","r") as file:
    for line in file:
        energy.append(float(line))

iterations = np.array(iterations)
energy = np.array(energy)

time = iterations / num_spheres

fig = plt.figure()
plt.semilogx(time, energy)
plt.xlabel('Time')
plt.ylabel('Energy')
fig.savefig("visuals/Energy.pdf", bbox_inches='tight')
plt.show()
