import matplotlib.pyplot as plt

r = []
g = []

file = open("../../data/outputRadial.txt","r")
iteration_on_line = False
for line in file:
    if iteration_on_line:
        iteration = int(line)
        iteration_on_line = False;
    elif line[0]=="i":
        iteration_on_line = True;
    else:
        line_split = line.split(",")
        r.append(float(line_split[0]))
        g.append(float(line_split[1]))

plt.plot(r,g)
plt.show()
