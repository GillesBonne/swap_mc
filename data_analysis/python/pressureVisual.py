#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt

num_spheres = 1000

iterations = []
pressure = []

with open("../../data/data/iterations.txt","r") as file:
    for line in file:
        iterations.append(float(line))
with open("../../data/data/pressure.txt","r") as file:
    for line in file:
        pressure.append(float(line))

iterations = np.array(iterations)
pressure = np.array(pressure)

time = iterations / num_spheres

fig = plt.figure()
plt.semilogx(time, pressure)
plt.xlabel('Time')
plt.ylabel('Pressure')
fig.savefig("visuals/Pressure.pdf", bbox_inches='tight')
plt.show()
