#!/usr/bin/python3

import subprocess
import numpy as np
import os
import matplotlib.pyplot as plt

import config

def read_file(path, is_integer):
    with open(path,"r") as file:
        file_content = []
        for line in file:
            if is_integer:
                file_content.append(int(line))
            else:
                file_content.append(float(line))
        return file_content

def change_swap_probability_to(p_swap):
    with open("config.txt","r") as config_file:
        with open("new_config.txt","w") as new_config_file:
            for line_individual in config_file:
                line = line_individual.replace(" ", "")
                line_split = line.split("=")
                if line_split[0] == "swapProbability":
                    new_config_file.write("swapProbability = "+str(p_swap)+"\n")
                else:
                    new_config_file.write(line_individual)
            os.rename("new_config.txt","config.txt")

num_iterations                                  = 50001
skip_samples                                    = 1000
num_spheres                                     = 1000
ratio_size_sphere                               = 1.2
num_density                                     = 1.0
temperature_fixed                               = 0.15
max_translation_distance_in_max_particle_size   = 0.15
swap_probability                                = 0.1

config.change_config_to(num_iterations,
                    skip_samples,
                    num_spheres,
                    ratio_size_sphere,
                    num_density,
                    temperature_fixed,
                    max_translation_distance_in_max_particle_size,
                    swap_probability)

num_averages = 1

subprocess.call(["bin/runner 0"])
iterations = read_file(path = "data/data/iterations.txt", is_integer=True)
num_samples = len(iterations)

with open("data/data/lastConfig.txt","r") as config_file:
    for line_individual in config_file:
        line = line_individual.replace(" ", "")
        line_split = line.split("=")
        if line_split[0] == "numSpheres":
            num_spheres = int(line_split[1])

swap_probabilities = np.array([0.0, 0.1, 0.2])
num_swap_probabilities = swap_probabilities.size

energy = np.zeros((num_samples,num_swap_probabilities))
pressure = np.zeros((num_samples,num_swap_probabilities))

for i,p in enumerate(swap_probabilities):
    print("swap probability: "+str(p))
    change_swap_probability_to(p)
    for iteration in range(num_averages):
        print("num times:"+str(iteration))
        subprocess.call(["bin/runner"])

        E = read_file(path = "data/data/energy.txt", is_integer=False)
        for j, e in enumerate(E):
            energy[j][i] += e
        P = read_file(path = "data/data/pressure.txt", is_integer=False)
        for j, p in enumerate(P):
            pressure[j][i] += p

energy /= num_averages
pressure /= num_averages

time = np.zeros(len(iterations))
for i,iteration in enumerate(iterations):
    time[i] = iteration/num_spheres

fig = plt.figure()
for i, p_swap in enumerate(swap_probabilities):
    plt.semilogx(time, energy[:,i], label=("Swap probability: "+str(p_swap)))

plt.legend(loc="best")
plt.xlabel("Time")
plt.ylabel("Energy")
plt.tick_params(which="both", direction="in")
plt.ylim(bottom=0)
fig.savefig("simulations_automated/visuals/swap_energy_equilibration.pdf", bbox_inches='tight')
plt.show()

fig = plt.figure()
for i, p_swap in enumerate(swap_probabilities):
    plt.semilogx(time, pressure[:,i], label=("Swap probability: "+str(p_swap)))

plt.legend(loc="best")
plt.xlabel("Time")
plt.ylabel("Pressure")
plt.tick_params(which="both", direction="in")
plt.ylim(bottom=0)
fig.savefig("simulations_automated/visuals/swap_pressure_equilibration.pdf", bbox_inches='tight')
plt.show()
