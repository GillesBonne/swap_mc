from vpython import *

x = []
y = []
z = []
radius = []

file = open("../../data/outputStates.txt","r")
iteration_on_line = False
for line in file:
    if iteration_on_line:
        iteration = int(line)
        iteration_on_line = False;
    elif line[0]=="i":
        iteration_on_line = True;
    else:
        line_split = line.split(",")
        x.append(float(line_split[0]))
        y.append(float(line_split[1]))
        z.append(float(line_split[2]))
        radius.append(float(line_split[3]))

configfile = open("../../config.txt","r")
for line_individual in configfile:
    line = line_individual.replace(" ", "")
    line_split = line.split("=")
    if line_split[0] == "numSpheres":
        num_spheres = int(line_split[1])
    if line_split[0] == "numDensity":
        num_density = float(line_split[1])

volume_box = num_spheres/num_density
length_box = volume_box**(1/3)

# Visualize axis
x_axis = arrow(pos=vector(0,0,0), axis=vector(length_box,0,0), color=color.red)
y_axis = arrow(pos=vector(0,0,0), axis=vector(0,length_box,0), color=color.green)
z_axis = arrow(pos=vector(0,0,0), axis=vector(0,0,length_box), color=color.blue)

for i in range(len(x)):
    sphere(radius=radius[i], pos=vector(x[i],y[i],z[i]))
