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

fig = plt.figure()
plt.plot(iterations, energy)
plt.xlabel('Iteration')
plt.ylabel('Energy')
fig.savefig("visuals/Energy.pdf", bbox_inches='tight')
plt.show()
