#include "googletest/include/gtest/gtest.h"
#include <cmath>
#include "Neurons.hpp"
#include "Experiment.hpp"

TEST (NeuronTest, OneSpike){
	Neurons neurones;
	neurones.update();
	
	for (int i(0); i < Neurons::nb_neurons; ++i){
		if (neurones.getPotential(i) >= Neurons::Vth){
			EXPECT_EQ(neurones.getSpikesNumber(i), 1);
			EXPECT_EQ(neurones.getTime(i, 0), 1);
		}
	}
}

TEST (NeuronTest, BufferEmptied){
	Neurons neurones;
	for (int t(1);t<=Neurons::delay/Neurons::h;++t){
		neurones.update();

		for (int n(0); n < Neurons::nb_neurons; ++n){
				EXPECT_EQ(neurones.getBuffer(n)[t%15] , 0.0);
		}
	}
}

TEST (NeuronTest, TransmissionToBuffer){ ///< Look at spike transmission after one update by looking at the buffer
	Neurons neurones;
	neurones.update();
	
	std::vector<std::vector<double>> buff;
	int Dmax(Neurons::delay/Neurons::h);
	
	for (unsigned int n(0); n < Neurons::nb_neurons; ++n){
		buff.push_back(std::vector<double> (Dmax,0.0));
		
		if (neurones.getPotential(n) > Neurons::Vth){
			for (unsigned int i(0); i<neurones.getTargets(n).size();++i){
				if (n<Neurons::N_E){
					EXPECT_EQ(neurones.getWeight(n),0.1);
					buff[neurones.getTargets(n)[i]][(Dmax+1)%Dmax]+=neurones.getWeight(n);			
				}else{
					EXPECT_EQ(neurones.getWeight(n),-0.1*Neurons::g);
					buff[neurones.getTargets(n)[i]][(Dmax+1)%Dmax]+=neurones.getWeight(n);
				}
			}
		}
	}
	
	for (int n(0); n < Neurons::nb_neurons; ++n){
		EXPECT_EQ(buff[n],neurones.getBuffer(n));
	}
}

TEST (NeuronTest, ExperimentNbSteps){
	Experiment ex;
	ex.run(0.1);
	EXPECT_EQ(ex.nb_steps,1);
	
	ex.run(1.0);
	EXPECT_EQ(ex.nb_steps,10);
}

TEST (NeuronTest, NeuronsSetters){
	Neurons neurones;
	for (int n(0); n < Neurons::nb_neurons; ++n){
		neurones.addTime(n, 1);
		neurones.addSpike(n);
		neurones.setPotential(n,10);
		
		EXPECT_EQ(neurones.getSpikesNumber(n),1);
		EXPECT_EQ(neurones.getTime(n,0),1);
		EXPECT_EQ(neurones.getPotential(n), 10);
	}
}

TEST (NeuronTest, RefractoryPeriod){
	Neurons neurones;
	
	for (int n(0); n < Neurons::nb_neurons; ++n){
		neurones.setPotential(n,Neurons::Vth);
		neurones.setTemps_Pause(n, 20); ///< tauRef = 2.0 ms
	}
	
	for (int t(1);t<=20;++t){
		neurones.update();
		for (int n(0); n < Neurons::nb_neurons; ++n){
			EXPECT_EQ(neurones.getTemps_Pause(n),20-t);
		}
	}
	
	for (int n(0); n < Neurons::nb_neurons; ++n){
		EXPECT_EQ(neurones.getPotential(n),0);
		EXPECT_EQ(neurones.getTemps_Pause(n),0);
	}
}

