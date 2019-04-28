import matplotlib.pyplot as plt

r = []
g = []

file = open("outputRadial.txt","r")
for line in file:
    line_split = line.split(",")
    r.append(float(line_split[0]))
    g.append(float(line_split[1]))

plt.plot(r,g)
plt.show()
