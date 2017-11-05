#ifndef EXPERIMENT_HPP
#define EXPERIMENT_HPP

#include "Neurons.hpp"
#include <iostream>
#include <fstream>

class Experiment{
	public:
		
		void run(double t);
		
		int nb_steps;
		
		Experiment();
		~Experiment(){}
		
};

#endif
