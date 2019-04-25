from vpython import *

x = []
y = []
z = []
radius = []

file = open("outputStates.txt","r")
for line in file:
    line_split = line.split(",")
    x.append(float(line_split[0]))
    y.append(float(line_split[1]))
    z.append(float(line_split[2]))
    radius.append(float(line_split[3]))

# num_spheres and num_density should be imported from the config file in the future.
num_spheres = 1400
num_density = 1
volume_box = num_spheres/num_density
length_box = volume_box**(1/3)


x_axis = arrow(pos=vector(0,0,0), axis=vector(length_box,0,0), color=color.red)
y_axis = arrow(pos=vector(0,0,0), axis=vector(0,length_box,0), color=color.green)
z_axis = arrow(pos=vector(0,0,0), axis=vector(0,0,length_box), color=color.blue)

for i in range(len(x)):
    sphere(radius=radius[i], pos=vector(x[i],y[i],z[i]))
