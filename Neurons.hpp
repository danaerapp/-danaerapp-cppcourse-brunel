#ifndef NEURON_HPP
#define NEURON_HPP

#include <array>
#include <cmath>
#include <iostream>
#include <vector>
#include <random>
#include <cassert>
#include "googletest/include/gtest/gtest_prod.h"

/**
 * The Neuron class simulates a network of many neurons (here 12500) and their interactions, following the paper of Nicolas Brunel :"Dynamics of Sparsely Connected Networks of Excitatory and Inhibitory Spiking Neurons". 
 * Which means we consider an integrate and fire network of inhibitory and excitatory neurons.
 */

class Neurons{
	
	private:
		
		static constexpr unsigned int C_E = 1000;///< 0.1% of N_E.
		static constexpr unsigned int C_I = 250; ///< 0.1% of N_I.
		static constexpr double J_E = 0.1; ///< Useful to generate random connections, in mV.
		static constexpr double C = 1.0; ///< capacity of the membrane, in pF.
		static constexpr double tau = 20.0; ///< tau = R*C, in ms.
		static constexpr double tauRef = 2.0; ///< Refractory time (= time during which a neuron is not sensible to external simulation), in ms.
		static constexpr double Erepos = 0.0; ///< In mV.
		static constexpr double Vreset = 0.0; ///< Reset potential, in mV.
		static constexpr double eta = 0.9; ///< a constant that is equal to nu_ext/nu_thr
		
		const int Dmax; ///< Dmax = delay/h, Maximum delay, useful for the delay buffer, in nb of steps.
		const double exphtau = exp(-(h/tau)); ///< Constant used in update, to calculate the potential.
		const double nu_ext = (eta*Vth)/(J_E*tau);///< nu_ext=eta*C_E*nu_thr, where nu_thr=Vthr/(J_E*tau*C_E)
		
		int clock_; ///< Clock of all neurons, in nb of steps (not in seconds).
		
		std::vector<double> potentials; ///< Membrane potential of the neuron at time correspondign to its local clock_.
		std::vector<double> weights; ///< Stores the post synaptic weight of every neuron, for the first 10000 : J_E, for the next 2500 : J_I
		std::vector<int> spikesNumber; ///< Stores the number of spikes that have occured until "now" according to the local clock_.
		std::vector<int> temps_pause; ///< For the refractory time after a spike (time during which the neuron is no more receptive to external current), in nb of steps.
		
		std::vector<std::vector<unsigned int >> times; ///< Storage of each time when a spike has occured, in nb of steps (not in seconds).
		std::vector<std::vector<double>> buffer; ///< Our ring buffer will contain maximum 15 values, since we are moving 0.1ms at a time and the delay is of 1.5ms.
		std::vector<std::vector<unsigned int>> targets; ///< targets[n][j]=i means that neuron n can transmit a PSP to neuron i
		
		/**
		 * Creation of friendship between the class and some of the unit tests (in UnitTest1.cpp)
		 * It allows these tests to have access to all attributes of the class and therefore not to create a multitude of getters and setters.
		 */
		FRIEND_TEST(NeuronTest, TransmissionToBuffer);
		FRIEND_TEST(NeuronTest, RefractoryPeriod);
		FRIEND_TEST(NeuronTest, BufferEmptied);
		
	public:
	
		/**
		 *  Class Attributes
		 */
		static constexpr int nb_neurons = 12500; ///< The first 10000 are excitatory, the 2500 following are inhibitory
		static constexpr int N_E = 10000;///< Number of excitatory neurons
		static constexpr int N_I = 2500; ///< Number of inhibitory neurons	
		static constexpr double h = 0.1; ///< Our time step, in milliseconds.
		static constexpr double Vth = 20.0; ///< Threshold potential, spike potential, in millivolts.
		static constexpr double R = 20.0; ///< Membrane resistance, in picoFarad, tau/C = R.
		static constexpr double delay = 1.5; ///< Time between a spike and the reception of this spike by a target neuron, in ms.
		static constexpr double g = 4.5; ///< g=J_I/J_E, where J_I and J_E are the current weights for inhibitory and excitatory neurons respectively.
	
