# Swap Monte Carlo 

The simulation implements the swap Monte Carlo method introduced by Grigera and Parisi [1] for a mixture with various levels of polydispersity using a nonadditive soft repulsive pair potential [2].

## Getting started
The simulation is written in the C++ programming language.
The executable can be build by using the Makefile (by GitHub Gist user outro56) provided in the repository.

## Performing a simulation
Before performing a simulation the config.txt file needs to be adapted to use specific simulation parameters.
When running the simulation it is possible to provide some command line arguments:

* None.

* Number of iterations before sampling.

* Simulation identification.

* Simulation identification of a previous simulation, such that the simulation can continue from the state where the previous simulation has stopped.

## Interpreting the output
The simulation samples the energy, pressure and Monte Carlo acceptance ratios on a logarithmic scale (iteration number: 1,2,...,9,10,20,...,90,100,200,...).
The particle configurations are sampled every set number of iterations.
The code in /python/ is used to visualize all this saved information.

## Supplementary Mathematica code
In the folder /Mathematica/ additional Mathematica files are supplemented.
These calculate the maximum and minimum particle diameter for continuous and discretized polydisperse systems.
In addition, the calculation of the coefficients of the soft repulsive pair potential is added.

## References
[1]  T.S. Grigera and G. Parisi. Fast Monte Carlo algorithm for supercooled soft spheres. Physical Review E - Statistical Physics, Plasmas, Fluids, and Related Interdisciplinary Topics, 63(4):63–66, 2001.

[2] A. Ninarello, L. Berthier, and D. Coslovich. Models and algorithms for the next generation of glass transition studies. Physical Review X, 7(2):1–22, 2017.

## Visualization
Visualization of a simulation with a continuous polydisperse mixture of 1000 particles from a lattice configuration to an equilibrated configuration at a reduced temperature of 1 using the nonadditive soft repulsive pair potential.

![Alt Text](simulation_visual.gif)
