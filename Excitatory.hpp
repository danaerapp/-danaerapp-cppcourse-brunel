#ifndef EXCITATORY_HPP
#define EXCITATORY_HPP

#include "Neuron.hpp"

class Excitatory : public Neuron{	
	public:
		
		//Constructeur
		Excitatory();
		Excitatory(double p, double s, std::vector<double> t);
};

#endif
