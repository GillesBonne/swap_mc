#!/usr/bin/python3

import matplotlib.pyplot as plt

r = []
g = []

bin_size_on_line = False
with open("../data/radialDistribution.txt","r") as fileRadial:
    for line in fileRadial:
        if bin_size_on_line:
            bin_size = float(line)
            bin_size_on_line = False
        elif line[0]=="b":
            bin_size_on_line = True;
        else:
            line_split = line.split(",")
            r.append(float(line_split[0]))
            g.append(float(line_split[1]))

fig = plt.figure()
plt.plot(r,g)
plt.xlabel('r')
plt.ylabel('g(r)')
fig.savefig("visuals/RDF.pdf", bbox_inches='tight')
plt.show()

k = []
s = []

bin_size_on_line = False
with open("../data/structureFactor.txt","r") as fileStructure:
    for line in fileStructure:
        if bin_size_on_line:
            bin_size = float(line)
            bin_size_on_line = False
        elif line[0]=="b":
            bin_size_on_line = True;
        else:
            line_split = line.split(",")
            k.append(float(line_split[0]))
            s.append(float(line_split[1]))

fig = plt.figure()
plt.plot(k,s)
plt.xlabel('k')
plt.ylabel('s(k)')
fig.savefig("visuals/SF.pdf", bbox_inches='tight')
plt.show()
