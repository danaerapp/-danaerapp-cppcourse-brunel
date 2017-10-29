#include "Network.hpp"

Network::Network()
{
	
	//Initialisation des connections entre neurones
	//Si connection[i][j]=true --> le neurone i peut envoyer un potentiel post synaptique au neurone j (quand i spike)
	//Les voisins sont aléatoirement attribués, un neuron peut recevoir plusieurs signaux d'un seul neurone
	
	//Par défaut, aucun n'est connecté
	for (auto& ligne : connection){
		for (auto& element: ligne){
			element =false;
		}
	}
	
	/*std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> d(1, 10000); //10000 excitatory neurons
	
	for (int j(0); j<12500;++j){
		for (int NE(0); NE<1000;++NE){ //NE = Number of connection with excitatory neurons = 1000 for each neuron j
			int i(d(gen));
			
			while(i == j){
				i = d(gen);
			}//Cannot receive a signal from itself
			connection[i-1][j]=true; //--> means that neuron i can transmit a PSP to neuron j
		}
	}
	
	std::uniform_int_distribution<> dis(10001, 12500); //2500 inhibitory neurons
	
	for (int j(0); j<12500;++j){
		for (int NI(0); NI<250;++NI){ //NI = Number of connection with inhibitory neurons = 250 for each neuron j
			int i(dis(gen));
			
			while(i == j){
				i = dis(gen);
			}
			connection[i-1][j]=true;
		}
	}*/

}

Network::~Network(){}

std::vector<int> Network::getTargets(int n) const{
	
	std::vector<int> targets;
	
	for (int i(0); i<nb_neurons;++i){
		if (connection[n][i]){
			targets.push_back(i);
		}
	}
	
	return targets;
}
