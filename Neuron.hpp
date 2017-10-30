#ifndef NEURON_HPP
#define NEURON_HPP

#include <array>
#include <cmath>
#include <iostream>
#include <vector>
#include <random>
#include "Network.hpp"

class Neuron{
	
	private:
	
		static constexpr double C = 1.0;
		static constexpr double tau = 20.0; ///< tau = R*C, in ms.
		static constexpr double tauRef = 2.0; ///< Refractory time, in ms.
		static constexpr double Erepos = 0.0; ///< In mV.
		static constexpr double Vreset = 0.0; ///< Reset potential, in mV.
		const int Dmax; ///< Dmax = delay/h, Maximum delay, useful for the delay buffer, in nb of steps.
		const double exphtau = exp(-h/tau); ///< Constant used in update, to calculate the potential.
		static constexpr double eta = 2.0;
		const double nu_ext = (eta*Vth)/(J_E*tau);///< nu_ext=eta*C_E*nu_thr, where nu_thr=Vthr/(J_E*tau*C_E)
		
		
		double potential; ///< Membrane potential of the neuron at time correspondign to its local clock_.
		double spikesNumber; ///< Stores the number of spikes that have occured until "now" according to the local clock_.
		std::vector<double> times; ///< Storage of each time when a spike has occured, in nb of steps (not in seconds).
		
		int temps_pause; ///< For the refractory time after a spike (time during which the neuron is no more receptive to external current), in nb of steps.
		
		int clock_; ///< In nb of steps (not in seconds).
		
		std::vector<double> buffer; ///< Our ring buffer will contain maximum 15 values, since we are moving 0.1ms at a time ans the delay is of 1.5ms.
		
	protected:
		double J; ///< Depending if the neuron is excitatory or inhibotry, J will change. 
				  ///< Useful for the receive function, correspond to the post synaptic current.
				  
		static constexpr double J_E = 0.1; ///< Useful to generate random connections, in mV.
		static constexpr double J_I = 0.5; 
	
	public:
	
	/**
	 *  Class Attributes
	 */
		static constexpr double h = 0.1; ///< Our time step, in milliseconds.
		static constexpr double Vth = 20.0; ///< Threshold potential, spike potential, in millivolts.
		static constexpr double R = 20.0; ///< Membrane resistance, in picoFarad, tau/C = R.
		static constexpr double delay = 1.5; ///< Time between a spike and the reception of this spike by a target neuron, in ms.
	
		/**
		 * a getter returning the membrane potential
		 */
		double getPotential() const;
		
		/**
		 * a getter returning the number of spikes that have occured
		 */
		double getSpikesNumber() const;
		
		/**
		 * a function taking an integer and returning the time at which a spike occured
		 * @param i an integer, corresponds to the cell in the vector containing all spikes times
		 * @return The ith spike time, in nb of steps (not in seconds)
		 */
		double getTime(unsigned int i) const;
		
		/**
		 * a getter returning the post synaptic potential
		 */
		double getJ() const;
		
		/**
		 * a function returning true if the neuron is refractory, false if not
		 * @return A bool corresponding to its refractory state
		 */
		bool isRefractory() const;
		
		/**
		 * a setter, to attribute a certain value to the membrane potential
		 * @param i a double
		 */
		void setPotential(double i);
		
		/**
		 * a function that increment the number of spikes by one
		 * @see spikesNumber
		 */
		void addSpike();
		
		/**
		 * a function that adds a new spike time in the spike time vector
		 * @param t a double, the time we want to store
		 * @see times
		 */
		void addTime(double t);
		
		/**
		 * 
		 * 
		 * 
		 * 
		 * 
		 * 
		 */
		void update();
		
		/**
		 * 
		 * 
		 * 
		 * 
		 * 
		 */
		void receive(double J); // J :current weight, post synaptic potential amplitude 
		
		/**
		 * a Constructor
		 * 
		 * 
		 * 
		 */
		Neuron(double J);
		
		/**
		 * a Constructor
		 * 
		 * 
		 * 
		 */
		Neuron(double p, double s, std::vector<double> t, double J);
		
		/**
		 * a Destructor
		 */
		~Neuron(){}
	
};

#endif
