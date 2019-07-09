#!/usr/bin/python3

import sys

if(len(sys.argv)==1):
    id = ""
elif(len(sys.argv)==2):
    id = sys.argv[1]

config_path = "data/data" + id + "/lastConfig.txt"

with open(config_path,"r") as config_file:
    for line_individual in config_file:
        line = line_individual.replace(" ", "")
        line_split = line.split("=")
        if line_split[0] == "numSpheres":
            num_spheres = int(line_split[1])

skip = 10

input_path = "data/data" + id + "/outputStates10000.txt"
output_path = "visuals/visuals" + id + "/ovitoStates.txt"

iteration_on_line = False
with open(input_path,"r") as in_file:
    with open(output_path,"w") as out_file:
        count = 0
        for line in in_file:
            if iteration_on_line:
                iteration_on_line = False
                out_file.write(line)
            elif line[0]=="i":
                if count%skip==0:
                    iteration_on_line = True
                    out_file.write(str(num_spheres))
                    out_file.write("\n")
                count += 1
            else:
                if (count-1)%skip==0:
                    line_split = line.split(",")
                    x = float(line_split[0])
                    y = float(line_split[1])
                    z = float(line_split[2])
                    rad = float(line_split[3])
                    out_file.write(str(x) + " " + str(y) + " " + str(z) + " " + str(rad) + "\n")
