#include <iostream>
#include <fstream>
#include "Neuron.hpp"
#include "Network.hpp"
#include "Excitatory.hpp"
#include "Inhibitory.hpp"


using namespace std;

//Système avec 2 neurones : le 2ème neurone réagit au potentiel post-synaptique du premier
//On écrit dans un fichier les valeurs du potentiel du 2ème neurone

int main(){
	
	//We say t_start=0
	double t_stop(0.0);
	cout << "Determine a time to stop :";
	cin >> t_stop;
	cout << endl;
	
	double ExtCurrent(0.0);
		
	ofstream sortie("Data.txt");
	
	Network network;
	
	int nb_excitatory(10000);
	int nb_inhibitory(2500);
	
	vector<Neuron> all;
	
	for (int i(0);i<nb_excitatory;++i){ //On initialise des neurones
		Excitatory n;
		all.push_back(n);
	}
	
	for (int i(0);i<nb_inhibitory;++i){ //On initialise des neurones
		Inhibitory n;
		all.push_back(n);
	}
	
	int nb_steps(t_stop/Neuron::h); //0.1ms est notre pas de temps
	
	/*for (int t(0); t < nb_steps; ++t){
		if (sortie.fail()){
			cerr<< "Erreur d'ouverture du fichier" << endl;
		}else{
			
			for (size_t i(0);i<all.size();++i){
					
					if ( (all[i].getPotential() >= Neuron::Vth) and not(all[i].isRefractory())){//Si le neurone spike et qu'il n'est pas en période réfractaire --> il envoi un signal à ses "voisins"
						
						vector<int> targets(network.getTargets(i));//Vector contenant les neurones pouvant recevoir le signal
						
						for (size_t j(0); j<targets.size();++j){
							all[targets[j]].receive(all[i].getJ(), Neuron::delay); //Transmission du potentiel post synaptique, les targets reçoive un PSP et enregistre le temps de transmission, On prend J =0.1 ici
						}
						
						all[i].update(ExtCurrent);
						
					}else{
							all[i].update(ExtCurrent);
					}
			}
		}
	}*/
	
	return 0;
}
