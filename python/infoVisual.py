#!/usr/bin/python3

import sys

if(len(sys.argv)==1):
    id = ""
elif(len(sys.argv)==2):
    id = sys.argv[1]

import numpy as np
import matplotlib.pyplot as plt

config_path = "data/data" + id + "/lastConfig.txt"

with open(config_path,"r") as config_file:
    for line_individual in config_file:
        line = line_individual.replace(" ", "")
        line_split = line.split("=")
        if line_split[0] == "numSpheres":
            num_spheres = int(line_split[1])

iterations = []
energy = []
pressure = []
swap_acceptance = []
translation_acceptance = []

iterations_path = "data/data" + id + "/iterations.txt"
energy_path = "data/data" + id + "/energy.txt"
pressure_path = "data/data" + id + "/pressure.txt"
swap_path = "data/data" + id + "/swapAcceptance.txt"
translation_path = "data/data" + id + "/translationAcceptance.txt"

with open(iterations_path,"r") as file:
    for line in file:
        iterations.append(float(line))
with open(energy_path,"r") as file:
    for line in file:
        energy.append(float(line))
with open(pressure_path,"r") as file:
    for line in file:
        pressure.append(float(line))
with open(swap_path,"r") as file:
    for line in file:
        swap_acceptance.append(float(line))
with open(translation_path,"r") as file:
    for line in file:
        translation_acceptance.append(float(line))

time = np.array(iterations) / num_spheres
energy = np.array(energy)
pressure = np.array(pressure)

for i in range(37):
    del iterations[0]
    del swap_acceptance[0]
    del translation_acceptance[0]

time_short = np.array(iterations) / num_spheres

swap_acceptance = np.array(swap_acceptance)
translation_acceptance = np.array(translation_acceptance)

visual_path_energy = "visuals/visuals" + id + "/energy.pdf"
fig = plt.figure()
plt.semilogx(time, energy)
plt.xlabel('Time')
plt.ylabel('Energy')
plt.tick_params(which="both", direction="in")
plt.ylim(bottom=0)
fig.savefig(visual_path_energy, bbox_inches='tight')

visual_path_pressure = "visuals/visuals" + id + "/pressure.pdf"
fig = plt.figure()
plt.semilogx(time, pressure)
plt.xlabel('Time')
plt.ylabel('Pressure')
plt.tick_params(which="both", direction="in")
plt.ylim(bottom=0)
fig.savefig(visual_path_pressure, bbox_inches='tight')

visual_swap_path = "visuals/visuals" + id + "/swapAcceptance.pdf"
fig = plt.figure()
plt.plot(time_short, swap_acceptance)
plt.xlabel('Time')
plt.ylabel('Swap acceptance')
plt.tick_params(which="both", direction="in")
plt.ylim(bottom=0, top=1)
fig.savefig(visual_swap_path, bbox_inches='tight')

visual_translation_path = "visuals/visuals" + id + "/translationAcceptance.pdf"
fig = plt.figure()
plt.plot(time_short, translation_acceptance)
plt.xlabel('Time')
plt.ylabel('Translation acceptance')
plt.tick_params(which="both", direction="in")
plt.ylim(bottom=0, top=1)
fig.savefig(visual_translation_path, bbox_inches='tight')

