#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt

iterations = []
pressure = []

with open("../../data/data/iterations.txt","r") as file:
    for line in file:
        iterations.append(float(line))
with open("../../data/data/pressure.txt","r") as file:
    for line in file:
        pressure.append(float(line))

fig = plt.figure()
plt.plot(iterations, pressure)
plt.xlabel('Iteration')
plt.ylabel('Pressure')
fig.savefig("visuals/Pressure.pdf", bbox_inches='tight')
plt.show()
