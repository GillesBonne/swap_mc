#!/usr/bin/python3

import sys

if(len(sys.argv)==1):
    id = ""
elif(len(sys.argv)==2):
    id = sys.argv[1]

import numpy as np
import matplotlib.pyplot as plt

config_path = "data/data" + id + "/lastConfig.txt"
input_path = "data/data" + id + "/outputStates.txt"
output_path = "data/data" + id + "/stateConfigs.xyz"

with open(config_path,"r") as config_file:
    for line_individual in config_file:
        line = line_individual.replace(" ", "")
        line_split = line.split("=")
        if line_split[0] == "numSpheres":
            num_spheres = int(line_split[1])

radii = np.zeros(num_spheres)

iteration_on_line = False
countFirst = 0
with open(input_path,"r") as in_file:
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
    num_bins = "auto"

print("Average radius: "+str(sum(radii)/len(radii)))

visual_size_path = "visuals/visuals" + id + "/sizeHistogram.pdf"
fig = plt.figure()
plt.hist(radii, bins=num_bins)
plt.xlabel('Radius')
plt.ylabel('Number of spheres')
plt.tick_params(which="both", direction="in")
plt.ylim(bottom=0)
fig.savefig(visual_size_path, bbox_inches='tight')
