#ifndef EXPERIMENT_HPP
#define EXPERIMENT_HPP

#include "Neurons.hpp"
#include <iostream>
#include <fstream>

class Experiment{
	public:
		
		int nb_steps;///< number of time steps, initialized to 0 in the constructor and recalculated in the method run.
		
		/**
		 * A function that creates an instance of the class Neurons and calls its method Neurons::update() for each time step.
		 * @param t, a double, correspond to the time of the simulation, used ot calculate the number of steps. nb_steps = t/Neurons::h
		 */
		void run(double t);
		
		Experiment();
		~Experiment(){}
		
};

#endif
