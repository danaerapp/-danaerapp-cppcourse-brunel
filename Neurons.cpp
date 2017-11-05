#include "Neurons.hpp"

double Neurons::getPotential(int n) const{
	return potentials[n];
}

double Neurons::getSpikesNumber(int n) const{
	return spikesNumber[n];
}

double Neurons::getWeight(unsigned int n) const{
	return weights[n];
}

std::vector<unsigned int> Neurons::getTargets(unsigned int n) const{
	return targets[n];
}

std::vector<double> Neurons::getBuffer(unsigned int n) const{
	return buffer[n];
}

int Neurons::getTemps_Pause(unsigned int n) const{
	return temps_pause[n];
}

double Neurons::getTime(int n, unsigned int i) const{
	return times[n][i];
}

void Neurons::setTemps_Pause(unsigned int n, int t){
	temps_pause[n]=t;
}

void Neurons::setPotential(int n, double i){
	potentials[n] = i;
}

void Neurons::addSpike(int n){
	spikesNumber[n]+=1;
}

void Neurons::addTime(int n,unsigned int t){
	times[n].push_back(t);
}
				
void Neurons::update(){
	
	++clock_;
	
	///< Background noise
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::poisson_distribution<> d(nu_ext*h); ///< random external connection
	
	for (size_t n(0);n<nb_neurons;++n){
			assert(n<nb_neurons);
		
		if (temps_pause[n] > 0){ //Neuron is refractory 
			--temps_pause[n]; //h est notre pas de temps
			potentials[n]=Vreset;
			
		}else{
			potentials[n] = exphtau*potentials[n] + buffer[n][clock_%Dmax] + J_E*d(gen); //Temps actuel modulo delay max+1 nous donne le nb de steps avant de recevoir le spike
			
			if (potentials[n] >= Vth){ //Le neurone spike
				//addTime(n,clock_);
				//addSpike(n);
				times[n].push_back(clock_);
				spikesNumber[n]+=1;
				temps_pause[n]=(tauRef/h);
				
				assert(targets[n].size() >= 0 );
				for (unsigned int j(0); j<targets[n].size();++j){
					receive(targets[n][j], weights[n]); //Transmission du potentiel post synaptique, les targets reçoive un PSP et enregistre le temps de transmission, On prend J =0.1 ici
				}	
			}
		}
		
		buffer[n][clock_%Dmax] = 0.0 ; //To clear the buffer
		assert(buffer[n][clock_%Dmax] == 0.0);
	}
}

void Neurons::receive(int n, double J){
	buffer[n][(Dmax+clock_)%Dmax]+=J; //The neurone n receives the potential of an other neuron's spike, delay/H : transforms in nb of steps
}

Neurons::Neurons()
: potentials(nb_neurons, 0.0), spikesNumber(nb_neurons,0), temps_pause(nb_neurons, 0), clock_(0), Dmax(delay/h), targets(nb_neurons), times(nb_neurons), buffer(nb_neurons, std::vector<double> (Dmax,0.0))
{
	
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> d(0, N_E-1); //10000 excitatory neurons
	std::uniform_int_distribution<> dis(N_E, nb_neurons-1); //2500 inhibitory neurons
	
	assert(temps_pause.size() == nb_neurons);
	assert(spikesNumber.size() == nb_neurons);
	assert(potentials.size() == nb_neurons);
	assert(spikesNumber[0] == 0);
	assert(targets.size() == nb_neurons);
	assert(times.size() == nb_neurons);
	assert(buffer[0].size() == Dmax);
	assert(buffer[0][0]==0.0);
	assert(buffer.size() == nb_neurons);
	
	for (int n(0); n<nb_neurons;++n){
		
		for (unsigned int NE(0); NE<C_E;++NE){ //C_E = Number of connection with excitatory neurons = 1000 for each neuron n
			unsigned int i(d(gen));
			assert(i < nb_neurons);
			targets[i].push_back(n); // means that neuron i can transmit a spike to neuron n
		}
		
		for (unsigned int NI(0); NI<C_I;++NI){ //C_I = Number of connection with inhibitory neurons = 250 for each neuron n
			unsigned int i(dis(gen));
			targets[i].push_back(n);
		}
		if (n<N_E){
			weights.push_back(0.1);
		}else{
			weights.push_back(-0.1*g);
		}
	}
	assert(weights[0] == 0.1);
	assert(weights[N_E-1] == 0.1);
	assert(weights[N_E] == -0.1*g);
	assert(weights[N_E+N_I-1] == -0.1*g);
	
}

