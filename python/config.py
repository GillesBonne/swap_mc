#!/usr/bin/python3

import os

def change_config_to(num_iterations,
                     skip_samples,
                     num_spheres,
                     ratio_size_sphere,
                     num_density,
                     temperature_fixed,
                     max_translation_distance_in_max_particle_size,
                     swap_probability):
    with open("config.txt","r") as config_file:
        with open("new_config.txt","w") as new_config_file:
            for line_individual in config_file:
                line = line_individual.replace(" ", "")
                line_split = line.split("=")
                if line_split[0] == "numIterations":
                    new_config_file.write("numIterations = "+str(num_iterations)+"\n")
                elif line_split[0] == "skipSamples":
                    new_config_file.write("skipSamples = "+str(skip_samples)+"\n")
                elif line_split[0] == "numSpheres":
                    new_config_file.write("numSpheres = "+str(num_spheres)+"\n")
                elif line_split[0] == "ratioSizeSphere":
                    new_config_file.write("ratioSizeSphere = "+str(ratio_size_sphere)+"\n")
                elif line_split[0] == "numDensity":
                    new_config_file.write("numDensity = "+str(num_density)+"\n")
                elif line_split[0] == "temperatureFixed":
                    new_config_file.write("temperatureFixed = "+str(temperature_fixed)+"\n")
                elif line_split[0] == "maxTranslationDistanceInMaxParticleSize":
                    new_config_file.write("maxTranslationDistanceInMaxParticleSize = "+str(max_translation_distance_in_max_particle_size)+"\n")
                elif line_split[0] == "swapProbability":
                    new_config_file.write("swapProbability = "+str(swap_probability)+"\n")
                else:
                    new_config_file.write(line_individual)
            os.rename("new_config.txt","config.txt")
