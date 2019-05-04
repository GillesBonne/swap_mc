import matplotlib.pyplot as plt

r = []
g = []

bin_size_on_line = False
with open("../data/radialDistribution.txt","r") as file:
    for line in file:
        if bin_size_on_line:
            bin_size = float(line)
            bin_size_on_line = False
        elif line[0]=="b":
            bin_size_on_line = True;
        else:
            line_split = line.split(",")
            r.append(float(line_split[0]))
            g.append(float(line_split[1]))

plt.plot(r,g)
plt.show()
