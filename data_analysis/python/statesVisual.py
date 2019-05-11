#!/usr/bin/python3

from vpython import *
import numpy as np

with open("../../data/lastConfig.txt","r") as config_file:
    for line_individual in config_file:
        line = line_individual.replace(" ", "")
        line_split = line.split("=")
        if line_split[0] == "numIterations":
            num_iterations = int(line_split[1])
        if line_split[0] == "numSpheres":
            num_spheres = int(line_split[1])
        if line_split[0] == "numDensity":
            num_density = float(line_split[1])
        if line_split[0] == "skipSamples":
            skip_samples = float(line_split[1])
            
num_sample_steps = 0
for i in range(num_iterations):
    if(i%skip_samples==0):
        num_sample_steps += 1

iterations = np.zeros(num_sample_steps)
x = np.zeros((num_spheres,num_sample_steps))
y = np.zeros((num_spheres,num_sample_steps))
z = np.zeros((num_spheres,num_sample_steps))
radius = np.zeros((num_spheres,num_sample_steps))

iteration_on_line = False
sample_step = 0
with open("../../data/outputStates.txt","r") as file:
    for line in file:
        if iteration_on_line:
            iteration = int(line)
            if iteration == 0:
                sample_step = 0
            else:
                sample_step += 1
            iterations[sample_step] = iteration
            iteration_on_line = False
        elif line[0]=="i":
            iteration_on_line = True;
            particle_index = 0
        else:
            line_split = line.split(",")
            x[particle_index][sample_step] = float(line_split[0])
            y[particle_index][sample_step] = float(line_split[1])
            z[particle_index][sample_step] = float(line_split[2])
            radius[particle_index][sample_step] = float(line_split[3])
            particle_index += 1

volume_box = num_spheres/num_density
length_box = volume_box**(1/3)

# Visualize axis
x_axis = arrow(pos=vector(0,0,0), axis=vector(length_box,0,0), color=color.red)
y_axis = arrow(pos=vector(0,0,0), axis=vector(0,length_box,0), color=color.green)
z_axis = arrow(pos=vector(0,0,0), axis=vector(0,0,length_box), color=color.blue)

spheres = []
for i in range(num_spheres):
    spheres.append(sphere(radius=radius[i][0], pos=vector(x[i][0],y[i][0],z[i][0])))

for i in range(1,num_sample_steps):
    sleep(0.1)
    for j in range(num_spheres):
        spheres[j].radius = radius[j][i]
        spheres[j].pos.x = x[j][i]
        spheres[j].pos.y = y[j][i]
        spheres[j].pos.z = z[j][i]
