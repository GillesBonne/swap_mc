import subprocess
import numpy as np
import os
import matplotlib.pyplot as plt

import config

def read_acceptance():
    with open("data/translationAcceptance.txt","r") as file:
        translation_acceptance = []
        for line in file:
            translation_acceptance.append(float(line))
        for i in range(len(translation_acceptance)//2):
            del translation_acceptance[0]
        translation_acceptance = np.array(translation_acceptance)
        return np.mean(translation_acceptance)


def change_temperature_to(T):
    with open("config.txt","r") as config_file:
        with open("new_config.txt","w") as new_config_file:
            for line_individual in config_file:
                line = line_individual.replace(" ", "")
                line_split = line.split("=")
                if line_split[0] == "temperatureFixed":
                    new_config_file.write("temperatureFixed = "+str(T)+"\n")
                else:
                    new_config_file.write(line_individual)
            os.rename("new_config.txt","config.txt")

config.change_config_to(num_iterations=100001,
                    skip_samples=1000,
                    num_spheres=1000,
                    ratio_size_sphere=1.2,
                    num_density=1.0,
                    temperature_fixed = 0.25,
                    max_translation_distance_in_max_particle_size=0.15,
                    swap_probability=0.1)

num_temperatures = 5
num_averages = 2
translation_acceptances = np.zeros(num_temperatures)
temperatures = np.linspace(0.05,0.95,num_temperatures)

for i,T in enumerate(temperatures):
    print("temperature: "+str(T))
    acceptance = 0
    for iteration in range(num_averages):
        print('num times:'+str(iteration))
        change_temperature_to(T)
        subprocess.call(["bin/runner"])
        acceptance += read_acceptance()
    translation_acceptances[i] = acceptance/num_averages

fig = plt.figure()
plt.plot(temperatures,translation_acceptances)
plt.xlabel('Temperature')
plt.ylabel('Translation acceptance')
fig.savefig("python/visuals/temperature_acceptance.pdf", bbox_inches='tight')
plt.show()