		/**
		 * A getter returning the membrane potential for neuron n
		 */
		double getPotential(int n) const;
		
		/**
		 * A getter returning the number of spikes that have occured until now (according to the clock_) for neuron n
		 */
		double getSpikesNumber(int n) const;
		
		/**
		 * a function taking two integers and returning the time at which a spike occured for neuron n.
		 * @param n an integers, corresponds to the index of the neuron of interest.
		 * @param i an integer, corresponds to the cell in the vector containing all spike times of neuron n.
		 * @return The ith spike time of neuron n, in nb of steps (not in seconds)
		 */
		double getTime(int n, unsigned int i) const;
		
		/**
		 * A setter, to attribute a certain value to the membrane potential of neuron n
		 * @param n an integer, corresponds to the index of the neuron of interest.
		 * @param i a double, corresponds to the new value we want to attribute to the potential of neuron n.
		 */
		void setPotential(int n, double i);
		
		/**
		 * A function that increment the number of spikes of neuron n by one (++spikesNumber[n])
		 * @see spikesNumber
		 * @param n an integer, corresponds to the neuron of interest.
		 */
		void addSpike(int n);
		
		/**
		 * A function that adds a new spike time in the spike time vector of neuron n (times[n].push_bakc(t)).
		 * @param n an integer, corresponds to the index of the neuron of interest.
		 * @param t a double, the time we want to store.
		 * @see times
		 */
		void addTime(int n, unsigned int t);
		
		/**
		 * An function that updates the membrane of each neurons for one step of time (i.e. h = 0.1 ms).
		 * It updates the membrane potential as such : 
		 * 
		 * potentials[n] = exphtau*potentials[n] + buffer[n][clock_%Dmax] + J_E*d(gen);
		 * 
		 * Where :
		 * 
		 * potentials[n] corresponds to the membrane potential of neuron n.
		 * 
		 * buffer[n][clock_%Dmax] corresponds to the incoming spikes of neuron n. It takes in consideration the delay between the transmission and the reception of a post synaptic current as well as the weight of this current.
		 * 
		 * J_E*d(gen) simulates a background noise, using the poisson distribution with a firing rate of nu_ext*h.
		 * @see exphtau, a variable, attributes of the class.
		 * 
		 * 
		 * It takes in consideration the refractory period during which a neuron is insensible to the reception of incoming spikes.
		 * @see temps_pause
		 * 
		 * If the new potential is higher than the threshold potential (Vth), then the neuron transmits a post synaptic potential to other neurons.
		 * The targets of a neuron is randomly created in the constructor.
		 * To take the delay in consideration we store the weight of the connection in a buffer (using the receive method).
		 * @see targets
		 * @see buffer
		 * @see receive
		 */
		void update();
		
		/**
		 * A function that transmit a post synpatic potential to neuron n.
		 * @param n an integer, correspond to the neuron reveiving a spike.
		 * @param J a double, corresponds to the current weight of the neuron transmitting the spike.
		 * @see update
		 * @see buffer
		 * @see weights
		 */
		void receive(int n,double J);
		
		/**
		 * A Constructor
		 * It initializes the potentials, the spikesNumber and the temps_pause of each neuron to 0.
		 * 
		 * It initializes the global clock to 0.
		 * 
		 * It also initializes the connections between neurons. 
		 * 	Each neuron can receive incoming spikes randomly chosen from 10% of all excitatory neurons and 10% of all inhibitory neurons.
		 * 	To create a randomness we use the poisson distribution.
		 * 
		 * It also initializes the post synaptic weight of each neuron (0.1mV for excitatory neurons and -g*0.1mV for inhibitory neurons).
		 */
		Neurons();

		/**
		 * A Destructor
		 */
		~Neurons(){}
	
};

#endif
