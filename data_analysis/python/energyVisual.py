import numpy as np
import matplotlib.pyplot as plt

iterations = []
energy = []

with open("../../data/iterations.txt","r") as file:
    for line in file:
        iterations.append(float(line))
with open("../../data/energy.txt","r") as file:
    for line in file:
        energy.append(float(line))

plt.plot(iterations, energy)
plt.show()
