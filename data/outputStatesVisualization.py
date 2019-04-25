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

# The length of the box should be imported from the config file in the future.
length_box = 10.08

x_axis = arrow(pos=vector(0,0,0), axis=vector(length_box,0,0), color=color.red)
y_axis = arrow(pos=vector(0,0,0), axis=vector(0,length_box,0), color=color.green)
z_axis = arrow(pos=vector(0,0,0), axis=vector(0,0,length_box), color=color.blue)

for i in range(len(x)):
    sphere(radius=radius[i], pos=vector(x[i],y[i],z[i]))
