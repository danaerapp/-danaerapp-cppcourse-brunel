#ifndef NEURON_HPP
#define NEURON_HPP

#include <array>
#include <cmath>
#include <iostream>
#include <vector>
#include <random>

class Neuron{
	
	private:
	
	//Constantes de classe
		const double C = 1.0;
		const double tau = 20.0; //milliseconds, tau = R*C
		const double tauRef = 2.0; //milliseconds, refractory time
		const double Erepos = 0.0; //milliVolts
		const double Vreset = 0.0; //milliVolts, potentiel de repos
		const double exphtau = exp(-h/tau); //Constante dont on a besoin pour calculer le nouveau potentiel
		const int Dmax;
		
		
	//Attributs
		double potential;
		double spikesNumber;
		std::vector<double> times;
		
		int temps_pause; //For the refractory time after a spike, in nb of steps
		
		int clock_; //in nb of steps (not in seconds)
		
		std::vector<double> buffer; //Our ring buffer will contain maximum 15 values, since we are moving 0.1ms at a time ans the delay is of 1.5ms
		
	protected:
		double J;
	
	public:
	
	//Attributs de classe
		static constexpr double h = 0.1; //milliseconds, laps de temps entre chaque mesures de potentiel
		static constexpr double Vth = 20.0; //threshold, spike potential
		static constexpr double R = 20.0; //picoFarad, tau/C = R, resistance membranaire
		static constexpr double delay = 1.5; //in ms, time between a spike and the reception of this spike by a target neuron
		static constexpr double connection_coeff_excitatory = 10000;
		static constexpr double connection_coeff_inhibitory = 2500;
	
		double getPotential() const;
		double getSpikesNumber() const;
		double getTime(unsigned int i) const;
		double getJ() const;
		
		bool isRefractory() const;
		
		void setPotential(double i);
		void addSpike(); //Ajout d'un spike
		void addTime(double i);
		
		void update(double current);
		
		void receive(double J, double delay); // J :current weight, post synaptic potential amplitude 
		
		Neuron(double J);
		Neuron(double p, double s, std::vector<double> t, double J);
		
		~Neuron(){}
	
};

#endif
