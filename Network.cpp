#include "Network.hpp"

Network::Network()
{
	
	/**
	 * Initialisation of connections between neurons
	 * If connection[n][i] = j--> le neurone n peut envoyer un potentiel post synaptique au neurone j (quand i spike)
	 * We consider that a neuron has as many targets as connections coming from other neurons
	 * Targets are randomly attributed
	 * A neuron can receive many connection from one neuron
	 * One neuron can be one of its targets, can be connected to itself
	 * For neuron n: connection[n][i] 0=<i<=9999 reppresent the excitatory connections, connection[n][i] 10000<=i<=12499 reppresent the inhibitory connections.
	 */
	
	//assert(connection.size() == nb_neurons);
	//assert(connection[0].size() == (C_I+C_E));
	
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> d(0, N_E-1); //10000 excitatory neurons
	
	for (int n(0); n<nb_neurons;++n){
		connection.push_back(std::vector<unsigned int> (C_E+C_I,0));
		for (int NE(0); NE<C_E;++NE){ //NE = Number of connection with excitatory neurons = 1000 for each neuron j
			unsigned int i(d(gen));
			connection[n][NE]=i; //--> means that neuron n can transmit a PSP to neuron i
		}
	}
	
	std::uniform_int_distribution<> dis(N_E, nb_neurons-1); //2500 inhibitory neurons
	
	for (int n(0); n<nb_neurons;++n){
		connection.push_back(std::vector<unsigned int> (C_E+C_I,0));
		for (int NI(0); NI<C_I;++NI){ //NI = Number of connection with inhibitory neurons = 250 for each neuron j
			unsigned int i(dis(gen));
			
			connection[n][NI]=i;
		}
	}

}

Network::~Network(){}

std::vector<unsigned int> Network::getTargets(int n) const{
	
	std::vector<unsigned int> targets(C_I+C_E,0);
	
	for (size_t j(0);j< connection[n].size() ; ++j){
		targets[j]=connection[n][j];
	}
	
	return targets;
}
