#ifndef NETWORK_HPP
#define NETWORK_HPP

#include <array>
#include <vector>
#include <random>

class Network{
	private:
	
		static constexpr int nb_neurons = 12500; //Les 10000 premiers sont excitatory, les 2500 suivants sont inhibitory
		
		std::array<std::array<bool,nb_neurons>, nb_neurons> connection; //Matrice carrée, si connection[i][j]=true --> i est connecté à j (et non l'inverse!)
		
	public:
		
		std::vector<int> getTargets(int n) const;
		
		bool isExcitatory(int i) const;
		
		~Network();
		Network();
};

#endif
