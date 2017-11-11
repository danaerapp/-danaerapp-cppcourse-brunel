#include "Experiment.hpp"

void Experiment::run(double t){
	
	nb_steps = t/Neurons::h;
	
	Neurons n;
	std::ofstream data("Data.txt");
	
	for (int t(0); t < nb_steps; ++t){
		n.update();
		
			if (data.fail()){
						std::cerr<< "Erreur d'ouverture du fichier" << std::endl;
					}else{
						for (unsigned int i(0);i<Neurons::nb_neurons;++i){
							assert(i<Neurons::nb_neurons);
							assert(n.getSpikesNumber(i) >= 0);
							
							if ( (n.getSpikesNumber(i) > 0) and (n.getTime(i, n.getSpikesNumber(i)-1) == t)){
								assert(n.getSpikesNumber(i) > 0);
								data << t << '\t' << i << '\n';
							}
						}
					}
	}
}

Experiment::Experiment()
:nb_steps(0)
{}
