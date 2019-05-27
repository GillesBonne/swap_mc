#!/usr/bin/python3

import subprocess
import numpy as np
import os
import matplotlib.pyplot as plt

import config

file_paths = ["configSTART.txt", "configCHANGE.txt"]
sim_ids = ["START", "CHANGE"]

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
    with open(file_paths[1],"r") as config_file:
        with open("new_config.txt","w") as new_config_file:
            for line_individual in config_file:
                line = line_individual.replace(" ", "")
                line_split = line.split("=")
                if line_split[0] == "swapProbability":
                    new_config_file.write("swapProbability = "+str(p_swap)+"\n")
                else:
                    new_config_file.write(line_individual)
            os.rename("new_config.txt",file_paths[1])

num_iterations                                  = 100000001
skip_samples                                    = 100000000
num_spheres                                     = 1000
ratio_size_sphere                               = 2.219
num_density                                     = 1.0
temperature_fixed                               = 0.25
max_translation_distance_in_length_units        = 0.1
swap_probability                                = 0.2

for file_path in file_paths:
    command = "cp config.txt " + str(file_path)
    os.system(command)

config.change_config_to(file_paths[0],
                        num_iterations,
                        skip_samples,
                        num_spheres,
                        ratio_size_sphere,
                        num_density,
                        0.5,
                        max_translation_distance_in_length_units,
                        swap_probability)

config.change_config_to(file_paths[1],
                        num_iterations,
                        skip_samples,
                        num_spheres,
                        ratio_size_sphere,
                        num_density,
                        temperature_fixed,
                        max_translation_distance_in_length_units,
                        swap_probability)

num_averages = 1

subprocess.call(["bin/runner", "0", sim_ids[0]])
iteration_path = "data/data" + sim_ids[0] + "/iterations.txt"
iterations = read_file(path = iteration_path, is_integer=True)
num_samples = len(iterations)

config_path = "data/data" + sim_ids[0] + "/lastConfig.txt"
with open(config_path,"r") as config_file:
    for line_individual in config_file:
        line = line_individual.replace(" ", "")
        line_split = line.split("=")
        if line_split[0] == "numSpheres":
            num_spheres = int(line_split[1])

swap_probabilities = np.array([0.0, 0.2])
num_swap_probabilities = swap_probabilities.size

energy = np.zeros((num_samples,num_swap_probabilities))
pressure = np.zeros((num_samples,num_swap_probabilities))

energy_path = "data/data" + sim_ids[1] + "/energy.txt"
pressure_path = "data/data" + sim_ids[1] + "/pressure.txt"
for i,p in enumerate(swap_probabilities):
    print("swap probability: "+str(p))
    change_swap_probability_to(p)
    for iteration in range(num_averages):
        print("num times:"+str(iteration))
        subprocess.call(["bin/runner", "0", sim_ids[1], sim_ids[0]])

        E = read_file(path = energy_path, is_integer=False)
        for j, e in enumerate(E):
            energy[j][i] += e
        P = read_file(path = pressure_path, is_integer=False)
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
#plt.ylim(bottom=0)
fig.savefig("simulations_automated/visuals/swap_energy_equilibration.pdf", bbox_inches='tight')
plt.show()

fig = plt.figure()
for i, p_swap in enumerate(swap_probabilities):
    plt.semilogx(time, pressure[:,i], label=("Swap probability: "+str(p_swap)))

plt.legend(loc="best")
plt.xlabel("Time")
plt.ylabel("Pressure")
plt.tick_params(which="both", direction="in")
#plt.ylim(bottom=0)
fig.savefig("simulations_automated/visuals/swap_pressure_equilibration.pdf", bbox_inches='tight')
plt.show()
