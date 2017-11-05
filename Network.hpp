/*#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <array>
#include <vector>
#include <random>
#include <cassert>
#include "Neuron.hpp"

class Network{
	
	public:
	
		static constexpr int nb_neurons = 12500; //Les 10000 premiers sont excitatory, les 2500 suivants sont inhibitory
		
		static constexpr int N_E = 0.8*nb_neurons;///< number of excitatory neurons
		static constexpr int N_I = 0.2*nb_neurons; ///< number of inhibitory neurons
		
		static constexpr unsigned int C_E = 0.1*Network::N_E; ///<
		static constexpr unsigned int C_I = 0.1*Network::N_I;
		
		std::vector<unsigned int> getTargets(int n) const;
		
		~Network();
		Network();
	
	private:
		
		std::vector<std::vector<unsigned int>> connection; ///< connection[n][j]=i means that neuron n can transmit a PSP to neuron i, size : nb_neuron x (C_I+C_E)
};

#endif
*/
