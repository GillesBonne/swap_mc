#!/usr/bin/python3

import sys
import numpy as np
import matplotlib.pyplot as plt

if(len(sys.argv)==3):
    data_path = sys.argv[1]
    visual_path = sys.argv[2]
else:
    print("Not the right number of arguments")
    sys.exit()

config_path = data_path + "/config.txt"

with open(config_path,"r") as config_file:
    for line_individual in config_file:
        line = line_individual.replace(" ", "")
        line_split = line.split("=")
        if line_split[0] == "numSpheres":
            num_spheres = int(line_split[1])

iterations = []
energy = []
swap_acceptance = []
translation_acceptance = []

iterations_path = data_path + "/iterations.txt"
energy_path = data_path + "/energy.txt"
swap_path = data_path + "/swapAcceptance.txt"
translation_path = data_path + "/translationAcceptance.txt"

with open(iterations_path,"r") as file:
    for line in file:
        iterations.append(float(line))
with open(energy_path,"r") as file:
    for line in file:
        energy.append(float(line))
with open(swap_path,"r") as file:
    for line in file:
        swap_acceptance.append(float(line))
with open(translation_path,"r") as file:
    for line in file:
        translation_acceptance.append(float(line))

time = np.array(iterations) / num_spheres
energy = np.array(energy)

for i in range(37):
    del iterations[0]
    del swap_acceptance[0]
    del translation_acceptance[0]

time_short = np.array(iterations) / num_spheres

swap_acceptance = np.array(swap_acceptance)
translation_acceptance = np.array(translation_acceptance)

visual_path_energy = visual_path + "/energy.pdf"
fig = plt.figure()
plt.semilogx(time, energy)
plt.xlabel('Time')
plt.ylabel('Energy')
plt.title("ave: "+str(np.mean(energy))+", last: "+str(energy[-1]))
plt.tick_params(which="both", direction="in")
fig.savefig(visual_path_energy, bbox_inches='tight')

visual_swap_path = visual_path + "/swapAcceptance.pdf"
fig = plt.figure()
plt.plot(time_short, swap_acceptance)
plt.xlabel('Time')
plt.ylabel('Swap acceptance')
plt.title("ave: "+str(np.mean(swap_acceptance))+", last: "+str(swap_acceptance[-1]))
plt.tick_params(which="both", direction="in")
fig.savefig(visual_swap_path, bbox_inches='tight')

visual_translation_path = visual_path + "/translationAcceptance.pdf"
fig = plt.figure()
plt.plot(time_short, translation_acceptance)
plt.xlabel('Time')
plt.ylabel('Translation acceptance')
plt.title("ave: "+str(np.mean(translation_acceptance))+", last: "+str(translation_acceptance[-1]))
plt.tick_params(which="both", direction="in")
fig.savefig(visual_translation_path, bbox_inches='tight')

output_path = data_path + "/lastState.txt"

radii = np.zeros(num_spheres)

iteration_on_line = False
countFirst = 0
with open(output_path,"r") as in_file:
    for i,line in enumerate(in_file):
        if iteration_on_line:
            iteration_on_line = False
        elif line[0]=="i":
            if countFirst == 1:
                break
            iteration_on_line = True
            countFirst += 1
        else:
            line_split = line.split(",")
            radii[i-2] = float(line_split[3])

unique, counts = np.unique(radii, return_counts=True)
if len(unique)<25:
    np.set_printoptions(suppress=True)
    print(np.asarray((unique, counts)).T)
    num_bins = len(unique)
else:
    num_bins = 24

visual_size_path = visual_path + "/sizeHistogram.pdf"
fig = plt.figure()
plt.hist(radii, bins=num_bins)
plt.xlabel('Radius')
plt.ylabel('Number of spheres')
plt.title("ave: "+str(sum(radii)/len(radii))+", num unique: "+str(len(unique)))
plt.tick_params(which="both", direction="in")
plt.ylim(bottom=0)
fig.savefig(visual_size_path, bbox_inches='tight')
