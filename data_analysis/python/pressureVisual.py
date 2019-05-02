import numpy as np
import matplotlib.pyplot as plt

iterations = []
pressure = []

with open("../../data/iterations.txt","r") as file:
    for line in file:
        iterations.append(float(line))
with open("../../data/pressure.txt","r") as file:
    for line in file:
        pressure.append(float(line))

plt.plot(iterations, pressure)
plt.show()
